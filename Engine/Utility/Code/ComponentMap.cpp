#include "ComponentMap.h"
#include "Component.h"

Engine::CComponentMap::CComponentMap()
{
}

Engine::CComponentMap::~CComponentMap()
{
	Release();
}

Engine::CComponent * Engine::CComponentMap::GetComponent(const std::wstring & component_key) const
{
	auto iter = map_component_.find(component_key);
	if (iter == map_component_.end())
		return nullptr;

	return iter->second;
}

void Engine::CComponentMap::AddComponent(const std::wstring & instance_key, CComponent * ptr_component)
{
	auto iter = map_component_.find(instance_key);
	if (iter == map_component_.end())
		map_component_.emplace(instance_key, ptr_component);
}

void Engine::CComponentMap::Update(float delta_time)
{
	for (auto& pair : map_component_)
	{
		if (false == pair.second->enable_)
			continue;
		pair.second->Update(delta_time);
	}
}

Engine::CComponentMap * Engine::CComponentMap::Create()
{
	return new CComponentMap;
}

void Engine::CComponentMap::Release()
{
	for (auto& pair : map_component_)
		Safe_Release_Delete(pair.second);

	map_component_.clear();
}

