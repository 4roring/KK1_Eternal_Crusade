#pragma once

// Sphere Collider에서만 디버그용으로 사용.

#include "Engine_Include.h"

BEGIN(Engine)

class CSphere
{
private:
	explicit CSphere(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CSphere();

private:
	HRESULT CreateSphere();

public:
	void Render();
	void Release();

public:
	void SetWorld(float radius, const Vector3& position);

public:
	static CSphere* Create(LPDIRECT3DDEVICE9 ptr_device);

private:
	LPDIRECT3DDEVICE9 ptr_device_ = nullptr;
	LPD3DXMESH ptr_sphere_mesh_ = nullptr;
};

END