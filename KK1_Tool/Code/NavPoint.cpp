#include "stdafx.h"
#include "NavPoint.h"

#include "Transform.h"
#include "Shader.h"
#include "Sphere.h"

NavPoint::NavPoint(LPDIRECT3DDEVICE9 ptr_device)
	: Engine::CGameObject(ptr_device)
{
}

NavPoint::~NavPoint()
{
	Safe_Delete(ptr_col_sphere_);
}

Vector3 & NavPoint::position()
{
	return ptr_transform_->position();
}

HRESULT NavPoint::Initialize()
{
	HRESULT hr = Engine::CGameObject::Initialize();
	assert(!FAILED(hr) && "CGameObject::Initialize call failed in NavPoint");

	hr = AddComponent();
	assert(!FAILED(hr) && "AddComponent call failed in NavPoint");

	++reference_count_;

	return S_OK;
}

void NavPoint::Update(float delta_time)
{
	Engine::CGameObject::Update(delta_time);
}

void NavPoint::Render()
{
	LPD3DXEFFECT ptr_effect = ptr_shader_->GetEffectHandle();
	if (nullptr == ptr_effect) return;

	Matrix mat_view, mat_proj;
	ptr_device_->GetTransform(D3DTS_VIEW, &mat_view);
	ptr_device_->GetTransform(D3DTS_PROJECTION, &mat_proj);

	ptr_effect->SetMatrix("g_mat_world", &ptr_transform_->mat_world());
	ptr_effect->SetMatrix("g_mat_view", &mat_view);
	ptr_effect->SetMatrix("g_mat_projection", &mat_proj);

	ptr_shader_->BegineShader(1);
	ptr_col_sphere_->Render();
	ptr_shader_->EndShader();
}

uint32 NavPoint::Release()
{
	if (reference_count_ == 0)
		return 0;

	return --reference_count_;
}

NavPoint * NavPoint::Create(LPDIRECT3DDEVICE9 ptr_device)
{
	NavPoint* ptr_obj = new NavPoint(ptr_device);
	if (FAILED(ptr_obj->Initialize()))
	{
		Safe_Delete(ptr_obj);
		assert(!"NavPoint Create Failed");
	}
	return ptr_obj;
}

NavPoint * NavPoint::ClonePoint()
{
	++reference_count_;
	return this;
}

void NavPoint::SetPosition(const Vector3 & pos)
{
	ptr_transform_->position() = pos;
}

bool NavPoint::RaycastToSphere(Vector3 ray_pos, Vector3 ray_dir)
{
	Matrix mat_local;
	D3DXMatrixIdentity(&mat_local);
	mat_local._41 = -ptr_transform_->position().x;
	mat_local._42 = -ptr_transform_->position().y;
	mat_local._43 = -ptr_transform_->position().z;

	D3DXVec3TransformCoord(&ray_pos, &ray_pos, &mat_local);
	D3DXVec3TransformNormal(&ray_dir, &ray_dir, &mat_local);

	BOOL is_pick = FALSE;
	D3DXIntersect(ptr_col_sphere_->ptr_sphere_mesh(), &ray_pos, &ray_dir, &is_pick
		, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
	
	if (TRUE == is_pick) return true;
	return false;
}

HRESULT NavPoint::AddComponent()
{
	HRESULT hr = E_FAIL;

	hr = Ready_Component(0, TEXT("Component_Transform"), TEXT("Transform"), ptr_transform_);
	assert(!FAILED(hr) && "Tranform Component ReadyComponent Failed");
	hr = Ready_Component(0, TEXT("Shader_Default"), TEXT("Shader"), ptr_shader_);
	assert(!FAILED(hr) && "ScreenImage Shader ReadyComponent Failed");

	ptr_col_sphere_ = Engine::CSphere::Create(ptr_device_, 0.35f, 5, 5);
	if (nullptr == ptr_col_sphere_)
		return E_FAIL;

	return S_OK;
}
