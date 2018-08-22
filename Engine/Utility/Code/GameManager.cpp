#include "GameManager.h"
#include "Scene.h"
#include "Renderer.h"
#include "ComponentManager.h"
#include "NavMeshAgent.h"
#include "LightManager.h"

Engine::CGameManager::CGameManager()
{
}

Engine::CGameManager::~CGameManager()
{
	Release();
}

void Engine::CGameManager::AddRenderLayer(RENDERLAYER render_id, class CGameObject * ptr_object)
{
	ptr_renderer_->AddRenderLayer(render_id, ptr_object);
}

Engine::CScene * Engine::CGameManager::GetCurrentScene() const
{
	return ptr_scene_;
}

Engine::CComponent * Engine::CGameManager::FindComponent(int container_index, const std::wstring & component_key)
{
	return ptr_component_manager_->FindComponent(container_index, component_key);
}

Engine::CComponent * Engine::CGameManager::CloneComponent(int container_index, const std::wstring & component_key)
{
	return ptr_component_manager_->CloneComponent(container_index, component_key);
}

Engine::CGameObject * Engine::CGameManager::FindObject(int layer_id, const std::wstring & object_key)
{
	return ptr_scene_->FindObjectInLayer(layer_id, object_key);
}

HRESULT Engine::CGameManager::InitComponentManager(const int container_size)
{
	return ptr_component_manager_->InitComponentManager(container_size);
}

HRESULT Engine::CGameManager::Add_Prototype(int container_index, const std::wstring& component_key, CComponent * ptr_component)
{
	return ptr_component_manager_->Add_Prototype(container_index, component_key, ptr_component);
}

void Engine::CGameManager::PrototypeClearances(int container_index)
{
	ptr_component_manager_->PrototypeClearances(container_index);
}

void Engine::CGameManager::RenderGroupRelease()
{
	ptr_renderer_->RenderGroupRelease();
}

HRESULT Engine::CGameManager::InitManager(LPDIRECT3DDEVICE9 ptr_device)
{
	ptr_device_ = ptr_device;

	ptr_component_manager_ = CComponentManager::Create();
	assert(nullptr != ptr_component_manager_ && "Renderer Create Failed");

	ptr_light_manager_ = CLightManager::GetInstance();
	assert(nullptr != ptr_component_manager_ && "LightManager Instance is nullptr");

	ptr_renderer_ = CRenderer::Create(ptr_device);
	assert(nullptr != ptr_renderer_ && "Renderer Create Failed");

	return S_OK;
}

void Engine::CGameManager::Update(float delta_time)
{
	if (nullptr == ptr_scene_) return;
	ptr_scene_->Update(delta_time);
}

void Engine::CGameManager::Render()
{
	if (nullptr == ptr_scene_) return;
	ptr_renderer_->Render();

#ifdef _DEBUG
	if(nullptr != ptr_scene_)
		ptr_scene_->Render();
	//if (nullptr != ptr_nav_mesh_agent_)
	//	ptr_nav_mesh_agent_->Debug_Render();
#endif	
}

void Engine::CGameManager::LastFrame()
{
	if (nullptr != ptr_scene_)
		ptr_scene_->LastFrame();

	if (ptr_next_scene_ != ptr_scene_)
	{
		Safe_Delete(ptr_scene_);
		ptr_scene_ = ptr_next_scene_;
		ptr_scene_->LateInit();
	}
}

HRESULT Engine::CGameManager::SetNextScene(CScene * ptr_scene)
{
	if (nullptr == ptr_scene) return E_FAIL;
	ptr_next_scene_ = ptr_scene;

	if (nullptr == ptr_scene_)
		ptr_scene_ = ptr_next_scene_;

	return S_OK;
}

HRESULT Engine::CGameManager::Create_NavMeshAgent(int cell_container_size)
{
	if (nullptr == ptr_device_) return E_FAIL;

	if (ptr_nav_mesh_agent_ != nullptr)
		Safe_Delete(ptr_nav_mesh_agent_);

	ptr_nav_mesh_agent_ = CNavMeshAgent::Create(ptr_device_, cell_container_size);
	if (nullptr == ptr_nav_mesh_agent_) return E_FAIL;

	return S_OK;
}

HRESULT Engine::CGameManager::AddNavCell(const Vector3 & point_a, const Vector3 & point_b, const Vector3 & point_c, int index, int option, int link_cell_index)
{
	return ptr_nav_mesh_agent_->AddNavCell(point_a, point_b, point_c, index, option, link_cell_index);
}

void Engine::CGameManager::LinkCell()
{
	ptr_nav_mesh_agent_->LinkCell();
}

int Engine::CGameManager::MoveFromNavMesh(Vector3 & pos, const Vector3 & dir, int current_index, int& out_pass_fail_option)
{
	return ptr_nav_mesh_agent_->MoveFromNavMesh(pos, dir, current_index, out_pass_fail_option);
}

int Engine::CGameManager::MoveFromNavMesh(Vector3 & pre_pos, const Vector3 & next_pos, int current_index)
{
	return ptr_nav_mesh_agent_->MoveFromNavMesh(pre_pos, next_pos, current_index);
}

int Engine::CGameManager::MoveFromNavMesh(Vector3 & pos, const Vector3 & dir, int current_index, int & out_pass_fail_option, Vector2 & out_line_normal)
{
	return ptr_nav_mesh_agent_->MoveFromNavMesh(pos, dir, current_index, out_pass_fail_option, out_line_normal);
}

int Engine::CGameManager::FindCellIndex(const Vector3 & pos)
{
	return ptr_nav_mesh_agent_->FindCellIndex(pos);
}

void Engine::CGameManager::ClearNavCell()
{
	Safe_Delete(ptr_nav_mesh_agent_);
}

bool Engine::CGameManager::PathFinder(int start_cell, int end_cell, std::vector<Vector3>& path)
{
	return ptr_nav_mesh_agent_->PathFinder(start_cell, end_cell, path);
}

bool Engine::CGameManager::PathFinder(int start_cell, const Vector3 & end_point, std::vector<Vector3>& path)
{
	return ptr_nav_mesh_agent_->PathFinder(start_cell, end_point, path);
}

HRESULT Engine::CGameManager::AddLight(const D3DLIGHT9 & light_info)
{
	return ptr_light_manager_->AddLight(ptr_device_, light_info);
}

void Engine::CGameManager::ClearLight()
{
	if(ptr_light_manager_ != nullptr)
		ptr_light_manager_->ClearLight();
}

void Engine::CGameManager::Release()
{
	ptr_light_manager_->DestroyInstance();
	ptr_light_manager_ = nullptr;
	Safe_Delete(ptr_nav_mesh_agent_);
	Safe_Delete(ptr_renderer_);
	Safe_Delete(ptr_scene_);
	Safe_Delete(ptr_component_manager_);
}
