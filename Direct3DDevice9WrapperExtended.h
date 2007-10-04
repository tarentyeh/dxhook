#pragma once
#include "id3d9wrapper_device.h"
#include "Direct3DDevice9WrapperExtended.h"
#include "Window.h"

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
	Direct3DDevice9WrapperExtended(IDirect3DDevice9* pDirect3DDevice9, IDirect3D9* pDirect3D9, D3DPRESENT_PARAMETERS *pPresentationParameters);
	virtual ~Direct3DDevice9WrapperExtended();

	//***********************************************************************
	//	Overridden d3d9 interface functions
	//	-----------------------------------
	STDMETHOD(Present)(THIS_ CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion);

private:

	//***********************************************************************
	//	Members section
	//	---------------
	Window m_winAltView;	
};
