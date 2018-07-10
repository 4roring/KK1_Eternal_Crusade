#pragma once

#include "Engine_Include.h"
#include "Engine_Singleton.h"

BEGIN(Engine)

class CScene;
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
	template<typename T>
	const T* GetComponent(int layer_id
		, const std::wstring& object_key, const std::wstring component_key);
	void AddRenderLayer(RENDERLAYER render_id, CGameObject* ptr_object);

public:
	template<typename T>
	const CComponent* GetComponent(int container_index, const std::wstring& component_key);
	CComponent* FindComponent(int container_index, const std::wstring& component_key);
	CComponent* CloneComponent(int container_index, const std::wstring& component_key);

public:
	HRESULT InitComponentManager(const int container_size);
	HRESULT Add_Prototype(int container_index, const std::wstring& component_key, CComponent* ptr_component);
	void PrototypeClearances(int container_index);

public:
	void RenderGroupRelease();

public:
	HRESULT InitManager(LPDIRECT3DDEVICE9 ptr_device);
	void Update(float delta_time);
	void Render();
	void LastFrame();

private:
	void Release();

#ifdef _OLD_SCENE_SYSTEM
public:
	template<typename T>
	HRESULT SceneChange(T& functor);
#endif

public:
	HRESULT SetNextScene(CScene* ptr_scene);

public:
	HRESULT Create_NavMeshAgent(int cell_container_size);
	HRESULT AddNavCell(const Vector3& point_a, const Vector3& point_b, const Vector3& point_c
		, int index, int option, int link_cell_index);
	void LinkCell();
	int MoveFromNavMesh(Vector3& pos, const Vector3& dir, int current_index, int out_pass_fail_option = -1);
	void ClearNavCell();

private:
	CScene* ptr_scene_ = nullptr;
	CScene* ptr_next_scene_ = nullptr;
	class CRenderer* ptr_renderer_ = nullptr;
	class CComponentManager* ptr_component_manager_ = nullptr;
	class CNavMeshAgent* ptr_nav_mesh_agent_ = nullptr;

private:
	LPDIRECT3DDEVICE9 ptr_device_;
};

template<typename T>
inline const T * CGameManager::GetComponent(int layer_id, const std::wstring & object_key, const std::wstring component_key)
{
	const CComponent* ptr_component = FindComponent(container_index, component_key);
	if (ptr_component == nullptr)
		return nullptr;

	dynamic_cast<T>(ptr_component);
	assert(nullptr != ptr_component && "GetComponent Dynamic_cast Error");

	return ptr_component;
}

#ifdef _OLD_SCENE_SYSTEM
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
#endif


END