#pragma once

#include "Engine_Include.h"

BEGIN(Engine)

class CComponent;

class CComponentMap
{
private:
	CComponentMap();

public:
	~CComponentMap();

public:
	const CComponent* GetComponent(const std::wstring& component_key);

public:
	void AddComponent(const std::wstring& instance_key, CComponent* ptr_component);

public:
	void Update(float delta_time);

private:
	void Release();

public:
	static CComponentMap* Create();

private:
	std::unordered_map<std::wstring, CComponent*> map_component_;
};

END