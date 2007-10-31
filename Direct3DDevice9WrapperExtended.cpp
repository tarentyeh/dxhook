#include "Direct3DDevice9WrapperExtended.h"

DWORD Direct3DDevice9WrapperExtended::m_dwWireframe = D3DFILL_SOLID;
HWND Direct3DDevice9WrapperExtended::m_GameHwnd = NULL;
WNDPROC Direct3DDevice9WrapperExtended::m_lpOldWndProc = NULL;
int Direct3DDevice9WrapperExtended::m_iSelected = 0;
int Direct3DDevice9WrapperExtended::m_iCurrent = 0;
set<int> *Direct3DDevice9WrapperExtended::m_setSelected = NULL;
bool Direct3DDevice9WrapperExtended::m_bLogCycle = false;

Direct3DDevice9WrapperExtended::Direct3DDevice9WrapperExtended(IDirect3DDevice9* pDirect3DDevice9, IDirect3D9* pDirect3D9, D3DPRESENT_PARAMETERS *pPresentationParameters, HWND hFocusWindow):
Direct3DDevice9Wrapper(pDirect3DDevice9, pDirect3D9, pPresentationParameters)
{
	m_setSelected = new set<int>();
	m_GameHwnd = hFocusWindow;
	m_bIsLogCycle = false;

	if (!m_GameHwnd){
		DXLOGALWAYS("No Game Handler :(\n");
	}

	if (!m_lpOldWndProc && m_GameHwnd){
		DXLOGALWAYS("Hooked on WndProc.\n");

		m_lpOldWndProc = (WNDPROC)(LONG_PTR)SetWindowLongPtr(m_GameHwnd, GWL_WNDPROC, (LONG)(LONG_PTR)Mine_WndProc);
	}
}

Direct3DDevice9WrapperExtended::~Direct3DDevice9WrapperExtended(void)
{
	delete m_setSelected;
}

HRESULT Direct3DDevice9WrapperExtended::Present(CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion)
{
	DXLOG("Direct3DDevice9WrapperExtended: Present\n");


	if (m_bIsLogCycle){
		m_bIsLogCycle = false;
	}

	if (m_bLogCycle){
		m_bIsLogCycle = true;
		m_bLogCycle = false;
	}

	//	First line is presenting to our window. second line's presenting to the intercepted app's window.
	//	m_winAltView.SetSize(pPresentParameters->BackBufferWidth, pPresentParameters->BackBufferHeight);
	//Direct3DDevice9->Present(pSourceRect, pDestRect, m_winAltView.GetHandler() ,pDirtyRegion);
	return Direct3DDevice9->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

HRESULT Direct3DDevice9WrapperExtended::SetRenderState(D3DRENDERSTATETYPE State,DWORD Value)
{
	char tmp[250];
	sprintf(tmp, "Direct3DDevice9WrapperExtended: SetRenderState: State = %u Value = %x\n", State, Value);
	DXLOG(tmp);
	if (State == D3DRS_FILLMODE){
		DWORD val = 0;
		Direct3DDevice9->GetRenderState(D3DRS_FILLMODE, &val);
		if (val != m_dwWireframe){
			return Direct3DDevice9->SetRenderState(D3DRS_FILLMODE, m_dwWireframe);
		}
	}

	/*if (State == D3DRS_ALPHAREF){
		return Direct3DDevice9->SetRenderState(D3DRS_ALPHAREF, 0);
	}*/

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
			case VK_F3:
				m_iSelected++;
				break;
			case VK_F2:
				m_iSelected--;
				if (m_iSelected < 0)
					m_iSelected = 0;
				break;
			case VK_F4:
				if (m_setSelected->find(m_iSelected) != m_setSelected->end())
					m_setSelected->erase(m_iSelected);
				else
					m_setSelected->insert(m_iSelected);
				break;
			case VK_F1:
				m_dwWireframe = (m_dwWireframe == D3DFILL_SOLID)? D3DFILL_WIREFRAME : D3DFILL_SOLID;
				if (Direct3DDevice9){
					Direct3DDevice9->SetRenderState(D3DRS_FILLMODE, m_dwWireframe);
				}
				break;
			case VK_DELETE:
				m_setSelected->clear();
				m_iSelected = 0;
				break;
			case VK_INSERT:
				m_bLogCycle = true;
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
	char tmp[250];
	sprintf(tmp, "Direct3DDevice9WrapperExtended: DrawIndexedPrimitive. PrimitiveType = %u, BaseVertexIndex = %d, MinVertexIndex = %u, NumVertices = %u, startIndex = %u, primCount = %u\n", PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
	DXLOG(tmp);
	if (m_iCurrent == m_iSelected || m_setSelected->find(m_iCurrent) != m_setSelected->end()){
		Direct3DDevice9->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		HRESULT res = Direct3DDevice9->DrawIndexedPrimitive( PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount); 
		Direct3DDevice9->SetRenderState(D3DRS_FILLMODE, m_dwWireframe);
		m_iCurrent++;
		return res;
	}
	m_iCurrent++;

	if(LOWORD(GetKeyState(VK_CAPITAL)) & 1) // Caps is on
		return D3D_OK;

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

HRESULT Direct3DDevice9WrapperExtended::SetVertexShader( IDirect3DVertexShader9* pShader)
{
	DXLOG("Direct3DDevice9WrapperExtended: SetVertexShader\n");
	return Direct3DDevice9->SetVertexShader(pShader);
}

HRESULT Direct3DDevice9WrapperExtended::SetVertexShaderConstantF( UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount)
{
	DXLOG("Direct3DDevice9WrapperExtended: SetVertexShaderConstantF\n");
	return Direct3DDevice9->SetVertexShaderConstantF(  StartRegister, pConstantData, Vector4fCount);
}

HRESULT Direct3DDevice9WrapperExtended::SetPixelShader( IDirect3DPixelShader9* pShader)
{
	DXLOG("Direct3DDevice9WrapperExtended: SetPixelShader\n");
	return Direct3DDevice9->SetPixelShader(  pShader);
}

HRESULT Direct3DDevice9WrapperExtended::SetPixelShaderConstantF( UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount)
{
	DXLOG("Direct3DDevice9WrapperExtended: SetPixelShaderConstantF\n");
	return Direct3DDevice9->SetPixelShaderConstantF(  StartRegister, pConstantData, Vector4fCount); 
}