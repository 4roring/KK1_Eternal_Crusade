#include "stdafx.h"
#include "CollisionSystem.h"
#include "Collider.h"
#include "GameObject.h"

CCollisionSystem::CCollisionSystem()
{
}

CCollisionSystem::~CCollisionSystem()
{
}

void CCollisionSystem::AddColisionList(Engine::CCollider * ptr_coll, OBJECT_TAG tag)
{
	if (nullptr == ptr_coll) return;
	collision_list_[tag].emplace_back(ptr_coll);
}

void CCollisionSystem::AddTriggerList(Engine::CCollider * ptr_coll, OBJECT_TAG tag)
{
	if (nullptr == ptr_coll) return;
	trigger_list_[tag].emplace_back(ptr_coll);
}

void CCollisionSystem::AddRaycastList(Engine::CCollider * ptr_coll, OBJECT_TAG tag)
{
	if (nullptr == ptr_coll) return;
	raycast_list_[tag].push_back(ptr_coll);
}

void CCollisionSystem::CollisionCheck(Engine::CCollider * ptr_coll, OBJECT_TAG target_tag)
{
	for (auto& coll : collision_list_[target_tag])
		ptr_coll->CollisionCheck(coll);
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
	for (auto& list : collision_list_)
		list.clear();

	for (auto& list : trigger_list_)
		list.clear();

	for (auto& list : raycast_list_)
		list.clear();
}
