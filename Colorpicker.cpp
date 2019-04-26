#include "GUI.h"

CColorpicker::CColorpicker(Color * n_color, std::string n_label /* = "" */)
{
	area.w = 15;
	area.h = 8;

	parent = config.lastGroup;
	preview_color = n_color;
	color = *preview_color;
	label = n_label;

	brightness = 1.0f;
}

void CColorpicker::Draw()
{
	Render.FilledRect(area.x, area.y, area.w, area.h, *preview_color);
	Render.OutlinedRect(area.x, area.y, area.w, area.h, Color::Black);

	rect_t n_area = rect_t(
		area.x, area.y + area.h,
		234, 254
	);

	if (open)
	{
		Render.FilledRect(n_area.x, n_area.y, n_area.w, n_area.h, Color{ 25, 25, 25 });
		Render.OutlinedRect(n_area.x, n_area.y, n_area.w, n_area.h, Color::Black);

		n_area = rect_t(
			n_area.x + 5,
			n_area.y + 5,
			n_area.w - 10,
			n_area.h - 10
		);
		
		Render.FilledRect(n_area.x, n_area.y, n_area.w, n_area.h, Color{ 35, 35, 35, 255 });
		Render.OutlinedRect(n_area.x, n_area.y, n_area.w, n_area.h, Color::Black);

		n_area = rect_t(
			n_area.x + 4,
			n_area.y + 4,
			n_area.w - 8,
			n_area.h - 28
		);

		Render.ColorSpectrum(n_area.x, n_area.y, 216, 216);
		Render.OutlinedRect(n_area.x, n_area.y, n_area.w, n_area.h, Color::Black);

		rect_t slider_area = rect_t(
			n_area.x,
			n_area.y + 218,
			216, 11
		);

		Render.GradientHorizontal(slider_area.x, slider_area.y, 108, 11, Color::White, color);
		Render.GradientHorizontal(slider_area.x + 108, slider_area.y, 108, 11, color, Color::Black);

		Render.OutlinedRect(slider_area.x, slider_area.y, 216, 11, Color::Black);

		slider_area.y += 12;

		float ratio = (brightness - 0.0f) / (2.0f - 0.0f);
		float location = ratio * 216;

		int points[6] = {
			slider_area.x + location,
			slider_area.y,
			slider_area.x + location + 4,
			slider_area.y + 6,
			slider_area.x + location - 4,
			slider_area.y + 6,
		};

		Render.Triangle(Vector2D(points[0], points[1]), Vector2D(points[2], points[3]), Vector2D(points[4], points[5]), Color(153, 153, 153));
	}
}

void CColorpicker::Update()
{
	auto recalculate_color = [this]() -> void {
		float new_brightness = 2.0f - brightness;

		int new_colors[3] = {
			new_brightness * color.r(),
			new_brightness * color.g(),
			new_brightness * color.b()
		};

		if (new_brightness > 1.0f) {
			new_colors[0] = color.r() + (new_brightness - 1.0f) * (255 - color.r());
			new_colors[1] = color.g() + (new_brightness - 1.0f) * (255 - color.g());
			new_colors[2] = color.b() + (new_brightness - 1.0f) * (255 - color.b());
		}

		*preview_color = Color(new_colors[0], new_colors[1], new_colors[2]);
	};

	if (!open) {
		return;
	}

	parent->FocusSubcontrol(this);

	POINT mouse; GetCursorPos(&mouse);

	rect_t open_area = rect_t(
		area.x, area.y + area.h,
		234, 254
	);

	if (CMenu::Get().KeyPress(VK_LBUTTON) && !open_area.ContainsPoint(mouse)) {
		open = false;
		parent->ResetBlock();
	}

	open_area = rect_t(
		area.x + 9, area.y + area.h + 9,
		216, 216
	);

	rect_t slider_area = rect_t(
		area.x + 9,
		area.y + area.h + 227,
		216, 11
	);

	if (GetAsyncKeyState(VK_LBUTTON) && slider_area.ContainsPoint(mouse)) {
		dragging = true;
	}

	if (dragging) {
		if (GetAsyncKeyState(VK_LBUTTON)) {
			float
				new_x,
				ratio;

			new_x = mouse.x - slider_area.x;

			if (new_x < 0) { new_x = 0; }
			if (new_x > slider_area.w) { new_x = 216; }

			ratio = new_x / float(slider_area.w);
			brightness = 2.0f * ratio;

			recalculate_color();
		}
		else {
			dragging = false;
		}
	}

	if (GetAsyncKeyState(VK_LBUTTON) && open_area.ContainsPoint(mouse) && !dragging) {
		color = Render.ColorFromPen(open_area.x, open_area.y, 216, 216, Vector2D(mouse.x - open_area.x, mouse.y - open_area.y));

		recalculate_color();
	}
}

void CColorpicker::Click()
{
	if (open) {
		parent->ResetBlock();
	}

	open = !open;
}