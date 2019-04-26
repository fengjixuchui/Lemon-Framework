#include "GUI.h"

CButton::CButton(std::string n_label, std::function<void()> n_func /* = */)
{
	parent = config.lastGroup;
	label = n_label;
	func = n_func;

	area.h = 20;

	parent->AddElement(this);
}

void CButton::Draw()
{
	Render.GradientVertical(area.x, area.y, area.w, 20, Color(58, 58, 58), Color(52, 52, 52));

	POINT mouse; GetCursorPos(&mouse);
	rect_t itemRegion = rect_t(area.x, area.y, area.w, 20);

	if (itemRegion.ContainsPoint(mouse))
		Render.GradientVertical(area.x, area.y, area.w, 20, Color(65, 65, 65), Color(59, 59, 59));

	Render.OutlinedRect(area.x, area.y, area.w, 20, Color::Black);

	Render.Text(area.x + 10, area.y + 5, label.c_str(), Fonts.Menu, Color(153, 153, 153));
}

void CButton::Update()
{

}

void CButton::LClick()
{
	func();
}