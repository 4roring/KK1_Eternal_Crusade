#include "stdafx.h"
#include "Rioreus_Fire.h"

#include "Rioreus.h"
#include "Transform.h"
#include "AnimController.h"

void CRioreus_Fire::InitState(CRioreus * ptr_target, Engine::CTransform * ptr_transform, Engine::CAnimController * ptr_anim_ctrl)
{
	CRioreus_State::InitState(ptr_target, ptr_transform, ptr_anim_ctrl);
}

void CRioreus_Fire::Update(float delta_time)
{
	switch (fire_state_)
	{
	case State::Setup: SetUpFireState();
		break;
	case State::Back_Explosion: Fire_Back_Explosion(delta_time);
		break;
	case State::Triple: Fire_Triple(delta_time);
		break;
	case State::Fire: Fire(delta_time);
		break;
	case State::End:
		condition_ = 0.f;
		ptr_target_->SetAnimSpeed(1.f);
		ptr_target_->SetState(CRioreus::State::Idle);
		break;
	}
}

void CRioreus_Fire::SetUpFireState()
{
	const float square_near = 15.f * 15.f;
	const float square_mid = 25.f * 25.f;
	const float square_dist = ((*Subject()->player_pos()) - ptr_transform_->position()).Magnitude();

	if (square_dist < square_near)
	{
		ptr_anim_ctrl_->SetAnimationTrack("Fire_Back_Explosion");
		fire_state_ = State::Back_Explosion;
	}
	else if (square_dist < square_mid)
	{
		ptr_anim_ctrl_->SetAnimationTrack("Fire_Triple");
		fire_state_ = State::Triple;
	}
	else
	{
		ptr_anim_ctrl_->SetAnimationTrack("Fire");
		fire_state_ = State::Fire;
	}

	const float next_anim_speed = (true == ptr_target_->rage()) ? 1.7f : 1.3f;

	ptr_target_->SetAnimSpeed(1.3f);
}

void CRioreus_Fire::Fire_Back_Explosion(float delta_time)
{
	if (false == ptr_anim_ctrl_->CheckCurrentAnimationEnd(7.8))
	{
		int option = -1;
		int& cell_index = ptr_target_->current_cell_index();
		const float back_move_speed = -3.f;

		ptr_transform_->move_dir() = Vector3(ptr_transform_->Forward().Normalize()) * back_move_speed * delta_time;
		cell_index = Engine::GameManager()->MoveFromNavMesh(ptr_transform_->position(), ptr_transform_->move_dir(), cell_index, option);
	}

	if (true == ptr_anim_ctrl_->CheckCurrentAnimationEnd(0.1))
	{
		fire_state_ = State::End;
	}
}

void CRioreus_Fire::Fire_Triple(float delta_time)
{
	if (true == ptr_anim_ctrl_->CheckCurrentAnimationEnd(0.1))
	{
		fire_state_ = State::End;
	}
}

void CRioreus_Fire::Fire(float delta_time)
{
	if (true == ptr_anim_ctrl_->CheckCurrentAnimationEnd(0.1))
	{
		fire_state_ = State::End;
	}
}

void CRioreus_Fire::CreateFire()
{
}

void CRioreus_Fire::CreateExplosion()
{
}
