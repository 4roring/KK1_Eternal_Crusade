#include "stdafx.h"
#include "Ork_WarBoss.h"

#include "Transform.h"
#include "Shader.h"
#include "DynamicMesh.h"
#include "AnimController.h"
#include "Collider.h"

#include "Ork_Gun.h"

COrk_WarBoss::COrk_WarBoss(LPDIRECT3DDEVICE9 ptr_device)
	: Engine::CGameObject(ptr_device)
{
}

COrk_WarBoss::~COrk_WarBoss()
{
	Release();
}

Engine::CTransform * COrk_WarBoss::ptr_lower_transform()
{
	return ptr_lower_transform_;
}

int COrk_WarBoss::current_cell_index() const
{
	return current_cell_index_;
}

HRESULT COrk_WarBoss::Initialize()
{
	HRESULT hr = CGameObject::Initialize();
	assert(!FAILED(hr) && "CGameObject::Initialize call failed in SpaceMarin");

	hr = AddComponent();
	assert(!FAILED(hr) && "AddComponent call failed in SpaceMarin");

	ptr_upper_start_frame_ = ptr_mesh_->FindFrame("joint_TorsoA_01");
	ptr_transform_->scale() = Vector3(1.5f, 1.5f, 1.5f);
	ptr_lower_transform_->scale() = ptr_transform_->scale();

	ptr_head_frame_ = ptr_mesh_->FindFrame("joint_Head_01");
	ptr_body_frame_ = ptr_mesh_->FindFrame("joint_TorsoC_01");

	current_cell_index_ = -1;
	hp_ = 10000;
	next_behavior_pattern_ = BehaviorPattern::Idle;

	return S_OK;
}

void COrk_WarBoss::LateInit()
{
	HRESULT hr = AddWeapon();
	assert(!FAILED(hr) && "Ork Add Weapon Failed");

	if (current_cell_index_ == -1)
		current_cell_index_ = Engine::GameManager()->FindCellIndex(ptr_transform_->position());

#ifdef _DEBUG
	D3DXCreateLine(ptr_device_, &ptr_line_);
	ptr_line_->SetAntialias(TRUE);
#endif

}

void COrk_WarBoss::Update(float delta_time)
{
	anim_time_ = delta_time;

	ptr_lower_transform_->position() = ptr_transform_->position();
	ptr_lower_transform_->rotation() = Vector3(0.f, ptr_transform_->rotation().y, 0.f);

	UpdateState(delta_time);
	Engine::CGameObject::Update(delta_time);
}

void COrk_WarBoss::LateUpdate()
{
	Engine::GameManager()->AddRenderLayer(Engine::RENDERLAYER::LAYER_NONEALPHA, this);

	UpdateAnimState();
}

void COrk_WarBoss::Render()
{
	LPD3DXEFFECT ptr_effect = ptr_shader_->GetEffectHandle();
	SetConstantTable(ptr_effect);

	Matrix mat_upper_rot_x;
	D3DXMatrixRotationAxis(&mat_upper_rot_x, &Vector3(-1.f, 0.f, 0.f), ptr_transform_->rotation().x);

	ptr_mesh_->FrameMove(anim_time_, ptr_lower_anim_ctrl_, ptr_upper_start_frame_, &mat_upper_rot_x);
	ptr_mesh_->FrameMove(anim_time_, ptr_upper_anim_ctrl_, ptr_upper_start_frame_);

	ptr_gun_->SetHandMatrix(ptr_left_hand_matrix_);
	ptr_head_collider_->SetSphereCollider(0.4f, *(Vector3*)&ptr_head_frame_->combined_matrix.m[3][0]);
	ptr_body_collider_->SetSphereCollider(0.7f, *(Vector3*)&ptr_body_frame_->combined_matrix.m[3][0]);

	ptr_mesh_->RenderMesh(ptr_effect);

#ifdef _DEBUG
	DebugRender();
#endif // _DEBUG

}

void COrk_WarBoss::ApplyDamage(int damage)
{
	hp_ -= damage;

	if (hp_ <= 50)
		next_behavior_pattern_ = BehaviorPattern::Down;
}

COrk_WarBoss * COrk_WarBoss::Create(LPDIRECT3DDEVICE9 ptr_device)
{
	COrk_WarBoss* ptr_obj = new COrk_WarBoss(ptr_device);
	if (FAILED(ptr_obj->Initialize()))
	{
		Safe_Delete(ptr_obj);
		assert(!"COrk Create Failed");
	}

	return ptr_obj;
}

HRESULT COrk_WarBoss::AddComponent()
{
	HRESULT hr = E_FAIL;

	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Component_Transform"), TEXT("Transform"), ptr_transform_);
	assert(hr == S_OK && "Tranform Component ReadyComponent Failed");
	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Component_Transform"), TEXT("Lower_Transform"), ptr_lower_transform_);
	assert(hr == S_OK && "Tranform Component ReadyComponent Failed");

	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Shader_DynamicMesh"), TEXT("Shader"), ptr_shader_);
	assert(hr == S_OK && "Ork Shader_DynamicMesh ReadyComponent Failed");
	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Shader_Default"), TEXT("Debug_Shader"), ptr_debug_shader_);
	assert(hr == S_OK && "Ork Shader_Default ReadyComponent Failed");

	hr = Ready_Component(MAINTAIN_STAGE, TEXT("Ork_WarBoss_Mesh"), TEXT("Mesh"), ptr_mesh_);
	assert(hr == S_OK && "Mesh ReadyComponent Failed");
	ptr_lower_anim_ctrl_ = ptr_mesh_->CloneAnimController();
	ptr_upper_anim_ctrl_ = ptr_mesh_->CloneAnimController();

	// Collider
	ptr_head_collider_ = Engine::CCollider::Create(ptr_device_, this, Collider_Sphere);
	ptr_body_collider_ = Engine::CCollider::Create(ptr_device_, this, Collider_Sphere);

	return S_OK;

}

HRESULT COrk_WarBoss::AddWeapon()
{
	// TODO: HeavyGun, Klaw Ãß°¡
	ptr_gun_ = COrk_Gun::Create(ptr_device_);
	ptr_left_hand_matrix_ = ptr_mesh_->FindFrameMatrix("joint_WeaponLT_01");
	if (nullptr == ptr_left_hand_matrix_) return E_FAIL;
	ptr_gun_->SetParentMatrix(&ptr_lower_transform_->mat_world());
	Engine::GameManager()->GetCurrentScene()->AddObject(MAINTAIN_STAGE, TEXT("WarBoss_Gun"), ptr_gun_);

	return S_OK;
}

void COrk_WarBoss::Release()
{
	Engine::Safe_Delete(ptr_lower_anim_ctrl_);
	Engine::Safe_Delete(ptr_upper_anim_ctrl_);

	Engine::Safe_Release_Delete(ptr_head_collider_);
	Engine::Safe_Release_Delete(ptr_body_collider_);

#ifdef _DEBUG
	Engine::Safe_Release(ptr_line_);
#endif

}

void COrk_WarBoss::UpdateState(float delta_time)
{
	switch (pre_behavior_pattern_)
	{
	case BehaviorPattern::Spawn:
		break;
	case BehaviorPattern::Idle:

		break;
	case BehaviorPattern::Shoot:
		break;
	case BehaviorPattern::Attack:
		break;
	case BehaviorPattern::Ground_Attack:
		break;
	case BehaviorPattern::Skill:
		break;
	case BehaviorPattern::Down:
		break;
	case BehaviorPattern::Victim:
		break;
	case BehaviorPattern::End:
		break;
	default:
		break;
	}
}

void COrk_WarBoss::UpdateAnimState()
{
	if (pre_behavior_pattern_ != next_behavior_pattern_)
	{
		switch (pre_behavior_pattern_)
		{
		case BehaviorPattern::Spawn:
			break;
		case BehaviorPattern::Idle:
			ptr_upper_anim_ctrl_->SetAnimationTrack("Heavy_Idle");
			ptr_lower_anim_ctrl_->SetAnimationTrack("Heavy_Idle");
			break;
		case BehaviorPattern::Shoot:
			break;
		case BehaviorPattern::Attack:
			break;
		case BehaviorPattern::Ground_Attack:
			break;
		case BehaviorPattern::Skill:
			break;
		case BehaviorPattern::Down:
			break;
		case BehaviorPattern::Victim:
			break;
		case BehaviorPattern::End:
			break;
		default:
			break;
		}
		pre_behavior_pattern_ = next_behavior_pattern_;
	}
}

void COrk_WarBoss::SetConstantTable(LPD3DXEFFECT ptr_effect)
{
	Matrix mat_view, mat_proj;
	ptr_device_->GetTransform(D3DTS_VIEW, &mat_view);
	ptr_device_->GetTransform(D3DTS_PROJECTION, &mat_proj);

	ptr_effect->SetMatrix("g_mat_world", &ptr_lower_transform_->mat_world());
	ptr_effect->SetMatrix("g_mat_view", &mat_view);
	ptr_effect->SetMatrix("g_mat_projection", &mat_proj);
}

#ifdef _DEBUG
void COrk_WarBoss::DebugRender()
{
	LPD3DXEFFECT ptr_effect = ptr_debug_shader_->GetEffectHandle();
	Matrix mat_coll;
	D3DXMatrixIdentity(&mat_coll);
	mat_coll._41 = ptr_head_collider_->GetSpherePos().x;
	mat_coll._42 = ptr_head_collider_->GetSpherePos().y;
	mat_coll._43 = ptr_head_collider_->GetSpherePos().z;
	ptr_effect->SetMatrix("g_mat_world", &mat_coll);

	ptr_debug_shader_->BegineShader(1);
	ptr_head_collider_->DebugRender();
	ptr_debug_shader_->EndShader();

	mat_coll._41 = ptr_body_collider_->GetSpherePos().x;
	mat_coll._42 = ptr_body_collider_->GetSpherePos().y;
	mat_coll._43 = ptr_body_collider_->GetSpherePos().z;
	ptr_effect->SetMatrix("g_mat_world", &mat_coll);

	ptr_debug_shader_->BegineShader(1);
	ptr_body_collider_->DebugRender();
	ptr_debug_shader_->EndShader();

	//Matrix mat_view, mat_proj;
	//ptr_device_->GetTransform(D3DTS_VIEW, &mat_view);
	//ptr_device_->GetTransform(D3DTS_PROJECTION, &mat_proj);

	//Vector3 line_[2] = { ray_pos_, ray_pos_ + ray_dir_ };

	//for (auto& point : line_)
	//{
	//	D3DXVec3TransformCoord(&point, &point, &mat_view);
	//	if (point.z < 0.f)
	//		point.z = 0.f;
	//	D3DXVec3TransformCoord(&point, &point, &mat_proj);
	//}

	//ptr_line_->SetWidth(2.f);
	//ptr_line_->Begin();

	//ptr_line_->DrawTransform(line_, 2, nullptr, D3DXCOLOR(1.f, 1.f, 0.f, 1.f));

	//ptr_line_->End();

}
#endif // _DEBUG

