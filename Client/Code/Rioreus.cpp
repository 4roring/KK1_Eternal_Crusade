#include "stdafx.h"
#include "Rioreus.h"

#include "Transform.h"
#include "Shader.h"
#include "DynamicMesh.h"
#include "AnimController.h"
#include "PartCollider.h"
#include "EyeTrail.h"
#include "Rioreus_Tail.h"

#include "Font.h"

const float hide_tail_pos_y = 999999.f;

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

int & CRioreus::current_cell_index()
{
	return current_cell_index_;
}

int & CRioreus::head_condition()
{
	return head_condition_;
}

int & CRioreus::left_leg_condition()
{
	return left_leg_condition_;
}

int & CRioreus::right_leg_condition()
{
	return right_leg_condition_;
}

int & CRioreus::tail_condition()
{
	return tail_condition_;
}

const Matrix * CRioreus::ptr_mat_fire()
{
	return ptr_mat_fire_;
}

void CRioreus::SetState(State next_state)
{
	next_state_ = next_state;
}

void CRioreus::SetAnimSpeed(float anim_speed)
{
	anim_speed_ = anim_speed;
}

void CRioreus::SetAttackState(PartColl part, bool attack_state)
{
	ptr_part_coll_[part]->SetAttackState(attack_state);
}

HRESULT CRioreus::Initialize()
{
	HRESULT hr = CGameObject::Initialize();
	assert(!FAILED(hr) && "CGameObject::Initialize call failed in SpaceMarin");

	hr = AddComponent();
	assert(!FAILED(hr) && "AddComponent call failed in Rioreus");

	SetTail();

	next_state_ = State::Cry;
	anim_speed_ = 1.2f;

	ptr_mat_fire_ = ptr_mesh_->FindFrameMatrix("TN-00");

	max_hp_ = 5000;
	hp_ = max_hp_;

	Subject()->SetBossHp(&hp_);
	Subject()->SetBossPosition(&ptr_transform_->position());

#ifdef _DEBUG
	ptr_font_ = Engine::GraphicDevice()->GetFont(TEXT("¹ÙÅÁ"));
#endif // _DEBUG

	return S_OK;
}

void CRioreus::LateInit()
{
	if (current_cell_index_ == -1)
		current_cell_index_ = Engine::GameManager()->FindCellIndex(ptr_transform_->position());

	SetCollider();

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
	state_somersault_.InitState(this, ptr_transform_, ptr_anim_ctrl_);
	state_bite_attack_.InitState(this, ptr_transform_, ptr_anim_ctrl_);
	state_tail_cut_.InitState(this, ptr_transform_, ptr_anim_ctrl_);

	Matrix mat_scale, mat_rot, mat_init;
	D3DXMatrixScaling(&mat_scale, 0.01f, 0.01f, 0.01f);
	D3DXMatrixRotationY(&mat_rot, -1.57f);
	mat_init = mat_scale * mat_rot;
	mat_init._42 = -0.03f;
	ptr_mesh_->SetInitMatrix(mat_init);
}

void CRioreus::Update(float delta_time)
{
	CGameObject::Update(delta_time);
	anim_time_ = delta_time;
	if (nullptr != ptr_state_)
		ptr_state_->Update(delta_time);

	if (damage_delay_ >= 0.f)
		damage_delay_ = max(damage_delay_ - delta_time, 0.f);

	if (current_state_ == State::Dead && ptr_anim_ctrl_->CheckCurrentAnimationEnd(0.1))
		anim_speed_ = 0.f;
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

#ifdef _DEBUG
	TCHAR hp[128] = {};
	wsprintf(hp, TEXT("Rioreusaws HP: %d"), hp_);
	ptr_font_->Render(hp, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), Vector3(float(kWinCx >> 1), 10.f, 0.f));
#endif // _DEBUG

}

void CRioreus::ApplyDamage(int damage)
{
	if (current_state_ == State::Dead) return;

	if (damage_delay_ > 0.f)
		damage = 0;
	else if (damage >= 50)
		damage_delay_ = 0.6f;

	hp_ -= damage;

	if (false == rage_ && hp_ < (max_hp_ >> 1))
	{
		rage_ = true;
		const Matrix* ptr_target_local = ptr_mesh_->FindFrameMatrix("HD-102");

		TCHAR obj_key[64] = TEXT("");
		for (int i = 0; i < 2; ++i)
		{
			wsprintf(obj_key, TEXT("Eye_Trail_%d"), i);
			ptr_eye_trail_[i] = CEyeTrail::Create(ptr_device_, ptr_target_local, &ptr_transform_->mat_world(), (i == 1));
			Engine::GameManager()->GetCurrentScene()->AddObject(MAINTAIN_STAGE, obj_key, ptr_eye_trail_[i]);
			next_state_ = State::Cry;
		}
	}
	
	if(current_state_ != State::Somersault)
		UpdateConditionState();

	if (hp_ <= 0)
	{
		ptr_state_ = nullptr;
		next_state_ = State::Dead;
	}
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

void CRioreus::UpdateConditionState()
{
	constexpr int max_leg_condition = 15;
	constexpr int max_head_condition = 20;
	constexpr int max_tail_condition = 25;

	if (current_state_ == State::TailCut) return;

	if (current_state_ != State::Groggy &&left_leg_condition_ >= max_leg_condition)
	{
		ptr_state_->Reset();
		next_state_ = State::Fall;
		state_fall_.SetFallDirection(false);
		left_leg_condition_ = 0;
		right_leg_condition_ = 0;
	}
	else if (current_state_ != State::Groggy &&right_leg_condition_ >= max_head_condition)
	{
		ptr_state_->Reset();
		next_state_ = State::Fall;
		state_fall_.SetFallDirection(true);
		left_leg_condition_ = 0;
		right_leg_condition_ = 0;
	}
	else if (current_state_ != State::Fall && head_condition_ >= max_head_condition)
	{
		ptr_state_->Reset();
		next_state_ = State::Groggy;
		head_condition_ = 0;
	}

	if (cut_tail_ == false && tail_condition_ >= max_tail_condition)
	{
		ptr_state_->Reset();
		next_state_ = State::TailCut;
		CutTail();
		cut_tail_ = true;
	}
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
		case CRioreus::State::Bite_Attack:
			ptr_state_ = &state_bite_attack_;
			ptr_anim_ctrl_->SetAnimationTrack("Bite_Attack");
			break;
		case CRioreus::State::TailAttack:
			ptr_state_ = &state_tail_attack_;
			break;
		case CRioreus::State::Fire:
			ptr_state_ = &state_fire_;
			break;
		case CRioreus::State::Somersault:
			ptr_state_ = &state_somersault_;
			ptr_anim_ctrl_->SetAnimationTrack("Somersault_Attack_Start");
			break;
		case CRioreus::State::Fall:
			ptr_state_ = &state_fall_;
			break;
		case CRioreus::State::Groggy:
			ptr_state_ = &state_groggy_;
			ptr_anim_ctrl_->SetAnimationTrack("Groggy");
			break;
		case CRioreus::State::TailCut:
			ptr_state_ = &state_tail_cut_;
			ptr_anim_ctrl_->SetAnimationTrack("Tale_Cut");
			break;
		case CRioreus::State::Dead:
			ptr_anim_ctrl_->SetAnimationTrack("Dead");
			break;
		case CRioreus::State::End:
			break;
		default:
			break;
		}
		current_state_ = next_state_;
	}
}

void CRioreus::SetTail()
{
	Engine::BoneFrame* ptr_tail = ptr_mesh_->FindFrame("TL-05_t");
	Engine::BoneFrame* ptr_new_tail_parent = ptr_mesh_->FindFrame("TL-05");
	ptr_new_tail_parent->pFrameFirstChild = ptr_tail;

	ptr_tail = ptr_mesh_->FindFrame("TL-04_t");
	ptr_tail->pFrameFirstChild = ptr_new_tail_parent;
	ptr_new_tail_parent = ptr_mesh_->FindFrame("TL-04");
	ptr_new_tail_parent->pFrameFirstChild = ptr_tail;

	ptr_tail = ptr_mesh_->FindFrame("TL-03_t");
	ptr_tail->pFrameFirstChild = ptr_new_tail_parent;
	ptr_new_tail_parent = ptr_mesh_->FindFrame("TL-03");
	ptr_new_tail_parent->pFrameFirstChild = ptr_tail;

	Engine::BoneFrame* ptr_old_tail_parent = ptr_mesh_->FindFrame("NULL_T");
	ptr_old_tail_parent->pFrameFirstChild = nullptr;
}

void CRioreus::CutTail()
{
	Engine::BoneFrame* ptr_tail[3] = { nullptr };

	ptr_tail[0] = ptr_mesh_->FindFrame("TL-03_t");
	ptr_tail[1] = ptr_mesh_->FindFrame("TL-04_t");
	ptr_tail[2] = ptr_mesh_->FindFrame("TL-05_t");

	Engine::BoneFrame* ptr_tail_bone[3] = { nullptr };

	ptr_tail_bone[0] = ptr_mesh_->FindFrame("TL-03");
	ptr_tail_bone[1] = ptr_mesh_->FindFrame("TL-04");
	ptr_tail_bone[2] = ptr_mesh_->FindFrame("TL-05");

	ptr_tail[0]->pFrameFirstChild = ptr_tail[1];
	ptr_tail[1]->pFrameFirstChild = ptr_tail[2];

	ptr_tail_bone[0]->pFrameFirstChild = ptr_tail_bone[1];
	ptr_tail_bone[1]->pFrameFirstChild = ptr_tail_bone[2];
	ptr_tail_bone[2]->pFrameFirstChild = nullptr;

	for (int i = 0; i < 3; ++i)
		ptr_tail[i]->combined_matrix._42 = -hide_tail_pos_y;

	ptr_part_coll_[Part_Tail]->SetPartMatrix(&ptr_tail_bone[0]->combined_matrix);

	Vector3 tail_start_pos = *(Vector3*)&ptr_tail_bone[0]->combined_matrix.m[3][0];
	D3DXVec3TransformCoord(&tail_start_pos, &tail_start_pos, &ptr_transform_->mat_world());
	Engine::GameManager()->GetCurrentScene()->AddObject(MAINTAIN_STAGE, TEXT("Rioreus_Tail")
		, CRioreus_Tail::Create(ptr_device_, tail_start_pos, ptr_transform_->rotation()));
}

void CRioreus::SetCollider()
{
	const Matrix* ptr_part_mat = ptr_mesh_->FindFrameMatrix("HD-01");
	ptr_part_coll_[Part_Head] = CPartCollider::Create(ptr_device_, ptr_part_mat, Part_Head, this);

	ptr_part_mat = ptr_mesh_->FindFrameMatrix("BD-00");
	ptr_part_coll_[Part_Body] = CPartCollider::Create(ptr_device_, ptr_part_mat, Part_Body, this);

	ptr_part_mat = ptr_mesh_->FindFrameMatrix("LL-01");
	ptr_part_coll_[Part_Left_Leg] = CPartCollider::Create(ptr_device_, ptr_part_mat, Part_Left_Leg, this);
	ptr_part_mat = ptr_mesh_->FindFrameMatrix("LR-01");
	ptr_part_coll_[Part_Right_Leg] = CPartCollider::Create(ptr_device_, ptr_part_mat, Part_Right_Leg, this);
	
	ptr_part_mat = ptr_mesh_->FindFrameMatrix("AL-02");
	ptr_part_coll_[Part_Left_Wing] = CPartCollider::Create(ptr_device_, ptr_part_mat, Part_Left_Wing, this);
	ptr_part_mat = ptr_mesh_->FindFrameMatrix("AR-02");
	ptr_part_coll_[Part_Right_Wing] = CPartCollider::Create(ptr_device_, ptr_part_mat, Part_Right_Wing, this);

	ptr_part_mat = ptr_mesh_->FindFrameMatrix("TL-05");
	ptr_part_coll_[Part_Tail] = CPartCollider::Create(ptr_device_, ptr_part_mat, Part_Tail, this);

	TCHAR obj_key[128] = TEXT("");
	for (int i = 0; i < Part_End; ++i)
	{
		wsprintf(obj_key, TEXT("Part_Collider_%d"), i);
		Engine::GameManager()->GetCurrentScene()->AddObject(MAINTAIN_STAGE, obj_key, ptr_part_coll_[i]);
	}
}
