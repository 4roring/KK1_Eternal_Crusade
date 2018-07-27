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

	ptr_target_ = dynamic_cast<CSpaceMarin*>(Engine::GameManager()->FindObject(MAINTAIN_STAGE, TEXT("SpaceMarin")));
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
	if (true == path_.empty())
		Engine::GameManager()->PathFinder(ptr_ctrl_unit_->current_cell_index(), ptr_target_->current_cell_index(), path_);
	else
		current_state_ = State::Move;
}

void CEnemyAIController::MoveToTarget(float delta_time)
{
	Vector3 move_dir = Vector3();
	constexpr float square_dist = 3.f * 3.f;

	if ((ptr_ctrl_transform_->position() - ptr_target_->transform()->position()).Magnitude() < square_dist)
	{
		current_state_ = State::Sword_Attack;
		ptr_ctrl_transform_->move_dir() = Vector3();
		return;
	}

	if ((path_.back() - ptr_ctrl_transform_->position()).Magnitude() < 9.f)
		path_.pop_back();

	if (true == path_.empty())
	{
		current_state_ = State::Idle;
		ptr_ctrl_transform_->move_dir() = Vector3();
		return;
	}

	ptr_ctrl_transform_->LookAt(path_.back());
	move_dir = ptr_ctrl_transform_->Forward().Normalize();
	ptr_ctrl_transform_->move_dir() = move_dir * speed_ * delta_time;
}

void CEnemyAIController::Gun_Attack(float delta_time)
{
	attack_delay_ -= delta_time;
	if (attack_delay_ <= 0.f)
	{
		ptr_ctrl_unit_->set_fire(true);
		attack_delay_ = 3.f;
	}
}

void CEnemyAIController::Sword_Attack(float delta_time)
{
	attack_delay_ -= delta_time;
	constexpr float square_dist = 5.f * 5.f;
	//if (attack_delay_ <= 0.f)
	//{
	//	ptr_ctrl_unit_->set_slash(true);
	//	attack_delay_ = 1.f;
	//}

	if ((ptr_ctrl_transform_->position() - ptr_target_->transform()->position()).Magnitude() < square_dist)
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
