#include "stdafx.h"
#include "Rioreus_Groggy.h"

#include "Rioreus.h"
#include "Transform.h"
#include "AnimController.h"

void CRioreus_Groggy::InitState(CRioreus * ptr_target, Engine::CTransform * ptr_transform, Engine::CAnimController * ptr_anim_ctrl)
{
	CRioreus_State::InitState(ptr_target, ptr_transform, ptr_anim_ctrl);
}

void CRioreus_Groggy::Update(float delta_time)
{
	if (ptr_anim_ctrl_->CheckCurrentAnimationEnd(0.1))
		ptr_target_->SetState(CRioreus::State::Idle);
	else if (false == ptr_anim_ctrl_->CheckCurrentAnimationEnd(6.0))
	{
		constexpr float back_speed = -2.f;
		Vector3 forward_vector = ptr_transform_->Forward().Normalize();
		ptr_transform_->move_dir() = forward_vector * back_speed * delta_time;

		int option = -1;
		int& cell_index = ptr_target_->current_cell_index();
		cell_index = Engine::GameManager()->MoveFromNavMesh(ptr_transform_->position(), ptr_transform_->move_dir(), cell_index, option);
	}
}
