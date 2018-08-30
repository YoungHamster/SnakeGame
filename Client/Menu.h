#pragma once

#include "Page.h"
#define MULTIPLAYER 1
#define OPTIONS 2
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
	void AddButton(int PageID, bool Clickable, bool Centered, int x, int y, wchar_t *Text, int Size, Renderer* renderer, int UBID);
	void ChangeButtonText(int PageID, int ButtonID, wchar_t* Text);
	int CheckMouseCollision(POINT mouse, bool *inmenu, bool *gamerunning);
	/*void AddButton(int PageID, button button);*/
};