#pragma once

#include "Rioreus_State.h"

class CRioreus_Sprint
	: public CRioreus_State
{
public:
	CRioreus_Sprint() = default;
	virtual ~CRioreus_Sprint() = default;

public:
	virtual void InitState(CRioreus* ptr_target, Engine::CTransform* ptr_transform, Engine::CAnimController* ptr_anim_ctrl) override;
	virtual void Reset() override;

public:
	virtual void Update(float delta_time) override;

private:
	void SetNextState();

private:
	float sprint_speed_ = 0.f;

private:
	SoundSource* ptr_sound_ = nullptr;
	bool sprint_sound_ = false;
};