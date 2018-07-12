#pragma once

#include "Component.h"

BEGIN(Engine)

class CNavCell;

class CNavMeshAgent

{
private:
	explicit CNavMeshAgent(LPDIRECT3DDEVICE9 ptr_device);

public:
	~CNavMeshAgent();

private:
	HRESULT Initialize(int cell_container_size);

public:
	void Debug_Render();

private:
	void Release();

public:
	static CNavMeshAgent* Create(LPDIRECT3DDEVICE9 ptr_device, int cell_container_size);

public:
	HRESULT AddNavCell(const Vector3& point_a, const Vector3& point_b, const Vector3& point_c
		, int index, int option, int link_cell_index);

public:
	void LinkCell();
	int MoveFromNavMesh(Vector3& pos, const Vector3& dir, int current_index, int out_pass_fail_option = -1);
	int FindCellIndex(const Vector3& pos);

private:
	void SetPosY(Vector3& pos, int next_index);

public:
	LPDIRECT3DDEVICE9 ptr_device_ = nullptr;
	LPD3DXLINE ptr_line_ = nullptr;

public:
	static CNavMeshAgent* ptr_nav_mesh_agent_;

public:
	int cell_container_size_ = 0;
	std::vector<CNavCell*> vec_nav_cell_;
};
END