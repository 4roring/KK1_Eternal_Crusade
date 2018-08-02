#pragma once

#include "Engine_Include.h"
#include "Engine_Singleton.h"

BEGIN(Engine)

class CLight;

class CLightManager
	: public CSingleton<CLightManager>
{
private:
	friend CSingleton;
	CLightManager();
	virtual ~CLightManager();

public:
	HRESULT AddLight(LPDIRECT3DDEVICE9 ptr_device, const D3DLIGHT9& light_info);
	void Render_Light(LPD3DXEFFECT ptr_effect);

public:
	void ClearLight();

private:
	void Release();

private:
	std::list<CLight*> light_list_;
};

END