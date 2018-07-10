#include "NavCell.h"
#include "NavLine.h"

Engine::CNavCell::CNavCell(LPDIRECT3DDEVICE9 ptr_device, const Vector3 & point_a, const Vector3 & point_b, const Vector3 & point_c)
	: ptr_device_(ptr_device)
{
	point_[POINT_A] = point_a;
	point_[POINT_B] = point_b;
	point_[POINT_C] = point_c;
}

Engine::CNavCell::~CNavCell()
{
	Release();
}

const Vector3 & Engine::CNavCell::GetPoint(POINTID point_id)
{
	return point_[point_id];
}

const Engine::CNavCell * Engine::CNavCell::GetNeighbor(NEIGHBORID neighbor_id)
{
	return ptr_neighbor_[neighbor_id];
}

Engine::CNavLine * Engine::CNavCell::GetLine(LINEID line_id) const
{
	return ptr_nav_line_[line_id];
}

int Engine::CNavCell::index() const
{
	return index_;
}

int Engine::CNavCell::option() const
{
	return option_;
}

int Engine::CNavCell::link_cell_index() const
{
	return link_cell_index_;
}

void Engine::CNavCell::SetNeighbor(NEIGHBORID neighbor_id, CNavCell* ptr_neighbor)
{
	ptr_neighbor_[neighbor_id] = ptr_neighbor;
}

HRESULT Engine::CNavCell::InitNavCell(int index, int option, int link_cell_index)
{
	index_ = index;
	option_ = option;
	link_cell_index_ = link_cell_index;

	ptr_nav_line_[LINE_AB] = CNavLine::Create(point_[POINT_A], point_[POINT_B]);
	if (nullptr == ptr_nav_line_[LINE_AB]) return E_FAIL;
	ptr_nav_line_[LINE_BC] = CNavLine::Create(point_[POINT_B], point_[POINT_C]);
	if (nullptr == ptr_nav_line_[LINE_BC]) return E_FAIL;
	ptr_nav_line_[LINE_CA] = CNavLine::Create(point_[POINT_C], point_[POINT_A]);
	if (nullptr == ptr_nav_line_[LINE_CA]) return E_FAIL;

	return S_OK;
}

void Engine::CNavCell::Render(LPD3DXLINE ptr_line)
{
	Vector3 viewport_point[4] = { point_[0], point_[1], point_[2], point_[0] };
	Matrix mat_view, mat_proj;
	ptr_device_->GetTransform(D3DTS_VIEW, &mat_view);
	ptr_device_->GetTransform(D3DTS_PROJECTION, &mat_proj);

	for (auto& point : viewport_point)
	{
		point.y += 0.1f;
		D3DXVec3TransformCoord(&point, &point, &mat_view);
		if (point.z < 0.f)
			point.z = 0.f;
		D3DXVec3TransformCoord(&point, &point, &mat_proj);
	}

	ptr_line->SetWidth(2.f);
	ptr_line->Begin();
	switch (option_)
	{
	case 0:
		ptr_line->DrawTransform(viewport_point, 4, nullptr, D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
		break;
	case 1:
		ptr_line->DrawTransform(viewport_point, 4, nullptr, D3DXCOLOR(1.f, 0.f, 1.f, 1.f));
		break;
	case 2:
		ptr_line->DrawTransform(viewport_point, 4, nullptr, D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
		break;
	}
	ptr_line->End();
}

Engine::CNavCell * Engine::CNavCell::Create(LPDIRECT3DDEVICE9 ptr_device, const Vector3 & point_a, const Vector3 & point_b, const Vector3 & point_c, int index, int option, int link_cell_index)
{
	CNavCell* ptr_nav_cell = new CNavCell(ptr_device, point_a, point_b, point_c);
	if (ptr_nav_cell->InitNavCell(index, option, link_cell_index))
	{
		Safe_Delete(ptr_nav_cell);
		assert(!"NavCell Create Failed");
	}
	return ptr_nav_cell;
}

void Engine::CNavCell::Release()
{
	for (auto& nav_line : ptr_nav_line_)
		Safe_Delete(nav_line);
}

bool Engine::CNavCell::ComparePoint(const Vector3 & first_point, const Vector3 & second_point, const CNavCell * ptr_neightbor)
{
	if (first_point == point_[POINT_A])
	{
		if (second_point == point_[POINT_B])
		{
			ptr_neighbor_[NEIGHBOR_AB] = *ptr_neighbor_;
			return true;
		}
		else if (second_point == point_[POINT_C])
		{
			ptr_neighbor_[NEIGHBOR_CA] = *ptr_neighbor_;
			return true;
		}
	}

	if (first_point == point_[POINT_B])
	{
		if (second_point == point_[POINT_A])
		{
			ptr_neighbor_[NEIGHBOR_AB] = *ptr_neighbor_;
			return true;
		}
		else if (second_point == point_[POINT_C])
		{
			ptr_neighbor_[NEIGHBOR_BC] = *ptr_neighbor_;
			return true;
		}
	}

	if (first_point == point_[POINT_C])
	{
		if (second_point == point_[POINT_A])
		{
			ptr_neighbor_[NEIGHBOR_CA] = *ptr_neighbor_;
			return true;
		}
		else if (second_point == point_[POINT_B])
		{
			ptr_neighbor_[NEIGHBOR_BC] = *ptr_neighbor_;
			return true;
		}
	}

	return false;
}

bool Engine::CNavCell::CheckPass(const Vector3 & pos, const Vector3 & dir, NEIGHBORID & neighbor_id)
{
	for (int i = 0; i < LINE_END; ++i)
	{
		if (ptr_nav_line_[i]->CheckLine(Vector2(pos.x + dir.x, pos.z + dir.z)))
		{
			neighbor_id = NEIGHBORID(i);
			return true;
		}
	}
	return false;
}

bool Engine::CNavCell::NeighborNullCheck()
{
	for (auto& neighbor : ptr_neighbor_)
		if (nullptr == neighbor) return true;

	return false;
}
