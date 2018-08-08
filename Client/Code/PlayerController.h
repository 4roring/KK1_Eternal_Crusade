#pragma once

#include "Controller.h"

class CPlayerCamera;
class CSpaceMarin;

class CPlayerController
	: public CController
{
private:
	explicit CPlayerController();

public:
	virtual ~CPlayerController();

public:
	virtual Engine::CComponent* CloneComponent() override;

private:
	HRESULT Initialize(CSpaceMarin * ptr_target, float speed, float sensitivity);

public:
	void LateInit();
	virtual void Update(float delta_time) override;

public:
	virtual int Release() override;

public:
	static CPlayerController* Create(CSpaceMarin * ptr_target, float speed, float sensitivity);

private:
	void CheckInput(float delta_time);
	void ControlMove(float delta_time);
	void ControlAimPoint(float delta_time);
	void ControlZoom(float delta_time);
	void ControlAttack(float delta_time);
	void ControlEvade(float delta_time);
	void ControlExecution();
	void ChangeWeapon();

private:
	void ComputeShootRay(Vector3& ray_pos, Vector3& ray_dir);

private:
	Engine::CTransform* ptr_forward_transform_ = nullptr;
	CSpaceMarin* ptr_ctrl_unit_ = nullptr;
	CPlayerCamera* ptr_ctrl_camera_ = nullptr;

private:
	bool zoom_ = false;
	float pre_fov_y = 0.f;
	float fov_y = 0.f;
	float speed_ = 0.f;
	float sensitivity_ = 0.f;
	float attack_delay_ = 0.f;

private:
	CEnemyObserver* ptr_enemy_observer_ = nullptr;
};