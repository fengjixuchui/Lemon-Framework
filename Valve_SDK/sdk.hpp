#pragma once

#define NOMINMAX
#include <Windows.h>

#include "Misc/Enums.hpp"
#include "Misc/vfunc.hpp"

#include "Math/VMatrix.hpp"
#include "Math/QAngle.hpp"
#include "Math/Vector.hpp"
#include "Misc/Studio.hpp"

#include "Interfaces/IAppSystem.hpp"
#include "Interfaces/IBaseClientDll.hpp"
#include "Interfaces/IClientEntity.hpp"
#include "Interfaces/IClientEntityList.hpp"
#include "Interfaces/IClientMode.hpp"
#include "Interfaces/IConVar.hpp"
#include "Interfaces/ICvar.hpp"
#include "Interfaces/IEngineTrace.hpp"
#include "Interfaces/IVEngineClient.hpp"
#include "Interfaces/IVDebugOverlay.hpp"
#include "Interfaces/ISurface.hpp"
#include "Interfaces/CInput.hpp"
#include "Interfaces/IVModelInfoClient.hpp"
#include "Interfaces/IVModelRender.hpp"
#include "Interfaces/IRenderView.hpp"
#include "Interfaces/IGameEventmanager.hpp"
#include "Interfaces/IMaterialSystem.hpp"
#include "Interfaces/IMoveHelper.hpp"
#include "Interfaces/IMDLCache.hpp"
#include "Interfaces/IPrediction.hpp"
#include "Interfaces/IPanel.hpp"
#include "Interfaces/IEngineSound.hpp"
#include "Interfaces/IViewRender.hpp"
#include "Interfaces/CClientState.hpp"
#include "Interfaces/IPhysics.hpp"
#include "Interfaces/IInputSystem.hpp"
#include "interfaces/IRefCounted.hpp"

#include "Misc/Convar.hpp"
#include "Misc/CUserCmd.hpp"
#include "Misc/glow_outline_effect.hpp"
#include "Misc/datamap.hpp"

#include "netvars.hpp"

#include "SDK.h"
#include "../ILocalize.h"

struct IDirect3DDevice9;

namespace I
{
    void Initialize();
    void Dump();
}

namespace I
{
	extern IVEngineClient*       Engine;
	extern IBaseClientDLL*       Client;
	extern IClientEntityList*    EntityList;
	extern CGlobalVarsBase*      GlobalVars;
	extern IEngineTrace*         EngineTrace;
	extern ICvar*                CVar;
	extern IPanel*               VGuiPanel;
	extern IClientMode*          ClientMode;
	extern IVDebugOverlay*       DebugOverlay;
	extern ISurface*             Surface;
	extern CInput*               Input;
	extern IVModelInfoClient*    MdlInfo;
	extern IVModelRender*        MdlRender;
	extern IVRenderView*         RenderView;
	extern IMaterialSystem*      MatSystem;
	extern IGameEventManager2*   GameEvents;
	extern IMoveHelper*          MoveHelper;
	extern IMDLCache*            MdlCache;
	extern IPrediction*          Prediction;
	extern CGameMovement*        GameMovement;
	extern IEngineSound*         EngineSound;
	extern CGlowObjectManager*   GlowObjManager;
	extern IViewRender*          ViewRender;
	extern IDirect3DDevice9*     D3DDevice9;
	extern CClientState*         ClientState;
	extern IPhysicsSurfaceProps* PhysSurface;
	extern IInputSystem*         InputSystem;
	extern ISteamClient*         SteamClient;
	extern ISteamMatchmaking*    SteamMatchmaking;
	extern ISteamUser*           SteamUser;
	extern ISteamFriends*        SteamFriends;
	extern ILocalize*            Localize;
}

template<typename... Args>
void ConMsg(const char* pMsg, Args... args)
{
    static auto import = (void(*)(const char*, ...))GetProcAddress(GetModuleHandleW(L"tier0.dll"), "?ConMsg@@YAXPBDZZ");
    return import(pMsg, args...);
}
template<typename... Args>
void ConColorMsg(const Color& clr, const char* pMsg, Args... args)
{
    static auto import = (void(*)(const Color&, const char*, ...))GetProcAddress(GetModuleHandleW(L"tier0.dll"), "?ConColorMsg@@YAXABVColor@@PBDZZ");
    return import(clr, pMsg, args...);
}

#include "Misc/EHandle.hpp"

class C_LocalPlayer
{
    friend bool operator==(const C_LocalPlayer& lhs, void* rhs);
public:
    C_LocalPlayer() : m_local(nullptr) {}

    operator bool() const { return *m_local != nullptr; }
    operator C_BasePlayer*() const { return *m_local; }

    C_BasePlayer* operator->() { return *m_local; }

private:
    C_BasePlayer** m_local;
};

namespace G
{
	extern int ScreenWidth;
	extern int ScreenHeight;

	extern QAngle RealAngle;
	extern QAngle FakeAngle;
	extern C_LocalPlayer LocalPlayer;
}

