#pragma once

#include "Component.h"
#include "Sphere.h"
#include "CubeColor.h"

enum ColliderType { Collider_AABB, Collider_StaticOBB, Collider_DynamicOBB, Collider_Sphere, Collider_Capsule, Collider_End };

struct SphereCollider 
{
	~SphereCollider() { Engine::Safe_Delete(ptr_sphere); }
	Matrix* ptr_world = nullptr;
	Vector3 center;
	Vector3 world_position;
	float radius = 0.f;
	Engine::CSphere* ptr_sphere = nullptr;
};

struct AABBCollider 
{
	~AABBCollider() { Engine::Safe_Release_Delete(ptr_cube); }
	Matrix* ptr_mat_world = nullptr;
	Vector3 min, max;
	Engine::CCubeColor* ptr_cube = nullptr;
};

struct StaticOBBCollider 
{
	~StaticOBBCollider() { Engine::Safe_Release_Delete(ptr_cube); }
	Matrix* ptr_mat_world = nullptr;
	Vector3 min, max;
	Matrix mat_local;
	Engine::CCubeColor* ptr_cube = nullptr;
};

struct DynamicOBBCollider 
{
	enum AXISID { AXIS_X, AXIS_Y, AXIS_Z, AXIS_END };
	~DynamicOBBCollider() { Engine::Safe_Release_Delete(ptr_cube); }
	Matrix* ptr_mat_world = nullptr;
	Vector3 min, max;
	Vector3 center;
	Vector3 proj_vector[AXIS_END];
	Vector3 parallel[AXIS_END];
	Engine::CCubeColor* ptr_cube = nullptr;
};

