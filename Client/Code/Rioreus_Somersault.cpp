#include "stdafx.h"
#include "Rioreus_Somersault.h"

#include "Rioreus.h"
#include "Transform.h"
#include "AnimController.h"

void CRioreus_Somersault::InitState(CRioreus * ptr_target, Engine::CTransform * ptr_transform, Engine::CAnimController * ptr_anim_ctrl)
{
	CRioreus_State::InitState(ptr_target, ptr_transform, ptr_anim_ctrl);
	ptr_sound_ = Sound()->FindSound(TEXT("Rioreus_Somersault"));
	ptr_sound_end_ = Sound()->FindSound(TEXT("Rioreus_Somersault_End"));
}

void CRioreus_Somersault::Reset()
{
	CRioreus_State::Reset();
	ptr_target_->SetAttackState(CRioreus::Part_Tail, false);
	state_ = State::Start;
}

void CRioreus_Somersault::Update(float delta_time)
{
	switch (state_)
	{
	case State::Start:
		if (true == ptr_anim_ctrl_->CheckCurrentAnimationEnd(0.1))
		{
			start_pos_y_ = ptr_transform_->position().y;
			ptr_anim_ctrl_->SetAnimationTrack("Somersault_Attack_Mid");
			state_ = State::Mid;
			Sound()->PlaySound(ptr_sound_, Sound()->CHANNEL_ENEMY_ATTACK);
			ptr_target_->SetAttackState(CRioreus::Part_Tail, true);
		}
		break;
	case State::Mid:
		if (true == ptr_anim_ctrl_->CheckCurrentAnimationEnd(0.1))
		{
			ptr_anim_ctrl_->SetAnimationTrack("Somersault_Attack_End");
			state_ = State::End;
			ptr_target_->SetAttackState(CRioreus::Part_Tail, false);
		}
		else if (false == ptr_anim_ctrl_->CheckCurrentAnimationEnd(5.3))
		{
			const float up_speed = (true == ptr_target_->rage()) ? 30.f : 20.f;
			ptr_transform_->position().y += up_speed * delta_time;
		}
		break;
	case State::End:
		if (condition_ == 0.f && true == ptr_anim_ctrl_->CheckCurrentAnimationEnd(3.8))
		{
			Sound()->PlaySound(ptr_sound_end_, Sound()->CHANNEL_ENEMY);
			++condition_;
		}

		if (true == ptr_anim_ctrl_->CheckCurrentAnimationEnd(4.4)
			&& false == ptr_anim_ctrl_->CheckCurrentAnimationEnd(0.1))
			ptr_transform_->position().y = start_pos_y_;
		else if (true == ptr_anim_ctrl_->CheckCurrentAnimationEnd(0.1))
		{
			ptr_target_->SetState(CRioreus::State::Idle);
			Reset();
		}
		break;
	}
}
