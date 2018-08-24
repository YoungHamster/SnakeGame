#pragma once

#include "Page.h"


class Menu
{
private:
	int CurrentPageID = 0;
	std::vector<Page> pages;
public:
	void Init(Renderer* rend);
	std::vector<button>& GetButtonsVectorForRenderer();
	//std::vector<button>& GetButtonsVectorForRenderer(int PageID);
	void ChangePage();
	void AddPage();
	/*void AddButton(bool Clickable, bool Centered, int x, int y, wchar_t *Text, double Size, int PointedPageID);
	void AddButton(button button);*/
	void AddButton(int PageID, bool Clickable, bool Centered, int x, int y, wchar_t *Text, int Size, int PointedPageID, Renderer* renderer);
	void CheckMouseCollision(POINT mouse);
	/*void AddButton(int PageID, button button);*/
};