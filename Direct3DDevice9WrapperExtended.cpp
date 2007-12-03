#include "Direct3DDevice9WrapperExtended.h"

DWORD Direct3DDevice9WrapperExtended::m_dwWireframe = D3DFILL_SOLID;
HWND Direct3DDevice9WrapperExtended::m_GameHwnd = NULL;
WNDPROC Direct3DDevice9WrapperExtended::m_lpOldWndProc = NULL;
set<int> *Direct3DDevice9WrapperExtended::m_setSelected = NULL;
int Direct3DDevice9WrapperExtended::m_iSelected = 0;
int Direct3DDevice9WrapperExtended::m_iCurrent = 0;
bool Direct3DDevice9WrapperExtended::m_bLogCycle = false;
bool Direct3DDevice9WrapperExtended::m_bIsUpdateSelectedCycle = false;
bool Direct3DDevice9WrapperExtended::m_bUpdateSelected = false;
bool Direct3DDevice9WrapperExtended::m_bPickedChanged = false;
bool Direct3DDevice9WrapperExtended::m_bIsPickedChangedCycle = false;

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

Direct3DDevice9WrapperExtended::~Direct3DDevice9WrapperExtended()
{
	delete m_setSelected;
}

HRESULT Direct3DDevice9WrapperExtended::Present(CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion)
{
	DXLOGCYCLE("Direct3DDevice9WrapperExtended: Present\n");	
	return Direct3DDevice9->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

HRESULT Direct3DDevice9WrapperExtended::SetRenderState(D3DRENDERSTATETYPE State,DWORD Value)
{
	//	Logging RenderState changes
	char tmp[250];
	sprintf(tmp, "Direct3DDevice9WrapperExtended: SetRenderState: State = %u Value = %x\n", State, Value);
	DXLOGCYCLE(tmp);
	return Direct3DDevice9->SetRenderState(State, Value);
}

HRESULT Direct3DDevice9WrapperExtended::BeginScene()
{
	DXLOGCYCLE("Direct3DDevice9WrapperExtended: BeginScene\n");
	StartCycle();
	return Direct3DDevice9->BeginScene();
}

HRESULT Direct3DDevice9WrapperExtended::DrawIndexedPrimitive( D3DPRIMITIVETYPE PrimitiveType,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount)
{
	char tmp[250];
	sprintf(tmp, "Direct3DDevice9WrapperExtended: DrawIndexedPrimitive. PrimitiveType = %u, BaseVertexIndex = %d, MinVertexIndex = %u, NumVertices = %u, startIndex = %u, primCount = %u\n", PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
	DXLOGCYCLE(tmp);
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

void Direct3DDevice9WrapperExtended::Lock(bool &action, bool &actionCycle)
{
	if (actionCycle)
		actionCycle = false;

	if (action){
		action = false;
		actionCycle = true;
	}
}

void Direct3DDevice9WrapperExtended::StartCycle()
{
	Lock(m_bLogCycle, m_bIsLogCycle);
	Lock(m_bUpdateSelected, m_bIsUpdateSelectedCycle);
	Lock(m_bPickedChanged, m_bIsPickedChangedCycle);
	m_iCurrent = 0;
}