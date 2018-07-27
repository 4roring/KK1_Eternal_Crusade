#include "stdafx.h"
#include "PlayerCamera.h"
#include "Transform.h"

CPlayerCamera::CPlayerCamera(LPDIRECT3DDEVICE9 ptr_device)
	: Engine::CCamera(ptr_device)
{
}

CPlayerCamera::~CPlayerCamera()
{
	Release();
}

HRESULT CPlayerCamera::InitCamera(MAINTAINID stage_id)
{
	HRESULT hr = CCamera::InitCamera();
	assert(!FAILED(hr) && "Camera Init Failed");
	hr = GetComponent(stage_id);

	SetProjection(D3DXToRadian(60.f), float(kWinCx) / kWinCy, 0.2f, 1000.f);
	return hr;
}

void CPlayerCamera::Update(float delta_time)
{
	CCamera::Update(delta_time);

	Vector3 player_right = ptr_player_transform_->Right().Normalize();
	Vector3 player_up = ptr_player_transform_->Up().Normalize();
	Vector3 player_forward = ptr_player_transform_->Forward().Normalize();

	eye() = ptr_player_transform_->position() + player_up * 2.f - player_forward * 3.5f + player_right * 1.5f;
	at() = eye() + player_forward * 100.f;
}

CPlayerCamera * CPlayerCamera::Create(LPDIRECT3DDEVICE9 ptr_device, MAINTAINID stage_id)
{
	CPlayerCamera* ptr_player_camera = new CPlayerCamera(ptr_device);
	if (FAILED(ptr_player_camera->InitCamera(stage_id)))
	{
		Safe_Delete(ptr_player_camera);
		assert(!"Player Camera Create Failed");
	}
	return ptr_player_camera;
}

void CPlayerCamera::Release()
{
	Safe_Release(ptr_player_transform_);
}

HRESULT CPlayerCamera::GetComponent(MAINTAINID stage_id)
{
	ptr_player_transform_ = Engine::GameManager()->GetComponent<Engine::CTransform>(stage_id, TEXT("SpaceMarin"), TEXT("Transform"));
	if (nullptr == ptr_player_transform_) return E_FAIL;
	ptr_player_transform_->AddReferenceCount();

	return S_OK;
}
