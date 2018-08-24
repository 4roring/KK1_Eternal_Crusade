#include "stdafx.h"
#include "EyeTrail.h"

#include "Transform.h"
#include "Shader.h"
#include "TrailColor.h"

CEyeTrail::CEyeTrail(LPDIRECT3DDEVICE9 ptr_device)
	: Engine::CGameObject(ptr_device)
{
	D3DXMatrixIdentity(&trail_world_);
}

CEyeTrail::~CEyeTrail()
{
	Release();
}

HRESULT CEyeTrail::Initialize(const Matrix * ptr_target_local, const Matrix * ptr_target_world, bool is_left)
{
	HRESULT hr = Engine::CGameObject::Initialize();
	assert(!FAILED(hr) && "Explosion Initialize Falied");
	hr = AddComponent();
	assert(!FAILED(hr) && "Explosion AddComponent Falied");

	ptr_target_local_ = ptr_target_local;
	ptr_target_world_ = ptr_target_world;
	is_left_ = is_left;

	return S_OK;
}

void CEyeTrail::Update(float delta_time)
{
	Engine::CGameObject::Update(delta_time);

	Matrix target_local = *ptr_target_local_;
	Vector3 forward_vector = (*(Vector3*)&target_local.m[0][0]).Normalize();
	Vector3 right_vector = (*(Vector3*)&target_local.m[2][0]).Normalize();

	if (true == is_left_)
		right_vector *= -1.f;

	*(Vector3*)&target_local.m[3][0] += forward_vector * -0.7f + right_vector * 0.3f;
	trail_world_ = target_local * *ptr_target_world_;
	PointMemorize(delta_time);
}

void CEyeTrail::LateUpdate()
{
	Engine::GameManager()->AddRenderLayer(Engine::LAYER_ALPHA, this);
}

void CEyeTrail::Render()
{
	if (point_list_.size() < 2) return;

	auto iter = point_list_.begin();
	auto iter_end = point_list_.end();

	Vector3* ptr_point = new Vector3[int(point_list_.size() * 0.5f)];
	Vector3* ptr_dir = new Vector3[int(point_list_.size() * 0.5f)];

	int index = 0;
	for (; iter != iter_end; ++iter)
	{
		ptr_point[index] = (*iter);
		++iter;
		ptr_dir[index] = (*iter);
		++index;
	}

	ptr_trail_->SetTrail(ptr_point, ptr_dir, length_, D3DXCOLOR(0.8f, 0.1f, 0.f, 1.f), point_list_.size());

	LPD3DXEFFECT ptr_effect = ptr_shader_->GetEffectHandle();
	if (nullptr == ptr_effect) return;

	Matrix mat_view, mat_proj;
	ptr_device_->GetTransform(D3DTS_VIEW, &mat_view);
	ptr_device_->GetTransform(D3DTS_PROJECTION, &mat_proj);

	ptr_effect->SetMatrix("g_mat_world", &ptr_transform_->mat_world());
	ptr_effect->SetMatrix("g_mat_view", &mat_view);
	ptr_effect->SetMatrix("g_mat_projection", &mat_proj);

	ptr_shader_->BegineShader(4);
	ptr_trail_->Render();
	ptr_shader_->EndShader();

	Engine::Safe_Delete_Array(ptr_point);
	Engine::Safe_Delete_Array(ptr_dir);
}

CEyeTrail * CEyeTrail::Create(LPDIRECT3DDEVICE9 ptr_device, const Matrix * ptr_target_local, const Matrix * ptr_target_world, bool is_left)
{
	CEyeTrail* ptr_obj = new CEyeTrail(ptr_device);
	if (FAILED(ptr_obj->Initialize(ptr_target_local, ptr_target_world, is_left)))
	{
		Safe_Delete(ptr_obj);
		assert(!"Explosion Create Failed");
	}

	return ptr_obj;
}

HRESULT CEyeTrail::AddComponent()
{
	HRESULT hr = E_FAIL;

	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Component_Transform"), TEXT("Transform"), ptr_transform_);
	assert(!FAILED(hr) && "ReadyComponent Failed of GranadeTrail Tranform Component ");
	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Shader_Mesh_Effect"), TEXT("Shader"), ptr_shader_);
	assert(!FAILED(hr) && "ReadyComponent Failed of Bomb Shader Component");
	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Buffer_TrailColor"), TEXT("Buffer"), ptr_trail_);
	assert(!FAILED(hr) && "ReadyComponent Failed of GranadeTrail TrailColor Component ");

	return S_OK;
}

void CEyeTrail::Release()
{
}

void CEyeTrail::PointMemorize(float delta_time)
{
	while (point_list_.size() >= 30)
	{
		point_list_.pop_front();
		point_list_.pop_front();
	}

	acc_time_ += delta_time;

	if (acc_time_ > 0.002f)
	{
		Vector3 point[2] = { Vector3(0.f, 0.f, 0.f), Vector3(0.f, 5.f, 0.f) };
		Vector3 pos, dir;
		dir = point[1] - point[0];
		length_ = dir.Length();
		dir = dir.Normalize();

		pos = dir * length_;
		D3DXVec3TransformCoord(&pos, &pos, &trail_world_);
		point_list_.emplace_back(pos);
		D3DXVec3TransformNormal(&dir, &dir, &trail_world_);
		point_list_.emplace_back(dir);

		acc_time_ = 0.f;
	}
}
