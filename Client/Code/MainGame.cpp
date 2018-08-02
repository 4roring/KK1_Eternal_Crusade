#include "stdafx.h"
#include "MainGame.h"

#include "RectTexture.h"
#include "SceneSelector.h"
#include "Font.h"

#include <time.h>

#include "Logo.h"

CMainGame::CMainGame()
	: ptr_game_manager_(Engine::GameManager())
	, ptr_time_(Engine::Time())
	, ptr_input_(Engine::Input())
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

	if (true == NullCheckOfManager()) return E_FAIL;

	hr = ptr_game_manager_->InitManager(ptr_device_);
	assert(hr == S_OK && "Init GameManager Failed");

	hr = ptr_game_manager_->InitComponentManager(MAINTAIN_END);
	assert(hr == S_OK && "Init Component Manager Failed");

	hr = Engine::GraphicDevice()->AddFont(TEXT("¹ÙÅÁ"), 28, 20, FW_NORMAL);
	assert(hr == S_OK && "Add FPS Font Failed");
	ptr_fps_font_ = Engine::GraphicDevice()->GetFont(TEXT("¹ÙÅÁ"));
	assert(nullptr != ptr_fps_font_ && "Get FPS Font Failed");

	hr = ptr_game_manager_->Add_Prototype(MAINTAIN_STATIC, TEXT("Buffer_RectTexture")
		, Engine::CRectTexture::Create(ptr_device_));
	assert(hr == S_OK && "Add RectTexture Component Failed");

	hr = ptr_game_manager_->SetNextScene(CLogo::Create(ptr_device_));

	D3DLIGHT9 light_info = {};
	light_info.Type = D3DLIGHT_DIRECTIONAL;
	light_info.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	light_info.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	light_info.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	light_info.Direction = Vector3(0.f, -1.f, 0.5f);

	ptr_game_manager_->AddLight(light_info);

	ptr_time_->InitTime();
	ptr_input_->InitInputDevice(g_hinstance, g_hwnd);

	//Engine::Time()->SetMaxFrame(9999999);
	return S_OK;
}

void CMainGame::Update()
{
	if (true == NullCheckOfManager()) return;
	ptr_input_->SetInputState();
	ptr_time_->SetTime();
	ptr_game_manager_->Update(Engine::Time()->GetDeltaTime());
}

void CMainGame::Render()
{
	if (true == NullCheckOfManager()) return;

	ptr_device_->Clear(0, nullptr, D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER
		, D3DCOLOR_ARGB(255, 0, 0, 255), 1.f, 0);
	ptr_device_->BeginScene();

	ptr_game_manager_->Render();
	Render_FPS();

	ptr_device_->EndScene();
	ptr_device_->Present(nullptr, nullptr, nullptr, nullptr);
}

void CMainGame::LastFrame()
{
	if (true == NullCheckOfManager()) return;

	ptr_game_manager_->LastFrame();
	CollSystem()->LastFrame();
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
	CollSystem()->DestroyInstance();
	ptr_input_->DestroyInstance();
	ptr_input_ = nullptr;
	ptr_game_manager_->DestroyInstance();
	ptr_game_manager_ = nullptr;
	ptr_time_->DestroyInstance();
	ptr_time_ = nullptr;
	Engine::GraphicDevice()->DestroyInstance();
	ptr_device_ = nullptr;
}

bool CMainGame::NullCheckOfManager()
{
	if (nullptr == ptr_device_) return true;
	if (nullptr == ptr_game_manager_) return true;
	if (nullptr == ptr_time_) return true;
	if (nullptr == ptr_input_) return true;

	return false;
}
