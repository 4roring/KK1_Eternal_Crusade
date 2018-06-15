#include "Transform.h"

Engine::CTransform::CTransform(const Vector3 & look)
	: direction_(look)
{
	D3DXMatrixIdentity(&mat_world_);
	++reference_count_;
}

Engine::CTransform::~CTransform()
{
}

Engine::CComponent * Engine::CTransform::CloneComponent()
{
	return new CTransform(Vector3(0.f, 0.f, 1.f));
}

void Engine::CTransform::Update(float delta_time)
{
	Matrix mat_scale, mat_rot, mat_trans;
	D3DXMatrixScaling(&mat_scale, scale_.x, scale_.y, scale_.z);
	D3DXMatrixRotationYawPitchRoll(&mat_rot, rotation_.y, rotation_.x, rotation_.z);
	mat_world_ = mat_scale * mat_rot;
	
	mat_world_._41 = position_.x;
	mat_world_._42 = position_.y;
	mat_world_._43 = position_.z;
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