#include "Layer.h"
#include "GameObject.h"

Engine::CLayer::CLayer()
{
}

Engine::CLayer::~CLayer()
{
	Release();
}

Engine::CComponent * Engine::CLayer::GetComponent(int layer_id, const std::wstring & object_key, const std::wstring & component_key) const
{
	auto layer_iter = object_layer_.find(layer_id);
	if (layer_iter == object_layer_.end())
		return nullptr;

	auto map_obj_iter = layer_iter->second.find(object_key);
	if (map_obj_iter == layer_iter->second.end())
		return nullptr;

	CComponent* ptr_component = map_obj_iter->second->GetComponent(component_key);
	if (nullptr == ptr_component)
		return nullptr;
	
	return ptr_component;
}

Engine::CGameObject * Engine::CLayer::FindObject(int layer_id, const std::wstring & object_key) const
{
	auto layer_iter = object_layer_.find(layer_id);
	if (layer_iter == object_layer_.end())
		return nullptr;

	auto map_obj_iter = layer_iter->second.find(object_key);
	if (map_obj_iter == layer_iter->second.end())
		return nullptr;

	return map_obj_iter->second;
}

HRESULT Engine::CLayer::AddObject(int layer_id, const std::wstring & object_key, Engine::CGameObject * ptr_object)
{
	assert(nullptr != ptr_object && "AddObject is nullptr gameobject");

	if (nullptr != ptr_object)
	{
		auto layer_iter = object_layer_.find(layer_id);
		if (layer_iter == object_layer_.end())
			object_layer_.emplace(layer_id, MapObject());

		auto map_obj_iter = object_layer_[layer_id].find(object_key);
		if (map_obj_iter == object_layer_[layer_id].end())
			object_layer_[layer_id].emplace(object_key, ptr_object);
		else
		{
			std::wstring obj_key = object_key + TEXT("_1");
			AddObject(layer_id, obj_key, ptr_object);
		}
	}
	return S_OK;
}

void Engine::CLayer::LateInit()
{
	for (auto& layer_pair : object_layer_)
	{
		for (auto& pair : layer_pair.second)
		{
			pair.second->LateInit();
		}
	}
}

void Engine::CLayer::Update(float delta_time)
{
	for (auto& layer_pair : object_layer_)
	{
		for (auto& pair : layer_pair.second)
		{
			if (true == pair.second->active())
				pair.second->Update(delta_time);
		}
	}
}

void Engine::CLayer::LateUpdate()
{
	for (auto& layer_pair : object_layer_)
	{
		for (auto& pair : layer_pair.second)
		{
			if(true == pair.second->active())
				pair.second->LateUpdate();
		}
	}
}

void Engine::CLayer::LastFrame()
{
	for (auto& layer_pair : object_layer_)
	{
		auto iter = layer_pair.second.begin();
		auto iter_end = layer_pair.second.end();

		for (; iter != iter_end; )
		{
			if (true == iter->second->destroy())
			{
				Safe_Delete(iter->second);
				iter = layer_pair.second.erase(iter);
			}
			else
				++iter;
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
			Safe_Delete(pair.second);
		layer_pair.second.clear();
	}
	object_layer_.clear();
}
