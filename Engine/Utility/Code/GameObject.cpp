#include "GameObject.h"
#include "Transform.h"

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

void Engine::CGameObject::ComputeViewZ(const Matrix & mat_view)
{

}

const float Engine::CGameObject::view_z() const
{
	return view_z_;
}

void Engine::CGameObject::Update(float delta_time)
{
	for (auto& pair : map_component_)
		pair.second->Update(delta_time);
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