//------------------------------------------------------------------------------------------
//	This Injector launch an application with dxhook injected to it.
//------------------------------------------------------------------------------------------
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <atlbase.h>
#include <atlstr.h>
#include <conio.h>
#include <iostream>
#include <fstream>
#include <string>
using std::cout;

#pragma comment(lib,"Detours\\detours.lib")
#include "Detours\detours.h"
	
#define GAME3

//------------------------------------------------
//	KEYS:
//---------
//	Logging -					INSERT
//	Reset selection -			DELETE
//	Switch wireframe mode -		F1
//	Select next -				F2
//	Select previous -			F3
//	Select/Deselect current -	F4

int main()
{
	////////////////////////////////////
	// Launch The Process
	////////////////////////////////////
	STARTUPINFO si = {sizeof(STARTUPINFO)};
	PROCESS_INFORMATION pi = {0};

	//	Run the application with our dll
#ifdef GAME1
	BOOL bResult = DetourCreateProcessWithDll(L"D:\\Games\\Tom Clancy's Splinter Cell Double Agent\\SCDALauncher.exe", NULL, 0, 0, TRUE,
		CREATE_DEFAULT_ERROR_MODE | CREATE_NEW_CONSOLE, NULL,
		L"D:\\Games\\Tom Clancy's Splinter Cell Double Agent",&si, &pi, L"D:\\Technion\\Semester 6\\Graphics Project\\Wrapper\\DXWrapper\\Release\\dxhook.dll" , 0 );
#endif
#ifdef GAME2
	BOOL bResult = DetourCreateProcessWithDll(L"D:\\Games\\Heroes of Might and Magic V\\bin\\H5_Game.exe", NULL, 0, 0, TRUE,
		CREATE_DEFAULT_ERROR_MODE | CREATE_NEW_CONSOLE, NULL,
		L"D:\\Games\\Heroes of Might and Magic V\\bin",&si, &pi, L"D:\\Technion\\Semester 6\\Graphics Project\\Wrapper\\DXWrapper\\Release\\dxhook.dll" , 0 );
#endif

#ifdef GAME3
	BOOL bResult = DetourCreateProcessWithDll(L"D:\\Games\\Tomb Raider - Anniversary\\tra.exe", NULL, 0, 0, TRUE,
		CREATE_DEFAULT_ERROR_MODE | CREATE_NEW_CONSOLE, NULL,
		L"D:\\Games\\Tomb Raider - Anniversary",&si, &pi, L"D:\\Technion\\Semester 6\\Graphics Project\\Wrapper\\DXWrapper\\Release\\dxhook.dll" , 0 );
#endif

	printf("Attached is %s.\n", (bResult)?"successful":"unsuccessful");
	if (!bResult)
		getch();
}