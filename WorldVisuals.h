#pragma once
#include "Cheat.h"

class CWorldVisuals : public Singleton<CWorldVisuals>
{
public:
	void OnPaintTraverse();
private:
	void DrawWeapons(C_BaseCombatWeapon* entity);
	void DrawC4(C_BaseEntity* entity);
};