#include "stdafx.h"
#include "PlayerController.h"
#include "SpaceMarin.h"
#include "Transform.h"
#include "PlayerCamera.h"
#include "EnemyObserver.h"

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

HRESULT CPlayerController::Initialize(CSpaceMarin * ptr_target, float speed, float sensitivity)
{
	if (nullptr == ptr_target) return E_FAIL;
	ptr_ctrl_unit_ = ptr_target;
	speed_ = speed;
	sensitivity_ = sensitivity;
	fov_y = 60.f;
	++reference_count_;
	Subject()->RegisterObserver(ptr_enemy_observer_);

	return S_OK;
}

void CPlayerController::LateInit()
{
	CController::LateInit();
	ptr_ctrl_transform_ = ptr_ctrl_unit_->transform();
	ptr_ctrl_transform_->AddReferenceCount();
	ptr_forward_transform_ = ptr_ctrl_unit_->ptr_lower_transform();
	ptr_forward_transform_->AddReferenceCount();
	ptr_ctrl_camera_ = dynamic_cast<CPlayerCamera*>(Engine::GameManager()->FindObject(MAINTAIN_STAGE, TEXT("Player_Camera")));
}

void CPlayerController::Update(float delta_time)
{
	CController::Update(delta_time);
	CheckInput(delta_time);
}

int CPlayerController::Release()
{
	if (--reference_count_ == 0)
	{
		Engine::Safe_Release_Delete(ptr_ctrl_transform_);
		Engine::Safe_Release_Delete(ptr_forward_transform_);
		Subject()->RemoveObserver(ptr_enemy_observer_);
		return 0;
	}
	return reference_count_;
}

CPlayerController * CPlayerController::Create(CSpaceMarin * ptr_target, float speed, float sensitivity)
{
	CPlayerController* ptr_ctrl = new CPlayerController;
	if (FAILED(ptr_ctrl->Initialize(ptr_target, speed, sensitivity)))
	{
		Safe_Delete(ptr_ctrl);
		assert(!"Player Controller Create Failed");
	}
	return ptr_ctrl;
}

void CPlayerController::CheckInput(float delta_time)
{
	ControlMove(delta_time);
	ControlAimPoint(delta_time);
	ControlZoom(delta_time);
	ControlAttack(delta_time);
	ControlEvade(delta_time);
	ControlExecution();
	ChangeWeapon();
}

void CPlayerController::ControlMove(float delta_time)
{
	if (ptr_ctrl_unit_->evade() == true) return;
	Vector3 _move_dir = Vector3();

	if (Engine::Input()->GetKey(KEY::W))
	{
		_move_dir += ptr_forward_transform_->Forward();
		ptr_ctrl_unit_->set_move_dir(CSpaceMarin::MoveDirection::Forward);
	}
	else if (Engine::Input()->GetKey(KEY::S))
	{
		_move_dir -= ptr_forward_transform_->Forward();
		ptr_ctrl_unit_->set_move_dir(CSpaceMarin::MoveDirection::Backward);
	}

	if (Engine::Input()->GetKey(KEY::A))
	{
		_move_dir -= ptr_ctrl_transform_->Right();
		ptr_ctrl_unit_->set_move_dir(CSpaceMarin::MoveDirection::Left);
	}
	else if (Engine::Input()->GetKey(KEY::D))
	{
		_move_dir += ptr_ctrl_transform_->Right();
		ptr_ctrl_unit_->set_move_dir(CSpaceMarin::MoveDirection::Right);
	}

	if (_move_dir.x != 0.f || _move_dir.z != 0.f)
		_move_dir = _move_dir.Normalize();
	else
		ptr_ctrl_unit_->set_move_dir(CSpaceMarin::MoveDirection::End);

	ptr_ctrl_transform_->move_dir() = _move_dir * speed_ * delta_time;
}

void CPlayerController::ControlAimPoint(float delta_time)
{
	Engine::Input()->FixMouseCenter(g_hwnd, kWinCx, kWinCy);
	const float max_rot_x = D3DXToRadian(80.f);
	const float delta_move_x = (float)Engine::Input()->GetDIMouseMove(Engine::CInputManager::DIM_X);
	const float delta_move_y = (float)Engine::Input()->GetDIMouseMove(Engine::CInputManager::DIM_Y);

	const float new_rot_x = ptr_ctrl_transform_->rotation().x + (sensitivity_ * delta_move_y * delta_time);
	if (-max_rot_x < new_rot_x && new_rot_x < max_rot_x)
		ptr_ctrl_transform_->rotation().x += sensitivity_ * delta_move_y * delta_time;

	ptr_ctrl_transform_->rotation().y += sensitivity_ * delta_move_x * delta_time;
}

void CPlayerController::ControlZoom(float delta_time)
{
	if (ptr_ctrl_unit_->weapon() == CSpaceMarin::Weapon::ChainSword) zoom_ = false;

	constexpr float min_fov_y = 35.f;
	constexpr float max_fov_y = 60.f;
	constexpr float zoom_speed = (max_fov_y - min_fov_y) * 2.f;

	if (Engine::Input()->GetKeyDown(KEY::MBUTTON))
	{
		zoom_ = !zoom_;
		ptr_ctrl_unit_->set_zoom(zoom_);
	}

	if (true == zoom_)
		fov_y = max(min_fov_y, fov_y -= zoom_speed * delta_time);
	else
		fov_y = min(max_fov_y, fov_y += zoom_speed * delta_time);
		
	if (pre_fov_y != fov_y)
	{
		ptr_ctrl_camera_->SetProjection(D3DXToRadian(fov_y), float(kWinCx) / kWinCy, 0.2f, 1000.f);
		pre_fov_y = fov_y;
	}
}

void CPlayerController::ControlAttack(float delta_time)
{
	if (Engine::Input()->GetKey(KEY::LBUTTON))
	{
		// TODO: 장비에 따른 공격 방법 변경.
		if (attack_delay_ <= 0.f)
		{
			ptr_ctrl_unit_->set_fire(true);
			Vector3 ray_pos, ray_dir;
			ComputeShootRay(ray_pos, ray_dir);
			ptr_ctrl_unit_->SetRay(ray_pos, ray_dir);
			attack_delay_ = 0.3f;
		}
		else
			attack_delay_ -= delta_time;
	}

	if (Engine::Input()->GetKeyDown(KEY::G))
		ptr_ctrl_unit_->set_toss();

	if (Engine::Input()->GetKeyDown(KEY::R))
		ptr_ctrl_unit_->set_reload();
}

void CPlayerController::ControlEvade(float delta_time)
{
	if (Engine::Input()->GetKeyDown(KEY::SPACE))
	{
		ptr_ctrl_unit_->set_evade(true);
		if (ptr_ctrl_transform_->move_dir().x == 0.f && ptr_ctrl_transform_->move_dir().z == 0.f)
		{
			ptr_ctrl_unit_->set_move_dir(CSpaceMarin::MoveDirection::Right);
			ptr_ctrl_unit_->set_evade_dir(ptr_ctrl_transform_->Right().Normalize());
		}
		else
			ptr_ctrl_unit_->set_evade_dir(ptr_ctrl_transform_->move_dir().Normalize());
	}
}

void CPlayerController::ControlExecution()
{
	if (ptr_enemy_observer_->boss_hp() == 0)
	{
		Vector3 target_pos = ptr_enemy_observer_->boss_pos();
		const float square_target_dist = (target_pos - ptr_ctrl_transform_->position()).Magnitude();
		const float square_dist = 3.f * 3.f;

		if (square_target_dist < square_dist && Engine::Input()->GetKeyDown(KEY::E))
		{
			ptr_ctrl_unit_->set_execution();
			ptr_ctrl_unit_->set_execution_target(target_pos);
		}
	}
}

void CPlayerController::ChangeWeapon()
{
	if (Engine::Input()->GetKeyDown(KEY::Q))
	{
		if (zoom_ == true)
			zoom_ = false;
		ptr_ctrl_unit_->set_weapon();
	}
}

void CPlayerController::ComputeShootRay(Vector3& ray_pos, Vector3& ray_dir)
{
	LPDIRECT3DDEVICE9 ptr_device = Engine::GraphicDevice()->GetDevice();
	POINT mouse_pos;
	Engine::Input()->GetMousePos(g_hwnd, &mouse_pos);

	Matrix mat_proj, inv_mat_view;
	ptr_device->GetTransform(D3DTS_PROJECTION, &mat_proj);

	Vector3 temp;
	temp.x = (float(mouse_pos.x) / (kWinCx >> 1) - 1.f) / mat_proj._11;
	temp.y = (float(-mouse_pos.y) / (kWinCy >> 1) + 1.f) / mat_proj._22;
	temp.z = 1.f;

	ray_pos = Vector3();
	ray_dir = (temp - ray_pos).Normalize();
	
	ptr_device->GetTransform(D3DTS_VIEW, &inv_mat_view);
	D3DXMatrixInverse(&inv_mat_view, nullptr, &inv_mat_view);
	D3DXVec3TransformCoord(&ray_pos, &ray_pos, &inv_mat_view);
	D3DXVec3TransformNormal(&ray_dir, &ray_dir, &inv_mat_view);	
	
	ptr_ctrl_unit_->set_fire(true);
}
