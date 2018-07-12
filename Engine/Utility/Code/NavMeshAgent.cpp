#include "NavMeshAgent.h"
#include "NavCell.h"
#include "NavLine.h"

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

void Engine::CNavMeshAgent::SetPosY(Vector3 & pos, int next_index)
{
	D3DXPLANE cell_plane = {};
	D3DXPlaneFromPoints(&cell_plane, &vec_nav_cell_[next_index]->GetPoint(POINT_A)
		, &vec_nav_cell_[next_index]->GetPoint(POINT_B)
		, &vec_nav_cell_[next_index]->GetPoint(POINT_C));

	pos.y = (-cell_plane.a * pos.x - cell_plane.c * pos.z - cell_plane.d) / cell_plane.b;
}

