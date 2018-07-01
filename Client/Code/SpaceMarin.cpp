#include "stdafx.h"
#include "SpaceMarin.h"

#include "Transform.h"
#include "Shader.h"
#include "DynamicMesh.h"
#include "AnimController.h"

CSpaceMarin::CSpaceMarin(LPDIRECT3DDEVICE9 ptr_device)
	: Engine::CGameObject(ptr_device)
{
}

CSpaceMarin::~CSpaceMarin()
{
	Release();
}

HRESULT CSpaceMarin::Initialize()
{
	HRESULT hr = CGameObject::Initialize();
	assert(!FAILED(hr) && "CGameObject::Initialize call failed in SpaceMarin");

	hr = AddComponent();
	assert(!FAILED(hr) && "AddComponent call failed in SpaceMarin");

	anim_track_ = rand() % 34;

	

	ptr_anim_ctrl_->SetAnimationTrack(anim_track_);

	return S_OK;
}

void CSpaceMarin::Update(float delta_time)
{
	Engine::GameManager()->AddRenderLayer(Engine::RENDERLAYER::LAYER_NONEALPHA, this);

	if (ptr_anim_ctrl_->GetPeriod() <= ptr_anim_ctrl_->GetTrackPosition())
	{
		ptr_anim_ctrl_->SetAnimationTrack(anim_track_);
		ptr_anim_ctrl_->SetTrackPosition(0.0);
	}

	Engine::CGameObject::Update(delta_time);
}

void CSpaceMarin::Render()
{	
	LPD3DXEFFECT ptr_effect = ptr_shader_->GetEffectHandle();
	
	Matrix mat_view, mat_proj;
	ptr_device_->GetTransform(D3DTS_VIEW, &mat_view);
	ptr_device_->GetTransform(D3DTS_PROJECTION, &mat_proj);

	ptr_effect->SetMatrix("g_mat_world", &ptr_transform_->mat_world());
	ptr_effect->SetMatrix("g_mat_view", &mat_view);
	ptr_effect->SetMatrix("g_mat_projection", &mat_proj);
	
	ptr_effect->SetVector("g_light_diffuse", &Vector4(1.f, 1.f, 1.f, 1.f));
	ptr_effect->SetVector("g_light_ambient", &Vector4(1.f, 1.f, 1.f, 1.f));
	ptr_effect->SetVector("g_light_dir", &Vector4(0.f, -1.f, 1.f, 0.f));

	ptr_mesh_->FrameMove(Engine::Time()->GetDeltaTime(), ptr_anim_ctrl_);
	ptr_mesh_->RenderMesh(ptr_effect);
}

CSpaceMarin * CSpaceMarin::Create(LPDIRECT3DDEVICE9 ptr_device)
{
	CSpaceMarin* ptr_obj = new CSpaceMarin(ptr_device);
	if (FAILED(ptr_obj->Initialize()))
	{
		Safe_Delete(ptr_obj);
		assert(!"SpaceMarin Create Failed");
	}

	return ptr_obj;
}

HRESULT CSpaceMarin::AddComponent()
{
	HRESULT hr = E_FAIL;

	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Component_Transform"), TEXT("Transform"), ptr_transform_);
	assert(hr == S_OK && "Tranform Component ReadyComponent Failed");
	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Shader_DynamicMesh"), TEXT("Shader"), ptr_shader_);
	assert(hr == S_OK && "ScreenImage Shader ReadyComponent Failed");
	hr = Ready_Component(MAINTAIN_STAGE, TEXT("SpaceMarin_Mesh"), TEXT("Mesh"), ptr_mesh_);
	assert(hr == S_OK && "Mesh ReadyComponent Failed");
	ptr_anim_ctrl_ = ptr_mesh_->CloneAnimController();
	return S_OK;
}

void CSpaceMarin::Release()
{
	Engine::Safe_Delete(ptr_anim_ctrl_);
}
