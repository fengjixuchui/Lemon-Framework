#pragma once
#include "Cheat.h"

class CMisc : public Singleton<CMisc>
{
public:
	void OnCreateMove(CUserCmd* cmd);
	void ChangeName();		
	void FOVOverride();//we need these here because in the menu we are intializing it as an object
	void ChangeClantagStatic();
private:
	void AnimatedClanTagChanger(CUserCmd* cmd);
	void BunnyHop(CUserCmd* cmd);
	void ChatSpammer();
	void DuckJump(CUserCmd* cmd);
	void EdgeJump(CUserCmd* cmd);
	void RevealCompetivieRanksOnScoreboard();
	void AutomaticWeapons(CUserCmd* cmd);
	void AutoStrafer(CUserCmd* cmd);
};


class Helpers : public Singleton<Helpers>
{
public:
	int GetTickBase(CUserCmd* cmd = nullptr);
	void marquee(std::string& tempstring);
};