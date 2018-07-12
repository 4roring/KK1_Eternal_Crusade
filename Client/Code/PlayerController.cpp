#include "stdafx.h"
#include "PlayerController.h"
#include "Transform.h"

CPlayerController::CPlayerController()
{
}

CPlayerController::~CPlayerController()
{
}

Engine::CComponent * CPlayerController::CloneComponent()
{
	++reference_count_;
	return this;
}

HRESULT CPlayerController::Initialize(Engine::CTransform * ptr_target_transform, float speed, float sensitivity)
{
	if (nullptr == ptr_target_transform) return E_FAIL;
	ptr_ctrl_transform_ = ptr_target_transform;
	speed_ = speed;
	sensitivity_ = sensitivity;

	++reference_count_;
	return S_OK;
}

void CPlayerController::Update(float delta_time)
{
	CheckInput(delta_time);
}

int CPlayerController::Release()
{
	if (--reference_count_ == 0)
	{
		Engine::Safe_Release(ptr_ctrl_transform_);
		return 0;
	}

	return reference_count_;
}

CPlayerController * CPlayerController::Create(Engine::CTransform * ptr_target_transform, float speed, float sensitivity)
{
	CPlayerController* ptr_ctrl = new CPlayerController;
	if (FAILED(ptr_ctrl->Initialize(ptr_target_transform, speed, sensitivity)))
	{
		Safe_Delete(ptr_ctrl);
		assert(!"Player Controller Create Failed");
	}

	return ptr_ctrl;
}

void CPlayerController::CheckInput(float delta_time)
{
	InputKeyboard(delta_time);
	MoveToMouse(delta_time);
}

void CPlayerController::InputKeyboard(float delta_time)
{
	Vector3 _move_dir = Vector3();

	if (Engine::Input()->GetKey(KEY::W))
		_move_dir += ptr_ctrl_transform_->Forward();
	else if (Engine::Input()->GetKey(KEY::S))
		_move_dir -= ptr_ctrl_transform_->Forward();

	if (Engine::Input()->GetKey(KEY::A))
		_move_dir -= ptr_ctrl_transform_->Right();
	else if (Engine::Input()->GetKey(KEY::D))
		_move_dir += ptr_ctrl_transform_->Right();

	if(_move_dir.x > 0.f || _move_dir.z > 0.f)
		_move_dir = _move_dir.Normalize();
	ptr_ctrl_transform_->move_dir() = _move_dir * speed_ * delta_time;
}

void CPlayerController::MoveToMouse(float delta_time)
{
}
