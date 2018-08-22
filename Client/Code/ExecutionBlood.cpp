#include "stdafx.h"
#include "ExecutionBlood.h"

#include "Transform.h"
#include "Shader.h"
#include "ParticleSystem.h"

CExecutionBlood::CExecutionBlood(LPDIRECT3DDEVICE9 ptr_device)
	: Engine::CGameObject(ptr_device)
{
}

CExecutionBlood::~CExecutionBlood()
{
	Release();
}

HRESULT CExecutionBlood::Initialize(const Vector3 & position)
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

void CExecutionBlood::Update(float delta_time)
{
	if ((life_time -= delta_time) <= 0.f)
		destroy_ = true;

	ComputeViewZ(*Subject()->camera_pos());

	CGameObject::Update(delta_time);
	ptr_transform_->LookAt_XY(*Subject()->camera_pos());
}

void CExecutionBlood::LateUpdate()
{
	Engine::GameManager()->AddRenderLayer(Engine::LAYER_ALPHA, this);
}

void CExecutionBlood::Render()
{
	LPD3DXEFFECT ptr_effect = ptr_shader_->GetEffectHandle();

	ptr_effect->Begin(nullptr, 0);
	ptr_particle_->Render(ptr_effect, &ptr_transform_->mat_world());
	ptr_effect->End();
}

CExecutionBlood * CExecutionBlood::Create(LPDIRECT3DDEVICE9 ptr_device, const Vector3 & position)
{
	CExecutionBlood* ptr_obj = new CExecutionBlood(ptr_device);
	if (FAILED(ptr_obj->Initialize(position)))
	{
		Safe_Delete(ptr_obj);
		assert(!"Explosion Create Failed");
	}

	return ptr_obj;
}

HRESULT CExecutionBlood::AddComponent()
{
	HRESULT hr = E_FAIL;

	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Component_Transform"), TEXT("Transform"), ptr_transform_);
	assert(!FAILED(hr) && "ReadyComponent Failed of FireEffect Tranform Component ");
	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Shader_Mesh_Effect"), TEXT("Shader"), ptr_shader_);
	assert(!FAILED(hr) && "ReadyComponent Failed of FireEffect Shader Component");
	hr = Ready_Component(MAINTAIN_STAGE, TEXT("ExecutionBlood"), TEXT("Effect"), ptr_particle_);
	assert(!FAILED(hr) && "ReadyComponent Failed of FireEffect ParticleSystem Component");

	return S_OK;
}

void CExecutionBlood::Release()
{
}