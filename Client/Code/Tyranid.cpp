#include "stdafx.h"
#include "Tyranid.h"

#include "Transform.h"
#include "Shader.h"
#include "DynamicMesh.h"
#include "AnimController.h"

CTyranid::CTyranid(LPDIRECT3DDEVICE9 ptr_device)
	: Engine::CGameObject(ptr_device)
{
}

CTyranid::~CTyranid()
{
	Release();
}

void CTyranid::set_attack(bool is_attack)
{
	attack_ = is_attack;
}

HRESULT CTyranid::Initialize()
{
	HRESULT hr = CGameObject::Initialize();
	assert(!FAILED(hr) && "CGameObject::Initialize call failed in SpaceMarin");

	hr = AddComponent();
	assert(!FAILED(hr) && "AddComponent call failed in SpaceMarin");

	current_cell_index_ = -1;
	next_state_ = LowerState::Idle;

	return S_OK;
}

void CTyranid::LateInit()
{
	if (current_cell_index_ == -1)
		current_cell_index_ = Engine::GameManager()->FindCellIndex(ptr_transform_->position());
	
	ptr_anim_ctrl_->SetAnimationTrack("Fall");
}

void CTyranid::Update(float delta_time)
{
	delta_time_ = delta_time;

	if (ptr_anim_ctrl_->GetPeriod() <= ptr_anim_ctrl_->GetTrackPosition())
		ptr_anim_ctrl_->SetTrackPosition(0.0);

	UpdateState(delta_time);
	Engine::CGameObject::Update(delta_time);
}

void CTyranid::LateUpdate()
{
	Engine::GameManager()->AddRenderLayer(Engine::RENDERLAYER::LAYER_NONEALPHA, this);
	UpdateAnimState();
}

void CTyranid::Render()
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

	ptr_mesh_->FrameMove(delta_time_, ptr_anim_ctrl_);
	ptr_mesh_->RenderMesh(ptr_effect, 0);
}

CTyranid * CTyranid::Create(LPDIRECT3DDEVICE9 ptr_device)
{
	CTyranid* ptr_obj = new CTyranid(ptr_device);
	if (FAILED(ptr_obj->Initialize()))
	{
		Safe_Delete(ptr_obj);
		assert(!"CTyranid Create Failed");
	}

	return ptr_obj;
}

HRESULT CTyranid::AddComponent()
{
	HRESULT hr = E_FAIL;

	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Component_Transform"), TEXT("Transform"), ptr_transform_);
	assert(hr == S_OK && "Tranform Component ReadyComponent Failed");

	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Shader_DynamicMesh"), TEXT("Shader"), ptr_shader_);
	assert(hr == S_OK && "ScreenImage Shader ReadyComponent Failed");
	hr = Ready_Component(MAINTAIN_STAGE, TEXT("Tyranid_Mesh"), TEXT("Mesh"), ptr_mesh_);
	assert(hr == S_OK && "Mesh ReadyComponent Failed");
	ptr_anim_ctrl_ = ptr_mesh_->CloneAnimController();

	// TODO: EnemyAIController 추가하기.

	return S_OK;
}

void CTyranid::Release()
{
	Engine::Safe_Delete(ptr_anim_ctrl_);
}

void CTyranid::UpdateState(float delta_time)
{
	Attack();
}

void CTyranid::UpdateAnimState()
{
}

void CTyranid::Run()
{
	//current_cell_index_ = Engine::GameManager()->MoveFromNavMesh(ptr_transform_->position(), ptr_transform_->move_dir(), current_cell_index_, 0);
}

void CTyranid::Attack()
{
}
