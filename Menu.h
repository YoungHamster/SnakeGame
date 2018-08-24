#pragma once

#include "Page.h"
#define MULTIPLAYER 2
#define OPTIONS 1
//bool inmenu = true;
//bool gamerunning = true;


class Menu
{
private:
	int CurrentPageID = 0;
	std::vector<Page> pages;
public:
	void Init(Renderer* rend);
	std::vector<button>& GetButtonsVectorForRenderer();
	//std::vector<button>& GetButtonsVectorForRenderer(int PageID);
	void ChangePage(int PageID);
	void AddPage();
	/*void AddButton(bool Clickable, bool Centered, int x, int y, wchar_t *Text, double Size, int PointedPageID);
	void AddButton(button button);*/
	void AddButton(int PageID, bool Clickable, bool Centered, int x, int y, wchar_t *Text, int Size, int PointedPageID, Renderer* renderer);
	wchar_t* CheckMouseCollision(POINT mouse, bool *inmenu, bool *gamerunning);
	/*void AddButton(int PageID, button button);*/
};