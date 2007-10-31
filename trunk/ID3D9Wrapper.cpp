#include "ID3D9Wrapper.h"
#include "Direct3DDevice9WrapperExtended.h"

IDirect3D9* Direct3D9Wrapper::Direct3D9 = NULL;

Direct3D9Wrapper::Direct3D9Wrapper(LPDIRECT3D9 pDirect3D)
{   
	Direct3D9 = pDirect3D;
	m_bIsLogCycle = false;
}

Direct3D9Wrapper::~Direct3D9Wrapper() {}

HRESULT Direct3D9Wrapper::QueryInterface(const IID &riid, void **ppvObj)
{
	DXLOG("D3D9Wrapper: QueryInterface\n");
	return Direct3D9->QueryInterface(riid, ppvObj);
}

ULONG Direct3D9Wrapper::AddRef()
{
	DXLOG("D3D9Wrapper: AddRef\n");
	return Direct3D9->AddRef();
}

ULONG Direct3D9Wrapper::Release()
{
	DXLOG("D3D9Wrapper: Release\n");
	return Direct3D9->Release();
}

/*** IDirect3D9 methods ***/
HRESULT Direct3D9Wrapper::RegisterSoftwareDevice(void* pInitializeFunction)
{
	DXLOG("D3D9Wrapper: RegisterSoftwareDevice\n");
	return Direct3D9->RegisterSoftwareDevice(pInitializeFunction);
}

UINT Direct3D9Wrapper::GetAdapterCount()
{
	DXLOG("D3D9Wrapper: GetAdapterCount\n");
	return Direct3D9->GetAdapterCount();
}

HRESULT Direct3D9Wrapper::GetAdapterIdentifier(UINT Adapter,DWORD Flags,D3DADAPTER_IDENTIFIER9* pIdentifier)
{
	DXLOG("D3D9Wrapper: GetAdapterIdentifier\n");
	return Direct3D9->GetAdapterIdentifier(Adapter, Flags, pIdentifier);
}

UINT Direct3D9Wrapper::GetAdapterModeCount(UINT Adapter,D3DFORMAT Format)
{
	DXLOG("D3D9Wrapper: GetAdapterModeCount\n");
	return Direct3D9->GetAdapterModeCount(Adapter, Format);
}

HRESULT Direct3D9Wrapper::EnumAdapterModes(UINT Adapter,D3DFORMAT Format,UINT Mode,D3DDISPLAYMODE* pMode)
{
	DXLOG("D3D9Wrapper: EnumAdapterModes\n");
	return Direct3D9->EnumAdapterModes(Adapter, Format, Mode, pMode);
}

HRESULT Direct3D9Wrapper::GetAdapterDisplayMode(UINT Adapter,D3DDISPLAYMODE* pMode)
{
	DXLOG("D3D9Wrapper: GetAdapterDisplayMode\n");
	return Direct3D9->GetAdapterDisplayMode(Adapter, pMode);
}

HRESULT Direct3D9Wrapper::CheckDeviceType(UINT Adapter,D3DDEVTYPE DevType,D3DFORMAT AdapterFormat,D3DFORMAT BackBufferFormat,BOOL bWindowed)
{
	DXLOG("D3D9Wrapper: CheckDeviceType\n");
	return Direct3D9->CheckDeviceType(Adapter, DevType, AdapterFormat, BackBufferFormat, bWindowed);
}

HRESULT Direct3D9Wrapper::CheckDeviceFormat(UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT AdapterFormat,DWORD Usage,D3DRESOURCETYPE RType,D3DFORMAT CheckFormat)
{
	DXLOG("D3D9Wrapper: CheckDeviceFormat\n");
	return Direct3D9->CheckDeviceFormat(Adapter, DeviceType, AdapterFormat, Usage, RType, CheckFormat);
}

HRESULT Direct3D9Wrapper::CheckDeviceMultiSampleType(UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT SurfaceFormat,BOOL Windowed,D3DMULTISAMPLE_TYPE MultiSampleType,DWORD* pQualityLevels)
{
	DXLOG("D3D9Wrapper: CheckDeviceMultiSampleType\n");
	return Direct3D9->CheckDeviceMultiSampleType(Adapter, DeviceType, SurfaceFormat, Windowed, MultiSampleType, pQualityLevels);
}

HRESULT Direct3D9Wrapper::CheckDepthStencilMatch(UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT AdapterFormat,D3DFORMAT RenderTargetFormat,D3DFORMAT DepthStencilFormat)
{
	DXLOG("D3D9Wrapper: CheckDepthStencilMatch\n");
	return Direct3D9->CheckDepthStencilMatch(Adapter, DeviceType, AdapterFormat, RenderTargetFormat, DepthStencilFormat);
}

HRESULT Direct3D9Wrapper::CheckDeviceFormatConversion(UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT SourceFormat,D3DFORMAT TargetFormat)
{
	DXLOG("D3D9Wrapper: CheckDeviceFormatConversion\n");
	return Direct3D9->CheckDeviceFormatConversion(Adapter, DeviceType, SourceFormat, TargetFormat);
}

HRESULT Direct3D9Wrapper::GetDeviceCaps(UINT Adapter,D3DDEVTYPE DeviceType,D3DCAPS9* pCaps)
{
	DXLOG("D3D9Wrapper: GetDeviceCaps\n");
	return Direct3D9->GetDeviceCaps(Adapter, DeviceType, pCaps);
}

HMONITOR Direct3D9Wrapper::GetAdapterMonitor (UINT Adapter)
{
	DXLOG("D3D9Wrapper: GetAdapterMonitor\n");
	return Direct3D9->GetAdapterMonitor(Adapter);
}

HRESULT Direct3D9Wrapper::CreateDevice(UINT Adapter, D3DDEVTYPE DeviceType,HWND hFocusWindow, DWORD BehaviorFlags,D3DPRESENT_PARAMETERS *pPresentationParameters,IDirect3DDevice9 **ppReturnedDeviceInterface)
{
	DXLOG("D3D9Wrapper: CreateDevice\n");

	IDirect3DDevice9* pDirect3DDevice9;
	HRESULT hRes = Direct3D9->CreateDevice( Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, &pDirect3DDevice9 );
	*ppReturnedDeviceInterface = new Direct3DDevice9WrapperExtended( pDirect3DDevice9, this, pPresentationParameters, hFocusWindow);
	return hRes;
}