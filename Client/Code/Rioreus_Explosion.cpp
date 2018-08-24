#include "stdafx.h"
#include "Rioreus_Explosion.h"

#include "Transform.h"
#include "Shader.h"
#include "ParticleSystem.h"
#include "Collider.h"

CRioreus_Explosion::CRioreus_Explosion(LPDIRECT3DDEVICE9 ptr_device)
	: Engine::CGameObject(ptr_device)
{
}

CRioreus_Explosion::~CRioreus_Explosion()
{
	Release();
}

HRESULT CRioreus_Explosion::Initialize(const Vector3 & position)
{
	HRESULT hr = Engine::CGameObject::Initialize();
	assert(!FAILED(hr) && "Explosion Initialize Falied");
	hr = AddComponent();
	assert(!FAILED(hr) && "Explosion AddComponent Falied");

	ptr_transform_->position() = position;
	life_time = ptr_particle_->particles()->duration;
	ptr_particle_->SetCameraPos(Subject()->camera_pos());

	return S_OK;
}

void CRioreus_Explosion::Update(float delta_time)
{
	if ((life_time -= delta_time) <= 0.f)
		destroy_ = true;

	ComputeViewZ(*Subject()->camera_pos());

	CGameObject::Update(delta_time);
	ptr_transform_->LookAt_XY(*Subject()->camera_pos());
	ptr_collider_->SetSphereCollider(5.f, Vector3());
}

void CRioreus_Explosion::LateUpdate()
{
	Engine::GameManager()->AddRenderLayer(Engine::LAYER_ALPHA, this);

	const CollList& coll_list_player = CollSystem()->GetColliderList(TAG_PLAYER);
	for (auto& target_coll : coll_list_player)
	{
		if (true == ptr_collider_->TriggerCheck(target_coll))
			target_coll->ptr_object()->ApplyDamage(100);
	}
}

void CRioreus_Explosion::Render()
{
	LPD3DXEFFECT ptr_effect = ptr_shader_->GetEffectHandle();

	ptr_effect->Begin(nullptr, 0);
	ptr_particle_->Render(ptr_effect, &ptr_transform_->mat_world());
	ptr_effect->End();

#ifdef _DEBUG
	ptr_effect = ptr_debug_shader_->GetEffectHandle();
	Matrix mat_coll;
	D3DXMatrixIdentity(&mat_coll);
	mat_coll._41 = ptr_collider_->GetSpherePos().x;
	mat_coll._42 = ptr_collider_->GetSpherePos().y;
	mat_coll._43 = ptr_collider_->GetSpherePos().z;
	ptr_effect->SetMatrix("g_mat_world", &mat_coll);

	ptr_debug_shader_->BegineShader(1);
	ptr_collider_->DebugRender();
	ptr_debug_shader_->EndShader();
#endif // _DEBUG
}

CRioreus_Explosion * CRioreus_Explosion::Create(LPDIRECT3DDEVICE9 ptr_device, const Vector3 & position)
{
	CRioreus_Explosion* ptr_obj = new CRioreus_Explosion(ptr_device);
	if (FAILED(ptr_obj->Initialize(position)))
	{
		Safe_Delete(ptr_obj);
		assert(!"Explosion Create Failed");
	}

	return ptr_obj;
}

HRESULT CRioreus_Explosion::AddComponent()
{
	HRESULT hr = E_FAIL;

	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Component_Transform"), TEXT("Transform"), ptr_transform_);
	assert(!FAILED(hr) && "ReadyComponent Failed of Bomb Tranform Component ");
	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Shader_Mesh_Effect"), TEXT("Shader"), ptr_shader_);
	assert(!FAILED(hr) && "ReadyComponent Failed of Bomb Shader Component");
	hr = Ready_Component(MAINTAIN_STAGE, TEXT("Explosion_3"), TEXT("Effect"), ptr_particle_);
	assert(!FAILED(hr) && "ReadyComponent Failed of Bomb ParticleSystem Component");

	ptr_collider_ = Engine::CCollider::Create(ptr_device_, this, Collider_Sphere);
#ifdef _DEBUG
	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Shader_Default"), TEXT("Debug_Shader"), ptr_debug_shader_);
	assert(hr == S_OK && "Ork Shader_Default ReadyComponent Failed");
#endif

	return S_OK;
}

void CRioreus_Explosion::Release()
{
	Safe_Release_Delete(ptr_collider_);
}
