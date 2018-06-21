#include "stdafx.h"
#include "TestPlane.h"

#include "Transform.h"
#include "Shader.h"
#include "VIBuffer.h"
#include "Texture.h"

CTestPlane::CTestPlane(LPDIRECT3DDEVICE9 ptr_device)
	: Engine::CGameObject(ptr_device)
{
}

CTestPlane::~CTestPlane()
{
	Release();
}

HRESULT CTestPlane::Initialize()
{
	HRESULT hr = CGameObject::Initialize();
	assert(!FAILED(hr) && "CGameObject::Initialize call failed in TestPlane1");

	hr = AddComponent();
	assert(!FAILED(hr) && "AddComponent call failed in TestPlane1");

	ptr_transform_->position_ = Vector3(10.f, 0.f, 10.f);
	ptr_transform_->rotation_.x = D3DXToRadian(90.f);
	ptr_transform_->scale_ = Vector3(10.f, 10.f, 1.f);

	return S_OK;
}

void CTestPlane::Update(float delta_time)
{
	Engine::GameManager()->AddRenderLayer(Engine::RENDERLAYER::LAYER_NONEALPHA, this);

	Engine::CGameObject::Update(delta_time);
}

void CTestPlane::Render()
{
	LPD3DXEFFECT ptr_effect = ptr_shader_->GetEffectHandle();

	Matrix mat_view, mat_proj;
	ptr_device_->GetTransform(D3DTS_VIEW, &mat_view);
	ptr_device_->GetTransform(D3DTS_PROJECTION, &mat_proj);

	ptr_effect->SetMatrix("g_mat_world", &ptr_transform_->mat_world_);
	ptr_effect->SetMatrix("g_mat_view", &mat_view);
	ptr_effect->SetMatrix("g_mat_projection", &mat_proj);
	ptr_effect->SetTexture("g_base_texture", ptr_color_texture_->GetTexture(0));

	ptr_shader_->BegineShader(0);
	ptr_buffer_->Render();
	ptr_shader_->EndShader();
}

CTestPlane * CTestPlane::Create(LPDIRECT3DDEVICE9 ptr_device)
{
	CTestPlane* ptr_obj = new CTestPlane(ptr_device);
	if (FAILED(ptr_obj->Initialize()))
	{
		Safe_Delete(ptr_obj);
		assert(!"TestPlane Create Failed");
	}

	return ptr_obj;
}

HRESULT CTestPlane::AddComponent()
{
	HRESULT hr = E_FAIL;

	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Buffer_RectTexture"), TEXT("Buffer"), ptr_buffer_);
	assert(hr == S_OK && "ScreenImage Buffer ReadyComponent Failed");
	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Component_Transform"), TEXT("Transform"), ptr_transform_);
	assert(hr == S_OK && "Tranform Component ReadyComponent Failed");
	hr = Ready_Component(MAINTAIN_STAGE, TEXT("Texture_TestPlane_Color"), TEXT("Texture_Color"), ptr_color_texture_);
	assert(hr == S_OK && "ScreenImage Texture ReadyComponent Failed");
	hr = Ready_Component(MAINTAIN_STAGE, TEXT("Texture_TestPlane_Normal"), TEXT("Texture_Normal"), ptr_normal_texture_);
	assert(hr == S_OK && "ScreenImage Texture ReadyComponent Failed");
	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Shader_Default"), TEXT("Shader"), ptr_shader_);
	assert(hr == S_OK && "ScreenImage Shader ReadyComponent Failed");

	return S_OK;
}

void CTestPlane::Release()
{
}
