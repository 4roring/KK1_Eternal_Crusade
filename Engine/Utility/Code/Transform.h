#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform
	: public CComponent
{
private:
	explicit CTransform(const Vector3& look);

public:
	virtual ~CTransform();

public:
	virtual CComponent* CloneComponent();

public:
	virtual void Update(float delta_time) override;
	virtual int Release() override;

public:
	static CTransform* Create(const Vector3& look);

public:
	Vector3 position_ = Vector3(0.f, 0.f, 0.f);
	Vector3 rotation_ = Vector3(0.f, 0.f, 0.f);
	Vector3 scale_ = Vector3(1.f, 1.f, 1.f);
	Vector3 direction_ = Vector3(0.f, 0.f, 0.f);
	Matrix mat_world_;
};

END