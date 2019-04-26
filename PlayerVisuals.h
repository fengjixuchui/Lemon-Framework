#pragma once
#include "Cheat.h"

class CPlayerVisuals : public Singleton<CPlayerVisuals>
{
public:
	void OnPaintTraverse();
private:
	void PlayerBox(C_BasePlayer* entity);
	void PlayerHealth(C_BasePlayer* entity);
	void PlayerArmor(C_BasePlayer* entity);
	void PlayerDistance(C_BasePlayer* entity);
	void PlayerAmmo(C_BasePlayer* entity);
	void PlayerWeapon(C_BasePlayer* entity);
	void PlayerView(C_BasePlayer* entity);
	void PlayerName(C_BasePlayer* entity);

	void Thirdperson();
};