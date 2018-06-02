#include "stdafx.h"
#include "MainGame.h"

#include "RectTexture.h"
#include "SceneSelector.h"

CMainGame::CMainGame()
{
}

CMainGame::~CMainGame()
{
	Release();
}

HRESULT CMainGame::InitGame()
{
	HRESULT hr = E_FAIL;

	hr = Engine::GraphicDevice()->InitGraphicDevice(Engine::CGraphicDevice::MODE_WINDOW
		, g_hwnd, kWinCx, kWinCy);
	assert(hr == S_OK && "Init Device Failed");

	ptr_device_ = Engine::GraphicDevice()->GetDevice();

	hr = Engine::GameManager()->InitManager(ptr_device_);
	assert(hr == S_OK && "Init GameManager Failed");

	hr = Engine::Component()->InitComponentManager(MAINTAIN_END);
	assert(hr == S_OK && "Init Component Manager Failed");

	hr = Engine::Component()->Add_Prototype(MAINTAIN_STATIC, TEXT("Buffer_RectTexture")
		, Engine::CRectTexture::Create(ptr_device_));
	assert(hr == S_OK && "Add RectTexture Component Failed");

	hr = Engine::GameManager()->SceneChange(CSceneSelector(CSceneSelector::SCENE::LOGO));

	D3DLIGHT9 light_info = {};
	light_info.Type = D3DLIGHT_DIRECTIONAL;
	light_info.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	light_info.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	light_info.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	light_info.Direction = Vector3(0.f, -1.f, 1.f);

	ptr_device_->SetLight(0, &light_info);

	Engine::Time()->InitTime();
	return S_OK;
}

void CMainGame::Update()
{
	Engine::Time()->SetTime();
	Engine::GameManager()->Update(Engine::Time()->GetDeltaTime());
}

void CMainGame::Render()
{
	ptr_device_->Clear(0, nullptr, D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER
		, D3DCOLOR_ARGB(255, 0, 0, 255), 1.f, 0);
	ptr_device_->BeginScene();

	Engine::GameManager()->Render();

	ptr_device_->EndScene();
	ptr_device_->Present(nullptr, nullptr, nullptr, nullptr);
}

CMainGame * CMainGame::Create()
{
	CMainGame* pMainGame = new CMainGame;
	if (FAILED(pMainGame->InitGame()))
	{
		delete pMainGame;
		pMainGame = nullptr;
	}

	return pMainGame;
}

void CMainGame::Release()
{
	Engine::GameManager()->DestroyInstance();
	Engine::Time()->DestroyInstance();
	Engine::Component()->DestroyInstance();
	Engine::GraphicDevice()->DestroyInstance();
}
