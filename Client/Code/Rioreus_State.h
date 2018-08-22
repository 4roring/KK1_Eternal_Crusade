#pragma once

BEGIN(Engine)
class CTransform;
class CAnimController;
END

class CRioreus;

class CRioreus_State
{
public:
	CRioreus_State() = default;
	virtual ~CRioreus_State() = default;

public:
	virtual void InitState(CRioreus* ptr_target, Engine::CTransform* ptr_transform, Engine::CAnimController* ptr_anim_ctrl);

public:
	virtual void Update(float delta_time) PURE;

protected:
	CRioreus* ptr_target_ = nullptr;
	Engine::CTransform* ptr_transform_ = nullptr;
	Engine::CAnimController* ptr_anim_ctrl_ = nullptr;
	float condition_ = 0.f;
};