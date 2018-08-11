#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CDynamicMesh;
class CAnimController;
class CCollider;
END

class CController;
class CGun_Phobos;
class CChainSword;
class CEnemyObserver;

class CSpaceMarin
	: public Engine::CGameObject
{
public:
	enum class Weapon { Gun, ChainSword, End };
	enum class LowerState { Idle, Turn, Run, Attack, Reload, Evade, Execution, End };
	enum class UpperState { Idle, Shoot, Run, Run_Aiming, End };
	enum class MoveDirection { Forward, Backward, Right, Left, End };

private:
	explicit CSpaceMarin(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CSpaceMarin();

public:
	Engine::CTransform* ptr_lower_transform();
	Vector3& GetFirePos() const;
	int current_cell_index() const;
	Weapon weapon() const;
	bool evade() const;
	bool execution() const;

public:
	void set_move_dir(MoveDirection move_dir);
	void set_zoom(bool is_zoom);
	void set_fire(bool is_fire);
	void set_fire_range_pos(const Vector3& fire_range_pos);
	void set_weapon();
	void set_evade_dir(const Vector3& evade_dir);
	void set_evade(bool is_evade);
	void set_execution();
	void set_execution_target(const Vector3& target_pos);
	void set_next_rot_y(float next_rot_y);
	void SetRay(const Vector3& ray_pos, const Vector3& ray_dir);

private:
	HRESULT Initialize(int ctrl_id);

public:
	virtual void LateInit() override;
	virtual void Update(float delta_time) override;
	virtual void LateUpdate() override;
	virtual void Render() override;

public:
	///<summary>
	/// Summary 테스트. 스페이스 마린의 인스턴스를 생성하는 함수.
	///</summary>
	///<param name = "ctrl_id">컨트롤 아이디 0번은 플레이어, 그 외에 번호는 AI가 조작한다.</param>
	static CSpaceMarin* Create(LPDIRECT3DDEVICE9 ptr_device, int ctrl_id = 0);

private:
	HRESULT AddComponent();
	HRESULT AddWeapon();
	void Release();

private:
	void UpdateState(float delta_time);
	void UpdateLowerAnimState();
	void UpdateUpperAnimState();
	void Run();
	void Evade(float delta_time);
	void Execution();

private:
	void Fire();
	void Attack();

private:
	void CreateFireEffect();

private:
	void SetConstantTable(LPD3DXEFFECT ptr_effect);

private:
	Engine::CDynamicMesh* ptr_mesh_ = nullptr;
	Engine::CTransform* ptr_lower_transform_ = nullptr;
	Engine::CCollider* ptr_head_collider_ = nullptr;
	const Matrix* ptr_head_matrix_ = nullptr;
	Engine::CCollider* ptr_body_collider_ = nullptr;
	const Matrix* ptr_body_matrix_ = nullptr;

private:
	Engine::CAnimController* ptr_upper_anim_ctrl_ = nullptr;
	Engine::CAnimController* ptr_lower_anim_ctrl_ = nullptr;
	Engine::BoneFrame* ptr_upper_start_frame_ = nullptr;
	float next_rot_y_ = 0.f;

private:
	CController* ptr_ctrl_ = nullptr;
	int current_cell_index_ = 0;
	int control_id_ = -1;

private:
	Vector3 fire_range_pos_ = {};
	bool zoom_ = false;
	bool fire_ = false;

private:
	Vector3 evade_dir_ = {};
	bool evade_ = false;

private:
	Vector3 execution_target_ = {};
	bool execution_ = false;

private:
	int max_shield_ = 0;
	int shield_ = 0;
	int max_hp_ = 0;
	int hp_ = 0;
	int bullet_count_ = 0;
	float damage_time_ = 0.f;
	float condition_ = 0.f;

private:
	float delta_time_ = 0.f;
	LowerState pre_lower_state_ = LowerState::End;
	LowerState next_lower_state_ = LowerState::End;
	UpperState pre_upper_state_ = UpperState::End;
	UpperState next_upper_state_ = UpperState::End;
	MoveDirection pre_move_dir_ = MoveDirection::End;
	MoveDirection next_move_dir_ = MoveDirection::End;
	Weapon weapon_ = Weapon::End;

private:
	CGun_Phobos* ptr_gun_ = nullptr;
	CChainSword* ptr_sword_ = nullptr;
	const Matrix* ptr_hand_matrix_ = nullptr;

private:
	Vector3 ray_pos_ = Vector3();
	Vector3 ray_dir_ = Vector3();

private:
	Vector4 color_[3] = {};

#ifdef _DEBUG
private:
	void DebugRender();

private:
	LPD3DXLINE ptr_line_ = nullptr;
#endif
};