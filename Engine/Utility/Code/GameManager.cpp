#include "GameManager.h"
#include "Scene.h"
#include "Renderer.h"
#include "ComponentManager.h"

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

Engine::CComponent * Engine::CGameManager::FindComponent(int container_index, const std::wstring & component_key)
{
	return ptr_component_manager_->FindComponent(container_index, component_key);
}

Engine::CComponent * Engine::CGameManager::CloneComponent(int container_index, const std::wstring & component_key)
{
	return ptr_component_manager_->CloneComponent(container_index, component_key);
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

	ptr_renderer_ = CRenderer::Create(ptr_device);
	assert(nullptr != ptr_renderer_ && "Renderer Create Failed");

	ptr_component_manager_ = CComponentManager::Create();
	assert(nullptr != ptr_component_manager_ && "Renderer Create Failed");

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

void Engine::CGameManager::Release()
{
	Safe_Delete(ptr_renderer_);
	Safe_Delete(ptr_scene_);
	Safe_Delete(ptr_component_manager_);
}
