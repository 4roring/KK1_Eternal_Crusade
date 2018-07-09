#include "Sphere.h"

Engine::CSphere::CSphere(LPDIRECT3DDEVICE9 ptr_device)
	: ptr_device_(ptr_device)
{
}

Engine::CSphere::~CSphere()
{
	Release();
}

LPD3DXMESH Engine::CSphere::ptr_sphere_mesh()
{
	return ptr_sphere_mesh_;
}

HRESULT Engine::CSphere::CreateSphere(float radius, uint32 slices, uint32 stacks)
{
	return D3DXCreateSphere(ptr_device_, radius, slices, stacks, &ptr_sphere_mesh_, nullptr);
}

void Engine::CSphere::Render()
{
	ptr_sphere_mesh_->DrawSubset(0);
}

void Engine::CSphere::Render(const Matrix * mat_world)
{
	ptr_device_->SetTransform(D3DTS_WORLD, mat_world);
	ptr_sphere_mesh_->DrawSubset(0);
}

void Engine::CSphere::Release()
{
	Safe_Release(ptr_sphere_mesh_);
}

Engine::CSphere * Engine::CSphere::Create(LPDIRECT3DDEVICE9 ptr_device, float radius, uint32 slices, uint32 stacks)
{
	CSphere* ptr_sphere = new CSphere(ptr_device);
	if (FAILED(ptr_sphere->CreateSphere(radius, slices, stacks)))
	{
		Safe_Delete(ptr_sphere);
		assert(!"Failed to Create Debug Sphere.");
	}

	return ptr_sphere;
}
