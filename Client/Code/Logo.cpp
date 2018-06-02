#include "stdafx.h"
#include "Logo.h"
#include "SceneSelector.h"
#include "Layer.h"

// component
#include "Texture.h"
#include "Transform.h"
#include "Shader.h"

// object
#include "ScreenImage.h"

CLogo::CLogo(LPDIRECT3DDEVICE9 ptr_device)
	: Engine::CScene(ptr_device)
{
}

CLogo::~CLogo()
{
	Release();
}

HRESULT CLogo::InitScene()
{
	HRESULT hr = E_FAIL;
	hr = Engine::Component()->Add_Prototype(MAINTAIN_LOGO
		, TEXT("Logo_Texture")
		, Engine::CTexture::Create(ptr_device_
			, Engine::TEXTURETYPE::NORMAL
			, TEXT("../bin/Resources/Texture/Logo.jpg")
			, 1));
	assert(hr == S_OK && "Add Texture Failed");

	hr = Engine::Component()->Add_Prototype(MAINTAIN_STATIC
		, TEXT("Component_Transform")
		, Engine::CTransform::Create(g_kVecLook));
	assert(hr == S_OK && "Add Transform Component Failed");

	hr = Engine::Component()->Add_Prototype(MAINTAIN_STATIC
		, TEXT("Shader_Default")
		, Engine::CShader::Create(ptr_device_
			, TEXT("../../Reference/Shader/DefaultShader.hlsl")));
	assert(hr == S_OK && "Add Transform Component Failed");

	hr = Add_GameLogic_Layer();
	assert(hr == S_OK && "Add_GameLogic_Layer Call Failed");

	return S_OK;
}

void CLogo::Update(float delta_time)
{
	Engine::CScene::Update(delta_time);
}

CLogo * CLogo::Create(LPDIRECT3DDEVICE9 ptr_device)
{
	CLogo* ptr_scene = new CLogo(ptr_device);
	assert(nullptr != ptr_scene && "Logo Scene Create Failed");

	return ptr_scene;
}

HRESULT CLogo::Add_GameLogic_Layer()
{
	Engine::CLayer* ptr_layer = Engine::CLayer::Create();
	assert(nullptr != ptr_layer && "Game Logic Layer Create Failed");
	map_layer_.emplace(LAYER_GAMELOGIC, ptr_layer);

	Engine::CGameObject* ptr_object = CScreenImage::Create(ptr_device_);
	assert(nullptr != ptr_object && "ScreenImage Object Create Failed");
	ptr_layer->AddObject(TEXT("LogoImage"), ptr_object);

	return S_OK;
}

void CLogo::Release()
{
	//Engine::Safe_Delete(ptr_loading_);
	Engine::Component()->PrototypeClearances(MAINTAIN_LOGO);
}