#include "GUI.h"

CCheckbox::CCheckbox(std::string n_label, bool* n_state, std::vector<CControlFlag*>n_flags /* = */)
{
	parent = config.lastGroup;
	label = n_label;
	state = n_state;

	flags = n_flags;

	area.h = 9;

	parent->AddElement(this);
}

void CCheckbox::Draw()
{
	Render.GradientVertical(area.x, area.y, 9, 9, Color(58, 58, 58), Color(52, 52, 52));

	POINT mouse; GetCursorPos(&mouse);
	rect_t itemRegion = rect_t(area.x, area.y, 9, 9);

	if (itemRegion.ContainsPoint(mouse)) //Hover
	{
		Render.GradientVertical(area.x, area.y, 9, 9, Color(65, 65, 65), Color(59, 59, 59));
	}

	if (*state)
	{
		Render.GradientVertical(area.x, area.y, 9, 9, Color(config.MenuColor[0].r() - 3, config.MenuColor[0].g() - 3, config.MenuColor[0].b() - 3), Color(config.MenuColor[0].r() + 3, config.MenuColor[0].g() + 3, config.MenuColor[0].b() + 3));
	}

	Render.OutlinedRect(area.x, area.y, 9, 9, Color(0, 0, 0));

	Render.Text(area.x + 15, area.y - 1, label.c_str(), Fonts.Menu, Color(153, 153, 153));
}

void CCheckbox::Update()
{

}

void CCheckbox::LClick()
{
	*state = !(*state);
}