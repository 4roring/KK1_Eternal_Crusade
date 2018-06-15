#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CCubeTexture
	: public CVIBuffer
{
private:
	explicit CCubeTexture(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CCubeTexture();

public:
	virtual CComponent* CloneComponent();

public:
	static CCubeTexture* Create(LPDIRECT3DDEVICE9 ptr_device);

private:
	HRESULT InitBuffer();
};

END