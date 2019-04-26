#include "GUI.h"

extern char* LowerCase[254];
extern char* UpperCase[254];

CTextField::CTextField(std::string n_label, std::string * n_input)
{
	parent = config.lastGroup;
	label = n_label;
	input = n_input;

	area.h = 19;

	if (n_label != "")
	{
		heightOffset = 14;
		area.h += 14;
	}

	parent->AddElement(this);
}

void CTextField::Draw()
{
	rect_t n_area = rect_t(area.x,
		area.y + heightOffset,
		area.w,
		19
	);

	if (label != "")
		Render.Text(area.x, area.y, label.c_str(), Fonts.Menu, Color(153, 153, 153));

	POINT mouse; GetCursorPos(&mouse);
	rect_t itemRegion = rect_t(n_area.x, n_area.y, n_area.w, 19);

	if (itemRegion.ContainsPoint(mouse))
		Render.GradientVertical(n_area.x, n_area.y, n_area.w, 19, Color(65, 65, 65), Color(59, 59, 59));
	else
		Render.GradientVertical(n_area.x, n_area.y, n_area.w, 19, Color(58, 58, 58), Color(52, 52, 52));

	Render.OutlinedRect(n_area.x, n_area.y, n_area.w, 19, Color::Black);

	std::string toRender = *input;

	if (GetTickCount64() >= next_blink)
		next_blink = GetTickCount64() + 800;

	if (taking_input && GetTickCount64() > (next_blink - 400))
		toRender += "|";

	if (taking_input)
		Render.Text(n_area.x + 10, n_area.y + 3, toRender.c_str(), Fonts.Menu, Color(200, 200, 200));
	else
		Render.Text(n_area.x + 10, n_area.y + 3, toRender.c_str(), Fonts.Menu, Color(153, 153, 153));
}

void CTextField::Update()
{
	if (!taking_input)
		return;

	std::string str = *input;

	for (int i = 0; i < 255; i++)
	{
		if (CMenu::Get().KeyPress(i))
		{
			if (i == VK_ESCAPE || i == VK_RETURN || i == VK_INSERT)
			{
				taking_input = false;
				return;
			}

			if (strlen(str.c_str()) != 0)
			{
				if (GetAsyncKeyState(VK_BACK))
					*input = str.substr(0, strlen(str.c_str()) - 1);
			}

			if (strlen(str.c_str()) < 42 && i != NULL && UpperCase[i] != nullptr)
			{
				if (GetAsyncKeyState(VK_SHIFT))
					*input = str + UpperCase[i];
				else
					*input = str + LowerCase[i];

				return;
			}

			if (strlen(str.c_str()) < 42 && i == 32)
			{
				*input = str + " ";
				return;
			}
		}
	}
}

void CTextField::LClick()
{
	taking_input = !taking_input;
}

char* LowerCase[254] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x",
"y", "z", nullptr, nullptr, nullptr, nullptr, nullptr, "0", "1", "2", "3", "4", "5", "6",
"7", "8", "9", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, ";", "+", ",", "-", ".", "/?", "~", nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "[", "\\", "]", "'", nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };

char* UpperCase[254] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X",
"Y", "Z", nullptr, nullptr, nullptr, nullptr, nullptr, "0", "1", "2", "3", "4", "5", "6",
"7", "8", "9", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, ";", "+", ",", "_", ".", "?", "~", nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "{", "|", "}", "\"", nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };