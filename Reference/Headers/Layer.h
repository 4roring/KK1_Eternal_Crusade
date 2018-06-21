#pragma once

#include "Engine_Include.h"

BEGIN(Engine)

class CGameObject;
class CComponent;

class CLayer
{
private:
	CLayer();

public:
	~CLayer();

public:
	const CComponent* GetComponent(int layer_id, const std::wstring& object_key, const std::wstring& component_key);

public:
	HRESULT AddObject(int layer_id, const std::wstring& object_key, CGameObject* ptr_object);

public:
	void Update(float delta_time);

public:
	static CLayer* Create();

public:
	void Release();

private:
	typedef std::list<CGameObject*> ObjectList;
	typedef std::unordered_map<std::wstring, ObjectList> MapObjectList;
	std::unordered_map<int, MapObjectList> object_layer_;
};

END