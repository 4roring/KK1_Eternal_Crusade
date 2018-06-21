#include "stdafx.h"
#include "MainGame.h"

#include "RectTexture.h"
#include "SceneSelector.h"
#include "Font.h"

#include <time.h>

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

	srand((unsigned)time(nullptr));

	hr = Engine::GraphicDevice()->InitGraphicDevice(Engine::CGraphicDevice::MODE_WINDOW
		, g_hwnd, kWinCx, kWinCy);
	assert(hr == S_OK && "Init Device Failed");

	ptr_device_ = Engine::GraphicDevice()->GetDevice();

	hr = Engine::GameManager()->InitManager(ptr_device_);
	assert(hr == S_OK && "Init GameManager Failed");

	hr = Engine::GameManager()->InitComponentManager(MAINTAIN_END);
	assert(hr == S_OK && "Init Component Manager Failed");

	hr = Engine::GraphicDevice()->AddFont(TEXT("¹ÙÅÁ"), 28, 20, FW_NORMAL);
	assert(hr == S_OK && "Add FPS Font Failed");
	ptr_fps_font_ = Engine::GraphicDevice()->GetFont(TEXT("¹ÙÅÁ"));
	assert(nullptr != ptr_fps_font_ && "Get FPS Font Failed");

	hr = Engine::GameManager()->Add_Prototype(MAINTAIN_STATIC, TEXT("Buffer_RectTexture")
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
	Engine::Input()->InitInputDevice(g_hinstance, g_hwnd);
	return S_OK;
}

void CMainGame::Update()
{
	Engine::Input()->SetInputState();
	Engine::Time()->SetTime();
	Engine::GameManager()->Update(Engine::Time()->GetDeltaTime());
}

void CMainGame::Render()
{
	ptr_device_->Clear(0, nullptr, D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER
		, D3DCOLOR_ARGB(255, 0, 0, 255), 1.f, 0);
	ptr_device_->BeginScene();

	Engine::GameManager()->Render();

	Render_FPS();

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

void CMainGame::Render_FPS()
{
	fps_time_ += Engine::Time()->GetDeltaTime();
	++frame_count_;

	if (fps_time_ >= 1.f)
	{
		wsprintf(fps_, TEXT("FPS: %d"), frame_count_);
		fps_time_ = 0.f;
		frame_count_ = 0;
	}

	ptr_fps_font_->Render(fps_, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), Vector3(0.f, 0.f, 0.f));
}

void CMainGame::Release()
{
	Engine::Input()->DestroyInstance();

	Engine::GameManager()->DestroyInstance();
	Engine::Time()->DestroyInstance();
	Engine::GraphicDevice()->DestroyInstance();
}
