#pragma once

#include "Engine_Include.h"

BEGIN(Engine)

class GBuffer;
class CLightManager;
class CGameObject;
class CShader;
class CViewTexture;

class CRenderer
{
private:
	explicit CRenderer(LPDIRECT3DDEVICE9 ptr_device);

public:
	~CRenderer();
private:
	HRESULT Initialize();

public:
	void Render();

public:
	static CRenderer* Create(LPDIRECT3DDEVICE9 ptr_device);

private:
	void Release();

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
	void Render_LightAcc();
	void Render_Blend();

private:
	LPDIRECT3DDEVICE9 ptr_device_ = nullptr;

private:
	std::vector<CGameObject*> render_layer_[LAYER_END];

private:
	GBuffer* ptr_gbuffer = nullptr;
	CLightManager* ptr_light_manager_ = nullptr;
	CShader* ptr_shader_shade_ = nullptr;
	CShader* ptr_shader_blend_ = nullptr;
	CViewTexture* ptr_blend_buffer_ = nullptr;
};

END