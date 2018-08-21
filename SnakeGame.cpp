#include <Windows.h>
#include <chrono>

#include "Random.h"
#include "Physics.h"
#include "Renderer.h"
#include "GameLogic.h"
#include "Apple.h"
#include "ReadInput.h"

static int tempDir1;
static int tempDir2;
HHOOK hhook = NULL;

int Snake1Length = 5;
int Snake2Length = 5;
int Snake3Length = 5;
int Snake4Length = 5;
unsigned int GameFieldWidth = 64;
unsigned int GameFieldHeight = 36;

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
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
	if (!game.Init(GameFieldWidth, GameFieldHeight, Snake1Length, Snake2Length, Snake3Length, Snake4Length))
	{
		SetWindowTextW(windowhandle, L"ERROR");
		MessageBox(windowhandle, "ERROR!!! Failed to initialize game logic", "ERROR!!! Failed to initialize game logic", MB_OK);
		return -1;
	}
	if (!rend.Init(windowhandle))
	{
		SetWindowTextW(windowhandle, L"ERROR");
		MessageBox(windowhandle, "ERROR!!! Failed to initialize renderer", "ERROR!!! Failed to initialize renderer", MB_OK);
		return -2;
	}
	//rend.SetGFW_GFH(16, 9);


	long lastmovetime = 0;
	long now = 0;
	long delta = 0;

	int snake1dir = LEFT;
	int snake2dir = LEFT;
	
	hhook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, 0, 0);
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			DispatchMessage(&msg);
		}
		else
		{
			//Sleep(50);
			//delta = clock() - lastmovetime;
			/*if (delta >= 16)
			{
				int tempdir = ReadInputSnake1();
				if (tempdir != 0)
					snake1dir = tempdir;
				tempdir = ReadInputSnake2();
				if (tempdir != 0)
					snake2dir = tempdir;
			}*/
			//if (delta >= 5)
			//{
				/*if (tempDir1 > 0 && tempDir1 < 5 && tempDir2 > 0 && tempDir2 < 5)
				{
					snake1dir = tempDir1;
					snake2dir = tempDir2;
				}*/
				//wchar_t str[2];
				//_itow_s(game.snakes[0].size(), str, 2, 10);
				//SetWindowTextW(windowhandle, str);
				game.OneTick(0, 0, 0, 0);
				rend.RenderFrame(game.physics, game.physics.size(), game.snakes[0].size(), game.snakes[1].size(), game.snakes[2].size(), game.snakes[3].size());
				lastmovetime = clock();
			//}
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

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{

	KBDLLHOOKSTRUCT* pkbs = (KBDLLHOOKSTRUCT*)lParam;
	if (WM_KEYDOWN == wParam)
	{
		/*char C[255];
		wsprintf(C, "нажата клавиша с кодом %x \n", pkbs->vkCode);
		OutputDebugString(C);*/
		switch (pkbs->vkCode)
		{
		case VK_UP:
			tempDir2 = UP;
			break;
		case VK_DOWN:
			tempDir2 = DOWN;
			break;
		case VK_LEFT:
			tempDir2 = LEFT;
			break;
		case VK_RIGHT:
			tempDir2 = RIGHT;
			break;
		case 0x57:
			tempDir1 = UP;
			break;
		case 0x53:
			tempDir1 = DOWN;
			break;
		case 0x41:
			tempDir1 = LEFT;
			break;
		case 0x44:
			tempDir1 = RIGHT;
			break;
		}
	}
	return CallNextHookEx(hhook, nCode, wParam, lParam);
}