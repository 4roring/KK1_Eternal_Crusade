#include "stdafx.h"
#include "Rioreus_Sprint.h"

#include "Rioreus.h"
#include "Transform.h"
#include "AnimController.h"

void CRioreus_Sprint::InitState(CRioreus * ptr_target, Engine::CTransform * ptr_transform, Engine::CAnimController * ptr_anim_ctrl)
{
	CRioreus_State::InitState(ptr_target, ptr_transform, ptr_anim_ctrl);
	// TODO: 필요한 사운드들 초기화.
}

void CRioreus_Sprint::Update(float delta_time)
{
	sprint_speed_ = (true == ptr_target_->rage()) ? 9.f : 7.f;

	Vector3 forward_vector = ptr_transform_->Forward().Normalize();
	ptr_transform_->move_dir() = forward_vector * sprint_speed_ * delta_time;

	int option = -1;
	int& cell_index = ptr_target_->current_cell_index();

	if (condition_ == 0.f && ptr_anim_ctrl_->CheckCurrentAnimationEnd(0.0))
	{
		if (++condition_ == 1.f)
			ptr_anim_ctrl_->SetAnimationTrack("Sprint_End");
	}
	else if (condition_ == 1.f && ptr_anim_ctrl_->CheckCurrentAnimationEnd(0.1))
	{
		ptr_target_->SetState(CRioreus::State::TailAttack);
		condition_ = 0.f;
	}
	else if (condition_ == 0.f || condition_ == 1.f && false == ptr_anim_ctrl_->CheckCurrentAnimationEnd(0.6))
		cell_index = Engine::GameManager()->MoveFromNavMesh(ptr_transform_->position(), ptr_transform_->move_dir(), cell_index, option);
}
