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

#define GAME2

int main()
{
	////////////////////////////////////
	// Launch The Process
	////////////////////////////////////
	STARTUPINFO si = {sizeof(STARTUPINFO)};
	PROCESS_INFORMATION pi = {0};

	//	Run the application with our dll
#ifdef GAME1
	BOOL bResult = DetourCreateProcessWithDll(L"D:\\Games\\World of Warcraft\\wow.exe", NULL, 0, 0, TRUE,
		CREATE_DEFAULT_ERROR_MODE | CREATE_NEW_CONSOLE, NULL,
		L"D:\\Games\\World of Warcraft",&si, &pi, L"D:\\Technion\\Semester 6\\Graphics Project\\Wrapper\\DXWrapper\\Release\\dxhook.dll" , 0 );
#endif
#ifdef GAME2
	BOOL bResult = DetourCreateProcessWithDll(L"D:\\Downloads\\DC\\Heroes of Might and Magic V\\bin\\H5_Game.exe", NULL, 0, 0, TRUE,
		CREATE_DEFAULT_ERROR_MODE | CREATE_NEW_CONSOLE, NULL,
		L"D:\\Downloads\\DC\\Heroes of Might and Magic V\\bin",&si, &pi, L"D:\\Technion\\Semester 6\\Graphics Project\\Wrapper\\DXWrapper\\Release\\dxhook.dll" , 0 );
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