#include "GUI.h"

CTab::CTab(CMenu* n_parent, int n_index, std::string n_icon, std::string n_name, std::vector<subtab_t> n_subtabs)
{
	parent = n_parent;
	index = n_index;
	icon = n_icon;
	name = n_name;

	selectedSub = -1;

	if (n_subtabs.size()) 
	{
		subtabs = n_subtabs;
		selectedSub = subtabs[0].index;
	}

	n_parent->AddTab(this);

	config.tabSub = -1;
	config.lastTab = this;
}

CTab::subtab_t::subtab_t(char* _name, int _index) {
	name = _name;
	index = _index;
	font = Fonts.MenuTabs;
}

CTab::subtab_t::subtab_t(char* _name, int _index, vgui::HFont _font) {
	name = _name;
	index = _index;
	font = _font;
}

void CTab::DrawSubtabs()
{
	if (subtabs.size())
	{
		rect_t area = rect_t(
			CMenu::Get().area.x + 8,
			CMenu::Get().area.y + 8,
			CMenu::Get().area.w - 15,
			15
		);

		float sub_width = area.w / subtabs.size();

		Render.FilledRect(area.x, area.y, area.w, 26, Color(32, 31, 31));
		Render.OutlinedRect(area.x, area.y, area.w, 27, Color(48, 48, 48));
		Render.OutlinedRect(area.x - 1, area.y - 1, area.w + 2, 29, Color(0, 0, 0));

		for (int i = 0; i < subtabs.size(); i++) {
			int text_x = area.x + (sub_width * i) + (sub_width / 2);

			int add = 0;

			if (subtabs[i].font == Fonts.SubtabWeapons)
				add += 3;

			Render.Text(text_x, area.y + 5 + add, subtabs[i].name, subtabs[i].font, selectedSub == subtabs[i].index ? config.MenuColor[0] : Color(153, 153, 153));
		}
	}
}

void CTab::UpdateSubtabs() {
	if (subtabs.size()) {
		rect_t area = rect_t(
			CMenu::Get().area.x + 8,
			CMenu::Get().area.y + 8,
			CMenu::Get().area.w - 16,
			30
		);

		float sub_width = area.w / subtabs.size();

		for (int i = 0; i < subtabs.size(); i++) {
			rect_t tab_area = rect_t(
				area.x + (sub_width * i),
				area.y, sub_width, 26
			);

			if (CMenu::Get().KeyPress(VK_LBUTTON) && tab_area.ContainsPoint(CMenu::Get().cursor())) {
				if (!animating && selectedSub != subtabs[i].index) {
					animating = true;
					animatingDirection = (subtabs[i].index < selectedSub) ? SUBTAB_ANIMATION_LEFT : SUBTAB_ANIMATION_RIGHT;
					animationDestination = tab_area.x;
					selectedSub = subtabs[i].index;

					resetInputBlock = true;
				}
			}
		}

		for (int i = 0; i < 9; i++) {
			if (!animating)
				break;

			if (animatingDirection == SUBTAB_ANIMATION_RIGHT) {
				if ((area.x + animationOffset) < animationDestination) {
					animationOffset += 1;
					clamp(animationOffset, 0.0f, area.x - animationDestination);
				}
				else {
					animating = false;
				}
			}
			else {
				if ((area.x + animationOffset) > animationDestination) {
					animationOffset -= 1;
					clamp(animationOffset, area.x - animationDestination, (float)area.w);
				}
				else {
					animating = false;
				}
			}
		}
	}
}

void CTab::Draw()
{
	rect_t tab_area = rect_t(
		CMenu::Get().area.x - 75,
		CMenu::Get().area.y + (index * (CMenu::Get().area.h / CMenu::Get().tabs.size())),
		75, (CMenu::Get().area.h / CMenu::Get().tabs.size())
	);

	rect_t tab_bar = rect_t(
		CMenu::Get().area.x - 10,
		CMenu::Get().area.y + CMenu::Get().animationOffset,
		10, 20
	);

	rect_t top2bottom = rect_t(
		CMenu::Get().area.x,
		CMenu::Get().area.y,
		1,
		CMenu::Get().area.h
	);

	if (index != CMenu::Get().selectedTab)
		Render.Text(tab_area.x + (tab_area.w / 2) - 20, tab_area.y + (tab_area.h / 2) - 20, name.c_str(), Fonts.MenuIcons, Color(210, 210, 210, 255), true);

	if (index == CMenu::Get().selectedTab) {
		Render.OutlinedRect(top2bottom.x, top2bottom.y, top2bottom.w, top2bottom.h + 2, Color(48, 48, 48));
		Render.OutlinedRect(top2bottom.x - 1, top2bottom.y, top2bottom.w, top2bottom.h, Color(0, 0, 0));

		Render.RectFilled(tab_area.x, tab_area.y, tab_area.w + 2, tab_area.h, Color(37, 36, 36));
		Render.Text(tab_area.x + (tab_area.w / 2) - 20, tab_area.y + (tab_area.h / 2) - 20, name.c_str(), Fonts.MenuIcons, config.MenuColor[0], true);

		/* black lines */

		Render.Line(tab_area.x, tab_area.y - 1, tab_area.x + tab_area.w, tab_area.y - 1, Color(0, 0, 0));
		Render.Line(tab_area.x, tab_area.y + tab_area.h, tab_area.x + tab_area.w, tab_area.y + tab_area.h, Color(0, 0, 0));

		/* grey lines */

		Render.Line(tab_area.x, tab_area.y, tab_area.x + tab_area.w + 1, tab_area.y, Color(48, 48, 48));
		Render.Line(tab_area.x, tab_area.y + tab_area.h - 1, tab_area.x + tab_area.w + 1, tab_area.y + tab_area.h - 1, Color(48, 48, 48));
		DrawSubtabs();
	}
}

void CTab::Update() {
	rect_t main_tab_area = rect_t(
		CMenu::Get().area.x - 75, CMenu::Get().area.y,
		75, CMenu::Get().area.h
	);

	rect_t tab_area = rect_t(
		CMenu::Get().area.x - 75,
		CMenu::Get().area.y + (index * (CMenu::Get().area.h / CMenu::Get().tabs.size())),
		75, (CMenu::Get().area.h / CMenu::Get().tabs.size())
	);

	if (tab_area.ContainsPoint(CMenu::Get().cursor()) && CMenu::Get().KeyPress(VK_LBUTTON)) {
		if (!CMenu::Get().animating && index != CMenu::Get().selectedTab) {
			CMenu::Get().animating = true;
			CMenu::Get().animateDirection = (index > CMenu::Get().selectedTab) ? TAB_ANIMATION_DOWN : TAB_ANIMATION_UP;
			CMenu::Get().animationDestination = tab_area.y;
			CMenu::Get().selectedTab = index;
		}
	}

	if (index == CMenu::Get().selectedTab) {
		UpdateSubtabs();
	}

	// controls
	if (index == CMenu::Get().selectedTab) {
		for (int c = groupBoxes.size() - 1; c >= 0; c--) {
			if (groupBoxes[c]->GetParentSub() == selectedSub)
				groupBoxes[c]->Update(CMenu::Get().area);
		}
	}
}