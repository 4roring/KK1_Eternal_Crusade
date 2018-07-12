#pragma once

#include "Controller.h"

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
	HRESULT Initialize(Engine::CTransform* ptr_target_transform, float speed, float sensitivity);

public:
	virtual void Update(float delta_time) override;

public:
	virtual int Release() override;

public:
	static CPlayerController* Create(Engine::CTransform* ptr_target_transform, float speed, float sensitivity);

private:
	void CheckInput(float delta_time);
	void InputKeyboard(float delta_time);
	void MoveToMouse(float delta_time);

private:
	CSpaceMarin* ctrl_unit_ = nullptr;
	
private:
	float speed_ = 0.f;
	float sensitivity_ = 0.f;
};