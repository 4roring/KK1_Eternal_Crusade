#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRectTexture
	: public CVIBuffer
{
private:
	explicit CRectTexture(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CRectTexture();

public:
	virtual CComponent* CloneComponent() override;

public:
	static CRectTexture* Create(LPDIRECT3DDEVICE9 ptr_device);

private:
	HRESULT InitBuffer();
};

END