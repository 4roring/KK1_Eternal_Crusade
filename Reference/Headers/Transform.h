#pragma once

#include "Component.h"

BEGIN(Engine)

struct Transform
{
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;
	Vector3 move_dir;
	Matrix mat_world;
};

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
	Vector3& position();
	Vector3& rotation();
	Vector3& scale();
	Vector3& move_dir();
	Matrix& mat_world();

public:
	Vector3& Right() const;
	Vector3& Up() const;
	Vector3& Forward() const;

public:
	virtual void Update(float delta_time) override;
	virtual int Release() override;

public:
	static CTransform* Create(const Vector3& look);

private:
	Transform transform_ = {};
};

END