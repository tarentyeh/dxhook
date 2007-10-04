//------------------------------------------------------------------------------------------
//
//	DXhook.cpp - A dll used to intercept function calls to dx9. It needs to be injected to
//				 a compiled binary file (.exe) in order to hook it correctly.
//											Implemented by Omer Ofir and Roey Ben-Haim
//				
//------------------------------------------------------------------------------------------
//
//	DXhook library was written with the help of Wiccaan's post in www.extalia.com:
//	http://www.extalia.com/forum/viewtopic.php?p=15723&sid=dd86d156d587e4d4b25ca151e929ae08
//
//------------------------------------------------------------------------------------------

#pragma comment(lib, "Detours\\detours.lib")
#include "stdafx.h"
#include "Detours\detours.h"
#include "ID3D9Wrapper.h"
#include <iostream>
#include <fstream>
using namespace std;

HANDLE HookHandle = NULL;

//------------------------------------------------------------------------------------------
//	Defining the trampoline prototype for the detour to intercept.
//	It's the assential, Direct3DCreate9, which is the basis for every DX based game.
//------------------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C"
{
#endif
	DETOUR_TRAMPOLINE(IDirect3D9 *WINAPI Real_Direct3DCreate9(UINT SDKVersion), Direct3DCreate9);
#ifdef __cplusplus
}
#endif

IDirect3D9* WINAPI Mine_Direct3DCreate9(UINT SDKVersion)
{
	IDirect3D9* Direct3D = Real_Direct3DCreate9(SDKVersion);
	IDirect3D9* Mine_Direct3D = new Direct3D9Wrapper(Direct3D);
	return Mine_Direct3D;
}

void HookAPI()
{
	DetourFunctionWithTrampoline( (PBYTE)Real_Direct3DCreate9, (PBYTE)Mine_Direct3DCreate9);
}

void UnhookAPI()
{
	DetourRemove( (PBYTE)Real_Direct3DCreate9, (PBYTE)Mine_Direct3DCreate9);
}

//------------------------------------------------------------------------------------------
//	Dll's main entry.
//------------------------------------------------------------------------------------------
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hModule);
		HookHandle = hModule;
		HookAPI();
	}
	else if (ul_reason_for_call == DLL_PROCESS_DETACH)
	{
		UnhookAPI();
	}
	return TRUE;
}