#include "Renderer.h"
#include "GameObject.h"
#include "G_Buffer.h"

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
	assert(nullptr != ptr_renderer && "Renderer Create Failed");

	return ptr_renderer;
}

HRESULT Engine::CRenderer::Initialize()
{
	// GBuffer 积己
	HRESULT hr = E_FAIL;
	D3DVIEWPORT9 view_port = {};
	ptr_device_->GetViewport(&view_port);

	hr = ptr_g_buffer->AddTargetBuffer(ptr_device_, TEXT("Albedo"), view_port.Width, view_port.Height, D3DFMT_A8B8G8R8, D3DXCOLOR(0.f, 0.f, 0.f, 1.f));

	hr = ptr_g_buffer->AddTargetBuffer(ptr_device_, TEXT("Normal"), view_port.Width, view_port.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f));

	return S_OK;
}

void Engine::CRenderer::RenderGroupRelease()
{
	Release();
}

void Engine::CRenderer::Render()
{
	Render_Priority();
	Render_Deferred();
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

void Engine::CRenderer::Render_Deferred()
{
	// GBuffer肺 何磐 Albedo, Normal, Depth 弊府扁.

	Render_NoneAlpha();
}
