#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CCubeColor
	: public CVIBuffer
{
private:
	explicit CCubeColor(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CCubeColor();

public:
	virtual CComponent* CloneComponent();

public:
	void SetBoundingBox(const Vector3& min, const Vector3& max, DWORD color);

private:
	HRESULT CreateBuffer(const Vector3& min, const Vector3& max, DWORD color);

public:
	static CCubeColor* Create(LPDIRECT3DDEVICE9 ptr_device, const Vector3& min, const Vector3& max, DWORD color);
};

END