#pragma once

#include "Rioreus_State.h"

class CRioreus_TailCut
	: public CRioreus_State
{
public:
	CRioreus_TailCut() = default;
	virtual ~CRioreus_TailCut() = default;

public:
	virtual void InitState(CRioreus* ptr_target, Engine::CTransform* ptr_transform, Engine::CAnimController* ptr_anim_ctrl) override;

public:
	virtual void Update(float delta_time) override;

private:
	SoundSource* ptr_sound_ = nullptr;
};