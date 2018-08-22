#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTrailColor
	: public Engine::CComponent
{
private:
	explicit CTrailColor(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CTrailColor();

public:
	virtual CComponent* CloneComponent() override;

private:
	HRESULT Initialize();

public:
	void SetTrail(const Vector3* pos, const Vector3* dir, float length, const D3DXCOLOR& color, DWORD count);

public:
	void Render();
	virtual int Release() override;

public:
	static CTrailColor* Create(LPDIRECT3DDEVICE9 ptr_device);

private:
	LPDIRECT3DDEVICE9 ptr_device_ = nullptr;
	VertexColor* ptr_vertex_ = nullptr;
	DWORD vertex_size_ = 0;
	DWORD vertex_FVF_ = 0;
	DWORD poly_ = 0;
};

END