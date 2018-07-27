#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CDynamicMesh;
class CAnimController;
END

class CController;

class CTyranid
	: public Engine::CGameObject
{
public:
	enum class LowerState { Idle, Run, Evade, End };
	enum class UpperState { Idle, Shoot, Run, Run_Aiming, End };
	enum class MoveDirection { Forward, Backward, Right, Left, End };

private:
	explicit CTyranid(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CTyranid();

public:
	void set_attack(bool is_attack);

private:
	HRESULT Initialize();

public:
	virtual void LateInit() override;
	virtual void Update(float delta_time) override;
	virtual void LateUpdate() override;
	virtual void Render() override;

public:
	static CTyranid* Create(LPDIRECT3DDEVICE9 ptr_device);

private:
	HRESULT AddComponent();
	void Release();

private:
	void UpdateState(float delta_time);
	void UpdateAnimState();
	void Run();
	void Attack();

private:
	Engine::CDynamicMesh* ptr_mesh_ = nullptr;

private:
	Engine::CAnimController* ptr_anim_ctrl_ = nullptr;

private:
	CController* ptr_ctrl_ = nullptr;
	int current_cell_index_ = 0;

private:
	bool attack_ = false;

private:
	float delta_time_ = 0.f;
	LowerState pre_state_ = LowerState::End;
	LowerState next_state_ = LowerState::End;
};