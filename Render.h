#pragma once
#include "Cheat.h"

class CRender
{
public:
	/* Filled Rect */

	void RectFilled(int x, int y, int w, int h, Color clr)
	{
		I::Surface->DrawSetColor(clr);
		I::Surface->DrawFilledRect(x, y, w, h);
	}

	void FilledRect(int x, int y, int w, int h, Color clr)
	{
		this->RectFilled(x, y, x + w, y + h, clr);
	}

	/* Outlined Rect */
	
	void RectOutlined(int x, int y, int w, int h, Color clr)
	{
		I::Surface->DrawSetColor(clr);
		I::Surface->DrawOutlinedRect(x, y, w, h);
	}

	void OutlinedRect(int x, int y, int w, int h, Color clr)
	{
		this->RectOutlined(x, y, x + w, y + h, clr);
	}

	/* Line */
	void Line(int x, int y, int w, int h, Color clr)
	{
		I::Surface->DrawSetColor(clr);
		I::Surface->DrawLine(x, y, w, h);
	}

	/* Gradients */
	void GradientVertical(int x, int y, int w, int h, Color c1, Color c2)
	{
		FilledRect(x, y, w, h, c1);
		BYTE first = c2.r();
		BYTE second = c2.g();
		BYTE third = c2.b();
		for (int i = 0; i < h; i++)
		{
			float fi = i, fh = h;
			float a = fi / fh;
			DWORD ia = a * 255;
			FilledRect(x, y + i, w, 1, Color(first, second, third, ia));
		}
	}

	void GradientHorizontal(int x, int y, int w, int h, Color c1, Color c2)
	{
		FilledRect(x, y, w, h, c1);
		BYTE first = c2.r();
		BYTE second = c2.g();
		BYTE third = c2.b();
		for (int i = 0; i < w; i++)
		{
			float fi = i, fw = w;
			float a = fi / fw;
			DWORD ia = a * 255;
			FilledRect(x + i, y, 1, h, Color(first, second, third, ia));
		}
	}

	/* Text */

	void Text(int x, int y, const char* _Input, vgui::HFont font, Color color, bool centered = false)
	{
		int apple = 0;
		char Buffer[2048] = { '\0' };
		va_list Args;
		va_start(Args, _Input);
		vsprintf_s(Buffer, _Input, Args);
		va_end(Args);
		size_t Size = strlen(Buffer) + 1;
		wchar_t* WideBuffer = new wchar_t[Size];
		mbstowcs_s(0, WideBuffer, Size, Buffer, Size - 1);

		int width = 0;
		int height = 0;
		GetTextSize2(font, _Input, width, height);

		I::Surface->DrawSetTextColor(color);
		I::Surface->DrawSetTextFont(font);
		if (centered)
			I::Surface->DrawSetTextPos(x - (width / 2), y);
		else
			I::Surface->DrawSetTextPos(x, y);
		I::Surface->DrawPrintText(WideBuffer, wcslen(WideBuffer));
	}

	RECT GetTextSize(const char* _Input, vgui::HFont font)
	{
		int apple = 0;
		char Buffer[2048] = { '\0' };
		va_list Args;
		va_start(Args, _Input);
		vsprintf_s(Buffer, _Input, Args);
		va_end(Args);
		size_t Size = strlen(Buffer) + 1;
		wchar_t* WideBuffer = new wchar_t[Size];
		mbstowcs_s(0, WideBuffer, Size, Buffer, Size - 1);
		int Width = 0, Height = 0;

		I::Surface->GetTextSize(font, WideBuffer, Width, Height);

		RECT outcome = { 0, 0, Width, Height };
		return outcome;
	}

	/* This is pretty useless, just the same as other but still gonna keep it because idk why just gonna keep it */
	void GetTextSize2(vgui::HFont font, const char* string, int w, int h) {
		va_list va_alist;
		char buf[1024];
		va_start(va_alist, string);
		_vsnprintf(buf, sizeof(buf), string, va_alist);
		va_end(va_alist);
		wchar_t out[1024];
		MultiByteToWideChar(CP_UTF8, 0, buf, 256, out, 256);

		I::Surface->GetTextSize(font, out, w, h);
	}

	RECT GetTextSize3(vgui::HFont font, const char* text, ...)
	{
		size_t origsize = strlen(text) + 1;
		const size_t newsize = 100;
		size_t convertedChars = 0;
		wchar_t wcstring[newsize];
		mbstowcs_s(&convertedChars, wcstring, origsize, text, _TRUNCATE);

		RECT rect; int x, y;
		I::Surface->GetTextSize(font, wcstring, x, y);
		rect.left = x; rect.bottom = y;
		rect.right = x;
		return rect;
	}

	void ColorSpectrum(int x, int y, int w, int h)
	{
		static int GradientTexture = 0;
		static std::unique_ptr<Color[]> Gradient = nullptr;
		if (!Gradient) {
			Gradient = std::make_unique<Color[]>(w * h);

			for (int i = 0; i < w; i++) {
				int div = w / 6;
				int phase = i / div;
				float t = (i % div) / (float)div;
				int r, g, b;

				switch (phase) {
				case(0):
					r = 255;
					g = 255 * t;
					b = 0;
					break;
				case(1):
					r = 255 * (1.f - t);
					g = 255;
					b = 0;
					break;
				case(2):
					r = 0;
					g = 255;
					b = 255 * t;
					break;
				case(3):
					r = 0;
					g = 255 * (1.f - t);
					b = 255;
					break;
				case(4):
					r = 255 * t;
					g = 0;
					b = 255;
					break;
				case(5):
					r = 255;
					g = 0;
					b = 255 * (1.f - t);
					break;
				}

				for (int k = 0; k < h; k++) {
					float sat = k / (float)h;
					int _r = r + sat * (128 - r);
					int _g = g + sat * (128 - g);
					int _b = b + sat * (128 - b);

					*reinterpret_cast<Color*>(Gradient.get() + i + k * w) = Color(_r, _g, _b);
				}
			}

			GradientTexture = I::Surface->CreateNewTextureID(true);
			I::Surface->DrawSetTextureRGBA(GradientTexture, (unsigned char*)Gradient.get(), w, h);
		}

		I::Surface->DrawSetColor(Color(255, 255, 255, 255));
		I::Surface->DrawSetTexture(GradientTexture);
		I::Surface->DrawTexturedRect(x, y, x + w, y + h);
	}

	auto ColorFromPen(int x, int y, int w, int h, Vector2D mouse) -> Color {
		int div = w / 6;
		int phase = mouse.x / div;
		float t = ((int)mouse.x % div) / (float)div;
		int r, g, b;

		switch (phase) {
		case(0):
			r = 255;
			g = 255 * t;
			b = 0;
			break;
		case(1):
			r = 255 * (1.f - t);
			g = 255;
			b = 0;
			break;
		case(2):
			r = 0;
			g = 255;
			b = 255 * t;
			break;
		case(3):
			r = 0;
			g = 255 * (1.f - t);
			b = 255;
			break;
		case(4):
			r = 255 * t;
			g = 0;
			b = 255;
			break;
		case(5):
			r = 255;
			g = 0;
			b = 255 * (1.f - t);
			break;
		}

		float sat = mouse.y / h;

		int rgb[3] = {
			r + sat * (128 - r),
			g + sat * (128 - g),
			b + sat * (128 - b)
		};

		return Color(rgb[0], rgb[1], rgb[2], 255);
	}

	void CircleFilled(int x, int y, int points, int radius, Color color) 
	{
		static bool once = true;

		static std::vector<float> temppointsx;
		static std::vector<float> temppointsy;

		if (once)
		{
			float step = (float)DirectX::XM_PI * 2.0f / points;
			for (float a = 0; a < (DirectX::XM_PI * 2.0f); a += step)
			{
				temppointsx.push_back(cosf(a));
				temppointsy.push_back(sinf(a));
			}
			once = false;
		}

		std::vector<int> pointsx;
		std::vector<int> pointsy;
		std::vector<Vertex_t> vertices;

		for (int i = 0; i < temppointsx.size(); i++)
		{
			float eeks = radius * temppointsx[i] + x;
			float why = radius * temppointsy[i] + y;
			pointsx.push_back(eeks);
			pointsy.push_back(why);

			vertices.push_back(Vertex_t(Vector2D(eeks, why)));
		}

		I::Surface->DrawSetColor(color);
		I::Surface->DrawTexturedPolygon(points, vertices.data());
	}

	void Triangle(Vector2D point_one, Vector2D point_two, Vector2D point_three, Color color) {

		Vertex_t verts[3] = {
			Vertex_t(point_one),
			Vertex_t(point_two),
			Vertex_t(point_three)
		};

		static int texture = I::Surface->CreateNewTextureID(true);
		unsigned char buffer[4] = { 255, 255, 255, 255 };

		I::Surface->DrawSetTextureRGBA(texture, buffer, 1, 1);
		I::Surface->DrawSetColor(color);
		I::Surface->DrawSetTexture(texture);

		I::Surface->DrawTexturedPolygon(3, verts);
	}
}; extern CRender Render;