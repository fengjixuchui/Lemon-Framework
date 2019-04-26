#include "ConfigManager.h"
#include <ShlObj.h>

CConfigManager* cfgManager = new CConfigManager();

void CConfigManager::Setup()
{
	SetupItem(&config.visuals.bEnablePlayerRender, false, "Enabled_Player_Rendering");
	SetupItem(&config.visuals.iPlayerBoxType, 0, "Player_Box");
	SetupItem(&config.visuals.bPlayerName, false, "Player_Name");
	SetupItem(&config.visuals.bPlayerHealth, false, "Player_Health");
	SetupItem(&config.visuals.bPlayerArmor, false, "Player_Armor");
	SetupItem(&config.visuals.iPlayerWeapon, 0, "Player_Weapon");
	SetupItem(&config.visuals.PlayerFlags, { { false, "Balance" }, { false, "Armor" }, { false, "Scoped" }, { false, "Flashed" }, { false, "Resolver" } }, "Player_Flags");
	SetupItem(&config.visuals.bChamsVis, false, "Player_Chams_Visible");
	SetupItem(&config.visuals.bChamsInvis, false, "Player_Chams_Invisible");
	SetupItem(&config.visuals.iPlayerChamsType, 0, "Player_Chams_Material");
	SetupItem(&config.visuals.bLocalChamsReal, false, "Local_Chams_Real");
	SetupItem(&config.visuals.bLocalChamsFake, false, "Local_Chams_Fake");
	SetupItem(&config.visuals.iLocalChamsTypeReal, 0, "Local_Chams_Material_Real");
	SetupItem(&config.visuals.iLocalChamsTypeFake, 0, "Local_Chams_Material_Fake");
	SetupItem(&config.visuals.bThirdperson, false, "Thirdperson_Enabled");
	SetupItem(&config.visuals.keyThirdperson, KEY_NONE, "Thirdperson_Hotkey");
	SetupItem(&config.visuals.iThirdpersonDist, 150, "Thirdperson_Distance");


	SetupItem(&config.visuals.bDrawWeapons, false, "World_Dropped_Weapons");
	SetupItem(&config.visuals.bDrawC4, false, "World_Planted_C4");
	SetupItem(&config.visuals.bNightmode, false, "World_Nightmode");
	SetupItem(&config.visuals.iFlashAlpha, 255, "World_Flash_Alpha");
}

void CConfigManager::AddItem(void* pointer, const char* name, std::string type)
{
	items.push_back(new CConfigItem(std::string(name), pointer, type));
}

void CConfigManager::SetupItem(int* pointer, int value, std::string name)
{
	AddItem(pointer, name.c_str(), "int");
	*pointer = value;
}

void CConfigManager::SetupItem(bool * pointer, bool value, std::string name)
{
	AddItem(pointer, name.c_str(), "bool");
	*pointer = value;
}

void CConfigManager::SetupItem(float * pointer, float value, std::string name)
{
	AddItem(pointer, name.c_str(), "float");
	*pointer = value;
}

void CConfigManager::SetupItem(ButtonCode_t * pointer, ButtonCode_t value, std::string name)
{
	AddItem(pointer, name.c_str(), "ButtonCode");
	*pointer = value;
}

void CConfigManager::SetupItem(Color * pointer, Color value, std::string name)
{
	AddItem(pointer, name.c_str(), "Color");
	*pointer = value;
}

void CConfigManager::SetupItem(std::vector< MultiDropdownItem_t > * pointer, std::vector< MultiDropdownItem_t > value, std::string name)
{
	AddItem(pointer, name.c_str(), "vector<MultiDropdownItem_t>");

	std::vector< MultiDropdownItem_t > array = *pointer;

	pointer->clear();

	for (int i = 0; i < value.size(); i++) {
		pointer->push_back({ value[i].enabled, value[i].name });
	}
}

void CConfigManager::Save(std::string config)
{
	std::string folder, file;

	auto GetDirectory = [&folder, &file, &config]()->void
	{
		static CHAR path[MAX_PATH];
		if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, path)))
		{
			folder = std::string(path) + "\\LemonCorp\\";
			file = std::string(path) + "\\LemonCorp\\" + config;
		}

		CreateDirectoryA(folder.c_str(), NULL);
	};
	GetDirectory();

	std::ofstream ofs;
	ofs.open(file + "", std::ios::out | std::ios::trunc);

	json allJson;

	for (auto it : items)
	{
		json j;

		j["Name"] = it->name;
		j["Type"] = it->type;

		if (!it->type.compare("int"))
		{
			j["Value"] = (int)*(int*)it->pointer;
		}
		else if (!it->type.compare("float"))
		{
			j["Value"] = (float)*(float*)it->pointer;
		}
		else if (!it->type.compare("bool"))
		{
			j["Value"] = (bool)*(bool*)it->pointer;
		}
		else if (!it->type.compare("ButtonCode"))
		{
			j["Value"] = (int)*(int*)it->pointer;
		}
		else if (!it->type.compare("Color"))
		{
			//do this later
		}
		else if (!it->type.compare("vector<MultiDropDownItem_t>"))
		{
			auto& ptr = *(std::vector<MultiDropdownItem_t>*)(it->pointer);

			std::vector< std::string > a_str;
			for (int i = 0; i < ptr.size(); i++) {
				a_str.push_back(ptr[i].enabled ? "1" : "0");
			}

			json ja;
			for (auto & i : a_str) {
				ja.push_back(i);
			}

			j["Value"] = ja.dump();
		}

		allJson.push_back(j);
	}

	std::string data = allJson.dump();

	ofs << std::setw(4) << data << std::endl;

	ofs.close();
}

void CConfigManager::Load(std::string config)
{
	static auto FindItem = [](std::vector<CConfigItem*>items, std::string name)->CConfigItem*
	{
		for (int i = 0; i < (int)items.size(); i++)
		{
			if (items[i]->name.compare(name) == 0)
				return(items[i]);
		}
	};

	static auto RightOfDelim = [](std::string const& str, std::string const& delim)->std::string
	{
		return str.substr(str.find(delim) + delim.size());
	};

	std::string folder, file;

	auto GetDirectory = [&folder, &file, &config]()->void
	{
		static CHAR path[MAX_PATH];
		if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, path)))
		{
			folder = std::string(path) + "\\LemonCorp\\";
			file = std::string(path) + "\\LemonCorp\\" + config;
		}

		CreateDirectoryA(folder.c_str(), NULL);
	};
	GetDirectory();

	std::ifstream ifs;
	std::string data;

	std::string path = file + "";

	ifs.open(path);

	json allJson;

	ifs >> allJson;

	for (json::iterator it = allJson.begin(); it != allJson.end(); ++it)
	{
		json j = *it;

		std::string name = j["Name"];
		std::string type = j["Type"];

		CConfigItem* item = FindItem(items, name);

		if (item)
		{
			if (!type.compare("int"))
			{
				*(int*)item->pointer = j["Value"].get<int>();
			}
			else if (!type.compare("float"))
			{
				*(float*)item->pointer = j["Value"].get<float>();
			}
			else if (!type.compare("bool"))
			{
				*(bool*)item->pointer = j["Value"].get<bool>();
			}
			else if (!type.compare("ButtonCode"))
			{
				*(int*)item->pointer = j["Value"].get<int>();
			}
			else if (!type.compare("vector<MultiDropDown_t>"))
			{
				auto ptr = static_cast<std::vector<MultiDropdownItem_t>*> (item->pointer);
				for (int i = 0; i < ptr->size(); i++) {
					ptr->at(i).enabled = false;
				}

				json ja = json::parse(j["Value"].get<std::string>().c_str());

				std::vector < std::string > option_array;
				for (json::iterator it = ja.begin(); it != ja.end(); ++it) {
					std::string it_converted = *it;
					it_converted.erase(std::remove(it_converted.begin(), it_converted.end(), '"'), it_converted.end());
					option_array.push_back(it_converted);
				}

				for (int i = 0; i < option_array.size(); i++) {
					ptr->at(i).enabled = option_array[i].c_str()[0] == '1' ? true : false;
				}
			}
		}
	}

	ifs.close();
}

void CConfigManager::Remove(std::string config)
{
	std::string folder, file;

	auto GetDirectory = [&folder, &file, &config]()->void
	{
		static CHAR path[MAX_PATH];
		if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, path)))
		{
			folder = std::string(path) + "\\LemonCorp\\";
			file = std::string(path) + "\\LemonCorp\\" + config;
		}

		CreateDirectoryA(folder.c_str(), NULL);
	};
	GetDirectory();

	std::string path = file + "";

	std::remove(path.c_str());
}

void CConfigManager::ConfigFiles()
{
	std::string folder;

	auto GetDirectory = [&folder]()->void
	{
		static CHAR path[MAX_PATH];
		if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, path)))
		{
			folder = std::string(path) + "\\LemonCorp\\";
		}

		CreateDirectoryA(folder.c_str(), NULL);
	};
	GetDirectory();

	files.clear();

	std::string path = folder + "/*.json";

	WIN32_FIND_DATAA fd;

	HANDLE hFind = ::FindFirstFileA(path.c_str(), &fd);

	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				files.push_back(fd.cFileName);
			}
		} while (::FindNextFileA(hFind, &fd));

		::FindClose(hFind);
	}
}