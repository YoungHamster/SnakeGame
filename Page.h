#pragma once
#include <vector>
#include <Windows.h>

#include "Button.h"
#include "SDL_Rect.h"
#include "Renderer.h"

class Page
{
private:
public:
	std::vector<button> buttons;

	bool AddButton(button button);
	//bool AddButton(bool Clickable, bool Centered, SDL_Rect rect, wchar_t *Text, double Size);
	bool AddButton(bool Clickable, bool Centered, int x, int y, wchar_t *Text, double Size, int PointedPageID);
	void CheckMouseCollisions(POINT mouse);
};