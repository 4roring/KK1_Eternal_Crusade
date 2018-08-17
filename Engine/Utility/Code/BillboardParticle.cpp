#include "BillboardParticle.h"
#include "GameManager.h"

#include "Transform.h"
#include "RectTexture.h"


Engine::CBillboardParticle::CBillboardParticle(LPDIRECT3DDEVICE9 ptr_device)
	: CParticle(ptr_device)
{
}

Engine::CBillboardParticle::~CBillboardParticle()
{
	Release();
}

HRESULT Engine::CBillboardParticle::Initialize(int buffer_stage_id, const std::wstring& buffer_key, bool is_uv_anim, bool uv_anim_loop, float max_u, float max_v)
{ 
	CParticle::Initialize();

	ptr_buffer_ = dynamic_cast<CRectTexture*>(CGameManager::GetInstance()->CloneComponent(buffer_stage_id, buffer_key));

	if (true == is_uv_anim)
	{
		type_ = Type::UV;
		max_u_ = max_u;
		max_v_ = max_v;
		uv_anim_loop_ = uv_anim_loop;
	}
	else
		type_ = Type::Normal;
	
	return S_OK;
}

void Engine::CBillboardParticle::Update(float delta_time, const Vector3& position, const Vector3& rotation, const Vector3& scale, const Vector3& camera_pos)
{
	frame_num_ += max_u_ * max_v_ * delta_time;
	if (frame_num_ >= max_u_ && scene_num_ < max_v_)
	{
		frame_num_ = 0.f;
		if (++scene_num_ >= max_v_ && uv_anim_loop_ == true)
			scene_num_ = 0.f;
	}

	CParticle::Update(delta_time, position, rotation, scale, camera_pos);
}

void Engine::CBillboardParticle::Render(LPD3DXEFFECT ptr_effect, const Matrix * mat_parent)
{
	CParticle::Render(ptr_effect, mat_parent);

	uint32 pass_index = 0;

	if (type_ == Type::UV)
	{
		ptr_effect->SetFloat("g_max_u", max_u_);
		ptr_effect->SetFloat("g_max_v", max_v_);
		ptr_effect->SetInt("g_frame_num", (int)frame_num_);
		ptr_effect->SetInt("g_scene_num", (int)scene_num_);
		pass_index = 1;
	}

	ptr_effect->CommitChanges();

	ptr_effect->BeginPass(pass_index);
	ptr_buffer_->Render();
	ptr_effect->EndPass();
}

void Engine::CBillboardParticle::Release()
{
	Safe_Release(ptr_buffer_);
}

Engine::CBillboardParticle * Engine::CBillboardParticle::Create(LPDIRECT3DDEVICE9 ptr_device, int buffer_stage_id, const std::wstring& buffer_key, bool is_uv_anim, bool uv_anim_loop, float max_u, float max_v)
{
	CBillboardParticle* ptr_particle = new CBillboardParticle(ptr_device);
	if (FAILED(ptr_particle->Initialize(buffer_stage_id, buffer_key, is_uv_anim, uv_anim_loop, max_u, max_v)))
	{
		Safe_Delete(ptr_particle);
		assert(!"Particle Create Failed");
	}

	return ptr_particle;
}
