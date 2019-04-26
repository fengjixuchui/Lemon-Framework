#include "Hooks.h"
#include "Features.h"
#include "Menu.h"
#include "GUI.h"
#include <intrin.h>
#include "ConfigManager.h"
#pragma intrinsic(_ReturnAddress)  

void CorrectMovement(QAngle vOldAngles, CUserCmd* pCmd, float fOldForward, float fOldSidemove);

namespace Hooks
{
	vfunc_hook hlclient_hook;
	vfunc_hook direct3d_hook;
	vfunc_hook vguipanel_hook;
	vfunc_hook vguisurf_hook;
	vfunc_hook sound_hook;
	vfunc_hook mdlrender_hook;
	vfunc_hook clientmode_hook;
	vfunc_hook sv_cheats;
	vfunc_hook renderview_hook;
	vfunc_hook matchmaking_hook;
	vfunc_hook event_hook;

	void Initialize()
	{
		hlclient_hook.setup(I::Client);
		direct3d_hook.setup(I::D3DDevice9);
		vguipanel_hook.setup(I::VGuiPanel);
		vguisurf_hook.setup(I::Surface);
		sound_hook.setup(I::EngineSound);
		mdlrender_hook.setup(I::MdlRender);
		clientmode_hook.setup(I::ClientMode);
		renderview_hook.setup(I::RenderView);
		matchmaking_hook.setup(I::SteamMatchmaking);
		event_hook.setup(I::GameEvents);
		ConVar* sv_cheats_con = I::CVar->FindVar("sv_cheats");
		sv_cheats.setup(sv_cheats_con);

		direct3d_hook.hook_index(index::EndScene, hkEndScene);
		direct3d_hook.hook_index(index::Reset, hkReset);

		hlclient_hook.hook_index(index::FrameStageNotify, hkFrameStageNotify);
		hlclient_hook.hook_index(index::CreateMove, hkCreateMove_Proxy);

		vguipanel_hook.hook_index(index::PaintTraverse, hkPaintTraverse);

		sound_hook.hook_index(index::EmitSound1, hkEmitSound1);
		vguisurf_hook.hook_index(index::LockCursor, hkLockCursor);

		mdlrender_hook.hook_index(index::DrawModelExecute, hkDrawModelExecute);

		clientmode_hook.hook_index(index::DoPostScreenSpaceEffects, hkDoPostScreenEffects);
		clientmode_hook.hook_index(index::OverrideView, hkOverrideView);

		event_hook.hook_index(index::FireEventClientSide, hkFireEventClientSideThink);

		sv_cheats.hook_index(index::SvCheatsGetBool, hkSvCheatsGetBool);

		matchmaking_hook.hook_index(index::SendLobbyChatMessage, hkSendLobbyChatMessage);

		Fonts.Initialize();

		I::Engine->GetScreenSize(G::ScreenWidth, G::ScreenHeight);
		MainMenu();
	}
	//--------------------------------------------------------------------------------
	void Shutdown()
	{
		hlclient_hook.unhook_all();
		direct3d_hook.unhook_all();
		vguipanel_hook.unhook_all();
		vguisurf_hook.unhook_all();
		mdlrender_hook.unhook_all();
		clientmode_hook.unhook_all();
		sound_hook.unhook_all();
		sv_cheats.unhook_all();

		//Glow::Get().Shutdown();
	}
	//--------------------------------------------------------------------------------
	long __stdcall hkEndScene(IDirect3DDevice9* pDevice)
	{
		auto oEndScene = direct3d_hook.get_original<EndScene>(index::EndScene);

		static auto viewmodel_fov = I::CVar->FindVar("viewmodel_fov");
		static auto crosshair_cvar = I::CVar->FindVar("crosshair");

		viewmodel_fov->m_fnChangeCallbacks.m_Size = 0;
		//viewmodel_fov->SetValue(g_Options.viewmodel_fov); //Change this to a setting later

		//crosshair_cvar->SetValue(!(g_Options.esp_enabled && g_Options.esp_crosshair)); //This too

		DWORD colorwrite, srgbwrite;
		pDevice->GetRenderState(D3DRS_COLORWRITEENABLE, &colorwrite);
		pDevice->GetRenderState(D3DRS_SRGBWRITEENABLE, &srgbwrite);

		pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
		//removes the source engine color correction
		pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, false);

		pDevice->SetSamplerState(NULL, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
		pDevice->SetSamplerState(NULL, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
		pDevice->SetSamplerState(NULL, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP);
		pDevice->SetSamplerState(NULL, D3DSAMP_SRGBTEXTURE, NULL);

		pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, colorwrite);
		pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, srgbwrite);

		return oEndScene(pDevice);
	}
	//--------------------------------------------------------------------------------
	long __stdcall hkReset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters)
	{
		auto oReset = direct3d_hook.get_original<Reset>(index::Reset);

		//Menu::Get().OnDeviceLost();

		auto hr = oReset(device, pPresentationParameters);

		//if (hr >= 0)
		//	Menu::Get().OnDeviceReset();

		return hr;
	}
	//--------------------------------------------------------------------------------
	void __stdcall hkCreateMove(int sequence_number, float input_sample_frametime, bool active, bool& bSendPacket)
	{
		auto oCreateMove = hlclient_hook.get_original<CreateMove>(index::CreateMove);

		oCreateMove(I::Client, sequence_number, input_sample_frametime, active);

		auto cmd = I::Input->GetUserCmd(sequence_number);
		auto verified = I::Input->GetVerifiedCmd(sequence_number);

		if (!cmd || !cmd->command_number)
			return;

		M::Normalize3(cmd->viewangles);
		M::ClampAngles(cmd->viewangles);
		QAngle oldAngle;
		float oldForward;
		float oldSideMove;
		I::Engine->GetViewAngles(oldAngle);
		oldForward = cmd->forwardmove;
		oldSideMove = cmd->sidemove;
		if (G::LocalPlayer->m_nMoveType() != MOVETYPE_LADDER)
			CorrectMovement(oldAngle, cmd, oldForward, oldSideMove);

		verified->m_cmd = *cmd;
		verified->m_crc = cmd->GetChecksum();
	}

	__declspec(naked) void __stdcall hkCreateMove_Proxy(int sequence_number, float input_sample_frametime, bool active)
	{
		__asm
		{
			push ebp
			mov  ebp, esp
			push ebx
			lea  ecx, [esp]
			push ecx
			push dword ptr[active]
			push dword ptr[input_sample_frametime]
			push dword ptr[sequence_number]
			call Hooks::hkCreateMove
			pop  ebx
			pop  ebp
			retn 0Ch
		}
	}
	//--------------------------------------------------------------------------------
	void __stdcall hkPaintTraverse(vgui::VPANEL panel, bool forceRepaint, bool allowForce)
	{
		static auto panelId = vgui::VPANEL{ 0 };
		static auto oPaintTraverse = vguipanel_hook.get_original<PaintTraverse>(index::PaintTraverse);

		oPaintTraverse(I::VGuiPanel, panel, forceRepaint, allowForce);

		if (!panelId) {
			const auto panelName = I::VGuiPanel->GetName(panel);
			if (!strcmp(panelName, "FocusOverlayPanel")) {
				panelId = panel;
			}
		}
		else if (panelId == panel) {
			//Draw calls here
			CMenu::Get().Update();
			CPlayerVisuals::Get().OnPaintTraverse();
			CWorldVisuals::Get().OnPaintTraverse();

			cfgManager->ConfigFiles();
		}
	}
	//--------------------------------------------------------------------------------
	void __stdcall hkEmitSound1(IRecipientFilter& filter, int iEntIndex, int iChannel, const char* pSoundEntry, unsigned int nSoundEntryHash, const char *pSample, float flVolume, int nSeed, float flAttenuation, int iFlags, int iPitch, const Vector* pOrigin, const Vector* pDirection, void* pUtlVecOrigins, bool bUpdatePositions, float soundtime, int speakerentity, int unk) {
		static auto ofunc = sound_hook.get_original<EmitSound1>(index::EmitSound1);


		if (!strcmp(pSoundEntry, "UIPanorama.popup_accept_match_beep")) {
			static auto fnAccept = reinterpret_cast<bool(__stdcall*)(const char*)>(U::PatternScan(GetModuleHandleA("client_panorama.dll"), "55 8B EC 83 E4 F8 8B 4D 08 BA ? ? ? ? E8 ? ? ? ? 85 C0 75 12"));

			if (fnAccept) {

				fnAccept("");

				//This will flash the CSGO window on the taskbar
				//so we know a game was found (you cant hear the beep sometimes cause it auto-accepts too fast)
				FLASHWINFO fi;
				fi.cbSize = sizeof(FLASHWINFO);
				fi.hwnd = InputSys::Get().GetMainWindow();
				fi.dwFlags = FLASHW_ALL | FLASHW_TIMERNOFG;
				fi.uCount = 0;
				fi.dwTimeout = 0;
				FlashWindowEx(&fi);
			}
		}

		ofunc(I::EngineSound, filter, iEntIndex, iChannel, pSoundEntry, nSoundEntryHash, pSample, flVolume, nSeed, flAttenuation, iFlags, iPitch, pOrigin, pDirection, pUtlVecOrigins, bUpdatePositions, soundtime, speakerentity, unk);

	}
	//--------------------------------------------------------------------------------
	int __stdcall hkDoPostScreenEffects(int a1)
	{
		auto oDoPostScreenEffects = clientmode_hook.get_original<DoPostScreenEffects>(index::DoPostScreenSpaceEffects);

		return oDoPostScreenEffects(I::ClientMode, a1);
	}
	//--------------------------------------------------------------------------------
	void __stdcall hkFrameStageNotify(ClientFrameStage_t stage)
	{
		static auto ofunc = hlclient_hook.get_original<FrameStageNotify>(index::FrameStageNotify);
		//Thirdperson stuff for antiaim here, also skinchanger and no vis recoil
		if (G::LocalPlayer && G::LocalPlayer->IsAlive() && stage == FRAME_RENDER_START)
		{
// 			if (!config.bFlip)
// 				G::LocalPlayer->SetVAngles(G::RealAngle);
// 			else
// 				G::LocalPlayer->SetVAngles(G::FakeAngle);
		}
		QAngle aim_punch_old;
		QAngle view_punch_old;
		QAngle* aim_punch = nullptr;
		QAngle* view_punch = nullptr;

		ofunc(I::Client, stage);
	}
	//--------------------------------------------------------------------------------
	void __stdcall hkOverrideView(CViewSetup* vsView)
	{
		static auto ofunc = clientmode_hook.get_original<OverrideView>(index::OverrideView);
		ofunc(I::ClientMode, vsView);
	}
	//--------------------------------------------------------------------------------
	void __stdcall hkLockCursor()
	{
		static auto ofunc = vguisurf_hook.get_original<LockCursor_t>(index::LockCursor);

		if (CMenu::Get().IsActive())
		{
			I::Surface->UnlockCursor();
			I::InputSystem->ResetInputState();
			return;
		}

		ofunc(I::Surface);

	}
	//--------------------------------------------------------------------------------
	void __fastcall hkSceneEnd(void* thisptr, void* edx)
	{
		static auto oSceneEnd = renderview_hook.get_original<iSceneEnd>(index::SceneEnd);
		oSceneEnd(thisptr, edx);

		
	}
	//--------------------------------------------------------------------------------
	void __stdcall hkDrawModelExecute(IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld)
	{
		static auto ofunc = mdlrender_hook.get_original<DrawModelExecute>(index::DrawModelExecute);

		CChams::Get().DME(ctx, state, pInfo, pCustomBoneToWorld);

		ofunc(I::MdlRender, ctx, state, pInfo, pCustomBoneToWorld);

		I::MdlRender->ForcedMaterialOverride(nullptr);
	}

	bool __fastcall hkFireEventClientSideThink(void* ecx, void* edx, IGameEvent* pEvent)
	{
		static auto oFireEventClientSide = event_hook.get_original<FireEventClientSide>(index::FireEventClientSide);

		return oFireEventClientSide(ecx, pEvent);
	};

	auto dwCAM_Think = U::PatternScan(GetModuleHandleW(L"client_panorama.dll"), "85 C0 75 30 38 86");
	typedef bool(__thiscall *svc_get_bool_t)(PVOID);
	bool __fastcall hkSvCheatsGetBool(PVOID pConVar, void* edx)
	{
		static auto ofunc = sv_cheats.get_original<svc_get_bool_t>(13);
		if (!ofunc)
			return false;

		if (reinterpret_cast<DWORD>(_ReturnAddress()) == reinterpret_cast<DWORD>(dwCAM_Think))
			return true;
		return ofunc(pConVar);
	}

	bool _stdcall hkSendLobbyChatMessage(CSteamID steamIdLobby, const void* pvMsgBody, int cubMsgBody)
	{
		typedef bool(__thiscall* SendLobbyChatMessage_t)(ISteamMatchmaking*, CSteamID, const void*, int);
		static auto Original_SendLobbyChatMessage = matchmaking_hook.get_original<SendLobbyChatMessage_t>(26);

		return true;
	}
}

void CorrectMovement(QAngle vOldAngles, CUserCmd* pCmd, float fOldForward, float fOldSidemove)
{
	float deltaView = pCmd->viewangles.yaw - vOldAngles.yaw;
	float f1;
	float f2;

	if (vOldAngles.yaw < 0.f)
	{
		f1 = 360.0f + vOldAngles.yaw;
	}
	else
	{
		f1 = vOldAngles.yaw;
	}

	if (pCmd->viewangles.yaw < 0.0f)
	{
		f2 = 360.0f + pCmd->viewangles.yaw;
	}
	else
	{
		f2 = pCmd->viewangles.yaw;
	}

	if (f2 < f1)
	{
		deltaView = abs(f2 - f1);
	}
	else
	{
		deltaView = 360.0f - abs(f1 - f2);
	}
	deltaView = 360.0f - deltaView;

	pCmd->forwardmove = cos(DEG2RAD(deltaView)) * fOldForward + cos(DEG2RAD(deltaView + 90.f)) * fOldSidemove;
	pCmd->sidemove = sin(DEG2RAD(deltaView)) * fOldForward + sin(DEG2RAD(deltaView + 90.f)) * fOldSidemove;

	pCmd->buttons &= ~IN_MOVERIGHT;
	pCmd->buttons &= ~IN_MOVELEFT;
	pCmd->buttons &= ~IN_FORWARD;
	pCmd->buttons &= ~IN_BACK;
}