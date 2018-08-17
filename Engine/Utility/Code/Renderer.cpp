#include "Renderer.h"
#include "GameObject.h"
#include "G_Buffer.h"
#include "LightManager.h"
#include "Shader.h"
#include "ViewTexture.h"

Engine::GBuffer* Engine::GBuffer::ptr_single_instance_ = nullptr;

Engine::CRenderer::CRenderer(LPDIRECT3DDEVICE9 ptr_device)
	: ptr_device_(ptr_device)
{
}

Engine::CRenderer::~CRenderer()
{
	Release();
}

HRESULT Engine::CRenderer::Initialize()
{
	// GBuffer »ý¼º
	HRESULT hr = E_FAIL;

	ptr_gbuffer = GBuffer::Create(ptr_device_);
	if (nullptr == ptr_gbuffer) return E_FAIL;

	ptr_shader_shade_ = CShader::Create(ptr_device_, TEXT("../../Reference/Shader/Deferred/Deferred_LightAcc.hlsl"));
	if (nullptr == ptr_shader_shade_) return E_FAIL;
	ptr_shader_blend_ = CShader::Create(ptr_device_, TEXT("../../Reference/Shader/Deferred/Deferred_Blend.hlsl"));
	if (nullptr == ptr_shader_blend_) return E_FAIL;

	D3DVIEWPORT9 view_port = {};
	ptr_device_->GetViewport(&view_port);

	// Deferred
	hr = ptr_gbuffer->AddTargetBuffer(ptr_device_, GBuffer::Target_Albedo, view_port.Width, view_port.Height, D3DFMT_A8B8G8R8, D3DXCOLOR(0.f, 0.f, 0.f, 0.f)); if (FAILED(hr)) return E_FAIL;
	hr = ptr_gbuffer->AddTargetBuffer(ptr_device_, GBuffer::Target_Normal, view_port.Width, view_port.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f)); if (FAILED(hr)) return E_FAIL;
	hr = ptr_gbuffer->AddTargetBuffer(ptr_device_, GBuffer::Target_Depth, view_port.Width, view_port.Height, D3DFMT_A32B32G32R32F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f)); if (FAILED(hr)) return E_FAIL;
	hr = ptr_gbuffer->AddTargetBuffer(ptr_device_, GBuffer::Target_Specular, view_port.Width, view_port.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f)); if (FAILED(hr)) return E_FAIL;

	//hr = ptr_gbuffer->Add_DebugTarget(GBuffer::Target_Albedo, 0.f, 0.f, 150.f, 150.f); if (FAILED(hr)) return E_FAIL;
	//hr = ptr_gbuffer->Add_DebugTarget(GBuffer::Target_Normal, 0.f, 150.f, 150.f, 150.f); if (FAILED(hr)) return E_FAIL;
	//hr = ptr_gbuffer->Add_DebugTarget(GBuffer::Target_Depth, 0.f, 300.f, 150.f, 150.f); if (FAILED(hr)) return E_FAIL;
	//hr = ptr_gbuffer->Add_DebugTarget(GBuffer::Target_Specular, 0.f, 450.f, 150.f, 150.f); if (FAILED(hr)) return E_FAIL;

	// LightAcc
	hr = ptr_gbuffer->AddTargetBuffer(ptr_device_, GBuffer::Target_Shade, view_port.Width, view_port.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f)); if (FAILED(hr)) return E_FAIL;
	hr = ptr_gbuffer->AddTargetBuffer(ptr_device_, GBuffer::Target_Light_Specular, view_port.Width, view_port.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f)); if (FAILED(hr)) return E_FAIL;
	//hr = ptr_gbuffer->Add_DebugTarget(GBuffer::Target_Shade, 150.f, 0.f, 150.f, 150.f); if (FAILED(hr)) return E_FAIL;
	//hr = ptr_gbuffer->Add_DebugTarget(GBuffer::Target_Light_Specular, 150.f, 150.f, 150.f, 150.f); if (FAILED(hr)) return E_FAIL;

	// MRT
	hr = ptr_gbuffer->AddMultiRenderTarget(GBuffer::MRT_DEFERRED, GBuffer::Target_Albedo); if (FAILED(hr)) return E_FAIL;
	hr = ptr_gbuffer->AddMultiRenderTarget(GBuffer::MRT_DEFERRED, GBuffer::Target_Normal); if (FAILED(hr)) return E_FAIL;
	hr = ptr_gbuffer->AddMultiRenderTarget(GBuffer::MRT_DEFERRED, GBuffer::Target_Depth); if (FAILED(hr)) return E_FAIL;
	hr = ptr_gbuffer->AddMultiRenderTarget(GBuffer::MRT_DEFERRED, GBuffer::Target_Specular); if (FAILED(hr)) return E_FAIL;

	hr = ptr_gbuffer->AddMultiRenderTarget(GBuffer::MRT_LIGHT, GBuffer::Target_Shade); if (FAILED(hr)) return E_FAIL;
	hr = ptr_gbuffer->AddMultiRenderTarget(GBuffer::MRT_LIGHT, GBuffer::Target_Light_Specular); if (FAILED(hr)) return E_FAIL;

	ptr_light_manager_ = CLightManager::GetInstance();
	ptr_blend_buffer_ = CViewTexture::Create(ptr_device_, 0.f, 0.f, (float)view_port.Width, (float)view_port.Height);

	return S_OK;
}

void Engine::CRenderer::Render()
{
	Render_Priority();
	Render_Deferred();
	Render_LightAcc();
	Render_Blend();
	Render_Alpha();
	Render_UI();

	//ptr_gbuffer->Render_DebugTarget(GBuffer::MRT_DEFERRED);
	//ptr_gbuffer->Render_DebugTarget(GBuffer::MRT_LIGHT);

	RenderGroupRelease();
}

Engine::CRenderer * Engine::CRenderer::Create(LPDIRECT3DDEVICE9 ptr_device)
{
	CRenderer* ptr_renderer = new CRenderer(ptr_device);
	if (FAILED(ptr_renderer->Initialize()))
	{
		Safe_Delete(ptr_renderer);
		assert(nullptr != ptr_renderer && "Renderer Create Failed");
	}

	return ptr_renderer;
}

void Engine::CRenderer::Release()
{
	RenderGroupRelease();
	Safe_Delete(ptr_gbuffer);
	Safe_Release_Delete(ptr_shader_blend_);
	Safe_Release_Delete(ptr_shader_shade_);
	Safe_Release_Delete(ptr_blend_buffer_);
}

void Engine::CRenderer::RenderGroupRelease()
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
	std::sort(render_layer_[LAYER_ALPHA].begin(), render_layer_[LAYER_ALPHA].end(),
		[](Engine::CGameObject* ptr_src, Engine::CGameObject* ptr_dst)->bool
	{
		return ptr_src->view_z() > ptr_dst->view_z();
	});

	for (auto& object : render_layer_[LAYER_ALPHA])
		object->Render();
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
	ptr_gbuffer->Begin_G_Buffer(GBuffer::MRT_DEFERRED);
	Render_NoneAlpha();
	ptr_gbuffer->End_G_Buffer(GBuffer::MRT_DEFERRED);
}

void Engine::CRenderer::Render_LightAcc()
{
	ptr_gbuffer->Begin_G_Buffer(GBuffer::MRT_LIGHT);

	LPD3DXEFFECT ptr_effect = ptr_shader_shade_->GetEffectHandle();

	if (FAILED(ptr_gbuffer->SetTargetOnShader(GBuffer::Target_Normal, ptr_effect, "g_normal_texture")))
		return;
	if (FAILED(ptr_gbuffer->SetTargetOnShader(GBuffer::Target_Depth, ptr_effect, "g_depth_texture")))
		return;
	if (FAILED(ptr_gbuffer->SetTargetOnShader(GBuffer::Target_Specular, ptr_effect, "g_specular_texture")))
		return;

	Matrix inv_mat_view, inv_mat_proj;
	ptr_device_->GetTransform(D3DTS_VIEW, &inv_mat_view);
	D3DXMatrixInverse(&inv_mat_view, nullptr, &inv_mat_view);

	ptr_device_->GetTransform(D3DTS_PROJECTION, &inv_mat_proj);
	D3DXMatrixInverse(&inv_mat_proj, nullptr, &inv_mat_proj);

	ptr_effect->SetMatrix("g_inv_mat_view", &inv_mat_view);
	ptr_effect->SetMatrix("g_inv_mat_proj", &inv_mat_proj);
	ptr_effect->SetVector("g_camera_position", (Vector4*)&inv_mat_view.m[3][0]);

	ptr_effect->Begin(nullptr, 0);

	ptr_light_manager_->Render_Light(ptr_effect);

	ptr_effect->End();

	ptr_gbuffer->End_G_Buffer(GBuffer::MRT_LIGHT);
}

void Engine::CRenderer::Render_Blend()
{
	LPD3DXEFFECT ptr_effect = ptr_shader_blend_->GetEffectHandle();

	if (FAILED(ptr_gbuffer->SetTargetOnShader(GBuffer::Target_Albedo, ptr_effect, "g_color_texture")))
		return;
	if (FAILED(ptr_gbuffer->SetTargetOnShader(GBuffer::Target_Shade, ptr_effect, "g_shade_texture")))
		return;
	if (FAILED(ptr_gbuffer->SetTargetOnShader(GBuffer::Target_Light_Specular, ptr_effect, "g_specular_texture")))
		return;

	ptr_shader_blend_->BegineShader(0);
	ptr_blend_buffer_->Render();
	ptr_shader_blend_->EndShader();
}
