#include "Glow.h"

void CGlow::Run()
{
	if (!config.visuals.bGlowEnemies)
		return;

	for (int i = 0; i < I::GlowObjManager->m_GlowObjectDefinitions.Count(); i++)
	{
		auto& GlowObject = I::GlowObjManager->m_GlowObjectDefinitions[i];
		auto entity = reinterpret_cast<C_BasePlayer*>(GlowObject.m_pEntity);

		if (GlowObject.IsUnused())
			continue;

		if (!entity || entity->IsDormant())
			continue;

		auto ClassID = entity->GetClientClass()->m_ClassID;

		GlowObject.m_nGlowStyle = config.visuals.iGlowTypePlayer;

		Color cGlow;

		switch (ClassID)
		{
		case ClassId_CCSPlayer:
		{
			bool IsEnemy = entity->m_iTeamNum() != G::LocalPlayer->m_iTeamNum();
			cGlow = IsEnemy ? config.visuals.cGlowEnemy : config.visuals.cGlowTeamate;
			if (!entity->IsAlive())
				continue;
			if (entity == G::LocalPlayer && !config.visuals.bGlowLocal)
				continue;

			if (entity == G::LocalPlayer && config.visuals.bGlowLocal)
			{
				GlowObject.m_nGlowStyle = config.visuals.iGlowTypeLocal;
			}
			else {
				GlowObject.m_nGlowStyle = config.visuals.iGlowTypePlayer;
			}
			break;
		}
		case ClassId_CChicken:
			continue;
			entity->m_bShouldGlow() = false;
			break;
		case ClassId_CBaseAnimating:
				continue;
			break;
		case ClassId_CPlantedC4:
				continue;
			break;
		default:
		{

		}
		}

		GlowObject.m_flRed = cGlow.r() / 255.0f;
		GlowObject.m_flGreen = cGlow.g() / 255.0f;
		GlowObject.m_flBlue = cGlow.b() / 255.0f;
		GlowObject.m_flAlpha = cGlow.a() / 255.0f;
		GlowObject.m_bRenderWhenOccluded = true;
		GlowObject.m_bRenderWhenUnoccluded = false;
	}
}