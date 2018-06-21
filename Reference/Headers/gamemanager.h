#pragma once

#include "Engine_Include.h"
#include "Engine_Singleton.h"

BEGIN(Engine)

class CComponent;
class CGameObject;

class ENGINE_DLL CGameManager
	: public CSingleton<CGameManager>
{
private:
	friend CSingleton;
	CGameManager();
	virtual ~CGameManager();

public:
	const CComponent* GetComponent(int layer_id
		, const std::wstring& object_key, const std::wstring component_key);
	void AddRenderLayer(RENDERLAYER render_id, CGameObject* ptr_object);

public:
	template<typename T>
	const CComponent* GetComponent(int container_index, const std::wstring& component_key);
	CComponent* FindComponent(int container_index, const std::wstring& component_key);
	CComponent* CloneComponent(int container_index, const std::wstring& component_key);

public:
	HRESULT InitComponentManager(const int container_size);
	HRESULT Add_Prototype(int container_index, std::wstring component_key, CComponent* ptr_component);
	void PrototypeClearances(int container_index);

public:
	void RenderGroupRelease();

public:
	HRESULT InitManager(LPDIRECT3DDEVICE9 ptr_device);
	void Update(float delta_time);
	void Render();

public:
	template<typename T>
	HRESULT SceneChange(T& functor);

private:
	void Release();

private:
	class CScene* ptr_scene_ = nullptr;
	class CRenderer* ptr_renderer_ = nullptr;
	class CComponentManager* ptr_component_manager_ = nullptr;

private:
	LPDIRECT3DDEVICE9 ptr_device_;
};

template<typename T>
inline const CComponent * CGameManager::GetComponent(int container_index, const std::wstring & component_key)
{
	const CComponent* ptr_component = FindComponent(container_index, component_key);
	if (ptr_component == nullptr)
		return nullptr;

	dynamic_cast<T>(ptr_component);
	assert(nullptr != ptr_component && "GetComponent Dynamic_cast Error");

	return ptr_component;
}

template<typename T>
inline HRESULT Engine::CGameManager::SceneChange(T& Functor)
{
	if (nullptr != ptr_scene_)
		Safe_Delete(ptr_scene_);

	HRESULT hr = Functor(&ptr_scene_, ptr_device_);
	assert(hr == S_OK && "SceneChange Error");
	RenderGroupRelease();

	return S_OK;
}


END