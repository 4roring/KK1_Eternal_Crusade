#pragma once

#include "Rioreus_State.h"

class CRioreus_Fire
	: public CRioreus_State
{
private:
	enum class State { Setup, Back_Explosion, Triple, Fire, End };

public:
	CRioreus_Fire() = default;
	virtual ~CRioreus_Fire() = default;

public:
	virtual void InitState(CRioreus* ptr_target, Engine::CTransform* ptr_transform, Engine::CAnimController* ptr_anim_ctrl) override;

public:
	virtual void Update(float delta_time) override;

private:
	void SetUpFireState();
	void Fire_Back_Explosion(float delta_time);
	void Fire_Triple(float delta_time);
	void Fire(float delta_time);

private:
	void CreateFire();
	void CreateExplosion();

private:
	State fire_state_ = State::Setup;
};