#pragma once

#include "Engine_Include.h"
#include "Engine_Singleton.h"
#include "Scene.h"

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
	T* GetComponent(int layer_id
		, const std::wstring& object_key, const std::wstring & component_key) const;
	void AddRenderLayer(RENDERLAYER render_id, CGameObject* ptr_object);

public:
	CScene* GetCurrentScene() const;

public:
	CComponent* FindComponent(int container_index, const std::wstring& component_key);
	CComponent* CloneComponent(int container_index, const std::wstring& component_key);
	CGameObject* FindObject(int layer_id, const std::wstring& object_key);

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
	int MoveFromNavMesh(Vector3& pos, const Vector3& dir, int current_index, int& out_pass_fail_option);
	int MoveFromNavMesh(Vector3& pre_pos, const Vector3& next_pos, int current_index);
	int MoveFromNavMesh(Vector3& pos, const Vector3& dir, int current_index, int& out_pass_fail_option, Vector2& out_line_normal);
	int FindCellIndex(const Vector3& pos);
	void ClearNavCell();

public:
	bool PathFinder(int start_cell, int end_cell, std::vector<Vector3>& path);
	bool PathFinder(int start_cell, const Vector3& end_point, std::vector<Vector3>& path);

public:
	HRESULT AddLight(const D3DLIGHT9& light_info);
	void ClearLight();

private:
	CScene* ptr_scene_ = nullptr;
	CScene* ptr_next_scene_ = nullptr;
	class CRenderer* ptr_renderer_ = nullptr;
	class CComponentManager* ptr_component_manager_ = nullptr;
	class CNavMeshAgent* ptr_nav_mesh_agent_ = nullptr;

private:
	LPDIRECT3DDEVICE9 ptr_device_;

private:
	class CLightManager* ptr_light_manager_ = nullptr;
};

template<typename T>
inline T * CGameManager::GetComponent(int layer_id, const std::wstring & object_key, const std::wstring & component_key) const
{
	T* ptr_component = dynamic_cast<T*>(ptr_scene_->GetComponentToLayerObject(layer_id, object_key, component_key));
	if (nullptr == ptr_component)
		return nullptr;

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