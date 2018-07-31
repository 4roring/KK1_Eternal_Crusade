#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CDynamicMesh;
class CAnimController;
class CCollider;
END

class CController;
class COrk_Gun;
class COrk_Klaw;
class COrk_HeavyGun;

class COrk_WarBoss
	: public Engine::CGameObject
{
public:
	enum class Weapon { HeavyGun, Klaw, End };
	enum class BehaviorPattern { Spawn, Idle, Shoot, Attack, Ground_Attack, Skill, Down, Victim, End};

private:
	explicit COrk_WarBoss(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~COrk_WarBoss();

public:
	Engine::CTransform* ptr_lower_transform();
	int current_cell_index() const;

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
	static COrk_WarBoss* Create(LPDIRECT3DDEVICE9 ptr_device);

private:
	HRESULT AddComponent();
	HRESULT AddWeapon();
	void Release();

private: // Update Function, 패턴에 따라 상태 조작.
	void UpdateState(float delta_time);

private: // LateUpdate Function
	void UpdateAnimState();

private: // Render Function
	void SetConstantTable(LPD3DXEFFECT ptr_effect);
#ifdef _DEBUG
	void DebugRender();
#endif

private:
	Engine::CDynamicMesh* ptr_mesh_ = nullptr;
	Engine::CTransform* ptr_lower_transform_ = nullptr;

private:
	Engine::CAnimController* ptr_upper_anim_ctrl_ = nullptr;
	Engine::CAnimController* ptr_lower_anim_ctrl_ = nullptr;
	Engine::BoneFrame* ptr_upper_start_frame_ = nullptr;

private:
	Engine::BoneFrame* ptr_head_frame_ = nullptr;
	Engine::CCollider* ptr_head_collider_ = nullptr;
	Engine::BoneFrame* ptr_body_frame_ = nullptr;
	Engine::CCollider* ptr_body_collider_ = nullptr;

private:
	int current_cell_index_ = 0;
	int move_option_ = 0;

private:
	Vector3 fire_range_pos_ = Vector3();
	float attack_delay_ = 0.f;

private:
	float anim_time_ = 0.f;
	BehaviorPattern pre_behavior_pattern_ = BehaviorPattern::End;
	BehaviorPattern next_behavior_pattern_ = BehaviorPattern::End;

private:
	COrk_Gun* ptr_gun_ = nullptr;
	const Matrix* ptr_left_hand_matrix_ = nullptr;
	COrk_Klaw* ptr_klaw_ = nullptr;
	const Matrix* ptr_right_hand_matrix_ = nullptr;
	COrk_Gun* ptr_heavy_gun_ = nullptr;
	const Matrix* ptr_back_matrix_ = nullptr;

private:
	Vector3 fire_pos_ = Vector3();
	Vector3 fire_dir_ = Vector3();

private:
	int hp_ = 0;
	int condition_ = 0;

#ifdef _DEBUG
private:
	Engine::CShader* ptr_debug_shader_ = nullptr;
	LPD3DXLINE ptr_line_ = nullptr;
#endif
};