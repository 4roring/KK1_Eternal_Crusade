#include "stdafx.h"
#include "Rioreus_Idle.h"

#include "Rioreus.h"
#include "AnimController.h"

void CRioreus_Idle::InitState(CRioreus * ptr_target, Engine::CTransform * ptr_transform, Engine::CAnimController * ptr_anim_ctrl)
{
	CRioreus_State::InitState(ptr_target, ptr_transform, ptr_anim_ctrl);
	// TODO: 필요한 사운드들 초기화.
}

void CRioreus_Idle::Update(float delta_time)
{
	next_state_time_ = (true == ptr_target_->rage()) ? 0.5f : 1.5f;
	condition_ += delta_time;

	if (acc_idle_ >= 5 && condition_ >= next_state_time_)
	{
		if (acc_idle_ == 5)
		{
			ptr_anim_ctrl_->SetAnimationTrack("Idle_Motion");
			++acc_idle_;
		}
		else if (acc_idle_ == 6 && ptr_anim_ctrl_->CheckCurrentAnimationEnd(0.1))
		{
			ptr_anim_ctrl_->SetAnimationTrack("Idle_Motion_End");
			++acc_idle_;
		}
		else if (acc_idle_ == 7 && ptr_anim_ctrl_->CheckCurrentAnimationEnd(0.1))
		{
			acc_idle_ = 0;
			ptr_anim_ctrl_->SetAnimationTrack("Idle");
		}
	}
	else if (condition_ >= next_state_time_)
	{
		ptr_target_->SetState(CRioreus::State::Groggy);
		++acc_idle_;
		condition_ = 0.f;
	}
}
