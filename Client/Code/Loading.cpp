#include "stdafx.h"
#include "Loading.h"
#include <process.h>

#include "Texture.h"
#include "LineTexture.h"
#include "CubeTexture.h"
#include "TrailColor.h"
#include "StaticMesh.h"
#include "DynamicMesh.h"
#include "Shader.h"

#include "SpaceMarin.h"
#include "PlayerCamera.h"

#include "Ork.h"
#include "Ork_WarBoss.h"
#include "Rioreus.h"

#include "Stage.h"
#include "LevelObject.h"
#include "Transform.h"
#include "ParticleSystem.h"

#include <filesystem>
namespace FILESYSTEM = std::experimental::filesystem;

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

	EventManager()->InitEvent_For_Stage1();

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
			, TEXT("../../Reference/Shader/Deferred/Deferred_DynamicMeshShader.hlsl")));
	assert(hr == S_OK && "Shader_DynamicMesh Component Add Failed");

	hr = Engine::GameManager()->Add_Prototype(MAINTAIN_STATIC
		, TEXT("Shader_Mesh_Effect")
		, Engine::CShader::Create(ptr_device_
			, TEXT("../../Reference/Shader/MeshEffectShader.hlsl")));
	assert(hr == S_OK && "Shader_Mesh_Effect Component Add Failed");

	hr = Engine::GameManager()->Add_Prototype(MAINTAIN_STATIC
		, TEXT("Shader_Point_Effect")
		, Engine::CShader::Create(ptr_device_
			, TEXT("../../Reference/Shader/ParticleEffectShader.hlsl")));
	assert(hr == S_OK && "Shader_Point_Effect Component Add Failed");

	hr = Engine::GameManager()->Add_Prototype(MAINTAIN_STATIC
		, TEXT("Shader_NormalMap")
		, Engine::CShader::Create(ptr_device_
			, TEXT("../../Reference/Shader/Deferred/Deferred_StaticMeshShader.hlsl")));
	assert(hr == S_OK && "Shader_NormalMap Component Add Failed");

	hr = Engine::GameManager()->Add_Prototype(MAINTAIN_STATIC
		, TEXT("Shader_Sky")
		, Engine::CShader::Create(ptr_device_
			, TEXT("../../Reference/Shader/SkyShader.hlsl")));
	assert(hr == S_OK && "Shader_Sky Component Add Failed");

	hr = Engine::GameManager()->Add_Prototype(MAINTAIN_STATIC
		, TEXT("Shader_UI")
		, Engine::CShader::Create(ptr_device_
			, TEXT("../../Reference/Shader/UIShader.hlsl")));
	assert(hr == S_OK && "Shader_UI Component Add Failed");


	// Buffer or Mesh
	lstrcpy(loading_message_, TEXT("Mesh Loading"));

	hr = Engine::GameManager()->Add_Prototype(MAINTAIN_STATIC
		, TEXT("Buffer_Line")
		, Engine::CLineTexture::Create(ptr_device_));
	assert(!FAILED(hr) && "Buffer_Line Add Failed");

	hr = Engine::GameManager()->Add_Prototype(MAINTAIN_STATIC
		, TEXT("Buffer_Cube")
		, Engine::CCubeTexture::Create(ptr_device_));
	assert(!FAILED(hr) && "Buffer_CubeTexture Add Failed");

	hr = Engine::GameManager()->Add_Prototype(MAINTAIN_STATIC
		, TEXT("Buffer_TrailColor")
		, Engine::CTrailColor::Create(ptr_device_));
	assert(!FAILED(hr) && "Buffer_TrailColor Add Failed");

	hr = SpaceMarinLoad();
	assert(!FAILED(hr) && "SpaceMarin Load Failed");
	hr = OrkLoad();
	assert(!FAILED(hr) && "Ork Load Failed");

	// Texture
	lstrcpy(loading_message_, TEXT("Texture Loading"));
	hr = TextureLoad();
	assert(!FAILED(hr) && "Texture Load Failed");

	// Particle Effect
	hr = EffectLoad();
	assert(!FAILED(hr) && "Effect Load Failed");


	//hr = Engine::GameManager()->Add_Prototype(MAINTAIN_STAGE
	//	, TEXT("Tyranid_Mesh")
	//	, Engine::CDynamicMesh::Create(ptr_device_
	//		, TEXT("../bin/Resources/Mesh/Tyranid/")
	//		, TEXT("Tyranid.X")));
	//assert(hr == S_OK && "Tyranid Mesh Add Failed");

	lstrcpy(loading_message_, TEXT("Scene Initializing"));
	*pp_next_scene_ = CStage::Create(ptr_device_);

	lstrcpy(loading_message_, TEXT("Stage Data Loading"));
	//NavMeshDataLoad(TEXT("../bin/Data/StageData/Test_Nav.dat"));
	//StageDataLoad(MAINTAIN_STAGE, TEXT("../bin/Data/StageData/Test.dat"));

	NavMeshDataLoad(TEXT("../bin/Data/StageData/Stage_1_Nav.dat"));
	StageDataLoad(MAINTAIN_STAGE, TEXT("../bin/Data/StageData/Stage_1.dat"));

	lstrcpy(loading_message_, TEXT("Sound Loading"));
	hr = Sound()->Initialize();
	assert(hr == S_OK && "Sound Initialize Failed");

	lstrcpy(loading_message_, TEXT("Loading Complete"));

	return S_OK;
}

HRESULT CLoading::Stage_MH_Loading()
{
	HRESULT hr = E_FAIL;

	/* ======================TestCode====================== */
	//lstrcpy(loading_message_, TEXT("Shader Complie and Loading"));
	//hr = Engine::GameManager()->Add_Prototype(MAINTAIN_STATIC
	//	, TEXT("Shader_Mesh")
	//	, Engine::CShader::Create(ptr_device_
	//		, TEXT("../../Reference/Shader/MeshShader.hlsl")));
	//assert(hr == S_OK && "Shader_Mesh Component Add Failed");

	//hr = Engine::GameManager()->Add_Prototype(MAINTAIN_STATIC
	//	, TEXT("Shader_DynamicMesh")
	//	, Engine::CShader::Create(ptr_device_
	//		, TEXT("../../Reference/Shader/Deferred/Deferred_DynamicMeshShader.hlsl")));
	//assert(hr == S_OK && "Shader_DynamicMesh Component Add Failed");

	//hr = Engine::GameManager()->Add_Prototype(MAINTAIN_STATIC
	//	, TEXT("Shader_Mesh_Effect")
	//	, Engine::CShader::Create(ptr_device_
	//		, TEXT("../../Reference/Shader/MeshEffectShader.hlsl")));
	//assert(hr == S_OK && "Shader_Mesh_Effect Component Add Failed");

	//hr = Engine::GameManager()->Add_Prototype(MAINTAIN_STATIC
	//	, TEXT("Shader_Point_Effect")
	//	, Engine::CShader::Create(ptr_device_
	//		, TEXT("../../Reference/Shader/ParticleEffectShader.hlsl")));
	//assert(hr == S_OK && "Shader_Point_Effect Component Add Failed");

	//hr = Engine::GameManager()->Add_Prototype(MAINTAIN_STATIC
	//	, TEXT("Shader_NormalMap")
	//	, Engine::CShader::Create(ptr_device_
	//		, TEXT("../../Reference/Shader/Deferred/Deferred_StaticMeshShader.hlsl")));
	//assert(hr == S_OK && "Shader_NormalMap Component Add Failed");

	//hr = Engine::GameManager()->Add_Prototype(MAINTAIN_STATIC
	//	, TEXT("Shader_Sky")
	//	, Engine::CShader::Create(ptr_device_
	//		, TEXT("../../Reference/Shader/SkyShader.hlsl")));
	//assert(hr == S_OK && "Shader_Sky Component Add Failed");

	//hr = Engine::GameManager()->Add_Prototype(MAINTAIN_STATIC
	//	, TEXT("Shader_UI")
	//	, Engine::CShader::Create(ptr_device_
	//		, TEXT("../../Reference/Shader/UIShader.hlsl")));
	//assert(hr == S_OK && "Shader_UI Component Add Failed");


	//lstrcpy(loading_message_, TEXT("Mesh Loading"));

	//hr = Engine::GameManager()->Add_Prototype(MAINTAIN_STATIC
	//	, TEXT("Buffer_Line")
	//	, Engine::CLineTexture::Create(ptr_device_));
	//assert(!FAILED(hr) && "Buffer_Line Add Failed");

	//hr = Engine::GameManager()->Add_Prototype(MAINTAIN_STATIC
	//	, TEXT("Buffer_Cube")
	//	, Engine::CCubeTexture::Create(ptr_device_));
	//assert(!FAILED(hr) && "Buffer_CubeTexture Add Failed");

	//hr = Engine::GameManager()->Add_Prototype(MAINTAIN_STATIC
	//	, TEXT("Buffer_TrailColor")
	//	, Engine::CTrailColor::Create(ptr_device_));
	//assert(!FAILED(hr) && "Buffer_TrailColor Add Failed");

	//lstrcpy(loading_message_, TEXT("Sound Loading"));
	//hr = Sound()->Initialize();
	//assert(hr == S_OK && "Sound Initialize Failed");
	/* ======================TestCodeEnd=================== */


	// Texture
	lstrcpy(loading_message_, TEXT("Texture Loading"));
	hr = TextureLoad();
	assert(!FAILED(hr) && "Texture Load Failed");

	// Buffer or Mesh
	lstrcpy(loading_message_, TEXT("Mesh Loading"));
	hr = SpaceMarinLoad();
	assert(!FAILED(hr) && "SpaceMarin Load Failed");

	hr = Engine::GameManager()->Add_Prototype(MAINTAIN_STAGE
		, TEXT("Rioreus_Mesh")
		, Engine::CDynamicMesh::Create(ptr_device_
			, TEXT("../bin/Resources/Mesh/Rioreus/")
			, TEXT("Rioreus.X")));
	assert(hr == S_OK && "Rioreus Mesh Add Failed");

	hr = Engine::GameManager()->Add_Prototype(MAINTAIN_STAGE
		, TEXT("FireBall_Mesh")
		, Engine::CStaticMesh::Create(ptr_device_
			, TEXT("..\\bin\\Resources\\Mesh\\Weapon\\FireBall\\")
			, TEXT("FireBall.X"), MAINTAIN_STAGE));
	if (FAILED(hr)) return E_FAIL;

	hr = Engine::GameManager()->Add_Prototype(MAINTAIN_STAGE
		, TEXT("RioreusTail_Mesh")
		, Engine::CStaticMesh::Create(ptr_device_
			, TEXT("..\\bin\\Resources\\Mesh\\Weapon\\Rioreus_Tail\\")
			, TEXT("Rioreus_Tail.X"), MAINTAIN_STAGE));
	if (FAILED(hr)) return E_FAIL;

	// Particle Effect
	hr = EffectLoad();
	assert(!FAILED(hr) && "Effect Load Failed");

	lstrcpy(loading_message_, TEXT("Scene Initializing"));
	*pp_next_scene_ = CStage::Create(ptr_device_);

	lstrcpy(loading_message_, TEXT("Stage Data Loading"));
	NavMeshDataLoad(TEXT("../bin/Data/StageData/Stage_2_Nav.dat"));
	StageDataLoad(MAINTAIN_STAGE, TEXT("../bin/Data/StageData/Stage_2.dat"));

	lstrcpy(loading_message_, TEXT("Loading Complete"));

	SoundSource* ptr_bgm = Sound()->FindSound(TEXT("Rioreus_Theme"));
	Sound()->PlayBGM(ptr_bgm);

	EventManager()->InitEvent_For_Stage2();

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

		if (0 == lstrcmp(mesh_key, TEXT("Ork")))
		{
			AddEnemyOrk(file, mesh_key, object_key, stage_id, byte);
			continue;
		}

		if (0 == lstrcmp(mesh_key, TEXT("Ork_WarBoss")))
		{
			AddOrkWarBoss(file, mesh_key, object_key, stage_id, byte);
			continue;
		}

		if (0 == lstrcmp(mesh_key, TEXT("Rioreus")))
		{
			AddRioreus(file, mesh_key, object_key, stage_id, byte);
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
	Engine::CGameObject* ptr_obj = CSpaceMarin::Create(ptr_device_, space_marin_count_++);
	assert(nullptr != ptr_obj && "SpaceMarin Create Failed");
	(*pp_next_scene_)->AddObject(stage_id, object_key, ptr_obj);

	Vector3 temp;
	ReadFile(file, ptr_obj->transform()->position(), sizeof(Vector3), &byte, nullptr);
	ReadFile(file, ptr_obj->transform()->rotation(), sizeof(Vector3), &byte, nullptr);
	ReadFile(file, temp, sizeof(Vector3), &byte, nullptr);

	// Test Dummy SpaceMarin
	//ptr_obj = CSpaceMarin::Create(ptr_device_, 1);
	//assert(nullptr != ptr_obj && "SpaceMarin Create Failed");
	//(*pp_next_scene_)->AddObject(stage_id, TEXT("Space_Marin_1"), ptr_obj);

	//ptr_obj->transform()->position() = Vector3(0.f, 0.f, 0.f);
	//ptr_obj->transform()->rotation() = Vector3(0.f, 1.7f, 0.f);

	//ptr_obj = CSpaceMarin::Create(ptr_device_, 2);
	//assert(nullptr != ptr_obj && "SpaceMarin Create Failed");
	//(*pp_next_scene_)->AddObject(stage_id, TEXT("Space_Marin_2"), ptr_obj);

	//ptr_obj->transform()->position() = Vector3(1.f, 0.f, 0.f);
	//ptr_obj->transform()->rotation() = Vector3(0.f, 1.f, 0.f);

}

void CLoading::AddTeamSpaceMarin(HANDLE file, const TCHAR * mesh_key, const TCHAR * object_key, MAINTAINID stage_id, DWORD & byte)
{
	//Engine::CGameObject* ptr_obj = CSpaceMarin::Create(ptr_device_, ++space_marin_count_);
	//assert(nullptr != ptr_obj && "SpaceMarin Create Failed");

	//(*pp_next_scene_)->AddObject(stage_id, object_key, ptr_obj);

	//Vector3 temp;

	//ReadFile(file, ptr_obj->transform()->position(), sizeof(Vector3), &byte, nullptr);
	//ReadFile(file, ptr_obj->transform()->rotation(), sizeof(Vector3), &byte, nullptr);
	//ReadFile(file, temp, sizeof(Vector3), &byte, nullptr);
}

void CLoading::AddEnemyOrk(HANDLE file, const TCHAR * mesh_key, const TCHAR * object_key, MAINTAINID stage_id, DWORD & byte)
{
	Engine::CGameObject* ptr_obj = COrk::Create(ptr_device_, ork_count_++);
	assert(nullptr != ptr_obj && "Ork Create Failed");

	(*pp_next_scene_)->AddObject(stage_id, object_key, ptr_obj);

	Vector3 temp;

	ReadFile(file, ptr_obj->transform()->position(), sizeof(Vector3), &byte, nullptr);
	ReadFile(file, ptr_obj->transform()->rotation(), sizeof(Vector3), &byte, nullptr);
	ReadFile(file, temp, sizeof(Vector3), &byte, nullptr);

	EventManager()->AddEnemy(object_key, ptr_obj);
}

void CLoading::AddOrkWarBoss(HANDLE file, const TCHAR * mesh_key, const TCHAR * object_key, MAINTAINID stage_id, DWORD & byte)
{
	Engine::CGameObject* ptr_obj = COrk_WarBoss::Create(ptr_device_);
	assert(nullptr != ptr_obj && "Ork Create Failed");

	(*pp_next_scene_)->AddObject(stage_id, object_key, ptr_obj);

	Vector3 temp;

	ReadFile(file, ptr_obj->transform()->position(), sizeof(Vector3), &byte, nullptr);
	ReadFile(file, ptr_obj->transform()->rotation(), sizeof(Vector3), &byte, nullptr);
	ReadFile(file, temp, sizeof(Vector3), &byte, nullptr);

	EventManager()->AddEnemy(object_key, ptr_obj);
}

void CLoading::AddRioreus(HANDLE file, const TCHAR * mesh_key, const TCHAR * object_key, MAINTAINID stage_id, DWORD & byte)
{
	Engine::CGameObject* ptr_obj = CRioreus::Create(ptr_device_);
	assert(nullptr != ptr_obj && "Ork Create Failed");

	(*pp_next_scene_)->AddObject(stage_id, object_key, ptr_obj);

	Vector3 temp;

	ReadFile(file, ptr_obj->transform()->position(), sizeof(Vector3), &byte, nullptr);
	ReadFile(file, ptr_obj->transform()->rotation(), sizeof(Vector3), &byte, nullptr);
	ReadFile(file, temp, sizeof(Vector3), &byte, nullptr);

	EventManager()->AddEnemy(object_key, ptr_obj);
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

		TCHAR ex[128] = TEXT("");
		lstrcmp(ex, directory.path().c_str());

		if (FILESYSTEM::is_regular_file(directory.status())
			&& 0 == mesh_key.compare(directory.path().stem()))
		{
			std::wstring file_path = directory.path();
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

HRESULT CLoading::EffectDataLoad(const TCHAR * path)
{
	HANDLE file = CreateFile(path, GENERIC_READ, 0, nullptr
		, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	DWORD byte = 0;
	TCHAR texture_key[MAX_PATH] = TEXT("");
	int max_particle = 0;
	int type = 0;

	ReadFile(file, &type, sizeof(unsigned int), &byte, nullptr);
	ReadFile(file, &max_particle, sizeof(int), &byte, nullptr);

	Engine::CParticleSystem* ptr_particle_system_ = Engine::CParticleSystem::Create(ptr_device_, max_particle, (Engine::ParticleType)type);

	ReadFile(file, ptr_particle_system_->particles()->origin, sizeof(Vector3), &byte, nullptr);
	ReadFile(file, ptr_particle_system_->particles()->origin_min, sizeof(Vector3), &byte, nullptr);
	ReadFile(file, ptr_particle_system_->particles()->origin_max, sizeof(Vector3), &byte, nullptr);

	ReadFile(file, ptr_particle_system_->particles()->velocity, sizeof(Vector3), &byte, nullptr);
	ReadFile(file, ptr_particle_system_->particles()->velocity_min, sizeof(Vector3), &byte, nullptr);
	ReadFile(file, ptr_particle_system_->particles()->velocity_max, sizeof(Vector3), &byte, nullptr);

	ReadFile(file, ptr_particle_system_->particles()->gravity, sizeof(Vector3), &byte, nullptr);

	ReadFile(file, &ptr_particle_system_->particles()->scale, sizeof(float), &byte, nullptr);
	ReadFile(file, &ptr_particle_system_->particles()->scale_min, sizeof(float), &byte, nullptr);
	ReadFile(file, &ptr_particle_system_->particles()->scale_max, sizeof(float), &byte, nullptr);
	ReadFile(file, &ptr_particle_system_->particles()->speed, sizeof(float), &byte, nullptr);
	ReadFile(file, &ptr_particle_system_->particles()->life_time, sizeof(float), &byte, nullptr);
	ReadFile(file, &ptr_particle_system_->particles()->color.r, sizeof(float), &byte, nullptr);
	ReadFile(file, &ptr_particle_system_->particles()->color.g, sizeof(float), &byte, nullptr);
	ReadFile(file, &ptr_particle_system_->particles()->color.b, sizeof(float), &byte, nullptr);
	ReadFile(file, &ptr_particle_system_->particles()->color.a, sizeof(float), &byte, nullptr);

	ReadFile(file, &texture_key, sizeof(texture_key), &byte, nullptr);

	ReadFile(file, &ptr_particle_system_->particles()->duration, sizeof(float), &byte, nullptr);

	ReadFile(file, &ptr_particle_system_->particles()->max_u, sizeof(float), &byte, nullptr);
	ReadFile(file, &ptr_particle_system_->particles()->max_v, sizeof(float), &byte, nullptr);

	ReadFile(file, &ptr_particle_system_->particles()->loop, sizeof(bool), &byte, nullptr);
	ReadFile(file, &ptr_particle_system_->particles()->uv_animation, sizeof(bool), &byte, nullptr);
	ReadFile(file, &ptr_particle_system_->particles()->uv_loop, sizeof(bool), &byte, nullptr);
	ReadFile(file, &ptr_particle_system_->particles()->random_origin, sizeof(bool), &byte, nullptr);
	ReadFile(file, &ptr_particle_system_->particles()->random_velocity, sizeof(bool), &byte, nullptr);
	ReadFile(file, &ptr_particle_system_->particles()->random_color, sizeof(bool), &byte, nullptr);
	ReadFile(file, &ptr_particle_system_->particles()->random_rotation, sizeof(bool), &byte, nullptr);
	ReadFile(file, &ptr_particle_system_->particles()->random_scale, sizeof(bool), &byte, nullptr);

	CloseHandle(file);

	if (type == 1)
	{
		if (true == CheckExistTexture(texture_key))
			ptr_particle_system_->SetParticleTexture(MAINTAIN_STAGE, texture_key);
		ptr_particle_system_->SetParticleMeshKey(MAINTAIN_STATIC, TEXT("Buffer_RectTexture"));
	}

	FILESYSTEM::path key = path;
	Engine::GameManager()->Add_Prototype(MAINTAIN_STAGE, key.stem(), ptr_particle_system_);

	return S_OK;
}

bool CLoading::CheckExistTexture(const TCHAR * texture_key)
{
	Engine::CComponent* ptr_component = Engine::GameManager()->FindComponent(MAINTAIN_STAGE, texture_key);
	if (nullptr == ptr_component)
	{
		FILESYSTEM::path tex_path = TEXT("../bin/Resources/Texture/Effect/");
		if (false == FILESYSTEM::is_directory(tex_path))
			return false;

		for (const auto directory : FILESYSTEM::directory_iterator(tex_path))
		{
			if (true == FILESYSTEM::is_regular_file(directory.path())
				&& 0 == directory.path().stem().compare(texture_key))
			{
				Engine::GameManager()->Add_Prototype(MAINTAIN_STAGE, texture_key,
					Engine::CTexture::Create(ptr_device_, Engine::TEXTURETYPE::NORMAL, directory.path(), 1));
				return true;
			}
		}
		return false;
	}
	else
		return true;
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

HRESULT CLoading::SpaceMarinLoad()
{
	HRESULT hr = E_FAIL;
	hr = Engine::GameManager()->Add_Prototype(MAINTAIN_STAGE
		, TEXT("SpaceMarin_Mesh")
		, Engine::CDynamicMesh::Create(ptr_device_
			, TEXT("../bin/Resources/Mesh/SpaceMarin/")
			, TEXT("SpaceMarin.X")));
	if (FAILED(hr)) return E_FAIL;

	hr = Engine::GameManager()->Add_Prototype(MAINTAIN_STAGE
		, TEXT("ChainSword_Mesh")
		, Engine::CDynamicMesh::Create(ptr_device_
			, TEXT("../bin/Resources/Mesh/Weapon/SpaceMarin_ChainSword/")
			, TEXT("ChainSword.X")));
	if (FAILED(hr)) return E_FAIL;

	hr = Engine::GameManager()->Add_Prototype(MAINTAIN_STAGE
		, TEXT("Gun_Phobos_Mesh")
		, Engine::CStaticMesh::Create(ptr_device_
			, TEXT("..\\bin\\Resources\\Mesh\\Weapon\\Gun_Phobos\\")
			, TEXT("Gun_Phobos.X"), MAINTAIN_STAGE));
	if (FAILED(hr)) return E_FAIL;

	hr = Engine::GameManager()->Add_Prototype(MAINTAIN_STAGE
		, TEXT("Granade_Mesh")
		, Engine::CStaticMesh::Create(ptr_device_
			, TEXT("..\\bin\\Resources\\Mesh\\Weapon\\Granade\\")
			, TEXT("Granade.X"), MAINTAIN_STAGE));
	if (FAILED(hr)) return E_FAIL;

	return S_OK;
}

HRESULT CLoading::OrkLoad()
{
	HRESULT hr = E_FAIL;

	hr = Engine::GameManager()->Add_Prototype(MAINTAIN_STAGE
		, TEXT("Ork_Mesh")
		, Engine::CDynamicMesh::Create(ptr_device_
			, TEXT("../bin/Resources/Mesh/Ork/")
			, TEXT("Ork.X")));
	if (FAILED(hr)) return E_FAIL;

	hr = Engine::GameManager()->Add_Prototype(MAINTAIN_STAGE
		, TEXT("Ork_WarBoss_Mesh")
		, Engine::CDynamicMesh::Create(ptr_device_
			, TEXT("../bin/Resources/Mesh/Ork_WarBoss/")
			, TEXT("Ork_WarBoss.X")));
	if (FAILED(hr)) return E_FAIL;

	hr = Engine::GameManager()->Add_Prototype(MAINTAIN_STAGE
		, TEXT("Ork_Gun_Mesh")
		, Engine::CStaticMesh::Create(ptr_device_
			, TEXT("..\\bin\\Resources\\Mesh\\Weapon\\Ork_Gun\\")
			, TEXT("Ork_Gun.X"), MAINTAIN_STAGE));
	if (FAILED(hr)) return E_FAIL;

	hr = Engine::GameManager()->Add_Prototype(MAINTAIN_STAGE
		, TEXT("Ork_Sword_Mesh")
		, Engine::CStaticMesh::Create(ptr_device_
			, TEXT("..\\bin\\Resources\\Mesh\\Weapon\\Ork_Sword\\")
			, TEXT("Ork_Sword.X"), MAINTAIN_STAGE));
	if (FAILED(hr)) return E_FAIL;

	hr = Engine::GameManager()->Add_Prototype(MAINTAIN_STAGE
		, TEXT("Ork_HeavyGun_Mesh")
		, Engine::CStaticMesh::Create(ptr_device_
			, TEXT("..\\bin\\Resources\\Mesh\\Weapon\\Ork_HeavyGun\\")
			, TEXT("Ork_HeavyGun.X"), MAINTAIN_STAGE));
	if (FAILED(hr)) return E_FAIL;

	hr = Engine::GameManager()->Add_Prototype(MAINTAIN_STAGE
		, TEXT("Ork_Klaw_Mesh")
		, Engine::CStaticMesh::Create(ptr_device_
			, TEXT("..\\bin\\Resources\\Mesh\\Weapon\\Ork_PowerRippa\\")
			, TEXT("Ork_PowerRippa.X"), MAINTAIN_STAGE));
	if (FAILED(hr)) return E_FAIL;


	return S_OK;
}

HRESULT CLoading::EffectLoad()
{
	HRESULT hr = E_FAIL;

	hr = EffectDataLoad(TEXT("../bin/Data/EffectData/HitBlood.dat"));
	if (FAILED(hr)) return E_FAIL;
	hr = EffectDataLoad(TEXT("../bin/Data/EffectData/SwordHitBlood.dat"));
	if (FAILED(hr)) return E_FAIL;
	hr = EffectDataLoad(TEXT("../bin/Data/EffectData/BulletHit.dat"));
	if (FAILED(hr)) return E_FAIL;
	hr = EffectDataLoad(TEXT("../bin/Data/EffectData/ExecutionBlood.dat"));
	if (FAILED(hr)) return E_FAIL;
	hr = EffectDataLoad(TEXT("../bin/Data/EffectData/Explosion_1.dat"));
	if (FAILED(hr)) return E_FAIL;
	hr = EffectDataLoad(TEXT("../bin/Data/EffectData/Sword_Lightning.dat"));
	if (FAILED(hr)) return E_FAIL;
	hr = EffectDataLoad(TEXT("../bin/Data/EffectData/Move_Dust.dat"));
	if (FAILED(hr)) return E_FAIL;
	hr = EffectDataLoad(TEXT("../bin/Data/EffectData/WarBoss_Skill_Dust.dat"));
	if (FAILED(hr)) return E_FAIL;
	hr = EffectDataLoad(TEXT("../bin/Data/EffectData/WarBoss_Skill_Mini_Stone.dat"));
	if (FAILED(hr)) return E_FAIL;
	hr = EffectDataLoad(TEXT("../bin/Data/EffectData/WarBoss_Skill_Jump.dat"));
	if (FAILED(hr)) return E_FAIL;
	hr = EffectDataLoad(TEXT("../bin/Data/EffectData/Bomb.dat"));
	if (FAILED(hr)) return E_FAIL;
	hr = EffectDataLoad(TEXT("../bin/Data/EffectData/Bomb_Smoke.dat"));
	if (FAILED(hr)) return E_FAIL;

	hr = EffectDataLoad(TEXT("../bin/Data/EffectData/Explosion_2.dat"));
	if (FAILED(hr)) return E_FAIL;
	hr = EffectDataLoad(TEXT("../bin/Data/EffectData/Explosion_3.dat"));
	if (FAILED(hr)) return E_FAIL;
	hr = EffectDataLoad(TEXT("../bin/Data/EffectData/FireBall_Effect.dat"));
	if (FAILED(hr)) return E_FAIL;

	return hr;
}

HRESULT CLoading::TextureLoad()
{
	HRESULT hr = E_FAIL;

	hr = Engine::GameManager()->Add_Prototype(MAINTAIN_STAGE
		, TEXT("Sky_Texture")
		, Engine::CTexture::Create(ptr_device_, Engine::TEXTURETYPE::CUBE
			, TEXT("../bin/Resources/Texture/Skybox/Skybox.dds"), 1));
	if (FAILED(hr)) return E_FAIL;

	hr = Engine::GameManager()->Add_Prototype(MAINTAIN_STAGE
		, TEXT("Explosion_Texture")
		, Engine::CTexture::Create(ptr_device_, Engine::TEXTURETYPE::NORMAL
			, TEXT("../bin/Resources/Texture/Effect/T_ExplosionSeq4x4_001.tga"), 1));
	if (FAILED(hr)) return E_FAIL;

	hr = Engine::GameManager()->Add_Prototype(MAINTAIN_STAGE
		, TEXT("LineSmoke_Texture")
		, Engine::CTexture::Create(ptr_device_, Engine::TEXTURETYPE::NORMAL
			, TEXT("../bin/Resources/Texture/Effect/T_RibbonSmoky.tga"), 1));
	if (FAILED(hr)) return E_FAIL;

	hr = Engine::GameManager()->Add_Prototype(MAINTAIN_STAGE
		, TEXT("SkillRange_Texture")
		, Engine::CTexture::Create(ptr_device_, Engine::TEXTURETYPE::NORMAL
			, TEXT("../bin/Resources/Texture/Effect/Skill_Range.png"), 1));
	if (FAILED(hr)) return E_FAIL;

	hr = Engine::GameManager()->Add_Prototype(MAINTAIN_STAGE
		, TEXT("HP_UI_Texture")
		, Engine::CTexture::Create(ptr_device_, Engine::TEXTURETYPE::NORMAL
			, TEXT("../bin/Resources/Texture/UI/HP_Panel%d.tga"), 2));
	if (FAILED(hr)) return E_FAIL;

	hr = Engine::GameManager()->Add_Prototype(MAINTAIN_STAGE
		, TEXT("Shield_UI_Texture")
		, Engine::CTexture::Create(ptr_device_, Engine::TEXTURETYPE::NORMAL
			, TEXT("../bin/Resources/Texture/UI/Shield_Panel%d.tga"), 2));
	if (FAILED(hr)) return E_FAIL;

	hr = Engine::GameManager()->Add_Prototype(MAINTAIN_STAGE
		, TEXT("Panel_UI_Texture")
		, Engine::CTexture::Create(ptr_device_, Engine::TEXTURETYPE::NORMAL
			, TEXT("../bin/Resources/Texture/UI/EquipmentPanel_I53.tga"), 1));
	if (FAILED(hr)) return E_FAIL;

	hr = Engine::GameManager()->Add_Prototype(MAINTAIN_STAGE
		, TEXT("Granade_UI_Texture")
		, Engine::CTexture::Create(ptr_device_, Engine::TEXTURETYPE::NORMAL
			, TEXT("../bin/Resources/Texture/UI/CM_SM_Grenade_Frag.png"), 1));
	if (FAILED(hr)) return E_FAIL;

	hr = Engine::GameManager()->Add_Prototype(MAINTAIN_STAGE
		, TEXT("CSM_Mark_UI_Texture")
		, Engine::CTexture::Create(ptr_device_, Engine::TEXTURETYPE::NORMAL
			, TEXT("../bin/Resources/Texture/UI/CSM.png"), 1));
	if (FAILED(hr)) return E_FAIL;

	hr = Engine::GameManager()->Add_Prototype(MAINTAIN_STAGE
		, TEXT("Gun_UI_Texture")
		, Engine::CTexture::Create(ptr_device_, Engine::TEXTURETYPE::NORMAL
			, TEXT("../bin/Resources/Texture/UI/SM_Boltgun_TruekillBoltgun.png"), 1));
	if (FAILED(hr)) return E_FAIL;

	hr = Engine::GameManager()->Add_Prototype(MAINTAIN_STAGE
		, TEXT("Sword_UI_Texture")
		, Engine::CTexture::Create(ptr_device_, Engine::TEXTURETYPE::NORMAL
			, TEXT("../bin/Resources/Texture/UI/CSM_Chainsword_FleshRipper.png"), 1));
	if (FAILED(hr)) return E_FAIL;

	hr = Engine::GameManager()->Add_Prototype(MAINTAIN_STAGE
		, TEXT("Aim_UI_Texture")
		, Engine::CTexture::Create(ptr_device_, Engine::TEXTURETYPE::NORMAL
			, TEXT("../bin/Resources/Texture/UI/Aim_%d.png"), 2));
	if (FAILED(hr)) return E_FAIL;

	hr = Engine::GameManager()->Add_Prototype(MAINTAIN_STAGE
		, TEXT("Damage_UI_Texture")
		, Engine::CTexture::Create(ptr_device_, Engine::TEXTURETYPE::NORMAL
			, TEXT("../bin/Resources/Texture/UI/DamageDisplay.tga"), 1));
	if (FAILED(hr)) return E_FAIL;

	return S_OK;
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
	case LOADINGID::STAGE_MH:
		hr = ptr_loading->Stage_MH_Loading();
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
