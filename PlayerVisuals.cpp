#include "PlayerVisuals.h"

void CPlayerVisuals::OnPaintTraverse()
{
	if (!G::LocalPlayer)
		return;
	if (!I::Engine->IsConnected())
		return;

	this->Thirdperson();

	if (!config.visuals.bEnablePlayerRender)
		return;

	for (int i = 1; i < I::GlobalVars->maxClients; i++)
	{
		C_BasePlayer* entity = static_cast<C_BasePlayer*>(I::EntityList->GetClientEntity(i));

		if (!entity || !entity->IsAlive())
			continue;

		if (entity->m_iTeamNum() == G::LocalPlayer->m_iTeamNum() && !config.visuals.bDrawTeam)
			continue;

		if (config.visuals.iPlayerBoxType != 0)
			this->PlayerBox(entity);

		if (config.visuals.bPlayerHealth)
			this->PlayerHealth(entity);

		if (config.visuals.bPlayerName)
			this->PlayerName(entity);

		if (config.visuals.bPlayerArmor)
			this->PlayerArmor(entity);
	}
}

void CPlayerVisuals::PlayerBox(C_BasePlayer* entity)
{
	Vector top3D, pos3D, top, pos;
	pos3D = entity->m_vecOrigin();
	top3D = pos3D;
	top3D.z += (entity->m_fFlags() & FL_DUCKING) ? 56.f : 72.f;

	if (!M::WorldToScreen(top3D, top) || !M::WorldToScreen(pos3D, pos))
		return;

	const auto x = int(std::roundf(top.x));
	const auto y = int(std::roundf(top.y));
	const auto h = int(std::roundf(pos.y - top.y));
	const auto w = int(std::roundf(h * 0.25f + 1));

	switch (config.visuals.iPlayerBoxType)
	{
	case 1:
	{
		//Full Box
		if (!entity->IsDormant())
		{
			Render.RectOutlined(x - w, y, x + w, y + h, config.visuals.cPlayerBox);
			Render.RectOutlined(x - w + 1, y + 1, x + w - 1, y + h - 1, Color(10, 10, 10, 85));
			Render.RectOutlined(x - w - 1, y - 1, x + w + 1, y + h + 1, Color(10, 10, 10, 85));
		}
	}
	break;
	case 2:
	{
		//Corner
		int hor = w * 0.50f;
		int ver = w * 0.50f;

		//top left - right
		Render.Line(x - w, y, x - w + hor, y, config.visuals.cPlayerBox);
		Render.Line(x - w + 1, y + 1, x - w + hor - 1, y - 1, Color(10, 10, 10, 85));
		Render.Line(x - w - 1, y - 1, x - w + hor + 1, y + 1, Color(10, 10, 10, 85));
		//top left - down
		Render.Line(x - w, y, x - w, y + ver, config.visuals.cPlayerBox);
		Render.Line(x - w - 1, y - 1, x - w + 1, y + ver + 1, Color(10, 10, 10, 85));
		Render.Line(x - w + 1, y + 1, x - w - 1, y + ver - 1, Color(10, 10, 10, 85));

		//top right - left
		Render.Line(x + w, y, (x + w) - hor, y, config.visuals.cPlayerBox);
		Render.Line(x + w + 1, y + 1, (x + w) - hor - 1, y - 1, Color(10, 10, 10, 85));
		Render.Line(x + w - 1, y - 1, (x + w) - hor + 1, y + 1, Color(10, 10, 10, 85));
		//top right - down
		Render.Line(x + w, y, x + w, y + ver, config.visuals.cPlayerBox);
		Render.Line(x + w + 1, y + 1, x + w - 1, y + ver - 1, Color(10, 10, 10, 85));
		Render.Line(x + w - 1, y - 1, x + w + 1, y + ver + 1, Color(10, 10, 10, 85));

		//bottom left - right
		Render.Line(x - w, y + h, x - w + hor, y + h, config.visuals.cPlayerBox);
		Render.Line(x - w + 1, y + h + 1, x - w + hor - 1, y + h - 1, Color(10, 10, 10, 85));
		Render.Line(x - w - 1, y + h - 1, x - w + hor + 1, y + h + 1, Color(10, 10, 10, 85));
		//bottom left - up
		Render.Line(x - w, y + h, x - w, (y + h) - ver, config.visuals.cPlayerBox);
		Render.Line(x - w - 1, y + h - 1, x - w + 1, (y + h) - ver + 1, Color(10, 10, 10, 85));
		Render.Line(x - w + 1, y + h + 1, x - w - 1, (y + h) - ver - 1, Color(10, 10, 10, 85));

		//bottom right - left
		Render.Line(x + w, y + h, (x + w) - hor, y + h, config.visuals.cPlayerBox);
		Render.Line(x + w + 1, y + h + 1, (x + w) - hor - 1, y + h - 1, Color(10, 10, 10, 85));
		Render.Line(x + w - 1, y + h - 1, (x + w) - hor + 1, y + h + 1, Color(10, 10, 10, 85));
		//botto right - up
		Render.Line(x + w, y + h, x + w, (y + h) - ver, config.visuals.cPlayerBox);
		Render.Line(x + w - 1, y + h - 1, x + w + 1, (y + h) - ver + 1, Color(10, 10, 10, 85));
		Render.Line(x + w + 1, y + h + 1, x + w - 1, (y + h) - ver - 1, Color(10, 10, 10, 85));
	}
	break;
	case 3:
	{
		//3D
	}
	}
}

void CPlayerVisuals::PlayerHealth(C_BasePlayer* entity)
{
	Vector top3D, pos3D, top, pos;
	pos3D = entity->m_vecOrigin();
	top3D = pos3D;
	top3D.z += (entity->m_fFlags() & FL_DUCKING) ? 56.f : 72.f;

	if (!M::WorldToScreen(top3D, top) || !M::WorldToScreen(pos3D, pos))
		return;

	int height = pos.y - top.y;
	int width = height * 0.25f + 1;

	const auto x = int(std::roundf(top.x));
	const auto y = top.y;
	const auto h = pos.y - top.y;
	const auto w = int(std::roundf(h * 0.25f + 1));

	float boxHeight = pos.y - top.y;

	int hp = entity->m_iHealth();
// 	if (hp > 100)
// 		hp = 100;

	Render.RectFilled(x - w - 6, y - 1, x - w - 3, y + h + 1, Color(10, 10, 10, 85));
	Render.RectOutlined(x - w - 6, y - 1, x - w - 3, y + h + 1, Color(0, 0, 0, 95));
	Render.RectFilled(x - w - 6 + 1, y, x - w - 4, y + (boxHeight / 100) * hp, Color(167, 255, 147));
}

void CPlayerVisuals::PlayerArmor(C_BasePlayer* entity)
{
	Vector top3D, pos3D, top, pos;
	pos3D = entity->m_vecOrigin();
	top3D = pos3D;
	top3D.z += (entity->m_fFlags() & FL_DUCKING) ? 56.f : 72.f;

	if (!M::WorldToScreen(top3D, top) || !M::WorldToScreen(pos3D, pos))
		return;

	int height = pos.y - top.y;
	int width = height * 0.25f + 1;

	const auto x = int(std::roundf(top.x));
	const auto y = int(std::roundf(pos.y + 4));
	const auto h = int(std::roundf(3));
	const auto w = int(std::roundf(height * 0.25f + 1));

	float boxWidth = (pos.x - width) + (pos.x + width);

	int ap = entity->m_ArmorValue();

	Render.RectFilled(x - w - 1, y, x + w + 1, y + h, Color(10, 10, 10, 85));
	Render.RectFilled(x - w - 1, y, x + w + 1, y + h, Color(0, 0, 0, 95));
	Render.RectFilled(x - w, y + 1, (x - w) + (x + ((w / 100.f) * ap)), y + h - 1, Color(133, 143, 219));
}

void CPlayerVisuals::PlayerName(C_BasePlayer* entity)
{
	Vector top3D, pos3D, top, pos;
	pos3D = entity->m_vecOrigin();
	top3D = pos3D;
	top3D.z += (entity->m_fFlags() & FL_DUCKING) ? 56.f : 72.f;

	if (!M::WorldToScreen(top3D, top) || !M::WorldToScreen(pos3D, pos))
		return;

	const auto x = int(std::roundf(top.x));
	const auto y = int(std::roundf(top.y));
	const auto h = int(std::roundf(pos.y - top.y));
	const auto w = int(std::roundf(h * 0.25f + 1));

	player_info_t pInfo = entity->GetPlayerInfo();
	bool isBot = pInfo.fakeplayer;

	std::string playerName = pInfo.szName;
	std::string nameBot = "Bot " + playerName;
	RECT textSize = Render.GetTextSize(playerName.c_str(), Fonts.Visuals);
	RECT textSizeBot = Render.GetTextSize(nameBot.c_str(), Fonts.Visuals);

	if (!entity->IsDormant())
	{
		if (!isBot)
			Render.Text(((x + w) / 2) - (textSize.right / 2), y - 13, pInfo.szName, Fonts.Visuals, config.visuals.cPlayerName);
		else
			Render.Text(((x + w) / 2) - (textSizeBot.right / 2), y - 13, nameBot.c_str(), Fonts.Visuals, config.visuals.cPlayerName);
	}
}

void CPlayerVisuals::Thirdperson()
{
	//add distance things
	if (!G::LocalPlayer)
		return;

	if (config.visuals.bThirdperson && G::LocalPlayer->IsAlive())
	{
		if (!I::Input->m_fCameraInThirdPerson)
			I::Input->m_fCameraInThirdPerson = true;
	}
	else
		I::Input->m_fCameraInThirdPerson = false;
}