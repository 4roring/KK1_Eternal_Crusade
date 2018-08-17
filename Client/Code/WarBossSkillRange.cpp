#include "stdafx.h"
#include "WarBossSkillRange.h"

#include "Transform.h"
#include "Texture.h"
#include "RectTexture.h"
#include "Shader.h"

CWarBossSkillRange::CWarBossSkillRange(LPDIRECT3DDEVICE9 ptr_device)
	: Engine::CGameObject(ptr_device)
{
}

CWarBossSkillRange::~CWarBossSkillRange()
{
	Release();
}

HRESULT CWarBossSkillRange::Initialize(Vector3* ptr_pos)
{
	HRESULT hr = Engine::CGameObject::Initialize();
	assert(!FAILED(hr) && "WarBossSkillRange Initialize Falied");
	hr = AddComponent();
	assert(!FAILED(hr) && "WarBossSkillRange AddComponent Falied");

	ptr_pos_ = ptr_pos;
	ptr_transform_->rotation() = Vector3(1.57f, 0.f, 0.f);
	ptr_transform_->scale() = Vector3(5.f, 5.f, 1.f);

	return hr;
}


void CWarBossSkillRange::Update(float delta_time)
{
	ptr_transform_->position() = *ptr_pos_;
	ptr_transform_->position().y = 0.1f;
	CGameObject::Update(delta_time);
}

void CWarBossSkillRange::LateUpdate()
{
	Engine::GameManager()->AddRenderLayer(Engine::LAYER_NONEALPHA, this);
}

void CWarBossSkillRange::Render()
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

	ptr_shader_->BegineShader(3);
	ptr_buffer_->Render();
	ptr_shader_->EndShader();
}

CWarBossSkillRange * CWarBossSkillRange::Create(LPDIRECT3DDEVICE9 ptr_device, Vector3* ptr_pos)
{
	CWarBossSkillRange* ptr_obj = new CWarBossSkillRange(ptr_device);
	if (FAILED(ptr_obj->Initialize(ptr_pos)))
	{
		Safe_Delete(ptr_obj);
		assert(!"LevelObject Create Failed");
	}

	return ptr_obj;
}

HRESULT CWarBossSkillRange::AddComponent()
{
	HRESULT hr = E_FAIL;

	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Buffer_RectTexture"), TEXT("Buffer"), ptr_buffer_);
	assert(hr == S_OK && "FireLineSmoke Buffer ReadyComponent Failed");
	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Component_Transform"), TEXT("Transform"), ptr_transform_);
	assert(!FAILED(hr) && "ReadyComponent Failed of FireLineSmoke Tranform Component ");
	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Shader_Mesh_Effect"), TEXT("Shader"), ptr_shader_);
	assert(!FAILED(hr) && "ReadyComponent Failed of FireLineSmoke Shader Component");
	hr = Ready_Component(MAINTAIN_STAGE, TEXT("SkillRange_Texture"), TEXT("Texture"), ptr_texture_);
	assert(hr == S_OK && "FireLineSmoke Texture ReadyComponent Failed");

	return S_OK;
}

void CWarBossSkillRange::Release()
{
}
