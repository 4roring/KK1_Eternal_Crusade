#pragma once

#include "Rioreus_State.h"

class CRioreus_Somersault
	: public CRioreus_State
{
private:
	enum class State { Start, Mid, End };

public:
	CRioreus_Somersault() = default;
	virtual ~CRioreus_Somersault() = default;

public:
	virtual void InitState(CRioreus* ptr_target, Engine::CTransform* ptr_transform, Engine::CAnimController* ptr_anim_ctrl) override;
	virtual void Reset() override;

public:
	virtual void Update(float delta_time) override;

private:
	State state_ = State::Start;
	float start_pos_y_ = 0.f;

private:
	SoundSource* ptr_sound_ = nullptr;
	SoundSource* ptr_sound_end_ = nullptr;
};