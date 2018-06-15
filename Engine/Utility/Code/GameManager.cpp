#include "GameManager.h"
#include "Scene.h"
#include "Renderer.h"

Engine::CGameManager::CGameManager()
{
}

Engine::CGameManager::~CGameManager()
{
	Release();
}

const Engine::CComponent * Engine::CGameManager::GetComponent(int layer_id, const std::wstring & object_key, const std::wstring component_key)
{
	return ptr_scene_->GetComponent(layer_id, object_key, component_key);
}

void Engine::CGameManager::AddRenderLayer(RENDERLAYER render_id, class CGameObject * ptr_object)
{
	ptr_renderer_->AddRenderLayer(render_id, ptr_object);
}

HRESULT Engine::CGameManager::InitManager(LPDIRECT3DDEVICE9 ptr_device)
{
	ptr_device_ = ptr_device;

	ptr_renderer_ = CRenderer::Create(ptr_device);
	assert(nullptr != ptr_renderer_ && "Renderer Create Failed");

	return S_OK;
}

void Engine::CGameManager::Update(float delta_time)
{
	assert(nullptr != ptr_scene_ && "Scene is nullptr");
	ptr_scene_->Update(delta_time);
}

void Engine::CGameManager::Render()
{
	assert(nullptr != ptr_renderer_ && "Renderer is nullptr");
	ptr_renderer_->Render();
	assert(nullptr != ptr_scene_ && "Scene is nullptr");
	ptr_scene_->Render();
}

void Engine::CGameManager::Release()
{
	Safe_Delete(ptr_renderer_);
	Safe_Delete(ptr_scene_);
}
