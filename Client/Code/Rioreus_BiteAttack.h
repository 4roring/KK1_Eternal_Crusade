#pragma once

#include "Rioreus_State.h"

class CRioreus_BiteAttack
	: public CRioreus_State
{
public:
	CRioreus_BiteAttack() = default;
	virtual ~CRioreus_BiteAttack() = default;

public:
	virtual void InitState(CRioreus* ptr_target, Engine::CTransform* ptr_transform, Engine::CAnimController* ptr_anim_ctrl) override;
	virtual void Reset() override;

public:
	virtual void Update(float delta_time) override;

private:
	float walk_speed_ = 0.f;

private:
	SoundSource* ptr_sound_ = nullptr;
};