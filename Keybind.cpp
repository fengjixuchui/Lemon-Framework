#include "GUI.h"

CKeybind::CKeybind(std::string n_label, ButtonCode_t* n_key)
{
	parent = config.lastGroup;
	label = n_label;
	key = n_key;

	area.h = 20;

	if (n_label != "")
	{
		heightOffset = 14;
		area.h += 14;
	}

	parent->AddElement(this);
}

void CKeybind::Draw()
{
	rect_t n_area = rect_t(area.x, area.y + heightOffset, area.w, 20);

	if (label != "")
		Render.Text(area.x, area.y, label.c_str(), Fonts.Menu, Color(153, 153, 153));

	POINT mouse; GetCursorPos(&mouse);
	rect_t itemRegion = rect_t(n_area.x, n_area.y, n_area.w, 20);

	if (itemRegion.ContainsPoint(mouse))
		Render.GradientVertical(n_area.x, n_area.y, n_area.w, 20, Color(65, 65, 65), Color(59, 59, 59));
	else
		Render.GradientVertical(n_area.x, n_area.y, n_area.w, 20, Color(58, 58, 58), Color(52, 52, 52));

	Render.OutlinedRect(n_area.x, n_area.y, n_area.w, 20, Color::Black);

	std::string toRender = I::InputSystem->ButtonCodeToString(*key);
	std::transform(toRender.begin(), toRender.end(), toRender.begin(), toupper);

	if (*key <= BUTTON_CODE_NONE)
		toRender = "";

	if (takingInput)
		toRender = "Press Any Key";

	Color textColor = takingInput ? config.MenuColor[0] : Color(153, 153, 153);

	Render.Text(n_area.x + 10, n_area.y, toRender.c_str(), Fonts.Menu, textColor);
}

void CKeybind::Update()
{
	if (!takingInput)
		return;

	parent->FocusControl(this);

	for (int i = 0; i < 255; i++)
	{
		if (CMenu::Get().KeyPress(i))
		{
			*key = I::InputSystem->VirtualKeyToButtonCode(i);

			switch (i)
			{
			case 1: *key = MOUSE_LEFT; break;
			case 2: *key = MOUSE_RIGHT; break;
			case 4: *key = MOUSE_MIDDLE; break;
			case 5: *key = MOUSE_4; break;
			case 6: *key = MOUSE_5; break;
			}

			if (*key == KEY_ESCAPE)
			{
				*key = BUTTON_CODE_NONE;
				takingInput = false;

				parent->ResetBlock();

				return;
			}

			takingInput = false;
			parent->ResetBlock();
			return;
		}
	}
}

void CKeybind::LClick()
{
	if (!takingInput)
		takingInput = true;
}