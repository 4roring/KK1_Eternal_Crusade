#include "ParticleSystem.h"

Engine::CParticleSystem::CParticleSystem(LPDIRECT3DDEVICE9 ptr_device)
	: ptr_device_(ptr_device)
{
}

Engine::CParticleSystem::~CParticleSystem()
{
}

Engine::CComponent * Engine::CParticleSystem::CloneComponent()
{
	++reference_count_;
	return this;
}

HRESULT Engine::CParticleSystem::Initialize()
{
	particles_ = new Particles;

	vertex_size_ = sizeof(VertexColor);
	vertex_count_ = 100;
	vertex_FVF_ = VertexColor_FVF;

	HRESULT hr = ptr_device_->CreateVertexBuffer(vertex_count_ * vertex_size_
		, D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY
		, vertex_FVF_, D3DPOOL_DEFAULT, &vertex_buffer_, 0);

	particles_->_min = Vector3(-5.f, -5.f, -5.f);
	particles_->_max = Vector3(5.f, 5.f, 5.f);
	particles_->speed = 1.f;

	++reference_count_;
	return hr;
}

void Engine::CParticleSystem::Update(float delta_time)
{
	for (auto& particle : particles_->list)
	{
		particle.position += particle.velocity * delta_time;
		particle.acc_time += delta_time;
		if (particle.acc_time > particle.life_time)
			particle.is_alive = false;
	}
}

void Engine::CParticleSystem::Render()
{
	if (false == particles_->list.empty())
	{
		ptr_device_->SetFVF(vertex_FVF_);
		ptr_device_->SetStreamSource(0, vertex_buffer_, 0, vertex_size_);

		if (vertex_offset_ >= vertex_size_)
			vertex_offset_ = 0;

		VertexColor* vertex = nullptr;
		vertex_buffer_->Lock(vertex_offset_ * vertex_size_, vertex_batch_size_ * vertex_size_
			, (void**)&vertex, vertex_offset_ ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

		DWORD particles_in_batch = 0;

		for (auto& particle : particles_->list)
		{
			if (true == particle.is_alive)
			{
				vertex->position = particle.position;
				vertex->color = particle.color;
				++vertex;
				++particles_in_batch;

				if (particles_in_batch == vertex_batch_size_)
				{
					vertex_buffer_->Unlock();

					ptr_device_->DrawPrimitive(D3DPT_POINTLIST, vertex_offset_, vertex_batch_size_);
					vertex_offset_ += vertex_batch_size_;

					if (vertex_offset_ >= vertex_size_)
						vertex_offset_ = 0;

					vertex_buffer_->Lock(vertex_offset_ * vertex_size_, vertex_batch_size_ * vertex_size_
						, (void**)&vertex, vertex_offset_ ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);
				}
			}
		}
		vertex_buffer_->Unlock();
		if (particles_in_batch != 0)
			ptr_device_->DrawPrimitive(D3DPT_POINTLIST, vertex_offset_, particles_in_batch);

		vertex_offset_ += vertex_batch_size_;
	}

	Remove_End_Particle();
}

int Engine::CParticleSystem::Release()
{
	if (--reference_count_ == 0)
	{
		Safe_Release(vertex_buffer_);
		Safe_Delete(particles_);
	}

	return reference_count_;
}

Engine::CParticleSystem * Engine::CParticleSystem::Create(LPDIRECT3DDEVICE9 ptr_device)
{
	CParticleSystem* ptr_particle = new CParticleSystem(ptr_device);
	if (FAILED(ptr_particle->Initialize()))
	{
		Safe_Release_Delete(ptr_particle);
		assert(!"Particle System Create Failed");
	}

	return ptr_particle;
}

void Engine::CParticleSystem::Reset()
{
	for (auto& particle : particles_->list)
		Reset_Particle(particle);
}

void Engine::CParticleSystem::Reset_Particle(Attribute & particle)
{
	particle.is_alive = true;
	particle.position = particles_->origin;

	particle.velocity = GetRandomVector(particles_->_min, particles_->_max);
	particle.velocity = particle.velocity.Normalize();
	particle.velocity *= particles_->speed;

	particle.color = D3DXCOLOR(RandomFloat(0.f, 1.f), RandomFloat(0.f, 1.f), RandomFloat(0.f, 1.f), 1.f);

	particle.acc_time = 0.f;
	particle.life_time = 2.f;
}

void Engine::CParticleSystem::Add_Particle()
{
	Attribute particle;
	Reset_Particle(particle);

	particles_->list.emplace_back(particle);
}

void Engine::CParticleSystem::Remove_End_Particle()
{
	auto iter = particles_->list.begin();
	auto iter_end = particles_->list.end();

	for (; iter != iter_end; )
	{
		if (false == iter->is_alive)
			iter = particles_->list.erase(iter);
		else
			++iter;
	}
}
