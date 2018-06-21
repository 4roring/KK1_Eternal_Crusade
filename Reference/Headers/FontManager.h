#pragma once

#include "Engine_Include.h"
#include "Engine_Singleton.h"

BEGIN(Engine)

class CFont;

class CFontManager
{
private:
	CFontManager();

public:
	~CFontManager();

public:
	HRESULT AddFont(LPDIRECT3DDEVICE9 ptr_device, const std::wstring font_key, int height, UINT width, UINT weight);
	CFont* GetFont(const std::wstring font_key);

public:
	static CFontManager* Create();

private:
	void Release();

private:
	std::unordered_map<std::wstring, CFont*> map_font_;
};

END