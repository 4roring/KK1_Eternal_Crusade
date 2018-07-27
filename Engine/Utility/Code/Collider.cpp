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

const Vector3 & Engine::CCollider::GetSpherePos()
{
	return ptr_sphere_info_->world_position;
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
	D3DXVec3TransformCoord(&ptr_aabb_info_->max, &_min, ptr_aabb_info_->ptr_mat_world);
	//ptr_aabb_info_->min = _min;
	//ptr_aabb_info_->max = _max;
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

bool Engine::CCollider::CollisionCheck(CCollider * ptr_coll)
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
	return false;
}

bool Engine::CCollider::RaycastCheck(const Vector3 & ray_pos, const Vector3 & ray_dir, float * out_dist)
{
	// World 공간에서 Sphere와 AABB만 체크
	switch (type_)
	{
	case Collider_AABB:
		if (true == CheckRayToBox(ray_pos, ray_dir, out_dist))
			return true;
		break;
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

	if (dist <= radius_sum)
	{
		Vector3 move_dir = (ptr_dst->ptr_object()->transform()->position() - ptr_src->ptr_object()->transform()->position()).Normalize();
		float move_dist = sqrtf(radius_sum - dist) * 0.3f;
		ptr_dst->ptr_object()->transform()->move_dir() = move_dir * move_dist;
		return true;
	}
	return false;
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

bool Engine::CCollider::CheckRayToBox(const Vector3 & ray_pos, const Vector3 & ray_dir, float * out_dist)
{
	// TODO: 다음에 또는 시간없으면 IntersectTri 노가다.

	return false;
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
