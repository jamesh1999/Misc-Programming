#include <DirectXMath.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include <cmath>
#include <string>
#include <sstream>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

IDXGISwapChain* swpChain;
ID3D11Device* device;
ID3D11DeviceContext* deviceContext;
ID3D11RenderTargetView* pBackBuffer;

ID3D11VertexShader* vertexShader;
ID3D11PixelShader* pixelShader;

ID3D11InputLayout* inputLayout;

ID3D11Buffer* vertexBuffer;
ID3D11Buffer* indexBuffer;
ID3D11Buffer* constantBuffer;

ID3D11ShaderResourceView* shaderTex;

struct Vertex
{
	float x;
	float y;
	float tex_u;
	float tex_v;
};

struct Rect
{
	double left;
	double top;
	double right;
	double bottom;
};
Rect mandlebrotRect { -3.0f, 2.5f, 2.0f, -2.5f };

Vertex vbData[4]
{
	{ -1.0f, -1.0f },
	{ -1.0f, 1.0f },
	{ 1.0f, 1.0f },
	{ 1.0f, -1.0f},
};

struct CBuffer
{
	int sf;
	int p1, p2, p3;
};

double move_speed = 0.5;
double zoom_speed = 0.5;

bool keyStates[6] { false };

void InitD3D(HWND hWnd)
{
	//Create device/swap chain
	DXGI_SWAP_CHAIN_DESC swpDesc;
	ZeroMemory(&swpDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swpDesc.BufferCount = 1;
	swpDesc.BufferDesc.Height = 800;
	swpDesc.BufferDesc.Width = 800;
	swpDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swpDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swpDesc.OutputWindow = hWnd;
	swpDesc.SampleDesc.Count = 4;
	swpDesc.SampleDesc.Quality = 0;
	swpDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swpDesc.Windowed = TRUE;

	D3D_FEATURE_LEVEL featureLevel;
	D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&swpDesc,
		&swpChain,
		&device,
		&featureLevel,
		&deviceContext
	);

	if(featureLevel < D3D_FEATURE_LEVEL_11_0)
	{
		std::cout << "DirectX 11 is unsupported" << std::endl;
		exit(-1);
	}

	//Set render target
	ID3D11Texture2D* backBuffer;
	swpChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
	device->CreateRenderTargetView(backBuffer, NULL, &pBackBuffer);
	deviceContext->OMSetRenderTargets(1, &pBackBuffer, NULL);
	backBuffer->Release();

	//Set viewport
	D3D11_VIEWPORT vp;
	vp.Width = 800;
	vp.Height = 800;
	vp.MaxDepth = 1.0f;
	vp.MinDepth = 0.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	deviceContext->RSSetViewports(1, &vp);

	//Load shaders
	ID3D10Blob *vtx, *pxl;
	ID3D10Blob* err;
	D3DCompileFromFile(L"shaders.hlsl", NULL, NULL, "VSMain", "vs_5_0", D3DCOMPILE_DEBUG, NULL, &vtx, &err);
	if(err != nullptr)
	{
		std::cout << (char*)err->GetBufferPointer() << std::endl;
		err->Release();
	}

	D3DCompileFromFile(L"shaders.hlsl", NULL, NULL, "PSMain", "ps_5_0", D3DCOMPILE_DEBUG, NULL, &pxl, &err);
	if (err != nullptr)
	{
		std::cout << (char*)err->GetBufferPointer() << std::endl;
		err->Release();
	}

	device->CreateVertexShader(vtx->GetBufferPointer(), vtx->GetBufferSize(), NULL, &vertexShader);
	device->CreatePixelShader(pxl->GetBufferPointer(), pxl->GetBufferSize(), NULL, &pixelShader);
	deviceContext->VSSetShader(vertexShader, NULL, NULL);
	deviceContext->PSSetShader(pixelShader, NULL, NULL);

	//Create input layout
	D3D11_INPUT_ELEMENT_DESC iLayout[2]
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(Vertex,x), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(Vertex,tex_u), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	device->CreateInputLayout(iLayout, 2, vtx->GetBufferPointer(), vtx->GetBufferSize(), &inputLayout);
	deviceContext->IASetInputLayout(inputLayout);

	vtx->Release();
	pxl->Release();

	//Create buffers
	//Vertex
	D3D11_BUFFER_DESC vbDesc;
	ZeroMemory(&vbDesc, sizeof(D3D11_BUFFER_DESC));
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.ByteWidth = 4 * sizeof(Vertex);
	vbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbDesc.Usage = D3D11_USAGE_DYNAMIC;
	device->CreateBuffer(&vbDesc, NULL, &vertexBuffer);

	//Index
	unsigned indexData[]
	{
		0,1,2,
		0,2,3
	};
	D3D11_BUFFER_DESC idxDesc;
	ZeroMemory(&idxDesc, sizeof(D3D11_BUFFER_DESC));
	idxDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	idxDesc.ByteWidth = 6 * sizeof(unsigned);
	idxDesc.Usage = D3D11_USAGE_IMMUTABLE;

	D3D11_SUBRESOURCE_DATA idxData;
	ZeroMemory(&idxData, sizeof(D3D11_SUBRESOURCE_DATA));
	idxData.pSysMem = indexData;
	device->CreateBuffer(&idxDesc, &idxData, &indexBuffer);
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//Constant
	D3D11_BUFFER_DESC cbDesc;
	ZeroMemory(&cbDesc, sizeof(D3D11_BUFFER_DESC));
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.ByteWidth = sizeof(CBuffer);
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	device->CreateBuffer(&cbDesc, NULL, &constantBuffer);
}

void D3DRender()
{
	int sf = 63;
	vbData[0].tex_u = mandlebrotRect.left *  std::pow(2, sf);
	vbData[0].tex_v = mandlebrotRect.bottom * std::pow(2, sf);

	vbData[1].tex_u = mandlebrotRect.left * std::pow(2, sf);
	vbData[1].tex_v = mandlebrotRect.top * std::pow(2, sf);

	vbData[2].tex_u = mandlebrotRect.right * std::pow(2, sf);
	vbData[2].tex_v = mandlebrotRect.top * std::pow(2, sf);

	vbData[3].tex_u = mandlebrotRect.right * std::pow(2, sf);
	vbData[3].tex_v = mandlebrotRect.bottom * std::pow(2, sf);

	D3D11_MAPPED_SUBRESOURCE mp;
	deviceContext->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, NULL, &mp);
	memcpy(mp.pData, vbData, sizeof(Vertex) * 4);
	deviceContext->Unmap(vertexBuffer, 0);


	//Update cb data
	deviceContext->Map(constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, NULL, &mp);
	reinterpret_cast<CBuffer*>(mp.pData)->sf = sf;
	deviceContext->Unmap(constantBuffer, 0);
	deviceContext->PSSetConstantBuffers(0, 1, &constantBuffer);

	//Set vb
	unsigned stride = sizeof(Vertex);
	unsigned offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Render
	deviceContext->DrawIndexed(6, 0, 0);
	swpChain->Present(NULL, NULL);
}

void D3DRelease()
{
	//Release all COM objects
	vertexShader->Release();
	pixelShader->Release();
	inputLayout->Release();
	vertexBuffer->Release();
	indexBuffer->Release();
	constantBuffer->Release();
	swpChain->Release();
	pBackBuffer->Release();
	device->Release();
	deviceContext->Release();
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	//Input messages
	case WM_KEYDOWN:
		switch(wParam)
		{
		case 0x41:
			keyStates[0] = true;
			break;
		case 0x44:
			keyStates[1] = true;
			break;
		case 0x53:
			keyStates[2] = true;
			break;
		case 0x57:
			keyStates[3] = true;
			break;
		case 0x45:
			keyStates[4] = true;
			break;
		case 0x51:
			keyStates[5] = true;
			break;
		}
		break;
	case WM_KEYUP:
		switch (wParam)
		{
		case 0x41:
			keyStates[0] = false;
			break;
		case 0x44:
			keyStates[1] = false;
			break;
		case 0x53:
			keyStates[2] = false;
			break;
		case 0x57:
			keyStates[3] = false;
			break;
		case 0x45:
			keyStates[4] = false;
			break;
		case 0x51:
			keyStates[5] = false;
			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}

double GetTime()
{
	auto clock = std::chrono::high_resolution_clock::now();
	return std::chrono::time_point_cast<std::chrono::nanoseconds>(clock).time_since_epoch().count() / 1000000000.0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmd, int nCmdShow)
{
	//Create a console window
	AllocConsole();
	std::ofstream consoleStream("CONOUT$", std::ios::out);
	std::cout.rdbuf(consoleStream.rdbuf());
	std::cerr.rdbuf(consoleStream.rdbuf());

	//Register window class
	WNDCLASSEX wnd;
	ZeroMemory(&wnd, sizeof(WNDCLASSEX));
	wnd.cbSize = sizeof(WNDCLASSEX);
	wnd.style = CS_HREDRAW | CS_VREDRAW;
	wnd.hCursor = LoadCursor(NULL, IDC_ARROW);
	wnd.hInstance = hInstance;
	wnd.lpszClassName = "Main";
	wnd.lpfnWndProc = WinProc;

	RegisterClassEx(&wnd);

	//Get size for window
	RECT wr = { 0, 0, 800, 800 };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	//Create and show window
	HWND hWnd = CreateWindowEx(NULL,
		"Main",
		"Mandelbrot Set",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hWnd, nCmdShow);

	//Start DirectX
	InitD3D(hWnd);

	//Keep track of last time for delta t
	double last_time = GetTime();

	MSG message;
	//Main loop
	while(true)
	{
		if(PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);

			if (message.message == WM_QUIT)
				break;
		}
		else
		{
			//Adjust view
			double new_time = GetTime();
			double deltaT = new_time - last_time;

			//Pan
			double dist = (mandlebrotRect.right - mandlebrotRect.left) * move_speed * deltaT;
			if(keyStates[0])
			{
				mandlebrotRect.left -= dist;
				mandlebrotRect.right -= dist;
			}
			if (keyStates[1])
			{
				mandlebrotRect.left += dist;
				mandlebrotRect.right += dist;
			}
			if (keyStates[2])
			{
				mandlebrotRect.top -= dist;
				mandlebrotRect.bottom -= dist;
			}
			if (keyStates[3])
			{
				mandlebrotRect.top += dist;
				mandlebrotRect.bottom += dist;
			}

			//Zoom
			double zoom = (mandlebrotRect.right - mandlebrotRect.left) * zoom_speed * deltaT / 2;
			if(keyStates[4])
			{
				mandlebrotRect.left -= zoom;
				mandlebrotRect.right += zoom;
				mandlebrotRect.bottom -= zoom;
				mandlebrotRect.top += zoom;

				std::stringstream ss;
				ss << "Mandelbrot Set - Zoom: ";
				ss << 5 / (mandlebrotRect.right - mandlebrotRect.left);
				SetWindowText(hWnd, ss.str().c_str());
			}
			if (keyStates[5])
			{
				mandlebrotRect.left += zoom;
				mandlebrotRect.right -= zoom;
				mandlebrotRect.bottom += zoom;
				mandlebrotRect.top -= zoom;

				std::stringstream ss;
				ss << "Mandelbrot Set - Zoom: ";
				ss << 5 / (mandlebrotRect.right - mandlebrotRect.left);
				SetWindowText(hWnd, ss.str().c_str());
			}

			//Render fractal
			D3DRender();

			last_time = new_time;
		}
	}

	//Cleanup
	D3DRelease();
	FreeConsole();
}