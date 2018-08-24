#include "stdafx.h"
#include "Rioreus_Fall.h"

#include "Rioreus.h"
#include "Transform.h"
#include "AnimController.h"

void CRioreus_Fall::SetFallDirection(bool dir)
{
	dir_ = dir;
}

void CRioreus_Fall::InitState(CRioreus * ptr_target, Engine::CTransform * ptr_transform, Engine::CAnimController * ptr_anim_ctrl)
{
	CRioreus_State::InitState(ptr_target, ptr_transform, ptr_anim_ctrl);
	dir_ = false;
	fall_speed_ = (true == ptr_target_->rage()) ? 5.f : 4.f;

	ptr_sound_start_ = Sound()->FindSound(TEXT("Rioreus_Fall_Start"));;
	ptr_sound_loop_ = Sound()->FindSound(TEXT("Rioreus_Fall_Loop"));;
	ptr_sound_end_ = Sound()->FindSound(TEXT("Rioreus_Fall_End"));;
}

void CRioreus_Fall::Update(float delta_time)
{
	if (condition_ == 0.f)
	{
		const char*&& next_anim_key = (true == dir_) ? "Fall_Right_Start" : "Fall_Left_Start";
		ptr_anim_ctrl_->SetAnimationTrack(next_anim_key);
		++condition_;
		Sound()->PlaySound(ptr_sound_start_, Sound()->CHANNEL_ENEMY);
	}

	if (condition_ == 1.f)
		FallStart(delta_time);
	else if (condition_ == 2.f || condition_ == 3.f)
		FallLoop(delta_time);
	else if (condition_ == 4.f)
		FallEnd(delta_time);
}

void CRioreus_Fall::FallStart(float delta_time)
{
	if (true == ptr_anim_ctrl_->CheckCurrentAnimationEnd(0.0))
	{
		const char*&& next_anim_key = (true == dir_) ? "Fall_Right_Loop" : "Fall_Left_Loop";
		ptr_anim_ctrl_->SetAnimationTrack(next_anim_key);
		condition_ = 2.f;
		Sound()->PlaySound(ptr_sound_loop_, Sound()->CHANNEL_ENEMY);
	}
	else if (false == ptr_anim_ctrl_->CheckCurrentAnimationEnd(2.5))
	{
		ptr_transform_->move_dir() = (true == dir_) ? ptr_transform_->Right().Normalize() : Vector3(ptr_transform_->Right().Normalize()) * -1.f;
		ptr_transform_->move_dir() *= fall_speed_ * delta_time;

		int option = -1;
		int& cell_index = ptr_target_->current_cell_index();
		cell_index = Engine::GameManager()->MoveFromNavMesh(ptr_transform_->position(), ptr_transform_->move_dir(), cell_index, option);
	}
}

void CRioreus_Fall::FallLoop(float delta_time)
{
	if (true == ptr_anim_ctrl_->CheckCurrentAnimationEnd(0.0))
	{
		if (condition_ == 2.f)
		{
			++condition_;
			ptr_anim_ctrl_->SetTrackPosition(0.0);
			Sound()->PlaySound(ptr_sound_loop_, Sound()->CHANNEL_ENEMY);
		}
		else if (condition_ == 3.f)
		{
			const char*&& next_anim_key = (true == dir_) ? "Fall_Right_End" : "Fall_Left_End";
			ptr_anim_ctrl_->SetAnimationTrack(next_anim_key);
			condition_ = 4.f;
			Sound()->PlaySound(ptr_sound_end_, Sound()->CHANNEL_ENEMY);
		}
	}
}

void CRioreus_Fall::FallEnd(float delta_time)
{
	if (true == ptr_anim_ctrl_->CheckCurrentAnimationEnd(0.1))
	{
		condition_ = 0.f;
		ptr_target_->SetState(CRioreus::State::Idle);
	}
}
