#include "Cheat.h"
#include "GUI.h"

CMenu::CMenu()
{
	area = rect_t(100, 100, 750, 550);
}

void CMenu::Draw()
{
	auto outline = [&](int offset, Color color)
	{
		rect_t m_pos = rect_t(area.x - 75, area.y, area.w + 75, area.h);

		auto box = [](int x, int y, int w, int h, Color c)
		{
			Render.Line(x, y, x, y + h, c);
			Render.Line(x, y + h, x + w + 1, y + h, c);
			Render.Line(x + w, y, x + w, y + h, c);
			Render.Line(x, y, x + w, y, c);
		};

		box(m_pos.x - offset, m_pos.y - offset, m_pos.w + offset * 2, m_pos.h + offset * 2, color);
	};

	auto topShadow = [this]()->void
	{
		rect_t shadowArea = rect_t(area.x - 75, area.y - 20,
			area.w + 75, 20);

		Render.GradientVertical(shadowArea.x, shadowArea.y + 20, shadowArea.w, 10, Color(26, 26, 26), Color(42, 42, 42));
		Render.GradientVertical(shadowArea.x, shadowArea.y + 20, shadowArea.w, 10, Color(26, 26, 26), Color(31, 31, 31));
	};

	auto topBar = [this]()->void
	{
		rect_t topArea = rect_t(area.x - 75,
			area.y - 25,
			area.w + 75,
			20);

		auto box = [](int x, int y, int w, int h, Color c)
		{
			Render.Line(x, y, x, y + h, c);
			Render.Line(x, y + h, x + w + 1, y + h, c);
			Render.Line(x + w, y, x + w, y + h, c);
			Render.Line(x, y, x + w, y, c);
		};

		Render.FilledRect(topArea.x, topArea.y, topArea.w, topArea.h, Color(37, 36, 36));

		box(topArea.x - 0, topArea.y - 0, topArea.w + 0 * 2, topArea.h + 0 * 2, Color(0, 0, 0));
		box(topArea.x - 1, topArea.y - 1, topArea.w + 1 * 2, topArea.h + 1 * 2, Color(48, 48, 48));
		box(topArea.x - 2, topArea.y - 2, topArea.w + 2 * 2, topArea.h + 2 * 2, Color(0, 0, 0));

		//This is the infamous ugly top bar (get rid of it lul)
	};

	rect_t mainTabArea = rect_t(area.x - 75, area.y,
		75, area.h);

	Render.FilledRect(area.x, area.y, area.w, area.h, Color(37, 36, 36));
	Render.FilledRect(mainTabArea.x, mainTabArea.y, mainTabArea.w, mainTabArea.h, Color(32, 31, 31));

	for (int i = 0; i < tabs.size(); i++) 
		tabs[i]->Draw();

	outline(0, Color(0, 0, 0));
	outline(1, Color(48, 48, 48));
	outline(2, Color(0, 0, 0));

	// function
	topBar();
}

void CMenu::Update()
{
	//Update Input
	PollKeyboard();

	int gradient[3] =
	{
		config.MenuColor[0].r() - 20,
		config.MenuColor[0].g() - 20,
		config.MenuColor[0].b() - 20
	};

	for (int i = 0; i < 3; i++)
	{
		if (gradient[i] < 42)
			gradient[i] = config.MenuColor[0][i] + 20;
	}

	config.MenuColor[1] = Color(gradient[0], gradient[1], gradient[2]);

	if (KeyPress(I::InputSystem->ButtonCodeToVirtualKey(config.MenuBind)))
	{
		Toggle();

		I::CVar->FindVar("cl_mouseenable")->SetValue(!IsActive());
		I::InputSystem->EnableInput(!IsActive());
		I::InputSystem->ResetInputState();
	}

	if (!IsActive())
		return;

	PollMouse();

	Draw();

	rect_t mainTabArea = rect_t(
		area.x - 95, area.y, 95, area.h
	);

	for (int i = 0; i < tabs.size(); i++)
		tabs[i]->Update();

	if (animating)
	{
		if (animateDirection == TAB_ANIMATION_DOWN)
		{
			if ((mainTabArea.y + animationOffset) < animationDestination) {
				animationOffset += 2;
			}
			else {
				animating = false;
			}
		}
		else
		{
			if ((mainTabArea.y + animationOffset) > animationDestination) {
				animationOffset -= 2;
			}
			else {
				animating = false;
			}
		}
	}

	cMouse.Paint();
	//^ Make a mouse cursor
}

void CMenu::PollKeyboard()
{
	std::copy(m_keystate, m_keystate + 255, m_oldstate);

	for (auto n = 0; n < 255; ++n)
	{
		m_keystate[n] = GetAsyncKeyState(n);
	}
}

void CMenu::PollMouse()
{
	rect_t topArea = rect_t(
		area.x - 75,
		area.y - 27,
		area.w + 75,
		20
	);

	if (dragging && !GetAsyncKeyState(VK_LBUTTON))
		dragging = false;

	if (GetAsyncKeyState(VK_LBUTTON) && topArea.ContainsPoint(cMouse.cursor))
		dragging = true;

	if (dragging) {
		drag_x = cMouse.cursor.x - area.x;
		drag_y = cMouse.cursor.y - area.y;

		GetCursorPos(&cMouse.cursor);

		area.x = cMouse.cursor.x - drag_x;
		area.y = cMouse.cursor.y - drag_y;
	}
	else {
		GetCursorPos(&cMouse.cursor);
	}
}

//Make a mouse cursor?
//Or not...

void CMenu::Mouse_t::Paint()
{

}