#pragma once

#include "Component.h"

BEGIN(Engine)
class CTransform;
END

class CController abstract
	: public Engine::CComponent
{
protected:
	explicit CController();

public:
	virtual ~CController();

public:
	virtual void LateInit();
	virtual void Update(float delta_time) PURE;

protected:
	Engine::CTransform* ptr_ctrl_transform_ = nullptr;
	
private:
	bool late_init_ = false;
};