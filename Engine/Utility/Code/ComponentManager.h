#pragma once

#include "Engine_Include.h"
#include "Engine_Singleton.h"

BEGIN(Engine)

class CComponent;

class ENGINE_DLL CComponentManager
	: public CSingleton<CComponentManager>
{
private:
	friend CSingleton;
	CComponentManager();
	virtual ~CComponentManager();

public:
	template<typename T>
	const CComponent* GetComponent(int container_index, const std::wstring& component_key);
	CComponent* FindComponent(int container_index, const std::wstring& component_key);
	CComponent* CloneComponent(int container_index, const std::wstring& component_key);

public:
	HRESULT InitComponentManager(const int container_size);
	HRESULT Add_Prototype(int container_index, std::wstring component_key, CComponent* ptr_component);

public:
	void PrototypeClearances(int container_index);

private:
	void Release();

private:
	int container_size_ = 0;

private:
	typedef std::map<std::wstring, CComponent*> MapComponent;
	MapComponent* ptr_map_component_ = nullptr;
};

template <typename T>
const Engine::CComponent * Engine::CComponentManager::GetComponent(int container_index, const std::wstring & component_key)
{
	const CComponent* ptr_component = FindComponent(container_index, component_key);
	if (ptr_component == nullptr)
		return nullptr;

	dynamic_cast<T>(ptr_component);
	assert(nullptr != ptr_component && "GetComponent Dynamic_cast Error");

	return ptr_component;
}

END