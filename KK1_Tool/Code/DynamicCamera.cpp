#include "stdafx.h"
#include "DynamicCamera.h"

DynamicCamera::DynamicCamera(LPDIRECT3DDEVICE9 ptr_device)
	: Engine::CCamera(ptr_device)
{
}

DynamicCamera::~DynamicCamera()
{

}

HRESULT DynamicCamera::InitCamera(const Vector3 & eye, const Vector3 & at)
{
	CCamera::InitCamera();
	
	speed_ = 15.f;

	SetView(eye, at);
	SetProjection(D3DXToRadian(45.f), float(g_kWinCx) / g_kWinCy, 0.2f, 1000.f);

	return S_OK;
}

void DynamicCamera::Update(float delta_time)
{
	CCamera::Update(delta_time);
	InputCheck(delta_time);


}

DynamicCamera * DynamicCamera::Create(LPDIRECT3DDEVICE9 ptr_device, const Vector3 & eye, const Vector3 & at)
{
	DynamicCamera* ptr_camera = new DynamicCamera(ptr_device);
	if (FAILED(ptr_camera->InitCamera(eye, at)))
	{
		Engine::Safe_Delete(ptr_camera);
		assert(!"Dynamic Camera Create Failed");
	}
	return ptr_camera;
}

void DynamicCamera::InputCheck(float delta_time)
{
	if (Engine::Input()->GetKey(KEY::RBUTTON))
	{
		Tool()->DisableCursor();

		if (Engine::Input()->GetKey(KEY::W))
		{
			Vector3 look_vector = at() - eye();
			D3DXVec3Normalize(&look_vector, &look_vector);

			eye() += look_vector * speed_ * delta_time;
			at() += look_vector * speed_ * delta_time;
		}

		if (Engine::Input()->GetKey(KEY::S))
		{
			Vector3 look_vector = at() - eye();
			D3DXVec3Normalize(&look_vector, &look_vector);

			eye() -= look_vector * speed_ * delta_time;
			at() -= look_vector * speed_ * delta_time;
		}

		if (Engine::Input()->GetKey(KEY::A))
		{
			Matrix camera_world;
			D3DXMatrixInverse(&camera_world, nullptr, &mat_view());
			Vector3 right_vector;
			memcpy(&right_vector, &camera_world.m[0][0], sizeof(Vector3));
			D3DXVec3Normalize(&right_vector, &right_vector);

			eye() -= right_vector * speed_ * delta_time;
			at() -= right_vector * speed_ * delta_time;
		}

		if (Engine::Input()->GetKey(KEY::D))
		{
			Matrix camera_world;
			D3DXMatrixInverse(&camera_world, nullptr, &mat_view());
			Vector3 right_vector;
			memcpy(&right_vector, &camera_world.m[0][0], sizeof(Vector3));
			D3DXVec3Normalize(&right_vector, &right_vector);

			eye() += right_vector * speed_ * delta_time;
			at() += right_vector * speed_ * delta_time;
		}

		MouseMoveCheck();
		MouseFixPositon();
	}

	if (Engine::Input()->GetKeyUp(KEY::RBUTTON))
		Tool()->EnableCursor();
}

void DynamicCamera::MouseMoveCheck()
{
	float mouse_delta_move_ = 0.f;
	if (mouse_delta_move_ = (float)Engine::Input()->GetDIMouseMove(Engine::CInputManager::DIM_X))
	{
		Matrix mat_axis;
		D3DXMatrixRotationAxis(&mat_axis, &Vector3(0.f, 1.f, 0.f), D3DXToRadian(mouse_delta_move_ * 0.1f));

		Vector3 dir = at() - eye();
		D3DXVec3TransformNormal(&dir, &dir, &mat_axis);

		at() = eye() + dir;
	}

	if (mouse_delta_move_ = (float)Engine::Input()->GetDIMouseMove(Engine::CInputManager::DIM_Y))
	{
		Matrix camera_world;
		D3DXMatrixInverse(&camera_world, nullptr, &mat_view());

		Vector3 right_vector;
		memcpy(&right_vector, &camera_world.m[0][0], sizeof(Vector3));
		D3DXVec3Normalize(&right_vector, &right_vector);

		Matrix mat_axis;
		D3DXMatrixRotationAxis(&mat_axis, &right_vector, D3DXToRadian(mouse_delta_move_ * 0.1f));

		Vector3 dir = at() - eye();
		D3DXVec3TransformNormal(&dir, &dir, &mat_axis);

		at() = eye() + dir;
	}
}

void DynamicCamera::MouseFixPositon()
{
	POINT mouse_fix_pos = { g_kWinCx >> 1, g_kWinCy >> 1 };
	ClientToScreen(g_hwnd, &mouse_fix_pos);
	SetCursorPos(mouse_fix_pos.x, mouse_fix_pos.y);
}
