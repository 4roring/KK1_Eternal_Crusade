#include "Collider.h"
#include "GameObject.h"
#include "Transform.h"
#include "Sphere.h"
#include "CubeColor.h"

Engine::CCollider::CCollider(LPDIRECT3DDEVICE9 ptr_device)
	: ptr_device_(ptr_device)
{
}

Engine::CCollider::~CCollider()
{
	Release();
}

Engine::CGameObject * Engine::CCollider::ptr_object()
{
	return ptr_object_;
}

ColliderType Engine::CCollider::type()
{
	return type_;
}

void Engine::CCollider::GetAABBMinMax(Vector3& min, Vector3& max)
{
	min = ptr_aabb_info_->min;
	max = ptr_aabb_info_->max;
}

const Vector3 & Engine::CCollider::GetSpherePos()
{
	return ptr_sphere_info_->world_position;
}

float Engine::CCollider::GetSphereRadius()
{
	return ptr_sphere_info_->radius;
}

void Engine::CCollider::SetWorld(Matrix * ptr_world)
{
	switch (type_)
	{
	case Collider_AABB:
		ptr_aabb_info_->ptr_mat_world = ptr_world;
		break;
	case Collider_StaticOBB:
		ptr_static_obb_info_->ptr_mat_world = ptr_world;
		break;
	case Collider_DynamicOBB:
		ptr_dynamic_obb_info_->ptr_mat_world = ptr_world;
		break;
	case Collider_Sphere:
		ptr_sphere_info_->ptr_world = ptr_world;
		break;
	}
}

void Engine::CCollider::set_delta_time(float delta_time)
{
	delta_time_ = delta_time;
}

HRESULT Engine::CCollider::InitCollider(CGameObject * ptr_object, ColliderType coll_type)
{
	if (nullptr == ptr_object) return E_FAIL;
	ptr_object_ = ptr_object;

	type_ = coll_type;

	switch (type_)
	{
	case Collider_AABB:
		ptr_aabb_info_ = new AABBCollider;
		break;
	case Collider_StaticOBB:
		ptr_static_obb_info_ = new StaticOBBCollider;
		break;
	case Collider_DynamicOBB:
		ptr_dynamic_obb_info_ = new DynamicOBBCollider;
		break;
	case Collider_Sphere:
		ptr_sphere_info_ = new SphereCollider;
		break;
	}

	return S_OK;
}

void Engine::CCollider::SetSphereCollider(float _radius, const Vector3& center)
{
	if(nullptr == ptr_sphere_info_->ptr_world)
		ptr_sphere_info_->ptr_world = &ptr_object_->transform()->mat_world();
	ptr_sphere_info_->radius = _radius;
	ptr_sphere_info_->center = center;
	D3DXVec3TransformCoord(&ptr_sphere_info_->world_position, &ptr_sphere_info_->center, ptr_sphere_info_->ptr_world);
	if(nullptr == ptr_sphere_info_->ptr_sphere)
		ptr_sphere_info_->ptr_sphere = CSphere::Create(ptr_device_, _radius, 8, 8);
}

void Engine::CCollider::SetAABBCollider(const Vector3 & _min, const Vector3 & _max)
{
	ptr_aabb_info_->ptr_mat_world = &ptr_object_->transform()->mat_world();
	D3DXVec3TransformCoord(&ptr_aabb_info_->min, &_min, ptr_aabb_info_->ptr_mat_world);
	D3DXVec3TransformCoord(&ptr_aabb_info_->max, &_max, ptr_aabb_info_->ptr_mat_world);
	ptr_aabb_info_->ptr_cube = CCubeColor::Create(ptr_device_, _min, _max, D3DXCOLOR(0.f, 1.f, 0.f, 1.f));
}

void Engine::CCollider::DebugRender()
{
	switch (type_)
	{
	case Collider_AABB:
		if (nullptr == ptr_aabb_info_->ptr_cube) return;
		ptr_aabb_info_->ptr_cube->Render();
		break;
	case Collider_StaticOBB:
		if (nullptr == ptr_static_obb_info_->ptr_cube) return;
		ptr_static_obb_info_->ptr_cube->Render();
		break;
	case Collider_DynamicOBB:
		if (nullptr == ptr_dynamic_obb_info_->ptr_cube) return;
		ptr_dynamic_obb_info_->ptr_cube->Render();
		break;
	case Collider_Sphere:
		if (nullptr == ptr_sphere_info_->ptr_sphere) return;
		ptr_sphere_info_->ptr_sphere->Render();
		break;
	}
}

bool Engine::CCollider::CollisionCheck(CCollider * ptr_coll, Vector3& dist)
{
	if (this == ptr_coll) return false;

	switch (type_)
	{
	case Collider_AABB:

		break;
	case Collider_StaticOBB:
		
		break;
	case Collider_DynamicOBB:
		
		break;
	case Collider_Sphere:
		switch (ptr_coll->type())
		{
		case Collider_AABB:

			break;
		case Collider_StaticOBB:

			break;
		case Collider_DynamicOBB:

			break;
		case Collider_Sphere:
			if (true == CollisionSphereToSphere(this, ptr_coll))
				return true;
			break;
		}
		break;
	}
	return false;
}

bool Engine::CCollider::TriggerCheck(CCollider * ptr_coll)
{
	switch (type_)
	{
	case Collider_AABB:
		switch (ptr_coll->type())
		{
		case Collider_AABB:
			if (true == TriggerSphereToAABB(ptr_coll, this))
				return true;
			break;
		case Collider_StaticOBB:

			break;
		case Collider_DynamicOBB:

			break;
		case Collider_Sphere:
			return true;
			break;
		}
		break;
	case Collider_StaticOBB:
		break;
	case Collider_DynamicOBB:
		break;
	case Collider_Sphere:
		switch (ptr_coll->type())
		{
		case Collider_AABB:
			if (true == TriggerSphereToAABB(this, ptr_coll))
				return true;
			break;
		case Collider_StaticOBB:

			break;
		case Collider_DynamicOBB:

			break;
		case Collider_Sphere:
			if (true == TriggerSphereToSphere(this, ptr_coll))
				return true;
			break;
		}
		break;
	}
	return false;
}

bool Engine::CCollider::RaycastCheck(const Vector3 & ray_pos, const Vector3 & ray_dir, float * out_dist)
{
	// World 공간에서 Sphere와 AABB만 체크
	switch (type_)
	{
	case Collider_AABB:
		CheckRayToBox(ray_pos, ray_dir, out_dist);
		return true;
	case Collider_Sphere:
		if (true == CheckRayToSphere(ray_pos, ray_dir, out_dist))
			return true;
		break;
	}

	return false;
}

bool Engine::CCollider::CollisionSphereToSphere(CCollider * ptr_src, CCollider * ptr_dst)
{
	const float dist = (ptr_src->ptr_sphere_info_->world_position - ptr_dst->ptr_sphere_info_->world_position).Magnitude();
	const float radius_sum = ptr_src->ptr_sphere_info_->radius + ptr_dst->ptr_sphere_info_->radius;

	if (dist <= (radius_sum * radius_sum))
	{
		Vector3 move_dir = (ptr_dst->ptr_object()->transform()->position() - ptr_src->ptr_object()->transform()->position()).Normalize();
		float move_dist = sqrtf(radius_sum - dist) * delta_time_;
		ptr_dst->ptr_object()->transform()->move_dir() = move_dir * move_dist;
		return true;
	}
	return false;
}

bool Engine::CCollider::TriggerSphereToSphere(CCollider * ptr_src, CCollider * ptr_dst)
{
	const float dist = (ptr_src->ptr_sphere_info_->world_position - ptr_dst->ptr_sphere_info_->world_position).Magnitude();
	const float radius_sum = ptr_src->ptr_sphere_info_->radius + ptr_dst->ptr_sphere_info_->radius;

	if (dist <= (radius_sum * radius_sum))
		return true;

	return false;
}

bool Engine::CCollider::TriggerSphereToAABB(CCollider * ptr_sphere_coll, CCollider * ptr_aabb_coll)
{
	Vector3 dist;

	SphereCollider* sphere = ptr_sphere_coll->ptr_sphere_info_;
	const AABBCollider* aabb = ptr_aabb_coll->ptr_aabb_info_;

	dist.x = (sphere->center.x < aabb->min.x) ? aabb->min.x - sphere->center.x : sphere->center.x - aabb->max.x;
	dist.y = (sphere->center.y < aabb->min.y) ? aabb->min.y - sphere->center.y : sphere->center.y - aabb->max.y;
	dist.z = (sphere->center.z < aabb->min.z) ? aabb->min.z - sphere->center.z : sphere->center.z - aabb->max.z;

	const float square_dist_length = dist.Magnitude();
	const float square_radius = sphere->radius * sphere->radius;

	if (square_dist_length > square_radius)
		return false;

	return true;
}

bool Engine::CCollider::CheckRayToSphere(const Vector3 & ray_pos, const Vector3 & ray_dir, float * out_dist)
{
	Vector3 i = ptr_sphere_info_->world_position - ray_pos;
	const float s = Vector3::Dot(i, ray_dir);
	const float i_squared = i.Magnitude();
	const float radius_squared = pow(ptr_sphere_info_->radius, 2);

	if (s < 0 && i_squared > radius_squared)
		return false;

	const float m_squared = i_squared - pow(s, 2);

	if (m_squared > radius_squared)
		return false;

	float q = sqrt(radius_squared - m_squared);

	*out_dist = (i_squared > radius_squared) ? s - q : s + q;
	return true;
}

void Engine::CCollider::CheckRayToBox(const Vector3 & ray_pos, const Vector3 & ray_dir, float * out_dist)
{
	float dist = 99999.f;
	
	const Vector3& _min = ptr_aabb_info_->min;
	const Vector3& _max = ptr_aabb_info_->max;

	const Vector3 point[8] =
	{
		{ _min.x, _max.y , _min.z } // left up front
		, { _max.x, _max.y , _min.z } // right up front
		, { _max.x, _min.y , _min.z } // right down front
		, { _min.x, _min.y , _min.z } // left down front

		, { _min.x, _max.y , _max.z } // left up rear
		, { _max.x, _max.y , _max.z } // right up rear
		, { _max.x, _min.y , _max.z } // right down rear
		, { _min.x, _min.y , _max.z } // left down rear
	};


	D3DXIntersectTri(&point[4], &point[0], &point[3], &ray_pos, &ray_dir, nullptr, nullptr, &dist); //-x left up
	if (dist < *out_dist) *out_dist = dist;
	D3DXIntersectTri(&point[4], &point[3], &point[7], &ray_pos, &ray_dir, nullptr, nullptr, &dist); //-x right down
	if (dist < *out_dist) *out_dist = dist;
	D3DXIntersectTri(&point[1], &point[5], &point[6], &ray_pos, &ray_dir, nullptr, nullptr, &dist); //+x left up
	if (dist < *out_dist) *out_dist = dist;
	D3DXIntersectTri(&point[1], &point[6], &point[2], &ray_pos, &ray_dir, nullptr, nullptr, &dist); //+x right down
	if (dist < *out_dist) *out_dist = dist;

	D3DXIntersectTri(&point[4], &point[5], &point[1], &ray_pos, &ray_dir, nullptr, nullptr, &dist); //+y left up
	if (dist < *out_dist) *out_dist = dist;
	D3DXIntersectTri(&point[4], &point[1], &point[0], &ray_pos, &ray_dir, nullptr, nullptr, &dist); //+y right down
	if (dist < *out_dist) *out_dist = dist;
	D3DXIntersectTri(&point[3], &point[2], &point[6], &ray_pos, &ray_dir, nullptr, nullptr, &dist); //-y left up
	if (dist < *out_dist) *out_dist = dist;
	D3DXIntersectTri(&point[3], &point[6], &point[7], &ray_pos, &ray_dir, nullptr, nullptr, &dist); //-y right down
	if (dist < *out_dist) *out_dist = dist;

	D3DXIntersectTri(&point[0], &point[1], &point[2], &ray_pos, &ray_dir, nullptr, nullptr, &dist); //-z left up
	if (dist < *out_dist) *out_dist = dist;
	D3DXIntersectTri(&point[0], &point[2], &point[3], &ray_pos, &ray_dir, nullptr, nullptr, &dist); //-z right down
	if (dist < *out_dist) *out_dist = dist;
	D3DXIntersectTri(&point[7], &point[6], &point[5], &ray_pos, &ray_dir, nullptr, nullptr, &dist); //+z left up
	if (dist < *out_dist) *out_dist = dist;
	D3DXIntersectTri(&point[7], &point[5], &point[4], &ray_pos, &ray_dir, nullptr, nullptr, &dist); //+z right down
	if (dist < *out_dist) *out_dist = dist;
}

int Engine::CCollider::Release()
{
	Safe_Delete(ptr_sphere_info_);
	Safe_Delete(ptr_aabb_info_);
	Safe_Delete(ptr_static_obb_info_);
	Safe_Delete(ptr_dynamic_obb_info_);

	return reference_count_;
}

Engine::CCollider * Engine::CCollider::Create(LPDIRECT3DDEVICE9 ptr_device, CGameObject * ptr_object, ColliderType coll_type)
{
	CCollider* ptr_coll = new CCollider(ptr_device);
	if (FAILED(ptr_coll->InitCollider(ptr_object, coll_type)))
	{
		Safe_Delete(ptr_coll);
		assert(!"Collider Create Failed");
	}

	return ptr_coll;
}
