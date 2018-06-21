#include "Scene.h"
#include "Layer.h"

Engine::CScene::CScene(LPDIRECT3DDEVICE9 ptr_device)
	: ptr_device_(ptr_device)
{
}

Engine::CScene::~CScene()
{
	Release();
}

const Engine::CComponent * Engine::CScene::GetComponent(int layer_id, const std::wstring & object_key, const std::wstring component_key)
{
	return ptr_layer_->GetComponent(layer_id, object_key, component_key);
}

HRESULT Engine::CScene::InitScene()
{
	ptr_layer_ = CLayer::Create();
	
	if (nullptr == ptr_layer_)
		return E_FAIL;

	return S_OK;
}

void Engine::CScene::Update(float delta_time)
{
	ptr_layer_->Update(delta_time);
}

void Engine::CScene::Render()
{
}

void Engine::CScene::AddObject(int layer_id, const std::wstring & object_key, CGameObject * ptr_object)
{
	ptr_layer_->AddObject(layer_id, object_key, ptr_object);
}

void Engine::CScene::Release()
{
	Safe_Delete(ptr_layer_);
}
