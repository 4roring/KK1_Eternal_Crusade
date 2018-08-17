#include "stdafx.h"
#include "Subject.h"

#include "SpaceMarinObserver.h"
#include "EnemyObserver.h"

CSubject::CSubject()
{
	D3DXMatrixIdentity(&mat_ortho_proj_);
}

CSubject::~CSubject()
{
	Release();
}

const Matrix & CSubject::inv_mat_proj()
{
	return *inv_mat_proj_;
}

const Matrix & CSubject::inv_mat_view()
{
	return *inv_mat_view_;
}

Vector3* CSubject::camera_pos() const
{
	return camera_pos_;
}

Vector3 * CSubject::player_pos() const
{
	return player_pos_;
}

bool CSubject::camera_shaking() const
{
	return camera_shaking_;
}

const Matrix & CSubject::GetOrthoProjection()
{
	return mat_ortho_proj_;
}

void CSubject::SetSpaceMarinData(int* ptr_shield, int* ptr_hp, int* ptr_current_cell, int ctrl_id)
{
	space_marin_shield_[ctrl_id] = ptr_shield;
	space_marin_hp_[ctrl_id] = ptr_hp;
	space_marin_current_cell[ctrl_id] = ptr_current_cell;
}

void CSubject::ReserveEnemyData(int size)
{
	vec_enemy_current_cell_.resize(size);
	vec_enemy_hp_.resize(size);
}

void CSubject::SetEnemyData(int* ptr_hp, int* ptr_current_cell, int ctrl_id)
{
	vec_enemy_hp_[ctrl_id] = ptr_hp;
	vec_enemy_current_cell_[ctrl_id] = ptr_current_cell;
}

void CSubject::SetBossHp(int * ptr_hp)
{
	ptr_boss_hp_ = ptr_hp;
}

void CSubject::SetBossPosition(Vector3 * ptr_boss_pos)
{
	ptr_boss_pos_ = ptr_boss_pos;
}

void CSubject::SetInverseCameraInfo(Matrix * inv_mat_proj, Matrix * inv_mat_view, Vector3 * camera_pos)
{
	inv_mat_proj_ = inv_mat_proj;
	inv_mat_view_ = inv_mat_view;
	camera_pos_ = camera_pos;
}

void CSubject::set_player_pos(Vector3 * player_pos)
{
	player_pos_ = player_pos;
}

void CSubject::set_camera_shaking(bool is_shaking)
{
	camera_shaking_ = is_shaking;
}

void CSubject::SetOrthoProjection(const Matrix & mat_ortho)
{
	mat_ortho_proj_ = mat_ortho;
}

void CSubject::RegisterObserver(CSpaceMarinObserver *& ptr_observer)
{
	ptr_observer = CSpaceMarinObserver::Create();
	space_marin_observer_list_.emplace_back(ptr_observer);
}

void CSubject::RegisterObserver(CEnemyObserver *& ptr_observer)
{
	ptr_observer = CEnemyObserver::Create();
	enemy_observer_list_.emplace_back(ptr_observer);
}

void CSubject::RemoveObserver(CSpaceMarinObserver *& ptr_observer)
{
	auto iter = std::find(space_marin_observer_list_.begin(), space_marin_observer_list_.end(), ptr_observer);
	if (iter != space_marin_observer_list_.end())
	{
		Engine::Safe_Delete(ptr_observer);
		space_marin_observer_list_.erase(iter);
	}
}

void CSubject::RemoveObserver(CEnemyObserver *& ptr_observer)
{
	auto iter = std::find(enemy_observer_list_.begin(), enemy_observer_list_.end(), ptr_observer);
	if (iter != enemy_observer_list_.end())
	{
		Engine::Safe_Delete(ptr_observer);
		enemy_observer_list_.erase(iter);
	}
}

void CSubject::Notify()
{
	for (auto& space_marin_observer : space_marin_observer_list_)
	{
		for (int i = 0; i < 4; ++i)
		{
			if (nullptr == space_marin_shield_[i] 
				|| nullptr == space_marin_hp_[i] 
				|| nullptr == space_marin_current_cell[i]) continue;

			space_marin_observer->Update(space_marin_shield_[i], space_marin_hp_[i], space_marin_current_cell[i], i);
		}
	}

	if (nullptr == ptr_boss_hp_) return;

	for (auto& enemy_observer : enemy_observer_list_)
		enemy_observer->Update(vec_enemy_hp_, vec_enemy_current_cell_, *ptr_boss_hp_, *ptr_boss_pos_);
}

void CSubject::Release()
{
	for (auto& space_marin_observer : space_marin_observer_list_)
		Engine::Safe_Delete(space_marin_observer);
	space_marin_observer_list_.clear();

	for (auto& enemy_observer : enemy_observer_list_)
		Engine::Safe_Delete(enemy_observer);
	enemy_observer_list_.clear();
}
