#pragma once

#include "Engine_Include.h"
#include "Engine_Singleton.h"

BEGIN(Engine)
class CTargetBuffer;

class G_Buffer
{
public:
	enum MRT { MRT_DEFERRED, MRT_LIGHT, MRT_END };

private:
	explicit G_Buffer(LPDIRECT3DDEVICE9 ptr_device);

public:
	~G_Buffer();

public:
	CTargetBuffer* FindTargetBuffer(const std::wstring& tag);
	
public:
	HRESULT AddTargetBuffer(LPDIRECT3DDEVICE9 ptr_device, const std::wstring& tag, uint32 size_x, uint32 size_y, const D3DFORMAT& format, const D3DXCOLOR& clear_color);
	HRESULT AddMultiRenderTarget(const std::wstring& mrt_tag, const std::wstring& tag);

public:
	HRESULT Begin_G_Buffer(MRT mrt_id);
	HRESULT End_G_Buffer();

private:
	void Release();

private:
	// 지속적인 탐색을 하는 wstring를 열거형으로 바꾸도록 하자.
	std::unordered_map<std::wstring, CTargetBuffer*> map_target_;
	//std::unordered_map<std::wstring, TargetBufferList> multi_render_target_;

private:
	LPDIRECT3DDEVICE9 ptr_device_ = nullptr;
};

END
