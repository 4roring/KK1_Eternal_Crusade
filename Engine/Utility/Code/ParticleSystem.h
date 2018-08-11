#pragma once

#include "Component.h"

BEGIN(Engine)

struct Particles
{
	Vector3 origin = {};
	Vector3 _min = {};
	Vector3 _max = {};
	float speed = 0.f;
	std::list<Attribute> list;
};

class ENGINE_DLL CParticleSystem
	: public CComponent
{
private:
	explicit CParticleSystem(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CParticleSystem();

public:
	virtual CComponent* CloneComponent() override;

private:
	HRESULT Initialize();

public:
	virtual void Update(float delta_time) override;
	void Render();
	virtual int Release() override;

public:
	static CParticleSystem* Create(LPDIRECT3DDEVICE9 ptr_device);

public:
	void Reset();
	void Reset_Particle(Attribute& particle);
	void Add_Particle();

private:
	void Remove_End_Particle();

private:
	float emit_rate_ = 0.f;
	float size_ = 0.f;
	int max_particle_ = 0;
	Particles* particles_;

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