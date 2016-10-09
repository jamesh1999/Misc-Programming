#include "Material.h"
#include <fstream>
#include <cmath>
#include "GraphicsController.h"
#include <Windows.h>

void RenderPass::LoadVS(std::wstring filename, std::string entry, D3D11_INPUT_ELEMENT_DESC* inputDesc, int numElements)
{
	ID3D10Blob* buff;
	D3DCompileFromFile(
		filename.c_str(),
		NULL,
		NULL,
		entry.c_str(),
		"vs_5_0",
		D3DCOMPILE_DEBUG,
		NULL,
		&buff,
		NULL);

	//Create shader and input layout
	GraphicsController::instance->device->CreateVertexShader(
		buff->GetBufferPointer(), buff->GetBufferSize(), NULL, &vs);
	GraphicsController::instance->device->CreateInputLayout(
		inputDesc, numElements, buff->GetBufferPointer(), buff->GetBufferSize(), &layout);
}

void RenderPass::LoadPS(std::wstring filename, std::string entry)
{
	ID3D10Blob* buff;
	D3DCompileFromFile(
		filename.c_str(),
		NULL,
		NULL,
		entry.c_str(),
		"ps_5_0",
		D3DCOMPILE_DEBUG,
		NULL,
		&buff,
		NULL);

	//Create shader
	GraphicsController::instance->device->CreatePixelShader(
		buff->GetBufferPointer(), buff->GetBufferSize(), NULL, &ps);
}

Material::~Material()
{
	for(int i = 0; i < passes.size(); ++i)
	{
		passes[i].vs->Release();
		passes[i].ps->Release();
		passes[i].layout->Release();
	}

	passes.clear();

	m_tex->Release();
	m_texView->Release();
	m_samplerState->Release();
}

ID3D11ShaderResourceView* Material::GetTexture()
{
	return m_texView;
}

ID3D11SamplerState* Material::GetSampler()
{
	return m_samplerState;
}

void Material::LoadTGA(std::string filename)
{
	std::ifstream in(filename, std::ios::binary);
	
	TGAHeader h;
	in.read(reinterpret_cast<char*>(&h), sizeof(TGAHeader));

	uint8_t* tgadata = new uint8_t[h.width * h.height * 4];
	in.read(reinterpret_cast<char*>(tgadata), h.width * h.height * 4);
	in.close();

	uint8_t* textureData = new uint8_t[h.width * h.height * 4];
	for(int i = 0; i < h.height; ++i)
	{
		for(int j = 0; j < h.width; ++j)
		{
			textureData[(i * h.width + j) * 4]     = tgadata[((h.height - i - 1) * h.width + j) * 4 + 2];
			textureData[(i * h.width + j) * 4 + 1] = tgadata[((h.height - i - 1) * h.width + j) * 4 + 1];
			textureData[(i * h.width + j) * 4 + 2] = tgadata[((h.height - i - 1) * h.width + j) * 4];
			textureData[(i * h.width + j) * 4 + 3] = tgadata[((h.height - i - 1) * h.width + j) * 4 + 3];
		}
	}
	delete[] tgadata;

	setTextureData(textureData, h.width, h.height);
	delete[] textureData;
}

void Material::SetTexture(DirectX::XMFLOAT3* data, int x, int y)
{
	uint8_t* textureData = new uint8_t[x * y * 4];

	for(int i = 0; i < y; ++i)
		for(int j = 0; j < x; ++j)
		{
			textureData[(i * x + j) * 4]     = std::round(data[i * x + j].x * 255);
			textureData[(i * x + j) * 4 + 1] = std::round(data[i * x + j].y * 255);
			textureData[(i * x + j) * 4 + 2] = std::round(data[i * x + j].z * 255);
			textureData[(i * x + j) * 4 + 3] = 1.0f;
		}

	setTextureData(textureData, x, y);

	delete[] textureData;
}


void Material::setTextureData(uint8_t* data, int x, int y)
{
	D3D11_TEXTURE2D_DESC tD;
	ZeroMemory(&tD, sizeof(D3D11_TEXTURE2D_DESC));
	tD.Usage = D3D11_USAGE_DEFAULT;
	tD.ArraySize = 1;
	tD.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	tD.Height = y;
	tD.Width = x;
	tD.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
	tD.MipLevels = 0;
	tD.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	tD.SampleDesc.Count = 1;
	tD.SampleDesc.Quality = 0;

	GraphicsController::instance->device->CreateTexture2D(&tD, NULL, &m_tex);
	GraphicsController::instance->devContext->UpdateSubresource(m_tex, 0, NULL, data, x * 4, 0);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvD;
	srvD.Format = tD.Format;
	srvD.ViewDimension = D3D10_1_SRV_DIMENSION_TEXTURE2D;
	srvD.Texture2D.MipLevels = -1;
	srvD.Texture2D.MostDetailedMip = 0;
	GraphicsController::instance->device->CreateShaderResourceView(m_tex, &srvD, &m_texView);
	GraphicsController::instance->devContext->GenerateMips(m_texView);

	D3D11_SAMPLER_DESC sD;
	sD.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sD.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sD.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sD.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sD.MipLODBias = 0.0f;
	sD.MaxAnisotropy = 1;
	sD.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	sD.BorderColor[0] = 0;
	sD.BorderColor[1] = 0;
	sD.BorderColor[2] = 0;
	sD.BorderColor[3] = 0;
	sD.MinLOD = 0;
	sD.MaxLOD = D3D11_FLOAT32_MAX;

	GraphicsController::instance->device->CreateSamplerState(&sD, &m_samplerState);
}
