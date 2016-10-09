#include <windows.h>
#include <windowsx.h>
#include <iostream>
#include <fstream>
#include <DirectXMath.h>
#include <d3d11.h>
#include "CompositeObject.h"
#include "GraphicsController.h"
#include "Renderer.h"
#include "Geometry.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "MeshLoader.h"
#include "Transform.h"
#include "Terrain.h"

int width = 500;
int height = 500;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//Handle input messages
	if (Input::HandleMessage(hWnd, message, wParam, lParam))
		return message;
	if (GraphicsController::instance->HandleMessage(hWnd, message, wParam, lParam))
		return message;

	switch(message)
	{
	//Quit the window
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	//Otherwise use the default handling
	default:
		DefWindowProc(hWnd, message, wParam, lParam);
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	srand(time(NULL));

	//Allocate a console and bind cout/cerr to it
	AllocConsole();
	std::ofstream consoleStream("CONOUT$", std::ios::out);
	std::cout.rdbuf(consoleStream.rdbuf());
	std::cerr.rdbuf(consoleStream.rdbuf());

	//Create the window class
	WNDCLASSEX winClass;
	ZeroMemory(&winClass, sizeof(WNDCLASSEX));

	winClass.cbSize = sizeof(winClass);
	winClass.style = CS_HREDRAW | CS_VREDRAW;
	winClass.lpfnWndProc = WndProc;
	winClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	winClass.hInstance = hInstance;
	winClass.lpszClassName = L"CLASS1";

	//Register the window class
	RegisterClassEx(&winClass);
	
	//Create a window with CLASS1
	HWND hWnd = CreateWindow(
		L"CLASS1",
		L"DirectXTest2",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		width,
		height,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	//Finally show the windows
	ShowWindow(hWnd, nCmdShow);

	Time::TimeManager time;
	Input::InputManager input;
	MeshLoader meshLoader;
	GraphicsController graphics(width, height, false, hWnd);

	//Initialize cylinder
	Material* mat = new Material;
	mat->passes.push_back(RenderPass());
	D3D11_INPUT_ELEMENT_DESC iLayout[]
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, pos), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, normal), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(Vertex, tex), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	mat->passes[0].LoadVS(L"shaders.hlsl", "VShader", iLayout, 3);
	mat->passes[0].LoadPS(L"shaders.hlsl", "PShader");
	//mat->LoadTGA("test.tga");

	MeshData* mesh = new MeshData;
	Geometry::GenerateQuad(mesh, 10.0f, 10.0f, 100, 100);
	Terrain::ApplyHeightmap(mesh);

	//Generate terrain texture
	DirectX::XMFLOAT3* groundTex = new DirectX::XMFLOAT3[512 * 512];
	Terrain::GenerateGroundTexture(&groundTex, 512, 512, mesh);
	mat->SetTexture(groundTex, 512, 512);

	//MeshLoader::LoadFBX(mesh, "test.fbx");
	//Geometry::GenerateSphere(mesh, 5.0f, 12, 12);
	Renderer r(mat, mesh);
	Transform t;
	t.SetPosition( { 0.0f, 0.2f, 12.0f } );
	t.SetScale( { 1.0f, 1.0f, 1.0f } );

	CompositeObject co;
	co.AttachComponent<Transform>(&t);
	co.AttachComponent<Renderer>(&r);

	//Add cylinder
	graphics.AddRenderer(co.GetComponent<Renderer>());

	MSG message;
	while (true)
	{
		if (PeekMessage(&message, hWnd, NULL, NULL, true))
		{
			//Translate then handle in WndProc()
			TranslateMessage(&message);
			DispatchMessage(&message);

			if (message.message == WM_QUIT)
				break;
		}
		else
		{
			//Regenerate terrain
			if(Input::InputManager::KeyIsPressed(Input::KeyQ))
			{
				Terrain::ApplyHeightmap(mesh);
				Terrain::GenerateGroundTexture(&groundTex, 512, 512, mesh);
				mat->SetTexture(groundTex, 512, 512);
			}

			int x, y;
			Input::InputManager::GetMousePosition(x, y);
			float dy = (y * 2.0 / GraphicsController::instance->m_scrHeight) - 1;
			float dx = (x * 2.0 / GraphicsController::instance->m_scrWidth) - 1;

			DirectX::XMVECTOR current = co.GetComponent<Transform>()->GetRotation();
			current = DirectX::XMVectorAdd(current, 
				{ dy * static_cast<float>(Time::TimeManager::DeltaT()) * 4.0f ,
				-dx * static_cast<float>(Time::TimeManager::DeltaT()) * 4.0f ,
				0.0f });
			co.GetComponent<Transform>()->SetRotation(current);

			graphics.StartDraw();
			graphics.RenderObjects();
			graphics.EndDraw();
			Time::TimeManager::EndFrame();
			Time::TimeManager::StartFrame();
		}
	}

	delete[] groundTex;

	//Clean up the console
	FreeConsole();

	return 0;
}