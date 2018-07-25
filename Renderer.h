#pragma once

#include <Windows.h>
#include <d2d1.h>
#include <vector>
#include <wincodec.h>

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
	ID2D1Bitmap* bitmaps[39];

	int WindowWidth;
	int WindowHeigth;
public:

	bool Init(HWND windowhandle);
	void RenderFrame(std::vector<PhysicalObject>& physics, int numberOfObjects);

	void BeginDraw() { rendertarget->BeginDraw(); }
	void EndDraw() { rendertarget->EndDraw(); }
	void ClearScreen(float r, float g, float b);
	bool LoadID2D1Bitmap(LPCWSTR filename, ID2D1Bitmap **ppBitmap);
	void DrawBitmap(ID2D1Bitmap* bmp, SDL_Rect* rect, SDL_Rect* srcrect);

};