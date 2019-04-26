#include "Cheat.h"

CFont Fonts;

void CFont::Initialize()
{
	Menu = I::Surface->CreateFont_();
	MenuIcons = I::Surface->CreateFont_();
	MenuBold = I::Surface->CreateFont_();
	Menu2 = I::Surface->CreateFont_();

	Visuals = I::Surface->CreateFont_();
	Visuals2 = I::Surface->CreateFont_();
	Visuals3 = I::Surface->CreateFont_();

	Watermark = I::Surface->CreateFont_();

	Notifications = I::Surface->CreateFont_();

	MenuTabs = I::Surface->CreateFont_();

	SubtabWeapons = I::Surface->CreateFont_();

	/* Defining fonts */
	I::Surface->SetFontGlyphSet(Visuals, "Verdana", 12, 400, 0, 0, FONTFLAG_DROPSHADOW);
	I::Surface->SetFontGlyphSet(Visuals2, "Smallest Pixel-7", 12, 400, 0, 0, FONTFLAG_OUTLINE);
	I::Surface->SetFontGlyphSet(Visuals3, "Small Fonts", 9, 100, 0, 0, FONTFLAG_OUTLINE);

	I::Surface->SetFontGlyphSet(Menu, "Verdana", 12, 400, 0, 0, FONTFLAG_DROPSHADOW);
	I::Surface->SetFontGlyphSet(MenuBold, "Verdana", 12, 600, 0, 0, FONTFLAG_DROPSHADOW);
	I::Surface->SetFontGlyphSet(MenuIcons, "cherryfont", 36, 400, 0, 0, FONTFLAG_ANTIALIAS);
	I::Surface->SetFontGlyphSet(MenuTabs, "Verdana", 16, 400, 0, 0, FONTFLAG_OUTLINE);

	I::Surface->SetFontGlyphSet(Notifications, "Arial", 16, 500, 0, 0, FONTFLAG_ANTIALIAS);

	I::Surface->SetFontGlyphSet(SubtabWeapons, "undefeated", 12, FW_MEDIUM, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE);
}