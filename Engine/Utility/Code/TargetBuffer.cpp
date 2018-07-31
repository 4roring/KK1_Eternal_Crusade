#include "TargetBuffer.h"

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
	assert(!FAILED(hr) && "");

	hr = ptr_target_tex_->GetSurfaceLevel(0, &ptr_surface_);
	assert(!FAILED(hr) && "");
	ptr_target_tex_->Release();

	clear_color_ = clear_color;

	return S_OK;
}

void Engine::CTargetBuffer::Release()
{
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

HRESULT Engine::CTargetBuffer::ClearBuffer()
{
	ptr_device_->GetRenderTarget(0, &ptr_old_surface_);
	ptr_device_->SetRenderTarget(0, ptr_surface_);
	ptr_device_->Clear(0, nullptr, D3DCLEAR_TARGET, clear_color_, 1, 0);
	ptr_device_->SetRenderTarget(0, ptr_old_surface_);
	ptr_old_surface_->Release();

	return S_OK;
}

HRESULT Engine::CTargetBuffer::SetBufferOnDevice(const uint32 index)
{
	ptr_device_->GetRenderTarget(index, &ptr_old_surface_);
	ptr_device_->SetRenderTarget(index, ptr_surface_);

	return S_OK;
}

HRESULT Engine::CTargetBuffer::ReleaseBufferOnDevice(const uint32 index)
{
	ptr_device_->SetRenderTarget(index, ptr_old_surface_);
	ptr_old_surface_->Release();

	return S_OK;
}
