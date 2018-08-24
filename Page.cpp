#include "Page.h"

bool Page::AddButton(button button)
{
	buttons.push_back(button);
	return true;
}

//bool Page::AddButton(bool Clickable, bool Centered, SDL_Rect rect, wchar_t *Text, double Size)
//{
//	button button;
//	button.Clickable = Clickable;
//	button.Centered = Centered;
//	button.Text = Text;
//	AABB box;
//	box.min.x = rect.x;
//	box.min.y = rect.y;
//	box.max.x = box.min.x + rect.w;
//	box.max.y = box.min.y + rect.h;
//	button.Size = Size;
//	button.ClickBox = box;
//	button.EnlargedClickBox.min.x = button.ClickBox.min.x - button.ClickBox.min.x / 20;
//	button.EnlargedClickBox.min.y = button.ClickBox.min.y - button.ClickBox.min.y / 20;
//	button.EnlargedClickBox.max.x = button.ClickBox.max.x + button.ClickBox.max.x / 20;
//	button.EnlargedClickBox.max.y = button.ClickBox.max.y + button.ClickBox.max.y / 20;
//	buttons.push_back(button);
//	return true;
//}

void Page::CheckMouseCollisions(POINT mouse)
{
	AABB mousebox;
	mousebox.min.x = mouse.x;
	mousebox.min.y = mouse.y;
	mousebox.max.x = mousebox.min.x + 1;
	mousebox.max.y = mousebox.min.y + 1;
	for (int i = 0; i < buttons.size(); i++)
	{
		if (AABBvsAABB(mousebox, buttons[i].ClickBox))
		{
			buttons[i].Enlarged = true;
		}
		else
		{
			buttons[i].Enlarged = false;
		}
	}
}

bool Page::AddButton(bool Clickable, bool Centered, int x, int y, wchar_t *Text, int Size, int PointedPageID, Renderer* renderer)
{
	button button;
	button.PointedPageID = PointedPageID;
	button.Clickable = Clickable;
	button.Centered = Centered;
	button.Text = Text;
	AABB box;
	if (!Centered)
	{
		box.min.x = x;
	}
	else
	{
		box.min.x = 1280 / 2 - renderer->CountTextWidth(Text, Size) / 2;
	}
	box.min.y = y;
	box.max.x = box.min.x + renderer->CountTextWidth(Text, Size);
	box.max.y = box.min.y + 8 * Size;
	button.Size = Size;
	button.ClickBox = box;
	button.EnlargedClickBox.min.x = button.ClickBox.min.x - Size;
	button.EnlargedClickBox.min.y = button.ClickBox.min.y - Size;
	button.EnlargedClickBox.max.x = button.ClickBox.max.x + Size;
	button.EnlargedClickBox.max.y = button.ClickBox.max.y + Size;
	buttons.push_back(button);
	return true;
}