#include "stdafx.h"
#include "Ork_Sword.h"

#include "Transform.h"
#include "Shader.h"
#include "StaticMesh.h"
#include "Collider.h"

COrk_Sword::COrk_Sword(LPDIRECT3DDEVICE9 ptr_device)
	: Engine::CGameObject(ptr_device)
{
	D3DXMatrixIdentity(&hand_matrix_);
}

COrk_Sword::~COrk_Sword()
{
	Release();
}

Engine::CCollider * COrk_Sword::ptr_sphere_coll()
{
	return ptr_sphere_coll_;
}

void COrk_Sword::SetParentMatrix(const Matrix * ptr_parent_matrix)
{
	ptr_parent_matrix_ = ptr_parent_matrix;
}

void COrk_Sword::SetHandMatrix(const Matrix * ptr_hand_matrix)
{
	hand_matrix_ = *ptr_hand_matrix;
}

HRESULT COrk_Sword::Initialize()
{
	HRESULT hr = CGameObject::Initialize();
	assert(!FAILED(hr) && "CGameObject::Initialize call failed in SpaceMarin");

	hr = AddComponent();
	assert(!FAILED(hr) && "AddComponent call failed in SpaceMarin");

	ptr_transform_->rotation().x = D3DXToRadian(90.f);
	ptr_sphere_coll_->enable_ = false;

	color_[0] = Vector4(0.7f, 0.52f, 0.42f, 1.f);
	color_[1] = Vector4(0.65f, 0.65f, 0.65f, 1.f);
	color_[2] = Vector4(0.52f, 0.34f, 0.25f, 1.f);

	return hr;
}

void COrk_Sword::Update(float delta_time)
{
	Engine::CGameObject::Update(delta_time);
	ptr_transform_->mat_world() *= hand_matrix_ * (*ptr_parent_matrix_);

	if (true == ptr_sphere_coll_->enable_)
		ptr_sphere_coll_->SetSphereCollider(0.3f, Vector3(0.f, 80.f, -40.f));
}

void COrk_Sword::LateUpdate()
{
	Engine::GameManager()->AddRenderLayer(Engine::RENDERLAYER::LAYER_NONEALPHA, this);
}

void COrk_Sword::Render()
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

COrk_Sword * COrk_Sword::Create(LPDIRECT3DDEVICE9 ptr_device)
{
	COrk_Sword* ptr_obj = new COrk_Sword(ptr_device);
	if (FAILED(ptr_obj->Initialize()))
	{
		Safe_Delete(ptr_obj);
		assert(!"Gun_Phobos Create Failed");
	}
	return ptr_obj;
}

HRESULT COrk_Sword::AddComponent()
{
	HRESULT hr = E_FAIL;

	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Component_Transform"), TEXT("Transform"), ptr_transform_);
	assert(hr == S_OK && "Tranform Component ReadyComponent Failed");
	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Shader_NormalMap"), TEXT("Shader"), ptr_shader_);
	assert(!FAILED(hr) && "ReadyComponent Failed of LevelObject Shader Component");
	hr = Ready_Component(MAINTAIN_STAGE, TEXT("Ork_Sword_Mesh"), TEXT("Mesh"), ptr_mesh_);
	assert(hr == S_OK && "Mesh ReadyComponent Failed");

	ptr_sphere_coll_ = Engine::CCollider::Create(ptr_device_, this, Collider_Sphere);

#ifdef _DEBUG
	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Shader_Default"), TEXT("Shader_Debug"), ptr_debug_shader_);
	assert(!FAILED(hr) && "ReadyComponent Failed of LevelObject Shader Component");
#endif
	return S_OK;
}

void COrk_Sword::Release()
{
	Safe_Release_Delete(ptr_sphere_coll_);
}
