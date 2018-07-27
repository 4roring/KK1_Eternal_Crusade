#include "GameObject.h"
#include "Component.h"
#include "ComponentMap.h"

Engine::CGameObject::CGameObject(LPDIRECT3DDEVICE9 ptr_device)
	: ptr_device_(ptr_device)
{
}

Engine::CGameObject::~CGameObject()
{
	Release();
}

Engine::CComponent * Engine::CGameObject::GetComponent(const std::wstring & component_key) const
{
	return ptr_component_map_->GetComponent(component_key);
}

void Engine::CGameObject::AddComponent(const std::wstring & instance_key, CComponent * ptr_component)
{
	ptr_component_map_->AddComponent(instance_key, ptr_component);
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

HRESULT Engine::CGameObject::Initialize()
{
	ptr_component_map_ = CComponentMap::Create();
	if (nullptr == ptr_component_map_) return E_FAIL;

	return S_OK;
}

void Engine::CGameObject::LateInit()
{
}

void Engine::CGameObject::Update(float delta_time)
{
	ptr_component_map_->Update(delta_time);
}

void Engine::CGameObject::LateUpdate()
{
}

void Engine::CGameObject::Render()
{
}

void Engine::CGameObject::ApplyDamage(int damage)
{
}

bool Engine::CGameObject::CheckRaycast(const Vector3 & ray_pos, const Vector3 & ray_dir, float * hit_dist)
{
	return false;
}

void Engine::CGameObject::Release()
{
	Safe_Delete(ptr_component_map_);
}