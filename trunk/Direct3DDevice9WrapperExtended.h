#pragma once
#include "id3d9wrapper_device.h"
#include "Direct3DDevice9WrapperExtended.h"
#include "Window.h"
#include <set>
using std::set;

//-----------------------------------------------------------------------------------
//	class: Direct3DDevice9WrapperExtended
//  -------------------------------------
//	This class is derived from Direct3DDevice9Wrapper, which implements a basic 
//	interception to all direct3ddevice9's functions. This class override the desired 
//	functions only, leaving all the clumsiness of the device interface untouched, 
//	in the base class.
//-----------------------------------------------------------------------------------

class Direct3DDevice9WrapperExtended : public Direct3DDevice9Wrapper
{
public:
	Direct3DDevice9WrapperExtended(IDirect3DDevice9* pDirect3DDevice9, IDirect3D9* pDirect3D9, D3DPRESENT_PARAMETERS *pPresentationParameters, HWND hFocusWindow);
	virtual ~Direct3DDevice9WrapperExtended();

	//***********************************************************************
	//	Overridden d3d9 interface functions
	//	-----------------------------------
	STDMETHOD(Present)(THIS_ CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion);
	STDMETHOD(SetRenderState)(THIS_ D3DRENDERSTATETYPE State,DWORD Value);
	STDMETHOD(BeginScene)(THIS);
	STDMETHOD(DrawPrimitive)(THIS_ D3DPRIMITIVETYPE PrimitiveType,UINT StartVertex,UINT PrimitiveCount);
	STDMETHOD(DrawIndexedPrimitive)(THIS_ D3DPRIMITIVETYPE,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount);
	STDMETHOD(DrawPrimitiveUP)(THIS_ D3DPRIMITIVETYPE PrimitiveType,UINT PrimitiveCount,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride);
	STDMETHOD(DrawIndexedPrimitiveUP)(THIS_ D3DPRIMITIVETYPE PrimitiveType,UINT MinVertexIndex,UINT NumVertices,UINT PrimitiveCount,CONST void* pIndexData,D3DFORMAT IndexDataFormat,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride);

	static LRESULT CALLBACK Mine_WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

private:

	//***********************************************************************
	//	Members section
	//	---------------
	//Window m_winAltView;	
	static DWORD m_dwWireframe;
	static WNDPROC	m_lpOldWndProc;
	static HWND m_GameHwnd;

	static int m_iSelected;
	static int m_iCurrent;
	static set<int> *m_setSelected;
	static bool m_bLogCycle;
};
