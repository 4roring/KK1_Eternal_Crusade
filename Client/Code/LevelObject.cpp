#include "stdafx.h"
#include "LevelObject.h"

#include "Transform.h"
#include "Shader.h"
#include "StaticMesh.h"

#include <filesystem>

namespace FILESYSTEM = std::experimental::filesystem::v1;

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

void CLevelObject::Update(float delta_time)
{
	Engine::CGameObject::Update(delta_time);

	Engine::GameManager()->AddRenderLayer(Engine::RENDERLAYER::LAYER_NONEALPHA, this);
}

void CLevelObject::LateUpdate()
{
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

	return S_OK;
}

void CLevelObject::Release()
{
}

HRESULT CLevelObject::LoadMesh(const std::wstring & mesh_key, MAINTAINID stage_id)
{
	const std::wstring path = TEXT("..\\bin\\Resources\\Mesh\\MapData");

	return FindMesh(mesh_key, path, stage_id);
}

HRESULT CLevelObject::FindMesh(const std::wstring& mesh_key, const std::wstring & path, MAINTAINID stage_id)
{
	FILESYSTEM::path find_file(path);

	if (false == FILESYSTEM::is_directory(find_file))
		return E_FAIL;

	for (const auto& directory : FILESYSTEM::directory_iterator(find_file))
	{
		if (true == FILESYSTEM::is_directory(directory.status()))
		{
			if (!FAILED(FindMesh(mesh_key, directory.path(), stage_id)))
				return S_OK;
		}

		if (FILESYSTEM::is_regular_file(directory.status()) && directory.path().extension() == TEXT(".X"))
		{
			if (0 == mesh_key.compare(directory.path().stem()))
			{
				std::wstring file_path = directory.path().c_str();
				const std::wstring file_name = mesh_key + TEXT(".X");
				auto iter = file_path.rfind(TEXT("\\"));
				file_path = file_path.substr(0, ++iter);

				ptr_mesh_ = Engine::CStaticMesh::Create(ptr_device_
					, file_path.c_str(), file_name.c_str(), stage_id);
				ptr_mesh_->AddReferenceCount();

				Engine::GameManager()->Add_Prototype(stage_id, mesh_key, ptr_mesh_);
			}
		}
	}
	return E_FAIL;
}
