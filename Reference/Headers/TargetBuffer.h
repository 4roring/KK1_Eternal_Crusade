#pragma once

#include "Engine_Include.h"

BEGIN(Engine)

class CViewTexture;

class CTargetBuffer
{
private:
	explicit CTargetBuffer(LPDIRECT3DDEVICE9 ptr_device);
	
public:
	~CTargetBuffer();

private:
	HRESULT Initialize(uint32 size_x, uint32 size_y, const D3DFORMAT& format, const D3DXCOLOR& clear_color);

private:
	void Release();

public:
	static CTargetBuffer* Create(LPDIRECT3DDEVICE9 ptr_device, uint32 size_x, uint32 size_y, const D3DFORMAT& format, const D3DXCOLOR& clear_color);

public:
	HRESULT Init_DebugTarget(float _x, float _y, float size_x, float size_y);
	HRESULT ClearBuffer();
	HRESULT Render_DebugTarget();
	HRESULT SetBufferOnDevice(const uint32 index);
	HRESULT SetTargetOnShader(LPD3DXEFFECT ptr_effect, const char* constant_name);
	HRESULT ReleaseBufferOnDevice(const uint32 index);

private:
	LPDIRECT3DDEVICE9 ptr_device_ = nullptr;
	LPDIRECT3DTEXTURE9 ptr_target_tex_ = nullptr;
	LPDIRECT3DSURFACE9 ptr_surface_ = nullptr;
	LPDIRECT3DSURFACE9 ptr_old_surface_ = nullptr;

private:
	D3DXCOLOR clear_color_;

private:
	CViewTexture* ptr_debug_buffer_ = nullptr;
};

END