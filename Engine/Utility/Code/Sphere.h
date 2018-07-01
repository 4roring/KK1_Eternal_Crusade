#pragma once

// Sphere Collider������ ����׿����� ���.

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