#include "Light.h"
#include "ViewTexture.h"

Engine::CLight::CLight(LPDIRECT3DDEVICE9 ptr_device)
	: ptr_device_(ptr_device)
{
}

Engine::CLight::~CLight()
{
	Release();
}

HRESULT Engine::CLight::Initialize(const D3DLIGHT9 & light_info)
{
	light_info_ = light_info;

	D3DVIEWPORT9 view_port = {};
	ptr_device_->GetViewport(&view_port);
	ptr_render_buffer_ = CViewTexture::Create(ptr_device_, 0.f, 0.f, (float)view_port.Width, (float)view_port.Height);

	return S_OK;
}

void Engine::CLight::Render_Light(LPD3DXEFFECT ptr_effect)
{
	uint32 pass_index = 0;

	switch (light_info_.Type)
	{
	case D3DLIGHT_DIRECTIONAL:
		pass_index = 0;
		ptr_effect->SetVector("g_light_dir", (Vector4*)&light_info_.Direction);
		break;
	case D3DLIGHT_POINT:
		pass_index = 1;
		break;
	}
	ptr_effect->SetVector("g_light_diffuse", (Vector4*)&light_info_.Diffuse);
	ptr_effect->SetVector("g_light_ambient", (Vector4*)&light_info_.Ambient);
	ptr_effect->SetVector("g_light_specular", (Vector4*)&light_info_.Specular);

	ptr_effect->CommitChanges();

	ptr_effect->BeginPass(pass_index);
	ptr_render_buffer_->Render();
	ptr_effect->EndPass();
}

void Engine::CLight::Release()
{
	Safe_Release_Delete(ptr_render_buffer_);
}

Engine::CLight * Engine::CLight::Create(LPDIRECT3DDEVICE9 ptr_device, const D3DLIGHT9 & light_info)
{
	CLight* ptr_light = new CLight(ptr_device);
	if (FAILED(ptr_light->Initialize(light_info)))
	{
		Safe_Delete(ptr_light);
		assert(!"Light Create Failed");
	}

	return ptr_light;
}
