#pragma once
#include "Core//Physics.h"

struct button
{
	bool Clickable = false;
	bool Centered = false;
	bool Enlarged = false;
	AABB ClickBox;
	AABB EnlargedClickBox;
	int Size;
	std::wstring* changableText = NULL;
	std::wstring constantText;
	int UBID;
	int AdditionalInfo;
};

struct CheckBox
{
	AABB ClickBox;
	bool selected;
};