#include "G_Buffer.h"
#include "TargetBuffer.h"

Engine::G_Buffer::G_Buffer(LPDIRECT3DDEVICE9 ptr_device)
	: ptr_device_(ptr_device)
{
}

Engine::G_Buffer::~G_Buffer()
{
}

Engine::CTargetBuffer * Engine::G_Buffer::FindTargetBuffer(const std::wstring & tag)
{
	return nullptr;
}

HRESULT Engine::G_Buffer::AddTargetBuffer(LPDIRECT3DDEVICE9 ptr_device, const std::wstring & tag, uint32 size_x, uint32 size_y, const D3DFORMAT & format, const D3DXCOLOR& clear_color)
{
	auto iter = map_target_.find(tag);
	if (iter != map_target_.end())
		return E_FAIL;

	return S_OK;
}

HRESULT Engine::G_Buffer::AddMultiRenderTarget(const std::wstring & mrt_tag, const std::wstring & tag)
{
	return S_OK;
}


HRESULT Engine::G_Buffer::Begin_G_Buffer(MRT mrt_id)
{
	return S_OK;
}

HRESULT Engine::G_Buffer::End_G_Buffer()
{
	return S_OK;
}

void Engine::G_Buffer::Release()
{
	for (auto& pair : map_target_)
		Safe_Delete(pair.second);
	map_target_.clear();
}
