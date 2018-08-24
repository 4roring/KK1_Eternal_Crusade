#pragma once

#include "Rioreus_State.h"

class CRioreus_Fall
	: public CRioreus_State
{
public:
	CRioreus_Fall() = default;
	virtual ~CRioreus_Fall() = default;

public:
	// false: Left, true: Right
	void SetFallDirection(bool dir);

public:
	virtual void InitState(CRioreus* ptr_target, Engine::CTransform* ptr_transform, Engine::CAnimController* ptr_anim_ctrl) override;

public:
	virtual void Update(float delta_time) override;

private:
	void FallStart(float delta_time);
	void FallLoop(float delta_time);
	void FallEnd(float delta_time);

private:
	bool dir_ = false;
	float fall_speed_ = 0.f;

private:
	SoundSource* ptr_sound_start_ = nullptr;
	SoundSource* ptr_sound_loop_ = nullptr;
	SoundSource* ptr_sound_end_ = nullptr;
};