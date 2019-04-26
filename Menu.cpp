#include "Cheat.h"
#include "GUI.h"
#include <ShlObj.h>
#include "ConfigManager.h"
#include "Misc.h"

std::string GetConfigDirectory();
void LoadConfig();
void SaveConfig();
void AddConfig();
void RemoveConfig();

void MainMenu()
{
	static auto SetSub = [](int sub) -> void {
		config.groupSub = sub;
	};

	static auto SetTabSub = [](int sub) -> void {
		config.tabSub = sub;
	};

	auto VisualsTab = new CTab(&CMenu::Get(), 0, "e", "D", {CTab::subtab_t("Players", 0, Fonts.Menu), CTab::subtab_t("Other", 1, Fonts.Menu)});
	{
		SetTabSub(0); //Player
		auto RenderingVis = new CGroupbox(GROUP_LEFT, 8, "Render");
		{
			auto enable = new CCheckbox("Enable", &config.visuals.bEnablePlayerRender);
			auto box = new CDropdown("Bounding Box", &config.visuals.iPlayerBoxType, { "Off", "Full", "Corners", "3D" });
			auto name = new CCheckbox("Name", &config.visuals.bPlayerName);
			auto healthbar = new CCheckbox("Healthbar", &config.visuals.bPlayerHealth);
			auto armorbar = new CCheckbox("Armorbar", &config.visuals.bPlayerArmor);
			auto weapon = new CDropdown("Player Weapon", &config.visuals.iPlayerWeapon, { "Off", "Text", "Icon" });
			auto flags = new CMultiDropdown("Flags", &config.visuals.PlayerFlags);
		}
		auto GameVis = new CGroupbox(GROUP_RIGHT, 8, "Game");
		{

			auto chamsv = new CCheckbox("Chams Visible", &config.visuals.bChamsVis);
			auto chamsiv = new CCheckbox("Chams Invisible", &config.visuals.bChamsInvis);
			auto pmaterial = new CDropdown("Player Material", &config.visuals.iPlayerChamsType, { "Normal", "Flat", "Wireframe", "Glass", "Pulse" });
			auto lchams = new CCheckbox("Local Chams", &config.visuals.bLocalChamsReal);
			auto lmaterialreal = new CDropdown("Local Material", &config.visuals.iLocalChamsTypeReal, { "Normal", "Flat", "Wireframe", "Glass", "Pulse" });
			auto lchamsf = new CCheckbox("Fake Chams", &config.visuals.bLocalChamsFake);
			auto lmaterialfake = new CDropdown("Local Material", &config.visuals.iLocalChamsTypeFake, { "Normal", "Flat", "Wireframe", "Glass", "Pulse" });
			auto tp = new CCheckbox("Thirdperson", &config.visuals.bThirdperson);
			auto tpk = new CKeybind("Thirdperson Key", &config.visuals.keyThirdperson);
			auto tpdist = new CSliderInt("Distance", &config.visuals.iThirdpersonDist, 100, 220, "");
		}
		SetTabSub(1); //Other
		auto WorldVis = new CGroupbox(GROUP_LEFT, 8, "World");
		{
			auto wpv = new CCheckbox("Dropped Weapons", &config.visuals.bDrawWeapons);
			auto c4v = new CCheckbox("Planted C4", &config.visuals.bDrawC4);
		}
		auto MiscVis = new CGroupbox(GROUP_RIGHT, 8, "Misc");
		{
			auto nmode = new CCheckbox("Nightmode", &config.visuals.bNightmode);
			auto flasha = new CSliderInt("Flash Alpha", &config.visuals.iFlashAlpha, 0, 255, ""); //Change this to float
		}
	}

	auto LegitTab = new CTab(&CMenu::Get(), 1, "b", "H", { 
		CTab::subtab_t("Pistols", 0, Fonts.Menu),
		CTab::subtab_t("SMGs", 1, Fonts.Menu),
		CTab::subtab_t("Rifles", 2, Fonts.Menu),
		CTab::subtab_t("Scout", 3, Fonts.Menu),
		CTab::subtab_t("AWP", 4, Fonts.Menu) 
	}); 
	{

		SetTabSub(0);
		SetTabSub(1);
		SetTabSub(2);
		SetTabSub(3);
		SetTabSub(4);
	}

	auto RageTab = new CTab(&CMenu::Get(), 2, "c", "C");
	{
		//Add subtabs
	}

	auto SkinsTab = new CTab(&CMenu::Get(), 3, "a", "B");
	{

	}

	auto PlayersTab = new CTab(&CMenu::Get(), 4, "d", "E");
	{

	}

	auto MiscTab = new CTab(&CMenu::Get(), 5, "e", "G", { CTab::subtab_t("Main", 0, Fonts.Menu), CTab::subtab_t("Troll", 1, Fonts.Menu) });
	{
		SetTabSub(0); //Main
		auto Misc = new CGroupbox(GROUP_LEFT, 8, "Misc", 2);
		{
			SetSub(0);
			SetSub(1);
			new CTextField("Desired Name", &config.misc.customName);
			auto setname = new CButton("Change Name", []() { CMisc::Get().ChangeName(); });
			new CTextField("Desired Clantag", &config.misc.customClantag);
			auto setclant = new CButton("Change Clantag", []() { CMisc::Get().ChangeClantag(); });
		}
		auto Config = new CGroupbox(GROUP_RIGHT, 8, "Config");
		{
			auto cfgd = new CDropdown("Configs", &config.selectedConfig, &cfgManager->files);
			//seperator??
			auto cload = new CButton("Load", []() {LoadConfig(); });
			auto csave = new CButton("Save", []() {SaveConfig(); });
			auto cremove = new CButton("Remove", []() {RemoveConfig(); });
			new CTextField("New Config Name", &config.newConfigName);
			auto cadd = new CButton("Add New Config", []() {AddConfig(); });
		}
		SetTabSub(1); //Troll
	}
}

std::string GetConfigDirectory()
{
	std::string folder;
	static CHAR path[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, path))) {
		folder = std::string(path) + "\\LemonCorp\\";
	}

	CreateDirectoryA(folder.c_str(), NULL);

	return folder;
}

void LoadConfig()
{
	if (!cfgManager->files.size()) {
		return;
	}

	cfgManager->Load(cfgManager->files[config.selectedConfig]);
}

void SaveConfig()
{
	if (!cfgManager->files.size())
		return;

	cfgManager->Save(cfgManager->files[config.selectedConfig]);
	cfgManager->ConfigFiles();
}

void AddConfig()
{
	if (config.newConfigName.find(".json") == -1)
		config.newConfigName += ".json";

	cfgManager->Save(config.newConfigName.c_str());
	cfgManager->ConfigFiles();

	config.newConfigName = "";

	config.selectedConfig = cfgManager->files.size() - 1;
}

void RemoveConfig()
{
	if (!cfgManager->files.size()) {
		return;
	}

	cfgManager->Remove(cfgManager->files[config.selectedConfig]);
	cfgManager->ConfigFiles();

	if (config.selectedConfig > cfgManager->files.size() - 1) {
		config.selectedConfig = cfgManager->files.size() - 1;
	}
}