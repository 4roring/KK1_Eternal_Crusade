#include "NavMeshAgent.h"
#include "NavCell.h"
#include "NavLine.h"
#include <queue>

Engine::CNavMeshAgent* Engine::CNavMeshAgent::ptr_nav_mesh_agent_ = nullptr;

Engine::CNavMeshAgent::CNavMeshAgent(LPDIRECT3DDEVICE9 ptr_device)
	: ptr_device_(ptr_device)
{
}

Engine::CNavMeshAgent::~CNavMeshAgent()
{
	if(nullptr != ptr_nav_mesh_agent_)
		Release();
}

HRESULT Engine::CNavMeshAgent::Initialize(int cell_container_size)
{
	cell_container_size_ = cell_container_size;
	vec_nav_cell_.resize(cell_container_size_, nullptr);

	return D3DXCreateLine(ptr_device_, &ptr_line_);
}

void Engine::CNavMeshAgent::Debug_Render()
{
	for (auto& nav_cell : vec_nav_cell_)
	{
		if (nullptr == nav_cell) continue;
		nav_cell->Render(ptr_line_);
	}
}

void Engine::CNavMeshAgent::Release()
{
	for (auto& nav_cell : vec_nav_cell_)
		Safe_Delete(nav_cell);
	vec_nav_cell_.clear();

	Safe_Release(ptr_line_);
}

Engine::CNavMeshAgent * Engine::CNavMeshAgent::Create(LPDIRECT3DDEVICE9 ptr_device, int cell_container_size)
{
	if (nullptr == ptr_nav_mesh_agent_)
	{
		ptr_nav_mesh_agent_ = new CNavMeshAgent(ptr_device);
		if (FAILED(ptr_nav_mesh_agent_->Initialize(cell_container_size)))
		{
			Safe_Delete(ptr_nav_mesh_agent_);
			assert(!"NavMeshAgent Create Failed");
		}
		return ptr_nav_mesh_agent_;
	}
	return nullptr;
}

HRESULT Engine::CNavMeshAgent::AddNavCell(const Vector3 & point_a, const Vector3 & point_b, const Vector3 & point_c, int index, int option, int link_cell_index)
{
	CNavCell* ptr_nav_cell = CNavCell::Create(ptr_device_, point_a, point_b, point_c, index, option, link_cell_index);
	if (nullptr == ptr_nav_cell) return E_FAIL;
	vec_nav_cell_[index] = ptr_nav_cell;

	return S_OK;
}

void Engine::CNavMeshAgent::LinkCell()
{
	for (const auto& src_cell : vec_nav_cell_)
	{
		if (nullptr == src_cell) continue;

		for (auto& dst_cell : vec_nav_cell_)
		{
			if (dst_cell == src_cell || nullptr == dst_cell)
				continue;

			if (src_cell->ComparePoint(dst_cell->GetPoint(POINT_A)
				, dst_cell->GetPoint(POINT_B), dst_cell))
			{
				dst_cell->SetNeighbor(NEIGHBOR_AB, src_cell);
			}
			
			if (src_cell->ComparePoint(dst_cell->GetPoint(POINT_B)
				, dst_cell->GetPoint(POINT_C), dst_cell))
			{
				dst_cell->SetNeighbor(NEIGHBOR_BC, src_cell);
			}
			
			if (src_cell->ComparePoint(dst_cell->GetPoint(POINT_C)
				, dst_cell->GetPoint(POINT_A), dst_cell))
			{
				dst_cell->SetNeighbor(NEIGHBOR_CA, src_cell);
			}

			//if (false == src_cell->NeighborNullCheck())
			//	break;
		}
	}
}

int Engine::CNavMeshAgent::MoveFromNavMesh(Vector3 & pos, const Vector3 & dir, int current_index, int out_pass_fail_option)
{
	NEIGHBORID neighbor_id = NEIGHBOR_END;
	int next_index = current_index;

	if (true == vec_nav_cell_[current_index]->CheckPass(pos, dir, neighbor_id))
	{
		const CNavCell* ptr_neighbor = vec_nav_cell_[current_index]->GetNeighbor(neighbor_id);
		if (nullptr == ptr_neighbor)
		{
			out_pass_fail_option = vec_nav_cell_[current_index]->option();
			Vector3 sliding_dir = vec_nav_cell_[current_index]->GetLine((LINEID)neighbor_id)->SlidingDirection(dir);
			
			if (true == vec_nav_cell_[current_index]->CheckPass(pos, sliding_dir, neighbor_id))
			{
				if (nullptr != ptr_neighbor)
				{
					next_index = ptr_neighbor->index();
					pos += sliding_dir;
				}
				else
					out_pass_fail_option = vec_nav_cell_[current_index]->option();
			}
			else
				pos += sliding_dir;
		}	
		else
		{
			next_index = ptr_neighbor->index();
			pos += dir;
		}
	}
	else
		pos += dir;

	SetPosY(pos, next_index);
	return next_index;
}

int Engine::CNavMeshAgent::FindCellIndex(const Vector3 & pos)
{
	for (auto& nav_cell : vec_nav_cell_)
	{
		if (true == nav_cell->CheckInsideCell(pos))
			return nav_cell->index();
	}
	return -1;
}

bool Engine::CNavMeshAgent::PathFinder(int start_cell, int end_cell, std::vector<Vector3>& path)
{
	if (start_cell == end_cell)
		return false;

	path.clear();
	return ComputePathAsAStar(start_cell, end_cell, path);
}

bool Engine::CNavMeshAgent::PathFinder(int start_cell, const Vector3 & end_point, std::vector<Vector3>& path)
{
	int end_cell = FindCellIndex(end_point);

	if (start_cell == end_cell || end_cell == -1)
		return false;

	path.clear();
	return ComputePathAsAStar(start_cell, end_cell, path);
}

bool Engine::CNavMeshAgent::ComputePathAsAStar(int start_cell, int end_cell, std::vector<Vector3>& path)
{
	AStarNode* parent_node = new AStarNode;
	parent_node->parent = nullptr;
	parent_node->index = start_cell;
	parent_node->cost = 0.f;

	int index = start_cell;
	std::list<AStarNode*> open_list;
	std::list<AStarNode*> close_list;

	close_list.emplace_back(parent_node);

	while (true)
	{
		for (int i = 0; i < NEIGHBOR_END; ++i)
		{
			const CNavCell* neighbor = vec_nav_cell_[parent_node->index]->GetNeighbor((NEIGHBORID)i);
			if (nullptr == neighbor)
				continue;
			
			index = neighbor->index();

			if (false == CheckListIndex(index, open_list, close_list))
				continue;
			
			AStarNode* node = MakeNode(index, end_cell, parent_node);
			open_list.emplace_back(node);
		}
		open_list.sort([](auto& src, auto& dst)->bool { return src->cost < dst->cost; });
		parent_node = open_list.front();
		if (parent_node->index == end_cell)
			break;

		open_list.pop_front();
		close_list.emplace_back(parent_node);
	}

	while (nullptr != parent_node->parent)
	{
		path.emplace_back(vec_nav_cell_[parent_node->index]->center());
		parent_node = parent_node->parent;
	}

	for (auto& node : open_list) Safe_Delete(node);
	open_list.clear();
	for (auto& node : close_list) Safe_Delete(node);
	close_list.clear();

	return true;
}

Engine::AStarNode * Engine::CNavMeshAgent::MakeNode(int index, int end_cell, AStarNode * parent)
{
	AStarNode* node = new AStarNode;
	float g = (vec_nav_cell_[index]->center() - vec_nav_cell_[parent->index]->center()).Magnitude();
	float h = (vec_nav_cell_[end_cell]->center() - vec_nav_cell_[index]->center()).Magnitude();

	node->cost = g + h;
	node->index = index;
	node->parent = parent;

	return node;
}

bool Engine::CNavMeshAgent::CheckListIndex(int index, const std::list<AStarNode*>& open_list, const std::list<AStarNode*>& close_list)
{
	if (false == open_list.empty())
	{
		auto iter_open = std::find_if(open_list.begin(), open_list.end(), [index](auto& node)->bool { return node->index == index; });
		if (iter_open != open_list.end()) return false;
	}

	if (false == close_list.empty())
	{
		auto iter_close = std::find_if(close_list.begin(), close_list.end(), [index](auto& node)->bool { return node->index == index; });
		if (iter_close != close_list.end()) return false;
	}

	return true;
}

void Engine::CNavMeshAgent::SetPosY(Vector3 & pos, int next_index)
{
	D3DXPLANE cell_plane = {};
	D3DXPlaneFromPoints(&cell_plane, &vec_nav_cell_[next_index]->GetPoint(POINT_A)
		, &vec_nav_cell_[next_index]->GetPoint(POINT_B)
		, &vec_nav_cell_[next_index]->GetPoint(POINT_C));

	pos.y = (-cell_plane.a * pos.x - cell_plane.c * pos.z - cell_plane.d) / cell_plane.b;
}

