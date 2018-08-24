#include "stdafx.h"
#include "FireBall.h"

#include "Transform.h"
#include "Shader.h"
#include "StaticMesh.h"
#include "Collider.h"
#include "Explosion.h"
#include "FireBallEffect.h"

CFireBall::CFireBall(LPDIRECT3DDEVICE9 ptr_device)
	: Engine::CGameObject(ptr_device)
{
}

CFireBall::~CFireBall()
{
	Release();
}

HRESULT CFireBall::Initialize(const Vector3 & position, const Vector3 & move_dir)
{
	HRESULT hr = Engine::CGameObject::Initialize();
	assert(!FAILED(hr) && "Explosion Initialize Falied");
	hr = AddComponent();
	assert(!FAILED(hr) && "Explosion AddComponent Falied");

	ptr_transform_->position() = position;
	ptr_transform_->move_dir() = move_dir;
	ptr_transform_->scale() = Vector3(0.01f, 0.01f, 0.01f);

	life_time = 5.f;

	ptr_effect_ = CFireBallEffect::Create(ptr_device_, &ptr_transform_->mat_world());
	TCHAR effect_key[64] = TEXT("");
	wsprintf(effect_key, TEXT("FireBall_Effect"));
	Engine::GameManager()->GetCurrentScene()->AddObject(MAINTAIN_STAGE, effect_key, ptr_effect_);

	return S_OK;
}

void CFireBall::Update(float delta_time)
{
	if ((life_time -= delta_time) <= 0.f)
	{
		destroy_ = true;
		ptr_effect_->Destroy();
	}

	ptr_transform_->position() += ptr_transform_->move_dir() * 50.f * delta_time;
	CGameObject::Update(delta_time);
	ptr_sphere_coll_->SetSphereCollider(0.5f, Vector3());
}

void CFireBall::LateUpdate()
{
	Engine::GameManager()->AddRenderLayer(Engine::LAYER_NONEALPHA, this);

	const CollList& coll_list_player = CollSystem()->GetColliderList(TAG_PLAYER);
	for (auto& target_coll : coll_list_player)
	{
		if (true == ptr_sphere_coll_->TriggerCheck(target_coll))
		{
			target_coll->ptr_object()->ApplyDamage(80);
			destroy_ = true;
			CreateExplosionEffect();
			ptr_effect_->Destroy();
		}
	}
}

void CFireBall::Render()
{
	LPD3DXEFFECT ptr_effect = ptr_shader_->GetEffectHandle();
	if (nullptr == ptr_effect) return;

	Matrix mat_view, mat_proj;
	ptr_device_->GetTransform(D3DTS_VIEW, &mat_view);
	ptr_device_->GetTransform(D3DTS_PROJECTION, &mat_proj);

	ptr_effect->SetMatrix("g_mat_world", &ptr_transform_->mat_world());
	ptr_effect->SetMatrix("g_mat_view", &mat_view);
	ptr_effect->SetMatrix("g_mat_projection", &mat_proj);

	ptr_mesh_->RenderMesh(ptr_effect, 2);

#ifdef _DEBUG
	ptr_effect = ptr_debug_shader_->GetEffectHandle();
	Matrix mat_coll;
	D3DXMatrixIdentity(&mat_coll);
	mat_coll._41 = ptr_sphere_coll_->GetSpherePos().x;
	mat_coll._42 = ptr_sphere_coll_->GetSpherePos().y;
	mat_coll._43 = ptr_sphere_coll_->GetSpherePos().z;
	ptr_effect->SetMatrix("g_mat_world", &mat_coll);

	ptr_debug_shader_->BegineShader(1);
	ptr_sphere_coll_->DebugRender();
	ptr_debug_shader_->EndShader();
#endif // _DEBUG
}

CFireBall * CFireBall::Create(LPDIRECT3DDEVICE9 ptr_device, const Vector3 & position, const Vector3 & move_dir)
{
	CFireBall* ptr_obj = new CFireBall(ptr_device);
	if (FAILED(ptr_obj->Initialize(position, move_dir)))
	{
		Safe_Delete(ptr_obj);
		assert(!"Gun_Phobos Create Failed");
	}
	return ptr_obj;
}

HRESULT CFireBall::AddComponent()
{
	HRESULT hr = E_FAIL;

	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Component_Transform"), TEXT("Transform"), ptr_transform_);
	assert(hr == S_OK && "Tranform Component ReadyComponent Failed");
	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Shader_NormalMap"), TEXT("Shader"), ptr_shader_);
	assert(hr == S_OK && "CFireBall Shader_DynamicMesh ReadyComponent Failed");
	hr = Ready_Component(MAINTAIN_STAGE, TEXT("FireBall_Mesh"), TEXT("Mesh"), ptr_mesh_);
	assert(hr == S_OK && "Mesh ReadyComponent Failed");

	ptr_sphere_coll_ = Engine::CCollider::Create(ptr_device_, this, Collider_Sphere);

#ifdef _DEBUG
	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Shader_Default"), TEXT("Debug_Shader"), ptr_debug_shader_);
	assert(hr == S_OK && "Ork Shader_Default ReadyComponent Failed");
#endif

	return S_OK;
}

void CFireBall::Release()
{
	Safe_Release_Delete(ptr_sphere_coll_);
}

void CFireBall::CreateExplosionEffect()
{
	TCHAR effect_key[64] = TEXT("");
	wsprintf(effect_key, TEXT("FireBall_Explosion_Effect"));
	Engine::GameManager()->GetCurrentScene()->AddObject(MAINTAIN_STAGE, effect_key, CExplosion::Create(ptr_device_, ptr_transform_->position(), 2));
}
