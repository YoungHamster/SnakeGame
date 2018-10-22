#pragma once
#include <vector>
#include <Windows.h>

//#include "Button.h"
#include "Renderer.h"

class Page
{
private:
public:
	std::vector<button> buttons;
	bool AddButton(bool Clickable, bool Centered, int x, int y, std::wstring* changableText, std::wstring constantText, int Size, Renderer* renderer, int UBID, int AdditionalInfo);
	int CheckMouseCollisions(POINT mouse);
};