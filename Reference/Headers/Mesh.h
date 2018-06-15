#pragma once

#include "Resources.h"

BEGIN(Engine)

class ENGINE_DLL CMesh
	: public CResources
{
protected:
	explicit CMesh(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CMesh();

public:
	void GetMinMax(const Vector3& min, const Vector3& max);

public:
	virtual void GetComputeBoundingBox(const Vector3& min, const Vector3& max) const PURE;

public:
	virtual void RenderMesh(LPD3DXEFFECT ptr_effect) PURE;

public:
	virtual int Release() PURE;

protected:
	Vector3 min_ = Vector3(0.f, 0.f, 0.f);
	Vector3 max_ = Vector3(0.f, 0.f, 0.f);
};

END