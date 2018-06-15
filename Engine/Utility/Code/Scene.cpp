#include "Scene.h"
#include "Layer.h"

Engine::CScene::CScene(LPDIRECT3DDEVICE9 ptr_device)
	: ptr_device_(ptr_device)
{
}

Engine::CScene::~CScene()
{
	Release();
}

const Engine::CComponent * Engine::CScene::GetComponent(int layer_id, const std::wstring & object_key, const std::wstring component_key)
{
	auto iter = map_layer_.find(layer_id);
	if (iter == map_layer_.end())
		return nullptr;

	return iter->second->GetComponent(object_key, component_key);
}

void Engine::CScene::Update(float delta_time)
{
	for (auto& pair : map_layer_)
		pair.second->Update(delta_time);
}

void Engine::CScene::Render()
{
}

void Engine::CScene::Release()
{
	for (auto& pair : map_layer_)
		Safe_Delete(pair.second);

	map_layer_.clear();
}
