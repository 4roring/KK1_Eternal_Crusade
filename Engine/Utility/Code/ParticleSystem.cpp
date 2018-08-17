#include "ParticleSystem.h"
#include "BillboardParticle.h"

#include "GameManager.h"
#include "Texture.h"
#include "Transform.h"

Engine::CParticleSystem::CParticleSystem(LPDIRECT3DDEVICE9 ptr_device)
	: ptr_device_(ptr_device)
{
}

Engine::CParticleSystem::CParticleSystem(const CParticleSystem & rhs)
{
	this->ptr_device_ = rhs.ptr_device_;

	this->Initialize(rhs.particles_->max_particle, rhs.particles_->type);
	this->particles_->origin = rhs.particles_->origin;
	this->particles_->origin_min = rhs.particles_->origin_min;
	this->particles_->origin_max = rhs.particles_->origin_max;

	this->particles_->velocity = rhs.particles_->velocity;
	this->particles_->velocity_min = rhs.particles_->velocity_min;
	this->particles_->velocity_max = rhs.particles_->velocity_max;

	this->particles_->gravity = rhs.particles_->gravity;
	
	this->particles_->scale = rhs.particles_->scale;
	this->particles_->scale_min = rhs.particles_->scale_min;
	this->particles_->scale_max = rhs.particles_->scale_max;
	
	this->particles_->speed = rhs.particles_->speed;
	this->particles_->life_time = rhs.particles_->life_time;
	
	this->particles_->duration = rhs.particles_->duration;
	this->particles_->loop = rhs.particles_->loop;
	this->particles_->random_origin = rhs.particles_->random_origin;
	this->particles_->random_velocity = rhs.particles_->random_velocity;

	if (rhs.particles_->type == ParticleType::Billboard)
	{
		this->SetParticleMeshKey(rhs.particles_->mesh_stage_id, rhs.particles_->mesh_key);
		this->SetParticleTexture(rhs.particles_->texture_stage_id, rhs.particles_->texture_key);
		this->particles_->max_u = rhs.particles_->max_u;
		this->particles_->max_v = rhs.particles_->max_v;
		this->particles_->uv_animation = rhs.particles_->uv_animation;
		this->particles_->uv_loop = rhs.particles_->uv_loop;
		this->particles_->random_rotation = rhs.particles_->random_rotation;
		this->particles_->random_scale = rhs.particles_->random_scale;
	}
	else
	{
		this->particles_->color = rhs.particles_->color;
		this->particles_->random_color = rhs.particles_->random_color;
	}
}

Engine::CParticleSystem::~CParticleSystem()
{
}

void Engine::CParticleSystem::SetParticleMeshKey(int mesh_stage_id, const std::wstring & mesh_key)
{
	particles_->mesh_stage_id = mesh_stage_id;
	particles_->mesh_key = mesh_key;
}

void Engine::CParticleSystem::SetParticleTexture(int texture_stage_id, const std::wstring & texture_key)
{
	if (nullptr != particles_->ptr_texture) Safe_Release_Delete(particles_->ptr_texture);

	particles_->texture_stage_id = texture_stage_id;
	particles_->texture_key = texture_key;
	particles_->ptr_texture = dynamic_cast<CTexture*>(CGameManager::GetInstance()->CloneComponent(texture_stage_id, texture_key));
}

void Engine::CParticleSystem::SetCameraPos(Vector3 * ptr_camera_pos)
{
	ptr_camera_pos_ = ptr_camera_pos;
}

Engine::CComponent * Engine::CParticleSystem::CloneComponent()
{
	return new CParticleSystem(*this);
}

HRESULT Engine::CParticleSystem::Initialize(DWORD max_particle, ParticleType type)
{
	particles_ = new Particles;
	HRESULT hr = S_OK;
	if (type == ParticleType::Point)
	{
		vertex_size_ = sizeof(VertexColor);
		vertex_count_ = max_particle;
		vertex_FVF_ = VertexColor_FVF;

		vertex_batch_size_ = max_particle;

		hr = ptr_device_->CreateVertexBuffer(vertex_count_ * vertex_size_
			, D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY
			, vertex_FVF_, D3DPOOL_DEFAULT, &vertex_buffer_, 0);
	}
	particles_->type = type;
	particles_->origin = Vector3(0.f, 0.f, 0.f);
	particles_->velocity_min = Vector3(-5.f, -5.f, -5.f);
	particles_->velocity_max = Vector3(5.f, 5.f, 5.f);
	particles_->random_velocity = true;
	particles_->random_color = true;
	particles_->scale = 0.5f;
	particles_->speed = 1.f;
	particles_->duration = 5.f;
	particles_->gravity = Vector3(0.f, 2.f, 0.f);
	particles_->life_time = 5.f;
	particles_->max_particle = max_particle;
	particles_->loop = true;

	particle_count = 0;
	++reference_count_;
	return hr;
}

void Engine::CParticleSystem::Update(float delta_time)
{
	if (true == particles_->loop)
		Add_Particle();
	else if((particles_->acc_time += delta_time) <= particles_->duration)
		Add_Particle();

	for (auto& particle : particles_->list)
	{
		particle.position += particle.velocity * delta_time;
		particle.acc_time += delta_time;
		particle.velocity -= particle.gravity * delta_time;
		
		if (particles_->type != ParticleType::Point)
			particle.ptr_particle->Update(delta_time, particle.position, particle.rotation, particle.scale, *ptr_camera_pos_);

		if (particle.acc_time > particle.life_time)
			particle.is_alive = false;
	}
}

void Engine::CParticleSystem::Render(LPD3DXEFFECT ptr_effect, const Matrix* mat_parent)
{
	switch (particles_->type)
	{
	case ParticleType::Point:
		ptr_effect->BeginPass(0);
		Render_Point(ptr_effect, mat_parent);
		ptr_effect->EndPass();
		break;
	case ParticleType::Billboard:
		Render_Billboard(ptr_effect, mat_parent);
		break;
	}

	Remove_End_Particle();
}

void Engine::CParticleSystem::Render_Point(LPD3DXEFFECT ptr_effect, const Matrix* mat_parent)
{
	Matrix mat_world, mat_view, mat_proj;

	ptr_device_->GetTransform(D3DTS_VIEW, &mat_view);
	ptr_device_->GetTransform(D3DTS_PROJECTION, &mat_proj);

	if (nullptr == mat_parent)
	{
		D3DXMatrixIdentity(&mat_world);
		ptr_effect->SetMatrix("g_mat_world", &mat_world);
	}
	else
		ptr_effect->SetMatrix("g_mat_world", mat_parent);

	ptr_effect->SetMatrix("g_mat_view", &mat_view);
	ptr_effect->SetMatrix("g_mat_projection", &mat_proj);
	ptr_effect->CommitChanges();

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

					if (vertex_offset_ >= vertex_count_)
						vertex_offset_ = 0;

					vertex_buffer_->Lock(vertex_offset_ * vertex_size_, vertex_batch_size_ * vertex_size_
						, (void**)&vertex, vertex_offset_ ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);
				
					particles_in_batch = 0;
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

void Engine::CParticleSystem::Render_Billboard(LPD3DXEFFECT ptr_effect, const Matrix* mat_parent)
{
	/*Matrix parent;
	if (nullptr == mat_parent)
		D3DXMatrixIdentity(&parent);
	else
		parent = *mat_parent;*/

	for (auto& particle : particles_->list)
		particle.ptr_particle->ComputeViewZ(*ptr_camera_pos_);

	particles_->list.sort([](Engine::Attribute& ptr_src, Engine::Attribute& ptr_dst)->bool
	{
		return ptr_src.ptr_particle->view_z() > ptr_dst.ptr_particle->view_z();
	});

	ptr_effect->SetTexture("g_color_texture", particles_->ptr_texture->GetTexture(0));
	for (auto& particle : particles_->list)
		particle.ptr_particle->Render(ptr_effect, mat_parent);
}

int Engine::CParticleSystem::Release()
{
	if (--reference_count_ == 0)
	{
		if(particles_->type == ParticleType::Point)
			Safe_Release(vertex_buffer_);
		else
		{
			Safe_Release_Delete(particles_->ptr_texture);
			for (auto& attribute : particles_->list)
				Safe_Delete(attribute.ptr_particle);
		}

		Safe_Delete(particles_);
	}

	return reference_count_;
}

Engine::CParticleSystem * Engine::CParticleSystem::Create(LPDIRECT3DDEVICE9 ptr_device, DWORD max_particle, ParticleType type)
{
	CParticleSystem* ptr_particle = new CParticleSystem(ptr_device);
	if (FAILED(ptr_particle->Initialize(max_particle, type)))
	{
		Safe_Release_Delete(ptr_particle);
		assert(!"Particle System Create Failed");
	}

	return ptr_particle;
}

void Engine::CParticleSystem::Reset()
{
	for (auto& attribute : particles_->list)
		attribute.is_alive = false;
	particles_->acc_time = 0.f;
}

void Engine::CParticleSystem::Init_Particle(Attribute & particle)
{
	particle.is_alive = true;
	particle.position = (true == particles_->random_origin) ? GetRandomVector(particles_->origin_min, particles_->origin_max) : particles_->origin;
	particle.velocity = (true == particles_->random_velocity) ? GetRandomVector(particles_->velocity_min, particles_->velocity_max) : particles_->velocity;
	particle.velocity = particle.velocity.Normalize();
	particle.velocity *= particles_->speed;
	particle.gravity = particles_->gravity;
	particle.color = (true == particles_->random_color) ? D3DXCOLOR(RandomFloat(0.f, 1.f), RandomFloat(0.f, 1.f), RandomFloat(0.f, 1.f), 1.f) : particles_->color;
	particle.scale = (true == particles_->random_scale) ? Vector3(RandomFloat(particles_->scale_min, particles_->scale_max)) : particle.scale = Vector3(particles_->scale);
	particle.rotation = (true == particles_->random_rotation) ? GetRandomVector(Vector3(0.f, 0.f, 0.f), Vector3(6.28f, 6.28f, 6.28f)) : Vector3(0.f, 0.f, 0.f);
	particle.acc_time = 0.f;
	particle.life_time = particles_->life_time;

	switch (particles_->type)
	{
	case ParticleType::Billboard:
		if (nullptr == particle.ptr_particle)
			particle.ptr_particle = CBillboardParticle::Create(ptr_device_, particles_->mesh_stage_id, particles_->mesh_key, particles_->uv_animation, particles_->uv_loop, particles_->max_u, particles_->max_v);
		particle.ptr_particle->transform()->rotation() = particle.rotation;
		particle.ptr_particle->transform()->scale() = particle.scale;
		break;
	case ParticleType::Mesh:
		break;
	}
}

void Engine::CParticleSystem::Add_Particle()
{
	if (particle_count >= particles_->max_particle) return;

	Attribute particle;
	Init_Particle(particle);

	particles_->list.emplace_back(particle);
	++particle_count;
}

void Engine::CParticleSystem::Remove_End_Particle()
{
	auto iter = particles_->list.begin();
	auto iter_end = particles_->list.end();

	for (; iter != iter_end; )
	{
		if (false == iter->is_alive)
		{
			if (particles_->type != ParticleType::Point)
				Safe_Delete(iter->ptr_particle);

			iter = particles_->list.erase(iter);
			--particle_count;
		}
		else
			++iter;
	}
}
