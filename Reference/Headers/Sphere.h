#pragma once

// Sphere Collider에서만 디버그용으로 사용.

#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CSphere
{
private:
	explicit CSphere(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CSphere();

public:
	LPD3DXMESH ptr_sphere_mesh();

private:
	HRESULT CreateSphere(float radius, uint32 slices, uint32 stacks);

public:
	void Render();
	void Render(const Matrix* mat_world);
	void Release();

public:
	static CSphere* Create(LPDIRECT3DDEVICE9 ptr_device, float radius, uint32 slices, uint32 stacks);

private:
	LPDIRECT3DDEVICE9 ptr_device_ = nullptr;
	LPD3DXMESH ptr_sphere_mesh_ = nullptr;
};

END