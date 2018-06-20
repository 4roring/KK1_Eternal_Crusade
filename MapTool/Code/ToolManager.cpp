#include "stdafx.h"
#include "ToolManager.h"

CToolManager::CToolManager()
{
}

CToolManager::~CToolManager()
{
	Release();
}

void CToolManager::Init_ToolManager()
{
	Engine::GraphicDevice()->InitGraphicDevice(Engine::GraphicDevice()->MODE_WINDOW, g_hwnd, g_kWinCx, g_kWinCy);
	ptr_device_ = Engine::GraphicDevice()->GetDevice();
}

void CToolManager::Update(float delta_time)
{
	Engine::Input()->SetInputState();
}

void CToolManager::Render()
{
}

void CToolManager::Release()
{
}

void CToolManager::BegineScene()
{
	ptr_device_->Clear(0, nullptr, D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER
		, D3DCOLOR_ARGB(255, 128, 128, 128), 1.f, 0);
	ptr_device_->BeginScene();
}

void CToolManager::EndScene(HWND hwnd)
{
	ptr_device_->EndScene();
	ptr_device_->Present(nullptr, nullptr, hwnd, nullptr);
}

void CToolManager::EnableCursor()
{
}

void CToolManager::DisableCursor()
{
}
