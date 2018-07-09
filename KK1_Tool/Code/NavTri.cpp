#include "stdafx.h"
#include "NavTri.h"
#include "NavPoint.h"

#include "Transform.h"

NavTri::NavTri(LPDIRECT3DDEVICE9 ptr_device)
	: ptr_device_(ptr_device)
{
}

NavTri::~NavTri()
{
	Release();
}

HRESULT NavTri::Initialize(const std::array<NavPoint*, 3>& nav_point_array)
{
	const int max_point = nav_point_array.size();
	for(int i = 0; i < max_point; ++i)
		nav_point_array_[i] = nav_point_array[i]->ClonePoint();

	D3DXCreateLine(ptr_device_, &ptr_line_);

	ptr_line_->SetAntialias(TRUE);

	return S_OK;
}

void NavTri::Update(float delta_time)
{
	Matrix mat_view, mat_proj;
	ptr_device_->GetTransform(D3DTS_VIEW, &mat_view);
	ptr_device_->GetTransform(D3DTS_PROJECTION, &mat_proj);

	for(int i=0; i<3;++i)
	{
		viewport_nav_point_[i] = nav_point_array_[i]->transform()->position();
		viewport_nav_point_[i].y += 0.1f;

		D3DXVec3TransformCoord(&viewport_nav_point_[i], &viewport_nav_point_[i], &mat_view);
		if (viewport_nav_point_[i].z <= 0.f)
			viewport_nav_point_[i].z = 0.f;
		D3DXVec3TransformCoord(&viewport_nav_point_[i], &viewport_nav_point_[i], &mat_proj);
	}
	viewport_nav_point_[3] = viewport_nav_point_[0];
}

void NavTri::Render()
{
	ptr_line_->SetWidth(5.f);
	ptr_line_->Begin();
	switch (option_)
	{
	case 0: 
		ptr_line_->DrawTransform(viewport_nav_point_, 4, nullptr, D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
		break;
	case 1:
		ptr_line_->DrawTransform(viewport_nav_point_, 4, nullptr, D3DXCOLOR(1.f, 0.f, 1.f, 1.f));
		break;
	case 2:
		ptr_line_->DrawTransform(viewport_nav_point_, 4, nullptr, D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
		break;
	default:
		break;
	}


	ptr_line_->End();
}

void NavTri::Release()
{
	for (auto& nav_point : nav_point_array_)
		nav_point->Release();

	Engine::Safe_Release(ptr_line_);
}

NavTri * NavTri::Create(LPDIRECT3DDEVICE9 ptr_device, const std::array<NavPoint*, 3>& nav_point_array)
{
	NavTri* ptr_nav_tri = new NavTri(ptr_device);
	if (FAILED(ptr_nav_tri->Initialize(nav_point_array)))
	{
		Engine::Safe_Delete(ptr_nav_tri);
		assert(!"NavTri Create Failed");
	}

	return ptr_nav_tri;
}

bool NavTri::PickNavMesh(const Vector3 & ray_pos, const Vector3 & ray_dir)
{
	BOOL is_hit_tri = D3DXIntersectTri(&nav_point_array_[0]->transform()->position()
		, &nav_point_array_[1]->transform()->position()
		, &nav_point_array_[2]->transform()->position()
		, &ray_pos, &ray_dir, nullptr, nullptr, nullptr);

	if (TRUE == is_hit_tri) return true;
	return false;
}
