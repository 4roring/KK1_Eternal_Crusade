#pragma once

#include "GameObject.h"

#include "Rioreus_Cry.h"
#include "Rioreus_Idle.h"
#include "Rioreus_Rotate.h"
#include "Rioreus_Walk.h"
#include "Rioreus_Sprint.h"
#include "Rioreus_TailAttack.h"
#include "Rioreus_Fall.h"
#include "Rioreus_Fire.h"
#include "Rioreus_Groggy.h"

BEGIN(Engine)
class CDynamicMesh;
class CAnimController;
class CCollider;
END

class CSpaceMarin;

class CRioreus
	: public Engine::CGameObject
{
public:
	enum class State { Cry, Idle, Idle_Motion, Rotate, Walk, Sprint, TailAttack, Fire, Glide, Sumersault, Fall, Groggy, TailCut, Dead, End };

private:
	explicit CRioreus(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CRioreus();

public:
	bool rage() const;
	float& pot_y();
	int& current_cell_index();

public:
	void SetState(State next_state);
	void SetAnimSpeed(float anim_speed);

private:
	HRESULT Initialize();

public:
	virtual void LateInit() override;
	virtual void Update(float delta_time) override;
	virtual void LateUpdate() override;
	virtual void Render() override;

public:
	virtual void ApplyDamage(int damage) override;
	
public:
	static CRioreus* Create(LPDIRECT3DDEVICE9 ptr_device);

private:
	HRESULT AddComponent();
	void Release();

private:
	void SetConstantTable(LPD3DXEFFECT ptr_effect);

private:
	void UpdateState();

private:
	void SetTaleBone();

private:
	Engine::CDynamicMesh* ptr_mesh_ = nullptr;

private:
	Engine::CAnimController* ptr_anim_ctrl_ = nullptr;

private:
	State current_state_ = State::End;
	State next_state_ = State::End;
	int current_cell_index_ = -1;
	float anim_time_ = 0.f;
	float anim_speed_ = 0.f;

private:
	bool rage_ = false;
	int max_hp_ = 0;
	int hp_ = 0;
	float pos_y_ = 0.f;

private:
	CRioreus_State* ptr_state_ = nullptr;

private: // Static State
	CRioreus_Idle state_idle_;
	CRioreus_Rotate state_rotate_;
	CRioreus_Walk state_walk_;
	CRioreus_Cry state_cry_;
	CRioreus_Sprint state_sprint_;
	CRioreus_TailAttack state_tail_attack_;
	CRioreus_Fall state_fall_;
	CRioreus_Fire state_fire_;
	CRioreus_Groggy state_groggy_;
};