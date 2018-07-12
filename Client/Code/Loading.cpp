#include "stdafx.h"
#include "Loading.h"
#include <process.h>

#include "Texture.h"
#include "CubeTexture.h"
#include "StaticMesh.h"
#include "DynamicMesh.h"
#include "Shader.h"

#include "SpaceMarin.h"
#include "PlayerCamera.h"

#include "Stage.h"
#include "LevelObject.h"
#include "Transform.h"

#include <filesystem>
namespace FILESYSTEM = std::experimental::filesystem::v1;

CLoading::CLoading(LOADINGID loading_id, Engine::CScene** pp_scene_)
	: loading_id_(loading_id), pp_next_scene_(pp_scene_)
{
}

CLoading::~CLoading()
{
}

const TCHAR * CLoading::loading_message()
{
	return loading_message_;
}

const bool CLoading::complete()
{
	return complete_;
}

HRESULT CLoading::InitLoading()
{
	ptr_device_ = Engine::GraphicDevice()->GetDevice();
	assert(nullptr != ptr_device_ && "Loading Object Device nullptr");

	thread_ = (HANDLE)_beginthreadex(nullptr, 0, LoadingFunction, this, 0, nullptr);
	assert(nullptr != thread_ && "Loading Thread Create Failed");
	return S_OK;
}

HRESULT CLoading::Stage_Loading()
{
	lstrcpy(loading_message_, TEXT("Texture Loading"));
	HRESULT hr = E_FAIL;

	// Shader
	lstrcpy(loading_message_, TEXT("Shader Complie and Loading"));
	hr = Engine::GameManager()->Add_Prototype(MAINTAIN_STATIC
		, TEXT("Shader_Mesh")
		, Engine::CShader::Create(ptr_device_
			, TEXT("../../Reference/Shader/MeshShader.hlsl")));
	assert(hr == S_OK && "Shader_Mesh Component Add Failed");

	hr = Engine::GameManager()->Add_Prototype(MAINTAIN_STATIC
		, TEXT("Shader_DynamicMesh")
		, Engine::CShader::Create(ptr_device_
			, TEXT("../../Reference/Shader/DynamicMeshShader.hlsl")));
	assert(hr == S_OK && "Shader_DynamicMesh Component Add Failed");

	hr = Engine::GameManager()->Add_Prototype(MAINTAIN_STATIC
		, TEXT("Shader_NormalMap")
		, Engine::CShader::Create(ptr_device_
			, TEXT("../../Reference/Shader/NormalMapShader.hlsl")));
	assert(hr == S_OK && "Shader_DynamicMesh Component Add Failed");

	// Mesh
	lstrcpy(loading_message_, TEXT("Mesh Loading"));
	hr = Engine::GameManager()->Add_Prototype(MAINTAIN_STAGE
		, TEXT("SpaceMarin_Mesh")
		, Engine::CDynamicMesh::Create(ptr_device_
			, TEXT("../bin/Resources/Mesh/SpaceMarin/")
			, TEXT("SpaceMarin.X")));
	assert(hr == S_OK && "SpaceMarin Mesh Add Failed");

	lstrcpy(loading_message_, TEXT("Scene Initializing"));
	*pp_next_scene_ = CStage::Create(ptr_device_);

	// NavMesh
	NavMeshDataLoad(TEXT("../bin/Data/StageData/Test_Nav.dat"));

	lstrcpy(loading_message_, TEXT("Stage Data Loading"));
	StageDataLoad(MAINTAIN_STAGE, TEXT("../bin/Data/StageData/Test.dat"));

	lstrcpy(loading_message_, TEXT("Loading Complete"));

	return S_OK;
}

HRESULT CLoading::Stage_Special_Loading()
{
	return S_OK;
}

HRESULT CLoading::StageDataLoad(MAINTAINID stage_id, const TCHAR* path)
{
	HRESULT hr = E_FAIL;
	Engine::CComponent* ptr_component = nullptr;

	HANDLE file = CreateFile(path, GENERIC_READ, 0, nullptr
		, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	size_t object_count = 0;
	Engine::CGameObject* ptr_obj = nullptr;
	TCHAR mesh_key[128] = TEXT("");
	TCHAR object_key[128] = TEXT("");
	DWORD byte = 0;

	ReadFile(file, &object_count, sizeof(size_t), &byte, nullptr);
	for (size_t i = 0; i < object_count; ++i)
	{
		ReadFile(file, mesh_key, sizeof(mesh_key), &byte, nullptr);
		ReadFile(file, object_key, sizeof(object_key), &byte, nullptr);

		if (0 == lstrcmp(mesh_key, TEXT("SpaceMarin")))
		{
			AddPlayerSpaceMarin(file, mesh_key, object_key, stage_id, byte);
			continue;
		}

		ptr_component = Engine::GameManager()->FindComponent(stage_id, mesh_key);
		if (nullptr == ptr_component)
		{
			hr = FindAndLoadMesh(stage_id, mesh_key, TEXT("..\\bin\\Resources\\Mesh\\MapData\\"));
			assert(!FAILED(hr) && "Failed to FindAndLoadMesh in Loading Class");
		}
		ptr_obj = CLevelObject::Create(ptr_device_, mesh_key);
		(*pp_next_scene_)->AddObject(stage_id, object_key, ptr_obj);

		ReadFile(file, ptr_obj->transform()->position(), sizeof(Vector3), &byte, nullptr);
		ReadFile(file, ptr_obj->transform()->rotation(), sizeof(Vector3), &byte, nullptr);
		ReadFile(file, ptr_obj->transform()->scale(), sizeof(Vector3), &byte, nullptr);
	}
	CloseHandle(file);

	return S_OK;
}

void CLoading::AddPlayerSpaceMarin(HANDLE file, const TCHAR * mesh_key, const TCHAR * object_key, MAINTAINID stage_id, DWORD & byte)
{
	Engine::CGameObject* ptr_obj = CSpaceMarin::Create(ptr_device_);
	assert(nullptr != ptr_obj && "SpaceMarin Create Failed");
	(*pp_next_scene_)->AddObject(stage_id, object_key, ptr_obj);

	Vector3 temp;

	ReadFile(file, ptr_obj->transform()->position(), sizeof(Vector3), &byte, nullptr);
	ReadFile(file, ptr_obj->transform()->rotation(), sizeof(Vector3), &byte, nullptr);
	ReadFile(file, temp, sizeof(Vector3), &byte, nullptr);
}

void CLoading::AddTeamSpaceMarin(HANDLE file, const TCHAR * mesh_key, const TCHAR * object_key, MAINTAINID stage_id, DWORD & byte)
{
	Engine::CGameObject* ptr_obj = CSpaceMarin::Create(ptr_device_);
	assert(nullptr != ptr_obj && "SpaceMarin Create Failed");

	(*pp_next_scene_)->AddObject(stage_id, object_key, ptr_obj);

	Vector3 temp;

	ReadFile(file, ptr_obj->transform()->position(), sizeof(Vector3), &byte, nullptr);
	ReadFile(file, ptr_obj->transform()->rotation(), sizeof(Vector3), &byte, nullptr);
	ReadFile(file, temp, sizeof(Vector3), &byte, nullptr);
}

void CLoading::AddEnemyObject(HANDLE file, const TCHAR * mesh_key, const TCHAR * object_key, MAINTAINID stage_id, DWORD & byte)
{
}

HRESULT CLoading::FindAndLoadMesh(MAINTAINID stage_id, const std::wstring & mesh_key, const std::wstring & path)
{
	FILESYSTEM::path find_file(path);

	if (false == FILESYSTEM::is_directory(find_file))
		return E_FAIL;

	for (const auto& directory : FILESYSTEM::directory_iterator(find_file))
	{
		if (true == FILESYSTEM::is_directory(directory.status()))
		{
			if (!FAILED(FindAndLoadMesh(stage_id, mesh_key, directory.path())))
				return S_OK;
		}

		if (FILESYSTEM::is_regular_file(directory.status())
			&& 0 == mesh_key.compare(directory.path().stem()))
		{
			std::wstring file_path = directory.path().c_str();
			const std::wstring file_name = mesh_key + TEXT(".X");
			auto iter = file_path.rfind(TEXT("\\"));
			file_path = file_path.substr(0, ++iter);

			Engine::CComponent* ptr_mesh = Engine::CStaticMesh::Create(ptr_device_
				, file_path.c_str(), file_name.c_str(), stage_id);
			assert(!(nullptr == ptr_mesh) && "Failed To LoadMesh in Loading Class");
			Engine::GameManager()->Add_Prototype(stage_id, mesh_key, ptr_mesh);

			return S_OK;
		}
	}

	return E_FAIL;
}

HRESULT CLoading::NavMeshDataLoad(const TCHAR * path)
{
	HANDLE file = CreateFile(path, GENERIC_READ, 0, nullptr
		, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	DWORD byte = 0;

	int vector_size = 0;
	size_t point_count = 0;
	std::vector<Vector3> vec_nav_point;
	ReadFile(file, &vector_size, sizeof(int), &byte, nullptr);
	ReadFile(file, &point_count, sizeof(size_t), &byte, nullptr);
	vec_nav_point.resize(point_count);
	for (size_t i = 0; i < point_count; ++i)
		ReadFile(file, vec_nav_point[i], sizeof(Vector3), &byte, nullptr);

	size_t nav_cell_count = 0;
	int point_index = 0;
	int cell_index = 0;
	int option = 0;
	int link_cell = 0;
	std::array<Vector3, 3> cell_point_array;

	ReadFile(file, &nav_cell_count, sizeof(size_t), &byte, nullptr);
	Engine::GameManager()->Create_NavMeshAgent(vector_size);
	
	for (size_t i = 0; i < nav_cell_count; ++i)
	{
		ReadFile(file, &point_index, sizeof(int), &byte, nullptr);
		cell_point_array[0] = vec_nav_point[point_index];
		ReadFile(file, &point_index, sizeof(int), &byte, nullptr);
		cell_point_array[1] = vec_nav_point[point_index];
		ReadFile(file, &point_index, sizeof(int), &byte, nullptr);
		cell_point_array[2] = vec_nav_point[point_index];

		while (false == ClockwiseCheckOfNavCell(cell_point_array));

		ReadFile(file, &cell_index, sizeof(int), &byte, nullptr);
		ReadFile(file, &option, sizeof(int), &byte, nullptr);
		ReadFile(file, &link_cell, sizeof(int), &byte, nullptr);

		Engine::GameManager()->AddNavCell(cell_point_array[0], cell_point_array[1], cell_point_array[2]
			, cell_index, option, link_cell);
	}
	
	Engine::GameManager()->LinkCell();

	CloseHandle(file);
	return S_OK;
}

bool CLoading::ClockwiseCheckOfNavCell(std::array<Vector3, 3>& cell_point_array)
{
	const Vector3 forward_vector = cell_point_array[1] - cell_point_array[0];
	const Vector3 right_vector = cell_point_array[2] - cell_point_array[0];
	Vector3 normal = Vector3::Cross(forward_vector, right_vector);

	if (normal.y > 0.f) return true;
	else
	{
		const Vector3 temp = cell_point_array[1];
		cell_point_array[1] = cell_point_array[2];
		cell_point_array[2] = temp;
		return false;
	}
	return true;
}

CLoading * CLoading::Create(LOADINGID loading_id, Engine::CScene** pp_scene_)
{
	CLoading* ptr_loading = new CLoading(loading_id, pp_scene_);
	if (FAILED(ptr_loading->InitLoading()))
	{
		Engine::Safe_Delete(ptr_loading);
		assert(!"Loading Object Create Failed");
	}

	return ptr_loading;
}

uint32 CLoading::LoadingFunction(void * ptr_arg)
{
	HRESULT hr = E_FAIL;
	CLoading* ptr_loading = (CLoading*)ptr_arg;

	switch (ptr_loading->loading_id_)
	{
	case LOADINGID::STAGE:
		hr = ptr_loading->Stage_Loading();
		break;
	case LOADINGID::STAGE_SPECIAL:
		hr = ptr_loading->Stage_Special_Loading();
		break;
	}
	assert(!FAILED(hr) && "Loading Failed");

	ptr_loading->complete_ = true;

	return 0;
}

void CLoading::Release()
{
	WaitForSingleObject(thread_, INFINITE);
	CloseHandle(thread_);
}
