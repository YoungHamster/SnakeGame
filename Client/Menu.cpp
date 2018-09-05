#include "Menu.h"

void Menu::Init(Renderer* rend)
{
	for (int i = 0; i < 5; i++)
	{
		AddPage();
	}
	AddButton(0, true, true, 0, 80, const_cast <wchar_t*>(L"START"), 10, rend, 0);
	AddButton(0, true, true, 0, 290, const_cast <wchar_t*>(L"MULTIPLAYER"), 8, rend, 1);
	AddButton(0, true, true, 0, 470, const_cast <wchar_t*>(L"OPTIONS"), 6, rend, 2);
	AddButton(0, true, true, 0, 630, const_cast <wchar_t*>(L"EXIT"), 5, rend, 3);

	AddButton(1, false, true, 0, 80, const_cast <wchar_t*>(L"MULTIPLAYER"), 10, rend, -1);
	//AddButton(1, false, true, 640, 320, const_cast <wchar_t*>(L"COMING"), 6, rend, 5);
	AddButton(1, true, true, 0, 360, const_cast <wchar_t*>(L"CONNECT"), 6, rend, 18);
	//AddButton(1, false, true, 640, 400, const_cast <wchar_t*>(L"SOON"), 6, rend, 6);
	AddButton(1, true, true, 0, 600, const_cast <wchar_t*>(L"BACK"), 8, rend, 7);

	AddButton(2, false, true, 0, 40, const_cast <wchar_t*>(L"OPTIONS"), 10, rend, -1);

	AddButton(2, true, true, 0, 300, const_cast <wchar_t*>(L"+"), 4, rend, 9);
	AddButton(2, false, true, 0, 360, const_cast <wchar_t*>(L"SPEED - 1.0"), 6, rend, -1);
	AddButton(2, true, true, 0, 420, const_cast <wchar_t*>(L"-"), 4, rend, 11);

	/*AddButton(2, true, true, 0, 220, const_cast <wchar_t*>(L"+"), 3, rend, 20);
	AddButton(2, false, true, 0, 245, const_cast <wchar_t*>(L"1ST SNAKE LENGHT - 5"), 3, rend, -1);
	AddButton(2, true, true, 0, 270, const_cast <wchar_t*>(L"-"), 3, rend, 21);

	AddButton(2, true, true, 0, 300, const_cast <wchar_t*>(L"+"), 3, rend, 22);
	AddButton(2, false, true, 0, 325, const_cast <wchar_t*>(L"2ND SNAKE LENGHT - 5"), 3, rend, -1);
	AddButton(2, true, true, 0, 350, const_cast <wchar_t*>(L"-"), 3, rend, 23);

	AddButton(2, true, true, 0, 370, const_cast <wchar_t*>(L"+"), 3, rend, 24);
	AddButton(2, false, true, 0, 395, const_cast <wchar_t*>(L"3RD SNAKE LENGHT - 5"), 3, rend, -1);
	AddButton(2, true, true, 0, 440, const_cast <wchar_t*>(L"-"), 3, rend, 25);

	AddButton(2, true, true, 0, 470, const_cast <wchar_t*>(L"+"), 3, rend, 26);
	AddButton(2, false, true, 0, 495, const_cast <wchar_t*>(L"4TH SNAKE LENGHT - 5"), 3, rend, -1);
	AddButton(2, true, true, 0, 520, const_cast <wchar_t*>(L"-"), 3, rend, 27);*/

	AddButton(2, true, true, 0, 600, const_cast <wchar_t*>(L"BACK"), 8, rend, 7);

	AddButton(3, true, true, 0, 5, const_cast <wchar_t*>(L"BACK TO MAIN MENU"), 4, rend, 7);
	AddButton(3, false, true, 0, 45, const_cast <wchar_t*>(L"1ST BLUE"), 3, rend, -1);
	AddButton(3, false, true, 0, 75, const_cast <wchar_t*>(L"2ND GREEN"), 3, rend, -1);
	AddButton(3, false, true, 0, 105, const_cast <wchar_t*>(L"3RD YELLOW"), 3, rend, -1);
	AddButton(3, false, true, 0, 135, const_cast <wchar_t*>(L"4TH VIOLET"), 3, rend, -1);

	AddButton(4, true, true, 0, 5, const_cast <wchar_t*>(L"BACK TO MAIN MENU"), 4, rend, 7);
	AddButton(4, false, true, 0, 45, const_cast <wchar_t*>(L"1ST BLUE"), 3, rend, -1);
	AddButton(4, false, true, 0, 75, const_cast <wchar_t*>(L"2ND GREEN"), 3, rend, -1);
	AddButton(4, false, true, 0, 105, const_cast <wchar_t*>(L"3RD YELLOW"), 3, rend, -1);
	AddButton(4, false, true, 0, 135, const_cast <wchar_t*>(L"4TH VIOLET"), 3, rend, -1);
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

void Menu::AddButton(int PageID, bool Clickable, bool Centered, int x, int y, wchar_t *Text, int Size, Renderer* renderer, int UBID)
{
	pages[PageID].AddButton(Clickable, Centered, x, y, Text, Size, renderer, UBID);
}

void Menu::ChangeButtonText(int PageID, int ButtonID, wchar_t* Text)
{
	//delete[] pages[PageID].buttons[ButtonID].Text;
	pages[PageID].buttons[ButtonID].Text = Text;
}

int Menu::CheckMouseCollision(POINT mouse)
{
	return pages[CurrentPageID].CheckMouseCollisions(mouse);
}

void Menu::ChangePage(int PageID)
{
	CurrentPageID = PageID;
}