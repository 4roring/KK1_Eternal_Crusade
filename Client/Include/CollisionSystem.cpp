#include "stdafx.h"
#include "CollisionSystem.h"
#include "Collider.h"
#include "GameObject.h"
#include "Frustum.h"

CCollisionSystem::CCollisionSystem()
{
}

CCollisionSystem::~CCollisionSystem()
{
}

const CollList & CCollisionSystem::GetColliderList(OBJECT_TAG tag)
{
	return collider_list_[tag];
}

void CCollisionSystem::AddColliderList(Engine::CCollider * ptr_coll, OBJECT_TAG tag)
{
	if (nullptr == ptr_coll) return;
	collider_list_[tag].emplace_back(ptr_coll);
}

void CCollisionSystem::AddRaycastList(Engine::CCollider * ptr_coll, OBJECT_TAG tag)
{
	if (nullptr == ptr_coll) return;
	raycast_list_[tag].push_back(ptr_coll);
}

void CCollisionSystem::SetFrustum(CFrustum * ptr_frustum)
{
	ptr_frustum_ = ptr_frustum;
}

void CCollisionSystem::CollisionCheck(Engine::CCollider * ptr_coll, OBJECT_TAG target_tag)
{
	for (auto& coll : collider_list_[target_tag])
		ptr_coll->CollisionCheck(coll, Vector3());
}

void CCollisionSystem::TriggerCheck(Engine::CCollider * ptr_coll, OBJECT_TAG target_tag, std::list<Engine::CCollider*>& obj_list)
{
	for (auto& coll : collider_list_[target_tag])
	{
		if (true == ptr_coll->TriggerCheck(coll))
			obj_list.emplace_back(coll);
	}
}

bool CCollisionSystem::CollisionCheckToFrustum(Engine::CCollider * ptr_coll)
{
	if (nullptr == ptr_coll) return false;

	switch (ptr_coll->type())
	{
	case Collider_AABB:
		return ptr_frustum_->CheckFrutumToAABB(ptr_coll);
	case Collider_Sphere:
		return ptr_frustum_->CheckFrutumToSphere(ptr_coll);
	}
	return false;
}

bool CCollisionSystem::RaycastCheck(const Vector3 & ray_pos, const Vector3 & ray_dir, float * out_dist, Engine::CGameObject*& out_hit_obj, OBJECT_TAG target_tag)
{
	*out_dist = 9999.f;
	float new_dist = 0.f;
	for (auto& coll : raycast_list_[target_tag])
	{
		if (true == coll->RaycastCheck(ray_pos, ray_dir, &new_dist))
		{
			if (new_dist < *out_dist)
			{
				*out_dist = new_dist;
				out_hit_obj = coll->ptr_object();
			}
		}
	}

	if (nullptr != out_hit_obj) return true;
	return false;
}

void CCollisionSystem::LastFrame()
{
	for (auto& list : collider_list_)
		list.clear();

	for (auto& list : raycast_list_)
		list.clear();
}
