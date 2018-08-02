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

Engine::CComponent * Engine::CScene::GetComponentToLayerObject(int layer_id, const std::wstring & object_key, const std::wstring & component_key) const
{
	return ptr_layer_->GetComponent(layer_id, object_key, component_key);
}

Engine::CGameObject * Engine::CScene::FindObjectInLayer(int layer_id, const std::wstring & object_key)
{
	return ptr_layer_->FindObject(layer_id, object_key);
}

HRESULT Engine::CScene::InitScene()
{
	ptr_layer_ = CLayer::Create();
	
	if (nullptr == ptr_layer_)
		return E_FAIL;

	return S_OK;
}

HRESULT Engine::CScene::LateInit()
{
	ptr_layer_->LateInit();

	return S_OK;
}

void Engine::CScene::Update(float delta_time)
{
	ptr_layer_->Update(delta_time);
	ptr_layer_->LateUpdate();
}

void Engine::CScene::Render()
{
}

void Engine::CScene::LastFrame()
{
	ptr_layer_->LastFrame();
}

void Engine::CScene::AddObject(int layer_id, const std::wstring & object_key, CGameObject * ptr_object)
{
	ptr_layer_->AddObject(layer_id, object_key, ptr_object);
}

void Engine::CScene::Release()
{
	Safe_Delete(ptr_layer_);
}
