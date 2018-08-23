#pragma once

#include <Windows.h>
#include <d2d1.h>
#include <vector>
#include <wincodec.h>
#include <string>

#include "Physics.h"
#include "Page.h"
#include "SDL_Rect.h"

#define BITMAPSNUMBER 39 + 41

struct BitmapHW
{
	int Width;
	int Height;
};

BitmapHW bitmapsHWs[BITMAPSNUMBER];

class Renderer
{
private:
	ID2D1Factory * factory;
	ID2D1HwndRenderTarget* rendertarget;
	ID2D1SolidColorBrush* brush;

	// 39 GameTextures + 41 ASCII Symbols
	ID2D1Bitmap* bitmaps[BITMAPSNUMBER];

	int GameFieldWidth = 64;
	int GameFieldHeigth = 36;
public:
	void SetGFW_GFH(int GFW, int GFH) { GameFieldWidth = GFW; GameFieldHeigth = GFH; }

	bool Init(HWND windowhandle);
	void RenderFrame(std::vector<PhysicalObject>& physics, int numberOfObjects, int Snake1size, int Snake2size, int Snake3size, int Snake4size);
	void RenderFrame(std::vector<button>& buttons);

	void BeginDraw() { rendertarget->BeginDraw(); }
	void EndDraw() { rendertarget->EndDraw(); }
	void ClearScreen(float r, float g, float b);
	bool LoadID2D1Bitmap(LPCWSTR filename, ID2D1Bitmap **ppBitmap);
	void DrawBitmap(ID2D1Bitmap* bmp, SDL_Rect* rect, SDL_Rect* srcrect);

	/* Counts text width in pixels */
	int CountTextWidth(const wchar_t* text, double Size);
	/* Use only inside RenderFrame function */
	void DrawTextOnRend(const wchar_t* text, double Size, int minX, int minY);
	void AutoDrawStats();
	/* Use only inside RenderFrame function */
	void RenderButton(button button);

};

int wstrlen(const wchar_t* str)
{
	int i = 0;
	while (str[i] != '\0')
	{
		i++;
	}
	return i;
}

wchar_t* strcatW(const wchar_t *str1, const wchar_t *str2)
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

int CountTextWidth(const wchar_t* text, double Size)
{
	int Width = 0;
	int CurrentCharacterInBitmapsArrayID = 77;
	int currentW = 0;
	for (int i = 0; i < wstrlen(text); i++)
	{
		if (text[i] >= L'A' && text[i] <= L'Z') { CurrentCharacterInBitmapsArrayID = text[i] - 26; }
		if (text[i] == L'+') { CurrentCharacterInBitmapsArrayID = 65; }
		if (text[i] == L'-') { CurrentCharacterInBitmapsArrayID = 66; }
		if (text[i] >= L'0' && text[i] <= L'9') { CurrentCharacterInBitmapsArrayID = text[i] + 19; }
		if (text[i] == L' ') { CurrentCharacterInBitmapsArrayID = 77; }
		if (text[i] == L'.') { CurrentCharacterInBitmapsArrayID = 78; }
		if (text[i] == L'!') { CurrentCharacterInBitmapsArrayID = 79; }
		currentW = Size * bitmapsHWs[i].Width;//bitmaps[CurrentCharacterInBitmapsArrayID]->GetPixelSize().width;
		Width = Width + currentW + Size;
	}
	return Width;
}