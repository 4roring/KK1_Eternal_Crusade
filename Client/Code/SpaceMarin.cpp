#include "stdafx.h"
#include "SpaceMarin.h"
#include "Scene.h"

#include "Transform.h"
#include "Shader.h"
#include "DynamicMesh.h"
#include "AnimController.h"

#include "PlayerController.h"
#include "Gun_Phobos.h"

CSpaceMarin::CSpaceMarin(LPDIRECT3DDEVICE9 ptr_device)
	: Engine::CGameObject(ptr_device)
{
}

CSpaceMarin::~CSpaceMarin()
{
	Release();
}

Engine::CTransform * CSpaceMarin::ptr_lower_transform()
{
	return ptr_lower_transform_;
}

Vector3 & CSpaceMarin::GetFirePos() const
{
	return ptr_gun_->GetFirePos();
}

int CSpaceMarin::current_cell_index() const
{
	return current_cell_index_;
}

void CSpaceMarin::set_move_dir(MoveDirection move_dir)
{
	next_move_dir_ = move_dir;
}

void CSpaceMarin::set_zoom(bool is_zoom)
{
	zoom_ = is_zoom;
}

void CSpaceMarin::set_fire(bool is_fire)
{
	fire_ = is_fire;
}

void CSpaceMarin::set_fire_range_pos(const Vector3 & fire_range_pos)
{
	fire_range_pos_ = fire_range_pos;
}

void CSpaceMarin::SetRay(const Vector3 & ray_pos, const Vector3 & ray_dir)
{
	ray_pos_ = ray_pos;
	ray_dir_ = ray_dir;
}

HRESULT CSpaceMarin::Initialize(int ctrl_id)
{
	HRESULT hr = CGameObject::Initialize();
	assert(!FAILED(hr) && "CGameObject::Initialize call failed in SpaceMarin");

	control_id_ = ctrl_id;
	hr = AddComponent();
	assert(!FAILED(hr) && "AddComponent call failed in SpaceMarin");

	ptr_lower_anim_ctrl_->SetAnimationTrack("Idle");
	ptr_upper_anim_ctrl_->SetAnimationTrack("Idle");

	ptr_upper_start_frame_ = ptr_mesh_->FindFrame("joint_TorsoA_01");
	ptr_lower_transform_->scale() = ptr_transform_->scale();

	current_cell_index_ = -1;

	next_lower_state_ = LowerState::Idle;

	return S_OK;
}

void CSpaceMarin::LateInit()
{
	HRESULT hr = AddWeapon();
	assert(!FAILED(hr) && "SpaceMarin Add Weapon Failed");

	if (current_cell_index_ == -1)
		current_cell_index_ = Engine::GameManager()->FindCellIndex(ptr_transform_->position());

#ifdef _DEBUG
	D3DXCreateLine(ptr_device_, &ptr_line_);
	ptr_line_->SetAntialias(TRUE);
#endif
}

void CSpaceMarin::Update(float delta_time)
{
	delta_time_ = delta_time;

	//if (ptr_upper_anim_ctrl_->GetPeriod() <= ptr_upper_anim_ctrl_->GetTrackPosition())
	//	ptr_upper_anim_ctrl_->SetTrackPosition(0.0);

	//if (ptr_lower_anim_ctrl_->GetPeriod() <= ptr_lower_anim_ctrl_->GetTrackPosition())
	//	ptr_lower_anim_ctrl_->SetTrackPosition(0.0);

	ptr_lower_transform_->position() = ptr_transform_->position();
	ptr_lower_transform_->rotation() = Vector3(0.f, ptr_transform_->rotation().y, 0.f);

	UpdateState(delta_time);
	Engine::CGameObject::Update(delta_time);
}

void CSpaceMarin::LateUpdate()
{
	Engine::GameManager()->AddRenderLayer(Engine::RENDERLAYER::LAYER_NONEALPHA, this);
	UpdateLowerAnimState();
	UpdateUpperAnimState();
	Fire();
}

void CSpaceMarin::Render()
{
	LPD3DXEFFECT ptr_effect = ptr_shader_->GetEffectHandle();

	Matrix mat_view, mat_proj;
	ptr_device_->GetTransform(D3DTS_VIEW, &mat_view);
	ptr_device_->GetTransform(D3DTS_PROJECTION, &mat_proj);

	ptr_effect->SetMatrix("g_mat_world", &ptr_lower_transform_->mat_world());
	ptr_effect->SetMatrix("g_mat_view", &mat_view);
	ptr_effect->SetMatrix("g_mat_projection", &mat_proj);

	Matrix mat_upper_rot_x;
	D3DXMatrixRotationAxis(&mat_upper_rot_x, &Vector3(-1.f, 0.f, 0.f), ptr_transform_->rotation().x);

	ptr_mesh_->FrameMove(delta_time_, ptr_lower_anim_ctrl_, ptr_upper_start_frame_, &mat_upper_rot_x);
	ptr_mesh_->FrameMove(delta_time_, ptr_upper_anim_ctrl_, ptr_upper_start_frame_);
	ptr_gun_->SetHandMatrix(ptr_hand_matrix_);
	ptr_mesh_->RenderMesh(ptr_effect);

#ifdef _DEBUG
	Vector3 line_[2] = { ptr_gun_->GetFirePos(), fire_range_pos_ };

	for (auto& point : line_)
	{
		D3DXVec3TransformCoord(&point, &point, &mat_view);
		if (point.z < 0.f)
			point.z = 0.f;
		D3DXVec3TransformCoord(&point, &point, &mat_proj);
	}

	ptr_line_->SetWidth(2.f);
	ptr_line_->Begin();

	ptr_line_->DrawTransform(line_, 2, nullptr, D3DXCOLOR(1.f, 0.f, 0.f, 1.f));

	ptr_line_->End();
#endif
}

CSpaceMarin * CSpaceMarin::Create(LPDIRECT3DDEVICE9 ptr_device, int ctrl_id)
{
	CSpaceMarin* ptr_obj = new CSpaceMarin(ptr_device);
	if (FAILED(ptr_obj->Initialize(ctrl_id)))
	{
		Safe_Delete(ptr_obj);
		assert(!"SpaceMarin Create Failed");
	}

	return ptr_obj;
}

HRESULT CSpaceMarin::AddComponent()
{
	HRESULT hr = E_FAIL;

	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Component_Transform"), TEXT("Transform"), ptr_transform_);
	assert(hr == S_OK && "Tranform Component ReadyComponent Failed");
	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Component_Transform"), TEXT("Lower_Transform"), ptr_lower_transform_);
	assert(hr == S_OK && "Tranform Component ReadyComponent Failed");

	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Shader_DynamicMesh"), TEXT("Shader"), ptr_shader_);
	assert(hr == S_OK && "ScreenImage Shader ReadyComponent Failed");
	hr = Ready_Component(MAINTAIN_STAGE, TEXT("SpaceMarin_Mesh"), TEXT("Mesh"), ptr_mesh_);
	assert(hr == S_OK && "Mesh ReadyComponent Failed");
	ptr_lower_anim_ctrl_ = ptr_mesh_->CloneAnimController();
	ptr_upper_anim_ctrl_ = ptr_mesh_->CloneAnimController();

	if (0 == control_id_)
	{
		ptr_ctrl_ = CPlayerController::Create(this, 5.f, 1.f);
		CGameObject::AddComponent(TEXT("0.PlayerController"), ptr_ctrl_);

		if (nullptr != ptr_ctrl_)
			Engine::GameManager()->Add_Prototype(MAINTAIN_STAGE, TEXT("Component_PlayerController"), ptr_ctrl_->CloneComponent());
	}
	else
	{
		// TODO: ctrl_id 1일때 AIController 추가하기.
	}

	return S_OK;
}

HRESULT CSpaceMarin::AddWeapon()
{
	ptr_gun_ = CGun_Phobos::Create(ptr_device_);
	ptr_hand_matrix_ = ptr_mesh_->FindFrameMatrix("joint_HandRT_01");
	if (nullptr == ptr_hand_matrix_) return E_FAIL;
	ptr_gun_->SetParentMatrix(&ptr_lower_transform_->mat_world());
	
	TCHAR object_key[128] = TEXT("");
	wsprintf(object_key, TEXT("Gun_Phobos_%d"), control_id_);
	Engine::GameManager()->GetCurrentScene()->AddObject(MAINTAIN_STAGE, object_key, ptr_gun_);
	return S_OK;
}

void CSpaceMarin::Release()
{
	Engine::Safe_Delete(ptr_lower_anim_ctrl_);
	Engine::Safe_Delete(ptr_upper_anim_ctrl_);

#ifdef _DEBUG
	Engine::Safe_Release(ptr_line_);
#endif
}

void CSpaceMarin::UpdateState(float delta_time)
{
	switch (pre_lower_state_)
	{
	case LowerState::Idle:
		if (ptr_transform_->move_dir().x != 0.f || ptr_transform_->move_dir().z != 0.f)
			next_lower_state_ = LowerState::Run;
		break;
	case LowerState::Run:
		Run();
		if (ptr_transform_->move_dir().x == 0.f && ptr_transform_->move_dir().z == 0.f)
		{
			next_lower_state_ = LowerState::Idle;
			next_upper_state_ = UpperState::Idle;
		}
		break;
	case LowerState::Evade:
		// TODO: 스위치 케이스에 따른 방향으로 구르기(move_dir은 별도의 상수로 고정한다.)

		break;
	}
}

void CSpaceMarin::UpdateLowerAnimState()
{
	if (pre_lower_state_ != next_lower_state_)
	{
		switch (next_lower_state_)
		{
		case LowerState::Idle:
			ptr_lower_anim_ctrl_->SetAnimationTrack("Idle");
			break;
		case LowerState::Evade:
			ptr_lower_anim_ctrl_->SetAnimationTrack("Idle");
			break;
		}
		ptr_lower_anim_ctrl_->SetTrackPosition(0.0);
		pre_lower_state_ = next_lower_state_;
	}
}

void CSpaceMarin::UpdateUpperAnimState()
{
	if (pre_upper_state_ != next_upper_state_)
	{
		switch (next_upper_state_)
		{
		case UpperState::Idle:
			ptr_upper_anim_ctrl_->SetAnimationTrack("Idle");
			break;
		case UpperState::Shoot:
			ptr_upper_anim_ctrl_->SetAnimationTrack("Idle");
			break;
		case UpperState::Run:
			ptr_upper_anim_ctrl_->SetAnimationTrack("Run_Forward");
			break;
		case UpperState::Run_Aiming:
			ptr_upper_anim_ctrl_->SetAnimationTrack("Run_Shoot");
			break;
		}
		ptr_upper_anim_ctrl_->SetTrackPosition(0.0);
		pre_upper_state_ = next_upper_state_;
	}
}

void CSpaceMarin::Run()
{
	if (pre_move_dir_ != next_move_dir_)
	{
		switch (next_move_dir_)
		{
		case MoveDirection::Forward:
			ptr_lower_anim_ctrl_->SetAnimationTrack("Run_Forward");
			break;
		case MoveDirection::Backward:
			ptr_lower_anim_ctrl_->SetAnimationTrack("Run_Backward");
			break;
		case MoveDirection::Right:
			ptr_lower_anim_ctrl_->SetAnimationTrack("Run_Right");
			break;
		case MoveDirection::Left:
			ptr_lower_anim_ctrl_->SetAnimationTrack("Run_Left");
			break;
		}
		pre_move_dir_ = next_move_dir_;
	}

	if (false == zoom_ && false == fire_)
		next_upper_state_ = UpperState::Run;
	else
		next_upper_state_ = UpperState::Run_Aiming;

	current_cell_index_ = Engine::GameManager()->MoveFromNavMesh(ptr_transform_->position(), ptr_transform_->move_dir(), current_cell_index_, 0);
}

void CSpaceMarin::Fire()
{
	if (true == fire_)
	{
		switch (pre_upper_state_)
		{
		case UpperState::Run:
			next_upper_state_ = UpperState::Shoot;
			break;
		case UpperState::Idle:
		case UpperState::Shoot:
		case UpperState::Run_Aiming:
			// TODO: Bullet Shoot
			Engine::CGameObject* hit_obj = nullptr;
			float dist = 0.f;
			if (true == CollSystem()->RaycastCheck(ray_pos_, ray_dir_, &dist, hit_obj, TAG_ENEMY))
			{
				if (nullptr != hit_obj)
					hit_obj->ApplyDamage(50);
				fire_range_pos_ = ray_pos_ + ray_dir_ * dist;
			}
			else
				fire_range_pos_ = ray_pos_ + ray_dir_ * 30.f;
			fire_ = false;
			break;
		}
	}
}

void CSpaceMarin::SetConstantTable(LPD3DXEFFECT ptr_effect)
{
}
