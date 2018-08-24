#pragma once

#include "Rioreus_State.h"

class CRioreus_Rotate
	: public CRioreus_State
{
public:
	CRioreus_Rotate() = default;
	virtual ~CRioreus_Rotate() = default;

public:
	virtual void InitState(CRioreus* ptr_target, Engine::CTransform* ptr_transform, Engine::CAnimController* ptr_anim_ctrl) override;
	virtual void Reset() override;

public:
	virtual void Update(float delta_time) override;

private:
	void SetNextState();

private:
	float rotate_dir_ = 0.f;
	float rotate_dist_ = 0.f;
};