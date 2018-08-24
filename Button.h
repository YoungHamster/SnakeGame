#pragma once
#include "Physics.h"

struct button
{
	bool Clickable = false;
	bool Centered = false;
	bool Enlarged = false;
	AABB ClickBox;
	AABB EnlargedClickBox;
	int Size;
	wchar_t *Text;
	void(*whattodo)();
};

struct CheckBox
{
	AABB ClickBox;
	bool selected;
};