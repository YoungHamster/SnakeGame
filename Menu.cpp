#include "Menu.h"

void Menu::Init(Renderer* rend)
{
	for (int i = 0; i < 4; i++)
	{
		AddPage();
	}
	AddButton(0, true, true, 605, 80, const_cast <wchar_t*>(L"START"), 10, rend);
	AddButton(0, true, true, 640, 290, const_cast <wchar_t*>(L"MULTIPLAYER"), 8, rend);
	AddButton(0, true, true, 640, 470, const_cast <wchar_t*>(L"OPTIONS"), 6, rend);
	AddButton(0, true, true, 640, 630, const_cast <wchar_t*>(L"EXIT"), 5, rend);

	AddButton(1, false, true, 605, 80, const_cast <wchar_t*>(L"MULTIPLAYER"), 10, rend);
	AddButton(1, false, true, 640, 320, const_cast <wchar_t*>(L"COMING"), 6, rend);
	AddButton(1, false, true, 640, 400, const_cast <wchar_t*>(L"SOON"), 6, rend);
	AddButton(1, true, true, 640, 600, const_cast <wchar_t*>(L"BACK"), 8, rend);

	AddButton(2, false, true, 605, 80, const_cast <wchar_t*>(L"OPTIONS"), 10, rend);
	AddButton(2, false, true, 640, 320, const_cast <wchar_t*>(L"COMING"), 6, rend);
	AddButton(2, false, true, 640, 400, const_cast <wchar_t*>(L"SOON"), 6, rend);
	AddButton(2, true, true, 640, 600, const_cast <wchar_t*>(L"BACK"), 8, rend);

	AddButton(3, true, true, 605, 5, const_cast <wchar_t*>(L"BACK TO MAIN MENU"), 4, rend);
	AddButton(3, false, true, 640, 45, const_cast <wchar_t*>(L"1ST BLUE"), 3, rend);
	AddButton(3, false, true, 640, 75, const_cast <wchar_t*>(L"2ND GREEN"), 3, rend);
	AddButton(3, false, true, 640, 105, const_cast <wchar_t*>(L"3RD YELLOW"), 3, rend);
	AddButton(3, false, true, 640, 135, const_cast <wchar_t*>(L"4TH VIOLET"), 3, rend);
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

void Menu::AddButton(int PageID, bool Clickable, bool Centered, int x, int y, wchar_t *Text, int Size, Renderer* renderer)
{
	pages[PageID].AddButton(Clickable, Centered, x, y, Text, Size, renderer);
}

void Menu::ChangeButtonText(int PageID, int ButtonID, wchar_t* Text)
{
	//delete[] pages[PageID].buttons[ButtonID].Text;
	pages[PageID].buttons[ButtonID].Text = Text;
}

wchar_t* Menu::CheckMouseCollision(POINT mouse, bool *inmenu, bool *gamerunning)
{
	return pages[CurrentPageID].CheckMouseCollisions(mouse);
}

void Menu::ChangePage(int PageID)
{
	CurrentPageID = PageID;
}