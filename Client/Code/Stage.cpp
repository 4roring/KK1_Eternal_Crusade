#include "stdafx.h"
#include "Stage.h"
#include "SceneSelector.h"
#include "Transform.h"

// Game Logic
#include "Skybox.h"
#include "SpaceMarin.h"

// Test
#include "TestPlane.h"
#include "TestPlane2.h"

// Camera
#include "DynamicCamera.h"
#include "PlayerCamera.h"

CStage::CStage(LPDIRECT3DDEVICE9 ptr_device)
	: Engine::CScene(ptr_device)
{
}

CStage::~CStage()
{
}

HRESULT CStage::LateInit()
{
	HRESULT hr = E_FAIL;

	hr = Add_Environmemt_Layer();
	assert(!FAILED(hr) && "Add_Environmemt_Layer Call Failed");
	hr = Add_GameLogic_Layer();
	assert(!FAILED(hr) && "Add_GameLogic_Layer Call Failed");
	hr = Add_Camera_Layer();
	assert(!FAILED(hr) && "Add_Camera_Layer Call Failed");
	hr = Add_UI_Layer();
	assert(!FAILED(hr) && "Add_UI_Layer Call Failed");
	hr = Add_Light();
	assert(!FAILED(hr) && "Add_Light Call Failed");
	
	CScene::LateInit();

	return S_OK;
}

void CStage::Update(float delta_time)
{
	Engine::CScene::Update(delta_time);
	EventManager()->CheckEvent(delta_time);
}

CStage * CStage::Create(LPDIRECT3DDEVICE9 ptr_device)
{
	CStage* ptr_scene = new CStage(ptr_device);
	if (FAILED(ptr_scene->InitScene()))
	{
		Safe_Delete(ptr_scene);
		assert(!"Stage Scene Create Failed");
	}
	return ptr_scene;
}

HRESULT CStage::Add_Environmemt_Layer()
{
	Engine::CGameObject* ptr_obj = nullptr;
	
	ptr_obj = CSkybox::Create(ptr_device_);
	assert(nullptr != ptr_obj && "Skybox Create Failed");
	AddObject(LAYER_ENVIRONMEMT, TEXT("Skybox"), ptr_obj);

	return S_OK;
}

HRESULT CStage::Add_GameLogic_Layer()
{
	//Engine::CGameObject* ptr_obj = nullptr;

	return S_OK;
}

HRESULT CStage::Add_Camera_Layer()
{
	Engine::CGameObject* ptr_obj = nullptr;

	ptr_obj = CPlayerCamera::Create(ptr_device_, MAINTAIN_STAGE);
	assert(nullptr != ptr_obj && "Player Camera Create Failed");
	AddObject(LAYER_CAMERA, TEXT("Player_Camera"), ptr_obj);

	//ptr_obj = CDynamicCamera::Create(ptr_device_, Vector3(0.f, 10.f, -20.f)
	//	, Vector3(0.f, 0.f, 0.f));
	//assert(nullptr != ptr_obj && "Dynamic Camera Create Failed");
	//AddObject(LAYER_UI, TEXT("DynamicCamera"), ptr_obj);

	return S_OK;
}

HRESULT CStage::Add_UI_Layer()
{
	return S_OK;
}

HRESULT CStage::Add_Light()
{
	return S_OK;
}

void CStage::Release()
{
	Engine::GameManager()->PrototypeClearances(MAINTAIN_STAGE);
	Engine::GameManager()->ClearLight();
}
