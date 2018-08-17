#include "stdafx.h"
#include "FireEffect.h"

#include "Transform.h"
#include "Texture.h"
#include "RectTexture.h"
#include "Shader.h"

CFireEffect::CFireEffect(LPDIRECT3DDEVICE9 ptr_device)
	: Engine::CGameObject(ptr_device)
{
}

CFireEffect::~CFireEffect()
{
	Release();
}

HRESULT CFireEffect::Initialize(Vector3* pos)
{
	HRESULT hr = Engine::CGameObject::Initialize();
	assert(!FAILED(hr) && "FireEffect Initialize Falied");
	hr = AddComponent();
	assert(!FAILED(hr) && "FireEffect AddComponent Falied");

	ptr_fix_pos_ = pos;
	ptr_transform_->scale() = Vector3(0.5f, 0.5f, 0.5f);

	frame_num_ = 0.f;
	scene_num_ = 0.f;
	max_u_ = 4.f;
	max_v_ = 4.f;
	return hr;
}

void CFireEffect::Update(float delta_time)
{
	ComputeViewZ(*Subject()->camera_pos());

	ptr_transform_->position() = *ptr_fix_pos_;
	CGameObject::Update(delta_time);

	//Matrix mat_bill;
	//ptr_device_->GetTransform(D3DTS_VIEW, &mat_bill);
	//mat_bill._41 = 0.f; mat_bill._42 = 0.f;	mat_bill._43 = 0.f;
	//D3DXMatrixInverse(&mat_bill, nullptr, &mat_bill);
	//
	//ptr_transform_->mat_world() = mat_bill * ptr_transform_->mat_world();

	ptr_transform_->LookAt_XY(*Subject()->camera_pos());

	frame_num_ += max_u_ * max_v_ * delta_time;
	if (frame_num_ >= max_u_)
	{
		frame_num_ = 0.f;
		if (++scene_num_ >= max_v_)
			destroy_ = true;
	}
}

void CFireEffect::LateUpdate()
{
	Engine::GameManager()->AddRenderLayer(Engine::LAYER_ALPHA, this);
}

void CFireEffect::Render()
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

	ptr_effect->SetFloat("g_max_u", max_u_);
	ptr_effect->SetFloat("g_max_v", max_v_);
	ptr_effect->SetInt("g_frame_num", (int)frame_num_);
	ptr_effect->SetInt("g_scene_num", (int)scene_num_);

	ptr_shader_->BegineShader(1);
	ptr_buffer_->Render();
	ptr_shader_->EndShader();
}

CFireEffect * CFireEffect::Create(LPDIRECT3DDEVICE9 ptr_device, Vector3* pos)
{
	CFireEffect* ptr_obj = new CFireEffect(ptr_device);
	if (FAILED(ptr_obj->Initialize(pos)))
	{
		Safe_Delete(ptr_obj);
		assert(!"LevelObject Create Failed");
	}

	return ptr_obj;
}

HRESULT CFireEffect::AddComponent()
{
	HRESULT hr = E_FAIL;
	
	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Buffer_RectTexture"), TEXT("Buffer"), ptr_buffer_);
	assert(hr == S_OK && "FireEffect Buffer ReadyComponent Failed");
	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Component_Transform"), TEXT("Transform"), ptr_transform_);
	assert(!FAILED(hr) && "ReadyComponent Failed of FireEffect Tranform Component ");
	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Shader_Mesh_Effect"), TEXT("Shader"), ptr_shader_);
	assert(!FAILED(hr) && "ReadyComponent Failed of FireEffect Shader Component");
	hr = Ready_Component(MAINTAIN_STAGE, TEXT("Explosion_Texture"), TEXT("Texture"), ptr_texture_);
	assert(hr == S_OK && "FireEffect ReadyComponent Failed");

	return S_OK;
}

void CFireEffect::Release()
{
}
