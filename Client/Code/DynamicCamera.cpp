#include "stdafx.h"
#include "DynamicCamera.h"

CDynamicCamera::CDynamicCamera(LPDIRECT3DDEVICE9 ptr_device)
	: Engine::CCamera(ptr_device)
{
}

CDynamicCamera::~CDynamicCamera()
{
}

HRESULT CDynamicCamera::InitCamera(const Vector3 & eye, const Vector3 & at)
{
	speed_ = 15.f;

	eye_ = eye;
	at_ = at;
	up_ = Vector3(0.f, 1.f, 0.f);
	Invalidate_View();

	fov_y_ = D3DXToRadian(45.f);
	aspect_ = float(kWinCx) / kWinCy;
	near_ = 1.f;
	far_ = 1000.f;
	Invalidate_Projection();

	return S_OK;
}

void CDynamicCamera::Update(float delta_time)
{
	CCamera::Update(delta_time);
	InputCheck(delta_time);
	if (true == mouse_fix_)
	{
		MouseFixMove();
		MouseMove();
	}
}

CDynamicCamera * CDynamicCamera::Create(LPDIRECT3DDEVICE9 ptr_device, const Vector3 & eye, const Vector3 & at)
{
	CDynamicCamera* ptr_camera = new CDynamicCamera(ptr_device);
	if (FAILED(ptr_camera->InitCamera(eye, at)))
	{
		Safe_Delete(ptr_camera);
		assert(!"Dynamic Camera Create Failed");
	}
	return ptr_camera;
}

void CDynamicCamera::InputCheck(float delta_time)
{
	if (Engine::Input()->GetKeyDown(KEY::Q))
		mouse_fix_ = !mouse_fix_;

	if (false == mouse_fix_)
		return;

	if (Engine::Input()->GetKey(KEY::W))
	{
		Vector3 look_vector = at_ - eye_;
		D3DXVec3Normalize(&look_vector, &look_vector);

		eye_ += look_vector * speed_ * delta_time;
		at_ += look_vector * speed_ * delta_time;
	}

	if (Engine::Input()->GetKey(KEY::S))
	{
		Vector3 look_vector = at_ - eye_;
		D3DXVec3Normalize(&look_vector, &look_vector);

		eye_ -= look_vector * speed_ * delta_time;
		at_ -= look_vector * speed_ * delta_time;
	}

	if (Engine::Input()->GetKey(KEY::A))
	{
		Matrix camera_world;
		D3DXMatrixInverse(&camera_world, nullptr, &mat_view_);
		Vector3 right_vector;
		memcpy(&right_vector, &camera_world.m[0][0], sizeof(Vector3));
		D3DXVec3Normalize(&right_vector, &right_vector);

		eye_ -= right_vector * speed_ * delta_time;
		at_ -= right_vector * speed_ * delta_time;
	}

	if (Engine::Input()->GetKey(KEY::D))
	{
		Matrix camera_world;
		D3DXMatrixInverse(&camera_world, nullptr, &mat_view_);
		Vector3 right_vector;
		memcpy(&right_vector, &camera_world.m[0][0], sizeof(Vector3));
		D3DXVec3Normalize(&right_vector, &right_vector);

		eye_ += right_vector * speed_ * delta_time;
		at_ += right_vector * speed_ * delta_time;
	}
}

void CDynamicCamera::MouseMove()
{
	int distance = 0;
	if (distance = Engine::Input()->GetDIMouseMove(Engine::CInputManager::DIM_X))
	{
		Matrix mat_axis;
		D3DXMatrixRotationAxis(&mat_axis, &Vector3(0.f, 1.f, 0.f), D3DXToRadian(distance * 0.1f));

		Vector3 dir = at_ - eye_;
		D3DXVec3TransformNormal(&dir, &dir, &mat_axis);

		at_ = eye_ + dir;
	}

	if (distance = Engine::Input()->GetDIMouseMove(Engine::CInputManager::DIM_Y))
	{
		Matrix camera_world;
		D3DXMatrixInverse(&camera_world, nullptr, &mat_view_);

		Vector3 right_vector;
		memcpy(&right_vector, &camera_world.m[0][0], sizeof(Vector3));
		D3DXVec3Normalize(&right_vector, &right_vector);

		Matrix mat_axis;
		D3DXMatrixRotationAxis(&mat_axis, &right_vector, D3DXToRadian(distance * 0.1f));

		Vector3 dir = at_ - eye_;
		D3DXVec3TransformNormal(&dir, &dir, &mat_axis);

		at_ = eye_ + dir;
	}
}

void CDynamicCamera::MouseFixMove()
{
	POINT mouse_fix_pos = { kWinCx >> 1, kWinCy >> 1 };
	ClientToScreen(g_hwnd, &mouse_fix_pos);
	SetCursorPos(mouse_fix_pos.x, mouse_fix_pos.y);
}
