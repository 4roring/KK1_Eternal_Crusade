#pragma once

#include "Engine_Include.h"

BEGIN(Engine)

class G_Buffer;
class CGameObject;

class CRenderer
{
private:
	explicit CRenderer(LPDIRECT3DDEVICE9 ptr_device);

public:
	~CRenderer();

public:
	static CRenderer* Create(LPDIRECT3DDEVICE9 ptr_device);

private:
	HRESULT Initialize();

public:
	void Render();

public:
	void AddRenderLayer(RENDERLAYER render_id, CGameObject* ptr_object);
	void RenderGroupRelease();

private:
	void Render_Priority();
	void Render_NoneAlpha();
	void Render_Alpha();
	void Render_UI();

private:
	void Render_Deferred();

private:
	void Release();

private:
	LPDIRECT3DDEVICE9 ptr_device_ = nullptr;

private:
	std::vector<CGameObject*> render_layer_[LAYER_END];

private:
	G_Buffer* ptr_g_buffer = nullptr;
};

END