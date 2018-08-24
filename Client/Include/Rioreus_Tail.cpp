#include "stdafx.h"
#include "Rioreus_Tail.h"

#include "Transform.h"
#include "Shader.h"
#include "StaticMesh.h"

CRioreus_Tail::CRioreus_Tail(LPDIRECT3DDEVICE9 ptr_device)
	: Engine::CGameObject(ptr_device)
{
}

CRioreus_Tail::~CRioreus_Tail()
{
	Release();
}

HRESULT CRioreus_Tail::Initialize(const Vector3 & start_pos, const Vector3 & start_rot)
{
	HRESULT hr = Engine::CGameObject::Initialize();
	assert(!FAILED(hr) && "Explosion Initialize Falied");
	hr = AddComponent();
	assert(!FAILED(hr) && "Explosion AddComponent Falied");

	ptr_transform_->position() = start_pos;
	ptr_transform_->rotation() = start_rot;
	ptr_transform_->scale() = Vector3(0.01f, 0.01f, 0.01f);

	CGameObject::Update(0.f);
	forward_vector_ = ptr_transform_->Forward().Normalize();

	return S_OK;
}

void CRioreus_Tail::Update(float delta_time)
{
	CGameObject::Update(delta_time);

	if (ptr_transform_->position().y > 0.f)
	{
		ptr_transform_->position() -= forward_vector_ * 10.f * delta_time;
		ptr_transform_->rotation().x -= 3.14f * delta_time;
		ptr_transform_->position().y = max(ptr_transform_->position().y - 3.f * delta_time, 0.f);
		if (ptr_transform_->position().y == 0.f) ptr_transform_->rotation().x = 0.f;
	}
}

void CRioreus_Tail::LateUpdate()
{
	Engine::GameManager()->AddRenderLayer(Engine::LAYER_NONEALPHA, this);
}

void CRioreus_Tail::Render()
{
	LPD3DXEFFECT ptr_effect = ptr_shader_->GetEffectHandle();
	if (nullptr == ptr_effect) return;

	Matrix mat_view, mat_proj;
	ptr_device_->GetTransform(D3DTS_VIEW, &mat_view);
	ptr_device_->GetTransform(D3DTS_PROJECTION, &mat_proj);

	ptr_effect->SetMatrix("g_mat_world", &ptr_transform_->mat_world());
	ptr_effect->SetMatrix("g_mat_view", &mat_view);
	ptr_effect->SetMatrix("g_mat_projection", &mat_proj);

	ptr_mesh_->RenderMesh(ptr_effect, 2);
}

CRioreus_Tail * CRioreus_Tail::Create(LPDIRECT3DDEVICE9 ptr_device, const Vector3 & start_pos, const Vector3 & start_rot)
{
	CRioreus_Tail* ptr_obj = new CRioreus_Tail(ptr_device);
	if (FAILED(ptr_obj->Initialize(start_pos, start_rot)))
	{
		Safe_Delete(ptr_obj);
		assert(!"Gun_Phobos Create Failed");
	}
	return ptr_obj;
}

HRESULT CRioreus_Tail::AddComponent()
{
	HRESULT hr = E_FAIL;

	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Component_Transform"), TEXT("Transform"), ptr_transform_);
	assert(hr == S_OK && "Tranform Component ReadyComponent Failed");
	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Shader_NormalMap"), TEXT("Shader"), ptr_shader_);
	assert(hr == S_OK && "CFireBall Shader_DynamicMesh ReadyComponent Failed");
	hr = Ready_Component(MAINTAIN_STAGE, TEXT("RioreusTail_Mesh"), TEXT("Mesh"), ptr_mesh_);
	assert(hr == S_OK && "Mesh ReadyComponent Failed");
	
	return S_OK;
}

void CRioreus_Tail::Release()
{
}
