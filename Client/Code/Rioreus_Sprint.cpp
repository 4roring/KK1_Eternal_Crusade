#include "stdafx.h"
#include "Rioreus_Sprint.h"

#include "Rioreus.h"
#include "Transform.h"
#include "AnimController.h"

void CRioreus_Sprint::InitState(CRioreus * ptr_target, Engine::CTransform * ptr_transform, Engine::CAnimController * ptr_anim_ctrl)
{
	CRioreus_State::InitState(ptr_target, ptr_transform, ptr_anim_ctrl);
	ptr_sound_ = Sound()->FindSound(TEXT("Rioreus_Sprint"));
}

void CRioreus_Sprint::Reset()
{
	CRioreus_State::Reset();
	sprint_sound_ = false;
	ptr_target_->SetAttackState(CRioreus::Part_Head, false);
	ptr_target_->SetAttackState(CRioreus::Part_Body, false);
}

void CRioreus_Sprint::Update(float delta_time)
{
	if (false == sprint_sound_)
	{
		Sound()->PlaySound(ptr_sound_, Sound()->CHANNEL_ENEMY);
		sprint_sound_ = true;
		ptr_target_->SetAttackState(CRioreus::Part_Head, true);
		ptr_target_->SetAttackState(CRioreus::Part_Body, true);

		const float next_anim_speed = (true == ptr_target_->rage()) ? 2.f : 1.5f;
		ptr_target_->SetAnimSpeed(next_anim_speed);
	}

	sprint_speed_ = (true == ptr_target_->rage()) ? 12.f : 9.f;

	Vector3 forward_vector = ptr_transform_->Forward().Normalize();
	ptr_transform_->move_dir() = forward_vector * sprint_speed_ * delta_time;

	int option = -1;
	int& cell_index = ptr_target_->current_cell_index();

	if (condition_ == 0.f && ptr_anim_ctrl_->CheckCurrentAnimationEnd(0.0))
	{
		condition_ = 1.f;
		ptr_anim_ctrl_->SetAnimationTrack("Sprint_End");
		ptr_target_->SetAttackState(CRioreus::Part_Head, false);
		ptr_target_->SetAttackState(CRioreus::Part_Body, false);
	}
	else if (condition_ == 1.f && ptr_anim_ctrl_->CheckCurrentAnimationEnd(0.1))
	{
		// tail or bite
		SetNextState();
		Reset();
	}
	else if (condition_ == 0.f || condition_ == 1.f && false == ptr_anim_ctrl_->CheckCurrentAnimationEnd(0.6))
		cell_index = Engine::GameManager()->MoveFromNavMesh(ptr_transform_->position(), ptr_transform_->move_dir(), cell_index, option);
}

void CRioreus_Sprint::SetNextState()
{
	constexpr float square_dist = 10.f * 10.f;
	const float square_target_dist = (*Subject()->player_pos() - ptr_transform_->position()).Magnitude();

	if (square_target_dist < square_dist)
	{
		const int next_state = random_range(0, 2);

		if (next_state == 0)
			ptr_target_->SetState(CRioreus::State::Bite_Attack);
		else if (next_state == 1)
			ptr_target_->SetState(CRioreus::State::TailAttack);
		else if (next_state == 2)
			ptr_target_->SetState(CRioreus::State::Somersault);
	}
	else
		ptr_target_->SetState(CRioreus::State::Rotate);
}
