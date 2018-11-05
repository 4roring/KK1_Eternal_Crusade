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
#include "Rioreus_Somersault.h"
#include "Rioreus_BiteAttack.h"
#include "Rioreus_TailCut.h"

BEGIN(Engine)
class CDynamicMesh;
class CAnimController;
class CCollider;
class KK1_Font;
END

class CSpaceMarin;
class CPartCollider;

class CRioreus
	: public Engine::CGameObject
{
public:
	enum class State { Cry, Idle, Idle_Motion, Rotate, Walk, Sprint, Bite_Attack, TailAttack, Fire, Somersault, Fall, Groggy, TailCut, Dead, End };
	enum PartColl { Part_Head, Part_Body, Part_Left_Leg, Part_Right_Leg, Part_Left_Wing, Part_Right_Wing, Part_Tail, Part_End };

private:
	explicit CRioreus(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CRioreus();

public:
	bool rage() const;
	int& current_cell_index();
	int& head_condition();
	int& left_leg_condition();
	int& right_leg_condition();
	int& tail_condition();
	const Matrix* ptr_mat_fire();

public:
	void SetState(State next_state);
	void SetAnimSpeed(float anim_speed);
	void SetAttackState(PartColl part, bool attack_state);

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
	void UpdateConditionState();
	void UpdateState();

private:
	void SetTail();
	void CutTail();
	void SetCollider();

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
	bool cut_tail_ = false;
	int max_hp_ = 0;
	int hp_ = 0;
	float damage_delay_ = 0.f;

private:
	int left_leg_condition_ = 0;
	int right_leg_condition_ = 0;
	int head_condition_ = 0;
	int tail_condition_ = 0;
	const Matrix* ptr_mat_fire_ = nullptr;

private:
	CPartCollider* ptr_part_coll_[Part_End] = { nullptr };
	Engine::CGameObject* ptr_eye_trail_[2] = { nullptr };

private: // Static State
	CRioreus_State* ptr_state_ = nullptr;
	CRioreus_Idle state_idle_;
	CRioreus_Rotate state_rotate_;
	CRioreus_Walk state_walk_;
	CRioreus_Cry state_cry_;
	CRioreus_Sprint state_sprint_;
	CRioreus_TailAttack state_tail_attack_;
	CRioreus_Fall state_fall_;
	CRioreus_Fire state_fire_;
	CRioreus_Groggy state_groggy_;
	CRioreus_Somersault state_somersault_;
	CRioreus_BiteAttack state_bite_attack_;
	CRioreus_TailCut state_tail_cut_;

#ifdef _DEBUG
private:
	Engine::KK1_Font* ptr_font_ = nullptr;
#endif // _DEBUG

};