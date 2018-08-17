#include "stdafx.h"
#include "SwordEletro.h"

#include "Transform.h"
#include "Shader.h"
#include "ParticleSystem.h"

CSwordEletro::CSwordEletro(LPDIRECT3DDEVICE9 ptr_device)
	: Engine::CGameObject(ptr_device)
{
}

CSwordEletro::~CSwordEletro()
{
	Release();
}

HRESULT CSwordEletro::Initialize(Matrix * ptr_parent) 
{
	HRESULT hr = Engine::CGameObject::Initialize();
	assert(!FAILED(hr) && "Explosion Initialize Falied");
	hr = AddComponent();
	assert(!FAILED(hr) && "Explosion AddComponent Falied");

	ptr_parent_ = ptr_parent;
	ptr_particle_->SetCameraPos(Subject()->camera_pos());

	return S_OK;
}

void CSwordEletro::Update(float delta_time)
{
	ComputeViewZ(*Subject()->camera_pos());

	CGameObject::Update(delta_time);
}

void CSwordEletro::LateUpdate()
{
	Engine::GameManager()->AddRenderLayer(Engine::LAYER_ALPHA, this);
	ptr_transform_->LookAt(*Subject()->camera_pos());
}

void CSwordEletro::Render()
{
	LPD3DXEFFECT ptr_effect = ptr_shader_->GetEffectHandle();
	
	ptr_effect->Begin(nullptr, 0);
	ptr_particle_->Render(ptr_effect, &(ptr_transform_->mat_world() * *ptr_parent_));
	ptr_effect->End();
}

CSwordEletro * CSwordEletro::Create(LPDIRECT3DDEVICE9 ptr_device, Matrix* ptr_parent)
{
	CSwordEletro* ptr_obj = new CSwordEletro(ptr_device);
	if (FAILED(ptr_obj->Initialize(ptr_parent)))
	{
		Safe_Delete(ptr_obj);
		assert(!"Explosion Create Failed");
	}

	return ptr_obj;
}

HRESULT CSwordEletro::AddComponent()
{
	HRESULT hr = E_FAIL;

	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Component_Transform"), TEXT("Transform"), ptr_transform_);
	assert(!FAILED(hr) && "ReadyComponent Failed of CSwordEletro Tranform Component ");
	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Shader_Mesh_Effect"), TEXT("Shader"), ptr_shader_);
	assert(!FAILED(hr) && "ReadyComponent Failed of CSwordEletro Shader Component");
	hr = Ready_Component(MAINTAIN_STAGE, TEXT("Sword_Lightning"), TEXT("Effect"), ptr_particle_);
	assert(!FAILED(hr) && "ReadyComponent Failed of CSwordEletro ParticleSystem Component");

	return S_OK;
}

void CSwordEletro::Release()
{
}
