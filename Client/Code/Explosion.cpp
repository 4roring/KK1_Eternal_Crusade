#include "stdafx.h"
#include "Explosion.h"

#include "Transform.h"
#include "Shader.h"
#include "ParticleSystem.h"

CExplosion::CExplosion(LPDIRECT3DDEVICE9 ptr_device)
	: Engine::CGameObject(ptr_device)
{
}

CExplosion::~CExplosion()
{
	Release();
}

HRESULT CExplosion::Initialize(const Vector3 & position)
{
	HRESULT hr = Engine::CGameObject::Initialize();
	assert(!FAILED(hr) && "Explosion Initialize Falied");
	hr = AddComponent();
	assert(!FAILED(hr) && "Explosion AddComponent Falied");

	ptr_transform_->position() = position;
	life_time = ptr_particle_->particles()->duration * 1.5f;
	ptr_particle_->SetCameraPos(Subject()->camera_pos());

	return S_OK;
}

void CExplosion::Update(float delta_time)
{
	if ((life_time -= delta_time) <= 0.f)
		destroy_ = true;

	ComputeViewZ(*Subject()->camera_pos());

	CGameObject::Update(delta_time);
	ptr_transform_->LookAt_XY(*Subject()->camera_pos());
}

void CExplosion::LateUpdate()
{
	Engine::GameManager()->AddRenderLayer(Engine::LAYER_ALPHA, this);
}

void CExplosion::Render()
{
	LPD3DXEFFECT ptr_effect = ptr_shader_->GetEffectHandle();

	ptr_effect->Begin(nullptr, 0);
	ptr_particle_->Render(ptr_effect, &ptr_transform_->mat_world());
	ptr_effect->End();
}

CExplosion * CExplosion::Create(LPDIRECT3DDEVICE9 ptr_device, const Vector3 & position)
{
	CExplosion* ptr_obj = new CExplosion(ptr_device);
	if (FAILED(ptr_obj->Initialize(position)))
	{
		Safe_Delete(ptr_obj);
		assert(!"Explosion Create Failed");
	}

	return ptr_obj;
}

HRESULT CExplosion::AddComponent()
{
	HRESULT hr = E_FAIL;

	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Component_Transform"), TEXT("Transform"), ptr_transform_);
	assert(!FAILED(hr) && "ReadyComponent Failed of FireEffect Tranform Component ");
	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Shader_Mesh_Effect"), TEXT("Shader"), ptr_shader_);
	assert(!FAILED(hr) && "ReadyComponent Failed of FireEffect Shader Component");
	hr = Ready_Component(MAINTAIN_STAGE, TEXT("Explosion_1"), TEXT("Effect"), ptr_particle_);
	assert(!FAILED(hr) && "ReadyComponent Failed of FireEffect ParticleSystem Component");

	return S_OK;
}

void CExplosion::Release()
{
}
