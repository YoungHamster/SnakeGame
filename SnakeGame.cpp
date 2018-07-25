#include <Windows.h>
#include <chrono>

#include "Random.h"
#include "Physics.h"
#include "Renderer.h"
#include "GameLogic.h"
#include "Apple.h"
#include "ReadInput.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR cmd, int nCmdShow)
{
	WNDCLASSEX windowclass;
	ZeroMemory(&windowclass, sizeof(WNDCLASSEX));
	windowclass.cbSize = sizeof(WNDCLASSEX);
	windowclass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	windowclass.hInstance = hInstance;
	windowclass.lpfnWndProc = WindowProc;
	windowclass.lpszClassName = "MainWindow";
	windowclass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClassEx(&windowclass);

	RECT rect = { 0, 0, 1280, 720 };
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, false, WS_EX_OVERLAPPEDWINDOW);

	HWND windowhandle = CreateWindowW(L"MainWindow", L"SnakeGame-DirectX", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, hInstance, 0);

	if (!windowhandle) return 1;

	ShowWindow(windowhandle, nCmdShow);

	MSG msg;
	msg.message = WM_NULL;

	GameLogic game;
	Renderer rend;
	int t = clock();
	if (!game.Init(36, 64, 5, 5, 5, 5))
	{
		MessageBox(windowhandle, "ERROR!!! Failed to initialize game logic", "ERROR!!! Failed to initialize game logic", MB_OK);
		return -1;
	}
	if (!rend.Init(windowhandle))
	{
		MessageBox(windowhandle, "ERROR!!! Failed to initialize renderer", "ERROR!!! Failed to initialize renderer", MB_OK);
		return -2;
	}


	long lastmovetime = 0;
	long now = 0;
	long delta = 0;

	int snake1dir = LEFT;
	int snake2dir = LEFT;

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			DispatchMessage(&msg);
		}
		else
		{
			delta = clock() - lastmovetime;
			if (delta >= 16)
			{
				int tempdir = ReadInputSnake1();
				if (tempdir != 0)
					snake1dir = tempdir;
				tempdir = ReadInputSnake2();
				if (tempdir != 0)
					snake2dir = tempdir;
			}
			if (delta >= 200)
			{
				game.OneTick(snake1dir, snake2dir, snake1dir, snake1dir);
				
				lastmovetime = clock();
			}
			rend.RenderFrame(game.physics, game.physics.size());
		}
	}

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}