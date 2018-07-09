#pragma once

#include "Engine_Include.h"
#include "Engine_Singleton.h"

BEGIN(Engine)

class KK1_Font;

class CFontManager
{
private:
	CFontManager();

public:
	~CFontManager();

public:
	HRESULT AddFont(LPDIRECT3DDEVICE9 ptr_device, const std::wstring font_key, int height, uint32 width, uint32 weight);
	KK1_Font* GetFont(const std::wstring font_key);

public:
	static CFontManager* Create();

private:
	void Release();

private:
	std::unordered_map<std::wstring, KK1_Font*> map_font_;
};

END