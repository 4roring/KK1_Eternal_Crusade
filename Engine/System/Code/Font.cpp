#include "Font.h"

Engine::CFont::CFont(LPDIRECT3DDEVICE9 ptr_device)
	: ptr_device_(ptr_device)
{
}

Engine::CFont::~CFont()
{
	Release();
}

HRESULT Engine::CFont::InitFont(int height, UINT width, UINT weight, const TCHAR * face_name)
{
	D3DXFONT_DESC font_info = {};
	font_info.Height = height;
	font_info.Width = width;
	font_info.Weight = weight;
	lstrcpy(font_info.FaceName, face_name);
	font_info.CharSet = HANGUL_CHARSET;

	return D3DXCreateFontIndirect(ptr_device_, &font_info, &ptr_font_);
}

void Engine::CFont::Render(const TCHAR * text, const D3DXCOLOR & color, const Vector3 & position)
{
	RECT rc = {(long)position.x, (long)position.y, 0, 0};
	ptr_font_->DrawTextW(nullptr, text, lstrlen(text), &rc, DT_NOCLIP, color);
}

Engine::CFont * Engine::CFont::Create(LPDIRECT3DDEVICE9 ptr_device, int height, UINT width, UINT weight, const TCHAR * face_name)
{
	CFont* ptr_font = new CFont(ptr_device);
	if (FAILED(ptr_font->InitFont(height, width, weight, face_name)))
	{
		Safe_Delete(ptr_font);
		assert(!"Init Font Failed");
	}

	return ptr_font;
}

void Engine::CFont::Release()
{
	Safe_Release(ptr_font_);
}
