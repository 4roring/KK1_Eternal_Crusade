#pragma once

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CCamera
	: public CGameObject
{
protected:
	explicit CCamera(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CCamera();

public:
	virtual void Update(float delta_time);

public:
	void Invalidate_View();
	void Invalidate_Projection();

protected:
	Matrix mat_view_, mat_projection_;

protected:
	Vector3 eye_ = Vector3(0.f, 0.f, 0.f);
	Vector3 at_ = Vector3(0.f, 0.f, 0.f);
	Vector3 up_ = Vector3(0.f, 0.f, 0.f);

protected:
	float fov_y_ = 0.f;
	float aspect_ = 0.f;
	float near_ = 0.f;
	float far_ = 0.f;
};

END