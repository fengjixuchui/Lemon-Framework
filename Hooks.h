#pragma once
#include "Valve_SDK/sdk.hpp"
#include "Valve_SDK/csgostructs.hpp"
#include "Helpers/vfunc_hook.hpp"
#include "Helpers/math.hpp"
#include <d3d9.h>
namespace index
{
	constexpr auto EmitSound1 = 5;
	constexpr auto EmitSound2 = 6;
	constexpr auto EndScene = 42;
	constexpr auto Reset = 16;
	constexpr auto SceneEnd = 9;
	constexpr auto PaintTraverse = 41;
	constexpr auto CreateMove = 22;
	constexpr auto FrameStageNotify = 37;
	constexpr auto DrawModelExecute = 21;
	constexpr auto DoPostScreenSpaceEffects = 44;
	constexpr auto SvCheatsGetBool = 13;
	constexpr auto OverrideView = 18;
	constexpr auto LockCursor = 67;
	constexpr auto SendLobbyChatMessage = 26;
	constexpr auto FireEventClientSide = 9;
}

namespace Hooks
{
	void Initialize();
	void Shutdown();

	extern vfunc_hook hlclient_hook;
	extern vfunc_hook event_hook;
	extern vfunc_hook direct3d_hook;
	extern vfunc_hook vguipanel_hook;
	extern vfunc_hook vguisurf_hook;
	extern vfunc_hook mdlrender_hook;
	extern vfunc_hook viewrender_hook;
	extern vfunc_hook renderview_hook;
	extern vfunc_hook matchmaking_hook;

	typedef bool(__thiscall *CreateMove_t)(IClientMode*, float, CUserCmd*);
	using EndScene = long(__stdcall *)(IDirect3DDevice9*);
	using Reset = long(__stdcall *)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
	using CreateMove = void(__thiscall*)(IBaseClientDLL*, int, float, bool);
	using PaintTraverse = void(__thiscall*)(IPanel*, vgui::VPANEL, bool, bool);
	using EmitSound1 = void(__thiscall*)(void*, IRecipientFilter&, int, int, const char*, unsigned int, const char*, float, int, float, int, int, const Vector*, const Vector*, void*, bool, float, int, int);

	using FrameStageNotify = void(__thiscall*)(IBaseClientDLL*, ClientFrameStage_t);
	using PlaySound = void(__thiscall*)(ISurface*, const char* name);
	using LockCursor_t = void(__thiscall*)(ISurface*);
	using DrawModelExecute = void(__thiscall*)(IVModelRender*, IMatRenderContext*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4_t*);
	using FireEventClientSide = bool(__thiscall*)(void*, IGameEvent*);
	using DoPostScreenEffects = int(__thiscall*)(IClientMode*, int);
	using FireEvent = bool(__thiscall*)(IGameEventManager2*, IGameEvent* pEvent);
	using OverrideView = void(__thiscall*)(IClientMode*, CViewSetup*);

	using iSceneEnd = void(__fastcall*)(void*, void*);


	long __stdcall hkEndScene(IDirect3DDevice9* device);
	long __stdcall hkReset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters);
	void __fastcall hkSceneEnd(void* thisptr, void* edx);
	void __stdcall hkCreateMove(int sequence_number, float input_sample_frametime, bool active, bool& bSendPacket);
	void __stdcall hkCreateMove_Proxy(int sequence_number, float input_sample_frametime, bool active);
	void __stdcall hkPaintTraverse(vgui::VPANEL panel, bool forceRepaint, bool allowForce);
	void __stdcall hkEmitSound1(IRecipientFilter & filter, int iEntIndex, int iChannel, const char * pSoundEntry, unsigned int nSoundEntryHash, const char * pSample, float flVolume, int nSeed, float flAttenuation, int iFlags, int iPitch, const Vector * pOrigin, const Vector * pDirection, void * pUtlVecOrigins, bool bUpdatePositions, float soundtime, int speakerentity, int unk);
	void __stdcall hkPlaySound(const char* name);
	void __stdcall hkDrawModelExecute(IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld);
	void __stdcall hkFrameStageNotify(ClientFrameStage_t stage);
	void __stdcall hkOverrideView(CViewSetup * vsView);
	void __stdcall hkLockCursor();
	int  __stdcall hkDoPostScreenEffects(int a1);
	bool __fastcall hkSvCheatsGetBool(PVOID pConVar, void* edx);
	bool __stdcall hkSendLobbyChatMessage(CSteamID steamIdLobby, const void* pvMsgBody, int cubMsgBody);
	bool __fastcall hkFireEventClientSideThink(void * ecx, void * edx, IGameEvent * pEvent);
}