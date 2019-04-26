#include "sdk.hpp"

#include "../Helpers/Utils.hpp"

namespace I
{
	IVEngineClient*       Engine = nullptr;
	IBaseClientDLL*       Client = nullptr;
	IClientEntityList*    EntityList = nullptr;
	CGlobalVarsBase*      GlobalVars = nullptr;
	IEngineTrace*         EngineTrace = nullptr;
	ICvar*                CVar = nullptr;
	IPanel*               VGuiPanel = nullptr;
	IClientMode*          ClientMode = nullptr;
	IVDebugOverlay*		  DebugOverlay = nullptr;
	ISurface*             Surface = nullptr;
	CInput*               Input = nullptr;
	IVModelInfoClient*    MdlInfo = nullptr;
	IVModelRender*        MdlRender = nullptr;
	IVRenderView*         RenderView = nullptr;
	IMaterialSystem*      MatSystem = nullptr;
	IGameEventManager2*   GameEvents = nullptr;
	IMoveHelper*          MoveHelper = nullptr;
	IMDLCache*            MdlCache = nullptr;
	IPrediction*          Prediction = nullptr;
	CGameMovement*        GameMovement = nullptr;
	IEngineSound*         EngineSound = nullptr;
	CGlowObjectManager*   GlowObjManager = nullptr;
	IViewRender*          ViewRender = nullptr;
	IDirect3DDevice9*     D3DDevice9 = nullptr;
	CClientState*         ClientState = nullptr;
	IPhysicsSurfaceProps* PhysSurface = nullptr;
	IInputSystem*         InputSystem = nullptr;
	ISteamMatchmaking*    SteamMatchmaking = nullptr;
	ISteamUser*           SteamUser = nullptr;
	ISteamFriends*        SteamFriends = nullptr;
	ISteamClient*         SteamClient = nullptr;
	ILocalize*            Localize = nullptr;
}

namespace G
{
	int ScreenWidth;
	int ScreenHeight;

	QAngle RealAngle;
	QAngle FakeAngle;
	C_LocalPlayer LocalPlayer;
}

namespace I
{
    CreateInterfaceFn get_module_factory(HMODULE module)
    {
        return reinterpret_cast<CreateInterfaceFn>(GetProcAddress(module, "CreateInterface"));
    }

    template<typename T>
    T* get_interface(CreateInterfaceFn f, const char* szInterfaceVersion)
    {
        auto result = reinterpret_cast<T*>(f(szInterfaceVersion, nullptr));

        if(!result) {
            throw std::runtime_error(std::string("[get_interface] Failed to GetOffset interface: ") + szInterfaceVersion);
        }

        return result;
    }

    void Initialize()
    {
        auto engineFactory    = get_module_factory(GetModuleHandleW(L"engine.dll"));
        auto clientFactory    = get_module_factory(GetModuleHandleW(L"client_panorama.dll"));
        auto valveStdFactory  = get_module_factory(GetModuleHandleW(L"vstdlib.dll"));
        auto vguiFactory      = get_module_factory(GetModuleHandleW(L"vguimatsurface.dll"));
        auto vgui2Factory     = get_module_factory(GetModuleHandleW(L"vgui2.dll"));
        auto matSysFactory    = get_module_factory(GetModuleHandleW(L"materialsystem.dll"));
        auto dataCacheFactory = get_module_factory(GetModuleHandleW(L"datacache.dll"));
        auto vphysicsFactory  = get_module_factory(GetModuleHandleW(L"vphysics.dll"));
        auto inputSysFactory  = get_module_factory(GetModuleHandleW(L"inputsystem.dll"));
		HMODULE steamFactory     = GetModuleHandleW(L"steam_api.dll");
		auto steamClientFactory = get_module_factory(GetModuleHandleW(L"steamclient.dll"));
		auto localizeFactory  = get_module_factory(GetModuleHandleA("localize.dll"));
        
		Client              = get_interface<IBaseClientDLL>      (clientFactory   , "VClient018");
        EntityList          = get_interface<IClientEntityList>   (clientFactory   , "VClientEntityList003");
        Prediction          = get_interface<IPrediction>         (clientFactory   , "VClientPrediction001");
        GameMovement        = get_interface<CGameMovement>       (clientFactory   , "GameMovement001");
        MdlCache            = get_interface<IMDLCache>           (dataCacheFactory, "MDLCache004");
        Engine              = get_interface<IVEngineClient>      (engineFactory   , "VEngineClient014");
        MdlInfo             = get_interface<IVModelInfoClient>   (engineFactory   , "VModelInfoClient004");
        MdlRender           = get_interface<IVModelRender>       (engineFactory   , "VEngineModel016");
        RenderView          = get_interface<IVRenderView>        (engineFactory   , "VEngineRenderView014");
        EngineTrace         = get_interface<IEngineTrace>        (engineFactory   , "EngineTraceClient004");
        DebugOverlay        = get_interface<IVDebugOverlay>      (engineFactory   , "VDebugOverlay004"); //Broken RN
        GameEvents          = get_interface<IGameEventManager2>  (engineFactory   , "GAMEEVENTSMANAGER002");
        EngineSound         = get_interface<IEngineSound>        (engineFactory   , "IEngineSoundClient003");
        MatSystem           = get_interface<IMaterialSystem>     (matSysFactory   , "VMaterialSystem080");
        CVar                = get_interface<ICvar>               (valveStdFactory , "VEngineCvar007");
        VGuiPanel           = get_interface<IPanel>              (vgui2Factory    , "VGUI_Panel009");
        Surface				= get_interface<ISurface>            (vguiFactory     , "VGUI_Surface031");
        PhysSurface         = get_interface<IPhysicsSurfaceProps>(vphysicsFactory , "VPhysicsSurfaceProps001");
        InputSystem         = get_interface<IInputSystem>        (inputSysFactory , "InputSystemVersion001");
		SteamClient         = get_interface<ISteamClient>        (steamClientFactory, "SteamClient017");
		Localize            = get_interface<ILocalize>(localizeFactory, "Localize_001");

        auto client = GetModuleHandleW(L"client_panorama.dll");
        auto engine = GetModuleHandleW(L"engine.dll");
        auto dx9api = GetModuleHandleW(L"shaderapidx9.dll");

        GlobalVars      =  **(CGlobalVarsBase***)(U::PatternScan(client, "A1 ? ? ? ? 5E 8B 40 10") + 1);
        ClientMode      =        *(IClientMode**)(U::PatternScan(client, "A1 ? ? ? ? 8B 80 ? ? ? ? 5D") + 1);
        Input           =             *(CInput**)(U::PatternScan(client, "B9 ? ? ? ? 8B 40 38 FF D0 84 C0 0F 85") + 1);
        MoveHelper      =      **(IMoveHelper***)(U::PatternScan(client, "8B 0D ? ? ? ? 8B 45 ? 51 8B D4 89 02 8B 01") + 2);
        GlowObjManager  = *(CGlowObjectManager**)(U::PatternScan(client, "0F 11 05 ? ? ? ? 83 C8 01") + 3);
        ViewRender      =        *(IViewRender**)(U::PatternScan(client, "A1 ? ? ? ? B9 ? ? ? ? C7 05 ? ? ? ? ? ? ? ? FF 10") + 1);
        D3DDevice9      = **(IDirect3DDevice9***)(U::PatternScan(dx9api, "A1 ? ? ? ? 50 8B 08 FF 51 0C") + 1);
        ClientState     =     **(CClientState***)(U::PatternScan(engine, "A1 ? ? ? ? 8B 80 ? ? ? ? C3") + 1);
		auto GetHSteamPipe = reinterpret_cast<HSteamPipe(*)()>(GetProcAddress(steamFactory, "GetHSteamPipe"));
		auto GetHSteamUser = reinterpret_cast<HSteamUser(*)()>(GetProcAddress(steamFactory, "GetHSteamUser"));
		SteamMatchmaking = SteamClient->GetISteamMatchmaking(GetHSteamUser(), GetHSteamPipe(), "SteamMatchMaking009");
		SteamFriends = SteamClient->GetISteamFriends(GetHSteamUser(), GetHSteamPipe(), "SteamFriends015");
		SteamUser = SteamClient->GetISteamUser(GetHSteamUser(), GetHSteamPipe(), "SteamUser019");

		G::LocalPlayer     = *(C_LocalPlayer*)(U::PatternScan(client, "8B 0D ? ? ? ? 83 FF FF 74 07") + 2);
    }

    void Dump()
    {
        // Ugly macros ugh
        #define STRINGIFY_IMPL(s) #s
        #define STRINGIFY(s)      STRINGIFY_IMPL(s)
        #define PRINT_INTERFACE(name) U::ConsolePrint("%-20s: %p\n", STRINGIFY(name), name)

        PRINT_INTERFACE(Client   );
        PRINT_INTERFACE(EntityList  );
        PRINT_INTERFACE(Prediction  );
        PRINT_INTERFACE(GameMovement);
        PRINT_INTERFACE(MdlCache    );
        PRINT_INTERFACE(Engine);
        PRINT_INTERFACE(MdlInfo     );
        PRINT_INTERFACE(MdlRender   );
        PRINT_INTERFACE(RenderView  );
        PRINT_INTERFACE(EngineTrace );
        PRINT_INTERFACE(DebugOverlay);
        PRINT_INTERFACE(GameEvents  );
        PRINT_INTERFACE(EngineSound );
        PRINT_INTERFACE(MatSystem   );
        PRINT_INTERFACE(CVar        );
        PRINT_INTERFACE(VGuiPanel   );
        PRINT_INTERFACE(Surface );
        PRINT_INTERFACE(PhysSurface );
        PRINT_INTERFACE(InputSystem );
    }
}