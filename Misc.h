#pragma once
#include "Cheat.h"

class CMisc : public Singleton<CMisc>
{
public:
	void OnCreateMove(CUserCmd* cmd);
	void ChangeName();						//we need these here because in the menu we are intializing it as an object
	void ChangeClantagStatic();
private:

	void BunnyHop(CUserCmd* cmd);
	void ChatSpammer();
	void FOVOverride();
	void RevealCompetivieRanksOnScoreboard();

};