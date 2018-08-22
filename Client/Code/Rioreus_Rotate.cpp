#include "stdafx.h"
#include "Rioreus_Rotate.h"

#include "Rioreus.h"
#include "Transform.h"
#include "AnimController.h"

void CRioreus_Rotate::InitState(CRioreus * ptr_target, Engine::CTransform * ptr_transform, Engine::CAnimController * ptr_anim_ctrl)
{
	CRioreus_State::InitState(ptr_target, ptr_transform, ptr_anim_ctrl);
}

void CRioreus_Rotate::Update(float delta_time)
{
	const Vector3 forward_vector = ptr_transform_->Forward();
	const Vector3 normal = ptr_transform_->Right().Normalize();
	const Vector3 target_normal = (*Subject()->player_pos() - ptr_transform_->position()).Normalize();

	rotate_dist_ = acosf(Vector3::Dot(ptr_transform_->Forward().Normalize(), target_normal));

	const float cos_ratio = Vector3::Dot(normal, target_normal);

	if (condition_ == 0.f)
	{
		rotate_dir_ = (cos_ratio > 0.f) ? 0.3925f : -0.3925f;
		if (true == ptr_target_->rage()) rotate_dir_ *= 2.f;

		const char*&& next_anim_key = (true == ptr_target_->rage()) 
			? (rotate_dir_ > 0.f) ? "Rage_Rot_Right" : "Rage_Rot_Left" : (rotate_dir_ > 0.f) ? "Normal_Rot_Right" : "Normal_Rot_Left";
		ptr_anim_ctrl_->SetAnimationTrack(next_anim_key);
		++condition_;
	}

	float target_rot = D3DXToDegree(rotate_dist_);

	if (target_rot <= 5.f && cos_ratio > 0.f)
	{
		// Next State
		condition_ = 0.f;
		ptr_target_->SetAnimSpeed(1.f);
		ptr_target_->SetState(CRioreus::State::Sprint);
	}
	else if (true == ptr_anim_ctrl_->CheckCurrentAnimationEnd(0.1))
	{
		ptr_target_->SetAnimSpeed(0.f);
		condition_ += delta_time;
		if (condition_ >= 1.5f)
		{
			condition_ = 0.f;
			ptr_target_->SetAnimSpeed(1.f);
			ptr_anim_ctrl_->SetTrackPosition(0.0);
		}
	}
	else
		ptr_transform_->rotation().y += rotate_dir_ * delta_time;
}
