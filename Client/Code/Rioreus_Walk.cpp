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
	constexpr float square_dist = 8.f * 8.f;
	const float square_target_dist = (*Subject()->player_pos() - ptr_transform_->position()).Magnitude();

	walk_speed_ = (true == ptr_target_->rage()) ? 4.f : 3.f;

	ptr_transform_->LookAt(*Subject()->player_pos());
	Vector3 forward_vector = ptr_transform_->Forward().Normalize();
	ptr_transform_->move_dir() = forward_vector * walk_speed_ * delta_time;
	
	int option = -1;
	int& cell_index = ptr_target_->current_cell_index();

	if (false == ptr_anim_ctrl_->CheckCurrentAnimationEnd(0.7) && true == ptr_anim_ctrl_->CheckCurrentAnimationEnd(2.3)
		|| false == ptr_anim_ctrl_->CheckCurrentAnimationEnd(2.6) && true == ptr_anim_ctrl_->CheckCurrentAnimationEnd(4.3))
		cell_index = Engine::GameManager()->MoveFromNavMesh(ptr_transform_->position(), ptr_transform_->move_dir(), cell_index, option);

	if (ptr_anim_ctrl_->CheckCurrentAnimationEnd(0.1) || square_target_dist < square_dist)
		SetNextState();
}

void CRioreus_Walk::SetNextState()
{
	constexpr float square_dist = 10.f * 10.f;
	const float square_target_dist = (*Subject()->player_pos() - ptr_transform_->position()).Magnitude();

	if (square_target_dist < square_dist)
	{
		const int next_state = random_range(0, 2);

		if (next_state == 0)
			ptr_target_->SetState(CRioreus::State::TailAttack);
		else if (next_state == 1)
			ptr_target_->SetState(CRioreus::State::Fire);
		else if (next_state == 2)
			ptr_target_->SetState(CRioreus::State::Somersault);
	}
	else
	{
		const int next_state = random_range(0, 1);

		if (next_state == 0)
			ptr_target_->SetState(CRioreus::State::Sprint);
		else if (next_state == 1)
			ptr_target_->SetState(CRioreus::State::Fire);
	}
}
