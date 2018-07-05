#include "stdafx.h"
#include "MapObject.h"

#include "Transform.h"
#include "StaticMesh.h"
#include "Shader.h"

MapObject::MapObject(LPDIRECT3DDEVICE9 ptr_device)
	: Engine::CGameObject(ptr_device)
{
}

MapObject::~MapObject()
{
	Release();
}

const std::wstring & MapObject::mesh_key()
{
	return mesh_key_;
}

const std::wstring & MapObject::object_key()
{
	return object_key_;
}

void MapObject::set_object_key(const std::wstring & object_key)
{
	object_key_ = object_key;
}

HRESULT MapObject::Initialize(const std::wstring& mesh_key)
{
	HRESULT hr = CGameObject::Initialize();
	assert(!FAILED(hr) && "CGameObject::Initialize call failed in MapObject");

	AddComponent(mesh_key);
	assert(!FAILED(hr) && "AddComponent call failed in MapObject");
	mesh_key_ = mesh_key;
	ptr_transform_->scale() = Vector3(0.01f, 0.01f, 0.01f);

	return S_OK;
}

void MapObject::Update(float delta_time)
{
	Engine::CGameObject::Update(delta_time);
}

void MapObject::Render()
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
}

bool MapObject::RaycastToMesh(Vector3 & ray_pos, Vector3 & ray_dir, float * hit_dir)
{
	Matrix local_matrix;
	D3DXMatrixInverse(&local_matrix, nullptr, &ptr_transform_->mat_world());
	Vector3 local_ray_pos, local_ray_dir;
	D3DXVec3TransformCoord(&local_ray_pos, &ray_pos, &local_matrix);
	D3DXVec3TransformNormal(&local_ray_dir, &ray_dir, &local_matrix);

	return ptr_mesh_->RaycastToMesh(local_ray_pos, local_ray_dir, hit_dir);
}

MapObject * MapObject::Create(LPDIRECT3DDEVICE9 ptr_device, const std::wstring& mesh_key)
{
	MapObject* ptr_obj = new MapObject(ptr_device);
	if (FAILED(ptr_obj->Initialize(mesh_key)))
	{
		Safe_Delete(ptr_obj);
		assert(!"MapObject Create Failed");
	}

	return ptr_obj;
}

HRESULT MapObject::AddComponent(const std::wstring& mesh_key)
{
	HRESULT hr = E_FAIL;

	hr = Ready_Component(0, mesh_key, TEXT("StaticMesh"), ptr_mesh_);
	assert(!FAILED(hr) && "ScreenImage Buffer ReadyComponent Failed");
	hr = Ready_Component(0, TEXT("Component_Transform"), TEXT("Transform"), ptr_transform_);
	assert(!FAILED(hr) && "Tranform Component ReadyComponent Failed");
	hr = Ready_Component(0, TEXT("Shader_NormalMap"), TEXT("Shader"), ptr_shader_);
	assert(!FAILED(hr) && "ScreenImage Shader ReadyComponent Failed");

	return S_OK;
}

void MapObject::Release()
{
}
