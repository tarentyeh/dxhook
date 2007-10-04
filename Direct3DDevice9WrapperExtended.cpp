#include "Direct3DDevice9WrapperExtended.h"

Direct3DDevice9WrapperExtended::~Direct3DDevice9WrapperExtended(void)
{
}

Direct3DDevice9WrapperExtended::Direct3DDevice9WrapperExtended(IDirect3DDevice9* pDirect3DDevice9, IDirect3D9* pDirect3D9, D3DPRESENT_PARAMETERS *pPresentationParameters):
Direct3DDevice9Wrapper(pDirect3DDevice9, pDirect3D9, pPresentationParameters)
{
}

HRESULT Direct3DDevice9WrapperExtended::Present(CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion)
{
	//	First line is presenting to our window. second line's presenting to the intercepted app's window.
	//m_winAltView.SetSize(pPresentParameters->BackBufferWidth, pPresentParameters->BackBufferHeight);
	Direct3DDevice9->Present(pSourceRect, pDestRect, m_winAltView.GetHandler() ,pDirtyRegion);
	return Direct3DDevice9->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}
