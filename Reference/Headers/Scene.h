#pragma once

#include "Engine_Include.h"

BEGIN(Engine)

class CLayer;
class CComponent;

class ENGINE_DLL CScene
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CScene();

public:
	const CComponent* GetComponent(int layer_id, const std::wstring& object_key, const std::wstring component_key);

public:
	virtual HRESULT InitScene() PURE;
	virtual void Update(float delta_time) PURE;

private:
	void Release();

protected:
	LPDIRECT3DDEVICE9 ptr_device_ = nullptr;

protected:
	std::unordered_map<int, CLayer*> map_layer_;
};

END