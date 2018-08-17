#pragma once

#include "Component.h"

BEGIN(Engine)

class CParticle;
class CTexture;

enum class ParticleType { Point, Billboard, Mesh, End };

struct Attribute
{
	CParticle* ptr_particle = nullptr;;
	Vector3 position = {};
	Vector3 rotation = {};
	Vector3 scale = {};
	Vector3 velocity = {};
	Vector3 acceleration = {};
	Vector3 gravity = {};
	float life_time = 0.f;
	float acc_time = 0.f;
	D3DXCOLOR color = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	D3DXCOLOR fade_color = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	bool is_alive = false;
} ;

struct Particles
{
	std::list<Attribute> list;
	ParticleType type = ParticleType::Point;
	Vector3 origin = {};
	Vector3 origin_min = {};
	Vector3 origin_max = {};
	Vector3 velocity = {};
	Vector3 velocity_min = {};
	Vector3 velocity_max = {};
	Vector3 gravity = {};
	float scale = 0.f;
	float scale_min = 0.f;
	float scale_max = 0.f;
	float speed = 0.f;
	float life_time = 0.f;
	int max_particle;
	D3DXCOLOR color;
	int mesh_stage_id = 0;
	std::wstring mesh_key;
	int texture_stage_id = 0;
	std::wstring texture_key;
	CTexture* ptr_texture = nullptr;
	float duration = 0.f;
	float acc_time = 0.f;
	float max_u = 0.f;
	float max_v = 0.f;
	bool loop = false;
	bool uv_animation = false;
	bool uv_loop = false;
	bool random_origin = false;
	bool random_velocity = false;
	bool random_color = false;
	bool random_rotation = false;
	bool random_scale = false;
};

class ENGINE_DLL CParticleSystem
	: public CComponent
{
private:
	explicit CParticleSystem(LPDIRECT3DDEVICE9 ptr_device);
	explicit CParticleSystem(const CParticleSystem& rhs);

public:
	virtual ~CParticleSystem();

public:
	Particles* particles() { return particles_; }

public:
	void SetParticleMeshKey(int mesh_stage_id, const std::wstring& mesh_key);
	void SetParticleTexture(int texture_stage_id, const std::wstring& texture_key);
	void SetCameraPos(Vector3* ptr_camera_pos);

public:
	virtual CComponent* CloneComponent() override;

private:
	HRESULT Initialize(DWORD max_particle, ParticleType type);

public:
	virtual void Update(float delta_time) override;
	void Render(LPD3DXEFFECT ptr_effect, const Matrix* mat_parent);
	void Render_Point(LPD3DXEFFECT ptr_effect, const Matrix* mat_parent);
	void Render_Billboard(LPD3DXEFFECT ptr_effect, const Matrix* mat_parent);
	virtual int Release() override;

public:
	static CParticleSystem* Create(LPDIRECT3DDEVICE9 ptr_device, DWORD max_particle, ParticleType type);

public:
	void Reset();
	void Init_Particle(Attribute& particle);
	void Add_Particle();

private:
	void Remove_End_Particle();

private:
	int particle_count = 0;
	Particles* particles_;

private:
	Vector3* ptr_camera_pos_ = nullptr;

private:
	LPDIRECT3DDEVICE9 ptr_device_ = nullptr;
	LPDIRECT3DVERTEXBUFFER9 vertex_buffer_ = nullptr;
	DWORD vertex_size_ = 0;
	DWORD vertex_count_ = 0;
	DWORD vertex_FVF_ = 0;

private:
	DWORD vertex_offset_ = 0;
	DWORD vertex_batch_size_ = 0;
};

END