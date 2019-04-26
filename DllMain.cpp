#include "Cheat.h"

void OnDllAttach(LPVOID base)
{
#ifdef _DEBUG
	//U::AttachConsole();
	SetConsoleTitleA("[LemonHook]: Debug");
#endif
	I::Initialize();
	NetvarSys::Get().Initialize();
	InputSys::Get().Initialize();
	Hooks::Initialize();
	//Maybe make some hotkeys?
}

bool __stdcall DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hMod);
		CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)OnDllAttach, hMod, 0, 0);
		return TRUE;
	case DLL_PROCESS_DETACH:
		break;
	}
} 