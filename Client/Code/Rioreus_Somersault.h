#pragma once

#include "Rioreus_State.h"

class CRioreus_Somersault
	: public CRioreus_State
{
public:
	CRioreus_Somersault() = default;
	virtual ~CRioreus_Somersault() = default;

public:
	virtual void InitState(CRioreus* ptr_target, Engine::CTransform* ptr_transform, Engine::CAnimController* ptr_anim_ctrl) override;

public:
	virtual void Update(float delta_time) override;

private:
	float pos_y_ = 0.f;
};