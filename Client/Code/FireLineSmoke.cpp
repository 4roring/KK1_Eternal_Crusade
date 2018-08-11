#include "stdafx.h"
#include "FireLineSmoke.h"

#include "Transform.h"
#include "Texture.h"
#include "LineTexture.h"
#include "Shader.h"

CFireLineSmoke::CFireLineSmoke(LPDIRECT3DDEVICE9 ptr_device)
	: Engine::CGameObject(ptr_device)
{
}

CFireLineSmoke::~CFireLineSmoke()
{
	Release();
}

HRESULT CFireLineSmoke::Initialize(const Vector3 & start_pos, const Vector3 & end_pos)
{
	HRESULT hr = Engine::CGameObject::Initialize();
	assert(!FAILED(hr) && "FireEffect Initialize Falied");
	hr = AddComponent();
	assert(!FAILED(hr) && "FireEffect AddComponent Falied");

	start_pos_ = start_pos;
	end_pos_ = end_pos;

	ptr_transform_->position() = end_pos;
	ptr_transform_->scale().y = 0.5f;
	ptr_transform_->scale().z = fabs((start_pos_ - end_pos_).Length());
	alpha_ = 1.f;

	return hr;
}


void CFireLineSmoke::LateInit()
{
}

void CFireLineSmoke::Update(float delta_time)
{
	ComputeViewZ(Subject()->camera_pos());
	CGameObject::Update(delta_time);
	ptr_transform_->LookAt_XY(start_pos_);

	alpha_ -= delta_time;
	if (alpha_ <= 0.f)
		destroy_ = true;
}

void CFireLineSmoke::LateUpdate()
{
	Engine::GameManager()->AddRenderLayer(Engine::LAYER_ALPHA, this);
}

void CFireLineSmoke::Render()
{
	LPD3DXEFFECT ptr_effect = ptr_shader_->GetEffectHandle();
	if (nullptr == ptr_effect) return;

	Matrix mat_view, mat_proj;
	ptr_device_->GetTransform(D3DTS_VIEW, &mat_view);
	ptr_device_->GetTransform(D3DTS_PROJECTION, &mat_proj);

	ptr_effect->SetMatrix("g_mat_world", &ptr_transform_->mat_world());
	ptr_effect->SetMatrix("g_mat_view", &mat_view);
	ptr_effect->SetMatrix("g_mat_projection", &mat_proj);

	ptr_effect->SetTexture("g_color_texture", ptr_texture_->GetTexture(0));
	ptr_effect->SetFloat("g_detail", ptr_transform_->scale().z * 0.25f);
	ptr_effect->SetFloat("g_alpha", alpha_);

	ptr_shader_->BegineShader(2);
	ptr_buffer_->Render();
	ptr_shader_->EndShader();
}

CFireLineSmoke * CFireLineSmoke::Create(LPDIRECT3DDEVICE9 ptr_device, const Vector3 & start_pos, const Vector3 & end_pos)
{
	CFireLineSmoke* ptr_obj = new CFireLineSmoke(ptr_device);
	if (FAILED(ptr_obj->Initialize(start_pos, end_pos)))
	{
		Safe_Delete(ptr_obj);
		assert(!"LevelObject Create Failed");
	}

	return ptr_obj;
}

HRESULT CFireLineSmoke::AddComponent()
{
	HRESULT hr = E_FAIL;

	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Buffer_Line"), TEXT("Buffer"), ptr_buffer_);
	assert(hr == S_OK && "FireLineSmoke Buffer ReadyComponent Failed");
	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Component_Transform"), TEXT("Transform"), ptr_transform_);
	assert(!FAILED(hr) && "ReadyComponent Failed of FireLineSmoke Tranform Component ");
	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Shader_Mesh_Effect"), TEXT("Shader"), ptr_shader_);
	assert(!FAILED(hr) && "ReadyComponent Failed of FireLineSmoke Shader Component");
	hr = Ready_Component(MAINTAIN_STAGE, TEXT("LineSmoke_Texture"), TEXT("Texture"), ptr_texture_);
	assert(hr == S_OK && "FireLineSmoke Texture ReadyComponent Failed");

	return S_OK;
}

void CFireLineSmoke::Release()
{
}
