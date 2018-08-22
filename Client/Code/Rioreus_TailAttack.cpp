#include "stdafx.h"
#include "Rioreus_TailAttack.h"

#include "Rioreus.h"
#include "Transform.h"
#include "AnimController.h"


void CRioreus_TailAttack::InitState(CRioreus * ptr_target, Engine::CTransform * ptr_transform, Engine::CAnimController * ptr_anim_ctrl)
{
	CRioreus_State::InitState(ptr_target, ptr_transform, ptr_anim_ctrl);
}

void CRioreus_TailAttack::Update(float delta_time)
{
	if (condition_ == 0.f)
	{
		const Vector3 forward_vector = ptr_transform_->Forward();
		const Vector3 normal = ptr_transform_->Right().Normalize();
		const Vector3 target_normal = (*Subject()->player_pos() - ptr_transform_->position()).Normalize();

		const float cos_ratio = Vector3::Dot(normal, target_normal);

		rotate_dir_ = (true == ptr_target_->rage()) ?
			(cos_ratio > 0.f) ? 1.57f : -1.57f : (cos_ratio > 0.f) ? 0.785f : -0.785f;

		const char*&& next_anim_key = (rotate_dir_ > 0.f) ? "Tale_Attack_Right" : "Tale_Attack_Left";
		ptr_anim_ctrl_->SetAnimationTrack(next_anim_key);
		++condition_;
	}

	if (false == ptr_anim_ctrl_->CheckCurrentAnimationEnd(0.3))
		ptr_transform_->rotation().y += rotate_dir_ * delta_time;
	else if (true == ptr_anim_ctrl_->CheckCurrentAnimationEnd(0.1))
	{
		if (condition_ < 2.f)
		{
			++condition_;
			ptr_anim_ctrl_->SetTrackPosition(0.0);
		}
		else
		{
			ptr_target_->SetState(CRioreus::State::Idle);
			condition_ = 0.f;
		}
	}
}
