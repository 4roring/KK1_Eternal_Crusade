#include "GraphicDevice.h"

Engine::CGraphicDevice::CGraphicDevice()
{
}

Engine::CGraphicDevice::~CGraphicDevice()
{
	Release();
}

LPDIRECT3DDEVICE9 Engine::CGraphicDevice::GetDevice()
{
	return ptr_device_;
}

HRESULT Engine::CGraphicDevice::InitGraphicDevice(WINMODE mode, HWND hwnd, WORD size_x, WORD size_y)
{
	ptr_sdk_ = Direct3DCreate9(D3D_SDK_VERSION);

	D3DCAPS9 devicecaps;
	ZeroMemory(&devicecaps, sizeof(D3DCAPS9));

	if (FAILED(ptr_sdk_->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &devicecaps)))
	{
		return E_FAIL;
	}

	DWORD vp;

	if (devicecaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	vp |= D3DCREATE_MULTITHREADED;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	SetParameters(d3dpp, mode, hwnd, size_x, size_y);

	if (FAILED(ptr_sdk_->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL
		, hwnd, vp, &d3dpp, &ptr_device_)))
	{
		return E_FAIL;
	}

	return S_OK;
}

void Engine::CGraphicDevice::SetParameters(D3DPRESENT_PARAMETERS & d3dpp, WINMODE mode, HWND hwnd, WORD size_x, WORD size_y)
{
	d3dpp.BackBufferWidth = size_x;
	d3dpp.BackBufferHeight = size_y;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;

	d3dpp.MultiSampleQuality = 0;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;

	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.EnableAutoDepthStencil = TRUE;

	d3dpp.hDeviceWindow = hwnd;
	d3dpp.Windowed = mode;

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}

void Engine::CGraphicDevice::Release()
{
	Engine::Safe_Release(ptr_device_);
	Engine::Safe_Release(ptr_sdk_);
}
