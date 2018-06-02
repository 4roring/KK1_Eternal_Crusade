#pragma once

#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CComponent 
{
protected:
	CComponent();

public:
	virtual ~CComponent();

public:
	virtual CComponent* CloneComponent();

public:
	virtual void Update(float delta_time);
	virtual int Release() PURE;

public:
	int AddReferenceCount();

public:
	bool enable_ = true;

protected:
	int reference_count_ = 0;
};

END