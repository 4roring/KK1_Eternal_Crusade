#include "stdafx.h"
#include "Ork_WarBoss.h"

#include "Transform.h"
#include "Shader.h"
#include "DynamicMesh.h"
#include "AnimController.h"
#include "Collider.h"

#include "Ork_Gun.h"
#include "Ork_HeavyGun.h"
#include "Ork_Klaw.h"

#include "SpaceMarin.h"

#include "Font.h"

#include "Explosion.h"
#include "WarBossSkill.h"
#include "WarBossSkillRange.h"
#include "ExecutionBlood.h"
#include "WarBossJump.h"

#include "FireEffect.h"
#include "FireLineSmoke.h"
#include "HitEffect.h"

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
	ptr_transform_->scale() = Vector3(1.2f, 1.2f, 1.2f);
	ptr_lower_transform_->scale() = ptr_transform_->scale();

	ptr_head_frame_ = ptr_mesh_->FindFrame("joint_Head_01");
	ptr_body_frame_ = ptr_mesh_->FindFrame("joint_TorsoC_01");

	current_cell_index_ = -1;
	max_hp_ = 2000;
	hp_ = max_hp_;
	next_behavior_pattern_ = BehaviorPattern::Spawn;
	weapon_ = Weapon::HeavyGun;
	speed_ = 4.f;

	SetActive(false);

	Subject()->SetBossHp(&hp_);
	Subject()->SetBossPosition(&ptr_transform_->position());

#ifdef _DEBUG
	ptr_font_ = Engine::GraphicDevice()->GetFont(TEXT("바탕"));
#endif // _DEBUG
	return S_OK;
}

void COrk_WarBoss::LateInit()
{
	HRESULT hr = AddWeapon();
	assert(!FAILED(hr) && "Ork Add Weapon Failed");

	if (current_cell_index_ == -1)
		current_cell_index_ = Engine::GameManager()->FindCellIndex(ptr_transform_->position());

	ptr_heavy_gun_->SetActive(false);
	ptr_gun_->SetActive(false);
	ptr_klaw_->SetActive(false);
	ptr_skill_collider_->SetSphereCollider(5.f, Vector3());

	ptr_target_ = dynamic_cast<CSpaceMarin*>(Engine::GameManager()->FindObject(MAINTAIN_STAGE, TEXT("SpaceMarin")));

	ptr_jump_effect_ = CWarBossJump::Create(ptr_device_, &ptr_transform_->position());
	Engine::GameManager()->GetCurrentScene()->AddObject(MAINTAIN_STAGE, TEXT("WarBoss Jump Effect"), ptr_jump_effect_);
	ptr_jump_effect_->SetActive(false);

#ifdef _DEBUG
	D3DXCreateLine(ptr_device_, &ptr_line_);
	ptr_line_->SetAntialias(TRUE);
#endif
}

void COrk_WarBoss::Update(float delta_time)
{
	if (current_cell_index_ == -1)
		current_cell_index_ = Engine::GameManager()->FindCellIndex(ptr_transform_->position());

	anim_time_ = delta_time;

	ptr_lower_transform_->position() = ptr_transform_->position();
	ptr_lower_transform_->rotation() = Vector3(0.f, ptr_transform_->rotation().y, 0.f);

	UpdateState(delta_time);
	Engine::CGameObject::Update(delta_time);

	CollSystem()->AddRaycastList(ptr_body_collider_, TAG_ENEMY);
	CollSystem()->AddRaycastList(ptr_head_collider_, TAG_ENEMY);
	CollSystem()->AddColliderList(ptr_body_collider_, TAG_ENEMY);

	if (damage_delay_ > 0.f) damage_delay_ -= delta_time;
}

void COrk_WarBoss::LateUpdate()
{
	Engine::GameManager()->AddRenderLayer(Engine::RENDERLAYER::LAYER_NONEALPHA, this);
	CollSystem()->CollisionCheck(ptr_body_collider_, TAG_ENEMY);
	CollSystem()->CollisionCheck(ptr_body_collider_, TAG_PLAYER);

	CheckCollision();

	// Heavy에 Attack 상태라면 몸에 구와 플레이어의 충돌 체크 후 데미지 주기.
	DamageChangeState();
	UpdateAnimState();
}

void COrk_WarBoss::Render()
{
	LPD3DXEFFECT ptr_effect = ptr_shader_->GetEffectHandle();
	SetConstantTable(ptr_effect);

	Matrix mat_upper_rot_y;
	D3DXMatrixRotationAxis(&mat_upper_rot_y, &Vector3(0.f, 0.f, -1.f), upper_shoot_rot_y_);

	ptr_mesh_->FrameMove(anim_time_, ptr_lower_anim_ctrl_, ptr_upper_start_frame_, &mat_upper_rot_y);
	ptr_mesh_->FrameMove(anim_time_, ptr_upper_anim_ctrl_, ptr_upper_start_frame_);

	ptr_gun_->SetHandMatrix(ptr_left_hand_matrix_);
	ptr_heavy_gun_->SetBackMatrix(ptr_back_matrix_);
	ptr_klaw_->SetHandMatrix(ptr_right_hand_matrix_);
	ptr_head_collider_->SetSphereCollider(0.4f, *(Vector3*)&ptr_head_frame_->combined_matrix.m[3][0]);
	ptr_body_collider_->SetSphereCollider(0.7f, *(Vector3*)&ptr_body_frame_->combined_matrix.m[3][0]);

	ptr_mesh_->RenderMesh(ptr_effect, 0);

#ifdef _DEBUG
	DebugRender();
#endif // _DEBUG
}

void COrk_WarBoss::ApplyDamage(int damage)
{
	if (next_behavior_pattern_ == BehaviorPattern::Down) return;
	
	if (damage_delay_ <= 0.f)
	{
		hp_ -= damage;
		if (hp_ < 0) hp_ = 0;
	}
		
	if (damage_delay_ <= 0.f && damage == 100) damage_delay_ = 0.6f;
}

void COrk_WarBoss::OnEnable()
{
	hp_ = max_hp_;
	next_behavior_pattern_ = BehaviorPattern::Spawn;
	weapon_ = Weapon::HeavyGun;
	ptr_heavy_gun_->SetActive(true);
	condition_ = -1.f;
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

#ifdef _DEBUG
	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Shader_Default"), TEXT("Debug_Shader"), ptr_debug_shader_);
	assert(hr == S_OK && "Ork Shader_Default ReadyComponent Failed");
#endif

	hr = Ready_Component(MAINTAIN_STAGE, TEXT("Ork_WarBoss_Mesh"), TEXT("Mesh"), ptr_mesh_);
	assert(hr == S_OK && "Mesh ReadyComponent Failed");
	ptr_lower_anim_ctrl_ = ptr_mesh_->CloneAnimController();
	ptr_upper_anim_ctrl_ = ptr_mesh_->CloneAnimController();

	// Collider
	ptr_head_collider_ = Engine::CCollider::Create(ptr_device_, this, Collider_Sphere);
	ptr_body_collider_ = Engine::CCollider::Create(ptr_device_, this, Collider_Sphere);
	ptr_skill_collider_ = Engine::CCollider::Create(ptr_device_, this, Collider_Sphere);
	ptr_skill_collider_->enable_ = false;

	return S_OK;
}

HRESULT COrk_WarBoss::AddWeapon()
{
	// TODO: HeavyGun, Klaw 추가
	ptr_gun_ = COrk_Gun::Create(ptr_device_);
	ptr_left_hand_matrix_ = ptr_mesh_->FindFrameMatrix("joint_WeaponLT_01");
	if (nullptr == ptr_left_hand_matrix_) return E_FAIL;
	ptr_gun_->SetParentMatrix(&ptr_lower_transform_->mat_world());
	Engine::GameManager()->GetCurrentScene()->AddObject(MAINTAIN_STAGE, TEXT("WarBoss_Gun"), ptr_gun_);

	ptr_heavy_gun_ = COrk_HeavyGun::Create(ptr_device_);
	ptr_back_matrix_ = &ptr_body_frame_->combined_matrix;
	if (nullptr == ptr_back_matrix_) return E_FAIL;
	ptr_heavy_gun_->SetParentMatrix(&ptr_lower_transform_->mat_world());
	Engine::GameManager()->GetCurrentScene()->AddObject(MAINTAIN_STAGE, TEXT("WarBoss_HeavyGun"), ptr_heavy_gun_);

	ptr_klaw_ = COrk_Klaw::Create(ptr_device_);
	ptr_right_hand_matrix_ = ptr_mesh_->FindFrameMatrix("joint_WeaponRT_01");
	if (nullptr == ptr_right_hand_matrix_) return E_FAIL;
	ptr_klaw_->SetParentMatrix(&ptr_lower_transform_->mat_world());
	Engine::GameManager()->GetCurrentScene()->AddObject(MAINTAIN_STAGE, TEXT("WarBoss_Klaw"), ptr_klaw_);

	return S_OK;
}

void COrk_WarBoss::Release()
{
	Engine::Safe_Delete(ptr_lower_anim_ctrl_);
	Engine::Safe_Delete(ptr_upper_anim_ctrl_);

	Engine::Safe_Release_Delete(ptr_head_collider_);
	Engine::Safe_Release_Delete(ptr_body_collider_);
	Engine::Safe_Release_Delete(ptr_skill_collider_);

#ifdef _DEBUG
	Engine::Safe_Release(ptr_line_);
#endif
}

void COrk_WarBoss::UpdateState(float delta_time)
{
	const float mind_time = (hp_ > (max_hp_ >> 2)) ? 1.5f : 0.5f;

	switch (pre_behavior_pattern_)
	{
	case BehaviorPattern::Spawn:
		if (condition_ < 0.f) 
		{
			anim_time_ = 0.f;
			condition_ = fmin(condition_ + delta_time, 0.f);
		}

		if (condition_ == 0.f && ptr_upper_anim_ctrl_->CheckCurrentAnimationEnd(0.1))
		{
			++condition_;
			ptr_upper_anim_ctrl_->SetAnimationTrack("Spawn_2");
			ptr_lower_anim_ctrl_->SetAnimationTrack("Spawn_2");
		}
		
		if (condition_ == 1.f && ptr_upper_anim_ctrl_->CheckCurrentAnimationEnd(0.1))
		{
			condition_ = 0.f;
			next_behavior_pattern_ = BehaviorPattern::Idle;
		}
		break;
	case BehaviorPattern::Idle:
		// 다음 패턴 고민
		condition_ += delta_time;
		if (condition_ > mind_time)
		{
			condition_ = 0.f;
			if (weapon_ == Weapon::HeavyGun)
				HeavyIdle();
			else if (weapon_ == Weapon::Klaw)
				KlawIdle();
		}
		break;
	case BehaviorPattern::Shoot:
		// Heavy - 플레이어에게 접근하면서 사격
		// Klaw - 사격하면서 플레이어 주변을 맴돌기
		if (weapon_ == Weapon::HeavyGun)
			HeavyShoot(delta_time);
		else if (weapon_ == Weapon::Klaw)
			KlawShoot(delta_time);
		break;
	case BehaviorPattern::Attack:
		// Heavy - 돌진
		// Klaw - 사격하면서 접근 후 2회 근접 공격
		if (weapon_ == Weapon::HeavyGun)
			HeavyAttack(delta_time);
		else if (weapon_ == Weapon::Klaw)
			MoveForKlawAttack(delta_time);
		break;
	case BehaviorPattern::BastionOfDestruction:
		// Klaw만, 파멸의 일격
		BastionOfDestruction(delta_time);
		break;
	case BehaviorPattern::Down:
		Down();
		break;
	case BehaviorPattern::Victim:
		// 다운 상태. 플레이어에게 Execution 당하고 사망
		if (ptr_upper_anim_ctrl_->CheckCurrentAnimationEnd(0.1))
			anim_time_ = 0.f;
		else
			Victim();
		break;
	case BehaviorPattern::End:
		break;
	default:
		break;
	}
}

void COrk_WarBoss::DamageChangeState()
{
	if (hp_ <= (max_hp_ >> 1) && weapon_ == Weapon::HeavyGun)
	{
		weapon_ = Weapon::Klaw;
		ptr_heavy_gun_->SetActive(false);
		ptr_gun_->SetActive(true);
		ptr_klaw_->SetActive(true);

		CreateExplosionEffect();
		next_behavior_pattern_ = BehaviorPattern::Spawn;
		condition_ = 0.f;

		upper_shoot_rot_y_ = 0.f;
		speed_ = 7.f;
		Subject()->set_camera_shaking(true);
	}
	else if (false == first_skill_ && hp_ < (max_hp_ >> 3))
	{
		next_behavior_pattern_ = BehaviorPattern::BastionOfDestruction;
		first_skill_ = true;
		condition_ = 0.f;
	}
	else if (hp_ == 0)
	{
		ptr_gun_->SetActive(false);
		ptr_klaw_->SetActive(false);

		next_behavior_pattern_ = BehaviorPattern::Down;
		condition_ = 0.f;
		skill_pos_y_ = 0.f;
	}
}

void COrk_WarBoss::UpdateAnimState()
{
	if (pre_behavior_pattern_ != next_behavior_pattern_)
	{
		switch (next_behavior_pattern_)
		{
		case BehaviorPattern::Spawn:
			ptr_upper_anim_ctrl_->SetAnimationTrack("Spawn_1");
			ptr_lower_anim_ctrl_->SetAnimationTrack("Spawn_1");
			break;
		case BehaviorPattern::Idle:
			if (weapon_ == Weapon::HeavyGun)
			{
				ptr_upper_anim_ctrl_->SetAnimationTrack("Heavy_Idle");
				ptr_lower_anim_ctrl_->SetAnimationTrack("Heavy_Idle");
			}
			else if (weapon_ == Weapon::Klaw)
			{
				ptr_upper_anim_ctrl_->SetAnimationTrack("Klaw_Idle");
				ptr_lower_anim_ctrl_->SetAnimationTrack("Klaw_Idle");
			} 
			break;
		case BehaviorPattern::Shoot:
			if (weapon_ == Weapon::HeavyGun)
			{
				ptr_upper_anim_ctrl_->SetAnimationTrack("Heavy_Walk_Forward");
				ptr_lower_anim_ctrl_->SetAnimationTrack("Heavy_Walk_Forward");
			}
			else if (weapon_ == Weapon::Klaw)
			{
				ptr_upper_anim_ctrl_->SetAnimationTrack("Klaw_Run_Aim");
				ptr_lower_anim_ctrl_->SetAnimationTrack("Klaw_Run");
			}
			break;
		case BehaviorPattern::Attack:
			if (weapon_ == Weapon::HeavyGun)
			{
				ptr_upper_anim_ctrl_->SetAnimationTrack("Heavy_Sprint");
				ptr_lower_anim_ctrl_->SetAnimationTrack("Heavy_Sprint");
			}
			else if (weapon_ == Weapon::Klaw)
			{
				ptr_upper_anim_ctrl_->SetAnimationTrack("Klaw_Run_Aim");
				ptr_lower_anim_ctrl_->SetAnimationTrack("Klaw_Run");
			}
			break;
		case BehaviorPattern::BastionOfDestruction:
			ptr_upper_anim_ctrl_->SetAnimationTrack("Klaw_Ground_Attack_Start");
			ptr_lower_anim_ctrl_->SetAnimationTrack("Klaw_Ground_Attack_Start");
			break;
		case BehaviorPattern::Down:
			ptr_upper_anim_ctrl_->SetAnimationTrack("Down_In");
			ptr_lower_anim_ctrl_->SetAnimationTrack("Down_In");
			break;
		case BehaviorPattern::Victim:
			ptr_upper_anim_ctrl_->SetAnimationTrack("Down_Victim");
			ptr_lower_anim_ctrl_->SetAnimationTrack("Down_Victim");
			break;
		case BehaviorPattern::End:
			break;
		default:
			break;
		}
		ptr_upper_anim_ctrl_->SetTrackPosition(0.0);
		ptr_lower_anim_ctrl_->SetTrackPosition(0.0);

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

void COrk_WarBoss::HeavyIdle()
{
	constexpr float square_dist_ = 10.f * 10.f;
	const float square_target_dist = (ptr_target_->transform()->position() - ptr_transform_->position()).Magnitude();

	condition_ = (float)random_range(0, 5);

	if (square_target_dist > square_dist_)
		next_behavior_pattern_ = (condition_ <= 3.f) ? BehaviorPattern::Shoot : BehaviorPattern::Attack;
	else
		next_behavior_pattern_ = (condition_ <= 3.f) ? BehaviorPattern::Attack : BehaviorPattern::Shoot;

	condition_ = 0.f;
}
void COrk_WarBoss::KlawIdle()
{
	condition_ = (float)random_range(0, 7);

	if (condition_ <= 3.f)
		next_behavior_pattern_ = BehaviorPattern::Shoot;
	else if (condition_ <= 5.f)
		next_behavior_pattern_ = BehaviorPattern::Attack;
	else if (hp_ < (max_hp_ >> 3) && condition_ <= 7.f)
		next_behavior_pattern_ = BehaviorPattern::BastionOfDestruction;

	condition_ = 0.f;
}

void COrk_WarBoss::HeavyShoot(float delta_time)
{
	if (attack_delay_ <= 0.f)
	{
		Fire();
		attack_delay_ = 0.1f;
	}
	attack_delay_ -= delta_time;

	if (condition_ == 0.f)
	{
		ptr_transform_->LookAt(ptr_target_->transform()->position());
		lower_shoot_rot_y_ = ptr_transform_->rotation().y;
		upper_shoot_rot_y_ = D3DXToRadian(60.f);
	}

	upper_shoot_rot_y_ += ((int)condition_ & 1) ? D3DXToRadian(120.f) * delta_time : D3DXToRadian(-120.f) * delta_time;
	ptr_lower_transform_->rotation() = Vector3(0.f, lower_shoot_rot_y_, 0.f);
	
	anim_time_ *= 0.5f;
	Vector3 move_dir = ptr_lower_transform_->Forward().Normalize();
	int option = -1;
	ptr_transform_->move_dir() = move_dir * speed_ * 0.5f * delta_time;
	current_cell_index_ = Engine::GameManager()->MoveFromNavMesh(ptr_transform_->position(), ptr_transform_->move_dir(), current_cell_index_, option);

	condition_ += delta_time;
	if (condition_ >= 5.f)
	{
		upper_shoot_rot_y_ = 0.f;
		condition_ = 0.f;
		ptr_transform_->LookAt(ptr_target_->transform()->position());
		next_behavior_pattern_ = BehaviorPattern::Idle;
	}
}

void COrk_WarBoss::KlawShoot(float delta_time)
{
	constexpr float square_dist = 10.f * 10.f;
	const float square_target_dist = (ptr_target_->transform()->position() - ptr_transform_->position()).Magnitude();

	if (attack_delay_ <= 0.f)
	{
		Fire();
		attack_delay_ = 0.5f;
	}
	attack_delay_ -= delta_time;

	ptr_transform_->LookAt(ptr_target_->transform()->position());

	Vector3 right_vector = ((int)condition_ & 1) ? ptr_lower_transform_->Right().Normalize() : (ptr_lower_transform_->Right() * -1.f).Normalize();
	ptr_transform_->move_dir() = right_vector * speed_ * delta_time;
	
	if (square_target_dist > square_dist)
	{
		Vector3 forward_vector = ptr_lower_transform_->Forward().Normalize();
		ptr_transform_->move_dir() += forward_vector * speed_ * 0.5f * delta_time;
	}

	int option = -1;
	current_cell_index_ = Engine::GameManager()->MoveFromNavMesh(ptr_transform_->position(), ptr_transform_->move_dir(), current_cell_index_, option);

	condition_ += delta_time * 0.5f;

	if (condition_ >= 3.f || option != -1)
	{
		upper_shoot_rot_y_ = 0.f;
		condition_ = 0.f;
		ptr_transform_->LookAt(ptr_target_->transform()->position());
		next_behavior_pattern_ = BehaviorPattern::Idle;
	}
}

void COrk_WarBoss::HeavyAttack(float delta_time)
{
	if (condition_ == 0.f)
	{
		++condition_;
		ptr_transform_->LookAt(ptr_target_->transform()->position());
	}

	Vector3 move_dir = ptr_transform_->Forward().Normalize();
	int option = -1;
	ptr_transform_->move_dir() = move_dir * speed_ * speed_ * delta_time;
	current_cell_index_ = Engine::GameManager()->MoveFromNavMesh(ptr_transform_->position(), ptr_transform_->move_dir(), current_cell_index_, option);
	
	if(current_cell_index_ == 223)
		next_behavior_pattern_ = BehaviorPattern::Shoot;

	if (option != -1 && ++condition_ > 3.f)
	{
		next_behavior_pattern_ = BehaviorPattern::Idle;
		condition_ = 0.f;
	}
}

void COrk_WarBoss::MoveForKlawAttack(float delta_time)
{
	constexpr float square_dist = 7.f * 7.f;
	const float square_target_dist = (ptr_target_->transform()->position() - ptr_transform_->position()).Magnitude();

	if (condition_ <= 1.f && true == path_.empty())
	{
		if (condition_ == 0.f)
		{
			ptr_upper_anim_ctrl_->SetAnimationTrack("Klaw_Run_Aim");
			++condition_;
		}

		ptr_transform_->LookAt(ptr_target_->transform()->position());

		Vector3 move_dir = ptr_transform_->Forward().Normalize();
		int option = -1;
		ptr_transform_->move_dir() = move_dir * speed_ * delta_time;
		current_cell_index_ = Engine::GameManager()->MoveFromNavMesh(ptr_transform_->position(), ptr_transform_->move_dir(), current_cell_index_, option);
	
		if (option != -1)
		{
			Engine::GameManager()->PathFinder(current_cell_index_, ptr_target_->current_cell_index(), path_);
			condition_ = 0.f;
		}
	}
	else if(condition_ <= 1.f)
	{
		if (condition_ == 0.f)
		{
			ptr_upper_anim_ctrl_->SetAnimationTrack("Klaw_Run");
			++condition_;
		}

		constexpr float square_path_end_dist = 3.f * 3.f;
		const float next_path_dist = (path_.back() - ptr_transform_->position()).Magnitude();

		ptr_transform_->LookAt(path_.back());

		Vector3 move_dir = ptr_transform_->Forward().Normalize();
		int option = -1;
		ptr_transform_->move_dir() = move_dir * speed_ * delta_time;
		current_cell_index_ = Engine::GameManager()->MoveFromNavMesh(ptr_transform_->position(), ptr_transform_->move_dir(), current_cell_index_, option);

		if (next_path_dist < square_path_end_dist)
		{
			if (false == path_.empty())
				path_.pop_back();
			else
				condition_ = 0.f;
		}
	}
	
	if (square_target_dist < square_dist)
	{
		if (condition_ <= 1.f)
			condition_ = 2.f;
		
		KlawAttack(delta_time);
	}
}
void COrk_WarBoss::KlawAttack(float delta_time)
{
	anim_time_ *= 0.7f;

	if (condition_ == 2.f)
	{
		++condition_;
		ptr_transform_->LookAt(ptr_target_->transform()->position());
		ptr_upper_anim_ctrl_->SetAnimationTrack("Klaw_Attack1");
		ptr_lower_anim_ctrl_->SetAnimationTrack("Klaw_Attack1");
	}
	else if (condition_ == 3.f && true == ptr_upper_anim_ctrl_->CheckCurrentAnimationEnd(0.1))
	{
		++condition_;
		ptr_transform_->LookAt(ptr_target_->transform()->position());
		ptr_upper_anim_ctrl_->SetAnimationTrack("Klaw_Attack2");
		ptr_lower_anim_ctrl_->SetAnimationTrack("Klaw_Attack2");
	}
	else if (condition_ == 4.f && true == ptr_upper_anim_ctrl_->CheckCurrentAnimationEnd(0.1))
	{
		ptr_klaw_->ptr_sphere_coll()->enable_ = false;
		next_behavior_pattern_ = BehaviorPattern::Idle;
		condition_ = 0.f;
	}

	if (false == ptr_upper_anim_ctrl_->CheckCurrentAnimationEnd(0.4))
	{
		Vector3 move_dir = ptr_transform_->Forward().Normalize();
		int option = -1;
		ptr_transform_->move_dir() = move_dir * (speed_ * 2.f) * delta_time;
		current_cell_index_ = Engine::GameManager()->MoveFromNavMesh(ptr_transform_->position(), ptr_transform_->move_dir(), current_cell_index_, option);
		ptr_klaw_->ptr_sphere_coll()->enable_ = true;
	}
}

void COrk_WarBoss::BastionOfDestruction(float delta_time)
{
	if (condition_ == 0.f)
	{
		anim_time_ *= 0.5f;
		if (true == ptr_upper_anim_ctrl_->CheckCurrentAnimationEnd(0.1))
		{
			ptr_upper_anim_ctrl_->SetAnimationTrack("Klaw_Ground_Attack_Loop");
			ptr_lower_anim_ctrl_->SetAnimationTrack("Klaw_Ground_Attack_Loop");
			++condition_;
			current_cell_index_ = -1;
			// Skill Range Effect
			if (nullptr == ptr_skill_range_)
			{
				ptr_skill_range_ = CWarBossSkillRange::Create(ptr_device_, &skill_pos_);
				Engine::GameManager()->GetCurrentScene()->AddObject(MAINTAIN_STAGE, TEXT("WarBoss_SkillRange"), ptr_skill_range_);
			}
			ptr_jump_effect_->SetActive(true);
		}
	}
	else if (condition_ == 1.f)
	{
		skill_pos_y_ += 100.f * delta_time;
		Vector3 pos = ptr_lower_transform_->position();
		pos.y = 0.f;
		Vector3 move_dir = (ptr_target_->transform()->position() - pos).Normalize();
		move_dir *= speed_ * 0.8f * delta_time;
		int option = -1;
		ptr_transform_->position() += move_dir;
		if (skill_pos_y_ > 500.f)
		{
			skill_pos_y_ = 500.f;
			++condition_;
			ptr_jump_effect_->transform()->rotation().x = 3.14f;
		}
		skill_pos_ = ptr_lower_transform_->position();
		skill_pos_.y = 0.f;
	}
	else if (condition_ == 2.f)
	{
		skill_pos_y_ -= 500.f * delta_time;
		if (skill_pos_y_ < 0.f)
		{
			skill_pos_y_ = 0.f;
			++condition_;
			ptr_upper_anim_ctrl_->SetAnimationTrack("Klaw_Ground_Attack_End");
			ptr_lower_anim_ctrl_->SetAnimationTrack("Klaw_Ground_Attack_End");
			CreateSkillEffect();
			ptr_jump_effect_->transform()->rotation().x = 0.f;
			ptr_jump_effect_->SetActive(false);
			Subject()->set_camera_shaking(true);
		}
	}
	else if (condition_ == 3.f)
	{
		if (nullptr != ptr_skill_range_)
		{
			ptr_skill_range_->Destroy();
			ptr_skill_range_ = nullptr;
			ptr_skill_collider_->enable_ = true;
		}
		else if (true == ptr_upper_anim_ctrl_->CheckCurrentAnimationEnd(0.1))
		{
			next_behavior_pattern_ = BehaviorPattern::Idle;
			condition_ = 0.f;
			ptr_skill_collider_->enable_ = false;
		}
	}
	ptr_transform_->position().y = skill_pos_y_;
	ptr_skill_collider_->SetSphereCollider(5.f, Vector3());
}

void COrk_WarBoss::Down()
{
	if (condition_ == 0.f && true == ptr_upper_anim_ctrl_->CheckCurrentAnimationEnd(0.1))
	{
		++condition_;
		ptr_upper_anim_ctrl_->SetAnimationTrack("Down_Loop");
		ptr_lower_anim_ctrl_->SetAnimationTrack("Down_Loop");
	}

	if (true == ptr_target_->execution())
	{
		hp_ = -10;
		next_behavior_pattern_ = BehaviorPattern::Victim;
	}
}
void COrk_WarBoss::Victim()
{
	if (true == ptr_upper_anim_ctrl_->CheckCurrentAnimationEnd(2.0) && false == victim_blood_)
	{
		victim_blood_ = true;
		CreateVictimBlood();
	}
	ptr_transform_->LookAt(ptr_target_->transform()->position());
}

void COrk_WarBoss::CheckCollision()
{
	const CollList& space_marin_coll_list = CollSystem()->GetColliderList(TAG_PLAYER);

	if (true == ptr_skill_collider_->enable_)
	{
		for (auto& space_marin_coll : space_marin_coll_list)
		{
			if (true == ptr_skill_collider_->TriggerCheck(space_marin_coll))
				space_marin_coll->ptr_object()->ApplyDamage(80);
		}
	}

	if (pre_behavior_pattern_ == BehaviorPattern::Attack && weapon_ == Weapon::HeavyGun)
	{
		for (auto& space_marin_coll : space_marin_coll_list)
		{
			if (true == ptr_body_collider_->TriggerCheck(space_marin_coll))
				space_marin_coll->ptr_object()->ApplyDamage(30);
		}
	}

}

void COrk_WarBoss::Fire()
{
	if (weapon_ == Weapon::HeavyGun)
	{
		Vector3 fire_dir = ptr_transform_->Forward();
		fire_dir.x += (float)random_range(-30, 30) * 0.01f;
		fire_dir.y += (float)random_range(-30, 30) * 0.01f;
		fire_dir_ = fire_dir.Normalize();
		Engine::CGameObject* hit_obj = nullptr;
		float dist = 0.f;

		for (int i = 0; i < 2; ++i)
		{
			fire_pos_ = ptr_heavy_gun_->GetFirePos(i);
			if (true == CollSystem()->RaycastCheck(fire_pos_, fire_dir_, &dist, hit_obj, TAG_PLAYER))
			{
				if (nullptr != hit_obj)
					hit_obj->ApplyDamage(10);
				fire_range_pos_ = fire_pos_ + fire_dir_ * dist;
				CreateBulletHitEffect(fire_range_pos_);
			}
			else
				fire_range_pos_ = fire_pos_ + fire_dir_ * 20.f;

			CreateFireEffect();
		}
	}
	else if(weapon_ == Weapon::Klaw)
	{
		Vector3 fire_dir = ptr_transform_->Forward();
		fire_dir.x += (float)random_range(-30, 30) * 0.01f;
		fire_dir.y += (float)random_range(-30, 30) * 0.01f;
		fire_dir_ = fire_dir.Normalize();
		fire_pos_ = ptr_gun_->GetFirePos();
		Engine::CGameObject* hit_obj = nullptr;
		float dist = 0.f;
		if (true == CollSystem()->RaycastCheck(fire_pos_, fire_dir_, &dist, hit_obj, TAG_PLAYER))
		{
			if (nullptr != hit_obj)
				hit_obj->ApplyDamage(10);
			fire_range_pos_ = fire_pos_ + fire_dir_ * dist;
			CreateBulletHitEffect(fire_range_pos_);
		}
		else
			fire_range_pos_ = fire_pos_ + fire_dir_ * 20.f;

		CreateFireEffect();
	}
}

void COrk_WarBoss::CreateExplosionEffect()
{
	TCHAR effect_key[64] = TEXT("");
	wsprintf(effect_key, TEXT("OrkWarboss_Explosion_Effect"));
	Engine::GameManager()->GetCurrentScene()->AddObject(MAINTAIN_STAGE, effect_key, CExplosion::Create(ptr_device_, ptr_transform_->position()));
}

void COrk_WarBoss::CreateSkillEffect()
{
	TCHAR effect_key[64] = TEXT("");
	wsprintf(effect_key, TEXT("OrkWarboss_Skill_Effect"));
	Engine::GameManager()->GetCurrentScene()->AddObject(MAINTAIN_STAGE, effect_key, CWarBossSkill::Create(ptr_device_, skill_pos_));
}

void COrk_WarBoss::CreateVictimBlood()
{
	TCHAR effect_key[64] = TEXT("");
	wsprintf(effect_key, TEXT("OrkWarboss_Victim_Blood"));
	Vector3 effect_pos = *(Vector3*)&ptr_body_frame_->combined_matrix.m[3][0];
	D3DXVec3TransformCoord(&effect_pos, &effect_pos, &ptr_transform_->mat_world());
	Engine::GameManager()->GetCurrentScene()->AddObject(MAINTAIN_STAGE, effect_key, CExecutionBlood::Create(ptr_device_, effect_pos));
}

void COrk_WarBoss::CreateFireEffect()
{
	TCHAR effect_key[64] = TEXT("");
	wsprintf(effect_key, TEXT("WarBoss_Bullet_Fire_Effect"));
	Engine::GameManager()->GetCurrentScene()->AddObject(MAINTAIN_STAGE, effect_key, CFireEffect::Create(ptr_device_, &fire_pos_));

	wsprintf(effect_key, TEXT("WarBoss_Fire_Line_"));
	Engine::GameManager()->GetCurrentScene()->AddObject(MAINTAIN_STAGE, effect_key, CFireLineSmoke::Create(ptr_device_, fire_pos_, fire_range_pos_));
}

void COrk_WarBoss::CreateBulletHitEffect(const Vector3 & hit_position)
{
	TCHAR effect_key[64] = TEXT("");
	wsprintf(effect_key, TEXT("WarBoss_Bullet_Hit_Effect"));
	Engine::GameManager()->GetCurrentScene()->AddObject(MAINTAIN_STAGE, effect_key, CHitEffect::Create(ptr_device_, hit_position, TEXT("HitBlood")));
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

	if (true == ptr_skill_collider_->enable_)
	{
		mat_coll._41 = ptr_skill_collider_->GetSpherePos().x;
		mat_coll._42 = ptr_skill_collider_->GetSpherePos().y;
		mat_coll._43 = ptr_skill_collider_->GetSpherePos().z;
		ptr_effect->SetMatrix("g_mat_world", &mat_coll);

		ptr_debug_shader_->BegineShader(1);
		ptr_skill_collider_->DebugRender();
		ptr_debug_shader_->EndShader();
	}

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

	TCHAR hp[128] = {};
	wsprintf(hp, TEXT("OrkWarBoss HP: %d"), hp_);
	ptr_font_->Render(hp, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), Vector3(float(kWinCx >> 1), 10.f, 0.f));
}
#endif
