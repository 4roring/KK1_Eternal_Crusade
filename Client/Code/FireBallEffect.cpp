#include "stdafx.h"
#include "FireBallEffect.h"

#include "Transform.h"
#include "Shader.h"
#include "ParticleSystem.h"

CFireBallEffect::CFireBallEffect(LPDIRECT3DDEVICE9 ptr_device)
	: Engine::CGameObject(ptr_device)
{
}

CFireBallEffect::~CFireBallEffect()
{
	Release();
}

HRESULT CFireBallEffect::Initialize(const Matrix * ptr_parent)
{
	HRESULT hr = Engine::CGameObject::Initialize();
	assert(!FAILED(hr) && "Explosion Initialize Falied");
	hr = AddComponent();
	assert(!FAILED(hr) && "Explosion AddComponent Falied");

	ptr_parent_ = ptr_parent;
	ptr_particle_->SetCameraPos(Subject()->camera_pos());

	return S_OK;
}

void CFireBallEffect::Update(float delta_time)
{
	ptr_transform_->position() = *(Vector3*)&ptr_parent_->m[3][0];
	CGameObject::Update(delta_time);
	ComputeViewZ(*Subject()->camera_pos());
	ptr_transform_->LookAt(*Subject()->camera_pos());
}

void CFireBallEffect::LateUpdate()
{
	Engine::GameManager()->AddRenderLayer(Engine::LAYER_ALPHA, this);
}

void CFireBallEffect::Render()
{
	LPD3DXEFFECT ptr_effect = ptr_shader_->GetEffectHandle();

	ptr_effect->Begin(nullptr, 0);
	ptr_particle_->Render(ptr_effect, &(ptr_transform_->mat_world()));
	ptr_effect->End();
}

CFireBallEffect * CFireBallEffect::Create(LPDIRECT3DDEVICE9 ptr_device, const Matrix * ptr_parent)
{
	CFireBallEffect* ptr_obj = new CFireBallEffect(ptr_device);
	if (FAILED(ptr_obj->Initialize(ptr_parent)))
	{
		Safe_Delete(ptr_obj);
		assert(!"Explosion Create Failed");
	}

	return ptr_obj;
}

HRESULT CFireBallEffect::AddComponent()
{
	HRESULT hr = E_FAIL;

	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Component_Transform"), TEXT("Transform"), ptr_transform_);
	assert(!FAILED(hr) && "ReadyComponent Failed of CFireBallEffect Tranform Component ");
	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Shader_Mesh_Effect"), TEXT("Shader"), ptr_shader_);
	assert(!FAILED(hr) && "ReadyComponent Failed of CFireBallEffect Shader Component");
	hr = Ready_Component(MAINTAIN_STAGE, TEXT("FireBall_Effect"), TEXT("Effect"), ptr_particle_);
	assert(!FAILED(hr) && "ReadyComponent Failed of CFireBallEffect ParticleSystem Component");

	return S_OK;

}

void CFireBallEffect::Release()
{
}
