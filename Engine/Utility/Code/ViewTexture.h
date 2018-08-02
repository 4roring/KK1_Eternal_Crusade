#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CViewTexture
	: public CVIBuffer
{
private:
	explicit CViewTexture(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CViewTexture();

public:
	virtual CComponent* CloneComponent() override;

public:
	static CViewTexture* Create(LPDIRECT3DDEVICE9 ptr_device, float _x, float _y, float size_x, float size_y);

private:
	HRESULT InitBuffer(float _x, float _y, float size_x, float size_y);
};

END