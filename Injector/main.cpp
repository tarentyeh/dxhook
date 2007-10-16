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

int main()
{
	////////////////////////////////////
	// Launch The Process
	////////////////////////////////////
	STARTUPINFO si = {sizeof(STARTUPINFO)};
	PROCESS_INFORMATION pi = {0};

	//	Run the application with our dll
	/*BOOL bResult = DetourCreateProcessWithDll(L"D:\\Games\\World of Warcraft\\wow.exe", NULL, 0, 0, TRUE,
		CREATE_DEFAULT_ERROR_MODE | CREATE_NEW_CONSOLE, NULL,
		L"D:\\Games\\World of Warcraft",&si, &pi, L"D:\\Technion\\Semester 6\\Graphics Project\\Wrapper\\DXWrapper\\Release\\dxhook.dll" , 0 );*/

	BOOL bResult = DetourCreateProcessWithDll(L"D:\\Games\\Loki\\Loki.exe", NULL, 0, 0, TRUE,
		CREATE_DEFAULT_ERROR_MODE | CREATE_NEW_CONSOLE, NULL,
		L"D:\\Games\\Loki",&si, &pi, L"D:\\Technion\\Semester 6\\Graphics Project\\Wrapper\\DXWrapper\\Release\\dxhook.dll" , 0 );

	printf("Attached is %s.", (bResult)?"successful":"unsuccessful");
	getch();
	return bResult;
}