#pragma once

#include "Rioreus_State.h"

class CRioreus_Walk
	: public CRioreus_State
{
public:
	CRioreus_Walk() = default;
	virtual ~CRioreus_Walk() = default;

public:
	virtual void InitState(CRioreus* ptr_target, Engine::CTransform* ptr_transform, Engine::CAnimController* ptr_anim_ctrl) override;

public:
	virtual void Update(float delta_time) override;

private:
	float walk_speed_ = 0.f;
};