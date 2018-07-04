#include "FontManager.h"
#include "Font.h"

Engine::CFontManager::CFontManager()
{
}

Engine::CFontManager::~CFontManager()
{
	Release();
}

HRESULT Engine::CFontManager::AddFont(LPDIRECT3DDEVICE9 ptr_device, const std::wstring font_key, int height, UINT width, UINT weight)
{
	auto iter = map_font_.find(font_key);
	if (iter != map_font_.end())
		assert(!"Add Font Error!!!");
	
	KK1_Font* ptr_font = KK1_Font::Create(ptr_device, height, width, weight, font_key.c_str());
	assert(nullptr != ptr_font && "Font Object Create Failed");

	map_font_.emplace(font_key, ptr_font);
	return S_OK;
}

Engine::KK1_Font * Engine::CFontManager::GetFont(const std::wstring font_key)
{
	auto iter = map_font_.find(font_key);
	if (iter == map_font_.end())
		assert(!"Get Font Find Error!!!");

	return iter->second;
}

Engine::CFontManager * Engine::CFontManager::Create()
{
	return new CFontManager;
}

void Engine::CFontManager::Release()
{
	for (auto& pair : map_font_)
		Safe_Delete(pair.second);
	map_font_.clear();
}
