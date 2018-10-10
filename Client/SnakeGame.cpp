#include <Windows.h>
#include <chrono>
#include <Windowsx.h>

#include "Random.h"
#include "Physics.h"
#include "GameLogic.h"
#include "Apple.h"
#include "ReadInput.h"
#include "Menu.h"
#include "ClientNetworkEngine.h"
#include "Renderer.h"

struct SnakeNameForSortingByLenght
{
	int lenght;
	wchar_t* name;
};

GameLogic game;
Renderer rend;
Menu menu;
ClientNetworkEngine networkEngine;

bool inmenu = true;
bool gamerunning = true;
static bool keyboardinputmodeText = true;
std::wstring inputString;

bool singleplayer = true;
bool twoPlayerMode = false;

/* Snake directions for user input */
int snake1dir = LEFT;
int snake2dir = LEFT;
static int tempDir1 = LEFT;
static int tempDir2 = LEFT;
HHOOK keyboardHook = NULL;
HHOOK mouseHook = NULL;

int Snake1Length = 5;
int Snake2Length = 5;
int Snake3Length = 5;
int Snake4Length = 5;
static float GameSpeed = 1;
short GameFieldWidth = GAMEFIELDWIDTH;
short GameFieldHeight = GAMEFIELDHEIGTH;

bool MenuTick(POINT p, int* lastMoveTime);
void SingleplayerTick(POINT p, int* lastMoveTime);
void MultiplayerTick(POINT p);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void SortSnakesByLenght(Menu *menu, GameLogic *game);

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

	RECT rect = { 0, 0, SCREENWIDTH, SCREENHEIGTH };
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, false, WS_EX_OVERLAPPEDWINDOW);

	HWND windowhandle = CreateWindowW(L"MainWindow", L"SnakeGame", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, hInstance, 0);

	if (!windowhandle) return 1;

	ShowWindow(windowhandle, nCmdShow);

	MSG msg;
	msg.message = WM_NULL;

	/* Game modules initialization */
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
		if (rend.GetLastRendererError() == CREATION_RENDER_TARGET_ERROR)
		{
			MessageBox(windowhandle, "Failed to create HWND RENDER TARGET", "Failed to create HWND RENDER TARGET", MB_OK);
		}
		return -2;
	}
	menu.Init(&rend, &GameSpeed, &inputString, &networkEngine);

	/* Game timer */
	int lastmovetime = 0;

	/* Things for getting cursor position */
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(windowhandle, &p);

	/* Main loop */
	while (msg.message != WM_QUIT && gamerunning)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			DispatchMessage(&msg);
		}
		else
		{
			GetCursorPos(&p);
			ScreenToClient(windowhandle, &p);
			if (inmenu)
			{
				Sleep(33);
				if (clock() - lastmovetime >= 75 / GameSpeed)
				{
					game.OneTick(0, 0, 0, 0);
					lastmovetime = clock();
				}
				//menu.HandleMouseMovement(p);
				switch (menu.GetMenuState())
				{
				case Game:
					inmenu = false;
					singleplayer = true;
					game.Init(GameFieldWidth, GameFieldHeight, Snake1Length, Snake1Length, Snake1Length, Snake1Length);
					break;
				case MultiplayerGame:
					inmenu = false;
					singleplayer = false;
					break;
				case Multiplayer:
					keyboardinputmodeText = true;
					break;
				case ExitGame:
					gamerunning = false;
					break;
				default:
					keyboardinputmodeText = false;
					inputString.clear();
					break;
				}
				rend.RenderFrame(game.physics, menu.GetButtonsVectorForRenderer(), true);
				//MenuTick(p, &lastmovetime);
			}
			else
			{
				if (singleplayer)
				{
					
					SingleplayerTick(p, &lastmovetime);
				}
				else
				{
					
					MultiplayerTick(p);
				}
			}
		}
	}
	return 0;
}

void SingleplayerTick(POINT p, int* lastMoveTime)
{
	/* Game loop */
	if (menu.GetMenuState() != Game)
	{
		inmenu = true;
	}
	if (GameSpeed > 0)
	{
		Sleep(100 / GameSpeed / 2);
	}
	if ((clock() - *lastMoveTime) >= 75 / GameSpeed)
	{
		if (tempDir1 > 0 && tempDir1 < 5)
		{
			snake1dir = tempDir1;
		}
		if (tempDir2 > 0 && tempDir2 < 5)
		{
			snake2dir = tempDir2;
		}
		if (!twoPlayerMode)
		{
			game.OneTick(0, snake1dir, 0, 0);
		}
		else
		{
			game.OneTick(snake2dir, snake1dir, 0, 0);
		}
		SortSnakesByLenght(&menu, &game);
		rend.RenderFrame(game.physics, menu.GetButtonsVectorForRenderer(), false);
		*lastMoveTime = clock();
	}
}

void MultiplayerTick(POINT p)
{
	MenuStates menuState = menu.GetMenuState();
	if (menuState != MultiplayerChoosingRoom && menuState != MultiplayerWaitingInRoom && menuState != MultiplayerWaitingStart && menuState != MultiplayerGame)
	{
		inmenu = true;
	}
	if (tempDir1 > 0 && tempDir1 < 5)
	{
		snake1dir = tempDir1;
	}
	networkEngine.NetworkTick((NetEngineInput)snake1dir);
	rend.RenderFrame(networkEngine.GetPhysicsForRenderer(), menu.GetButtonsVectorForRenderer(), false);
	Sleep(50);
}

void SortSnakesByLenght(Menu *menu, GameLogic *game)
{
	/*SnakeNameForSortingByLenght lenghts[4];
	for (int i = 0; i < 4; i++)
	{
		lenghts[i].lenght = game->snakes[i].size();
	}
	lenghts[0].name = const_cast<wchar_t*>(L"BLUE");
	lenghts[1].name = const_cast<wchar_t*>(L"GREEN");
	lenghts[2].name = const_cast<wchar_t*>(L"YELLOW");
	lenghts[3].name = const_cast<wchar_t*>(L"VIOLET");
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (lenghts[i].lenght > lenghts[j].lenght)
			{
				SnakeNameForSortingByLenght bigger = lenghts[i];
				lenghts[i] = lenghts[j];
				lenghts[j] = bigger;
			}
		}
	}
	std::wstring str1(L"1ST ");
	str1 += std::wstring(lenghts[0].name);
	menu->ChangeButtonText(3, 1, const_cast<wchar_t*>(str1.c_str()));
	std::wstring str2(L"2ND ");
	str2 += std::wstring(lenghts[1].name);
	menu->ChangeButtonText(3, 2, const_cast<wchar_t*>(str2.c_str()));
	std::wstring str3(L"3RD ");
	str3 += std::wstring(lenghts[2].name);
	menu->ChangeButtonText(3, 3, const_cast<wchar_t*>(str3.c_str()));
	std::wstring str4(L"4TH ");
	str4 += std::wstring(lenghts[3].name);
	menu->ChangeButtonText(3, 4, const_cast<wchar_t*>(str4.c_str()));*/
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;
	case WM_KEYDOWN:
		if (keyboardinputmodeText)
		{
			/*           English alphabet                          Numbers 0-9            */
			if ((wParam >= 0x41 && wParam <= 0x5a) || (wParam >= 0x30 && wParam <= 0x39))
			{
				inputString.push_back((wchar_t)wParam);
			}

			/* Num pad numbers 0-9 */
			if (wParam >= 0x60 && wParam <= 0x69)
			{
				inputString.push_back((wchar_t)(wParam - 0x30));
			}

			/* Backspace */
			if (wParam == 0x08)
			{
				if (inputString.length() > 0)
				{
					inputString.pop_back();
				}
			}
			if (wParam == VK_OEM_PERIOD)
			{
				inputString.push_back(L'.');
			}
			if (wParam == VK_SPACE)
			{
				inputString.push_back(L' ');
			}
			menu.ChangeButtonText(Multiplayer, 1, const_cast<wchar_t*>(inputString.c_str()));
		}
		else
		{
			switch (wParam)
			{
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
			}
		}
		break;
	case WM_LBUTTONDOWN:
		POINT p;
		p.x = GET_X_LPARAM(lParam);
		p.y = GET_Y_LPARAM(lParam);
		//ScreenToClient(hwnd, &p);
		menu.HandleMouseClick(p);
		break;
	case WM_MOUSEMOVE:
		POINT mouse;
		mouse.x = GET_X_LPARAM(lParam);
		mouse.y = GET_Y_LPARAM(lParam);
		menu.HandleMouseMovement(mouse);
		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}