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
	CComponent* GetComponent(int layer_id, const std::wstring& object_key, const std::wstring& component_key) const;
	CGameObject* FindObject(int layer_id, const std::wstring& object_key) const;

public:
	HRESULT AddObject(int layer_id, const std::wstring& object_key, CGameObject* ptr_object);

public:
	void LateInit();
	void Update(float delta_time);
	void LateUpdate();
	void LastFrame();

public:
	static CLayer* Create();

public:
	void Release();

private:
	typedef std::unordered_map<std::wstring, CGameObject*> MapObject;
	std::unordered_map<int, MapObject> object_layer_;
};

END