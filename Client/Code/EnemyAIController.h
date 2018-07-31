#pragma once

#include "Controller.h"

class COrk;
class CSpaceMarin;

class CEnemyAIController
	: public CController
{
private:
	enum class State { Idle, Move, MovingShoot, Gun_Attack, Sword_Attack, End };
private:
	explicit CEnemyAIController();

public:
	virtual ~CEnemyAIController();

public:
	virtual Engine::CComponent* CloneComponent() override;

private:
	HRESULT Initialize(COrk* ptr_target, float speed);

public:
	void LateInit();
	virtual void Update(float delta_time) override;

public:
	virtual int Release() override;

private:
	void Idle(float delta_time);
	void MoveToTarget(float delta_time);
	void MovingShootToTarget(float delta_time);
	void Gun_Attack(float delta_time);
	void Sword_Attack(float delta_time);

public:
	static CEnemyAIController* Create(COrk* ptr_target, float speed);

private:
	Engine::CTransform* ptr_forward_transform_ = nullptr;
	COrk* ptr_ctrl_unit_ = nullptr;
	std::vector<CSpaceMarin*> vec_target_;
	CSpaceMarin* ptr_target_ = nullptr;
	Engine::CTransform* ptr_target_transform_ = nullptr;


private:
	State current_state_ = State::End;
	float speed_ = 0.f;
	float attack_delay_ = 0.f;
	int condition_ = 0;

private:
	std::vector<Vector3> path_;
};