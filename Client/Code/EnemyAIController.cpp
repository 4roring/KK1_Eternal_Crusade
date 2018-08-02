#include "stdafx.h"
#include "EnemyAIController.h"

#include "Transform.h"
#include "Ork.h"
#include "SpaceMarin.h"

CEnemyAIController::CEnemyAIController()
{
}

CEnemyAIController::~CEnemyAIController()
{
}

Engine::CComponent * CEnemyAIController::CloneComponent()
{
	++reference_count_;
	return this;
}

HRESULT CEnemyAIController::Initialize(COrk * ptr_target, float speed)
{
	if (nullptr == ptr_target) return E_FAIL;
	ptr_ctrl_unit_ = ptr_target;
	speed_ = speed;
	++reference_count_;

	return S_OK;
}

void CEnemyAIController::LateInit()
{
	CController::LateInit();
	ptr_ctrl_transform_ = ptr_ctrl_unit_->transform();
	ptr_ctrl_transform_->AddReferenceCount();
	ptr_forward_transform_ = ptr_ctrl_unit_->ptr_lower_transform();
	ptr_forward_transform_->AddReferenceCount();
	current_state_ = State::Idle;

	ptr_target_ = dynamic_cast<CSpaceMarin*>(Engine::GameManager()->FindObject(MAINTAIN_STAGE, TEXT("SpaceMarin_1")));
	ptr_target_transform_ = ptr_target_->transform();
}

void CEnemyAIController::Update(float delta_time)
{
	CController::Update(delta_time);

	switch (current_state_)
	{
	case State::Idle: Idle(delta_time);
		break;
	case State::Move: MoveToTarget(delta_time);
		break;
	case State::MovingShoot: MovingShootToTarget(delta_time);
		break;
	case State::Gun_Attack: Gun_Attack(delta_time);
		break;
	case State::Sword_Attack: Sword_Attack(delta_time);
		break;
	}
}

int CEnemyAIController::Release()
{
	if (--reference_count_ == 0)
	{
		Engine::Safe_Release_Delete(ptr_ctrl_transform_);
		Engine::Safe_Release_Delete(ptr_forward_transform_);
		return 0;
	}
	return reference_count_;
}

void CEnemyAIController::Idle(float delta_time)
{
	condition_ = random_range(0, 1);
	if (condition_ == 0)
	{
		Engine::GameManager()->PathFinder(ptr_ctrl_unit_->current_cell_index(), ptr_target_->current_cell_index(), path_);
		current_state_ = State::Move;
	}
	else
	{
		condition_ = 0;
		current_state_ = State::MovingShoot;
	}
}

void CEnemyAIController::MoveToTarget(float delta_time)
{
	Vector3 move_dir = Vector3();
	Vector3 next_path = (true == path_.empty()) ? ptr_target_transform_->position() : path_.back();
	const float square_target_dist = (ptr_ctrl_transform_->position() - ptr_target_->transform()->position()).Magnitude();
	const float square_path_dist = (next_path - ptr_ctrl_transform_->position()).Magnitude();
	constexpr float square_attack_dist = 3.f * 3.f;
	constexpr float square_path_end_dist = 4.f * 4.f;

	if (square_path_dist < square_path_end_dist)
	{
		if(false == path_.empty())
			path_.pop_back();
	}

	if (square_target_dist < square_attack_dist)
	{
		current_state_ = State::Sword_Attack;
		ptr_ctrl_transform_->move_dir() = Vector3();
		path_.clear();
		return;
	}

	if (true == path_.empty())
	{
		current_state_ = State::Idle;
		ptr_ctrl_transform_->move_dir() = Vector3();
		return;
	}

	ptr_ctrl_transform_->LookAt(next_path);
	move_dir = ptr_ctrl_transform_->Forward().Normalize();
	ptr_ctrl_transform_->move_dir() = move_dir * speed_ * delta_time;
}

void CEnemyAIController::MovingShootToTarget(float delta_time)
{
	Vector3 move_dir = Vector3();
	const float square_target_dist = (ptr_ctrl_transform_->position() - ptr_target_->transform()->position()).Magnitude();
	constexpr float square_shoot_dist = 9.f * 9.f;
	constexpr float square_escape_dist = 5.f * 5.f;

	ptr_ctrl_transform_->LookAt(ptr_target_transform_->position());
	Gun_Attack(delta_time);
	move_dir = ptr_ctrl_transform_->Forward().Normalize();

	if (square_target_dist < square_escape_dist)
		move_dir *= -1.f;
	else if (square_target_dist < square_shoot_dist)
	{
		current_state_ = State::Gun_Attack;
		move_dir = Vector3();
	}
	
	ptr_ctrl_transform_->move_dir() = move_dir * speed_ * delta_time;

	if (condition_ > 5)
		current_state_ = State::Idle;
}

void CEnemyAIController::Gun_Attack(float delta_time)
{
	attack_delay_ -= delta_time;
	const float square_target_dist = (ptr_ctrl_transform_->position() - ptr_target_transform_->position()).Magnitude();
	constexpr float square_shoot_dist = 9.f * 9.f;
	constexpr float square_dist = 4.f * 4.f;
	if (attack_delay_ <= 0.f)
	{
		ptr_ctrl_unit_->set_fire(true);
		ptr_ctrl_transform_->LookAt(ptr_target_transform_->position());
		attack_delay_ = 1.f;
		++condition_;
	}

	if (square_target_dist < square_dist)
		current_state_ = State::MovingShoot;
	else if (square_target_dist > square_shoot_dist || condition_ >= 4)
		current_state_ = State::Idle;
}

void CEnemyAIController::Sword_Attack(float delta_time)
{
	attack_delay_ -= delta_time;
	const float square_target_dist = (ptr_ctrl_transform_->position() - ptr_target_transform_->position()).Magnitude();
	constexpr float square_dist = 6.f * 6.f;
	if (attack_delay_ <= 0.f)
	{
		ptr_ctrl_unit_->set_slash(true);
		ptr_ctrl_transform_->LookAt(ptr_target_transform_->position());
		attack_delay_ = 1.f;
		++condition_;
	}

	if (condition_ >=2 || square_target_dist > square_dist)
		current_state_ = State::Idle;
}

CEnemyAIController * CEnemyAIController::Create(COrk * ptr_target, float speed)
{
	CEnemyAIController* ptr_ctrl = new CEnemyAIController;
	if (FAILED(ptr_ctrl->Initialize(ptr_target, speed)))
	{
		Safe_Delete(ptr_ctrl);
		assert(!"EnemyAIController Create Failed");
	}

	return ptr_ctrl;
}
