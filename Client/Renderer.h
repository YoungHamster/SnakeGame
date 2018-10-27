#pragma once


/* Part of windows 10 sdk in visual studio */
#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "d2d1.lib")

#include <Windows.h>
#include <d2d1.h>
#include <vector>
#include <wincodec.h>
#include <string>

#define RENDERER

#include "Physics.h"
//#include "Page.h"
#include "Button.h"
#include "SDL_Rect.h"

#define CREATION_RENDER_TARGET_ERROR 1
#define CREATION_BRUSH_ERROR 2

#define BITMAPSNUMBER 39 + 41
#define SCREENHEIGTH 1080
#define SCREENWIDTH 1920
#define GAMEFIELDHEIGTH (SCREENHEIGTH / 30)
#define GAMEFIELDWIDTH (SCREENWIDTH / 30)

static int bitmapsHWs[BITMAPSNUMBER];


class Renderer
{
private:
	ID2D1Factory * factory;
	ID2D1HwndRenderTarget* rendertarget;
	//ID2D1SolidColorBrush* brush;

	// 39 GameTextures + 41 ASCII Symbols
	ID2D1Bitmap* bitmaps[BITMAPSNUMBER];

	int GameFieldWidth = GAMEFIELDWIDTH;
	int GameFieldHeigth = GAMEFIELDHEIGTH;

	int LastRendererError = 0;
public:

	bool Init(HWND windowhandle);
	void RenderFrame(std::vector<PhysicalObject>& physics, std::vector<button>& buttons, bool menumode);
	//void RenderFrame(char compressedPhysics[GAMEFIELDHEIGTH * GAMEFIELDWIDTH], std::vector<button>& buttons);
	//void RenderFrame(char* compressedPhysics, std::vector<button>& buttons, int height, int width);
	//void RenderFrame(std::vector<PhysicalObject>& physics, std::vector<button>& buttons);

	void BeginDraw() { rendertarget->BeginDraw(); }
	void EndDraw() { rendertarget->EndDraw(); }
	void ClearScreen(float r, float g, float b);
	bool LoadID2D1Bitmap(LPCWSTR filename, ID2D1Bitmap **ppBitmap);
	void DrawBitmap(ID2D1Bitmap* bmp, RECT* rect, RECT* srcrect, float opacity);

	/* Counts text width in pixels */
	int CountTextWidth(const wchar_t* text, int Size);
	/* Use only inside RenderFrame function */
	void DrawTextOnRend(const wchar_t* text, int Size, int minX, int minY);
	void AutoDrawStats();
	/* Use only inside RenderFrame function */
	void RenderButton(button button);

	D2D1_SIZE_U GetRenderTargetSize();

	int GetLastRendererError();

	void SetGameFieldSize(int width, int heigth) { GameFieldWidth = width; GameFieldHeigth = heigth; }

};

inline int wstrlen(const wchar_t* str)
{
	int i = 0;
	while (str[i] != '\0')
	{
		++i;
	}
	return i;
}

inline wchar_t* strcatW(const wchar_t *str1, const wchar_t *str2)
{
	wchar_t *str = new wchar_t[(wstrlen(str1) + wstrlen(str2) + 1)];
	for (int i = 0; i < wstrlen(str); i++)
	{
		if (i < wstrlen(str1))
		{
			str[i] = str1[i];
		}
		else
		{
			str[i] = str2[i - wstrlen(str1)];
		}
	}
	return str;
}