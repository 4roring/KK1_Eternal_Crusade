#include "stdafx.h"
#include "Logo.h"
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
	
	hr = CScene::InitScene();
	assert(hr == S_OK && "CScnen Class InitScene Failed");

	hr = Engine::GameManager()->Add_Prototype(MAINTAIN_LOADING
		, TEXT("Logo_Texture")
		, Engine::CTexture::Create(ptr_device_
			, Engine::TEXTURETYPE::NORMAL
			, TEXT("../bin/Resources/Texture/Logo.jpg")
			, 1));
	assert(!FAILED(hr) && "Add Texture Failed");

	hr = Engine::GameManager()->Add_Prototype(MAINTAIN_STATIC
		, TEXT("Component_Transform")
		, Engine::CTransform::Create(g_kLook));
	assert(!FAILED(hr) && "Add Transform Component Failed");

	hr = Engine::GameManager()->Add_Prototype(MAINTAIN_STATIC
		, TEXT("Shader_Default")
		, Engine::CShader::Create(ptr_device_
			, TEXT("../../Reference/Shader/DefaultShader.hlsl")));
	assert(!FAILED(hr) && "Shader_Default Component Add Failed");

	hr = Add_GameLogic_Layer();
	assert(!FAILED(hr) && "Add_GameLogic_Layer Call Failed");

	ptr_loading_ = CLoading::Create(CLoading::LOADINGID::STAGE_MH, &ptr_next_scene_);
	assert(nullptr != ptr_loading_ && "Failed to Create Loading Instance");

	ptr_loading_text_ = Engine::GraphicDevice()->GetFont(TEXT("¹ÙÅÁ"));
	assert(nullptr != ptr_loading_text_ && "Get FPS Font Failed");

	return S_OK;
}

void CLogo::Update(float delta_time)
{
	Engine::CScene::Update(delta_time);

	if (true == ptr_loading_->complete())
	{
		if (nullptr != ptr_next_scene_)
			Engine::GameManager()->SetNextScene(ptr_next_scene_);
	}
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
	ptr_scene->InitScene();

	return ptr_scene;
}

HRESULT CLogo::Add_GameLogic_Layer()
{
	Engine::CGameObject* ptr_object = CScreenImage::Create(ptr_device_, TEXT("Logo_Texture"));
	assert(nullptr != ptr_object && "ScreenImage Object Create Failed");
	AddObject(LAYER_GAMELOGIC, TEXT("LogoImage"), ptr_object);

	return S_OK;
}

void CLogo::Release()
{
	Engine::Safe_Delete(ptr_loading_);
	Engine::GameManager()->PrototypeClearances(MAINTAIN_LOADING);
}