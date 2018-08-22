#include "stdafx.h"
#include "LoadingScene.h"

#include "Loading.h"
#include "Font.h"
#include "Texture.h"
#include "Transform.h"
#include "Shader.h"
#include "ScreenImage.h"

CLoadingScene::CLoadingScene(LPDIRECT3DDEVICE9 ptr_device)
	: Engine::CScene(ptr_device)
{
}

CLoadingScene::~CLoadingScene()
{
	Release();
}

HRESULT CLoadingScene::InitScene()
{
	HRESULT hr = CScene::InitScene();

	hr = Engine::GameManager()->Add_Prototype(MAINTAIN_LOADING
		, TEXT("Loading_Texture")
		, Engine::CTexture::Create(ptr_device_
			, Engine::TEXTURETYPE::NORMAL
			, TEXT("../bin/Resources/Texture/Loading.jpg")
			, 1));
	assert(!FAILED(hr) && "Add Texture Failed");


	hr = Add_GameLogic_Layer();
	assert(!FAILED(hr) && "Add_GameLogic_Layer Call Failed");

	ptr_loading_ = CLoading::Create(CLoading::LOADINGID::STAGE_MH, &ptr_next_scene_);
	assert(nullptr != ptr_loading_ && "Failed to Create Loading Instance");

	ptr_loading_text_ = Engine::GraphicDevice()->GetFont(TEXT("¹ÙÅÁ"));
	assert(nullptr != ptr_loading_text_ && "Get FPS Font Failed");

	return S_OK;
}

void CLoadingScene::Update(float delta_time)
{
	Engine::CScene::Update(delta_time);

	if (true == ptr_loading_->complete())
	{
		if (nullptr != ptr_next_scene_)
			Engine::GameManager()->SetNextScene(ptr_next_scene_);
	}
}

void CLoadingScene::Render()
{
	const TCHAR* loading_text = ptr_loading_->loading_message();
	ptr_loading_text_->Render(loading_text, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), Vector3(50.f, 750.f, 0.f));
}

CLoadingScene * CLoadingScene::Create(LPDIRECT3DDEVICE9 ptr_device)
{
	CLoadingScene* ptr_scene = new CLoadingScene(ptr_device);
	assert(nullptr != ptr_scene && "Logo Scene Create Failed");
	ptr_scene->InitScene();

	return ptr_scene;

}

HRESULT CLoadingScene::Add_GameLogic_Layer()
{
	Engine::CGameObject* ptr_object = CScreenImage::Create(ptr_device_, TEXT("Loading_Texture"));
	assert(nullptr != ptr_object && "ScreenImage Object Create Failed");
	AddObject(LAYER_GAMELOGIC, TEXT("LogoImage"), ptr_object);

	return S_OK;
}

void CLoadingScene::Release()
{
	Engine::Safe_Delete(ptr_loading_);
	Engine::GameManager()->PrototypeClearances(MAINTAIN_LOADING);
}
