#include "stdafx.h"
#include "WarBossJump.h"

#include "Transform.h"
#include "Shader.h"
#include "ParticleSystem.h"

CWarBossJump::CWarBossJump(LPDIRECT3DDEVICE9 ptr_device)
	: Engine::CGameObject(ptr_device)
{
}

CWarBossJump::~CWarBossJump()
{
	Release();
}

HRESULT CWarBossJump::Initialize(Vector3 * ptr_parent)
{
	HRESULT hr = Engine::CGameObject::Initialize();
	assert(!FAILED(hr) && "Explosion Initialize Falied");
	hr = AddComponent();
	assert(!FAILED(hr) && "Explosion AddComponent Falied");

	ptr_parent_ = ptr_parent;
	ptr_particle_->SetCameraPos(Subject()->camera_pos());

	return S_OK;
}

void CWarBossJump::Update(float delta_time)
{
	ptr_transform_->position() = *ptr_parent_;

	ComputeViewZ(*Subject()->camera_pos());

	CGameObject::Update(delta_time);
	ptr_transform_->LookAt(*Subject()->camera_pos());
}

void CWarBossJump::LateUpdate()
{
	Engine::GameManager()->AddRenderLayer(Engine::LAYER_ALPHA, this);
}

void CWarBossJump::Render()
{
	LPD3DXEFFECT ptr_effect = ptr_shader_->GetEffectHandle();

	ptr_effect->Begin(nullptr, 0);
	ptr_particle_->Render(ptr_effect, &(ptr_transform_->mat_world()));
	ptr_effect->End();
}

CWarBossJump * CWarBossJump::Create(LPDIRECT3DDEVICE9 ptr_device, Vector3 * ptr_parent)
{
	CWarBossJump* ptr_obj = new CWarBossJump(ptr_device);
	if (FAILED(ptr_obj->Initialize(ptr_parent)))
	{
		Safe_Delete(ptr_obj);
		assert(!"Explosion Create Failed");
	}

	return ptr_obj;
}

HRESULT CWarBossJump::AddComponent()
{
	HRESULT hr = E_FAIL;

	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Component_Transform"), TEXT("Transform"), ptr_transform_);
	assert(!FAILED(hr) && "ReadyComponent Failed of CSwordEletro Tranform Component ");
	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Shader_Mesh_Effect"), TEXT("Shader"), ptr_shader_);
	assert(!FAILED(hr) && "ReadyComponent Failed of CSwordEletro Shader Component");
	hr = Ready_Component(MAINTAIN_STAGE, TEXT("WarBoss_Skill_Jump"), TEXT("Effect"), ptr_particle_);
	assert(!FAILED(hr) && "ReadyComponent Failed of CSwordEletro ParticleSystem Component");

	return S_OK;
}

void CWarBossJump::Release()
{
}
