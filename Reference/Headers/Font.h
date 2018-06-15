#pragma once

#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CFont
{
private:
	explicit CFont(LPDIRECT3DDEVICE9 ptr_device);

public:
	~CFont();

public:
	HRESULT InitFont(int height, UINT width, UINT weight, const TCHAR* face_name);
	void Render(const TCHAR* text, const D3DXCOLOR& color, const Vector3& position);

public:
	static CFont* Create(LPDIRECT3DDEVICE9 ptr_device, int height, UINT width, UINT weight, const TCHAR* face_name);

private:
	void Release();

private:
	LPDIRECT3DDEVICE9 ptr_device_ = nullptr;
	LPD3DXFONT ptr_font_ = nullptr;
};


END