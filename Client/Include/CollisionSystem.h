#pragma once

#include "Engine_Singleton.h"

BEGIN(Engine)
class CCollider;
END

class CFrustum;

typedef std::list<Engine::CCollider*> CollList;

class CCollisionSystem
	: public Engine::CSingleton<CCollisionSystem>
{
private:
	friend CSingleton;
	CCollisionSystem();
	virtual ~CCollisionSystem();

public:
	const CollList& GetColliderList(OBJECT_TAG tag);

public:
	void AddColliderList(Engine::CCollider* ptr_coll, OBJECT_TAG tag);
	void AddRaycastList(Engine::CCollider * ptr_coll, OBJECT_TAG tag);
	void SetFrustum(CFrustum* ptr_frustum);

public:
	void CollisionCheck(Engine::CCollider* ptr_coll, OBJECT_TAG target_tag);
	void TriggerCheck(Engine::CCollider* ptr_coll, OBJECT_TAG target_tag, std::list<Engine::CCollider*>& obj_list);
	bool CollisionCheckToFrustum(Engine::CCollider* ptr_coll);
	bool RaycastCheck(const Vector3& ray_pos, const Vector3& ray_dir, float* out_dist, Engine::CGameObject*& out_hit_obj, OBJECT_TAG target_tag);
	void LastFrame();

private:
	std::list<Engine::CCollider*> collider_list_[TAG_END];
	std::list<Engine::CCollider*> raycast_list_[TAG_END];

private:
	CFrustum* ptr_frustum_ = nullptr;
};

static CCollisionSystem* CollSystem() { return CCollisionSystem::GetInstance(); }