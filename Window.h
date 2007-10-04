#pragma once
#include <Windows.h>

class Window
{
public:
	Window();
	~Window();

	void SetSize(int width, int height);
	static LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	const HWND GetHandler() { return m_hAlternativeHWnd; }

private:

	//*****************************************************
	//	Members
	//	-------
	WNDCLASSEX m_wcAlternativeWC;
	HWND m_hAlternativeHWnd;
};
