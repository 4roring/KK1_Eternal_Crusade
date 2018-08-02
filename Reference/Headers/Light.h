#pragma once

#include "Engine_Include.h"

BEGIN(Engine)

class CViewTexture;

class CLight
{
private:
	explicit CLight(LPDIRECT3DDEVICE9 ptr_device);

public:
	~CLight();

public:
	const D3DLIGHT9& light_info() const {}

private:
	HRESULT Initialize(const D3DLIGHT9& light_info);

public:
	void Render_Light(LPD3DXEFFECT ptr_effect);

private:
	void Release();

public:
	static CLight* Create(LPDIRECT3DDEVICE9 ptr_device, const D3DLIGHT9& light_info);

private:
	D3DLIGHT9 light_info_ = {};

private:
	LPDIRECT3DDEVICE9 ptr_device_ = nullptr;
	CViewTexture* ptr_render_buffer_ = nullptr;
};

END