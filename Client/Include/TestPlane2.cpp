#include "stdafx.h"
#include "TestPlane2.h"

#include "Transform.h"
#include "Shader.h"
#include "StaticMesh.h"
#include "Texture.h"

CTestPlane2::CTestPlane2(LPDIRECT3DDEVICE9 ptr_device)
	: Engine::CGameObject(ptr_device)
{
}

CTestPlane2::~CTestPlane2()
{
	Release();
}

HRESULT CTestPlane2::Initialize()
{
	HRESULT hr = CGameObject::Initialize();
	assert(!FAILED(hr) && "CGameObject::Initialize call failed in TestPlane2");

	hr = AddComponent();
	assert(!FAILED(hr) && "AddComponent call failed in TestPlane2");

	ptr_transform_->scale() = Vector3(0.01f, 0.01f, 0.01f);
	return S_OK;
}

void CTestPlane2::Update(float delta_time)
{
	Engine::GameManager()->AddRenderLayer(Engine::RENDERLAYER::LAYER_NONEALPHA, this);

	Engine::CGameObject::Update(delta_time);
}

void CTestPlane2::Render()
{
	LPD3DXEFFECT ptr_effect = ptr_shader_->GetEffectHandle();

	Matrix mat_view, mat_proj;
	ptr_device_->GetTransform(D3DTS_VIEW, &mat_view);
	ptr_device_->GetTransform(D3DTS_PROJECTION, &mat_proj);

	ptr_effect->SetMatrix("g_mat_world", &ptr_transform_->mat_world());
	ptr_effect->SetMatrix("g_mat_view", &mat_view);
	ptr_effect->SetMatrix("g_mat_projection", &mat_proj);

	ptr_effect->SetVector("g_light_diffuse", &Vector4(1.f, 1.f, 1.f, 1.f));
	ptr_effect->SetVector("g_light_ambient", &Vector4(1.f, 1.f, 1.f, 1.f));
	ptr_effect->SetVector("g_light_dir", &Vector4(0.f, -1.f, 1.f, 0.f));

	ptr_mesh_->RenderMesh(ptr_effect);
}

CTestPlane2 * CTestPlane2::Create(LPDIRECT3DDEVICE9 ptr_device)
{
	CTestPlane2* ptr_obj = new CTestPlane2(ptr_device);
	if (FAILED(ptr_obj->Initialize()))
	{
		Safe_Delete(ptr_obj);
		assert(!"TestPlane Create Failed");
	}

	return ptr_obj;
}

HRESULT CTestPlane2::AddComponent()
{
	HRESULT hr = E_FAIL;

	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Component_Transform"), TEXT("Transform"), ptr_transform_);
	assert(!FAILED(hr) && "Tranform Component ReadyComponent Failed");
	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Shader_NormalMap"), TEXT("Shader"), ptr_shader_);
	assert(!FAILED(hr) && "ScreenImage Shader ReadyComponent Failed");

	return S_OK;
}

void CTestPlane2::Release()
{
}
