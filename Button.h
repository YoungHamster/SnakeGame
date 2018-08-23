#pragma once
#include "Physics.h"

struct button
{
	bool Clickable = false;
	bool Centered = false;
	bool Enlarged = false;
	AABB ClickBox;
	AABB EnlargedClickBox;
	double Size;
	wchar_t *Text;
	int PointedPageID;
};

struct CheckBox
{
	AABB ClickBox;
	bool selected;
};