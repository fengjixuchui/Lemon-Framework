#include "AntiAim.h"

void CAntiAim::OnCreateMove(CUserCmd* cmd, bool &bSendpacket)
{
	if (!I::Engine->IsConnected())
		return;

	if (!G::LocalPlayer || !G::LocalPlayer->IsAlive())
		return;

	if (!config.rage.bEnableAA)
		return;

	if (G::LocalPlayer->m_nMoveType() == MOVETYPE_LADDER || G::LocalPlayer->m_nMoveType() == MOVETYPE_NOCLIP)
		return;

	if (cmd->buttons & IN_ATTACK)
		return;
	
	DoAntiAim(cmd, bSendpacket);
}

void CAntiAim::DoAntiAim(CUserCmd* cmd, bool &bSendpacket)
{
	bool IsMoving = G::LocalPlayer->m_vecVelocity().Length2D() > 0.1;
	bool IsInAir = !(G::LocalPlayer->m_fFlags() & FL_ONGROUND);
	bool IsStill = !IsMoving && !IsInAir;

	if (IsStill)
	{
		switch (config.rage.iAAPitchReal[0])
		{
		case 1:
			cmd->viewangles.pitch = -90.f;
			break;
		case 2:
			cmd->viewangles.pitch = 90.f;
			break;
		case 3:
			cmd->viewangles.pitch = 82.f;
			break;
		}

		switch (config.rage.iAAYawReal[0])
		{
		case 1:
			cmd->viewangles.yaw += 180.f;
			break;
		case 2:
			cmd->viewangles.yaw -= 180.f;
			break;
		case 3:
			cmd->viewangles.yaw = fmodf(I::GlobalVars->tickcount * config.rage.iSpinSpeed, 360.f);
			break;
		}
	}
	else if (IsMoving)
	{
		switch (config.rage.iAAPitchReal[1])
		{
		case 1:
			cmd->viewangles.pitch = -90.f;
			break;
		case 2:
			cmd->viewangles.pitch = 90.f;
			break;
		case 3:
			cmd->viewangles.pitch = 82.f;
			break;
		}

		switch (config.rage.iAAYawReal[1])
		{
		case 1:
			cmd->viewangles.yaw += 180.f;
			break;
		case 2:
			cmd->viewangles.yaw -= 180.f;
			break;
		case 3:
			cmd->viewangles.yaw = fmodf(I::GlobalVars->tickcount * config.rage.iSpinSpeed, 360.f);
			break;
		}
	}
	else if (IsInAir)
	{
		switch (config.rage.iAAPitchReal[2])
		{
		case 1:
			cmd->viewangles.pitch = -90.f;
			break;
		case 2:
			cmd->viewangles.pitch = 90.f;
			break;
		case 3:
			cmd->viewangles.pitch = 82.f;
			break;
		}

		switch (config.rage.iAAYawReal[2])
		{
		case 2:
			cmd->viewangles.yaw -= 180.f;
			break;
		case 3:
			cmd->viewangles.yaw = fmodf(I::GlobalVars->tickcount * config.rage.iSpinSpeed, 360.f);
			break;
		}
	}
}