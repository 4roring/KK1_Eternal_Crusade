#include "Transform.h"

Engine::CTransform::CTransform(const Vector3& look)
{
	D3DXMatrixIdentity(&transform_.mat_world);
	transform_.scale = Vector3(1.f, 1.f, 1.f);
	transform_.move_dir = look;
	++reference_count_;
}

Engine::CTransform::~CTransform()
{

}

Engine::CComponent * Engine::CTransform::CloneComponent()
{
	return new CTransform(Vector3(0.f, 0.f, 0.f));
}

Vector3 & Engine::CTransform::position()
{
	return transform_.position;
}

Vector3 & Engine::CTransform::rotation()
{
	return transform_.rotation;
}

Vector3 & Engine::CTransform::scale()
{
	return transform_.scale;
}

Vector3 & Engine::CTransform::move_dir()
{
	return transform_.move_dir;
}

Matrix & Engine::CTransform::mat_world()
{
	return transform_.mat_world;
}

Vector3 & Engine::CTransform::Right() const
{
	return *(Vector3*)&transform_.mat_world.m[0][0];
}

Vector3 & Engine::CTransform::Up() const
{
	return *(Vector3*)&transform_.mat_world.m[1][0];
}

Vector3 & Engine::CTransform::Forward() const
{
	return *(Vector3*)&transform_.mat_world.m[2][0];
}

void Engine::CTransform::LookAt(Vector3& look_target)
{
	const Vector3 dir = Vector3(0.f, 0.f, 1.f);
	const Vector3 target_dir = (look_target - transform_.position).Normalize();

	float cos = Vector3::Dot(dir, target_dir);
	transform_.rotation.y = acosf(cos);
	if (look_target.x < transform_.position.x)
		transform_.rotation.y *= -1.f;

	this->Update(0.f);
}

void Engine::CTransform::LookAt_XY(Vector3 & look_target)
{
	Vector3 forward_vector = (look_target - transform_.position).Normalize();
	Vector3 right_vector = Vector3::Cross(Vector3(0.f, 1.f), forward_vector).Normalize();
	Vector3 up_vector = Vector3::Cross(forward_vector, right_vector).Normalize();

	*(Vector3*)&transform_.mat_world.m[0][0] = right_vector * transform_.scale.x; 
	*(Vector3*)&transform_.mat_world.m[1][0] = up_vector * transform_.scale.y;
	*(Vector3*)&transform_.mat_world.m[2][0] = forward_vector * transform_.scale.z;
}

void Engine::CTransform::Update(float delta_time)
{
	Matrix mat_scale, mat_rot, mat_trans;
	D3DXMatrixScaling(&mat_scale, transform_.scale.x, transform_.scale.y, transform_.scale.z);
	D3DXMatrixRotationYawPitchRoll(&mat_rot, transform_.rotation.y, transform_.rotation.x, transform_.rotation.z);
	transform_.mat_world = mat_scale * mat_rot;
	
	transform_.mat_world._41 = transform_.position.x;
	transform_.mat_world._42 = transform_.position.y;
	transform_.mat_world._43 = transform_.position.z;
}

int Engine::CTransform::Release()
{
	if (--reference_count_ == 0)
		return 0;
	
	return reference_count_;
}

Engine::CTransform * Engine::CTransform::Create(const Vector3 & look)
{
	return new CTransform(look);
}