#include "Layer.h"
#include "GameObject.h"

Engine::CLayer::CLayer()
{
}

Engine::CLayer::~CLayer()
{
	Release();
}

const Engine::CComponent * Engine::CLayer::GetComponent(int layer_id, const std::wstring & object_key, const std::wstring & component_key)
{
	auto layer_iter = object_layer_.find(layer_id);
	if (layer_iter == object_layer_.end())
		return nullptr;

	auto map_obj_list_iter = layer_iter->second.find(object_key);
	if (map_obj_list_iter == layer_iter->second.end())
		return nullptr;

	for (auto& object : map_obj_list_iter->second)
	{
		const CComponent* ptr_component = object->GetComponent(component_key);
		if (nullptr != ptr_component)
			return ptr_component;
	}
	return nullptr;
}

HRESULT Engine::CLayer::AddObject(int layer_id, const std::wstring & object_key, Engine::CGameObject * ptr_object)
{
	assert(nullptr != ptr_object && "AddObject is nullptr gameobject");

	if (nullptr != ptr_object)
	{
		auto layer_iter = object_layer_.find(layer_id);
		if (layer_iter == object_layer_.end())
			object_layer_.emplace(layer_id, MapObjectList());

		auto map_obj_list_iter = object_layer_[layer_id].find(object_key);
		if (map_obj_list_iter == object_layer_[layer_id].end())
			object_layer_[layer_id].emplace(object_key, ObjectList());

		object_layer_[layer_id][object_key].emplace_back(ptr_object);
	}
	return S_OK;
}

void Engine::CLayer::Update(float delta_time)
{
	for (auto& layer_pair : object_layer_)
	{
		for (auto& pair : layer_pair.second)
		{
			for (auto& object : pair.second)
				object->Update(delta_time);
		}
	}
}

void Engine::CLayer::LateUpdate()
{
	for (auto& layer_pair : object_layer_)
	{
		for (auto& pair : layer_pair.second)
		{
			for (auto& object : pair.second)
				object->LateUpdate();
		}
	}
}

void Engine::CLayer::LastFrame()
{
	for (auto& layer_pair : object_layer_)
	{
		for (auto& pair : layer_pair.second)
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
	for (auto& layer_pair : object_layer_)
	{
		for (auto& pair : layer_pair.second)
		{
			for (auto& object : pair.second)
				Safe_Delete(object);

			pair.second.clear();
		}
		layer_pair.second.clear();
	}
	object_layer_.clear();
}
