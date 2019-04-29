#include "Misc.h"
#include "Valve_SDK/math/Vector.hpp"
void CMisc::OnCreateMove(CUserCmd* cmd)
{
	if (!I::Engine->IsConnected() && !I::Engine->IsInGame())
		return;
	if (!G::LocalPlayer)
		return;

	if (config.misc.bBunnyhop)
		this->BunnyHop(cmd);
	if (config.misc.iChatType != 0)
		this->ChatSpammer();
	if (config.misc.bRevealCompetitiveRanks)
		this->RevealCompetivieRanksOnScoreboard();
	if (config.misc.bAirDuck) //not done
		this->DuckJump(cmd);
	if (config.misc.bEdgeJump)
		this->EdgeJump(cmd);
	if (config.misc.bAutomaticWeapons) //not done
		this->AutomaticWeapons(cmd);
	if (config.misc.bAirStrafe)
		this->AutoStrafer(cmd);
	if (config.misc.iClanTagSelection != 0)
		this->AnimatedClanTagChanger(cmd);
}


std::vector<std::string> TrashTalk =
{
	"Remember the name 'LemonCorp' when you lose this game.",
	"get rekt by lemonhook.xyz || lemonhook.wtf.",
	"The only thing lower than your k/d ratio is your I.Q.",
	"Your aim is so poor that people held a fundraiser for it",
	"The only thing more unreliable than you is the condom your dad used.",
	"If I jumped from your ego to your intelligence, Id die of starvation half-way down.",
};

std::vector<std::string> Lilpump
{
	"Move them bricks to the bando like, ouu",
	"Move them bricks to the bando like, ouu", "Diamond on my wrist and it look like glue",
	"Diamond on my wrist and it look like glue",
	"Walk in the club and I flex like ouu",
	"Walk in the club and I flex like ouu",
	"Maison Margiela all over my shoe", "Maison Margiela all over my shoe",
	"Pull up swerving in a coupe",
	"Pull up swerving in a coupe",
	"Pull up swerving in a coupe",
	"Pull up swerving in a coupe",
	"Move them bricks to the bando like, ouu",
	"Diamond on my wrist and it look like glue",
	"Made a couple bands in the kitchen (kitchen)",
	"Made a couple bands water whippin' (brrr)",
	"Take a nigga bitch, go missing, oou",
	"Bitch I ball like a piston, ou",
	"Bitch I flex Ric Flair, damn",
	"All they do is stare, ouu",
	"Lil Pump really don't care, no",
	"All they do is stare, ouu",
	"Lil Pump really don't care, no",
	"Told a bih lil pump yeah, ouu",
	"Everybody know I'm up next, yeah",
	"Everybody know I flex, ouu",
	"I run through these checks, ouu",
	"I flex on my ex, ouu",
	"Left 4 bands in the trap (trap)",
	"Left 4 bands in the trap (trap)",
	"Smokin' on loud pack, ouu",
	"Smokin' on loud pack, ouu",
	"Move them bricks to the bando like, ouu",
	"Move them bricks to the bando like, ouu",
	"Diamond on my wrist and it look like glue",
	"Diamond on my wrist and it look like glue",
	"Walk in the club and I flex like ouu",
	"Walk in the club and I flex like ouu",
	"Maison Margiela all over my shoe",
	"Maison Margiela all over my shoe",
	"Pull up swerving in a coupe",
	"Pull up swerving in a coupe",
	"Pull up swerving in a coupe",
	"Pull up swerving in a coupe",
	"Move them bricks to the bando like, ouu",
	"Move them bricks to the bando like, ouu",
	"Diamond on my wrist and it look like glue",
	"Diamond on my wrist and it look like glue",
	"Walk in the club and I flex like ouu",
	"Maison Margiela all over my shoe",
	"Walk in the club and I flex like ouu"
};

std::vector<std::string> General =
{
	"work in progress"
};


void CMisc::ChangeName()
{
	U::SetName(config.misc.customName.c_str());
}

void CMisc::ChangeClantagStatic()
{
	U::SetClantag(config.misc.customClantag.c_str());
}



void CMisc::BunnyHop(CUserCmd * cmd)
{
	static bool Did_Jump_Last_Tick = false;
	static bool Should_Jump_Last_Tick = false;
	if (!Did_Jump_Last_Tick && Should_Jump_Last_Tick)
	{
		Should_Jump_Last_Tick = false;
		cmd->buttons |= IN_JUMP;
	}
	else if (cmd->buttons & IN_JUMP)
	{
		if (G::LocalPlayer->m_fFlags() & FL_ONGROUND)
		{
			Did_Jump_Last_Tick = true;
			Should_Jump_Last_Tick = true;
		}
		else
		{
			cmd->buttons &= ~IN_JUMP;
			Did_Jump_Last_Tick = false;
		}
	}
	else
	{
		Did_Jump_Last_Tick = false;
		Should_Jump_Last_Tick = false;
	}




}

static DWORD LastSpammed = 0;
auto say = "say """;


void CMisc::ChatSpammer()
{

	if (config.misc.iChatType == 1)
	{
		
		if (GetTickCount() - LastSpammed > 800)
		{
			LastSpammed = GetTickCount();
			std::string msg = say + General[rand() % General.size()];
			I::Engine->ExecuteClientCmd(msg.c_str());
		}
	}
	else if (config.misc.iChatType == 2)
	{
		
		if (GetTickCount() - LastSpammed > 800)
		{
			LastSpammed = GetTickCount();
			std::string msg = say + TrashTalk[rand() % TrashTalk.size()];
			I::Engine->ExecuteClientCmd(msg.c_str());
		}
	}
	else
	{
		if (config.misc.iChatType == 3)
		{
			
			if (GetTickCount() - LastSpammed > 800)
			{
				LastSpammed = GetTickCount();
				std::string msg = say + Lilpump[rand() % TrashTalk.size()];
				I::Engine->ExecuteClientCmd(msg.c_str());
			}
		}
	}



}

void CMisc::DuckJump(CUserCmd* cmd)
{
	if (cmd->buttons | IN_JUMP)
	{
		cmd->buttons |= IN_DUCK;
	}
}

void CMisc::FOVOverride()
{
	//done in hooks
}


void CMisc::RevealCompetivieRanksOnScoreboard()
{
	U::RankRevealAll();
}


void CMisc::EdgeJump(CUserCmd* cmd)
{
	if (G::LocalPlayer->m_nMoveType() == MOVETYPE_LADDER || G::LocalPlayer->m_nMoveType() == MOVETYPE_NOCLIP)
		return;
	Vector Start, End;
	Start = G::LocalPlayer->m_vecOrigin();
	M::VectorCopy(G::LocalPlayer->m_vecOrigin(), Start);
	M::VectorCopy(Start, End);
	End.z -= 32;
	Ray_t ray;
	ray.Init(Start, End);
	trace_t trace;
	CTraceFilter filter;
	filter.pSkip = G::LocalPlayer;
	I::EngineTrace->TraceRay(ray, MASK_PLAYERSOLID_BRUSHONLY, &filter, &trace);
	if (trace.fraction == 1.0f)
	{
		cmd->buttons |= IN_JUMP;
	}
}


int Helpers::GetTickBase(CUserCmd* cmd)
{
	static int g_tick = 0;
	static CUserCmd* g_pLastCmd = nullptr;

	if (!cmd)
		return g_tick;

	if (!g_pLastCmd || g_pLastCmd->hasbeenpredicted) {
		g_tick = G::LocalPlayer->m_nTickBase();
	}
	else {
		//required due to csgo being dumb and running predicition every tick and not ig sec ~ MemZ
		++g_tick;
	}

	g_pLastCmd = cmd;
	return g_tick;
}

void CMisc::AutomaticWeapons(CUserCmd* cmd)
{
	if (!G::LocalPlayer)
		return;
	C_BaseCombatWeapon* local_weapon = G::LocalPlayer->m_hActiveWeapon().Get();
	if (!local_weapon)
		return;
	if (!local_weapon->IsPistol())					//FIX THIS
		return;
	float currenttime = G::LocalPlayer->m_nTickBase() * I::GlobalVars->interval_per_tick;
	if (currenttime >= local_weapon->m_flNextPrimaryAttack() && currenttime >= local_weapon->m_flNextPrimaryAttack())
		return;
	cmd->buttons &= (local_weapon->m_iItemDefinitionIndex() == WEAPON_REVOLVER ? IN_ATTACK2 : IN_ATTACK);
}



void CMisc::AutoStrafer(CUserCmd* cmd)
{
	if (G::LocalPlayer->m_nMoveType() == MOVETYPE_LADDER || MOVETYPE_NOCLIP || !G::LocalPlayer->IsAlive())
		return;
	if (I::InputSystem->IsButtonDown(ButtonCode_t::KEY_SPACE) ||
		I::InputSystem->IsButtonDown(ButtonCode_t::KEY_A) ||					// If we're not jumping or want to manually move out of the way/jump over an obstacle don't strafe ~MemZ
		I::InputSystem->IsButtonDown(ButtonCode_t::KEY_D) ||
		I::InputSystem->IsButtonDown(ButtonCode_t::KEY_S) ||
		I::InputSystem->IsButtonDown(ButtonCode_t::KEY_W))
		return;
	if (!(G::LocalPlayer->m_fFlags() & FL_ONGROUND))
	{
		if (cmd->mousedx > 1 || cmd->mousedx < -1)
		{
			cmd->sidemove = clamp(cmd->mousedx < 0.f ? -400.f : 400.f, -400, 400);
		}
		else
		{
			if (G::LocalPlayer->m_vecVelocity().Length2D() == 0 || G::LocalPlayer->m_vecVelocity().Length2D() == NAN || G::LocalPlayer->m_vecVelocity().Length2D() == INFINITE)
			{
				cmd->forwardmove = 400;
				return;
			}
			cmd->forwardmove = clamp(5850.f / G::LocalPlayer->m_vecVelocity().Length2D(), -400, 400);
			if (cmd->forwardmove < -400 || cmd->forwardmove > 400)
				cmd->forwardmove = 0;
			cmd->sidemove = clamp((cmd->command_number % 2) == 0 ? -400.f : 400.f, -400, 400);
			if (cmd->sidemove < -400 || cmd->sidemove > 400)
				cmd->sidemove = 0;
		}
	}
}




void CMisc::AnimatedClanTagChanger(CUserCmd* cmd)
{
	std::string tag = "www.LEMON-HOOK.xqz";

	if (!I::Engine->IsInGame())
		return;
	
	static size_t lastime = 0;

	if (config.misc.iClanTagSelection == 1)
	{
		if (GetTickCount() > lastime)
		{
			tag += tag.at(0);
			tag.erase(0, 1);
			Helpers::Get().marquee(tag);
			U::SetClantag(tag.c_str());
			lastime = GetTickCount() + 650;
		}
	}


}


void Helpers::marquee(std::string& tempstring)
{
	std::string temp_string = tempstring;
	tempstring.erase(0, 1);
	tempstring += temp_string[0];
}