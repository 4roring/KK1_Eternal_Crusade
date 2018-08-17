#include "Particle.h"
#include "Transform.h"

Engine::CParticle::CParticle(LPDIRECT3DDEVICE9 ptr_device)
	: ptr_device_(ptr_device)
{
}

Engine::CParticle::~CParticle()
{
	Release();
}

void Engine::CParticle::ComputeViewZ(const Vector3 & camera_pos)
{
	view_z_ = (ptr_transform_->position() - camera_pos).Magnitude();
}

Engine::CTransform * Engine::CParticle::transform()
{
	return ptr_transform_;
}

float Engine::CParticle::view_z()
{
	return view_z_;
}

HRESULT Engine::CParticle::Initialize()
{
	ptr_transform_ = Engine::CTransform::Create(Vector3(0.f, 0.f, 1.f));

	return S_OK;
}

void Engine::CParticle::Update(float delta_time, const Vector3& position, const Vector3& rotation, const Vector3& scale, const Vector3& camera_pos)
{
	ptr_transform_->position() = position;
	ptr_transform_->Update(delta_time);
	camera_pos_ = camera_pos;
}

void Engine::CParticle::Render(LPD3DXEFFECT ptr_effect, const Matrix * mat_parent)
{
	Matrix mat_view, mat_proj;

	ptr_device_->GetTransform(D3DTS_VIEW, &mat_view);
	ptr_device_->GetTransform(D3DTS_PROJECTION, &mat_proj);
	
	if(nullptr == mat_parent)
		ptr_transform_->LookAt_XY(camera_pos_);
	else
		ptr_transform_->mat_world() *= *mat_parent;

	ptr_effect->SetMatrix("g_mat_world", &ptr_transform_->mat_world());
	ptr_effect->SetMatrix("g_mat_view", &mat_view);
	ptr_effect->SetMatrix("g_mat_projection", &mat_proj);
}

void Engine::CParticle::Release()
{
	Safe_Delete(ptr_transform_);
}
