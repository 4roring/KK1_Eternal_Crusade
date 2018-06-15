#include "Camera.h"

Engine::CCamera::CCamera(LPDIRECT3DDEVICE9 ptr_device)
	: CGameObject(ptr_device)
{
	D3DXMatrixIdentity(&mat_view_);
	D3DXMatrixIdentity(&mat_projection_);
}

Engine::CCamera::~CCamera()
{
}

void Engine::CCamera::Update(float delta_time)
{
	Invalidate_View();
}

void Engine::CCamera::Invalidate_View()
{
	D3DXMatrixLookAtLH(&mat_view_, &eye_, &at_, &up_);
	ptr_device_->SetTransform(D3DTS_VIEW, &mat_view_);
}

void Engine::CCamera::Invalidate_Projection()
{
	D3DXMatrixPerspectiveFovLH(&mat_projection_, fov_y_, aspect_, near_, far_);
	ptr_device_->SetTransform(D3DTS_PROJECTION, &mat_projection_);
}
