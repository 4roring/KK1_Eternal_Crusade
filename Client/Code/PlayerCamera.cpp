#include "stdafx.h"
#include "PlayerCamera.h"
#include "Transform.h"
#include "Frustum.h"
#include "Collider.h"

#include "Shader.h"

CPlayerCamera::CPlayerCamera(LPDIRECT3DDEVICE9 ptr_device)
	: Engine::CCamera(ptr_device)
{
	D3DXMatrixIdentity(&inv_mat_proj_);
	D3DXMatrixIdentity(&inv_mat_view_);
}

CPlayerCamera::~CPlayerCamera()
{
	Release();
}

void CPlayerCamera::EnableEvent()
{
	event_mode_ = true;
	next_eye_ = eye();
	next_at_ = at();
}

HRESULT CPlayerCamera::InitCamera(MAINTAINID stage_id)
{
	HRESULT hr = CCamera::InitCamera();
	assert(!FAILED(hr) && "Camera Init Failed");
	hr = CGameObject::Initialize();
	assert(!FAILED(hr) && "Camera GameObject Init Failed");

	hr = GetComponent(stage_id);

	ptr_frustum_ = CFrustum::Create();
	CollSystem()->SetFrustum(ptr_frustum_);
	SetProjection(D3DXToRadian(60.f), float(kWinCx) / kWinCy, 0.2f, 500.f);

	hr = AddComponent();

#ifdef _DEBUG
	D3DXCreateLine(ptr_device_, &ptr_line_);
	ptr_line_->SetAntialias(TRUE);
#endif

	current_cell_index_ = -1;

	Subject()->SetInverseCameraInfo(&inv_mat_proj_, &inv_mat_view_, &eye());
	EventManager()->SetEventCamera(this);

	return hr;
}

void CPlayerCamera::Update(float delta_time)
{
	if (true == event_mode_)
	{
		EventCamera(delta_time);
		return;
	}

	Vector3 player_right = ptr_player_transform_->Right().Normalize();
	Vector3 player_up = ptr_player_transform_->Up().Normalize();
	Vector3 player_forward = ptr_player_transform_->Forward().Normalize();
	ray_pos_ = ptr_player_transform_->position() + player_up * 2.f + player_right * 1.5f;
	ray_dir_ = player_forward * -1.f;

	Vector3 pre_eye = eye();
	next_eye_ = ray_pos_ + ray_dir_ * 3.5f;

	if (next_eye_.y < 0.f)
		next_eye_.y = 0.f;

	if (current_cell_index_ == -1)
	{
		next_at_ = next_eye_ + player_forward * 100.f;
		current_cell_index_ = Engine::GameManager()->FindCellIndex(next_eye_);
		SetView(next_eye_, next_at_);
	}
	else
	{
		Vector3 eye_dir = (next_eye_ - pre_eye);
		next_eye_ = pre_eye + (eye_dir * delta_time * 5.f);
		next_at_ = next_eye_ + player_forward * 100.f;

		Vector3 at_dir = (next_at_ - at());
		at() += at_dir * delta_time * 5.f;

		current_cell_index_ = Engine::GameManager()->MoveFromNavMesh(pre_eye, next_eye_, current_cell_index_);
		SetView(pre_eye, at());
	}

	//ptr_transform_->position() = CCamera::eye();
	//CGameObject::Update(0.f);
	//ptr_sphere_coll_->SetSphereCollider(2.f, Vector3());

	if (true == Subject()->camera_shaking())
	{
		Subject()->SetShakingInfo(shaking_time_, shaking_power_, shaking_dir_);
		CameraShaking(delta_time);
	}

	CCamera::Update(delta_time);
}

void CPlayerCamera::LateUpdate()
{
	//check_list_.clear();
	//CollSystem()->TriggerCheck(ptr_sphere_coll_, TAG_MAPOBJECT, check_list_);
	//if (false == check_list_.empty())
	//{
	//	float dist = 9999.f;

	//	for(auto& target : check_list_)
	//		target->RaycastCheck(ray_pos_, ray_dir_, &dist);

	//	if(dist < 5.5f)
	//		eye() = ray_pos_ + ray_dir_ * dist;
	//}

	//Engine::GameManager()->AddRenderLayer(Engine::RENDERLAYER::LAYER_NONEALPHA, this);
}

void CPlayerCamera::Render()
{
#ifdef _DEBUG
	//LPD3DXEFFECT ptr_effect = ptr_debug_shader_->GetEffectHandle();

	//Matrix mat_trans;
	//D3DXMatrixIdentity(&mat_trans);
	//mat_trans._41 = ptr_sphere_coll_->GetSpherePos().x;
	//mat_trans._42 = ptr_sphere_coll_->GetSpherePos().y;
	//mat_trans._43 = ptr_sphere_coll_->GetSpherePos().z;

	//ptr_effect->SetMatrix("g_mat_world", &mat_trans);

	//ptr_debug_shader_->BegineShader(1);
	//ptr_sphere_coll_->DebugRender();
	//ptr_debug_shader_->EndShader();


	//Vector3 line_[2] = { ray_pos_, ray_pos_ - ray_dir_ * 15.f  };

	//for (auto& point : line_)
	//{
	//	D3DXVec3TransformCoord(&point, &point, &mat_view());
	//	if (point.z < 0.f)
	//		point.z = 0.f;
	//	D3DXVec3TransformCoord(&point, &point, &mat_projection());
	//}

	//ptr_line_->SetWidth(5.f);
	//ptr_line_->Begin();

	//ptr_line_->DrawTransform(line_, 2, nullptr, D3DXCOLOR(1.f, 0.f, 1.f, 1.f));

	//ptr_line_->End();
#endif
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

void CPlayerCamera::SetView(const Vector3 & eye, const Vector3 & at)
{
	CCamera::SetView(eye, at);

	D3DXMatrixInverse(&inv_mat_view_, nullptr, &mat_view());
	ptr_frustum_->FrustumToWorld(inv_mat_view_);
}

void CPlayerCamera::SetProjection(float fov_y, float aspect, float z_near, float z_far)
{
	CCamera::SetProjection(fov_y, aspect, z_near, z_far);

	D3DXMatrixInverse(&inv_mat_proj_, nullptr, &mat_projection());
	ptr_frustum_->FrustumToView(inv_mat_proj_);
}

HRESULT CPlayerCamera::AddComponent()
{
	HRESULT hr = Ready_Component(MAINTAIN_STATIC, TEXT("Component_Transform"), TEXT("Transform"), ptr_transform_);
	assert(!FAILED(hr) && "ReadyComponent Failed of Camera Tranform Component ");

	ptr_sphere_coll_ = Engine::CCollider::Create(ptr_device_, this, ColliderType::Collider_Sphere);
	if (nullptr == ptr_sphere_coll_) return E_FAIL;

	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Shader_Default"), TEXT("Debug_Shader"), ptr_debug_shader_);

	return S_OK;
}

void CPlayerCamera::Release()
{
	Engine::Safe_Release_Delete(ptr_sphere_coll_);
	Engine::Safe_Delete(ptr_frustum_);
	Engine::Safe_Release(ptr_player_transform_);
	Engine::Safe_Release(ptr_boss_transform_);

#ifdef _DEBUG
	Engine::Safe_Release(ptr_line_);
#endif
}

HRESULT CPlayerCamera::GetComponent(MAINTAINID stage_id)
{
	ptr_player_transform_ = Engine::GameManager()->GetComponent<Engine::CTransform>(stage_id, TEXT("SpaceMarin"), TEXT("Transform"));
	if (nullptr == ptr_player_transform_) return E_FAIL;
	ptr_player_transform_->AddReferenceCount();

	ptr_boss_transform_ = Engine::GameManager()->GetComponent<Engine::CTransform>(stage_id, TEXT("Ork_WarBoss"), TEXT("Transform"));
	if (nullptr == ptr_boss_transform_) return E_FAIL;
	ptr_boss_transform_->AddReferenceCount();

	return S_OK;
}

void CPlayerCamera::CameraShaking(float delta_time)
{
	shaking_acc_time_ += delta_time;

	shaking_value_ = (shaking_value_ == shaking_power_) ? -shaking_power_ : shaking_power_;
	eye() += shaking_dir_ * shaking_value_;
	at() += shaking_dir_ * shaking_value_;

	if (shaking_acc_time_ >= shaking_time_)
	{
		Subject()->SetCameraShaking(false, 0.f, 0.f, Vector3());
		shaking_acc_time_ = 0.f;
	}
}

void CPlayerCamera::EventCamera(float delta_time)
{
	Vector3 boss_forward = ptr_boss_transform_->Forward().Normalize();
	Vector3 boss_up = ptr_boss_transform_->Up().Normalize();
	Vector3 target_eye = ptr_boss_transform_->position() + boss_forward * 3.5f + boss_up * 2.f;
	Vector3 target_at = ptr_boss_transform_->position();
	target_at.y += 1.f;

	event_time_ += delta_time * 0.5f;

	if(event_time_ <= 1.f)
		D3DXVec3Lerp(&eye(), &next_eye_, &target_eye, event_time_);
	else if(event_time_ >= 3.f)
		D3DXVec3Lerp(&eye(), &target_eye, &next_eye_, event_time_ - 3.f);

	if (event_time_ >= 4.f)
		event_mode_ = false;


	CCamera::Update(delta_time);
}
