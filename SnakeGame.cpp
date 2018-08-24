#include <Windows.h>
#include <chrono>

#include "Random.h"
#include "Physics.h"
#include "GameLogic.h"
#include "Apple.h"
#include "ReadInput.h"
#include "Menu.h"
#ifndef RENDERER
#include "Renderer.h"
#endif

bool inmenu = true;
bool gamerunning = true;

static int tempDir1;
static int tempDir2;
HHOOK hhook = NULL;

int Snake1Length = 5;
int Snake2Length = 5;
int Snake3Length = 5;
int Snake4Length = 5;
double GameSpeed = 1;
unsigned int GameFieldWidth = 64;
unsigned int GameFieldHeight = 36;

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

bool ComparePoints(POINT p1, POINT p2)
{
	if (p1.x == p2.x && p1.y == p2.y)
		return true;
	return false;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR cmd, int nCmdShow)
{
	/* Win api initialization */
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

	/* Game modules initialization */
	GameLogic game;
	Renderer rend;
	Menu menu;
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
	menu.Init(&rend);

	/* Game timer */
	long lastmovetime = 0;
	long now = 0;
	long delta = 0;

	/* Snake directions for user input */
	int snake1dir = LEFT;
	int snake2dir = LEFT;

	/* Things for getting cursor position */
	POINT p;
	POINT prevp;
	GetCursorPos(&p);
	ScreenToClient(windowhandle, &p);
	prevp = p;
	
	/* Keyboard input */
	hhook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, 0, 0);

	/* Main loop */
	while (msg.message != WM_QUIT && gamerunning)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			DispatchMessage(&msg);
		}
		else
		{
			if (inmenu)
			{
				/* Menu loop */
				GetCursorPos(&p);
				ScreenToClient(windowhandle, &p);
				wchar_t* button = menu.CheckMouseCollision(p, &inmenu, &gamerunning);
				if (GetAsyncKeyState(VK_LBUTTON) < 0)
				{
					if (button == L"START")
					{
						game.Reset();
						game.Init(GameFieldWidth, GameFieldHeight, Snake1Length, Snake2Length, Snake3Length, Snake4Length);
						inmenu = false;
					}
					if (button == L"MULTIPLAYER")
					{
						menu.ChangePage(MULTIPLAYER);
					}
					if (button == L"OPTIONS")
					{
						menu.ChangePage(OPTIONS);
					}
					if (button == L"EXIT")
					{
						gamerunning = false;
					}
				}
				game.OneTick(0, 0, 0, 0);
				rend.RenderFrame(game.physics, menu.GetButtonsVectorForRenderer(), true);
				prevp = p;
				Sleep(50);
			}
			else
			{
				/* Game loop */
				//Sleep(50);
				delta = clock() - lastmovetime;
				if (delta >= 100 / GameSpeed)
				{
					if (tempDir1 > 0 && tempDir1 < 5 && tempDir2 > 0 && tempDir2 < 5)
					{
						snake1dir = tempDir1;
						snake2dir = tempDir2;
					}
					game.OneTick(0, 0, 0, 0);
					rend.RenderFrame(game.physics, menu.GetButtonsVectorForRenderer(), false);
					lastmovetime = clock();
				}
			}
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