#include "stdafx.h"
#include "Rioreus_Walk.h"

#include "Rioreus.h"
#include "Transform.h"
#include "AnimController.h"

void CRioreus_Walk::InitState(CRioreus * ptr_target, Engine::CTransform * ptr_transform, Engine::CAnimController * ptr_anim_ctrl)
{
	CRioreus_State::InitState(ptr_target, ptr_transform, ptr_anim_ctrl);
}

void CRioreus_Walk::Update(float delta_time)
{
	walk_speed_ = (true == ptr_target_->rage()) ? 4.f : 3.f;

	Vector3 forward_vector = ptr_transform_->Forward().Normalize();
	ptr_transform_->move_dir() = forward_vector * walk_speed_ * delta_time;
	
	int option = -1;
	int& cell_index = ptr_target_->current_cell_index();

	if (false == ptr_anim_ctrl_->CheckCurrentAnimationEnd(0.7) && true == ptr_anim_ctrl_->CheckCurrentAnimationEnd(2.3)
		|| false == ptr_anim_ctrl_->CheckCurrentAnimationEnd(2.6) && true == ptr_anim_ctrl_->CheckCurrentAnimationEnd(4.3))
		cell_index = Engine::GameManager()->MoveFromNavMesh(ptr_transform_->position(), ptr_transform_->move_dir(), cell_index, option);

	if (ptr_anim_ctrl_->CheckCurrentAnimationEnd(0.1))
	{
		// TODO: Idle로 가거나 플레이어와 거리를 계산 후 패턴 발생
		ptr_target_->SetState(CRioreus::State::Idle);

	}
}
