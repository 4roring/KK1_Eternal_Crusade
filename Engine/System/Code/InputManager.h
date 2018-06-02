#pragma once

#include "Engine_Include.h"
#include "Engine_Singleton.h"
#pragma comment(lib, "dxguid.lib");
#pragma comment(lib, "dinput8.lib");

BEGIN(Engine)

class ENGINE_DLL CInputManager
	: public CSingleton<CInputManager>
{
private:
	friend CSingleton;
	CInputManager();
	virtual ~CInputManager();

public:
	enum MOUSECLICK { DIM_LBUTTON, DIM_RBUTTON, DIM_MBUTTON };
	enum MOUSEMOVE { DIM_X, DIM_Y, DIM_Z };

private:
	BYTE GetDIKeyState(BYTE key_flag);
	BYTE GetDIMouseState(MOUSECLICK key_flag);
	long GetDIMouseMove(MOUSEMOVE key_flag);

public:
	HRESULT InitInputDevice(HINSTANCE hinstance, HWND hwnd);
	void SetInputState();

private:
	HRESULT InitKeyBoard(HWND hwnd);
	HRESULT InitMouse(HWND hwnd);
	void Release();

private:
	LPDIRECTINPUT8 ptr_input_ = nullptr;
	LPDIRECTINPUTDEVICE8 ptr_keyboard_ = nullptr;
	LPDIRECTINPUTDEVICE8 ptr_mouse_ = nullptr;

private:
	BYTE key_state_[256] = {};
	DIMOUSESTATE mouse_state_ = {};
};

END