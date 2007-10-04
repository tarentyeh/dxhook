#include "Window.h"
#include <fstream>
#include <iostream>
using std::ofstream;

Window::Window(void)
{
	WNDCLASSEX tmp = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		L"AltView", NULL };

	m_wcAlternativeWC = tmp;

	RegisterClassEx( &m_wcAlternativeWC );

	// Create the application's window
	m_hAlternativeHWnd = CreateWindow( L"AltView", L"AltView",
		WS_OVERLAPPEDWINDOW, 0, 0, 200, 200,
		NULL, NULL, m_wcAlternativeWC.hInstance, NULL );

	ShowWindow( m_hAlternativeHWnd, SW_SHOWDEFAULT );
	UpdateWindow( m_hAlternativeHWnd );
}

Window::~Window(void)
{
	UnregisterClass( L"AltView", m_wcAlternativeWC.hInstance);
	CloseWindow(m_hAlternativeHWnd);
}

LRESULT WINAPI Window::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	return DefWindowProc( hWnd, msg, wParam, lParam );
}

void Window::SetSize(int width, int height)
{
	SetWindowPos(m_hAlternativeHWnd, HWND_TOP, 0, 0, width, height, SWP_NOZORDER | SWP_NOREPOSITION );
}
