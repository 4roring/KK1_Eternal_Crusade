#pragma once

#include "Engine_Include.h"
#include "Engine_Singleton.h"

BEGIN(Engine)

class CComponent;

class CComponentManager
{
private:
	CComponentManager();

public:
	~CComponentManager();

public:
	CComponent* FindComponent(int container_index, const std::wstring& component_key);
	CComponent* CloneComponent(int container_index, const std::wstring& component_key);

public:
	HRESULT InitComponentManager(const int container_size);
	HRESULT Add_Prototype(int container_index, std::wstring component_key, CComponent* ptr_component);

public:
	void PrototypeClearances(int container_index);

public:
	static CComponentManager* Create();

private:
	void Release();

private:
	int container_size_ = 0;

private:
	typedef std::unordered_map<std::wstring, CComponent*> MapComponent;
	MapComponent* ptr_map_component_ = nullptr;
};

END