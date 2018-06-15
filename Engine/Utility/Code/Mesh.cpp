#include "Mesh.h"

Engine::CMesh::CMesh(LPDIRECT3DDEVICE9 ptr_device)
	: CResources(ptr_device)
{
}

Engine::CMesh::~CMesh()
{
}

void Engine::CMesh::GetMinMax(const Vector3 & min, const Vector3 & max)
{
	min_ = min;
	max_ = max;
}
