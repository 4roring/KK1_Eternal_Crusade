#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CDynamicMesh;
class CAnimController;
END

class CController;
class CGun_Phobos;

class CSpaceMarin
	: public Engine::CGameObject
{
public:
	enum class LowerState { Idle, Run, Evade, End };
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

public:
	void set_move_dir(MoveDirection move_dir);
	void set_zoom(bool is_zoom);
	void set_fire(bool is_fire);
	void set_fire_range_pos(const Vector3& fire_range_pos);
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
	/// Summary �׽�Ʈ. �����̽� ������ �ν��Ͻ��� �����ϴ� �Լ�.
	///</summary>
	///<param name = "ctrl_id">��Ʈ�� ���̵� 0���� �÷��̾�, �� �ܿ� ��ȣ�� AI�� �����Ѵ�.</param>
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

private:
	void Fire();

private:
	void SetConstantTable(LPD3DXEFFECT ptr_effect);

private:
	Engine::CDynamicMesh* ptr_mesh_ = nullptr;
	Engine::CTransform* ptr_lower_transform_ = nullptr;

private:
	Engine::CAnimController* ptr_upper_anim_ctrl_ = nullptr;
	Engine::CAnimController* ptr_lower_anim_ctrl_ = nullptr;
	Engine::BoneFrame* ptr_upper_start_frame_ = nullptr;

private:
	CController* ptr_ctrl_ = nullptr;
	int current_cell_index_ = 0;
	int control_id_ = -1;

private:
	Vector3 fire_range_pos_ = Vector3();
	bool zoom_ = false;
	bool fire_ = false;

private:
	float delta_time_ = 0.f;
	LowerState pre_lower_state_ = LowerState::End;
	LowerState next_lower_state_ = LowerState::End;
	UpperState pre_upper_state_ = UpperState::End;
	UpperState next_upper_state_ = UpperState::End;
	MoveDirection pre_move_dir_ = MoveDirection::End;
	MoveDirection next_move_dir_ = MoveDirection::End;

private:
	CGun_Phobos* ptr_gun_ = nullptr;
	const Matrix* ptr_hand_matrix_ = nullptr;

private:
	Vector3 ray_pos_ = Vector3();
	Vector3 ray_dir_ = Vector3();

#ifdef _DEBUG
private:
	LPD3DXLINE ptr_line_ = nullptr;
#endif
};