#pragma once

#include "Engine_Include.h"
#include "Engine_Singleton.h"
#include "Renderer.h"

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

private:
	LPDIRECT3DDEVICE9 ptr_device_;
};

template<typename T>
HRESULT Engine::CGameManager::SceneChange(T& Functor)
{
	if (ptr_scene_ != nullptr)
		Safe_Delete(ptr_scene_);

	HRESULT hr = Functor(&ptr_scene_, ptr_device_);
	assert(hr == S_OK && "SceneChange Error");

	return S_OK;
}

END