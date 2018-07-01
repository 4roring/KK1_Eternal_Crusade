#pragma once

#include "Component.h"

BEGIN(Engine)

class CGameObject;

class ENGINE_DLL CColiision
	: public CComponent
{
private:
	explicit CColiision();

public:
	virtual ~CColiision();

public:
	virtual void Update(float delta_time);
	virtual int Release() override;

protected:
	const CGameObject* ptr_object = nullptr;
	const Vector3* ptr_position_ = nullptr;
};

END