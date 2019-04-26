#include "Cheat.h"

void CWorldVisuals::OnPaintTraverse()
{
	if (!G::LocalPlayer)
		return;
	if (!I::Engine->IsConnected())
		return;

	for (int i = 0; i < I::EntityList->GetHighestEntityIndex(); i++)
	{
		C_BaseEntity* entity = static_cast<C_BaseEntity*>(I::EntityList->GetClientEntity(i));

		if (!entity || entity->IsPlayer())
			continue;

		if (config.visuals.bDrawWeapons && entity->IsWeapon())
			this->DrawWeapons((C_BaseCombatWeapon*)entity);

		if (config.visuals.bDrawC4 && entity->IsPlantedC4())
			this->DrawC4(entity);
	}
}

void CWorldVisuals::DrawWeapons(C_BaseCombatWeapon* entity)
{
	if (entity->m_hOwnerEntity().IsValid())
		return;

	Vector origin, screenOrigin;
	origin = entity->m_vecOrigin();

	if (!M::WorldToScreen(origin, screenOrigin))
		return;

	const auto x = (screenOrigin.x);
	const auto y = (screenOrigin.y);
	const auto w = 20;
	const auto h = 14;

	std::string WeaponName = entity->GetCSWeaponData()->szWeaponName + 7;
	std::transform(WeaponName.begin(), WeaponName.end(), WeaponName.begin(), toupper);
	RECT textSize = Render.GetTextSize3(Fonts.Visuals2, WeaponName.c_str());

	Render.RectOutlined(x - w, y - h, x + w, y + h, config.visuals.cWeapons);
	Render.RectOutlined(x - w - 1, y - h - 1, x + w + 1, y + h + 1, Color(10, 10, 10, 85));
	Render.RectOutlined(x - w + 1, y - h + 1 , x + w - 1, y + h - 1, Color(10, 10, 10, 85));

	Render.Text((x - w) + (w)-(textSize.right / 2), y - h - 11, WeaponName.c_str(), Fonts.Visuals2, Color(225, 225, 255));
}

void CWorldVisuals::DrawC4(C_BaseEntity* entity)
{
	Vector origin, screenOrigin;
	origin = entity->m_vecOrigin();

	if (!M::WorldToScreen(origin, screenOrigin))
		return;

	const auto x = (screenOrigin.x);
	const auto y = (screenOrigin.y);
	const auto w = 15;
	const auto h = 16;

	Render.RectOutlined(x - w, y - h, x + w, y + h, config.visuals.cC4);
	Render.RectOutlined(x - w - 1, y - h - 1, x + w + 1, y + h + 1, Color(10, 10, 10, 85));
	Render.RectOutlined(x - w + 1, y - h + 1, x + w - 1, y + h - 1, Color(10, 10, 10, 85));

	RECT textSize = Render.GetTextSize3(Fonts.Visuals2, "C4");

	Render.Text((x - w) + (w) - (textSize.right / 2), y - h - 5, "C4", Fonts.Visuals2, Color(225, 225, 255));
}