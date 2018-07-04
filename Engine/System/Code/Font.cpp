#include "Font.h"

Engine::KK1_Font::KK1_Font(LPDIRECT3DDEVICE9 ptr_device)
	: ptr_device_(ptr_device)
{
}

Engine::KK1_Font::~KK1_Font()
{
	Release();
}

HRESULT Engine::KK1_Font::InitFont(int height, UINT width, UINT weight, const TCHAR * face_name)
{
	D3DXFONT_DESC font_info = {};
	font_info.Height = height;
	font_info.Width = width;
	font_info.Weight = weight;
	lstrcpy(font_info.FaceName, face_name);
	font_info.CharSet = HANGUL_CHARSET;

	return D3DXCreateFontIndirect(ptr_device_, &font_info, &ptr_font_);
}

void Engine::KK1_Font::Render(const TCHAR * text, const D3DXCOLOR & color, const Vector3 & position)
{
	RECT rc = {(long)position.x, (long)position.y, 0, 0};
	ptr_font_->DrawTextW(nullptr, text, lstrlen(text), &rc, DT_NOCLIP, color);
}

Engine::KK1_Font * Engine::KK1_Font::Create(LPDIRECT3DDEVICE9 ptr_device, int height, UINT width, UINT weight, const TCHAR * face_name)
{
	KK1_Font* ptr_font = new KK1_Font(ptr_device);
	if (FAILED(ptr_font->InitFont(height, width, weight, face_name)))
	{
		Safe_Delete(ptr_font);
		assert(!"Init Font Failed");
	}

	return ptr_font;
}

void Engine::KK1_Font::Release()
{
	Safe_Release(ptr_font_);
}
