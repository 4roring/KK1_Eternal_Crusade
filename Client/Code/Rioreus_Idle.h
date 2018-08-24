#pragma once

#include "Rioreus_State.h"

class CRioreus_Idle
	: public CRioreus_State
{
public:
	CRioreus_Idle() = default;
	virtual ~CRioreus_Idle() = default;

public:
	virtual void InitState(CRioreus* ptr_target, Engine::CTransform* ptr_transform, Engine::CAnimController* ptr_anim_ctrl) override;

public:
	virtual void Update(float delta_time) override;

private:
	float next_state_time_ = 0.f;
	int acc_idle_ = 0;

private:
	SoundSource* ptr_sound_ = nullptr;
};