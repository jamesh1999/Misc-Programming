#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <vector>
#include <string>

struct TGAHeader
{
	char d1[12];
	short width;
	short height;
	char bpp;
	char d2;
};

class RenderPass
{
public:
	ID3D11VertexShader* vs = NULL;
	ID3D11PixelShader* ps = NULL;
	ID3D11InputLayout* layout = NULL;

	void LoadVS(std::wstring filename, std::string entry, D3D11_INPUT_ELEMENT_DESC* inputDesc, int numElements);
	void LoadPS(std::wstring filename, std::string entry);
};

class Material
{
	ID3D11Texture2D* m_tex = NULL;

	void setTextureData(uint8_t*, int, int);

public:
	std::vector<RenderPass> passes;

	ID3D11ShaderResourceView* m_texView = NULL;
	ID3D11SamplerState* m_samplerState = NULL;

	Material() = default;
	~Material();

	Material(const Material&) = delete;
	Material(Material&&) = delete;
	Material& operator=(const Material&) = delete;
	Material& operator=(Material&&) = delete;

	ID3D11ShaderResourceView* GetTexture();
	ID3D11SamplerState* GetSampler();
	void LoadTGA(std::string);
	void SetTexture(DirectX::XMFLOAT3*, int, int);
};

#endif
