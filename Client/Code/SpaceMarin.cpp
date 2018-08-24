#include "stdafx.h"
#include "SpaceMarin.h"
#include "Scene.h"

#include "Transform.h"
#include "Shader.h"
#include "DynamicMesh.h"
#include "AnimController.h"
#include "Collider.h"

#include "PlayerController.h"
#include "Gun_Phobos.h"
#include "ChainSword.h"
#include "Granade.h"

#include "FireEffect.h"
#include "FireLineSmoke.h"
#include "HitEffect.h"
#include "MoveDust.h"

#include "UI_Hp.h"
#include "UI_Shield.h"
#include "UI_Weapon.h"
#include "UI_Aim.h"
#include "CUI_Damage.h"

CSpaceMarin::CSpaceMarin(LPDIRECT3DDEVICE9 ptr_device)
	: Engine::CGameObject(ptr_device)
{
}

CSpaceMarin::~CSpaceMarin()
{
	Release();
}

Engine::CTransform * CSpaceMarin::ptr_lower_transform()
{
	return ptr_lower_transform_;
}

Vector3 & CSpaceMarin::GetFirePos() const
{
	return ptr_gun_->GetFirePos();
}

int CSpaceMarin::current_cell_index() const
{
	return current_cell_index_;
}

CSpaceMarin::Weapon CSpaceMarin::weapon() const
{
	return weapon_;
}

bool CSpaceMarin::evade() const
{
	return evade_;
}

bool CSpaceMarin::execution() const
{
	return execution_;
}

void CSpaceMarin::set_move_dir(MoveDirection move_dir)
{
	next_move_dir_ = move_dir;
}

void CSpaceMarin::set_zoom(bool is_zoom)
{
	zoom_ = is_zoom;
}

void CSpaceMarin::set_fire(bool is_fire)
{
	fire_ = is_fire;
}

void CSpaceMarin::set_fire_range_pos(const Vector3 & fire_range_pos)
{
	fire_range_pos_ = fire_range_pos;
}

void CSpaceMarin::set_weapon()
{
	pre_lower_state_ = LowerState::End;
	pre_upper_state_ = UpperState::End;

	switch (weapon_)
	{
	case Weapon::Gun:
		weapon_ = Weapon::ChainSword;
		ptr_gun_->SetActive(false);
		ptr_sword_->SetActive(true);
		zoom_ = false;
		break;
	case Weapon::ChainSword:
		weapon_ = Weapon::Gun;
		ptr_gun_->SetActive(true);
		ptr_sword_->SetActive(false);
		break;
	}
}

void CSpaceMarin::set_evade_dir(const Vector3 & evade_dir)
{
	evade_dir_ = evade_dir;
}

void CSpaceMarin::set_evade(bool is_evade)
{
	evade_ = is_evade;
}

void CSpaceMarin::set_execution()
{
	execution_ = true;
}

void CSpaceMarin::set_execution_target(const Vector3 & target_pos)
{
	execution_target_ = target_pos;
}

void CSpaceMarin::set_next_rot_y(float next_rot_y)
{
	next_rot_y_ = next_rot_y;
}

void CSpaceMarin::set_toss()
{
	toss_ = true;
}

void CSpaceMarin::set_reload()
{
	if (weapon_ == Weapon::Gun && evade_ == false && bullet_count_ != max_bullet_)
	{
		Sound()->PlaySound(ptr_reload_sound_, Sound()->CHANNEL_PLAYER);
		reload_ = true;
	}
}

void CSpaceMarin::SetRay(const Vector3 & ray_pos, const Vector3 & ray_dir)
{
	ray_pos_ = ray_pos;
	ray_dir_ = ray_dir;
}

HRESULT CSpaceMarin::Initialize(int ctrl_id)
{
	HRESULT hr = CGameObject::Initialize();
	assert(!FAILED(hr) && "CGameObject::Initialize call failed in SpaceMarin");

	control_id_ = ctrl_id;
	hr = AddComponent();
	assert(!FAILED(hr) && "AddComponent call failed in SpaceMarin");

	ptr_lower_anim_ctrl_->SetAnimationTrack("Idle");
	ptr_upper_anim_ctrl_->SetAnimationTrack("Idle");

	ptr_upper_start_frame_ = ptr_mesh_->FindFrame("joint_TorsoA_01");
	ptr_lower_transform_->scale() = ptr_transform_->scale();

	ptr_head_collider_->SetWorld(&ptr_lower_transform_->mat_world());
	ptr_head_matrix_ = ptr_mesh_->FindFrameMatrix("joint_NeckCounter01_01");
	ptr_body_collider_->SetWorld(&ptr_lower_transform_->mat_world());
	ptr_body_matrix_ = ptr_mesh_->FindFrameMatrix("joint_TorsoB_01");
	ptr_left_foot_matrix_ = ptr_mesh_->FindFrameMatrix("joint_FootLT_01");
	ptr_right_foot_matrix_ = ptr_mesh_->FindFrameMatrix("joint_FootRT_01");

	current_cell_index_ = -1;

	next_lower_state_ = LowerState::Idle;

	color_[0] = Vector4(0.19f, 0.24f, 0.35f, 1.f);
	color_[1] = Vector4(0.7f, 0.7f, 0.7f, 1.f);
	color_[2] = Vector4(0.9f, 0.79f, 0.46f, 1.f);

	max_hp_ = 100;
	hp_ = max_hp_;
	max_shield_ = 100;
	shield_ = max_shield_;

	max_bullet_ = 30;
	bullet_count_ = max_bullet_;

	max_granade_ = 2;
	granade_count_ = max_granade_;

	return S_OK;
}

void CSpaceMarin::LateInit()
{
	HRESULT hr = AddWeapon();
	assert(!FAILED(hr) && "SpaceMarin Add Weapon Failed");

	if (current_cell_index_ == -1)
		current_cell_index_ = Engine::GameManager()->FindCellIndex(ptr_transform_->position());

	weapon_ = Weapon::Gun;
	ptr_sword_->SetActive(false);

	if (0 == control_id_)
	{
		hr = AddUI();
		assert(!FAILED(hr) && "Player UI Create Failed");
	}

	ptr_gun_sound_ = Sound()->FindSound(TEXT("SpaceMarin_Gun"));
	ptr_sword_sound_ = Sound()->FindSound(TEXT("Chain_Sword"));
	ptr_run_sound_ = Sound()->FindSound(TEXT("SpaceMarin_Run"));
	ptr_evade_sound_ = Sound()->FindSound(TEXT("Evade"));
	ptr_reload_sound_ = Sound()->FindSound(TEXT("Reload"));
	ptr_toss_sound_ = Sound()->FindSound(TEXT("Toss"));

	CGameObject::Update(0.f);

#ifdef _DEBUG
	D3DXCreateLine(ptr_device_, &ptr_line_);
	ptr_line_->SetAntialias(TRUE);
#endif
}

void CSpaceMarin::Update(float delta_time)
{
	delta_time_ = delta_time;
	shield_recovery_time_ -= 10.f * delta_time;
	if (damage_delay_ > 0.f) damage_delay_ -= delta_time;
	if (shield_recovery_time_ <= 0.f)
	{
		shield_ = min(shield_ + 1, max_shield_);
		shield_recovery_time_ = 1.f;
	}

	ptr_lower_transform_->position() = ptr_transform_->position();
	if (pre_lower_state_ == LowerState::Idle)
	{
		if (fabs(ptr_transform_->rotation().y - ptr_lower_transform_->rotation().y) > 0.78f)
		{
			next_rot_y_ = ptr_transform_->rotation().y - ptr_lower_transform_->rotation().y;
			next_lower_state_ = LowerState::Turn;

		}
	}
	else if (pre_lower_state_ != LowerState::Turn)
		ptr_lower_transform_->rotation() = Vector3(0.f, ptr_transform_->rotation().y, 0.f);

	if (false == evade_)
	{
		CollSystem()->AddRaycastList(ptr_head_collider_, TAG_PLAYER);
		CollSystem()->AddRaycastList(ptr_body_collider_, TAG_PLAYER);
	}
	CollSystem()->AddColliderList(ptr_body_collider_, TAG_PLAYER);
	ptr_body_collider_->set_delta_time(delta_time);

	UpdateState(delta_time);
	Engine::CGameObject::Update(delta_time);
}

void CSpaceMarin::LateUpdate()
{
	Engine::GameManager()->AddRenderLayer(Engine::RENDERLAYER::LAYER_NONEALPHA, this);

	if (true == toss_)
		GranadeToss();

	if (true == reload_)
		Reload();
	
	UpdateLowerAnimState();
	UpdateUpperAnimState();

	if (weapon_ == Weapon::Gun)
		Fire();
	else
		Attack();


	if (true == evade_)
	{
		ptr_sword_->ptr_sphere_coll()->enable_ = false;
		ptr_body_collider_->enable_ = false;

		fire_ = false;
		next_lower_state_ = LowerState::Evade;
		next_upper_state_ = UpperState::End;
	}

	CollSystem()->CollisionCheck(ptr_body_collider_, TAG_ENEMY);

	if (true == execution_)
	{
		next_lower_state_ = LowerState::Execution;
		next_upper_state_ = UpperState::End;
	}
}

void CSpaceMarin::Render()
{
	LPD3DXEFFECT ptr_effect = ptr_shader_->GetEffectHandle();

	SetConstantTable(ptr_effect);

	Matrix mat_upper_rot_x;
	if (weapon_ == Weapon::Gun)
		D3DXMatrixRotationAxis(&mat_upper_rot_x, &Vector3(-1.f, 0.f, 0.f), ptr_transform_->rotation().x);
	else
		D3DXMatrixRotationAxis(&mat_upper_rot_x, &Vector3(-1.f, 0.f, 0.f), 0.f);

	ptr_mesh_->FrameMove(delta_time_, ptr_lower_anim_ctrl_, ptr_upper_start_frame_, &mat_upper_rot_x);
	ptr_mesh_->FrameMove(delta_time_, ptr_upper_anim_ctrl_, ptr_upper_start_frame_);
	ptr_gun_->SetHandMatrix(ptr_hand_matrix_);
	ptr_sword_->SetHandMatrix(ptr_hand_matrix_);

	ptr_mesh_->RenderMesh(ptr_effect, 1);

	ptr_head_collider_->SetSphereCollider(0.2f, *(Vector3*)(&ptr_head_matrix_->m[3][0]));
	ptr_body_collider_->SetSphereCollider(0.4f, *(Vector3*)(&ptr_body_matrix_->m[3][0]));

#ifdef _DEBUG
	DebugRender();
#endif // _DEBUG
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

void CSpaceMarin::ApplyDamage(int damage)
{
	if (damage_delay_ > 0.f)
		damage = 0;
	else if (damage >= 30)
		damage_delay_ = 1.f;

	if (damage != 0)
	{
		shield_ -= damage;
		ptr_damage_ui_->SetActive(true);
	}

	if (shield_ < 0)
	{
		hp_ += shield_;
		shield_ = 0;
	}

	shield_recovery_time_ = 40.f;
}

HRESULT CSpaceMarin::AddComponent()
{
	HRESULT hr = E_FAIL;

	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Component_Transform"), TEXT("Transform"), ptr_transform_);
	assert(hr == S_OK && "Tranform Component ReadyComponent Failed");
	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Component_Transform"), TEXT("Lower_Transform"), ptr_lower_transform_);
	assert(hr == S_OK && "Tranform Component ReadyComponent Failed");

	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Shader_DynamicMesh"), TEXT("Shader"), ptr_shader_);
	assert(hr == S_OK && "ScreenImage Shader ReadyComponent Failed");
	hr = Ready_Component(MAINTAIN_STAGE, TEXT("SpaceMarin_Mesh"), TEXT("Mesh"), ptr_mesh_);
	assert(hr == S_OK && "Mesh ReadyComponent Failed");
	ptr_lower_anim_ctrl_ = ptr_mesh_->CloneAnimController();
	ptr_upper_anim_ctrl_ = ptr_mesh_->CloneAnimController();

	if (0 == control_id_)
	{
		ptr_ctrl_ = CPlayerController::Create(this, 5.f, 0.5f);
		CGameObject::AddComponent(TEXT("0.PlayerController"), ptr_ctrl_);

		if (nullptr != ptr_ctrl_)
			Engine::GameManager()->Add_Prototype(MAINTAIN_STAGE, TEXT("Component_PlayerController"), ptr_ctrl_->CloneComponent());

		Subject()->set_player_pos(&ptr_transform_->position());
	}
	else
	{
		// TODO: ctrl_id 1일때 AIController 추가하기.
	}

	ptr_head_collider_ = Engine::CCollider::Create(ptr_device_, this, Collider_Sphere);
	ptr_body_collider_ = Engine::CCollider::Create(ptr_device_, this, Collider_Sphere);

	Subject()->SetSpaceMarinData(&shield_, &hp_, &bullet_count_, &granade_count_, &current_cell_index_, control_id_);

#ifdef _DEBUG
	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Shader_Default"), TEXT("Debug_Shader"), ptr_debug_shader_);
	assert(hr == S_OK && "Ork Shader_Default ReadyComponent Failed");
#endif

	return S_OK;
}

HRESULT CSpaceMarin::AddWeapon()
{
	// Gun
	ptr_gun_ = CGun_Phobos::Create(ptr_device_);
	ptr_hand_matrix_ = ptr_mesh_->FindFrameMatrix("joint_HandRT_01");
	if (nullptr == ptr_hand_matrix_) return E_FAIL;
	ptr_gun_->SetParentMatrix(&ptr_lower_transform_->mat_world());

	TCHAR object_key[128] = TEXT("");
	wsprintf(object_key, TEXT("Gun_Phobos_%d"), control_id_);
	Engine::GameManager()->GetCurrentScene()->AddObject(MAINTAIN_STAGE, object_key, ptr_gun_);

	// Sword
	ptr_sword_ = CChainSword::Create(ptr_device_);
	ptr_sword_->SetParentMatrix(&ptr_lower_transform_->mat_world());
	wsprintf(object_key, TEXT("ChainSword_%d"), control_id_);
	Engine::GameManager()->GetCurrentScene()->AddObject(MAINTAIN_STAGE, object_key, ptr_sword_);
	return S_OK;
}

HRESULT CSpaceMarin::AddUI()
{
	Engine::GameManager()->GetCurrentScene()->AddObject(MAINTAIN_STAGE, TEXT("Player_HPUI"), CUI_Hp::Create(ptr_device_));
	Engine::GameManager()->GetCurrentScene()->AddObject(MAINTAIN_STAGE, TEXT("Player_Shield_UI"), CUI_Shield::Create(ptr_device_));
	Engine::GameManager()->GetCurrentScene()->AddObject(MAINTAIN_STAGE, TEXT("Player_Weapon_UI"), CUI_Weapon::Create(ptr_device_));

	ptr_aim_ui_ = CUI_Aim::Create(ptr_device_);
	Engine::GameManager()->GetCurrentScene()->AddObject(MAINTAIN_STAGE, TEXT("Player_Aim_UI"), ptr_aim_ui_);

	ptr_damage_ui_ = CUI_Damage::Create(ptr_device_);
	Engine::GameManager()->GetCurrentScene()->AddObject(MAINTAIN_STAGE, TEXT("Player_Aim_UI"), ptr_damage_ui_);

	return S_OK;
}

void CSpaceMarin::Release()
{
	Engine::Safe_Delete(ptr_lower_anim_ctrl_);
	Engine::Safe_Delete(ptr_upper_anim_ctrl_);

	Engine::Safe_Release_Delete(ptr_head_collider_);
	Engine::Safe_Release_Delete(ptr_body_collider_);

#ifdef _DEBUG
	Engine::Safe_Release(ptr_line_);
#endif
}

void CSpaceMarin::UpdateState(float delta_time)
{
	switch (pre_lower_state_)
	{
	case LowerState::Idle:
		if (ptr_transform_->move_dir().x != 0.f || ptr_transform_->move_dir().z != 0.f)
			next_lower_state_ = LowerState::Run;
		break;
	case LowerState::Turn:
		if (fabs(ptr_transform_->rotation().y - ptr_lower_transform_->rotation().y) > 6.28f)
			ptr_lower_transform_->rotation() = Vector3(0.f, ptr_transform_->rotation().y, 0.f);

		ptr_lower_transform_->rotation().y += next_rot_y_* delta_time * 4.f;
		if (fabs(ptr_transform_->rotation().y - ptr_lower_transform_->rotation().y) <= 0.2f)
		{
			next_rot_y_ = 0.f;
			next_lower_state_ = LowerState::Idle;
		}
		break;
	case LowerState::Run:
		Run();
		if (ptr_transform_->move_dir().x == 0.f && ptr_transform_->move_dir().z == 0.f)
		{
			SwitchToIdle();
			foot_effect_ = false;
		}
		break;
	case LowerState::Evade:
		Evade(delta_time);
		if (ptr_upper_anim_ctrl_->CheckCurrentAnimationEnd(0.5))
			evade_dir_ = Vector3(0.f, 0.f, 0.f);
		if (ptr_upper_anim_ctrl_->CheckCurrentAnimationEnd(0.1))
		{
			SwitchToIdle();
			evade_ = false;
		}
		break;
	case LowerState::Attack:
		if (false == ptr_lower_anim_ctrl_->CheckCurrentAnimationEnd(1.1)
			&& true == ptr_lower_anim_ctrl_->CheckCurrentAnimationEnd(1.5))
		{
			Vector3 move_dir = ptr_lower_transform_->Forward().Normalize();
			int option = -1;
			current_cell_index_ = Engine::GameManager()->MoveFromNavMesh(ptr_transform_->position(), move_dir * 5.f * delta_time_, current_cell_index_, option);
		}

		if (true == ptr_lower_anim_ctrl_->CheckCurrentAnimationEnd(0.1))
		{
			SwitchToIdle();
			ptr_sword_->ptr_sphere_coll()->enable_ = false;
		}
		break;
	case LowerState::Execution:
		ptr_gun_->SetActive(false);
		ptr_sword_->SetActive(true);
		weapon_ = Weapon::ChainSword;
		fire_ = false;
		evade_ = false;
		toss_ = false;
		reload_ = false;
		ptr_transform_->LookAt(execution_target_);

		if (true == ptr_lower_anim_ctrl_->CheckCurrentAnimationEnd(1.9))
			ptr_sword_->transform()->rotation().z = D3DXToRadian(100.f);
		else if (true == ptr_lower_anim_ctrl_->CheckCurrentAnimationEnd(2.7))
			ptr_sword_->transform()->rotation().z = D3DXToRadian(280.f);

		if (true == ptr_lower_anim_ctrl_->CheckCurrentAnimationEnd(0.1))
		{
			SwitchToIdle();
			ptr_sword_->ptr_sphere_coll()->enable_ = false;
			execution_ = false;
			//ptr_sword_->transform()->rotation().z = D3DXToRadian(100.f);
		}
		break;
	}
}

void CSpaceMarin::UpdateLowerAnimState()
{
	if (pre_lower_state_ != next_lower_state_)
	{
		switch (next_lower_state_)
		{
		case LowerState::Idle:
			ptr_lower_anim_ctrl_->SetAnimationTrack("Idle");
			break;
		case LowerState::Turn:
			ptr_lower_anim_ctrl_->SetAnimationTrack("Turn");
			break;
		case LowerState::Attack:
			Sound()->PlaySound(ptr_sword_sound_, Sound()->CHANNEL_PLAYER_ATTACK);
			ptr_upper_anim_ctrl_->SetAnimationTrack("Sword_Attack1");
			ptr_lower_anim_ctrl_->SetAnimationTrack("Sword_Attack1");
			break;
		case LowerState::Evade:
			Sound()->PlaySound(ptr_evade_sound_, Sound()->CHANNEL_PLAYER);
			switch (pre_move_dir_)
			{
			case MoveDirection::Forward:
				ptr_upper_anim_ctrl_->SetAnimationTrack("Evade_Forward");
				ptr_lower_anim_ctrl_->SetAnimationTrack("Evade_Forward");
				break;
			case MoveDirection::Backward:
				ptr_upper_anim_ctrl_->SetAnimationTrack("Evade_Backward");
				ptr_lower_anim_ctrl_->SetAnimationTrack("Evade_Backward");
				break;
			case MoveDirection::Left:
				ptr_upper_anim_ctrl_->SetAnimationTrack("Evade_Left");
				ptr_lower_anim_ctrl_->SetAnimationTrack("Evade_Left");
				break;
			case MoveDirection::Right:
			default:
				ptr_upper_anim_ctrl_->SetAnimationTrack("Evade_Right");
				ptr_lower_anim_ctrl_->SetAnimationTrack("Evade_Right");
				break;
			}
			break;
		case LowerState::Execution:
			ptr_upper_anim_ctrl_->SetAnimationTrack("Execution");
			ptr_lower_anim_ctrl_->SetAnimationTrack("Execution");
			break;
		}
		ptr_lower_anim_ctrl_->SetTrackPosition(0.0);
		pre_lower_state_ = next_lower_state_;
	}
}

void CSpaceMarin::UpdateUpperAnimState()
{
	if (pre_upper_state_ != next_upper_state_)
	{
		switch (next_upper_state_)
		{
		case UpperState::Idle:
			if (weapon_ == Weapon::Gun)
				ptr_upper_anim_ctrl_->SetAnimationTrack("Idle");
			else
				ptr_upper_anim_ctrl_->SetAnimationTrack("Sword_Idle");
			break;
		case UpperState::Shoot:
			ptr_upper_anim_ctrl_->SetAnimationTrack("Idle");
			break;
		case UpperState::Run:
			if (weapon_ == Weapon::Gun)
				ptr_upper_anim_ctrl_->SetAnimationTrack("Run_Forward");
			else
				ptr_upper_anim_ctrl_->SetAnimationTrack("Sword_Run");
			break;
		case UpperState::Run_Aiming:
			ptr_upper_anim_ctrl_->SetAnimationTrack("Run_Shoot");
			break;
		case UpperState::Toss:
			ptr_upper_anim_ctrl_->SetAnimationTrack("Granade_Toss");
			break;
		case UpperState::Reload:
			ptr_upper_anim_ctrl_->SetAnimationTrack("Reload");
			break;
		}
		ptr_upper_anim_ctrl_->SetTrackPosition(0.0);
		pre_upper_state_ = next_upper_state_;
	}
}

void CSpaceMarin::SwitchToIdle()
{
	next_lower_state_ = LowerState::Idle;
	next_upper_state_ = UpperState::Idle;
	condition_ = 0.f;
}

void CSpaceMarin::Run()
{
	if (pre_move_dir_ != next_move_dir_)
	{
		switch (next_move_dir_)
		{
		case MoveDirection::Forward:
			ptr_lower_anim_ctrl_->SetAnimationTrack("Run_Forward");
			break;
		case MoveDirection::Backward:
			ptr_lower_anim_ctrl_->SetAnimationTrack("Run_Backward");
			break;
		case MoveDirection::Right:
			ptr_lower_anim_ctrl_->SetAnimationTrack("Run_Right");
			break;
		case MoveDirection::Left:
			ptr_lower_anim_ctrl_->SetAnimationTrack("Run_Left");
			break;
		}
		pre_move_dir_ = next_move_dir_;
		CreateRunEffect(*(Vector3*)&ptr_left_foot_matrix_->m[3][0]);
		foot_effect_ = true;
	}

	if (weapon_ == Weapon::Gun)
	{
		if (false == zoom_ && false == fire_)
			next_upper_state_ = UpperState::Run;
		else
			next_upper_state_ = UpperState::Run_Aiming;
	}
	else
		next_upper_state_ = UpperState::Run;

	int option = -1;
	current_cell_index_ = Engine::GameManager()->MoveFromNavMesh(ptr_transform_->position(), ptr_transform_->move_dir(), current_cell_index_, option);

	// 애니메이션 시간에 따른 이펙트 생성

	if (true == ptr_lower_anim_ctrl_->CheckCurrentAnimationEnd(0.0) && false == foot_effect_)
	{
		CreateRunEffect(*(Vector3*)&ptr_left_foot_matrix_->m[3][0]);
		foot_effect_ = true;
		ptr_lower_anim_ctrl_->SetTrackPosition(0.0);
	}
	else if (true == ptr_lower_anim_ctrl_->CheckCurrentAnimationEnd(0.5) && true == foot_effect_)
	{
		CreateRunEffect(*(Vector3*)&ptr_right_foot_matrix_->m[3][0]);
		foot_effect_ = false;
	}
}

void CSpaceMarin::Evade(float delta_time)
{
	toss_ = false;
	reload_ = false;
	fire_ = false;

	int option = -1;
	current_cell_index_ = Engine::GameManager()->MoveFromNavMesh(ptr_transform_->position(), evade_dir_ * 7.f * delta_time, current_cell_index_, option);
}

void CSpaceMarin::Execution()
{
	fire_ = false;
	evade_ = false;
}

void CSpaceMarin::Reload()
{
	fire_ = false;

	next_upper_state_ = UpperState::Reload;
	if (true == ptr_upper_anim_ctrl_->CheckCurrentAnimationEnd(0.1))
	{
		next_upper_state_ = UpperState::Idle;
		bullet_count_ = max_bullet_;
		reload_ = false;
	}
}

void CSpaceMarin::GranadeToss()
{
	next_upper_state_ = UpperState::Toss;


	if (true == ptr_upper_anim_ctrl_->CheckCurrentAnimationEnd(0.1))
	{
		next_upper_state_ = UpperState::Idle;
		granade_toss_ = false;
		toss_ = false;
	}
	else if (true == ptr_upper_anim_ctrl_->CheckCurrentAnimationEnd(0.5) && false == granade_toss_)
	{
		Vector3 start_pos = *(Vector3*)&ptr_hand_matrix_->m[3][0];
		const Vector3 start_dir = ptr_transform_->Forward().Normalize();
		D3DXVec3TransformCoord(&start_pos, &start_pos, &ptr_transform_->mat_world());
		CreateGranade(start_pos, start_dir);
		granade_toss_ = true;
	}
}

void CSpaceMarin::Fire()
{
	ptr_aim_ui_->set_aim_state(0);

	if (true == fire_)
	{
		switch (pre_upper_state_)
		{
		case UpperState::Run:
			next_upper_state_ = UpperState::Shoot;
			break;
		case UpperState::Idle:
		case UpperState::Shoot:
		case UpperState::Run_Aiming:
			Engine::CGameObject* hit_obj = nullptr;
			float dist = 0.f;
			if (true == CollSystem()->RaycastCheck(ray_pos_, ray_dir_, &dist, hit_obj, TAG_ENEMY))
			{
				if (nullptr != hit_obj)
					hit_obj->ApplyDamage(25);
				fire_range_pos_ = ray_pos_ + ray_dir_ * dist;
				CreateBulletHitEffect(fire_range_pos_);
			}
			else
				fire_range_pos_ = ray_pos_ + ray_dir_ * 30.f;
			fire_ = false;
			CreateFireEffect();
			--bullet_count_;

			if (bullet_count_ <= 0)
				reload_ = true;

			ptr_aim_ui_->set_aim_state(1);
			
			if (false == Subject()->camera_shaking())
				Subject()->SetCameraShaking(true, 0.1f, 0.1f, Vector3(ptr_transform_->Forward().Normalize()) * -1.f);
			break;
		}
	}
}

void CSpaceMarin::Attack()
{
	if (true == fire_)
	{
		next_upper_state_ = UpperState::End;
		next_lower_state_ = LowerState::Attack;
		ptr_sword_->ptr_sphere_coll()->enable_ = true;

		if (true == ptr_lower_anim_ctrl_->CheckCurrentAnimationEnd(0.6))
		{
			if (condition_ == 0.f)
			{
				Sound()->PlaySound(ptr_sword_sound_, Sound()->CHANNEL_PLAYER);
				ptr_upper_anim_ctrl_->SetAnimationTrack("Sword_Attack2");
				ptr_lower_anim_ctrl_->SetAnimationTrack("Sword_Attack2");
				++condition_;
			}
			else
			{
				Sound()->PlaySound(ptr_sword_sound_, Sound()->CHANNEL_PLAYER);
				ptr_upper_anim_ctrl_->SetAnimationTrack("Sword_Attack1");
				ptr_lower_anim_ctrl_->SetAnimationTrack("Sword_Attack1");
				condition_ = 0.f;
			}
		}
		fire_ = false;
	}
}

void CSpaceMarin::CreateFireEffect()
{
	Sound()->PlaySound(ptr_gun_sound_, Sound()->CHANNEL_PLAYER_ATTACK);

	TCHAR effect_key[64] = TEXT("");
	wsprintf(effect_key, TEXT("Space_Marin_%d_Fire_Effect_%d"), control_id_, bullet_count_);
	Engine::GameManager()->GetCurrentScene()->AddObject(MAINTAIN_STAGE, effect_key, CFireEffect::Create(ptr_device_, &ptr_gun_->GetFirePos()));

	wsprintf(effect_key, TEXT("Space_Marin_%d_Fire_Line_%d"), control_id_, bullet_count_);
	Vector3 effect_pos = ptr_gun_->GetFirePos().Right().Normalize();
	effect_pos = ptr_gun_->GetFirePos() + effect_pos * -0.1f;
	Engine::GameManager()->GetCurrentScene()->AddObject(MAINTAIN_STAGE, effect_key, CFireLineSmoke::Create(ptr_device_, effect_pos, fire_range_pos_));
}

void CSpaceMarin::CreateBulletHitEffect(const Vector3 & hit_position)
{
	TCHAR effect_key[64] = TEXT("");
	wsprintf(effect_key, TEXT("Space_Marin_%d_Bullet_Hit_Effect_%d"), control_id_, bullet_count_);
	Engine::GameManager()->GetCurrentScene()->AddObject(MAINTAIN_STAGE, effect_key, CHitEffect::Create(ptr_device_, hit_position, TEXT("HitBlood")));
}

void CSpaceMarin::CreateRunEffect(const Vector3 & foot_pos)
{
	Sound()->PlaySound(ptr_run_sound_, Sound()->CHANNEL_PLAYER);
	Vector3 effect_pos = {};
	D3DXVec3TransformCoord(&effect_pos, &foot_pos, &ptr_transform_->mat_world());
	TCHAR effect_key[64] = TEXT("");
	wsprintf(effect_key, TEXT("Space_Marin_%d_Move_Dust_Effect_%d"), control_id_, (int)foot_effect_);
	Engine::GameManager()->GetCurrentScene()->AddObject(MAINTAIN_STAGE, effect_key, CMoveDust::Create(ptr_device_, effect_pos));
}

void CSpaceMarin::CreateGranade(const Vector3 & start_pos, const Vector3 & start_dir)
{
	Engine::CGameObject* ptr_granade = CGranade::Create(ptr_device_, start_pos, start_dir);

	TCHAR object_key[128] = TEXT("");
	wsprintf(object_key, TEXT("Granade_%d"), granade_count_--);
	Engine::GameManager()->GetCurrentScene()->AddObject(MAINTAIN_STAGE, object_key, ptr_granade);
	Sound()->PlaySound(ptr_toss_sound_, Sound()->CHANNEL_PLAYER);
}

void CSpaceMarin::SetConstantTable(LPD3DXEFFECT ptr_effect)
{
	Matrix mat_view, mat_proj;
	ptr_device_->GetTransform(D3DTS_VIEW, &mat_view);
	ptr_device_->GetTransform(D3DTS_PROJECTION, &mat_proj);

	ptr_effect->SetMatrix("g_mat_world", &ptr_lower_transform_->mat_world());
	ptr_effect->SetMatrix("g_mat_view", &mat_view);
	ptr_effect->SetMatrix("g_mat_projection", &mat_proj);

	ptr_effect->SetVector("set_color_r", &color_[0]);
	ptr_effect->SetVector("set_color_g", &color_[1]);
	ptr_effect->SetVector("set_color_b", &color_[2]);
}

#ifdef _DEBUG
void CSpaceMarin::DebugRender()
{
	Matrix mat_view, mat_proj;
	ptr_device_->GetTransform(D3DTS_VIEW, &mat_view);
	ptr_device_->GetTransform(D3DTS_PROJECTION, &mat_proj);

	//Vector3 line_[2] = { ptr_gun_->GetFirePos(), fire_range_pos_ };

	//for (auto& point : line_)
	//{
	//	D3DXVec3TransformCoord(&point, &point, &mat_view);
	//	if (point.z < 0.f)
	//		point.z = 0.f;
	//	D3DXVec3TransformCoord(&point, &point, &mat_proj);
	//}

	//ptr_line_->SetWidth(2.f);
	//ptr_line_->Begin();

	//ptr_line_->DrawTransform(line_, 2, nullptr, D3DXCOLOR(1.f, 0.f, 0.f, 1.f));

	//ptr_line_->End();

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
}
#endif // _DEBUG