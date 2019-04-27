#include "Misc.h"

void CMisc::OnCreateMove(CUserCmd* cmd)
{
	if (!I::Engine->IsConnected() && !I::Engine->IsInGame())
		return;
	if (!G::LocalPlayer)
		return;

	if (config.misc.Bunnyhop)
		this->BunnyHop(cmd);
	if (config.misc.ChatSpam != 0)
		this->ChatSpammer();
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

	if (config.misc.iChatType == 0)
	{
		
		if (GetTickCount() - LastSpammed > 800)
		{
			LastSpammed = GetTickCount();
			std::string msg = say + General[rand() % General.size()];
			I::Engine->ExecuteClientCmd(msg.c_str());
		}
	}
	else if (config.misc.iChatType == 1)
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
		if (config.misc.iChatType == 2)
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

void CMisc::FOVOverride()
{

}





