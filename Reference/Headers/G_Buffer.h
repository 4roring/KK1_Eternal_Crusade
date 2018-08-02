#pragma once

#include "Engine_Include.h"
#include "Engine_Singleton.h"

#include <array>

BEGIN(Engine)
class CTargetBuffer;

class GBuffer
{
public:
	enum TARGET_TAG { Target_Albedo, Target_Normal, Target_Specular, Target_Depth, Target_Shade, Target_Light_Specular, Target_End };
	enum MRT_TAG { MRT_DEFERRED, MRT_LIGHT, MRT_END };

private:
	explicit GBuffer(LPDIRECT3DDEVICE9 ptr_device);

public:
	~GBuffer();

public:
	CTargetBuffer* FindTargetBuffer(TARGET_TAG target_tag);
	std::vector<CTargetBuffer*>* FindMRT(MRT_TAG mrt_tag);

public:
	HRESULT AddTargetBuffer(LPDIRECT3DDEVICE9 ptr_device, TARGET_TAG target_tag, uint32 size_x, uint32 size_y, const D3DFORMAT& format, const D3DXCOLOR& clear_color);
	HRESULT AddMultiRenderTarget(MRT_TAG mrt_tag, TARGET_TAG target_tag);

public:
	HRESULT Add_DebugTarget(TARGET_TAG target_tag, float _x, float _y, float size_x, float size_y);
	HRESULT Render_DebugTarget(MRT_TAG mrt_tag);

public:
	HRESULT Begin_G_Buffer(MRT_TAG mrt_tag);
	HRESULT End_G_Buffer(MRT_TAG mrt_tag);

public:
	HRESULT SetTargetOnShader(TARGET_TAG target_tag, LPD3DXEFFECT ptr_effect, const char* constant_name);

public:
	static GBuffer* Create(LPDIRECT3DDEVICE9 ptr_device);

private:
	void Release();

private:
	// 지속적인 탐색을 하는 wstring를 열거형으로 바꾸도록 하자.
	std::array<CTargetBuffer*, Target_End> target_array_ = { nullptr };
	std::vector<CTargetBuffer*> mrt_[MRT_END];

private:
	LPDIRECT3DDEVICE9 ptr_device_ = nullptr;
	static GBuffer* ptr_single_instance_;
};

END
