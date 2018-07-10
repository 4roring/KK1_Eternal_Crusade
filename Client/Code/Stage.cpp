#include "stdafx.h"
#include "Stage.h"
#include "SceneSelector.h"
#include "Transform.h"

// Game Logic
#include "SpaceMarin.h"

// Test
#include "TestPlane.h"
#include "TestPlane2.h"

// UI
#include "DynamicCamera.h"

CStage::CStage(LPDIRECT3DDEVICE9 ptr_device)
	: Engine::CScene(ptr_device)
{
}

CStage::~CStage()
{
}

HRESULT CStage::InitScene()
{
	HRESULT hr = E_FAIL;

	hr = CScene::InitScene();
	assert(hr == S_OK && "CScnen Class InitScene Failed");

	hr = Add_Environmemt_Layer();
	assert(!FAILED(hr) && "Add_Environmemt_Layer Call Failed");
	hr = Add_GameLogic_Layer();
	assert(!FAILED(hr) && "Add_GameLogic_Layer Call Failed");
	hr = Add_UI_Layer();
	assert(!FAILED(hr) && "Add_UI_Layer Call Failed");
	hr = Add_Light();
	assert(!FAILED(hr) && "Add_Light Call Failed");

	return S_OK;
}

void CStage::Update(float delta_time)
{
	Engine::CScene::Update(delta_time);
}

CStage * CStage::Create(LPDIRECT3DDEVICE9 ptr_device)
{
	CStage* ptr_scene = new CStage(ptr_device);
	assert(nullptr != ptr_scene && "Stage Scene Create Failed");

	return ptr_scene;
}

HRESULT CStage::Add_Environmemt_Layer()
{
	return S_OK;
}

HRESULT CStage::Add_GameLogic_Layer()
{
	Engine::CGameObject* ptr_obj = nullptr;

	//ptr_obj = CTestPlane::Create(ptr_device_);
	//assert(nullptr != ptr_obj && "Test Plane Create Failed");
	//AddObject(LAYER_GAMELOGIC,TEXT("TestPlane"), ptr_obj);

	//ptr_obj = CTestPlane2::Create(ptr_device_);
	//assert(nullptr != ptr_obj && "Test Plane2 Create Failed");
	//AddObject(LAYER_GAMELOGIC, TEXT("TestPlane2"), ptr_obj);

	return S_OK;
}

HRESULT CStage::Add_UI_Layer()
{
	Engine::CGameObject* ptr_obj = nullptr;
	ptr_obj = CDynamicCamera::Create(ptr_device_, Vector3(0.f, 10.f, -20.f)
		, Vector3(0.f, 0.f, 0.f));
	assert(nullptr != ptr_obj && "Dynamic Camera Create Failed");
	AddObject(LAYER_UI, TEXT("DynamicCamera"), ptr_obj);

	return S_OK;
}

HRESULT CStage::Add_Light()
{
	D3DLIGHT9 light_info = {};
	light_info.Type = D3DLIGHT_DIRECTIONAL;
	light_info.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	light_info.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 0.2f);
	light_info.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	light_info.Direction = D3DXVECTOR3(0.f, -1.f, 1.f);

	ptr_device_->SetLight(0, &light_info);

	return S_OK;
}

void CStage::Release()
{
	Engine::GameManager()->PrototypeClearances(MAINTAIN_STAGE);
}
