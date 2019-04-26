#include "GUI.h"

#define gg 19;

CSliderInt::CSliderInt(std::string n_label, int* n_value, int n_min, int n_max, std::string n_follower, std::vector<CControlFlag *>n_flags /* = */)
{
	parent = config.lastGroup;
	label = n_label;
	value = n_value;
	min = n_min;
	max = n_max;
	follower = n_follower;

	flags = n_flags;

	area.h = 15;

	if (n_label != "") 
	{
		heightOffset = 14;
		area.h += 14;
	}

	parent->AddElement(this);
}

void CSliderInt::Draw()
{
	rect_t n_area = area;

	Render.Text(area.x, area.y, label.c_str(), Fonts.Menu, Color(153, 153, 153));

	n_area.y += heightOffset;
	n_area.h -= heightOffset;

	POINT mouse; GetCursorPos(&mouse);
	rect_t itemRegion = rect_t(n_area.x, n_area.y, area.w, 9);

	if (itemRegion.ContainsPoint(mouse))
	{
		Render.GradientVertical(n_area.x, n_area.y, area.w, 9, Color(65, 65, 65), Color(59, 59, 59));
	}
	else
	{
		Render.GradientVertical(n_area.x, n_area.y, area.w, 9, Color(58, 58, 58), Color(52, 52, 52));
	}

	float ratio = (*value - min) / (max - min);
	float location = ratio * area.w;

	Render.GradientVertical(n_area.x, n_area.y, location, 9, Color(config.MenuColor[0].r() - 3, config.MenuColor[0].g() - 3, config.MenuColor[0].b() - 3), Color(config.MenuColor[0].r() + 3, config.MenuColor[0].g() + 3, config.MenuColor[0].b() + 3));
	Render.OutlinedRect(n_area.x, n_area.y, area.w, 9, Color::Black);

	std::stringstream ss;
	ss << *value << follower.c_str();

	Render.Text(n_area.x + location, n_area.y + 6, ss.str().c_str(), Fonts.Menu, Color(153, 153, 153), true);
}

void CSliderInt::Update()
{
	if (dragging)
	{
		if (GetAsyncKeyState(VK_LBUTTON))
		{
			POINT mouse; GetCursorPos(&mouse);

			float newX, ratio;

			newX = mouse.x - area.x;

			if (newX < 0) { newX = 0; }
			if (newX > area.w) { newX = area.w; }

			ratio = newX / float(area.w);
			*value = min + (max - min) * ratio;
		}
		else
			dragging = false;
	}
}

void CSliderInt::LClick()
{
	dragging = true;
}