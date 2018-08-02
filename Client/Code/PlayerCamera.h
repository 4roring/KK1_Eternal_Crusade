#pragma once

#include "Camera.h"

BEGIN(Engine)
class CTransform;
END

class CFrustum;

class CPlayerCamera
	: public Engine::CCamera
{
private:
	explicit CPlayerCamera(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CPlayerCamera();

private:
	HRESULT InitCamera(MAINTAINID stage_id);

public:
	virtual void Update(float delta_time) override;

public:
	static CPlayerCamera* Create(LPDIRECT3DDEVICE9 ptr_device, MAINTAINID stage_id);

public:
	virtual void SetView(const Vector3& eye, const Vector3& at) override;
	virtual void SetProjection(float fov_y, float aspect, float z_near, float z_far) override;

private:
	void Release();

public:
	HRESULT GetComponent(MAINTAINID stage_id);

private:
	Engine::CTransform* ptr_player_transform_ = nullptr;
	CFrustum* ptr_frustum_ = nullptr;
};