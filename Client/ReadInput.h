#pragma once
#include <Windows.h>

#include "GameLogic.h"

int ReadInput()
{
	/* Keyboard buttons handling */
	if (GetAsyncKeyState(0x57) == -32768 || GetAsyncKeyState(VK_UP) == -32768) return UP; // W key
	if (GetAsyncKeyState(0x41) == -32768 || GetAsyncKeyState(VK_LEFT) == -32768) return LEFT; // A key
	if (GetAsyncKeyState(0x53) == -32768 || GetAsyncKeyState(VK_DOWN) == -32768) return DOWN; // S key
	if (GetAsyncKeyState(0x44) == -32768 || GetAsyncKeyState(VK_RIGHT) == -32768) return RIGHT; // D key
	//if (GetAsyncKeyState(VK_F4) == -32768 && GetAsyncKeyState(VK_MENU) == -32768)
	//{
	//	//gamerunning = false;
	//	return STOPGAME;
	//}
	//if (GetAsyncKeyState(VK_F10) == -32768 && GetAsyncKeyState(VK_F1) == -32768) cout << "Easter Egg!!" << endl;

	//if (GetAsyncKeyState(VK_RETURN) == -32768) return ENTER;

	///* Mouse buttons handling */
	//if (GetAsyncKeyState(VK_LBUTTON) == -32768) return LEFT_BUTTON_CLICK;
	//if (GetAsyncKeyState(VK_RBUTTON) == -32768) return RIGHT_BUTTON_CLICK;
	//if (GetAsyncKeyState(VK_MBUTTON) == -32768) return WHEEL_CLICK;
	return 0;
}

int ReadInputSnake1()
{
	/* Keyboard buttons handling */
	if (GetAsyncKeyState(0x57) == -32768) return UP; // W key
	if (GetAsyncKeyState(0x53) == -32768) return DOWN; // S key
	if (GetAsyncKeyState(0x41) == -32768) return LEFT; // A key
	if (GetAsyncKeyState(0x44) == -32768) return RIGHT; // D key
	return 0;
}

int ReadInputSnake2()
{
	/* Keyboard buttons handling */
	if (GetAsyncKeyState(VK_UP) == -32768) return UP; // W key
	if (GetAsyncKeyState(VK_DOWN) == -32768) return DOWN; // S key
	if (GetAsyncKeyState(VK_LEFT) == -32768) return LEFT; // A key
	if (GetAsyncKeyState(VK_RIGHT) == -32768) return RIGHT; // D key
	return 0;
}