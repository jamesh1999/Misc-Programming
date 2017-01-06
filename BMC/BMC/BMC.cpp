#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include <fstream>
#include "GraphicsController.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "Config.h"
#include "Camera.h"
#include "Simulation.h"


#define ZOOM_SPEED 0.1f

/*
Basic program flow:
Genetic algorithm to optimise overall route
Evaluate based on a function of payback time and max capacity
Track routes generated via simulated annealing based on maximum cost from general route plan
Evaluate routes based on time to travel
Penalise routes that don't meet spec

Required input:

Obstruction maps

Track types:
- Cost per unit length
- Repulsion force

Train types:
- Variable seat angle
- Seat inclination
- Acceleration force
- Coefficient of drag
- Mass
- Capacity
- Running cost
- Initial cost

Global Settings:
- Maximum G-forces
- Necessary capacity

Config format:
[Category]
(#Number)
Label: value

() data is for instanced data types only
*/

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (GraphicsController::instance->HandleMessage(hWnd, msg, wParam, lParam))
		return msg;
	if (Input::HandleMessage(hWnd, msg, wParam, lParam))
		return msg;

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdShow, int nCmdShow)
{
	//Connect console
	AllocConsole();
	std::ofstream consoleOut("CONOUT$", std::ios::out);
	std::cout.set_rdbuf(consoleOut.rdbuf());
	std::cerr.set_rdbuf(consoleOut.rdbuf());

	//Create window
	WNDCLASSEX wndClass;
	ZeroMemory(&wndClass, sizeof(WNDCLASSEX));
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hInstance = hInstance;
	wndClass.lpszClassName = L"MainClass";

	RegisterClassEx(&wndClass);

	HWND hWnd = CreateWindow(
		L"MainClass",
		L"Route Planner",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		640,
		640,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	GraphicsController controller(640, 640, false, hWnd);
	Camera cam;
	Time::TimeManager tm;
	Input::InputManager input;

	Config config;
	Config::LoadFromFile("config.txt");

	Route r;
	r.push_back({ 0.0, 10000.0 });
	r.push_back({ 50000.0, 10000.0 });
	r.push_back({ 14000.0, 4000.0 });
	r.push_back({ 100000.0, 10000.0 });
	r.push_back({ 0.0, 5000 });
	auto res = Simulate(r, 0);
	std::cout << res[0].time << std::endl;

	ShowWindow(hWnd, nCmdShow);

	MSG msg;
	while (true)
	{
		if (PeekMessage(&msg, hWnd, NULL, NULL, true))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (msg.message == WM_QUIT) break;
		}
		else
		{
			/*//Main program loop
			//Pan camera
			if (Input::InputManager::MouseIsPressed(Input::MouseButton::Left))
			{
				int x, y;
				Input::InputManager::GetMouseDelta(x, y);

				cam.position.x += x / 640.0f / cam.scale;
				cam.position.y += y / 640.0f / cam.scale;
			}
			//Zoom camera
			float delta = 0.0f;
			if (input.KeyIsPressed(Input::KeyboardButton::KeyI))
				delta += ZOOM_SPEED * Time::TimeManager::DeltaT();
			else if (input.KeyIsPressed(Input::KeyboardButton::KeyO))
				delta -= ZOOM_SPEED * Time::TimeManager::DeltaT();


			GraphicsController::instance->StartDraw();
			GraphicsController::instance->RenderObjects();
			GraphicsController::instance->EndDraw();
			Time::TimeManager::EndFrame();
			Time::TimeManager::StartFrame();*/
		}
	}

	FreeConsole();
	return 0;
}

