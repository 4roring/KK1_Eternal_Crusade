#include "InputManager.h"

Engine::CInputManager::CInputManager()
{
}

Engine::CInputManager::~CInputManager()
{
	Release();
}

BYTE Engine::CInputManager::GetDIKeyState(BYTE key_flag)
{
	return 0;
}

BYTE Engine::CInputManager::GetDIMouseState(MOUSECLICK key_flag)
{
	return 0;
}

long Engine::CInputManager::GetDIMouseMove(MOUSEMOVE key_flag)
{
	return 0;
}

HRESULT Engine::CInputManager::InitInputDevice(HINSTANCE hinstance, HWND hwnd)
{
	return E_NOTIMPL;
}

void Engine::CInputManager::SetInputState()
{
}

HRESULT Engine::CInputManager::InitKeyBoard(HWND hwnd)
{
	return E_NOTIMPL;
}

HRESULT Engine::CInputManager::InitMouse(HWND hwnd)
{
	return E_NOTIMPL;
}

void Engine::CInputManager::Release()
{
}
