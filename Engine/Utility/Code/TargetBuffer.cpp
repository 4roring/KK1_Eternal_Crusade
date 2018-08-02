#include "TargetBuffer.h"
#include "ViewTexture.h"

Engine::CTargetBuffer::CTargetBuffer(LPDIRECT3DDEVICE9 ptr_device)
	: ptr_device_(ptr_device)
{
}

Engine::CTargetBuffer::~CTargetBuffer()
{
	Release();
}

HRESULT Engine::CTargetBuffer::Initialize(uint32 size_x, uint32 size_y, const D3DFORMAT & format, const D3DXCOLOR& clear_color)
{
	HRESULT hr = E_FAIL;
	hr = D3DXCreateTexture(ptr_device_, size_x, size_y, 1
		, D3DUSAGE_RENDERTARGET, format, D3DPOOL_DEFAULT, &ptr_target_tex_);
	assert(!FAILED(hr) && "Create Target Buffer Texture Create Failed");

	hr = ptr_target_tex_->GetSurfaceLevel(0, &ptr_surface_);
	assert(!FAILED(hr) && "Target Buffer Get Surface Failed");
	ptr_target_tex_->Release();

	clear_color_ = clear_color;

	return S_OK;
}

void Engine::CTargetBuffer::Release()
{
	Safe_Release_Delete(ptr_debug_buffer_);
	Safe_Release(ptr_old_surface_);
	Safe_Release(ptr_target_tex_);
}

Engine::CTargetBuffer * Engine::CTargetBuffer::Create(LPDIRECT3DDEVICE9 ptr_device, uint32 size_x, uint32 size_y, const D3DFORMAT & format, const D3DXCOLOR & clear_color)
{
	CTargetBuffer* target_buffer = new CTargetBuffer(ptr_device);

	if (FAILED(target_buffer->Initialize(size_x, size_y, format, clear_color)))
	{
		Safe_Delete(target_buffer);
		assert(!"target buffer Create Failed");
	}

	return target_buffer;
}

HRESULT Engine::CTargetBuffer::Init_DebugTarget(float _x, float _y, float size_x, float size_y)
{
	ptr_debug_buffer_ = CViewTexture::Create(ptr_device_, _x, _y, size_x, size_y);
	if (nullptr == ptr_debug_buffer_)
		return E_FAIL;

	return S_OK;
}

HRESULT Engine::CTargetBuffer::ClearBuffer()
{
	ptr_device_->GetRenderTarget(0, &ptr_old_surface_);
	ptr_device_->SetRenderTarget(0, ptr_surface_);
	ptr_device_->Clear(0, nullptr, D3DCLEAR_TARGET, clear_color_, 1.f, 0);
	ptr_device_->SetRenderTarget(0, ptr_old_surface_);
	Safe_Release(ptr_old_surface_);

	return S_OK;
}

HRESULT Engine::CTargetBuffer::Render_DebugTarget()
{
	if (nullptr == ptr_device_) return E_FAIL;
	if (nullptr == ptr_debug_buffer_) return E_FAIL;
	ptr_device_->SetTexture(0, ptr_target_tex_);
	ptr_debug_buffer_->Render();

	return S_OK;
}

HRESULT Engine::CTargetBuffer::SetBufferOnDevice(const uint32 index)
{
	ptr_device_->GetRenderTarget(index, &ptr_old_surface_);
	ptr_device_->SetRenderTarget(index, ptr_surface_);

	return S_OK;
}

HRESULT Engine::CTargetBuffer::SetTargetOnShader(LPD3DXEFFECT ptr_effect, const char * constant_name)
{
	if (nullptr == ptr_device_) return E_FAIL;
	ptr_effect->SetTexture(constant_name, ptr_target_tex_);

	return S_OK;
}

HRESULT Engine::CTargetBuffer::ReleaseBufferOnDevice(const uint32 index)
{
	ptr_device_->SetRenderTarget(index, ptr_old_surface_);
	Safe_Release(ptr_old_surface_);

	return S_OK;
}
