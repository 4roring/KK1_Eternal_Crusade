#pragma once

#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL KK1_Font
{
private:
	explicit KK1_Font(LPDIRECT3DDEVICE9 ptr_device);

public:
	~KK1_Font();

public:
	HRESULT InitFont(int height, uint32 width, uint32 weight, const TCHAR* face_name);
	HRESULT InitFont(int height, uint32 width, uint32 weight, const TCHAR* face_name, const TCHAR* font_path);
	void Render(const TCHAR* text, const D3DXCOLOR& color, const Vector3& position);

public:
	static KK1_Font* Create(LPDIRECT3DDEVICE9 ptr_device, int height, uint32 width, uint32 weight, const TCHAR* face_name);
	static KK1_Font* Create(LPDIRECT3DDEVICE9 ptr_device, int height, uint32 width, uint32 weight, const TCHAR* face_name, const TCHAR* font_path);

private:
	void Release();

private:
	LPDIRECT3DDEVICE9 ptr_device_ = nullptr;
	LPD3DXFONT ptr_font_ = nullptr;
};


END