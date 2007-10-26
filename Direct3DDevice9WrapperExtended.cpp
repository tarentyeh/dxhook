#include "Direct3DDevice9WrapperExtended.h"

Direct3DDevice9WrapperExtended::~Direct3DDevice9WrapperExtended(void)
{
}

Direct3DDevice9WrapperExtended::Direct3DDevice9WrapperExtended(IDirect3DDevice9* pDirect3DDevice9, IDirect3D9* pDirect3D9, D3DPRESENT_PARAMETERS *pPresentationParameters, HWND hFocusWindow):
Direct3DDevice9Wrapper(pDirect3DDevice9, pDirect3D9, pPresentationParameters)
{
	m_GameHwnd = hFocusWindow;

	if (!m_GameHwnd){
		DXLOGALWAYS("No Game Handler :(\n");
	}

	if (!m_lpOldWndProc && m_GameHwnd){
		DXLOGALWAYS("Hooked on WndProc.\n");

		m_lpOldWndProc = (WNDPROC)(LONG_PTR)SetWindowLongPtr(m_GameHwnd, GWL_WNDPROC, (LONG)(LONG_PTR)Mine_WndProc);
	}
}

DWORD Direct3DDevice9WrapperExtended::m_dwWireframe = D3DFILL_SOLID;
HWND Direct3DDevice9WrapperExtended::m_GameHwnd = NULL;
WNDPROC Direct3DDevice9WrapperExtended::m_lpOldWndProc = NULL;
int Direct3DDevice9WrapperExtended::m_iSelected = 0;
int Direct3DDevice9WrapperExtended::m_iCurrent = 0;

HRESULT Direct3DDevice9WrapperExtended::Present(CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion)
{
	DXLOG("Direct3DDevice9WrapperExtended: Present\n");

	//	First line is presenting to our window. second line's presenting to the intercepted app's window.
	//	m_winAltView.SetSize(pPresentParameters->BackBufferWidth, pPresentParameters->BackBufferHeight);
	//Direct3DDevice9->Present(pSourceRect, pDestRect, m_winAltView.GetHandler() ,pDirtyRegion);
	return Direct3DDevice9->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

HRESULT Direct3DDevice9WrapperExtended::SetRenderState(D3DRENDERSTATETYPE State,DWORD Value)
{
	DXLOG("Direct3DDevice9WrapperExtended: SetRenderState\n");
	if (State == D3DRS_FILLMODE){
		DWORD val = 0;
		Direct3DDevice9->GetRenderState(D3DRS_FILLMODE, &val);
		if (val != m_dwWireframe){
			return Direct3DDevice9->SetRenderState(D3DRS_FILLMODE, m_dwWireframe);
		}
	}

	return Direct3DDevice9->SetRenderState(State, Value);
}

LRESULT CALLBACK Direct3DDevice9WrapperExtended::Mine_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if (uMsg == WM_KEYDOWN){

		if (GetKeyState(VK_SHIFT) < 0){
			switch (wParam)
			{
			}
		}
		else {	//	Pressed w/o Shift
			switch (wParam)
			{
			case VK_RIGHT:
				m_iSelected++;
				break;
			case VK_LEFT:
				m_iSelected--;
				if (m_iSelected < 0)
					m_iSelected = 0;
				break;
			case VK_UP:
				break;
			case VK_DOWN:
				break;
			case VK_F1:
				m_dwWireframe = (m_dwWireframe == D3DFILL_SOLID)? D3DFILL_WIREFRAME : D3DFILL_SOLID;
				if (Direct3DDevice9){
					Direct3DDevice9->SetRenderState(D3DRS_FILLMODE, m_dwWireframe);
				}
				break;
			}
		}
	}

	return CallWindowProc(m_lpOldWndProc, hWnd, uMsg, wParam, lParam);
}

HRESULT Direct3DDevice9WrapperExtended::BeginScene()
{
	DXLOG("Direct3DDevice9WrapperExtended: BeginScene\n");
	m_iCurrent = 0;
	return Direct3DDevice9->BeginScene();
}

HRESULT Direct3DDevice9WrapperExtended::DrawPrimitive( D3DPRIMITIVETYPE PrimitiveType,UINT StartVertex,UINT PrimitiveCount)
{
	DXLOG("Direct3DDevice9WrapperExtended: DrawPrimitive\n");
	if (m_iCurrent == m_iSelected && m_dwWireframe == D3DFILL_WIREFRAME){
		Direct3DDevice9->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		HRESULT res = Direct3DDevice9->DrawPrimitive(PrimitiveType,StartVertex,PrimitiveCount);
		Direct3DDevice9->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		m_iCurrent++;
		return res;
	}
	m_iCurrent++;
	return Direct3DDevice9->DrawPrimitive(PrimitiveType,StartVertex,PrimitiveCount);
}

HRESULT Direct3DDevice9WrapperExtended::DrawIndexedPrimitive( D3DPRIMITIVETYPE PrimitiveType,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount)
{
	DXLOG("Direct3DDevice9WrapperExtended: DrawIndexedPrimitive\n");
	if (m_iCurrent == m_iSelected && m_dwWireframe == D3DFILL_WIREFRAME){
		Direct3DDevice9->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		HRESULT res = Direct3DDevice9->DrawIndexedPrimitive( PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount); 
		Direct3DDevice9->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		m_iCurrent++;
		return res;
	}
	m_iCurrent++;
	return Direct3DDevice9->DrawIndexedPrimitive( PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount); 
}

HRESULT Direct3DDevice9WrapperExtended::DrawPrimitiveUP( D3DPRIMITIVETYPE PrimitiveType,UINT PrimitiveCount,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride)
{
	DXLOG("Direct3DDevice9WrapperExtended: DrawPrimitiveUP\n");
	if (m_iCurrent == m_iSelected && m_dwWireframe == D3DFILL_WIREFRAME){
		Direct3DDevice9->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		HRESULT res = Direct3DDevice9->DrawPrimitiveUP(  PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride); 
		Direct3DDevice9->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		m_iCurrent++;
		return res;
	}
	m_iCurrent++;
	return Direct3DDevice9->DrawPrimitiveUP(  PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride); 
}

HRESULT Direct3DDevice9WrapperExtended::DrawIndexedPrimitiveUP( D3DPRIMITIVETYPE PrimitiveType,UINT MinVertexIndex,UINT NumVertices,UINT PrimitiveCount,CONST void* pIndexData,D3DFORMAT IndexDataFormat,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride)
{
	DXLOG("Direct3DDevice9WrapperExtended: DrawIndexedPrimitiveUP\n");
	if (m_iCurrent == m_iSelected && m_dwWireframe == D3DFILL_WIREFRAME){
		Direct3DDevice9->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		HRESULT res = Direct3DDevice9->DrawIndexedPrimitiveUP( PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride); 
		Direct3DDevice9->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		m_iCurrent++;
		return res;
	}
	m_iCurrent++;
	return Direct3DDevice9->DrawIndexedPrimitiveUP( PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride); 
}