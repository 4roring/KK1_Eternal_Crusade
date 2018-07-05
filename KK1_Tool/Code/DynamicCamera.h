#pragma once

#include <Camera.h>

class DynamicCamera
	: public Engine::CCamera
{
private:
	explicit DynamicCamera(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~DynamicCamera();

private:
	HRESULT InitCamera(const Vector3& eye, const Vector3 & at);

public:
	virtual void Update(float delta_time) override;

public:
	static DynamicCamera* Create(LPDIRECT3DDEVICE9 ptr_device, const Vector3& eye, const Vector3& at);

private:
	void InputCheck(float delta_time);
	void MouseMoveCheck();
	void MouseFixPositon();

private:
	float speed_ = 0.f;
	bool click_ = false;
};