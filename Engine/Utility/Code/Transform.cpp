#include "Transform.h"

Engine::CTransform::CTransform(const Vector3 & look)
{
	D3DXMatrixIdentity(&transform_.mat_world);
	transform_.direction = look;
	++reference_count_;
}

Engine::CTransform::~CTransform()
{

}

Engine::CComponent * Engine::CTransform::CloneComponent()
{
	return new CTransform(Vector3(0.f, 0.f, 1.f));
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

Vector3 & Engine::CTransform::direction()
{
	return transform_.direction;
}

Matrix & Engine::CTransform::mat_world()
{
	return transform_.mat_world;
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