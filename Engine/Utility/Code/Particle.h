#pragma once

#include "Engine_Include.h"

BEGIN(Engine)

class CTransform;

class CParticle
{
protected:
	explicit CParticle(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CParticle();

public:
	void ComputeViewZ(const Vector3& camera_pos);

public:
	CTransform* transform();
	float view_z();

protected:
	HRESULT Initialize();

public:
	virtual void Update(float delta_time, const Vector3& position, const Vector3& rotation, const Vector3& scale, const Vector3& camera_pos);
	virtual void Render(LPD3DXEFFECT ptr_effect, const Matrix* mat_parent);

private:
	void Release();

protected:
	LPDIRECT3DDEVICE9 ptr_device_ = nullptr;
	CTransform* ptr_transform_ = nullptr;
	float view_z_ = 0.f;
	Vector3 camera_pos_ = {};
};

END