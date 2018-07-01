#include "Sphere.h"

Engine::CSphere::CSphere(LPDIRECT3DDEVICE9 ptr_device)
{
}

Engine::CSphere::~CSphere()
{
}

HRESULT Engine::CSphere::CreateSphere()
{
	return E_NOTIMPL;
}

void Engine::CSphere::Render()
{
	ptr_sphere_mesh_->DrawSubset(0);
}

void Engine::CSphere::Release()
{
	Safe_Release(ptr_sphere_mesh_);
}

void Engine::CSphere::SetWorld(float radius, const Vector3 & position)
{
	Matrix mat_world;
	D3DXMatrixIdentity(&mat_world);

	mat_world._11 = mat_world._22 = mat_world._33 = radius * 2.f;
	memcpy(&mat_world._41, position, sizeof(Vector3));

	ptr_device_->SetTransform(D3DTS_WORLD, &mat_world);
}

Engine::CSphere * Engine::CSphere::Create(LPDIRECT3DDEVICE9 ptr_device)
{
	CSphere* ptr_sphere = new CSphere(ptr_device);
	if (FAILED(ptr_sphere->CreateSphere()))
	{
		Safe_Delete(ptr_sphere);
		assert(!"Failed to Create Debug Sphere.");
	}

	return ptr_sphere;
}
