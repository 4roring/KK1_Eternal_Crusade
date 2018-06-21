#include "Renderer.h"
#include "GameObject.h"

Engine::CRenderer::CRenderer(LPDIRECT3DDEVICE9 ptr_device)
	: ptr_device_(ptr_device)
{
}

Engine::CRenderer::~CRenderer()
{
	Release();
}

Engine::CRenderer * Engine::CRenderer::Create(LPDIRECT3DDEVICE9 ptr_device)
{
	CRenderer* ptr_renderer = new CRenderer(ptr_device);
#ifdef _DEBUG
	assert(nullptr != ptr_renderer && "Renderer Create Failed");
#endif
	return ptr_renderer;
}

void Engine::CRenderer::RenderGroupRelease()
{
	Release();
}

void Engine::CRenderer::Render()
{
	Render_Priority();
	Render_NoneAlpha();
	Render_Alpha();
	Render_UI();

	Release();
}

void Engine::CRenderer::Release()
{
	for (int i = 0; i < LAYER_END; ++i)
		render_layer_[i].clear();
}

void Engine::CRenderer::AddRenderLayer(RENDERLAYER render_id, CGameObject * ptr_object)
{
	assert(nullptr != ptr_object && "AddRenderLayer Error(object null)");
	render_layer_[render_id].emplace_back(ptr_object);
}

void Engine::CRenderer::Render_Priority()
{
	ptr_device_->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	ptr_device_->SetRenderState(D3DRS_ZENABLE, FALSE);

	for (auto& object : render_layer_[LAYER_PRIORITY])
		object->Render();

	ptr_device_->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	ptr_device_->SetRenderState(D3DRS_ZENABLE, TRUE);
}

void Engine::CRenderer::Render_NoneAlpha()
{
	for (auto& object : render_layer_[LAYER_NONEALPHA])
		object->Render();
}

void Engine::CRenderer::Render_Alpha()
{
}

void Engine::CRenderer::Render_UI()
{
	ptr_device_->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	ptr_device_->SetRenderState(D3DRS_ZENABLE, FALSE);

	for (auto& object : render_layer_[LAYER_UI])
		object->Render();

	ptr_device_->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	ptr_device_->SetRenderState(D3DRS_ZENABLE, TRUE);
}
