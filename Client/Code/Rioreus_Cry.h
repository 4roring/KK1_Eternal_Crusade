#pragma once

#include "Rioreus_State.h"

class CRioreus_Cry
	: public CRioreus_State
{
public:
	CRioreus_Cry() = default;
	virtual ~CRioreus_Cry() = default;

public:
	virtual void InitState(CRioreus* ptr_target, Engine::CTransform* ptr_transform, Engine::CAnimController* ptr_anim_ctrl) override;

public:
	virtual void Update(float delta_time) override;
};