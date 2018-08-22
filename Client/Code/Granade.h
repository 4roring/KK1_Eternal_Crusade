#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CStaticMesh;
class CSphere;
END

class CGranade
	: public Engine::CGameObject
{
private:
	explicit CGranade(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CGranade();

private:
	HRESULT Initialize(const Vector3& start_pos, const Vector3& start_dir);

public:

	virtual void Update(float delta_time) override;
	virtual void LateUpdate() override;
	virtual void Render() override;

public:
	static CGranade* Create(LPDIRECT3DDEVICE9 ptr_device, const Vector3& start_pos, const Vector3& start_dir);

private:
	HRESULT AddComponent();
	void Release();

private:
	Engine::CStaticMesh* ptr_mesh_ = nullptr;

private:
	Vector4 color_[3] = {};

private:
	float destroy_time_ = 0.f;
	float pos_y_ = 0.f;
	Vector3 velocity_ = {};
	int current_cell_index_ = -1;

private:
	SoundSource* ptr_bomb_sound_ = nullptr;
	
private:
	Engine::CGameObject* ptr_trail_ = nullptr;
};