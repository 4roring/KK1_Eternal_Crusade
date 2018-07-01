#include "ComponentManager.h"
#include "Component.h"

Engine::CComponentManager::CComponentManager()
{
}

Engine::CComponentManager::~CComponentManager()
{
	Release();
}

Engine::CComponent * Engine::CComponentManager::FindComponent(int container_index, const std::wstring & component_key)
{
	auto iter = ptr_map_component_[container_index].find(component_key);
	if (iter == ptr_map_component_[container_index].end())
		return nullptr;

	return iter->second;
}

Engine::CComponent * Engine::CComponentManager::CloneComponent(int container_index, const std::wstring & component_key)
{
	CComponent* ptr_component = FindComponent(container_index, component_key);

	if (nullptr == ptr_component)
		return nullptr;

	return ptr_component->CloneComponent();
}

HRESULT Engine::CComponentManager::InitComponentManager(const int container_size)
{
	if (container_size == 0)
		return E_FAIL;

	ptr_map_component_ = new MapComponent[container_size];
	container_size_ = container_size;
	return S_OK;
}

HRESULT Engine::CComponentManager::Add_Prototype(int container_index, std::wstring component_key, CComponent * ptr_component)
{
	assert(nullptr != ptr_component && "Add Prototype Failed");

	auto iter = ptr_map_component_[container_index].find(component_key);
	if (iter != ptr_map_component_[container_index].end())
		assert(!"A component that already exists.");

	ptr_map_component_[container_index].emplace(component_key, ptr_component);
	return S_OK;
}

void Engine::CComponentManager::PrototypeClearances(int container_index)
{
	for (auto& pair : ptr_map_component_[container_index])
		Safe_Release_Delete(pair.second);
	ptr_map_component_[container_index].clear();
}

Engine::CComponentManager * Engine::CComponentManager::Create()
{
	return new CComponentManager;
}

void Engine::CComponentManager::Release()
{
	for (int i = 0; i < container_size_; ++i)
	{
		for(auto& pair : ptr_map_component_[i])
			Safe_Release_Delete(pair.second);
		ptr_map_component_[i].clear();
	}
	Safe_Delete_Array(ptr_map_component_);
}
