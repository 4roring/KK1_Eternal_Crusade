#pragma once

#include "Engine_Include.h"

BEGIN(Engine)

class CGameObject;

class ENGINE_DLL CRenderer
{
private:
	explicit CRenderer(LPDIRECT3DDEVICE9 ptr_device);

public:
	~CRenderer();

public:
	static CRenderer* Create(LPDIRECT3DDEVICE9 ptr_device);

public:
	void Render();
	void Release();

public:
	void AddRenderLayer(RENDERLAYER render_id, CGameObject* ptr_object);

private:
	void Render_Priority();
	void Render_NoneAlpha();
	void Render_Alpha();
	void Render_UI();

private:
	LPDIRECT3DDEVICE9 ptr_device_ = nullptr;

private:
	std::vector<CGameObject*> render_layer_[LAYER_END];
};

END