#pragma once

#include "Camera.h"

BEGIN(Engine)
class CTransform;
END

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

private:
	void Release();

public:
	HRESULT GetComponent(MAINTAINID stage_id);

private:
	Engine::CTransform* ptr_player_transform_ = nullptr;
};