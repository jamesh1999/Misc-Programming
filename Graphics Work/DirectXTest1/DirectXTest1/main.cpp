#include "main.h"
#include "Vertex.h"
#include <iostream>

#include <comdef.h>

#define HEIGHT 1000
#define WIDTH 1000

ID3D11Device* dev;
ID3D11DeviceContext* devCon;
IDXGISwapChain* swapChain;
ID3D11RenderTargetView* backBuffer;
ID3D11DepthStencilView* depthView;

ID3D11VertexShader* pVertex;
ID3D11PixelShader* pPixel;

ID3D11Buffer* vertexBuffer;
ID3D11InputLayout* pILayout;

void InitD3D(HWND hWnd)
{
	//Create device and context
	D3D_FEATURE_LEVEL featureLevel;
	D3D11CreateDevice(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&dev,
		&featureLevel,
		&devCon);

	if(featureLevel < D3D_FEATURE_LEVEL_11_0)
	{
		std::cerr << "The adapter does not support DirectX 11 or above" << std::endl;
		exit(-1);
	}

	//Check MSAA quality levels
	unsigned msaaQLevels;
	dev->CheckMultisampleQualityLevels(DXGI_FORMAT_B8G8R8A8_UNORM,
		4,
		&msaaQLevels);

	//Swap chain description
	DXGI_SWAP_CHAIN_DESC swpDesc;
	ZeroMemory(&swpDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swpDesc.BufferCount = 1;
	swpDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swpDesc.BufferDesc.Width = WIDTH;
	swpDesc.BufferDesc.Height = HEIGHT;
	swpDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swpDesc.OutputWindow = hWnd;
	swpDesc.SampleDesc.Count = 4;
	swpDesc.SampleDesc.Quality = msaaQLevels - 1;
	swpDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swpDesc.Windowed = TRUE;
	swpDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	//Create swap chain
	IDXGIDevice* dxgiDevice;
	dev->QueryInterface(__uuidof(IDXGIDevice), (LPVOID*)&dxgiDevice);

	IDXGIAdapter* dxgiAdapter;
	dxgiDevice->GetAdapter(&dxgiAdapter);

	IDXGIFactory* dxgiFactory;
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (LPVOID*)&dxgiFactory);

	dxgiFactory->CreateSwapChain(dev, &swpDesc, &swapChain);

	//Release COM objects
	dxgiDevice->Release();
	dxgiAdapter->Release();
	dxgiFactory->Release();

	//Bind back buffer
	ID3D11Texture2D* pBackBuffer;
	swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	dev->CreateRenderTargetView(pBackBuffer, NULL, &backBuffer);
	pBackBuffer->Release();

	//Create depth/stencil buffer
	D3D11_TEXTURE2D_DESC depthDesc;
	ZeroMemory(&depthDesc, sizeof(D3D11_TEXTURE2D_DESC));

	depthDesc.Width = WIDTH;
	depthDesc.Height = HEIGHT;
	depthDesc.ArraySize = 1;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthDesc.MipLevels = 1;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.SampleDesc.Count = 4;
	depthDesc.SampleDesc.Quality = msaaQLevels - 1;

	ID3D11Texture2D* depthBuffer;
	dev->CreateTexture2D(&depthDesc, NULL, &depthBuffer);
	dev->CreateDepthStencilView(depthBuffer, NULL, &depthView);
	depthBuffer->Release();

	//Set back buffer / depth & stencil buffer as render targets
	devCon->OMSetRenderTargets(1, &backBuffer, depthView);

	//Setup viewport
	D3D11_VIEWPORT vP;

	ZeroMemory(&vP, sizeof(D3D11_VIEWPORT));

	vP.TopLeftX = 0;
	vP.TopLeftY = 0;
	vP.Height = HEIGHT;
	vP.Width = WIDTH;

	devCon->RSSetViewports(1, &vP);
}

void InitPipeline()
{
	//Compile shaders from shaders.shader
	ID3D10Blob *vertex, *pixel;
	D3DCompileFromFile(L"shaders.fx", NULL, NULL, "VShader", "vs_4_0", D3DCOMPILE_DEBUG, NULL, &vertex, NULL);
	D3DCompileFromFile(L"shaders.fx", NULL, NULL, "PShader", "ps_4_0", D3DCOMPILE_DEBUG, NULL, &pixel, NULL);

	//Create and activate shaders
	dev->CreateVertexShader(vertex->GetBufferPointer(), vertex->GetBufferSize(), NULL, &pVertex);
	dev->CreatePixelShader(pixel->GetBufferPointer(), pixel->GetBufferSize(), NULL, &pPixel);

	devCon->VSSetShader(pVertex, NULL, NULL);
	devCon->PSSetShader(pPixel, NULL, NULL);

	//Create input layout
	//(describes VERTEX data to GPU)
	D3D11_INPUT_ELEMENT_DESC ieDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VERTEX,x), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offsetof(VERTEX,Color), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	dev->CreateInputLayout(ieDesc, 2, vertex->GetBufferPointer(), vertex->GetBufferSize(), &pILayout);
	devCon->IASetInputLayout(pILayout);

	vertex->Release();
	pixel->Release();

	D3D11_RASTERIZER_DESC rsState;
	ZeroMemory(&rsState, sizeof(D3D11_RASTERIZER_DESC));
	rsState.CullMode = D3D11_CULL_BACK;
	rsState.FillMode = D3D11_FILL_SOLID;
	rsState.DepthClipEnable = true;
	rsState.AntialiasedLineEnable = true;


	ID3D11RasterizerState* rs;
	dev->CreateRasterizerState(&rsState, &rs);
	devCon->RSSetState(rs);
	rs->Release();
}

typedef struct
{
	DirectX::XMFLOAT4X4 projMatrix;
} cBuffer0;

typedef struct
{
	DirectX::XMFLOAT4X4 objTransform;
} cBuffer1;

ID3D11Buffer* constBuffer0;
ID3D11Buffer* constBuffer1;
ID3D11Buffer* indexBuffer;
void InitGraphics()
{
	VERTEX vertices[] =
	{
		{ -0.2f, 0.2f, 0.2f, DirectX::XMVECTORF32{ 1.0f, 0.0f, 0.0f, 1.0f } },
		{ 0.2f, 0.2f, 0.2f, DirectX::XMVECTORF32{ 1.0f, 1.0f, 1.0f, 1.0f } },
		{ -0.2f, -0.2f, 0.2f, DirectX::XMVECTORF32{ 1.0f, 1.0f, 0.0f, 1.0f } },
		{ 0.2f, -0.2f, 0.2f, DirectX::XMVECTORF32{ 1.0f, 0.0f, 1.0f, 1.0f } },
		{ -0.2f, 0.2f, -0.2f, DirectX::XMVECTORF32{ 0.0f, 0.0f, 0.0f, 1.0f } },
		{ 0.2f, 0.2f, -0.2f, DirectX::XMVECTORF32{ 0.0f, 1.0f, 1.0f, 1.0f } },
		{ -0.2f, -0.2f, -0.2f, DirectX::XMVECTORF32{ 0.0f, 1.0f, 0.0f, 1.0f } },
		{ 0.2f, -0.2f, -0.2f, DirectX::XMVECTORF32{ 0.0f, 0.0f, 1.0f, 1.0f } },
	};

	//Create vertex buffer
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));

	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(VERTEX) * 8;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	dev->CreateBuffer(&bufferDesc, NULL, &vertexBuffer);

	//Fill buffer data (map first to prevent GPU access then unmap after)
	D3D11_MAPPED_SUBRESOURCE mapped;
	devCon->Map(vertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mapped);
	memcpy(mapped.pData, vertices, sizeof(VERTEX) * 8);
	devCon->Unmap(vertexBuffer, NULL);

	UINT Indices[] =
	{
		4, 5, 7,
		4, 7, 6,
		1, 0, 2,
		1, 2, 3,
		0, 1, 5,
		5, 4, 0,
		2, 6, 7,
		7, 3, 2,
		7, 5, 1,
		1, 3, 7,
		0, 4, 6,
		0, 6, 2
	};

	//Create index buffer
	D3D11_BUFFER_DESC ibufferDesc;
	ZeroMemory(&ibufferDesc, sizeof(D3D11_BUFFER_DESC));

	ibufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	ibufferDesc.ByteWidth = sizeof(Indices);
	ibufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indexData;
	ZeroMemory(&indexData, sizeof(D3D11_SUBRESOURCE_DATA));
	indexData.pSysMem = &Indices;

	dev->CreateBuffer(&ibufferDesc, &indexData, &indexBuffer);

	//Constant buffer for vertex shader
	cBuffer0 buffData;
	DirectX::XMMATRIX matt = DirectX::XMMatrixTranspose(DirectX::XMMatrixPerspectiveFovLH(1.0f, 1.0f, 0.01f, 10.0f));
	XMStoreFloat4x4(&buffData.projMatrix, matt);

	D3D11_BUFFER_DESC cbufferDesc;
	ZeroMemory(&cbufferDesc, sizeof(cbufferDesc));

	cbufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbufferDesc.ByteWidth = sizeof(cBuffer0);
	cbufferDesc.Usage = D3D11_USAGE_IMMUTABLE;

	D3D11_SUBRESOURCE_DATA buffDataresource;
	ZeroMemory(&buffDataresource, sizeof(D3D11_SUBRESOURCE_DATA));
	buffDataresource.pSysMem = &buffData;

	dev->CreateBuffer(&cbufferDesc, &buffDataresource, &constBuffer0);
	devCon->VSSetConstantBuffers(0, 1, &constBuffer0);

	D3D11_BUFFER_DESC cbufferDesc1;
	ZeroMemory(&cbufferDesc1, sizeof(cbufferDesc1));

	cbufferDesc1.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbufferDesc1.ByteWidth = sizeof(cBuffer0);
	cbufferDesc1.Usage = D3D11_USAGE_DYNAMIC;
	cbufferDesc1.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	dev->CreateBuffer(&cbufferDesc1, NULL, &constBuffer1);
}

void RenderFrame()
{
	static int rotation = 0;
	rotation = (rotation + 1) % (1024*16);

	static int rotation1 = 0;
	rotation1 = (rotation1 + 1) % (1639*16);

	DirectX::XMMATRIX matt = DirectX::XMMatrixTranspose(DirectX::XMMatrixRotationRollPitchYaw(rotation / (162.97f*16), rotation1 / (260.85f*16), 0.0f) * DirectX::XMMatrixTranslation(0.0f, 0.0f, 2.0f));
	DirectX::XMFLOAT4X4 data;
	DirectX::XMStoreFloat4x4(&data, matt);

	D3D11_MAPPED_SUBRESOURCE mp;
	devCon->Map(constBuffer1, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mp);
	memcpy(mp.pData, &data, sizeof(cBuffer1));
	devCon->Unmap(constBuffer1, NULL);

	devCon->VSSetConstantBuffers(1, 1, &constBuffer1);

	devCon->ClearRenderTargetView(backBuffer, DirectX::XMVECTORF32 {0.0, 0.0, 0.0, 1.0 });

	devCon->ClearDepthStencilView(depthView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0,
		0);

	//Tell vertex buffers to read
	UINT strides = sizeof(VERTEX);
	UINT offsets = 0;
	devCon->IASetVertexBuffers(0, 1, &vertexBuffer, &strides, &offsets);
	devCon->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//Tell what type of vertex data
	devCon->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//DRAW IT!!!!
	devCon->DrawIndexed(36, 0, 0);

	swapChain->Present(NULL, NULL);
}

void CleanD3D()
{
	//Must be windowed to release
	swapChain->SetFullscreenState(FALSE, NULL);

	pVertex->Release();
	pPixel->Release();
	pILayout->Release();
	vertexBuffer->Release();
	constBuffer0->Release();
	constBuffer1->Release();
	swapChain->Release();
	backBuffer->Release();
	depthView->Release();
	dev->Release();
	devCon->Release();
}


// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// sort through and find what code to run for the message given
	switch (message)
	{
		// this message is read when the window is closed
	case WM_DESTROY:
		// close the application entirely
		PostQuitMessage(0);
		return 0;
	}

	// Handle any messages the switch statement didn't
	return DefWindowProc(hWnd, message, wParam, lParam);
}

// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	AllocConsole();
	FILE* ftemp;
	freopen_s(&ftemp,"CONOUT$", "w", stdout);
	freopen_s(&ftemp,"CONOUT$", "w", stderr);
	// the handle for the window, filled by a function
	HWND hWnd;
	// this struct holds information for the window class
	WNDCLASSEX wc;

	// clear out the window class for use
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	// fill in the struct with the needed information
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	//wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = L"WindowClass1";

	// register the window class
	RegisterClassEx(&wc);

	// calculate the size of the client area
	RECT wr = { 0, 0, WIDTH, HEIGHT };    // set the size, but not the position
	//AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);    // adjust the size

														  // create the window and use the result as the handle
	hWnd = CreateWindowEx(NULL,
		L"WindowClass1",    // name of the window class
		L"DirectX Test",    // title of the window
		WS_OVERLAPPEDWINDOW,    // window style
		300,    // x-position of the window
		300,    // y-position of the window
		wr.right - wr.left,    // width of the window
		wr.bottom - wr.top,    // height of the window
		NULL,    // we have no parent window, NULL
		NULL,    // we aren't using menus, NULL
		hInstance,    // application handle
		NULL);    // used with multiple windows, NULL

	MessageBox(hWnd, L"Waiting", L"Waiting", MB_OK);

	InitD3D(hWnd);
	InitPipeline();
	InitGraphics();

				  // display the window on the screen
	ShowWindow(hWnd, nCmdShow);

	// enter the main loop:

	// this struct holds Windows event messages
	MSG msg = { 0 };

	// Enter the infinite message loop
	while (TRUE)
	{
		// Check to see if any messages are waiting in the queue
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// translate keystroke messages into the right format
			TranslateMessage(&msg);

			// send the message to the WindowProc function
			DispatchMessage(&msg);

			// check to see if it's time to quit
			if (msg.message == WM_QUIT)
				break;
		}
		else
		{
			// Run game code here
			// ...
			// ...

			RenderFrame();
		}
	}
	FreeConsole();
	CleanD3D();

	// return this part of the WM_QUIT message to Windows
	return msg.wParam;
}