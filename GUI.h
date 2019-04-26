#pragma once
#include "Cheat.h"
#include "Menu.h"

class CGroupbox;
class CTab;

struct rect_t
{
	int x, y, w, h;

	rect_t() {};

	rect_t(int n_x, int n_y, int n_w, int n_h)
	{
		x = n_x;
		y = n_y;
		w = n_w;
		h = n_h;
	}

	bool ContainsPoint(POINT pnt) {
		return
			pnt.x > x &&
			pnt.y > y &&
			pnt.x < x + w &&
			pnt.y < y + h;
	}
};

enum ControlFlagTypes
{
	CONTROLFL_KEYBIND,
	CONTROLFL_COLORSELECTOR
};

class CControlFlag
{
public:
	virtual void Draw() = 0;
	virtual void Update() = 0;
	virtual void Click() = 0;

	CGroupbox* parent;

	rect_t area;
};

enum
{
	TAB_ANIMATION_UP,
	TAB_ANIMATION_DOWN
};

class CMenu
{
private:
	struct Mouse_t
	{
		POINT cursor;
		void Paint();
	};
	Mouse_t cMouse;

public:
	void Draw();
	void Update();

	CMenu();

	static CMenu & Get() {
		static CMenu instance;
		return instance;
	}

public:
	bool open = false;
	bool mouse_enable = false;
	bool dragging = false;

	void Toggle()
	{
		open = !open;
	}

	bool IsActive()
	{
		return open;
	}

	int drag_x, drag_y;

	int selectedTab = 0;
	rect_t area;

	rect_t m_pos = rect_t(area.x - 74, area.y, area.w + 74, 1);

	std::vector<CTab*>tabs;

	bool animating, animateDirection;

	int animationOffset, animationDestination;

	void AddTab(CTab* tab)
	{
		tabs.push_back(tab);
	}

	bool m_keystate[256] = { };
	bool m_oldstate[256] = { };

	bool KeyPress(int key) {
		return m_keystate[key] && !m_oldstate[key];
	}

	POINT cursor() {
		return cMouse.cursor;
	}
public:
	void PollMouse();
	void PollKeyboard();
};

class CControl
{
public:
	virtual void Draw() = 0;
	virtual void Update() = 0;
	virtual void LClick() = 0;

	std::vector<CControlFlag*> flags;
	
	CGroupbox* parent;

	rect_t area;
	int heightOffset = 0;
	int sub = -1;

	bool* overrideFunc;
	bool lastOverrideState;

	void SetOverride(bool* nOverride);
};

class CTab
{
public:
	std::string name, icon;
	int index;
	std::vector<CGroupbox*>groupBoxes;
	CMenu* parent;

	CControl* overridingControl;
	CControlFlag* overridingFlag;
	bool blockInput, resetInputBlock, blockedControlFlag;

	struct subtab_t
	{
		char* name;
		int index;
		vgui::HFont font;
		bool chuj;

		subtab_t(char* name, int index);
		subtab_t(char* name, int index, vgui::HFont _font);
	};

	enum
	{
		SUBTAB_ANIMATION_LEFT,
		SUBTAB_ANIMATION_RIGHT
	};

	bool animating, animatingDirection;
	float animationOffset, animationDestination;

	int selectedSub;
	std::vector<subtab_t>subtabs;

	void DrawSubtabs();
	void UpdateSubtabs();
public:
	void Draw();
	void Update();
	void AddGroup(CGroupbox* groupBox)
	{
		groupBoxes.push_back(groupBox);
	}

	CTab(CMenu* n_parent, int n_index, std::string n_icon, std::string n_name, std::vector<subtab_t>n_subtabs = { });
};

enum GroupRow
{
	GROUP_LEFT,
	GROUP_RIGHT
};

class CGroupbox
{
private:
	CTab* parent;
	float lastPos = 8;
	std::vector<CControl*>controls;
	rect_t area;
	std::string name;

	struct SubData_t
	{
		int selectedSub = -1;
		int lastSub;

		std::vector<int>subTabs;
	} subData;

	int parentSub;
public:
	int GetParentSub() { return parentSub; }
	CTab* GetTab() { return parent; }

	void ResetBlock() { parent->resetInputBlock = true; }

	void FocusControl(CControl* pointer)
	{
		parent->blockInput = true;
		parent->resetInputBlock = false;
		parent->blockedControlFlag = false;
		parent->overridingFlag = nullptr;
		parent->overridingControl = pointer;
	}

	void FocusSubcontrol(CControlFlag* pointer)
	{
		parent->blockInput = true;
		parent->resetInputBlock = false;
		parent->blockedControlFlag = false;
		parent->overridingFlag = pointer;
		parent->overridingControl = nullptr;
	}

public:
	void Draw();
	void Update(rect_t mparent_area);
	void UpdateSubs();

	void ResetControlPositions();
	void AddElement(CControl* newControl, bool first = true);

	CGroupbox(GroupRow row, int y, std::string n_name, int subTabCount = 0);
};

class CColorpicker : public CControlFlag
{
public:
	void Draw();
	void Update();
	void Click();
public:
	std::string label;
	Color color;
	Color * preview_color;
	float brightness = 1.0f;
	bool
		open,
		dragging;

	CColorpicker(Color * n_color, std::string n_label = "");
};

class CCheckbox : public CControl
{
public:
	void Draw();
	void Update();
	void LClick();
public:
	std::string label;
	bool* state;

	CCheckbox(std::string n_label, bool* n_state, std::vector<CControlFlag*>n_flags = {});
};

class CSliderInt : public CControl
{
public:
	void Draw();
	void Update();
	void LClick();
public:
	std::string label;
	float min, max;
	int* value;
	std::string follower;
	bool dragging;

	CSliderInt(std::string n_label, int* n_value, int n_min, int n_max, std::string n_follower, std::vector<CControlFlag*>n_flags = {});
};

class CSliderFloat : public CControl
{
public:
	void Draw();
	void Update();
	void LClick();
public:
	std::string label;
	float min, max;
	float* value;
	std::string follower;
	bool dragging;

	CSliderFloat(std::string n_label, int* n_value, int n_min, int n_max, std::string n_follower, std::vector<CControlFlag*>n_flags = {});
};

class CDropdown : public CControl
{
public:
	void Draw();
	void Update();
	void LClick();
public:
	std::string label;
	int* selected;
	std::vector<std::string> items;
	std::vector<std::string> *itemsPointer;
	bool open = false, usingPointer;

	CDropdown(std::string n_label, int * n_selected, std::vector< std::string > n_items, std::vector< CControlFlag * > n_flags = { });
	CDropdown(std::string n_label, int * n_selected, std::vector< std::string >* n_items, std::vector< CControlFlag * > n_flags = { });
};

struct MultiDropdownItem_t {
	bool enabled;
	std::string name;

	MultiDropdownItem_t(bool n_enabled, std::string n_name) {
		enabled = n_enabled;
		name = n_name;
	}
};

class CMultiDropdown : public CControl
{
public:
	void Draw();
	void Update();
	void LClick();
public:
	std::string label;
	std::vector< MultiDropdownItem_t > * items_ptr;
	bool open = false;

	CMultiDropdown(std::string n_label, std::vector< MultiDropdownItem_t >* n_items, std::vector<CControlFlag*> n_flags = { });
};

class CButton : public CControl
{
public:
	void Draw();
	void Update();
	void LClick();
public:
	std::string label;
	std::function< void() > func;

	CButton(std::string n_label, std::function< void() > n_func = { });
};

class CTextField : public CControl
{
public:
	void Draw();
	void Update();
	void LClick();
public:
	std::string label;
	std::string * input;
	bool taking_input = false;
	float next_blink;

	CTextField(std::string n_label, std::string * n_input);
};

class CKeybind : public CControl
{
public:
	void Draw();
	void Update();
	void LClick();
public:
	std::string label;
	ButtonCode_t* key;
	bool takingInput;

	CKeybind(std::string n_label, ButtonCode_t* n_key);
};

enum TextSeperatorType
{
	SEPERATOR_NORMAL,
	SEPERATOR_BOLD
};

class CTextSeperator : public CControl
{
public:
	void Draw();
	void Update();
	void LClick();
public:
	std::string label;
	TextSeperatorType type;

	CTextSeperator(std::string n_label, TextSeperatorType type, std::vector<CControlFlag*>n_flags = {});
};