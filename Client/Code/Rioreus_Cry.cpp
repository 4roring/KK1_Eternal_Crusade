#include "stdafx.h"
#include "Rioreus_Cry.h"

#include "Rioreus.h"
#include "Transform.h"
#include "AnimController.h"

void CRioreus_Cry::InitState(CRioreus * ptr_target, Engine::CTransform * ptr_transform, Engine::CAnimController * ptr_anim_ctrl)
{
	CRioreus_State::InitState(ptr_target, ptr_transform, ptr_anim_ctrl);
}

void CRioreus_Cry::Update(float delta_time)
{
	if (ptr_anim_ctrl_->CheckCurrentAnimationEnd(0.1))
		ptr_target_->SetState(CRioreus::State::Idle);
}
