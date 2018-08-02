#include "stdafx.h"
#include "Ork_Gun.h"

#include "Transform.h"
#include "Shader.h"
#include "StaticMesh.h"

#ifdef _DEBUG
#include "Sphere.h"
#endif

COrk_Gun::COrk_Gun(LPDIRECT3DDEVICE9 ptr_device)
	: Engine::CGameObject(ptr_device)
{
	D3DXMatrixIdentity(&hand_matrix_);
}

COrk_Gun::~COrk_Gun()
{
	Release();
}

Vector3 & COrk_Gun::GetFirePos() const
{
	return *(Vector3*)&mat_muzzle_of_gun_.m[3][0];
}

void COrk_Gun::SetParentMatrix(const Matrix * ptr_parent_matrix)
{
	ptr_parent_matrix_ = ptr_parent_matrix;
}

void COrk_Gun::SetHandMatrix(const Matrix * ptr_hand_matrix)
{
	hand_matrix_ = *ptr_hand_matrix;
}

HRESULT COrk_Gun::Initialize()
{
	HRESULT hr = CGameObject::Initialize();
	assert(!FAILED(hr) && "CGameObject::Initialize call failed in SpaceMarin");

	hr = AddComponent();
	assert(!FAILED(hr) && "AddComponent call failed in SpaceMarin");

	//ptr_transform_->position().x = 4.f;
	//ptr_transform_->position().y = 5.f;
	//ptr_transform_->position().z = -15.f;

	ptr_transform_->rotation().x = D3DXToRadian(90.f);
	//ptr_transform_->rotation().y = D3DXToRadian(5.f);
	//ptr_transform_->rotation().z = D3DXToRadian(90.f);

	return hr;
}

void COrk_Gun::Update(float delta_time)
{
	Engine::CGameObject::Update(delta_time);
	ptr_transform_->mat_world() *= hand_matrix_ * (*ptr_parent_matrix_);

	D3DXMatrixTranslation(&mat_muzzle_of_gun_, 0.f, 22.f, -75.f);
	mat_muzzle_of_gun_ *= ptr_transform_->mat_world();
}

void COrk_Gun::LateUpdate()
{
	Engine::GameManager()->AddRenderLayer(Engine::RENDERLAYER::LAYER_NONEALPHA, this);
}

void COrk_Gun::Render()
{
	LPD3DXEFFECT ptr_effect = ptr_shader_->GetEffectHandle();
	if (nullptr == ptr_effect) return;

	Matrix mat_view, mat_proj;
	ptr_device_->GetTransform(D3DTS_VIEW, &mat_view);
	ptr_device_->GetTransform(D3DTS_PROJECTION, &mat_proj);

	ptr_effect->SetMatrix("g_mat_world", &ptr_transform_->mat_world());
	ptr_effect->SetMatrix("g_mat_view", &mat_view);
	ptr_effect->SetMatrix("g_mat_projection", &mat_proj);

	ptr_mesh_->RenderMesh(ptr_effect);

#ifdef _DEBUG
	LPD3DXEFFECT ptr_debug_effect = ptr_debug_shader_->GetEffectHandle();

	ptr_debug_effect->SetMatrix("g_mat_world", &mat_muzzle_of_gun_);
	ptr_debug_effect->SetMatrix("g_mat_view", &mat_view);
	ptr_debug_effect->SetMatrix("g_mat_projection", &mat_proj);

	ptr_debug_shader_->BegineShader(1);
	ptr_debug_fire_pos_->Render();
	ptr_debug_shader_->EndShader();
#endif

}

COrk_Gun * COrk_Gun::Create(LPDIRECT3DDEVICE9 ptr_device)
{
	COrk_Gun* ptr_obj = new COrk_Gun(ptr_device);
	if (FAILED(ptr_obj->Initialize()))
	{
		Safe_Delete(ptr_obj);
		assert(!"Ork_Gun Create Failed");
	}
	return ptr_obj;
}

HRESULT COrk_Gun::AddComponent()
{
	HRESULT hr = E_FAIL;

	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Component_Transform"), TEXT("Transform"), ptr_transform_);
	assert(hr == S_OK && "Tranform Component ReadyComponent Failed");
	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Shader_NormalMap"), TEXT("Shader"), ptr_shader_);
	assert(!FAILED(hr) && "ReadyComponent Failed of LevelObject Shader Component");
	hr = Ready_Component(MAINTAIN_STAGE, TEXT("Ork_Gun_Mesh"), TEXT("Mesh"), ptr_mesh_);
	assert(hr == S_OK && "Mesh ReadyComponent Failed");

#ifdef _DEBUG
	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Shader_Default"), TEXT("Shader_Debug"), ptr_debug_shader_);
	assert(!FAILED(hr) && "ReadyComponent Failed of LevelObject Shader Component");
	ptr_debug_fire_pos_ = Engine::CSphere::Create(ptr_device_, 10.f, 8, 5);
#endif
	return S_OK;
}

void COrk_Gun::Release()
{
#ifdef _DEBUG
	Safe_Delete(ptr_debug_fire_pos_);
#endif
}
