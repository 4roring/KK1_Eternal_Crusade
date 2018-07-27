#include "stdafx.h"
#include "LevelObject.h"

#include "Transform.h"
#include "Shader.h"
#include "StaticMesh.h"
#include "Collider.h"

CLevelObject::CLevelObject(LPDIRECT3DDEVICE9 ptr_device)
	: Engine::CGameObject(ptr_device)
{
}

CLevelObject::~CLevelObject()
{
	Release();
}

HRESULT CLevelObject::Initialize(const std::wstring & mesh_key)
{
	HRESULT hr = Engine::CGameObject::Initialize();
	assert(!FAILED(hr) && "LevelObject Initialize Falied");
	hr = AddComponent(mesh_key);
	assert(!FAILED(hr) && "LevelObject AddComponent Falied");

	return hr;
}

void CLevelObject::LateInit()
{
	Engine::CGameObject::Update(0.f);

	Vector3 min, max;
	ptr_mesh_->GetMinMax(min, max);
	ptr_box_coll_->SetAABBCollider(min, max);
}

void CLevelObject::Update(float delta_time)
{
	Engine::CGameObject::Update(delta_time);
}

void CLevelObject::LateUpdate()
{
	Engine::GameManager()->AddRenderLayer(Engine::RENDERLAYER::LAYER_NONEALPHA, this);
}

void CLevelObject::Render()
{
	LPD3DXEFFECT ptr_effect = ptr_shader_->GetEffectHandle();
	if (nullptr == ptr_effect) return;

	Matrix mat_view, mat_proj;
	ptr_device_->GetTransform(D3DTS_VIEW, &mat_view);
	ptr_device_->GetTransform(D3DTS_PROJECTION, &mat_proj);

	ptr_effect->SetMatrix("g_mat_world", &ptr_transform_->mat_world());
	ptr_effect->SetMatrix("g_mat_view", &mat_view);
	ptr_effect->SetMatrix("g_mat_projection", &mat_proj);

	ptr_effect->SetVector("g_light_diffuse", &Vector4(1.f, 1.f, 1.f, 1.f));
	ptr_effect->SetVector("g_light_ambient", &Vector4(1.f, 1.f, 1.f, 1.f));
	ptr_effect->SetVector("g_light_dir", &Vector4(0.f, -1.f, 1.f, 0.f));

	ptr_mesh_->RenderMesh(ptr_effect);

#ifdef _DEBUG
	ptr_effect = ptr_debug_shader_->GetEffectHandle();

	Matrix mat_identity;
	D3DXMatrixIdentity(&mat_identity);

	ptr_effect->SetMatrix("g_mat_world", &ptr_transform_->mat_world());
	ptr_effect->SetMatrix("g_mat_view", &mat_view);
	ptr_effect->SetMatrix("g_mat_projection", &mat_proj);

	ptr_debug_shader_->BegineShader(1);
	ptr_box_coll_->DebugRender();
	ptr_debug_shader_->EndShader();
#endif
}

CLevelObject * CLevelObject::Create(LPDIRECT3DDEVICE9 ptr_device, const std::wstring & mesh_key)
{
	CLevelObject* ptr_obj = new CLevelObject(ptr_device);
	if (FAILED(ptr_obj->Initialize(mesh_key)))
	{
		Safe_Delete(ptr_obj);
		assert(!"LevelObject Create Failed");
	}

	return ptr_obj;
}

HRESULT CLevelObject::AddComponent(const std::wstring & mesh_key)
{
	HRESULT hr = E_FAIL;

	hr = Ready_Component(MAINTAIN_STAGE, mesh_key, TEXT("StaticMesh"), ptr_mesh_);
	assert(!FAILED(hr) && "ReadyComponent Failed of LevelObject StaticMesh Component");
	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Component_Transform"), TEXT("Transform"), ptr_transform_);
	assert(!FAILED(hr) && "ReadyComponent Failed of LevelObject Tranform Component ");
	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Shader_NormalMap"), TEXT("Shader"), ptr_shader_);
	assert(!FAILED(hr) && "ReadyComponent Failed of LevelObject Shader Component");

	ptr_box_coll_ = Engine::CCollider::Create(ptr_device_, this, ColliderType::Collider_AABB);

#ifdef _DEBUG
	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Shader_Default"), TEXT("Debug_Shader"), ptr_debug_shader_);
#endif

	return S_OK;
}

void CLevelObject::Release()
{
	Safe_Delete(ptr_box_coll_);
}