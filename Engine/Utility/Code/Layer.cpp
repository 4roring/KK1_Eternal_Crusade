#include "Layer.h"
#include "GameObject.h"

Engine::CLayer::CLayer()
{
}

Engine::CLayer::~CLayer()
{
	Release();
}

const Engine::CComponent * Engine::CLayer::GetComponent(const std::wstring & object_key, const std::wstring & component_key)
{
	auto mapiter = map_object_list_.find(object_key);
	if (mapiter == map_object_list_.end())
		return nullptr;

	for (auto& object : mapiter->second)
	{
		const CComponent* ptr_component = object->GetComponent(component_key);
		if (nullptr != ptr_component)
			return ptr_component;
	}
	return nullptr;
}

HRESULT Engine::CLayer::AddObject(const std::wstring & object_key, Engine::CGameObject * ptr_object)
{
#ifdef _DEBUG
	assert(nullptr != ptr_object && "AddObject is nullptr gameobject");
#endif

	if (nullptr != ptr_object)
	{
		auto iter = map_object_list_.find(object_key);
		if (iter == map_object_list_.end())
			map_object_list_[object_key] = ObjectList();

		map_object_list_[object_key].emplace_back(ptr_object);
	}
	return S_OK;
}

void Engine::CLayer::Update(float delta_time)
{
	for (auto& pair : map_object_list_)
	{
		auto iter = pair.second.begin();
		auto iter_end = pair.second.end();

		for (; iter != iter_end; )
		{
			if (true == (*iter)->destroy())
				iter = pair.second.erase(iter);
			else if (false == (*iter)->active())
				continue;
			else
			{
				(*iter)->Update(delta_time);
				++iter;
			}
		}
	}
}

Engine::CLayer * Engine::CLayer::Create()
{
	return new CLayer;
}

void Engine::CLayer::Release()
{
	for (auto& pair : map_object_list_)
	{
		for (auto& object : pair.second)
			Safe_Delete(object);

		pair.second.clear();
	}
	map_object_list_.clear();
}
