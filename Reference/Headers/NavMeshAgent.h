#pragma once

#include "Component.h"

BEGIN(Engine)

struct AStarNode
{
	int index = 0;
	float cost = 0.f;
	AStarNode* parent = nullptr;
};

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
	int MoveFromNavMesh(Vector3& pos, const Vector3& dir, int current_index, int& out_pass_fail_option);
	int MoveFromNavMesh(Vector3& pre_pos, const Vector3& next_pos, int current_index);
	int FindCellIndex(const Vector3& pos);

public:
	bool PathFinder(int start_cell, int end_cell, std::vector<Vector3>& path);
	bool PathFinder(int start_cell, const Vector3& end_point, std::vector<Vector3>& path);
	
private:
	bool ComputePathAsAStar(int start_cell, int end_cell, std::vector<Vector3>& path);
	AStarNode* MakeNode(int index, int end_cell, AStarNode* parent);
	bool CheckListIndex(int index, const std::list<AStarNode*>& open_list, const std::list<AStarNode*>& close_list);

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