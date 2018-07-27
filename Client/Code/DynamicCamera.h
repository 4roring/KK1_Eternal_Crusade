#pragma once

#include "Camera.h"

class CDynamicCamera
	: public Engine::CCamera
{
private:
	explicit CDynamicCamera(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CDynamicCamera();

private:
	HRESULT InitCamera(const Vector3& eye, const Vector3& at);

public:
	virtual void Update(float delta_time) override;

public:
	static CDynamicCamera* Create(LPDIRECT3DDEVICE9 ptr_device, const Vector3& eye, const Vector3& at);

private:
	void InputCheck(float delta_time);
	void MouseMove();

private:
	float speed_ = 0.f;
	bool mouse_fix_ = true;
};