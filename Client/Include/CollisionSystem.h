#pragma once

#include "Engine_Singleton.h"

BEGIN(Engine)
class CCollider;
END

class CSphereCollider;

class CCollisionSystem
	: public Engine::CSingleton<CCollisionSystem>
{
private:
	friend CSingleton;
	CCollisionSystem();
	virtual ~CCollisionSystem();

public:
	void AddColisionList(Engine::CCollider* ptr_coll, OBJECT_TAG tag);
	void AddTriggerList(Engine::CCollider * ptr_coll, OBJECT_TAG tag);
	void AddRaycastList(Engine::CCollider * ptr_coll, OBJECT_TAG tag);

public:
	void CollisionCheck(Engine::CCollider* ptr_coll, OBJECT_TAG target_tag);
	bool RaycastCheck(const Vector3& ray_pos, const Vector3& ray_dir, float* out_dist, Engine::CGameObject*& out_hit_obj, OBJECT_TAG target_tag);
	void LastFrame();

private:
	std::list<Engine::CCollider*> collision_list_[TAG_END];
	std::list<Engine::CCollider*> trigger_list_[TAG_END];
	std::list<Engine::CCollider*> raycast_list_[TAG_END];
};

static CCollisionSystem* CollSystem() { return CCollisionSystem::GetInstance(); }