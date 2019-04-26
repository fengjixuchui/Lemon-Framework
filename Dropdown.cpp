#include "GUI.h"
#include <math.h>
#include <algorithm>

#define HEIGHT 19;

CDropdown::CDropdown(std::string n_label, int* n_selected, std::vector<std::string> n_items, std::vector<CControlFlag *> n_flags /* = */)
{
	parent = config.lastGroup;
	label = n_label;
	selected = n_selected;
	items = n_items;
	usingPointer = false;

	flags = n_flags;

	area.h = HEIGHT;

	if (n_label != "")
	{
		heightOffset = 14;
		area.h += 14;
	}

	parent->AddElement(this);
}

CDropdown::CDropdown(std::string n_label, int * n_selected, std::vector<std::string>* n_items, std::vector<CControlFlag *> n_flags /* = */)
{
	parent = config.lastGroup;
	label = n_label;
	selected = n_selected;
	itemsPointer = n_items;
	usingPointer = true;

	flags = n_flags;

	area.h = HEIGHT;

	if (n_label != "") 
	{
		heightOffset = 14;
		area.h += 14;
	}

	parent->AddElement(this);
}

void CDropdown::Draw()
{
	rect_t n_area = rect_t(
		area.x,
		area.y + heightOffset,
		area.w,
		19
	);

	POINT mouse; GetCursorPos(&mouse);
	rect_t itemRegion = rect_t(n_area.x, n_area.y, n_area.w, 19);

	if (label != "")
		Render.Text(area.x, area.y, label.c_str(), Fonts.Menu, Color(153, 153, 153));

	if (itemRegion.ContainsPoint(mouse))
		Render.GradientVertical(n_area.x, n_area.y, n_area.w, 19, Color(65, 65, 65), Color(59, 59, 59));
	else
		Render.GradientVertical(n_area.x, n_area.y, n_area.w, 19, Color(58, 58, 58), Color(52, 52, 52));

	Render.OutlinedRect(n_area.x, n_area.y, n_area.w, 19, Color::Black);

	auto arrow = [](int x, int y) {
		for (auto i = 5; i >= 2; --i) {
			auto offset = 5 - i;

			Render.Line(x + offset, y + offset, x + offset + clamp(i - offset, 0, 5), y + offset, Color(153, 153, 153));
		}
	};

	arrow(n_area.x + n_area.w - 10, n_area.y + 9);

	if (!items.size()) {
		return;
	}

	Render.Text(n_area.x + 10, n_area.y + 3, items[*selected].c_str(), Fonts.Menu, Color(153, 153, 153));

	if (!open)
		return;

	n_area.y += 21;
	n_area.h -= 19;

	Render.FilledRect(n_area.x, n_area.y, area.w, items.size() * 19, Color(55, 55, 55));

	for (int i = 0; i < items.size(); i++)
	{
		auto &current = items.at(i);

		rect_t itemRegion = rect_t(n_area.x, n_area.y + (19 * i), n_area.w, 19);

		if (itemRegion.ContainsPoint(mouse))
			Render.FilledRect(n_area.x, n_area.y + (19 * i), n_area.w, 19, Color(40, 40, 40, 255));

		if (i == *selected)
			Render.Text(area.x + 10, n_area.y + (19 * i) + 3, current.c_str(), Fonts.Menu, config.MenuColor[0]);
		else
			Render.Text(area.x + 10, n_area.y + (19 * i) + 3, current.c_str(), Fonts.Menu, Color(153, 153, 153));
	}

	Render.OutlinedRect(n_area.x, n_area.y, area.w, items.size() * 19, Color::Black);
}

void CDropdown::Update()
{
	if (usingPointer)
		items = *itemsPointer;

	if (!open || !items.size())
		return;

	POINT mouse; GetCursorPos(&mouse);

	parent->FocusControl(this);

	for (int i = 0; i < items.size(); i++)
	{
		rect_t itemRegion = rect_t(area.x, area.y + 19 + (heightOffset) + (i * 19), area.w, 19);

		if (CMenu::Get().KeyPress(VK_LBUTTON) && itemRegion.ContainsPoint(mouse))
		{
			open = false;
			*selected = i;

			parent->ResetBlock();
		}
	}

	rect_t fullArea = rect_t(area.x, area.y + heightOffset, area.w, area.h + (items.size() * 19));
	if (CMenu::Get().KeyPress(VK_LBUTTON) && fullArea.ContainsPoint(mouse))
	{
		open = false;
		parent->ResetBlock();
	}
}

void CDropdown::LClick()
{
	if (open)
		parent->ResetBlock();

	open = !open;
}