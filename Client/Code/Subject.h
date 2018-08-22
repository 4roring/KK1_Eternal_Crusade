#pragma once

#include "Engine_Singleton.h"

class CSpaceMarinObserver;
class CEnemyObserver;

class CSubject
	: public Engine::CSingleton<CSubject>
{
private:
	friend CSingleton;
	CSubject();
	virtual ~CSubject();

public:
	const Matrix& inv_mat_proj();
	const Matrix& inv_mat_view();
	Vector3* camera_pos() const;
	Vector3* player_pos() const;
	bool camera_shaking() const;
	const Matrix& GetOrthoProjection();

public:
	void SetSpaceMarinData(int* ptr_shield, int* ptr_hp, int* ptr_bullet, int* ptr_granade, int* ptr_current_cell, int ctrl_id);
	void ReserveEnemyData(int size);
	void SetEnemyData(int* ptr_hp, int* ptr_current_cell, int ctrl_id);
	void SetBossHp(int* ptr_hp);
	void SetBossPosition(Vector3* ptr_boss_pos);
	void SetInverseCameraInfo(Matrix* inv_mat_proj, Matrix* inv_mat_view, Vector3* camera_pos);
	void set_player_pos(Vector3* player_pos);
	void SetOrthoProjection(const Matrix& mat_ortho);
	void SetCameraShaking(bool is_shaking, float shaking_time, float shaking_power, const Vector3&& shaking_dir);
	void SetShakingInfo(float& out_shaking_time, float& out_shaking_power, Vector3& out_shaking_dir);

public:
	void RegisterObserver(CSpaceMarinObserver*& ptr_observer);
	void RegisterObserver(CEnemyObserver*& ptr_observer);
	void RemoveObserver(CSpaceMarinObserver*& ptr_observer);
	void RemoveObserver(CEnemyObserver*& ptr_observer);
	void Notify();

private:
	void Release();

private: // Data
	int* space_marin_shield_[4] = {};
	int* space_marin_hp_[4] = {};
	int* space_marin_bullet_[4] = {};
	int* space_marin_granade_[4] = {};
	int* space_marin_current_cell[4] = {};
	std::vector<int*> vec_enemy_hp_;
	std::vector<int*> vec_enemy_current_cell_;
	int* ptr_boss_hp_ = nullptr;
	Vector3* ptr_boss_pos_ = nullptr;

private: // Camera Data
	Matrix* inv_mat_proj_ = nullptr;
	Matrix* inv_mat_view_ = nullptr;
	Vector3* camera_pos_ = nullptr;

private:
	Vector3* player_pos_ = nullptr;

private: // Observer
	std::list<CSpaceMarinObserver*> space_marin_observer_list_;
	std::list<CEnemyObserver*> enemy_observer_list_;

private:
	bool camera_shaking_ = false;
	float shaking_time_ = 0.f;
	float shaking_power_ = 0.f;
	Vector3 shaking_dir_ = {};

private:
	Matrix mat_ortho_proj_;
};

static CSubject* Subject()
{
	return CSubject::GetInstance();
}