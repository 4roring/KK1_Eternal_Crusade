#include "stdafx.h"
#include "Frustum.h"
#include "Collider.h"

CFrustum::CFrustum()
{
}

CFrustum::~CFrustum()
{
}

HRESULT CFrustum::Initialize()
{
	ori_point_[0] = Vector3(-1.f, 1.f, 0.f);
	ori_point_[1] = Vector3(1.f, 1.f, 0.f);
	ori_point_[2] = Vector3(1.f, -1.f, 0.f);
	ori_point_[3] = Vector3(-1.f, -1.f, 0.f);

	ori_point_[4] = Vector3(-1.f, 1.f, 1.f);
	ori_point_[5] = Vector3(1.f, 1.f, 1.f);
	ori_point_[6] = Vector3(1.f, -1.f, 1.f);
	ori_point_[7] = Vector3(-1.f, -1.f, 1.f);

	return S_OK;
}

CFrustum * CFrustum::Create()
{
	CFrustum* ptr_frustum = new CFrustum;
	if (ptr_frustum->Initialize())
		Engine::Safe_Delete(ptr_frustum);

	return ptr_frustum;
}

void CFrustum::FrustumToView(const Matrix & inv_mat_proj)
{
	for (int i = 0; i < 8; ++i)
		D3DXVec3TransformCoord(&view_point_[i], &ori_point_[i], &inv_mat_proj);
}

void CFrustum::FrustumToWorld(const Matrix & inv_mat_view)
{
	for (int i = 0; i < 8; ++i)
		D3DXVec3TransformCoord(&world_point_[i], &view_point_[i], &inv_mat_view);

	MakeFrustumPlane();
}

void CFrustum::MakeFrustumPlane()
{
	Vector3 vec_ab, vec_ac;

	D3DXPlaneFromPoints(&plane_[0], &world_point_[1], &world_point_[5], &world_point_[6]); // +x
	D3DXPlaneFromPoints(&plane_[1], &world_point_[4], &world_point_[0], &world_point_[3]); // -x
	D3DXPlaneFromPoints(&plane_[2], &world_point_[4], &world_point_[5], &world_point_[1]); // +y
	D3DXPlaneFromPoints(&plane_[3], &world_point_[3], &world_point_[2], &world_point_[6]); // -y
	D3DXPlaneFromPoints(&plane_[4], &world_point_[7], &world_point_[6], &world_point_[5]); // +z
	D3DXPlaneFromPoints(&plane_[5], &world_point_[0], &world_point_[1], &world_point_[2]); // -z

	for (int i = 0; i < 6; ++i)
	{
		vec_ab = plane_[i].b - plane_[i].a;
		vec_ac = plane_[i].c - plane_[i].a;
		plane_normal_[i] = Vector3::Cross(vec_ab, vec_ac).Normalize();
	}
}

bool CFrustum::CheckFrutumToSphere(Engine::CCollider * ptr_sphere_coll)
{
	const float radius = ptr_sphere_coll->GetSphereRadius();
	const Vector3& center = ptr_sphere_coll->GetSpherePos();

	for (auto& plane : plane_)
	{
		if (D3DXPlaneDotCoord(&plane, &center) > radius)
			return false;
	}
	return true;
}

bool CFrustum::CheckFrutumToAABB(Engine::CCollider * ptr_aabb_coll)
{
	bool is_intersect = false;
	Vector3 aabb_min, aabb_max;
	ptr_aabb_coll->GetAABBMinMax(aabb_min, aabb_max);
	Vector3 _min, _max;

	for(int i = 0; i < 6; ++i)
	{
		if (plane_normal_[i].x > 0)
		{
			_min.x = aabb_min.x;
			_max.x = aabb_max.x;
		}
		else
		{
			_min.x = aabb_max.x;
			_max.x = aabb_min.x;
		}

		if (plane_normal_[i].y > 0)
		{
			_min.y = aabb_min.y;
			_max.y = aabb_max.y;
		}
		else
		{
			_min.y = aabb_max.y;
			_max.y = aabb_min.y;
		}

		if (plane_normal_[i].z > 0)
		{
			_min.z = aabb_min.z;
			_max.z = aabb_max.z;
		}
		else
		{
			_min.z = aabb_max.z;
			_max.z = aabb_min.z;
		}

		if (Vector3::Dot(plane_normal_[i], _min) + plane_[i].d > 0)
			return false;

		if (Vector3::Dot(plane_normal_[i], _max) + plane_[i].d >= 0)
			is_intersect = true;
	}

	return is_intersect;
}
