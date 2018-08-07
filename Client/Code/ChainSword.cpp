#include "stdafx.h"
#include "ChainSword.h"

#include "Transform.h"
#include "Shader.h"
#include "DynamicMesh.h"
#include "Collider.h"
#include "AnimController.h"

CChainSword::CChainSword(LPDIRECT3DDEVICE9 ptr_device)
	: Engine::CGameObject(ptr_device)
{
	D3DXMatrixIdentity(&hand_matrix_);
}

CChainSword::~CChainSword()
{
	Release();
}

Engine::CCollider * CChainSword::ptr_sphere_coll()
{
	return ptr_sphere_coll_;
}

void CChainSword::SetParentMatrix(const Matrix * ptr_parent_matrix)
{
	ptr_parent_matrix_ = ptr_parent_matrix;
}

void CChainSword::SetHandMatrix(const Matrix * ptr_hand_matrix)
{
	hand_matrix_ = *ptr_hand_matrix;
}

void CChainSword::set_anim_speed(float anim_speed)
{
	anim_speed_ = anim_speed;
}

HRESULT CChainSword::Initialize()
{
	HRESULT hr = CGameObject::Initialize();
	assert(!FAILED(hr) && "CGameObject::Initialize call failed in SpaceMarin");

	hr = AddComponent();
	assert(!FAILED(hr) && "AddComponent call failed in SpaceMarin");

	ptr_transform_->position().x = -2.f;
	ptr_transform_->position().y = 2.f;
	ptr_transform_->position().z = -10.f;

	ptr_transform_->rotation().x = D3DXToRadian(10.f);
	ptr_transform_->rotation().y = D3DXToRadian(-20.f);
	ptr_transform_->rotation().z = D3DXToRadian(100.f);

	ptr_transform_->scale() = Vector3(100.f, 100.f, 100.f);

	ptr_sphere_coll_->enable_ = false;

	ptr_anim_ctrl_->SetAnimationTrack((uint32)0);
	ptr_anim_ctrl_->SetTrackPosition(0.0);
	anim_speed_ = 0.1f;

	return hr;
}

void CChainSword::Update(float delta_time)
{
	Engine::CGameObject::Update(delta_time);
	ptr_transform_->mat_world() *= hand_matrix_ * (*ptr_parent_matrix_);
	anim_time_ = delta_time;

	if (true == ptr_sphere_coll_->enable_)
		ptr_sphere_coll_->SetSphereCollider(0.5f, Vector3(0.f, 1.2f, 0.f));
}

void CChainSword::LateUpdate()
{
	Engine::GameManager()->AddRenderLayer(Engine::RENDERLAYER::LAYER_NONEALPHA, this);
	const CollList& enemy_coll_list = CollSystem()->GetColliderList(TAG_ENEMY);
	for (auto& enemy_coll : enemy_coll_list)
	{
		if (true == ptr_sphere_coll_->TriggerCheck(enemy_coll))
			enemy_coll->ptr_object()->ApplyDamage(100);
	}
}

void CChainSword::Render()
{
	LPD3DXEFFECT ptr_effect = ptr_shader_->GetEffectHandle();
	if (nullptr == ptr_effect) return;

	Matrix mat_view, mat_proj;
	ptr_device_->GetTransform(D3DTS_VIEW, &mat_view);
	ptr_device_->GetTransform(D3DTS_PROJECTION, &mat_proj);

	ptr_effect->SetMatrix("g_mat_world", &ptr_transform_->mat_world());
	ptr_effect->SetMatrix("g_mat_view", &mat_view);
	ptr_effect->SetMatrix("g_mat_projection", &mat_proj);

	ptr_mesh_->FrameMove(anim_time_, ptr_anim_ctrl_);
	ptr_mesh_->RenderMesh(ptr_effect);

#ifdef _DEBUG
	if (true == ptr_sphere_coll_->enable_)
	{
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
	}
#endif // _DEBUG
}

CChainSword * CChainSword::Create(LPDIRECT3DDEVICE9 ptr_device)
{
	CChainSword* ptr_obj = new CChainSword(ptr_device);
	if (FAILED(ptr_obj->Initialize()))
	{
		Safe_Delete(ptr_obj);
		assert(!"Gun_Phobos Create Failed");
	}
	return ptr_obj;
}

HRESULT CChainSword::AddComponent()
{
	HRESULT hr = E_FAIL;

	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Component_Transform"), TEXT("Transform"), ptr_transform_);
	assert(hr == S_OK && "ReadyComponent Failed of ChainSword Tranform Component");
	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Shader_DynamicMesh"), TEXT("Shader"), ptr_shader_);
	assert(!FAILED(hr) && "ReadyComponent Failed of ChainSword Shader Component");
	hr = Ready_Component(MAINTAIN_STAGE, TEXT("ChainSword_Mesh"), TEXT("Mesh"), ptr_mesh_);
	assert(hr == S_OK && "ReadyComponent Failed of ChainSword Mesh Component");
	ptr_anim_ctrl_ = ptr_mesh_->CloneAnimController();

	ptr_sphere_coll_ = Engine::CCollider::Create(ptr_device_, this, Collider_Sphere);

#ifdef _DEBUG
	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Shader_Default"), TEXT("Shader_Debug"), ptr_debug_shader_);
	assert(!FAILED(hr) && "ReadyComponent Failed of LevelObject Shader Component");
#endif
	return S_OK;
}

void CChainSword::Release()
{
	Safe_Delete(ptr_anim_ctrl_);
	Safe_Release_Delete(ptr_sphere_coll_);
}
