#pragma once
#include "GUI.h"
#include "json.hpp"

class Color;
class C_GroupBox;
class C_Tab;
struct MultiDropdownItem_t;

using json = nlohmann::json;

class CConfigManager
{
	class CConfigItem
	{
	public:
		std::string name;
		void* pointer;
		std::string type;

		CConfigItem(std::string name, void *pointer, std::string type)
		{
			this->name = name;
			this->pointer = pointer;
			this->type = type;
		}
	};

protected:
	std::vector<CConfigItem*> items;
private:
	void AddItem(void* pointer, const char* name, std::string type);
	void SetupItem(int*, int, std::string);
	void SetupItem(bool*, bool, std::string);
	void SetupItem(float*, float, std::string);
	void SetupItem(ButtonCode_t*, ButtonCode_t, std::string);
	void SetupItem(Color*, Color, std::string);
	void SetupItem(std::vector<MultiDropdownItem_t>*, std::vector<MultiDropdownItem_t>, std::string);
public:
	CConfigManager::CConfigManager() { Setup(); }

	void Setup();
	void Save(std::string config);
	void Load(std::string config);
	void Remove(std::string config);

	std::vector<std::string> files;
	void ConfigFiles();
}; extern CConfigManager* cfgManager;

enum
{
	FLAG_BALANCE,
	FLAG_ARMOR,
	FLAG_SCOPED,
	FLAG_FLASHED,
	FLAG_FAKEDUCK,
	FLAG_RESOLVER
};

enum
{
	CHAMS_PLAYER_VISIBLE,
	CHAMS_PLAYER_INVISIBLE
};