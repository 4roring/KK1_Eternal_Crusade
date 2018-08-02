#include "G_Buffer.h"
#include "TargetBuffer.h"

Engine::GBuffer::GBuffer(LPDIRECT3DDEVICE9 ptr_device)
	: ptr_device_(ptr_device)
{
}

Engine::GBuffer::~GBuffer()
{
	Release();
}

Engine::CTargetBuffer * Engine::GBuffer::FindTargetBuffer(TARGET_TAG target_tag)
{
	return nullptr;
}

std::vector<Engine::CTargetBuffer*>* Engine::GBuffer::FindMRT(MRT_TAG mrt_tag)
{
	return nullptr;
}

HRESULT Engine::GBuffer::AddTargetBuffer(LPDIRECT3DDEVICE9 ptr_device, TARGET_TAG target_tag, uint32 size_x, uint32 size_y, const D3DFORMAT & format, const D3DXCOLOR& clear_color)
{
	if (nullptr != target_array_[target_tag]) return E_FAIL;

	target_array_[target_tag] = CTargetBuffer::Create(ptr_device_, size_x, size_y, format, clear_color);
	if (nullptr == target_array_[target_tag]) return E_FAIL;

	return S_OK;
}

HRESULT Engine::GBuffer::AddMultiRenderTarget(MRT_TAG mrt_tag, TARGET_TAG target_tag)
{
	if (nullptr == target_array_[target_tag]) return E_FAIL;
	mrt_[mrt_tag].emplace_back(target_array_[target_tag]);

	return S_OK;
}

HRESULT Engine::GBuffer::Add_DebugTarget(TARGET_TAG target_tag, float _x, float _y, float size_x, float size_y)
{
	if (nullptr == target_array_[target_tag]) return E_FAIL;

	return target_array_[target_tag]->Init_DebugTarget(_x, _y, size_x, size_y);
}

HRESULT Engine::GBuffer::Render_DebugTarget(MRT_TAG mrt_tag)
{
	if (true == mrt_[mrt_tag].empty()) return E_FAIL;

	for (auto& target : mrt_[mrt_tag])
	{
		if (nullptr != target)
			target->Render_DebugTarget();
	}

	return S_OK;
}


HRESULT Engine::GBuffer::Begin_G_Buffer(MRT_TAG mrt_tag)
{
	if (true == mrt_[mrt_tag].empty()) return E_FAIL;

	for (auto& target : mrt_[mrt_tag])
	{
		if (nullptr != target)
			target->ClearBuffer();
	}

	uint32 index = 0;

	for (auto& target : mrt_[mrt_tag])
	{
		if (nullptr != target)
			target->SetBufferOnDevice(index++);
	}

	return S_OK;
}

HRESULT Engine::GBuffer::End_G_Buffer(MRT_TAG mrt_tag)
{
	if (true == mrt_[mrt_tag].empty()) return E_FAIL;

	uint32 index = 0;

	for (auto& target : mrt_[mrt_tag])
	{
		if (nullptr != target)
			target->ReleaseBufferOnDevice(index++);
	}

	return S_OK;
}

HRESULT Engine::GBuffer::SetTargetOnShader(TARGET_TAG target_tag, LPD3DXEFFECT ptr_effect, const char * constant_name)
{
	if (nullptr == target_array_[target_tag]) return E_FAIL;
	return target_array_[target_tag]->SetTargetOnShader(ptr_effect, constant_name);
}

Engine::GBuffer * Engine::GBuffer::Create(LPDIRECT3DDEVICE9 ptr_device)
{
	if (nullptr != ptr_single_instance_) return nullptr;

	ptr_single_instance_ = new GBuffer(ptr_device);
	if (nullptr == ptr_single_instance_)
		return nullptr;

	return ptr_single_instance_;
}

void Engine::GBuffer::Release()
{
	for (auto& target : target_array_)
		Safe_Delete(target);

	//for(size_t i = 0; i < target_array_.size(); ++i)
	//	Safe_Delete(target_array_[i]);
}