#pragma once

#include "Engine_Include.h"
#include "Engine_Singleton.h"

BEGIN(Engine)

class ENGINE_DLL CGraphicDevice
	: public CSingleton<CGraphicDevice>
{
public:
	enum WINMODE { MODE_FULL, MODE_WINDOW };

private:
	friend CSingleton;
	CGraphicDevice();
	virtual ~CGraphicDevice();

public:
	LPDIRECT3DDEVICE9 GetDevice();

public:
	HRESULT InitGraphicDevice(WINMODE mode, HWND hwnd, WORD size_x, WORD size_y);

private:
	LPDIRECT3D9 ptr_sdk_ = nullptr;
	LPDIRECT3DDEVICE9 ptr_device_ = nullptr;

private:
	void SetParameters(D3DPRESENT_PARAMETERS& d3dpp, WINMODE mode, HWND hwnd, WORD size_x, WORD size_y);
	void Release();
};

END