#include "Cheat.h"
#include "GUI.h"

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
		}
		SetTabSub(1); //Other
		auto WorldVis = new CGroupbox(GROUP_LEFT, 8, "World");
		{

		}
		auto MiscVis = new CGroupbox(GROUP_RIGHT, 8, "Misc");
		{

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

	auto MiscTab = new CTab(&CMenu::Get(), 5, "e", "G");
	{

	}
}