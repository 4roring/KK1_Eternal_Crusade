#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CDynamicMesh;
class CAnimController;
class CCollider;
END

class CController;
class COrk_Gun;
class COrk_Sword;

class COrk
	: public Engine::CGameObject
{
public:
	enum class LowerState { Idle, Run, Attack_1, Attack_2, Down, Down_Loop, Down_Dead, End };
	enum class UpperState { Idle, Shoot, Attack_1, Attack_2, Run, Run_Aiming, Down, Down_Loop, Down_Dead, End };

private:
	explicit COrk(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~COrk();

public:
	Engine::CTransform* ptr_lower_transform();
	Vector3& GetFirePos() const;
	int current_cell_index() const;
	int cell_option() const;

public:
	void set_fire(bool is_fire);
	void set_slash(bool is_slash);
	void set_fire_range_pos(const Vector3& fire_range_pos);
	void SetController(bool is_control);

private:
	HRESULT Initialize(int ctrl_id);

public:
	virtual void LateInit() override;
	virtual void Update(float delta_time) override;
	virtual void LateUpdate() override;
	virtual void Render() override;

public:
	virtual void ApplyDamage(int damage) override;
	virtual void OnEnable() override;

public:
	static COrk* Create(LPDIRECT3DDEVICE9 ptr_device, int ctrl_id);

private:
	HRESULT AddComponent();
	HRESULT AddWeapon();
	void Release();

private: // Update Function
	void UpdateState(float delta_time);
	void AttackState(float delta_time);
	void Run();

private: // LateUpdate Function
	void UpdateLowerAnimState();
	void UpdateUpperAnimState();
	void Fire();
	void Slash();

private:
	void CreateFireEffect();
	void CreateBulletHitEffect(const Vector3& hit_position);


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
	CController* ptr_ctrl_ = nullptr;
	int current_cell_index_ = 0;
	int control_id_ = -1;
	int move_option_ = 0;

private:
	Vector3 fire_range_pos_ = Vector3();
	bool fire_ = false;
	bool slash_ = false;
	float attack_delay_ = 0.f;
	float damage_delay_ = 0.f;
	int cell_option_ = -1;

private:
	float anim_time_ = 0.f;
	LowerState pre_lower_state_ = LowerState::End;
	LowerState next_lower_state_ = LowerState::End;
	UpperState pre_upper_state_ = UpperState::End;
	UpperState next_upper_state_ = UpperState::End;

private:
	COrk_Gun* ptr_gun_ = nullptr;
	const Matrix* ptr_left_hand_matrix_ = nullptr;
	COrk_Sword* ptr_sword_ = nullptr;
	const Matrix* ptr_right_hand_matrix_ = nullptr;

private:
	Vector3 ray_pos_ = Vector3();
	Vector3 ray_dir_ = Vector3();

private:
	int max_hp_ = 0;
	int hp_ = 0;
	int condition_ = 0;

#ifdef _DEBUG
private:
	Engine::CShader* ptr_debug_shader_ = nullptr;
	LPD3DXLINE ptr_line_ = nullptr;
#endif
};