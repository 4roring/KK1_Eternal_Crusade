#include "stdafx.h"
#include "Rioreus_BiteAttack.h"

#include "Rioreus.h"
#include "Transform.h"
#include "AnimController.h"

void CRioreus_BiteAttack::InitState(CRioreus * ptr_target, Engine::CTransform * ptr_transform, Engine::CAnimController * ptr_anim_ctrl)
{
	CRioreus_State::InitState(ptr_target, ptr_transform, ptr_anim_ctrl);
	ptr_sound_ = Sound()->FindSound(TEXT("Rioreus_BiteAttack"));
}

void CRioreus_BiteAttack::Reset()
{
	CRioreus_State::Reset();
	ptr_target_->SetAttackState(CRioreus::Part_Head, false);
}

void CRioreus_BiteAttack::Update(float delta_time)
{
	if (condition_ == 0.f)
	{
		ptr_target_->SetAttackState(CRioreus::Part_Head, true);
		Sound()->PlaySound(ptr_sound_, Sound()->CHANNEL_ENEMY_ATTACK);
		++condition_;
	}

	if (ptr_anim_ctrl_->CheckCurrentAnimationEnd(2.0))
		ptr_target_->SetAttackState(CRioreus::Part_Head, false);

	if (ptr_anim_ctrl_->CheckCurrentAnimationEnd(0.1))
	{
		ptr_target_->SetState(CRioreus::State::Idle);
		condition_ = 0.f;
	}
}
