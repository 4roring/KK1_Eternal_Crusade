#pragma once

#include "ColliderInfo.h"

BEGIN(Engine)

class CGameObject;

class ENGINE_DLL CCollider
	: public CComponent
{	
private:
	explicit CCollider(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CCollider();

public:
	CGameObject* ptr_object();
	ColliderType type();
	const Vector3& GetSpherePos();

private:
	HRESULT InitCollider(CGameObject* ptr_object, ColliderType coll_type);

public:
	void DebugRender();
	bool CollisionCheck(CCollider* ptr_coll);
	bool TriggerCheck(CCollider* ptr_coll);
	bool RaycastCheck(const Vector3& ray_pos, const Vector3& ray_dir, float* out_dist);

private:
	bool CollisionSphereToSphere(CCollider* ptr_src, CCollider* ptr_dst);

private:
	bool CheckRayToSphere(const Vector3& ray_pos, const Vector3& ray_dir, float* out_dist);
	bool CheckRayToBox(const Vector3& ray_pos, const Vector3& ray_dir, float* out_dist);

public:
	int Release() override;

public:
	static CCollider* Create(LPDIRECT3DDEVICE9 ptr_device, CGameObject* ptr_object, ColliderType coll_type);
	
public:
	void SetSphereCollider(float _radius, const Vector3 & center);
	void SetAABBCollider(const Vector3& _min, const Vector3& _max);

private:
	ColliderType type_ = Collider_End;
	CGameObject* ptr_object_ = nullptr;

private:
	SphereCollider* ptr_sphere_info_ = nullptr;
	AABBCollider* ptr_aabb_info_ = nullptr;
	StaticOBBCollider* ptr_static_obb_info_ = nullptr;
	DynamicOBBCollider* ptr_dynamic_obb_info_ = nullptr;

private:
	LPDIRECT3DDEVICE9 ptr_device_ = nullptr;
};

END