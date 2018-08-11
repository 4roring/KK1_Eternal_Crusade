#include "stdafx.h"
#include "Ork_HeavyGun.h"

#include "Transform.h"
#include "Shader.h"
#include "StaticMesh.h"

#ifdef _DEBUG
#include "Sphere.h"
#endif

COrk_HeavyGun::COrk_HeavyGun(LPDIRECT3DDEVICE9 ptr_device)
	: Engine::CGameObject(ptr_device)
{
	D3DXMatrixIdentity(&back_matrix_);
}

COrk_HeavyGun::~COrk_HeavyGun()
{
	Release();
}

Vector3 & COrk_HeavyGun::GetFirePos(int index) const
{
	return *(Vector3*)&mat_muzzle_of_gun_[index].m[3][0];
}

void COrk_HeavyGun::SetParentMatrix(const Matrix * ptr_parent_matrix)
{
	ptr_parent_matrix_ = ptr_parent_matrix;
}

void COrk_HeavyGun::SetBackMatrix(const Matrix * ptr_back_matrix)
{
	back_matrix_ = *ptr_back_matrix;
}

HRESULT COrk_HeavyGun::Initialize()
{
	HRESULT hr = CGameObject::Initialize();
	assert(!FAILED(hr) && "CGameObject::Initialize call failed in SpaceMarin");

	hr = AddComponent();
	assert(!FAILED(hr) && "AddComponent call failed in SpaceMarin");

	ptr_transform_->position().x = 5.f;
	//ptr_transform_->position().y = 5.f;
	ptr_transform_->position().z = 65.f;

	//ptr_transform_->rotation().x = D3DXToRadian(90.f);
	ptr_transform_->rotation().y = D3DXToRadian(90.f);
	ptr_transform_->rotation().z = D3DXToRadian(90.f);

	color_[0] = Vector4(0.4f, 0.05f, 0.05f, 1.f);
	color_[1] = Vector4(0.4f, 0.4f, 0.4f, 1.f);
	color_[2] = Vector4(0.25f, 0.2f, 0.17f, 1.f);

	return hr;

}

void COrk_HeavyGun::Update(float delta_time)
{
	Engine::CGameObject::Update(delta_time);
	ptr_transform_->mat_world() *= back_matrix_ * (*ptr_parent_matrix_);

	for (int i = 0; i < 2; ++i)
	{
		D3DXMatrixTranslation(&mat_muzzle_of_gun_[i], -50.f, (-25.f * i), -190.f);
		mat_muzzle_of_gun_[i] *= ptr_transform_->mat_world();
	}
}

void COrk_HeavyGun::LateUpdate()
{
	Engine::GameManager()->AddRenderLayer(Engine::RENDERLAYER::LAYER_NONEALPHA, this);
}

void COrk_HeavyGun::Render()
{
	LPD3DXEFFECT ptr_effect = ptr_shader_->GetEffectHandle();
	if (nullptr == ptr_effect) return;

	Matrix mat_view, mat_proj;
	ptr_device_->GetTransform(D3DTS_VIEW, &mat_view);
	ptr_device_->GetTransform(D3DTS_PROJECTION, &mat_proj);

	ptr_effect->SetMatrix("g_mat_world", &ptr_transform_->mat_world());
	ptr_effect->SetMatrix("g_mat_view", &mat_view);
	ptr_effect->SetMatrix("g_mat_projection", &mat_proj);

	ptr_effect->SetVector("set_color_r", &color_[0]);
	ptr_effect->SetVector("set_color_g", &color_[1]);
	ptr_effect->SetVector("set_color_b", &color_[2]);

	ptr_mesh_->RenderMesh(ptr_effect, 1);

#ifdef _DEBUG
	LPD3DXEFFECT ptr_debug_effect = ptr_debug_shader_->GetEffectHandle();

	ptr_debug_effect->SetMatrix("g_mat_world", &mat_muzzle_of_gun_[0]);

	ptr_debug_shader_->BegineShader(1);
	ptr_debug_fire_pos_->Render();
	ptr_debug_shader_->EndShader();

	ptr_debug_effect->SetMatrix("g_mat_world", &mat_muzzle_of_gun_[1]);

	ptr_debug_shader_->BegineShader(1);
	ptr_debug_fire_pos_->Render();
	ptr_debug_shader_->EndShader();
#endif

}

COrk_HeavyGun * COrk_HeavyGun::Create(LPDIRECT3DDEVICE9 ptr_device)
{
	COrk_HeavyGun* ptr_obj = new COrk_HeavyGun(ptr_device);
	if (FAILED(ptr_obj->Initialize()))
	{
		Safe_Delete(ptr_obj);
		assert(!"Ork_HeavyGun Create Failed");
	}
	return ptr_obj;
}

HRESULT COrk_HeavyGun::AddComponent()
{
	HRESULT hr = E_FAIL;

	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Component_Transform"), TEXT("Transform"), ptr_transform_);
	assert(hr == S_OK && "Tranform Component ReadyComponent Failed");
	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Shader_NormalMap"), TEXT("Shader"), ptr_shader_);
	assert(!FAILED(hr) && "ReadyComponent Failed of LevelObject Shader Component");
	hr = Ready_Component(MAINTAIN_STAGE, TEXT("Ork_HeavyGun_Mesh"), TEXT("Mesh"), ptr_mesh_);
	assert(hr == S_OK && "Mesh ReadyComponent Failed");

#ifdef _DEBUG
	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Shader_Default"), TEXT("Shader_Debug"), ptr_debug_shader_);
	assert(!FAILED(hr) && "ReadyComponent Failed of LevelObject Shader Component");
	ptr_debug_fire_pos_ = Engine::CSphere::Create(ptr_device_, 10.f, 8, 5);
#endif
	return S_OK;

}

void COrk_HeavyGun::Release()
{
#ifdef _DEBUG
	Safe_Delete(ptr_debug_fire_pos_);
#endif
}
