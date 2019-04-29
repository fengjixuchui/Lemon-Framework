#include "Chams.h"

CChams::CChams()
{
	std::ofstream("csgo\\materials\\simple_regular.vmt") << R"#("VertexLitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "0"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";
	std::ofstream("csgo\\materials\\simple_ignorez.vmt") << R"#("VertexLitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "1"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";
	std::ofstream("csgo\\materials\\simple_flat.vmt") << R"#("UnlitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "0"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";
	std::ofstream("csgo\\materials\\simple_flat_ignorez.vmt") << R"#("UnlitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "1"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";
	std::ofstream("csgo\\materials\\simple_pulse.vmt") << R"#("UnlitGeneric"
{
	"$additive"  "1"
	"$nofog"     "1"
	"$flat"      "0"
	"$selfillum"    "1"
	"proxies"
	{
		"sine"
		{
			"sineperiod"    1.0
			"sinemin"		0.4
			"sinemax"		0.8
			"resultvar"     "$alpha"
		}
	}
}
)#";

	std::ofstream("csgo\\materials\\simple_pulse_ignorez.vmt") << R"#("UnlitGeneric"
{
	"$additive"  "1"
	"$ignorez"   "1"
	"$additive"  "1"
	"$nofog"     "1"
	"$flat"      "0"
	"$selfillum"    "1"
	"proxies"
	{
		"sine"
		{
			"sineperiod"    1.0
			"sinemin"		0.4
			"sinemax"		0.8
			"resultvar"     "$alpha"
		}
	}
}
)#";

	materialRegular = I::MatSystem->FindMaterial("simple_regular", TEXTURE_GROUP_MODEL);
	materialRegularIgnoreZ = I::MatSystem->FindMaterial("simple_ignorez", TEXTURE_GROUP_MODEL);
	materialFlatIgnoreZ = I::MatSystem->FindMaterial("simple_flat_ignorez", TEXTURE_GROUP_MODEL);
	materialFlat = I::MatSystem->FindMaterial("simple_flat", TEXTURE_GROUP_MODEL);
	materialPulse = I::MatSystem->FindMaterial("simple_pulse", TEXTURE_GROUP_MODEL);
	materialPulseIgnoreZ = I::MatSystem->FindMaterial("simple_pulse_ignorez", TEXTURE_GROUP_MODEL);
}

CChams::~CChams()
{
	std::remove("csgo\\materials\\simple_regular.vmt");
	std::remove("csgo\\materials\\simple_ignorez.vmt");
	std::remove("csgo\\materials\\simple_flat.vmt");
	std::remove("csgo\\materials\\simple_flat_ignorez.vmt");
	std::remove("csgo\\materials\\simple_pulse.vmt");
	std::remove("csgo\\materials\\simple_pulse_ignorez.vmt");
}

void CChams::OverrideMaterial(bool IgnoreZ, bool flat, bool wireframe, bool glass, bool pulse, const Color& rgba)
{
	IMaterial* material = nullptr;

	if (flat) {
		if (IgnoreZ)
			material = materialFlatIgnoreZ;
		else
			material = materialFlat;
	}
	else {
		if (IgnoreZ)
			material = materialRegularIgnoreZ;
		else
			material = materialRegular;
	}

	if (pulse)
	{
		if (IgnoreZ)
			material = materialPulseIgnoreZ;
		else
			material = materialPulse;
	}


	if (glass) {
		material = materialFlat;
		material->AlphaModulate(0.45f);
	}
	else {
		material->AlphaModulate(
			rgba.a() / 255.0f);
	}

	material->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, wireframe);
	material->ColorModulate(
		rgba.r() / 255.0f,
		rgba.g() / 255.0f,
		rgba.b() / 255.0f);

	I::MdlRender->ForcedMaterialOverride(material);
}

void CChams::DME(IMatRenderContext* ctx, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4_t* pCustomBoneToWorld)
{
	static auto fnDME = Hooks::mdlrender_hook.get_original<Hooks::DrawModelExecute>(index::DrawModelExecute);
	const auto mdl = pInfo.pModel;

	bool IsArm = strstr(mdl->szName, "arms") != nullptr;
	bool IsPlayer = strstr(mdl->szName, "models/player") != nullptr;
	bool IsSleeve = strstr(mdl->szName, "sleeve") != nullptr;

	if (IsPlayer && config.visuals.bChamsVis)
	{
		auto entity = C_BasePlayer::GetPlayerByIndex(pInfo.entity_index);

		if (entity && entity != G::LocalPlayer && entity->IsAlive())
		{
			const auto enemy = entity->m_iTeamNum() != G::LocalPlayer->m_iTeamNum();

			if (!enemy)
				return;


			if (!config.visuals.bChamsInvis)
			{
				switch (config.visuals.iPlayerChamsType)
				{
				case 0:
					OverrideMaterial(false, false, false, false, false, config.visuals.cPlayerChamsVis);
					break;
				case 1:
					OverrideMaterial(false, true, false, false, false, config.visuals.cPlayerChamsVis);
					break;
				case 2:
					OverrideMaterial(false, false, true, false, false, config.visuals.cPlayerChamsVis);
					break;
				case 3:
					OverrideMaterial(false, false, false, true, false, config.visuals.cPlayerChamsVis);
					break;
				case 4:
					OverrideMaterial(false, false, false, false, true, config.visuals.cPlayerChamsVis);
					break;
				}
			}
			else
			{
				switch (config.visuals.iPlayerChamsType)
				{
				case 0:
					OverrideMaterial(true, false, false, false, false, config.visuals.cPlayerChamsInvis);
					fnDME(I::MdlRender, ctx, state, pInfo, pCustomBoneToWorld);
					OverrideMaterial(false, false, false, false, false, config.visuals.cPlayerChamsVis);
					break;
				case 1:
					OverrideMaterial(true, true, false, false, false, config.visuals.cPlayerChamsInvis);
					fnDME(I::MdlRender, ctx, state, pInfo, pCustomBoneToWorld);
					OverrideMaterial(false, true, false, false, false, config.visuals.cPlayerChamsVis);
					break;
				case 2:
					OverrideMaterial(true, false, true, false, false, config.visuals.cPlayerChamsInvis);
					fnDME(I::MdlRender, ctx, state, pInfo, pCustomBoneToWorld);
					OverrideMaterial(false, false, true, false, false, config.visuals.cPlayerChamsVis);
					break;
				case 3:
					OverrideMaterial(true, false, false, true, false, config.visuals.cPlayerChamsInvis);
					fnDME(I::MdlRender, ctx, state, pInfo, pCustomBoneToWorld);
					OverrideMaterial(false, false, false, true, false, config.visuals.cPlayerChamsVis);
					break;
				case 4:
					OverrideMaterial(true, false, false, false, true, config.visuals.cPlayerChamsInvis);
					fnDME(I::MdlRender, ctx, state, pInfo, pCustomBoneToWorld);
					OverrideMaterial(false, false, false, false, true, config.visuals.cPlayerChamsVis);
					break;
				}
			}
		}
		else if (entity == G::LocalPlayer && config.visuals.bLocalChamsReal)
		{
			switch (config.visuals.iLocalChamsTypeReal)
			{
			case 0:
				OverrideMaterial(false, false, false, false, false, config.visuals.cLocalChamsReal);
				break;
			case 1:
				OverrideMaterial(false, true, false, false, false, config.visuals.cLocalChamsReal);
				break;
			case 2:
				OverrideMaterial(false, false, true, false, false, config.visuals.cLocalChamsReal);
				break;
			case 3:
				OverrideMaterial(false, false, false, true, false, config.visuals.cLocalChamsReal);
				break;
			case 4:
				OverrideMaterial(false, false, false, false, true, config.visuals.cLocalChamsReal);
				break;
			}
		}
	}
}