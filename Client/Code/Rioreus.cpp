#include "stdafx.h"
#include "Rioreus.h"

#include "Transform.h"
#include "Shader.h"
#include "DynamicMesh.h"
#include "AnimController.h"

CRioreus::CRioreus(LPDIRECT3DDEVICE9 ptr_device)
	: Engine::CGameObject(ptr_device)
{
}

CRioreus::~CRioreus()
{
	Release();
}

bool CRioreus::rage() const 
{
	return rage_;
}

float& CRioreus::pot_y()
{
	return pos_y_;
}

int & CRioreus::current_cell_index()
{
	return current_cell_index_;
}

void CRioreus::SetState(State next_state)
{
	next_state_ = next_state;
}

void CRioreus::SetAnimSpeed(float anim_speed)
{
	anim_speed_ = anim_speed;
}

HRESULT CRioreus::Initialize()
{
	HRESULT hr = CGameObject::Initialize();
	assert(!FAILED(hr) && "CGameObject::Initialize call failed in SpaceMarin");

	hr = AddComponent();
	assert(!FAILED(hr) && "AddComponent call failed in Rioreus");

	SetTaleBone();

	next_state_ = State::Idle;
	anim_speed_ = 1.f;

	return S_OK;
}

void CRioreus::LateInit()
{
	if (current_cell_index_ == -1)
		current_cell_index_ = Engine::GameManager()->FindCellIndex(ptr_transform_->position());

	// Static State Initializaion
	state_idle_.InitState(this, ptr_transform_, ptr_anim_ctrl_);
	state_rotate_.InitState(this, ptr_transform_, ptr_anim_ctrl_);
	state_walk_.InitState(this, ptr_transform_, ptr_anim_ctrl_);
	state_cry_.InitState(this, ptr_transform_, ptr_anim_ctrl_);
	state_sprint_.InitState(this, ptr_transform_, ptr_anim_ctrl_);
	state_tail_attack_.InitState(this, ptr_transform_, ptr_anim_ctrl_);
	state_fall_.InitState(this, ptr_transform_, ptr_anim_ctrl_);
	state_fire_.InitState(this, ptr_transform_, ptr_anim_ctrl_);
	state_groggy_.InitState(this, ptr_transform_, ptr_anim_ctrl_);

	Matrix mat_scale, mat_rot, mat_init;
	D3DXMatrixScaling(&mat_scale, 0.01f, 0.01f, 0.01f);
	D3DXMatrixRotationY(&mat_rot, -1.57f);
	mat_init = mat_scale * mat_rot;
	mat_init._42 = -0.05f;
	ptr_mesh_->SetInitMatrix(mat_init);
}

void CRioreus::Update(float delta_time)
{
	CGameObject::Update(delta_time);
	anim_time_ = delta_time;
	if (nullptr != ptr_state_)
		ptr_state_->Update(delta_time);
}

void CRioreus::LateUpdate()
{
	Engine::GameManager()->AddRenderLayer(Engine::RENDERLAYER::LAYER_NONEALPHA, this);
	UpdateState();
}

void CRioreus::Render()
{
	LPD3DXEFFECT ptr_effect = ptr_shader_->GetEffectHandle();
	SetConstantTable(ptr_effect);

	ptr_mesh_->FrameMove(anim_time_ * anim_speed_, ptr_anim_ctrl_);
	ptr_mesh_->RenderMesh_Soft(ptr_effect, 2);
}

void CRioreus::ApplyDamage(int damage)
{
	// TODO 다리에 누적데미지에 따라 Fall 상태로 가면서 true는 오른다리, false는 왼다리 걸려넘어지기.

}

CRioreus * CRioreus::Create(LPDIRECT3DDEVICE9 ptr_device)
{
	CRioreus* ptr_obj = new CRioreus(ptr_device);
	if (FAILED(ptr_obj->Initialize()))
	{
		Safe_Delete(ptr_obj);
		assert(!"Rioreus Create Failed");
	}

	return ptr_obj;
}

HRESULT CRioreus::AddComponent()
{
	HRESULT hr = E_FAIL;

	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Component_Transform"), TEXT("Transform"), ptr_transform_);
	assert(hr == S_OK && "Tranform Component ReadyComponent Failed");
	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Shader_NormalMap"), TEXT("Shader"), ptr_shader_);
	assert(hr == S_OK && "Ork Shader_DynamicMesh ReadyComponent Failed");
	hr = Ready_Component(MAINTAIN_STAGE, TEXT("Rioreus_Mesh"), TEXT("Mesh"), ptr_mesh_);
	assert(hr == S_OK && "Mesh ReadyComponent Failed");
	ptr_anim_ctrl_ = ptr_mesh_->CloneAnimController();

	return S_OK;
}

void CRioreus::Release()
{
	Engine::Safe_Delete(ptr_anim_ctrl_);
}

void CRioreus::SetConstantTable(LPD3DXEFFECT ptr_effect)
{
	Matrix mat_view, mat_proj;
	ptr_device_->GetTransform(D3DTS_VIEW, &mat_view);
	ptr_device_->GetTransform(D3DTS_PROJECTION, &mat_proj);

	ptr_effect->SetMatrix("g_mat_world", &ptr_transform_->mat_world());
	ptr_effect->SetMatrix("g_mat_view", &mat_view);
	ptr_effect->SetMatrix("g_mat_projection", &mat_proj);
}

void CRioreus::UpdateState()
{
	if (current_state_ != next_state_)
	{
		switch (next_state_)
		{
		case CRioreus::State::Cry:
			ptr_state_ = &state_cry_;
			ptr_anim_ctrl_->SetAnimationTrack("Cry");
			break;
		case CRioreus::State::Idle:
			ptr_state_ = &state_idle_;
			ptr_anim_ctrl_->SetAnimationTrack("Idle");
			break;
		case CRioreus::State::Rotate:
			ptr_state_ = &state_rotate_;
			break;
		case CRioreus::State::Walk:
			ptr_state_ = &state_walk_;
			ptr_anim_ctrl_->SetAnimationTrack("Walk");
			break;
		case CRioreus::State::Sprint:
			ptr_state_ = &state_sprint_;
			ptr_anim_ctrl_->SetAnimationTrack("Sprint");
			break;
		case CRioreus::State::TailAttack:
			ptr_state_ = &state_tail_attack_;
			break;
		case CRioreus::State::Fire:
			ptr_state_ = &state_fire_;
			break;
		case CRioreus::State::Glide:
			break;
		case CRioreus::State::Sumersault:
			break;
		case CRioreus::State::Fall:
			ptr_state_ = &state_fall_;
			break;
		case CRioreus::State::Groggy:
			ptr_state_ = &state_groggy_;
			ptr_anim_ctrl_->SetAnimationTrack("Groggy");
			break;
		case CRioreus::State::TailCut:
			break;
		case CRioreus::State::Dead:
			break;
		case CRioreus::State::End:
			break;
		default:
			break;
		}

		current_state_ = next_state_;
	}
}

void CRioreus::SetTaleBone()
{
	Engine::BoneFrame* ptr_tail_ = ptr_mesh_->FindFrame("TL-05_t");
	Engine::BoneFrame* ptr_new_tail_parent = ptr_mesh_->FindFrame("TL-05");
	ptr_new_tail_parent->pFrameFirstChild = ptr_tail_;

	ptr_tail_ = ptr_mesh_->FindFrame("TL-04_t");
	ptr_tail_->pFrameFirstChild = ptr_new_tail_parent;
	ptr_new_tail_parent = ptr_mesh_->FindFrame("TL-04");
	ptr_new_tail_parent->pFrameFirstChild = ptr_tail_;

	ptr_tail_ = ptr_mesh_->FindFrame("TL-03_t");
	ptr_tail_->pFrameFirstChild = ptr_new_tail_parent;
	ptr_new_tail_parent = ptr_mesh_->FindFrame("TL-03");
	ptr_new_tail_parent->pFrameFirstChild = ptr_tail_;

	Engine::BoneFrame* ptr_old_tail_parent = ptr_mesh_->FindFrame("NULL_T");
	ptr_old_tail_parent->pFrameFirstChild = nullptr;
}
