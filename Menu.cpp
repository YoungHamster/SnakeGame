#include "Menu.h"

void Menu::Init()
{

}

void Menu::AddPage()
{
	Page page;
	pages.push_back(page);
}

void Menu::AddButton(int PageID, bool Clickable, bool Centered, int x, int y, wchar_t *Text, double Size, int PointedPageID)
{
	pages[PageID].AddButton(Clickable, Centered, x, y, Text, Size, PointedPageID);
}

void Menu::AddButton(bool Clickable, bool Centered, int x, int y, wchar_t *Text, double Size, int PointedPageID)
{
	AddButton(CurrentPageID, Clickable, Centered, x, y, Text, Size, PointedPageID);
}

void Menu::AddButton(int PageID, button button)
{
	pages[PageID].AddButton(button);
}

void Menu::AddButton( button button)
{
	AddButton(CurrentPageID, button);
}
