#include "stdafx.h"
#include "SpaceMarin.h"

#include "Transform.h"
#include "Shader.h"
#include "DynamicMesh.h"
#include "AnimController.h"

#include "PlayerController.h"

CSpaceMarin::CSpaceMarin(LPDIRECT3DDEVICE9 ptr_device)
	: Engine::CGameObject(ptr_device)
{
}

CSpaceMarin::~CSpaceMarin()
{
	Release();
}

HRESULT CSpaceMarin::Initialize(int ctrl_id)
{
	HRESULT hr = CGameObject::Initialize();
	assert(!FAILED(hr) && "CGameObject::Initialize call failed in SpaceMarin");

	hr = AddComponent(ctrl_id);
	assert(!FAILED(hr) && "AddComponent call failed in SpaceMarin");

	ptr_anim_ctrl_->SetAnimationTrack("Idle");

	current_cell_index_ = -1;

	return S_OK;
}

void CSpaceMarin::Update(float delta_time)
{
	if(current_cell_index_ == -1)
		current_cell_index_ = Engine::GameManager()->FindCellIndex(ptr_transform_->position());

	if (ptr_anim_ctrl_->GetPeriod() <= ptr_anim_ctrl_->GetTrackPosition())
		ptr_anim_ctrl_->SetTrackPosition(0.0);

	Engine::CGameObject::Update(delta_time);
	current_cell_index_ = Engine::GameManager()->MoveFromNavMesh(ptr_transform_->position(), ptr_transform_->move_dir(), current_cell_index_, 0);
	//ptr_transform_->position() += ptr_transform_->move_dir();
}

void CSpaceMarin::LateUpdate()
{
	Engine::GameManager()->AddRenderLayer(Engine::RENDERLAYER::LAYER_NONEALPHA, this);
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

CSpaceMarin * CSpaceMarin::Create(LPDIRECT3DDEVICE9 ptr_device, int ctrl_id)
{
	CSpaceMarin* ptr_obj = new CSpaceMarin(ptr_device);
	if (FAILED(ptr_obj->Initialize(ctrl_id)))
	{
		Safe_Delete(ptr_obj);
		assert(!"SpaceMarin Create Failed");
	}

	return ptr_obj;
}

HRESULT CSpaceMarin::AddComponent(int ctrl_id)
{
	HRESULT hr = E_FAIL;

	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Component_Transform"), TEXT("Transform"), ptr_transform_);
	assert(hr == S_OK && "Tranform Component ReadyComponent Failed");
	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Shader_DynamicMesh"), TEXT("Shader"), ptr_shader_);
	assert(hr == S_OK && "ScreenImage Shader ReadyComponent Failed");
	hr = Ready_Component(MAINTAIN_STAGE, TEXT("SpaceMarin_Mesh"), TEXT("Mesh"), ptr_mesh_);
	assert(hr == S_OK && "Mesh ReadyComponent Failed");
	ptr_anim_ctrl_ = ptr_mesh_->CloneAnimController();

	if (0 == ctrl_id)
	{
		ptr_ctrl_ = CPlayerController::Create(ptr_transform_, 5.f, 5.f);
		ptr_transform_->AddReferenceCount();
		CGameObject::AddComponent(TEXT("PlayerController"), ptr_ctrl_);

		if (nullptr != ptr_ctrl_)
			Engine::GameManager()->Add_Prototype(MAINTAIN_STAGE, TEXT("Component_PlayerController"), ptr_ctrl_->CloneComponent());
	}
	// TODO: ctrl_id 1일때 AIController 추가하기.

	return S_OK;
}

void CSpaceMarin::Release()
{
	Engine::Safe_Delete(ptr_anim_ctrl_);
}
