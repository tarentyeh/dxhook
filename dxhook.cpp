//------------------------------------------------------------------------------------------
//
//	DXhook.cpp - A dll used to intercept function calls to dx9. It needs to be injected to
//				 a compiled binary file (.exe) in order to hook it correctly.
//											Implemented by Omer Ofir and Roey Ben-Haim
//				
//------------------------------------------------------------------------------------------
//
//	DXhook library was written with the help of Wiccaan's post in www.extalia.com:
//	http://www.extalia.com/forums/viewtopic.php?f=45&t=2578
//
//------------------------------------------------------------------------------------------

#pragma comment(lib, "Detours\\detours.lib")
#include "stdafx.h"
#include "Detours\detours.h"
#include "ID3D9Wrapper.h"
#include <iostream>
#include <fstream>
#include <string>
#include <xstring>
using std::ofstream;
using std::ios;
using std::string;
using std::wstring;

//	-------------------------------	Macros ----------------------------------------
#define LOGFILE(text)		{ofstream log;\
							log.open("D:\\log.txt", ios::app);\
							log << (text);\
							log.close();}

//	-------------------------------	Forward Decelerations -------------------------
LPSTR UnicodeToAnsi(LPCWSTR s);
void StyleSettings(DWORD &dwExStyle, DWORD &dwStyle, int &nWidth, int& nHeight);
LRESULT CALLBACK Mine_WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

//	-------------------------------	Global Variables ------------------------------
HANDLE	HookHandle		= NULL;
HWND	GameHwnd		= NULL;
DWORD	dwGameStyle		= NULL;
DWORD	dwGameStyleEx	= NULL;

//------------------------------------------------------------------------------------------
//	Defining the trampoline prototype for the detour to intercept.
//	It's the assential, Direct3DCreate9, which is the basis for every DX based game.
//------------------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C"
{
#endif

	//--------------------------------------------------------------------------------------
	//	DirectX detoured function
	//--------------------------------------------------------------------------------------
	DETOUR_TRAMPOLINE(IDirect3D9 *WINAPI Real_Direct3DCreate9(UINT SDKVersion), Direct3DCreate9);

	//--------------------------------------------------------------------------------------
	//	Various windows API detoured functions
	//--------------------------------------------------------------------------------------
	DETOUR_TRAMPOLINE(	HWND	WINAPI Real_CreateWindowExA		(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam), CreateWindowExA);
	DETOUR_TRAMPOLINE(	HWND	WINAPI Real_CreateWindowExW		(DWORD dwExStyle, LPCWSTR lpClassName, LPCWSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam), CreateWindowExW);
	DETOUR_TRAMPOLINE(	ATOM	WINAPI Real_RegisterClassExW	(CONST WNDCLASSEXW* wndClass), RegisterClassExW);
	DETOUR_TRAMPOLINE(	ATOM	WINAPI Real_RegisterClassExA	(CONST WNDCLASSEXA* wndClass), RegisterClassExA);
	DETOUR_TRAMPOLINE(	LONG	WINAPI Real_SetWindowLongA		(HWND hWnd, int nIndex, LONG dwNewLong), SetWindowLongA);
	DETOUR_TRAMPOLINE(	LONG	WINAPI Real_SetWindowLongW		(HWND hWnd, int nIndex, LONG dwNewLong), SetWindowLongW);

#ifdef __cplusplus
}
#endif

//	----------------------------------------------------------------------
//	Our Trampoline callbacks
//	----------------------------------------------------------------------

//	***	Direct3DCreate9 ***********************************************************
IDirect3D9* WINAPI Mine_Direct3DCreate9(UINT SDKVersion)
{
	LOGFILE("Direct3DCreate9 called.\n");
	IDirect3D9* Direct3D = Real_Direct3DCreate9(SDKVersion);
	IDirect3D9* Mine_Direct3D = new Direct3D9Wrapper(Direct3D);
	return Mine_Direct3D;
}
//	*******************************************************************************

//	*** CreateWindowExA ***********************************************************
HWND WINAPI Mine_CreateWindowExA(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	if (nWidth > 600 && nWidth < 100000 && !GameHwnd){	//	This might be our game window

		//	************************	Game window settings override		***********************
		StyleSettings(dwExStyle, dwStyle, nWidth, nHeight);

		string winName(lpClassName);

		char tmp[256];
		sprintf(tmp, "Entered CreateWindowExW. name: '%s'. size: (%d, %d)\n", winName.c_str() ,nWidth, nHeight);
		LOGFILE(tmp);

		GameHwnd = Real_CreateWindowExA(dwExStyle, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
		return GameHwnd;
	}

	return Real_CreateWindowExA(dwExStyle, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
}
//	*******************************************************************************

//	*** CreateWindowExW ***********************************************************
HWND WINAPI Mine_CreateWindowExW(DWORD dwExStyle, LPCWSTR lpClassName, LPCWSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	if (nWidth > 600 && nWidth < 100000 && !GameHwnd){	//	This might be our game window

		//	************************	Game window settings override		***********************
		StyleSettings(dwExStyle, dwStyle,nWidth, nHeight);

		string winName(UnicodeToAnsi(lpWindowName));

		char tmp[256];
		sprintf(tmp, "Entered CreateWindowExW. name: '%s'. size: (%d, %d)\n", winName.c_str() ,nWidth, nHeight);
		LOGFILE(tmp);

		winName = UnicodeToAnsi(lpClassName);
		sprintf(tmp, "Classname: '%s'.\n", winName.c_str());
		LOGFILE(tmp);

		GameHwnd = Real_CreateWindowExW(dwExStyle, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
		return GameHwnd;
	}

	return Real_CreateWindowExW(dwExStyle, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
}
//	*******************************************************************************

// ** SetWindowLongA **************************************************************
LONG WINAPI Mine_SetWindowLongA(HWND hWnd, int nIndex, LONG dwNewLong)
{
	LOGFILE("Entered SetWindowLongA.\n");
	return Real_SetWindowLongA(hWnd, nIndex, dwNewLong);
}
//	*******************************************************************************

// ** SetWindowLongW **************************************************************
LONG WINAPI Mine_SetWindowLongW(HWND hWnd, int nIndex, LONG dwNewLong)
{
	LOGFILE("Entered SetWindowLongW.\n");
	return Real_SetWindowLongW(hWnd, nIndex, dwNewLong);
}
//	*******************************************************************************

//	RegisterClassExW
//	----------------
ATOM WINAPI Mine_RegisterClassExW(WNDCLASSEXW *wndClass)
{
	if (wndClass){
		char tmp[256];
		LOGFILE("Mine_RegisterClassExW: ");

		if (wndClass->lpszClassName){
			string wndClassName(UnicodeToAnsi(wndClass->lpszClassName));
			sprintf(tmp, "className: %s", wndClassName.c_str());
			LOGFILE(tmp);
		}

		if (wndClass->lpszMenuName){
			string wndMenuName(UnicodeToAnsi(wndClass->lpszMenuName));
			sprintf(tmp, "menuName: %s", wndMenuName.c_str());
			LOGFILE(tmp);
		}

		if (wndClass->lpfnWndProc){
			LOGFILE("There's a WNDPROC.");
		}

		LOGFILE("\n");
	}
	
	return Real_RegisterClassExW(wndClass);
}
//	*******************************************************************************

//	RegisterClassExA
//	----------------
ATOM WINAPI Mine_RegisterClassExA (CONST WNDCLASSEXA* wndClass)
{
#ifdef LOG_REGISTERCLASS
	LOGFILE(Mine_RegisterClassExA, "Mine_RegisterClassExA");
#endif

	return Real_RegisterClassExA(wndClass);
}
//	*******************************************************************************

//	----------------------------------------------------------------------
//	Other hooking operations
//	----------------------------------------------------------------------
void HookAPI()
{
	LOGFILE("----------------------------- Hooking API ---------------------\n");	
	DetourFunctionWithTrampoline( (PBYTE)Real_Direct3DCreate9, (PBYTE)Mine_Direct3DCreate9);
	DetourFunctionWithTrampoline( (PBYTE)Real_CreateWindowExA,	(PBYTE)Mine_CreateWindowExA );
	DetourFunctionWithTrampoline( (PBYTE)Real_CreateWindowExW,	(PBYTE)Mine_CreateWindowExW );
	//DetourFunctionWithTrampoline( (PBYTE)Real_RegisterClassExW, (PBYTE)Mine_RegisterClassExW);
	//DetourFunctionWithTrampoline( (PBYTE)Real_RegisterClassExA, (PBYTE)Mine_RegisterClassExA);
	DetourFunctionWithTrampoline( (PBYTE)Real_CreateWindowExA,	(PBYTE)Mine_CreateWindowExA );
	DetourFunctionWithTrampoline( (PBYTE)Real_CreateWindowExW,	(PBYTE)Mine_CreateWindowExW );
}

void UnhookAPI()
{
	LOGFILE("----------------------------- Unhooking API ---------------------\n");	
	DetourRemove( (PBYTE)Real_Direct3DCreate9, (PBYTE)Mine_Direct3DCreate9);
	DetourRemove( (PBYTE)Real_CreateWindowExA,	(PBYTE)Mine_CreateWindowExA );
	DetourRemove( (PBYTE)Real_CreateWindowExW,	(PBYTE)Mine_CreateWindowExW );
	//DetourRemove( (PBYTE)Real_RegisterClassExW, (PBYTE)Mine_RegisterClassExW);
	//DetourRemove( (PBYTE)Real_RegisterClassExA, (PBYTE)Mine_RegisterClassExA);
	DetourRemove( (PBYTE)Real_CreateWindowExA,	(PBYTE)Mine_CreateWindowExA );
	DetourRemove( (PBYTE)Real_CreateWindowExW,	(PBYTE)Mine_CreateWindowExW );
}

//	----------------------------------------------------------------------
//	Helper functions
//	----------------------------------------------------------------------
LPSTR UnicodeToAnsi(LPCWSTR s)
{
	if (s==NULL) return NULL;
	int cw=lstrlenW(s);
	if (cw==0) {CHAR *psz=new CHAR[1];*psz='\0';return psz;}
	int cc=WideCharToMultiByte(CP_ACP,0,s,cw,NULL,0,NULL,NULL);
	if (cc==0) return NULL;
	CHAR *psz=new CHAR[cc+1];
	cc=WideCharToMultiByte(CP_ACP,0,s,cw,psz,cc,NULL,NULL);
	if (cc==0) {delete[] psz;return NULL;}
	psz[cc]='\0';
	return psz;
}

void StyleSettings(DWORD &dwExStyle, DWORD &dwStyle, int &nWidth, int& nHeight)
{
	dwStyle = WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_OVERLAPPEDWINDOW;
	// ** Remove Extra Settings From The Style If They Exist
	if( dwExStyle & WS_EX_CLIENTEDGE )
		dwExStyle -= WS_EX_CLIENTEDGE;

	if( dwExStyle & WS_EX_DLGMODALFRAME )
		dwExStyle -= WS_EX_DLGMODALFRAME;

	if( dwExStyle & WS_EX_WINDOWEDGE )
		dwExStyle -= WS_EX_WINDOWEDGE;

	if( dwExStyle & WS_EX_STATICEDGE )
		dwExStyle -= WS_EX_STATICEDGE;

	//nWidth = 640;
	//nHeight = 480;
}

//	----------------------------------------------------------------------
//	Dll's main entry.
//	----------------------------------------------------------------------
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