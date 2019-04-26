#pragma once
#include "Cheat.h"

class CFont
{
public:
	vgui::HFont Menu;
	vgui::HFont MenuIcons;
	vgui::HFont MenuBold;
	vgui::HFont Menu2;

	vgui::HFont Visuals;
	vgui::HFont Visuals2;
	vgui::HFont Visuals3;

	vgui::HFont Watermark;

	vgui::HFont Notifications;
	vgui::HFont MenuTabs;

	vgui::HFont SubtabWeapons;

	void Initialize();

}; extern CFont Fonts;