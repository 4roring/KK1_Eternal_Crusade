#include "InputManager.h"

Engine::CInputManager::CInputManager()
{
}

Engine::CInputManager::~CInputManager()
{
	Release();
}

bool Engine::CInputManager::GetKey(DWORD input)
{
	if (input_state_ & input)
		return true;

	return false;
}

bool Engine::CInputManager::GetKeyDown(DWORD input)
{
	if (!(input_down_ & input))
	{
		if (input_state_ & input)
		{
			input_down_ |= input;
			return true;
		}
	}
	else if (input_down_ & input)
	{
		if (!(input_state_ & input))
			input_down_ ^= input;
	}

	return false;
}

bool Engine::CInputManager::GetKeyUp(DWORD input)
{
	if (!(input_up_ & input))
	{
		if (!(input_state_ & input))
		{
			input_up_ |= input;
			return true;
		}
	}
	else if (input_up_ & input)
	{
		if (input_state_ & input)
			input_up_ ^= input;
	}

	return false;
}

BYTE Engine::CInputManager::GetDIKeyState(BYTE key_flag)
{
	return key_state_[key_flag];
}

BYTE Engine::CInputManager::GetDIMouseState(MOUSECLICK key_flag)
{
	return mouse_state_.rgbButtons[key_flag];
}

long Engine::CInputManager::GetDIMouseMove(MOUSEMOVE key_flag)
{
	return *(((long*)&mouse_state_) + key_flag);
}

void Engine::CInputManager::GetMousePos(HWND hwnd, POINT * mouse_pos)
{
	GetCursorPos(mouse_pos);
	ScreenToClient(hwnd, mouse_pos);
}

HRESULT Engine::CInputManager::InitInputDevice(HINSTANCE hinstance, HWND hwnd)
{
	HRESULT hr = E_FAIL;
	hr = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8
		, (void**)&ptr_input_, nullptr);
	assert(!FAILED(hr) && "Direct Input Create Failed");

	hr = InitKeyBoard(hwnd);
	assert(!FAILED(hr) && "Keyboard Input Init Failed");

	hr = InitMouse(hwnd);
	assert(!FAILED(hr) && "Mouse Input Init Failed");

	return S_OK;
}

void Engine::CInputManager::SetInputState()
{
	ptr_keyboard_->GetDeviceState(256, key_state_);
	ptr_mouse_->GetDeviceState(sizeof(DIMOUSESTATE), &mouse_state_);

	input_state_ = 0;

	if (GetDIMouseState(DIM_LBUTTON) & 0x80)
		input_state_ |= KEY::LBUTTON;

	if (GetDIMouseState(DIM_RBUTTON) & 0x80)
		input_state_ |= KEY::RBUTTON;

	if (GetDIMouseState(DIM_MBUTTON) & 0x80)
		input_state_ |= KEY::MBUTTON;

	if (GetDIKeyState(DIK_W) & 0x80)
		input_state_ |= KEY::W;

	if (GetDIKeyState(DIK_A) & 0x80)
		input_state_ |= KEY::A;

	if (GetDIKeyState(DIK_S) & 0x80)
		input_state_ |= KEY::S;

	if (GetDIKeyState(DIK_D) & 0x80)
		input_state_ |= KEY::D;

	if (GetDIKeyState(DIK_Q) & 0x80)
		input_state_ |= KEY::Q;

	if (GetDIKeyState(DIK_E) & 0x80)
		input_state_ |= KEY::E;

	if (GetDIKeyState(DIK_R) & 0x80)
		input_state_ |= KEY::R;

	if (GetDIKeyState(DIK_X) & 0x80)
		input_state_ |= KEY::X;

	if (GetDIKeyState(DIK_Y) & 0x80)
		input_state_ |= KEY::Y;

	if (GetDIKeyState(DIK_Z) & 0x80)
		input_state_ |= KEY::Z;
}

HRESULT Engine::CInputManager::InitKeyBoard(HWND hwnd)
{
	HRESULT hr = E_FAIL;
	hr = ptr_input_->CreateDevice(GUID_SysKeyboard, &ptr_keyboard_, nullptr);
	assert(!FAILED(hr) && "Keyboard Device Create Failed");

	hr = ptr_keyboard_->SetCooperativeLevel(hwnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	assert(!FAILED(hr) && "Keyboard CooperativeLevel Set Failed");
	hr = ptr_keyboard_->SetDataFormat(&c_dfDIKeyboard);
	assert(!FAILED(hr) && "Keyboard DataFormat Set Failed");
	hr = ptr_keyboard_->Acquire();
	assert(!FAILED(hr) && "Keyboard Acquire Failed");

	return S_OK;
}

HRESULT Engine::CInputManager::InitMouse(HWND hwnd)
{
	HRESULT hr = E_FAIL;
	hr = ptr_input_->CreateDevice(GUID_SysMouse, &ptr_mouse_, nullptr);
	assert(!FAILED(hr) && "Mouse Device Create Failed");

	hr = ptr_mouse_->SetCooperativeLevel(hwnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	assert(!FAILED(hr) && "Mouse CooperativeLevel Set Failed");
	hr = ptr_mouse_->SetDataFormat(&c_dfDIMouse);
	assert(!FAILED(hr) && "Mouse DataFormat Set Failed");
	hr = ptr_mouse_->Acquire();
	assert(!FAILED(hr) && "Mouse Acquire Failed");

	return S_OK;
}

void Engine::CInputManager::Release()
{
	Safe_Release(ptr_mouse_);
	Safe_Release(ptr_keyboard_);
	Safe_Release(ptr_input_);
}
