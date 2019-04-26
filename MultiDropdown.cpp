#include "GUI.h"

CMultiDropdown::CMultiDropdown(std::string n_label, std::vector<MultiDropdownItem_t>* n_items, std::vector<CControlFlag *> n_flags /* = */)
{
	parent = config.lastGroup;
	label = n_label;
	items_ptr = n_items;

	flags = n_flags;

	area.h = 19;

	if (n_label != "")
	{
		heightOffset = 14;
		area.h += 14;
	}

	parent->AddElement(this);
}

void CMultiDropdown::Draw()
{
	std::vector<MultiDropdownItem_t>items = *items_ptr;

	rect_t n_area = rect_t(area.x, area.y + heightOffset, area.w, 19);

	if (label != "")
		Render.Text(area.x, area.y, label.c_str(), Fonts.Menu, Color(153, 153, 153));
	else
		Render.Text(area.x, area.y, "None", Fonts.Menu, Color(153, 153, 153));

	POINT mouse; GetCursorPos(&mouse);
	rect_t itemRegion = rect_t(n_area.x, n_area.y, n_area.w, 19);

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

	if (!items.size())
	{
		return;
	}

	std::string ss;
	for (auto n = 0; n < items.size(); ++n)
	{
		auto formatLen = ss.length() < 13;
		auto first = ss.length() <= 0;

		if (items[n].enabled && formatLen)
		{
			if (!first)
				ss.append(", ");

			ss.append(items[n].name);
		}
		else if (!formatLen)
		{
			ss.append("...");
			break;
		}
	}

	if (ss.length() <= 0)
		ss += "None";

	Render.Text(n_area.x + 10, n_area.y + 5, ss.c_str(), Fonts.Menu, Color(153, 153, 153));

	if (!open)
		return;

	n_area.y += 21;
	n_area.h -= 19;

	Render.GradientVertical(n_area.x, n_area.y, area.w, items.size() * 19, Color(58, 58, 58), Color(52, 52, 52));

	for (int i = 0; i < items.size(); i++)
	{
		auto & current = items.at(i);

		rect_t itemRegion(n_area.x, n_area.y + (19 * i), n_area.w, 19);

		if (itemRegion.ContainsPoint(mouse))
			Render.FilledRect(n_area.x, n_area.y + (19 * i), n_area.w, 19, Color(190, 190, 190, 100));
		
		if (current.enabled)
			Render.Text(area.x, n_area.y + (19 * i) + 10, current.name.c_str(), Fonts.Menu, config.MenuColor[0]);
		else
			Render.Text(area.x, n_area.y + (19 * i) + 10, current.name.c_str(), Fonts.Menu, Color(153, 153, 153));
	}

	Render.OutlinedRect(n_area.x, n_area.y, area.w, items.size() * 19, Color::Black);
}

void CMultiDropdown::Update()
{
	std::vector<MultiDropdownItem_t>items = *items_ptr;

	if (!open || !items.size())
		return;

	POINT mouse; GetCursorPos(&mouse);

	parent->FocusControl(this);

	for (int i = 0; i < items.size(); i++)
	{
		rect_t itemRegion = rect_t(area.x, area.y + 19 + (heightOffset)+(i * 19), area.w, 19);

		if (CMenu::Get().KeyPress(VK_LBUTTON) && itemRegion.ContainsPoint(mouse))
			items[i].enabled = !items[i].enabled;
	}

	rect_t fullArea = rect_t(area.x, area.y + heightOffset, area.w, area.h + (items.size() * 19));

	if (CMenu::Get().KeyPress(VK_LBUTTON) && !fullArea.ContainsPoint(mouse))
	{
		open = false;
		parent->ResetBlock();
	}

	*items_ptr = items;
}

void CMultiDropdown::LClick()
{
	if (open)
		parent->ResetBlock();

	open = !open;
}