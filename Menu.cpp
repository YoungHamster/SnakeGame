#include "Menu.h"

void Menu::Init(Renderer* rend)
{
	for (int i = 0; i < 3; i++)
	{
		AddPage();
	}
	AddButton(0, true, true, 605, 80, const_cast <wchar_t*>(L"START"), 10, -1, rend);
	AddButton(0, true, true, 640, 470, const_cast <wchar_t*>(L"OPTIONS"), 6, -1, rend);
	AddButton(0, true, true, 640, 290, const_cast <wchar_t*>(L"MULTIPLAYER"), 8, -1, rend);
	AddButton(0, true, true, 640, 630, const_cast <wchar_t*>(L"EXIT"), 5, -1, rend);
}

std::vector<button>& Menu::GetButtonsVectorForRenderer()
{
	return pages[CurrentPageID].buttons;
}

void Menu::AddPage()
{
	Page page;
	pages.push_back(page);
}

void Menu::AddButton(int PageID, bool Clickable, bool Centered, int x, int y, wchar_t *Text, int Size, int PointedPageID, Renderer* renderer)
{
	pages[PageID].AddButton(Clickable, Centered, x, y, Text, Size, PointedPageID, renderer);
}

wchar_t* Menu::CheckMouseCollision(POINT mouse, bool *inmenu, bool *gamerunning)
{
	return pages[CurrentPageID].CheckMouseCollisions(mouse);
}

void Menu::ChangePage(int PageID)
{
	CurrentPageID = PageID;
}