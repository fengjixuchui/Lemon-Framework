#include "GUI.h"
#include "Menu.h"

CGroupbox::CGroupbox(GroupRow row, int y, std::string n_name, int subTabCount /* = 0 */)
{
	parent = config.lastTab;

	int x = 8;
	if (row == GROUP_RIGHT)
		x += int(std::roundf(CMenu::Get().area.w / 2));

	area = rect_t(x, y, int(std::roundf(CMenu::Get().area.w / 2)) - 26, CMenu::Get().area.h - 20);

	name = n_name;

	if (subTabCount != 0)
	{
		for (int i = 0; i < subTabCount; i++)
			subData.subTabs.push_back(i);

		subData.selectedSub = 0;

		lastPos = 23;
	}

	parentSub = config.tabSub;
	if (parentSub > -1)
	{
		area.y += 40;
		area.h -= 40;
	}

	parent->AddGroup(this);

	config.lastGroup = this;
	config.groupSub = -1;
}

void CGroupbox::Draw()
{
	Render.FilledRect(area.x, area.y, area.w + 10, area.h, Color(32, 31, 31));
	Render.OutlinedRect(area.x, area.y, area.w + 10, area.h, Color(48, 48, 48));
	Render.OutlinedRect(area.x - 1, area.y - 1, area.w + 12, area.h + 2, Color(0, 0, 0));

	RECT size = Render.GetTextSize(name.c_str(), Fonts.Menu);

	Render.FilledRect(area.x + (area.w / 2) - size.left - 7, area.y - 1, size.right + 2, 2, Color(37, 36, 36));
	Render.Text(area.x + (area.w / 2) - size.left - 5, area.y - 7, name.c_str(), Fonts.Menu, Color(153, 153, 153));
}

void CGroupbox::UpdateSubs()
{
	if (subData.subTabs.size())
	{
		bool even = !(subData.subTabs.size() % 2);
		int size = subData.subTabs.size();

		for (int i = 0; i < subData.subTabs.size(); i++)
		{
			int index = subData.subTabs[i];
			int pos_x = area.x;

			index += 1;

			if (!even)
			{
				int middleIndex = subData.subTabs[(size + 1) / 2];

				pos_x += area.w / 2;

				if (index < middleIndex)
				{
					int dta = middleIndex - index;
					pos_x -= 14 * dta;
				}

				if (index > middleIndex)
				{
					int dta = index - middleIndex;
					pos_x += 14 * dta;
				}
			}
			else
			{
				int startPos = area.x + (area.w / 2) - 8;
				startPos -= (size / 2) * 14;

				pos_x = startPos + (14 * index);
			}

			index -= 1;

			Render.CircleFilled(pos_x, area.y + 15, 4, 6, Color(0, 0, 0));
			Render.CircleFilled(pos_x, area.y + 15, 4, 5, index == subData.selectedSub ? config.MenuColor[0] : Color(59, 59, 59));

			POINT mouse;
			GetCursorPos(&mouse);

			if (CMenu::Get().KeyPress(VK_LBUTTON) && !parent->blockInput)
			{
				if (pow(mouse.x - pos_x, 2) + pow(mouse.y - (area.y + 15), 2) < pow(7, 2))
				{
					subData.selectedSub = index;
					ResetBlock();
				}
			}
		}
	}
}

void CGroupbox::Update(rect_t mparent_area)
{
	rect_t fallbackRect = area;
	area = rect_t(mparent_area.x + area.x,
		mparent_area.y + area.y,
		area.w,
		area.h
	);

	Draw();
	UpdateSubs();

	bool ResetPositions = false;
	for (int i = 0; i < controls.size(); i++)
	{
		auto & current = controls.at(i);

		if (current->sub != subData.selectedSub)
			continue;

		if (current->overrideFunc)
		{
			if (*current->overrideFunc != current->lastOverrideState)
			{
				current->lastOverrideState = current->overrideFunc;

				ResetPositions = true;
			}
		}
	}

	if (ResetPositions == true)
		ResetControlPositions();

	for (int i = controls.size() - 1; i >= 0; i--)
	{
		auto & current = controls.at(i);

		if (current->sub != subData.selectedSub)
			continue;

		rect_t flb_rect = current->area;
		current->area = rect_t(area.x + current->area.x,
			area.y + current->area.y,
			current->area.w,
			current->area.h
		);

		if ((current->overrideFunc && *current->overrideFunc) || !current->overrideFunc)
		{
			if (!parent->blockInput || current == parent->overridingControl)
				current->Update();

			current->Draw();

			if (!parent->blockInput || current == parent->overridingControl)
			{
				POINT mouse; GetCursorPos(&mouse);

				if (current->heightOffset != 0)
				{
					current->area.y += current->heightOffset;
					current->area.h -= current->heightOffset;
				}

				if (current->area.ContainsPoint(mouse))
				{
					if (CMenu::Get().KeyPress(VK_LBUTTON))
					{
						current->LClick();
					}
				}
			}

			if (current->flags.size())
			{
				for (int j = current->flags.size() - 1; i >= 0; j--)
				{
					auto & flag = current->flags.at(j);

					rect_t flbFlagRect = flag->area;

					flag->area = rect_t(area.x + flag->area.x,
						area.y + flag->area.y,
						flag->area.w,
						flag->area.h);

					if (!parent->blockInput || flag == parent->overridingFlag)
					{
						flag->Update();
					}

					flag->Draw();

					if (!parent->blockInput || flag == parent->overridingFlag)
					{
						POINT mouse; GetCursorPos(&mouse);

						if (flag->area.ContainsPoint(mouse))
						{
							if (CMenu::Get().KeyPress(VK_LBUTTON))
								flag->Click();
						}
					}

					flag->area = flbFlagRect;
				}
			}
		}

		current->area = flb_rect;
	}

	if (parent->resetInputBlock && parent->blockInput) {
		parent->resetInputBlock = false;
		parent->blockInput = false;
		parent->blockedControlFlag = false;
		parent->overridingControl = nullptr;
		parent->overridingControl = nullptr;
	}

	area = fallbackRect;
}

void CGroupbox::ResetControlPositions()
{
	lastPos = 8;

	std::vector<CControl*> backupControls = controls;

	controls.clear();

	for (int i = 0; i < backupControls.size(); i++)
	{
		auto & current = backupControls.at(i);

		AddElement(current, false);
	}
}

void CGroupbox::AddElement(CControl* control, bool first)
{
	controls.push_back(control);

	if (subData.subTabs.size())
	{
		int current = first ? config.groupSub : control->sub;

		if (subData.lastSub != current)
		{
			lastPos = 23;
			subData.lastSub = current;
		}
	}

	float flagsOffset = 0.f;

	if (control->flags.size())
	{
		int nextFlagPos = 8 + area.w - 16;

		for (int i = 0; i < control->flags.size(); i++)
		{
			auto & flag = control->flags.at(i);

			if (!control->heightOffset)
				flagsOffset += flag->area.w + 2;

			nextFlagPos -= flag->area.w;

			flag->area.x = nextFlagPos;

			int flag_y = lastPos;

			if (control->heightOffset)
				flag_y += (control->heightOffset / 2) - 4;
			else
				flag_y += (control->area.h / 2) - 4;

			flag->area.y = flag_y;

			nextFlagPos -= 2;
		}
	}

	control->area = rect_t(8, lastPos, area.w - 16 - flagsOffset, control->area.h);
	if (config.groupSub != -1)
		control->sub = config.groupSub;

	lastPos += control->area.h + 4;
}