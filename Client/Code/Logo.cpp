#include "stdafx.h"
#include "Logo.h"
#include "SceneSelector.h"
#include "Layer.h"
#include "Loading.h"
#include "Font.h"

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
		, Engine::CTransform::Create(g_kLook));
	assert(hr == S_OK && "Add Transform Component Failed");

	hr = Engine::Component()->Add_Prototype(MAINTAIN_STATIC
		, TEXT("Shader_Default")
		, Engine::CShader::Create(ptr_device_
			, TEXT("../../Reference/Shader/DefaultShader.hlsl")));
	assert(hr == S_OK && "Shader_Default Component Add Failed");

	hr = Engine::Component()->Add_Prototype(MAINTAIN_STATIC
		, TEXT("Shader_Mesh")
		, Engine::CShader::Create(ptr_device_
			, TEXT("../../Reference/Shader/MeshShader.hlsl")));
	assert(hr == S_OK && "Shader_Default Component Add Failed");


	hr = Add_GameLogic_Layer();
	assert(hr == S_OK && "Add_GameLogic_Layer Call Failed");

	ptr_loading_ = CLoading::Create(CLoading::LOADINGID::STAGE);

	ptr_loading_text_ = Engine::Font()->GetFont(TEXT("¹ÙÅÁ"));
	assert(nullptr != ptr_loading_text_ && "Get FPS Font Failed");

	return S_OK;
}

void CLogo::Update(float delta_time)
{
	Engine::CScene::Update(delta_time);

	if (true == ptr_loading_->complete())
		Engine::GameManager()->SceneChange(CSceneSelector(CSceneSelector::SCENE::STAGE));
}

void CLogo::Render()
{
	const TCHAR* loading_text = ptr_loading_->loading_message();
	ptr_loading_text_->Render(loading_text, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), Vector3(50.f, 750.f, 0.f));
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
	Engine::Safe_Delete(ptr_loading_);
	Engine::Component()->PrototypeClearances(MAINTAIN_LOGO);
}