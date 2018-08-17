#include "stdafx.h"
#include "HitEffect.h"

#include "Transform.h"
#include "Shader.h"
#include "ParticleSystem.h"

CHitEffect::CHitEffect(LPDIRECT3DDEVICE9 ptr_device)
	: Engine::CGameObject(ptr_device)
{
}

CHitEffect::~CHitEffect()
{
	Release();
}

HRESULT CHitEffect::Initialize(const Vector3 & position, const std::wstring& effect_key)
{
	HRESULT hr = Engine::CGameObject::Initialize();
	assert(!FAILED(hr) && "FireEffect Initialize Falied");
	hr = AddComponent(effect_key);
	assert(!FAILED(hr) && "FireEffect AddComponent Falied");

	ptr_transform_->position() = position;
	life_time = ptr_particle_->particles()->duration * 1.5f;
	ptr_particle_->SetCameraPos(Subject()->camera_pos());
	ptr_blood_particle_->SetCameraPos(Subject()->camera_pos());

	return S_OK;
}

void CHitEffect::Update(float delta_time)
{
	if ((life_time -= delta_time) <= 0.f)
		destroy_ = true;

	ComputeViewZ(*Subject()->camera_pos());

	CGameObject::Update(delta_time);
	ptr_transform_->LookAt_XY(*Subject()->camera_pos());
}

void CHitEffect::LateUpdate()
{
	Engine::GameManager()->AddRenderLayer(Engine::LAYER_ALPHA, this);
}

void CHitEffect::Render()
{
	LPD3DXEFFECT ptr_effect = ptr_shader_->GetEffectHandle();

	ptr_effect->Begin(nullptr, 0);
	ptr_particle_->Render(ptr_effect, &ptr_transform_->mat_world());
	ptr_effect->End();

	ptr_effect = ptr_effect_shader_->GetEffectHandle();
	ptr_effect->Begin(nullptr, 0);
	ptr_blood_particle_->Render(ptr_effect, &ptr_transform_->mat_world());
	ptr_effect->End();
}

CHitEffect * CHitEffect::Create(LPDIRECT3DDEVICE9 ptr_device, const Vector3 & position, const std::wstring& effect_key)
{
	CHitEffect* ptr_obj = new CHitEffect(ptr_device);
	if (FAILED(ptr_obj->Initialize(position, effect_key)))
	{
		Safe_Delete(ptr_obj);
		assert(!"BulletHit Create Failed");
	}

	return ptr_obj;
}

HRESULT CHitEffect::AddComponent(const std::wstring& effect_key)
{
	HRESULT hr = E_FAIL;

	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Component_Transform"), TEXT("Transform"), ptr_transform_);
	assert(!FAILED(hr) && "ReadyComponent Failed of FireEffect Tranform Component ");
	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Shader_Point_Effect"), TEXT("Shader"), ptr_shader_);
	assert(!FAILED(hr) && "ReadyComponent Failed of FireEffect Shader Component");
	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Shader_Mesh_Effect"), TEXT("Shader_Effect"), ptr_effect_shader_);
	assert(!FAILED(hr) && "ReadyComponent Failed of FireEffect Shader Component");

	hr = Ready_Component(MAINTAIN_STAGE, TEXT("BulletHit"), TEXT("Hit_Particle"), ptr_particle_);
	assert(!FAILED(hr) && "ReadyComponent Failed of FireEffect ParticleSystem Component");
	hr = Ready_Component(MAINTAIN_STAGE, effect_key, TEXT("Blood_Particle"), ptr_blood_particle_);
	assert(!FAILED(hr) && "ReadyComponent Failed of FireEffect ParticleSystem Component");

	return S_OK;
}

void CHitEffect::Release()
{
}
