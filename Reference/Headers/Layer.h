#pragma once

#include "Engine_Include.h"

BEGIN(Engine)

class CGameObject;
class CComponent;

class ENGINE_DLL CLayer
{
private:
	CLayer();

public:
	~CLayer();

public:
	const CComponent* GetComponent(const std::wstring& object_key, const std::wstring& component_key);

public:
	HRESULT AddObject(const std::wstring& object_key, CGameObject* ptr_object);

public:
	void Update(float delta_time);

public:
	static CLayer* Create();

public:
	void Release();

private:
	typedef std::list<CGameObject*> ObjectList;
	typedef std::map<std::wstring, ObjectList> MapObjectList;
	MapObjectList map_object_list_;
};

END