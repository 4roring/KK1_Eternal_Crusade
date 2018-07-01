#include "Mesh.h"

Engine::CMesh::CMesh(LPDIRECT3DDEVICE9 ptr_device)
	: CResources(ptr_device)
{
}

Engine::CMesh::~CMesh()
{
}

void Engine::CMesh::GetComputeBoundingBox(const Vector3 & min, const Vector3 & max) const
{
}

void Engine::CMesh::GetComputeBoundingSphere(const Vector3 & min, const Vector3 & max) const
{
}
