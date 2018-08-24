#include "stdafx.h"
#include "Rioreus_TailCut.h"

#include "Rioreus.h"
#include "Transform.h"
#include "AnimController.h"

void CRioreus_TailCut::InitState(CRioreus * ptr_target, Engine::CTransform * ptr_transform, Engine::CAnimController * ptr_anim_ctrl)
{
	CRioreus_State::InitState(ptr_target, ptr_transform, ptr_anim_ctrl);
	ptr_sound_ = Sound()->FindSound(TEXT("Rioreus_Fall_Start"));
}

void CRioreus_TailCut::Update(float delta_time)
{
	if (condition_ == 0.f)
	{
		Sound()->StopSound(Sound()->CHANNEL_ENEMY);
		Sound()->PlaySound(ptr_sound_, Sound()->CHANNEL_ENEMY);
		++condition_;
	}

	if (ptr_anim_ctrl_->CheckCurrentAnimationEnd(0.1))
		ptr_target_->SetState(CRioreus::State::Idle);
}
