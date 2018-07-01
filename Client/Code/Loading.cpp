#include "stdafx.h"
#include "Loading.h"
#include <process.h>

#include "Texture.h"
#include "CubeTexture.h"
#include "StaticMesh.h"
#include "DynamicMesh.h"
#include "Shader.h"


CLoading::CLoading(LOADINGID loading_id)
	: loading_id_(loading_id)
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

	// Texture
	//hr = Engine::GameManager()->Add_Prototype(MAINTAIN_STAGE
	//	, TEXT("Texture_TestPlane_Color")
	//	, Engine::CTexture::Create(ptr_device_
	//		, Engine::TEXTURETYPE::NORMAL
	//		, TEXT("../bin/Resources/TestAsset/T_FullGrass_D.tga")
	//		, 1));
	//assert(hr == S_OK && "Add Texture Failed");

	//hr = Engine::GameManager()->Add_Prototype(MAINTAIN_STAGE
	//	, TEXT("Texture_TestPlane_Normal")
	//	, Engine::CTexture::Create(ptr_device_
	//		, Engine::TEXTURETYPE::NORMAL
	//		, TEXT("../bin/Resources/TestAsset/T_FullGrass_N.tga")
	//		, 1));
	//assert(hr == S_OK && "Add Texture Failed");


	// Mesh
	lstrcpy(loading_message_, TEXT("Mesh Loading"));
	hr = Engine::GameManager()->Add_Prototype(MAINTAIN_STAGE
		, TEXT("SpaceMarin_Mesh")
		, Engine::CDynamicMesh::Create(ptr_device_
			, TEXT("../bin/Resources/Mesh/SpaceMarin/")
			, TEXT("SpaceMarin.X")));
	assert(hr == S_OK && "SpaceMarin Mesh Add Failed");

	lstrcpy(loading_message_, TEXT("Mesh Loading"));
	hr = Engine::GameManager()->Add_Prototype(MAINTAIN_STAGE
		, TEXT("Floor_12x12_Mesh")
		, Engine::CStaticMesh::Create(ptr_device_
			, TEXT("../bin/Resources/Mesh/MapData/Floor/")
			, TEXT("Floor_12x12.X"), MAINTAIN_STAGE));
	assert(hr == S_OK && "Floor_12x12 Mesh Add Failed");

	// Shader
	lstrcpy(loading_message_, TEXT("Shader Loading"));
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

	lstrcpy(loading_message_, TEXT("Loading Complete"));
	return S_OK;
}

HRESULT CLoading::Stage_Special_Loading()
{
	return S_OK;
}

CLoading * CLoading::Create(LOADINGID loading_id)
{
	CLoading* ptr_loading = new CLoading(loading_id);
	if (FAILED(ptr_loading->InitLoading()))
	{
		Engine::Safe_Delete(ptr_loading);
		assert(!"Loading Object Create Failed");
	}

	return ptr_loading;
}

UINT CLoading::LoadingFunction(void * ptr_arg)
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
