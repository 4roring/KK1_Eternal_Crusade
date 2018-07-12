#include "Camera.h"

struct Engine::CameraInfo
{
	Vector3 eye;
	Vector3 at;
	Vector3 up;

	float fov_y = 0.f;
	float aspect = 0.f;
	float z_near = 0.f;
	float z_far = 0.f;

	Matrix mat_view, mat_projection;
};

Engine::CCamera::CCamera(LPDIRECT3DDEVICE9 ptr_device)
	: CGameObject(ptr_device)
{
}

Engine::CCamera::~CCamera()
{
	Release();
}

Vector3 & Engine::CCamera::eye()
{
	return ptr_camera_info_->eye;
}

Vector3 & Engine::CCamera::at()
{
	return ptr_camera_info_->at;
}

Matrix & Engine::CCamera::mat_view()
{
	return ptr_camera_info_->mat_view;
}

Matrix & Engine::CCamera::mat_projection()
{
	return ptr_camera_info_->mat_projection;
}

void Engine::CCamera::SetView(const Vector3 & eye, const Vector3 & at)
{
	ptr_camera_info_->eye = eye;
	ptr_camera_info_->at = at;

	Invalidate_View();
}

void Engine::CCamera::SetProjection(float fov_y, float aspect, float z_near, float z_far)
{
	ptr_camera_info_->fov_y = fov_y;
	ptr_camera_info_->aspect = aspect;
	ptr_camera_info_->z_near = z_near;
	ptr_camera_info_->z_far = z_far;

	Invalidate_Projection();
}

HRESULT Engine::CCamera::InitCamera()
{
	ptr_camera_info_ = new CameraInfo;

	ptr_camera_info_->up = Vector3(0.f, 1.f, 0.f);
	
	D3DXMatrixIdentity(&ptr_camera_info_->mat_view);
	D3DXMatrixIdentity(&ptr_camera_info_->mat_projection);

	return S_OK;
}

void Engine::CCamera::Update(float delta_time)
{
	Invalidate_View();
}

void Engine::CCamera::Release()
{
	Safe_Delete(ptr_camera_info_);
}

void Engine::CCamera::Invalidate_View()
{
	D3DXMatrixLookAtLH(&ptr_camera_info_->mat_view, &ptr_camera_info_->eye, &ptr_camera_info_->at, &ptr_camera_info_->up);
	ptr_device_->SetTransform(D3DTS_VIEW, &ptr_camera_info_->mat_view);
}

void Engine::CCamera::Invalidate_Projection()
{
	D3DXMatrixPerspectiveFovLH(&ptr_camera_info_->mat_projection
		, ptr_camera_info_->fov_y, ptr_camera_info_->aspect
		, ptr_camera_info_->z_near, ptr_camera_info_->z_far);
	ptr_device_->SetTransform(D3DTS_PROJECTION, &ptr_camera_info_->mat_projection);
}
