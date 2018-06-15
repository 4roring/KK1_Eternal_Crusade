#include "GameObject.h"
#include "Component.h"

Engine::CGameObject::CGameObject(LPDIRECT3DDEVICE9 ptr_device)
	: ptr_device_(ptr_device)
{
}

Engine::CGameObject::~CGameObject()
{
	Release();
}

const Engine::CComponent * Engine::CGameObject::GetComponent(const std::wstring & component_key)
{
	auto iter = map_component_.find(component_key);
	if (iter == map_component_.end())
		return nullptr;

	return iter->second;
}

void Engine::CGameObject::SetActive(bool active)
{
	active_ = active;
}

void Engine::CGameObject::Destroy()
{
	destroy_ = true;
}

void Engine::CGameObject::ComputeViewZ(const Matrix & mat_view)
{

}

float Engine::CGameObject::view_z() const
{
	return view_z_;
}

bool Engine::CGameObject::active() const
{
	return active_;
}

bool Engine::CGameObject::destroy() const
{
	return destroy_;
}

Engine::CTransform * Engine::CGameObject::transform()
{
	return ptr_transform_;
}

void Engine::CGameObject::Update(float delta_time)
{
	for (auto& pair : map_component_)
	{
		if (false == pair.second->enable_)
			continue;
		pair.second->Update(delta_time);
	}
}

void Engine::CGameObject::Render()
{
}

void Engine::CGameObject::Release()
{
	for (auto& pair : map_component_)
	{
		int reference_count = pair.second->Release();
		if (reference_count == 0)
			Safe_Delete(pair.second);
	}
	map_component_.clear();
}