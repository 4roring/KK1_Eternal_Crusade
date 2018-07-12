#pragma once

#include "Engine_Include.h"

BEGIN(Engine)

class CLayer;
class CComponent;
class CGameObject;

class ENGINE_DLL CScene
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CScene();

public:
	CComponent* GetComponentToLayerObject(int layer_id, const std::wstring& object_key, const std::wstring& component_key) const;

public:
	virtual HRESULT InitScene();
	virtual HRESULT LateInit();
	virtual void Update(float delta_time) PURE;
	virtual void Render();
	virtual void LastFrame();

public:
	void AddObject(int layer_id, const std::wstring& object_key, CGameObject* ptr_object);
	
private:
	void Release();

protected:
	LPDIRECT3DDEVICE9 ptr_device_ = nullptr;

private:
	CLayer* ptr_layer_ = nullptr;
};

END