#include "LightManager.h"
#include "Light.h"

Engine::CLightManager::CLightManager()
{
}

Engine::CLightManager::~CLightManager()
{
	Release();
}

HRESULT Engine::CLightManager::AddLight(LPDIRECT3DDEVICE9 ptr_device, const D3DLIGHT9 & light_info)
{
	CLight* ptr_light = CLight::Create(ptr_device, light_info);
	if (nullptr == ptr_light) return E_FAIL;

	light_list_.emplace_back(ptr_light);
	return S_OK;
}

void Engine::CLightManager::Render_Light(LPD3DXEFFECT ptr_effect)
{
	for (auto& light : light_list_)
		light->Render_Light(ptr_effect);
}

void Engine::CLightManager::ClearLight()
{
	Release();
}

void Engine::CLightManager::Release()
{
	for (auto& light : light_list_)
		Safe_Delete(light);

	light_list_.clear();
}
