#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CResources
	: public CComponent
{
protected:
	explicit CResources(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CResources();

protected:
	LPDIRECT3DDEVICE9 ptr_device_ = nullptr;
};

END