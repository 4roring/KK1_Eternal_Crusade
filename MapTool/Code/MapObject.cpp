#include "stdafx.h"
#include "MapObject.h"

#include "Transform.h"
#include "StaticMesh.h"
#include "Shader.h"

CMapObject::CMapObject(LPDIRECT3DDEVICE9 ptr_device)
	: Engine::CGameObject(ptr_device)
{
}

CMapObject::~CMapObject()
{
	Release();
}

HRESULT CMapObject::Initialize(const std::wstring& mesh_key)
{
	HRESULT hr = CGameObject::Initialize();
	assert(!FAILED(hr) && "CGameObject::Initialize call failed in MapObject");

	AddComponent(mesh_key);
	assert(!FAILED(hr) && "AddComponent call failed in MapObject");

	ptr_transform_->scale() = Vector3(0.01f, 0.01f, 0.01f);

	return S_OK;
}

void CMapObject::Update(float delta_time)
{
	Engine::GameManager()->AddRenderLayer(Engine::RENDERLAYER::LAYER_NONEALPHA, this);

	Engine::CGameObject::Update(delta_time);
}

void CMapObject::Render()
{
	LPD3DXEFFECT ptr_effect = ptr_shader_->GetEffectHandle();

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
}

CMapObject * CMapObject::Create(LPDIRECT3DDEVICE9 ptr_device, const std::wstring& mesh_key)
{
	CMapObject* ptr_obj = new CMapObject(ptr_device);
	if (FAILED(ptr_obj->Initialize(mesh_key)))
	{
		Safe_Delete(ptr_obj);
		assert(!"CMapObject Create Failed");
	}

	return ptr_obj;
}

HRESULT CMapObject::AddComponent(const std::wstring& mesh_key)
{
	HRESULT hr = E_FAIL;

	hr = Ready_Component(0, TEXT("mesh_key"), TEXT("StaticMesh"), ptr_mesh_);
	assert(!FAILED(hr) && "ScreenImage Buffer ReadyComponent Failed");
	hr = Ready_Component(0, TEXT("Component_Transform"), TEXT("Transform"), ptr_transform_);
	assert(!FAILED(hr) && "Tranform Component ReadyComponent Failed");
	hr = Ready_Component(0, TEXT("Shader_NormalMap"), TEXT("Shader"), ptr_shader_);
	assert(!FAILED(hr) && "ScreenImage Shader ReadyComponent Failed");

	return S_OK;
}

void CMapObject::Release()
{
}
