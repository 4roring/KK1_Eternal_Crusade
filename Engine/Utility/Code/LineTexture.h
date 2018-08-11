#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CLineTexture
	: public CVIBuffer
{
private:
	explicit CLineTexture(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CLineTexture();

public:
	virtual CComponent* CloneComponent() override;

public:
	static CLineTexture* Create(LPDIRECT3DDEVICE9 ptr_device);

private:
	HRESULT InitBuffer();
};

END