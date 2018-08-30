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
	int UBID;
	//void(*whattodo)(Menu *menu);
	//void(*whattodo)() = NULL;
};

struct CheckBox
{
	AABB ClickBox;
	bool selected;
};