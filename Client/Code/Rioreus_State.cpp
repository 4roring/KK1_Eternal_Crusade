#include "stdafx.h"
#include "Rioreus_State.h"

void CRioreus_State::InitState(CRioreus * ptr_target, Engine::CTransform * ptr_transform, Engine::CAnimController * ptr_anim_ctrl)
{
	ptr_target_ = ptr_target;
	ptr_transform_ = ptr_transform;
	ptr_anim_ctrl_ = ptr_anim_ctrl;
}

void CRioreus_State::Reset()
{
	condition_ = 0.f;
}
