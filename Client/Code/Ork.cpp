#include "stdafx.h"
#include "Ork.h"

#include "Transform.h"
#include "Shader.h"
#include "DynamicMesh.h"
#include "AnimController.h"
#include "Collider.h"

#include "EnemyAIController.h"
#include "Ork_Gun.h"
#include "Ork_Sword.h"

COrk::COrk(LPDIRECT3DDEVICE9 ptr_device)
	: Engine::CGameObject(ptr_device)
{
}

COrk::~COrk()
{
	Release();
}

Engine::CTransform * COrk::ptr_lower_transform()
{
	return ptr_lower_transform_;
}

Vector3 & COrk::GetFirePos() const
{
	return ptr_gun_->GetFirePos();
}

int COrk::current_cell_index() const
{
	return current_cell_index_;
}

void COrk::set_fire(bool is_fire)
{
	fire_ = is_fire;
}

void COrk::set_slash(bool is_slash)
{
	slash_ = is_slash;
}

void COrk::set_fire_range_pos(const Vector3 & fire_range_pos)
{
	fire_range_pos_ = fire_range_pos;
}

HRESULT COrk::Initialize(int ctrl_id)
{
	HRESULT hr = CGameObject::Initialize();
	assert(!FAILED(hr) && "CGameObject::Initialize call failed in SpaceMarin");

	control_id_ = ctrl_id;
	hr = AddComponent();
	assert(!FAILED(hr) && "AddComponent call failed in SpaceMarin");

	ptr_upper_start_frame_ = ptr_mesh_->FindFrame("joint_TorsoA_01");
	ptr_lower_transform_->scale() = ptr_transform_->scale();

	ptr_head_frame_ = ptr_mesh_->FindFrame("joint_Head_01");
	ptr_body_frame_ = ptr_mesh_->FindFrame("joint_TorsoC_01");

	current_cell_index_ = -1;
	hp_ = 100;
	next_lower_state_ = LowerState::Idle;

	return S_OK;
}

void COrk::LateInit()
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

void COrk::Update(float delta_time)
{
	anim_time_ = delta_time;

	ptr_lower_transform_->position() = ptr_transform_->position();
	ptr_lower_transform_->rotation() = Vector3(0.f, ptr_transform_->rotation().y, 0.f);

	//ray_pos_ = ptr_gun_->GetFirePos();
	//ray_dir_ = (fire_range_pos_ - ray_pos_).Normalize();

	CollSystem()->AddRaycastList(nullptr, TAG_ENEMY);

	UpdateState(delta_time);
	Engine::CGameObject::Update(delta_time);
	CollSystem()->AddRaycastList(ptr_head_collider_, TAG_ENEMY);
	CollSystem()->AddRaycastList(ptr_body_collider_, TAG_ENEMY);
	CollSystem()->AddColisionList(ptr_body_collider_, TAG_ENEMY);
}

void COrk::LateUpdate()
{
	Engine::GameManager()->AddRenderLayer(Engine::RENDERLAYER::LAYER_NONEALPHA, this);
	Fire();
	Slash();
	UpdateLowerAnimState();
	UpdateUpperAnimState();
	CollSystem()->CollisionCheck(ptr_body_collider_, TAG_ENEMY);
}

void COrk::Render()
{
	LPD3DXEFFECT ptr_effect = ptr_shader_->GetEffectHandle();
	SetConstantTable(ptr_effect);

	Matrix mat_upper_rot_x;
	D3DXMatrixRotationAxis(&mat_upper_rot_x, &Vector3(-1.f, 0.f, 0.f), ptr_transform_->rotation().x);

	ptr_mesh_->FrameMove(anim_time_, ptr_lower_anim_ctrl_, ptr_upper_start_frame_, &mat_upper_rot_x);
	ptr_mesh_->FrameMove(anim_time_, ptr_upper_anim_ctrl_, ptr_upper_start_frame_);

	ptr_gun_->SetHandMatrix(ptr_left_hand_matrix_);
	ptr_sword_->SetHandMatrix(ptr_right_hand_matrix_);
	ptr_head_collider_->SetSphereCollider(0.2f, Vector3(*(Vector3*)&ptr_head_frame_->combined_matrix.m[3][0]));
	ptr_body_collider_->SetSphereCollider(0.5f, Vector3(*(Vector3*)&ptr_body_frame_->combined_matrix.m[3][0]));

	ptr_mesh_->RenderMesh(ptr_effect);

#ifdef _DEBUG
	DebugRender();
#endif // _DEBUG
}

void COrk::ApplyDamage(int damage)
{
	hp_ -= damage;

	if (hp_ <= 0)
	{
		next_lower_state_ = LowerState::Down;
		next_upper_state_ = UpperState::Down;
	}
}

COrk * COrk::Create(LPDIRECT3DDEVICE9 ptr_device, int ctrl_id)
{
	COrk* ptr_obj = new COrk(ptr_device);
	if (FAILED(ptr_obj->Initialize(ctrl_id)))
	{
		Safe_Delete(ptr_obj);
		assert(!"COrk Create Failed");
	}

	return ptr_obj;
}

HRESULT COrk::AddComponent()
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

	hr = Ready_Component(MAINTAIN_STAGE, TEXT("Ork_Mesh"), TEXT("Mesh"), ptr_mesh_);
	assert(hr == S_OK && "Mesh ReadyComponent Failed");
	ptr_lower_anim_ctrl_ = ptr_mesh_->CloneAnimController();
	ptr_upper_anim_ctrl_ = ptr_mesh_->CloneAnimController();

	// EnemyAIController
	ptr_ctrl_ = CEnemyAIController::Create(this, 4.f);
	CGameObject::AddComponent(TEXT("EnemyAIController"), ptr_ctrl_);
	if (nullptr != ptr_ctrl_)
	{
		TCHAR component_key[128] = TEXT("");
		wsprintf(component_key, TEXT("Component_EnemyAIController_%d"), control_id_);
		Engine::GameManager()->Add_Prototype(MAINTAIN_STAGE, component_key, ptr_ctrl_->CloneComponent());
	}

	// Collider
	ptr_head_collider_ = Engine::CCollider::Create(ptr_device_, this, Collider_Sphere);
	ptr_body_collider_ = Engine::CCollider::Create(ptr_device_, this, Collider_Sphere);

	return S_OK;
}

HRESULT COrk::AddWeapon()
{
	ptr_gun_ = COrk_Gun::Create(ptr_device_);
	ptr_left_hand_matrix_ = ptr_mesh_->FindFrameMatrix("joint_WeaponLT_01");
	if (nullptr == ptr_left_hand_matrix_) return E_FAIL;
	ptr_gun_->SetParentMatrix(&ptr_lower_transform_->mat_world());

	TCHAR object_key[128] = TEXT("");
	wsprintf(object_key, TEXT("Ork_Gun_%d"), control_id_);
	Engine::GameManager()->GetCurrentScene()->AddObject(MAINTAIN_STAGE, object_key, ptr_gun_);

	ptr_sword_ = COrk_Sword::Create(ptr_device_);
	ptr_right_hand_matrix_ = ptr_mesh_->FindFrameMatrix("joint_WeaponRT_01");
	if (nullptr == ptr_right_hand_matrix_) return E_FAIL;
	ptr_sword_->SetParentMatrix(&ptr_lower_transform_->mat_world());

	wsprintf(object_key, TEXT("Ork_Sword_%d"), control_id_);
	Engine::GameManager()->GetCurrentScene()->AddObject(MAINTAIN_STAGE, object_key, ptr_sword_);

	return S_OK;
}

void COrk::Release()
{
	Engine::Safe_Delete(ptr_lower_anim_ctrl_);
	Engine::Safe_Delete(ptr_upper_anim_ctrl_);

	Engine::Safe_Release_Delete(ptr_head_collider_);
	Engine::Safe_Release_Delete(ptr_body_collider_);

#ifdef _DEBUG
	Engine::Safe_Release(ptr_line_);
#endif
}

void COrk::UpdateState(float delta_time)
{
	switch (pre_lower_state_)
	{
	case LowerState::Idle:
		if (ptr_transform_->move_dir().x != 0.f || ptr_transform_->move_dir().z != 0.f)
		{
			next_lower_state_ = LowerState::Run;
			next_upper_state_ = UpperState::Run;
		}
		break;
	case LowerState::Run:
		Run();
		if (ptr_transform_->move_dir().x == 0.f && ptr_transform_->move_dir().z == 0.f)
		{
			next_lower_state_ = LowerState::Idle;
			next_upper_state_ = UpperState::Idle;
		}
		break;
	case LowerState::Attack_1:
	case LowerState::Attack_2:
		AttackState(delta_time);
		break;
	case LowerState::Down:
		if (ptr_upper_anim_ctrl_->GetPeriod() - 0.1 <= ptr_upper_anim_ctrl_->GetTrackPosition())
		{
			next_upper_state_ = UpperState::Down_Dead;
			next_lower_state_ = LowerState::Down_Dead;
			ptr_transform_->move_dir() = Vector3();
		}
		break;
	case LowerState::Down_Dead:
		if (ptr_upper_anim_ctrl_->GetPeriod() - 0.1 <= ptr_upper_anim_ctrl_->GetTrackPosition())
			anim_time_ = 0.f;
		break;
	}
}

void COrk::AttackState(float delta_time)
{
	anim_time_ *= 0.7f;
	ptr_sword_->ptr_sphere_coll()->enable_ = true;
	if (ptr_upper_anim_ctrl_->GetPeriod() - 0.1 <= ptr_upper_anim_ctrl_->GetTrackPosition())
	{
		anim_time_ = 0.f;
		attack_delay_ += delta_time;
		ptr_sword_->ptr_sphere_coll()->enable_ = false;
		if (attack_delay_ >= 1.f)
		{
			next_lower_state_ = LowerState::Idle;
			next_upper_state_ = UpperState::Idle;
			ptr_transform_->move_dir() = Vector3();
			condition_ = 0;
		}
	}
	else
	{
		ptr_transform_->move_dir() = ptr_transform_->Forward() * 1.f * delta_time;
		Run();
	}

}

void COrk::UpdateLowerAnimState()
{
	if (pre_lower_state_ != next_lower_state_)
	{
		switch (next_lower_state_)
		{
		case LowerState::Idle:
			ptr_lower_anim_ctrl_->SetAnimationTrack("Idle");
			break;
		case LowerState::Run:
			ptr_lower_anim_ctrl_->SetAnimationTrack("Run");
			break;
		case LowerState::Attack_1:
			ptr_lower_anim_ctrl_->SetAnimationTrack("Attack_1");
			break;
		case LowerState::Attack_2:
			ptr_lower_anim_ctrl_->SetAnimationTrack("Attack_2");
			break;
		case LowerState::Down:
			ptr_lower_anim_ctrl_->SetAnimationTrack("Down_In");
			break;
		case LowerState::Down_Dead:
			ptr_lower_anim_ctrl_->SetAnimationTrack("Down_Death");
			break;
		}
		ptr_lower_anim_ctrl_->SetTrackPosition(0.0);
		pre_lower_state_ = next_lower_state_;
	}

}

void COrk::UpdateUpperAnimState()
{
	if (pre_upper_state_ != next_upper_state_)
	{
		switch (next_upper_state_)
		{
		case UpperState::Idle:
			ptr_upper_anim_ctrl_->SetAnimationTrack("Idle");
			break;
		case UpperState::Shoot:
			ptr_upper_anim_ctrl_->SetAnimationTrack("Idle_Aim");
			break;
		case UpperState::Run:
			ptr_upper_anim_ctrl_->SetAnimationTrack("Run");
			break;
		case UpperState::Run_Aiming:
			ptr_upper_anim_ctrl_->SetAnimationTrack("Run_Aim");
			break;
		case UpperState::Attack_1:
			ptr_upper_anim_ctrl_->SetAnimationTrack("Attack_1");
			break;
		case UpperState::Attack_2:
			ptr_upper_anim_ctrl_->SetAnimationTrack("Attack_2");
			break;
		case UpperState::Down:
			ptr_upper_anim_ctrl_->SetAnimationTrack("Down_In");
			break;
		case UpperState::Down_Dead:
			ptr_upper_anim_ctrl_->SetAnimationTrack("Down_Death");
			break;
		}
		ptr_upper_anim_ctrl_->SetTrackPosition(0.0);
		pre_upper_state_ = next_upper_state_;
	}
}

void COrk::Run()
{
	if (pre_lower_state_ == LowerState::Down_Dead) return;
	current_cell_index_ = Engine::GameManager()->MoveFromNavMesh(ptr_transform_->position(), ptr_transform_->move_dir(), current_cell_index_, 0);
}

void COrk::Fire()
{
	if (true == fire_)
	{
		Vector3 fire_dir = transform()->Forward();
		fire_dir.x += random_range(-30, 30) * 0.01f;
		fire_dir.y += random_range(-30, 30) * 0.01f;
		fire_dir = fire_dir.Normalize();

		switch (pre_upper_state_)
		{
		case UpperState::Idle:
			next_upper_state_ = UpperState::Shoot;
			break;
		case UpperState::Run:
			next_upper_state_ = UpperState::Run_Aiming;
			break;
		case UpperState::Shoot:
		case UpperState::Run_Aiming:
			// Fire!!!
			ray_pos_ = ptr_gun_->GetFirePos();
			ray_dir_ = fire_dir * 5.f;
			fire_ = false;
			break;
		}
	}
}

void COrk::Slash()
{
	if (true == slash_)
	{
		if (condition_++ == 0)
		{
			next_upper_state_ = UpperState::Attack_1;
			next_lower_state_ = LowerState::Attack_1;
		}
		else
		{
			next_upper_state_ = UpperState::Attack_2;
			next_lower_state_ = LowerState::Attack_2;
			condition_ = 0;
		}
		slash_ = false;
	}
}

void COrk::SetConstantTable(LPD3DXEFFECT ptr_effect)
{
	Matrix mat_view, mat_proj;
	ptr_device_->GetTransform(D3DTS_VIEW, &mat_view);
	ptr_device_->GetTransform(D3DTS_PROJECTION, &mat_proj);

	ptr_effect->SetMatrix("g_mat_world", &ptr_lower_transform_->mat_world());
	ptr_effect->SetMatrix("g_mat_view", &mat_view);
	ptr_effect->SetMatrix("g_mat_projection", &mat_proj);

	ptr_effect->SetVector("g_light_diffuse", &Vector4(1.f, 1.f, 1.f, 1.f));
	ptr_effect->SetVector("g_light_ambient", &Vector4(1.f, 1.f, 1.f, 1.f));
	ptr_effect->SetVector("g_light_dir", &Vector4(0.f, -1.f, 1.f, 0.f));
}

#ifdef _DEBUG
void COrk::DebugRender()
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

	Matrix mat_view, mat_proj;
	ptr_device_->GetTransform(D3DTS_VIEW, &mat_view);
	ptr_device_->GetTransform(D3DTS_PROJECTION, &mat_proj);

	Vector3 line_[2] = { ray_pos_, ray_pos_ + ray_dir_};

	for (auto& point : line_)
	{
		D3DXVec3TransformCoord(&point, &point, &mat_view);
		if (point.z < 0.f)
			point.z = 0.f;
		D3DXVec3TransformCoord(&point, &point, &mat_proj);
	}

	ptr_line_->SetWidth(2.f);
	ptr_line_->Begin();

	ptr_line_->DrawTransform(line_, 2, nullptr, D3DXCOLOR(1.f, 1.f, 0.f, 1.f));

	ptr_line_->End();
}
#endif // _DEBUG
