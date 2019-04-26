#pragma once
#include "Cheat.h"
#include "GUI.h"

extern class CGroupbox;
extern class CTab;
struct MultiDropdownItem_t;

class CSettings
{
public:

	Color MenuColor[2] = { Color(207, 69, 64), Color(195, 30, 67) };
	ButtonCode_t MenuBind = KEY_INSERT;

	CGroupbox* lastGroup;
	CTab* lastTab;

	int groupSub;
	int tabSub;

	int selectedConfig;
	std::string newConfigName;

	class VisualsSettings
	{
	public:
		/* Rendered */
		bool bEnablePlayerRender;
		bool bPlayerBox;
		bool bIsBoxDynamic;
		bool bPlayerHealth;
		bool bPlayerArmor;
		bool bPlayerDistance;
		bool bPlayerAmmobar;
		bool bHitsound;
		bool bPlayerViewAngles;
		bool bOutFOV;
		bool bNoVisRecoil;
		bool bPlayerName;
		bool bGrenadeTracers;
		bool bPenReticule;
		bool bRemoveScope;
		bool DisablePP;
		bool bDrawDormant;
		bool bForceSniperCross;
		bool bBombESP;
		bool bBombTimer;
		bool bWeaponESP;
		bool bThirdperson;
		bool bSkeleton;
		bool bSkeletonHistory;

		bool bDrawTeam;

		/* Game */
		bool bGlowTeamates;
		bool bGlowEnemies;
		bool bChamsVis;
		bool bChamsInvis;
		bool bChamsTeam;
		bool bLocalChamsReal;
		bool bLocalChamsFake;

		int iPlayerBoxType;
		int iPlayerChamsType;
		int iLocalChamsTypeReal;
		int iLocalChamsTypeFake;
		int iPlayerWeapon;
		int iThirdpersonDist = 150;

		Color cPlayerBox = Color(255, 255, 255);
		Color cPlayerName = Color(255, 255, 255);
		Color cPlayerChamsVis = Color(255, 255, 255);
		Color cPlayerChamsInvis = Color(255, 255, 255);
		Color cLocalChamsReal = Color(255, 255, 255);
		Color cLocalChamsFake = Color(255, 255, 255);

		ButtonCode_t keyThirdperson;

		std::vector<MultiDropdownItem_t>PlayerFlags;

		/* World */
		bool bNightmode;


		int iFlashAlpha = 255;

		bool bDrawWeapons;
		bool bDrawC4;

		Color cWeapons = Color(255, 255, 255);
		Color cC4 = Color(255, 255, 255);
	};
	class LegitSettings
	{

	};
	class RageSettings
	{

	};
	class SkinSettings
	{

	};
	class PlayerSettings
	{

	};
	class MiscSettings
	{
	public:
		std::string customName;
		std::string customClantag;
	};

	VisualsSettings visuals;
	MiscSettings misc;

}; extern CSettings config;