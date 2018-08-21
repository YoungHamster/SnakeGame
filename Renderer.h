#pragma once

#include <Windows.h>
#include <d2d1.h>
#include <vector>
#include <wincodec.h>
#include <string>

#include "Physics.h"

struct SDL_Rect
{
	int x;
	int y;
	int w;
	int h;
};

class Renderer
{
private:
	ID2D1Factory * factory;
	ID2D1HwndRenderTarget* rendertarget;
	ID2D1SolidColorBrush* brush;

	// 39 GameTextures + 41 ASCII Symbols
	ID2D1Bitmap* bitmaps[39 + 41];

	int GameFieldWidth = 64;
	int GameFieldHeigth = 36;
public:
	void SetGFW_GFH(int GFW, int GFH) { GameFieldWidth = GFW; GameFieldHeigth = GFH; }

	bool Init(HWND windowhandle);
	void RenderFrame(std::vector<PhysicalObject>& physics, int numberOfObjects, int Snake1size, int Snake2size, int Snake3size, int Snake4size);

	void BeginDraw() { rendertarget->BeginDraw(); }
	void EndDraw() { rendertarget->EndDraw(); }
	void ClearScreen(float r, float g, float b);
	bool LoadID2D1Bitmap(LPCWSTR filename, ID2D1Bitmap **ppBitmap);
	void DrawBitmap(ID2D1Bitmap* bmp, SDL_Rect* rect, SDL_Rect* srcrect);

	int CountTextWidth(const wchar_t* text, int TextSize, int Size);
	void DrawTextOnRend(const wchar_t* text, int TextSize, int Size, int minX, int minY);
	void AutoDrawStats();

};