#include "stdafx.h"
#include "Skybox.h"
#include "Transform.h"
#include "Shader.h"
#include "CubeTexture.h"
#include "Texture.h"

CSkybox::CSkybox(LPDIRECT3DDEVICE9 ptr_device)
	: Engine::CGameObject(ptr_device)
{
}

CSkybox::~CSkybox()
{
	Release();
}

HRESULT CSkybox::Initialize()
{
	HRESULT hr = CGameObject::Initialize();
	assert(!FAILED(hr) && "Skybox Initialize Failed");
	hr = AddComponent();
	assert(!FAILED(hr) && "Skybox AddComponent Failed");

	ptr_transform_->scale() = Vector3(10.f, 10.f, 10.f);

	return hr;
}

void CSkybox::Update(float delta_time)
{
	Matrix inv_mat_view;
	ptr_device_->GetTransform(D3DTS_VIEW, &inv_mat_view);
	D3DXMatrixInverse(&inv_mat_view, nullptr, &inv_mat_view);

	ptr_transform_->position() = *(Vector3*)&inv_mat_view.m[3][0];

	CGameObject::Update(delta_time);
}

void CSkybox::LateUpdate()
{
	Engine::GameManager()->AddRenderLayer(Engine::LAYER_PRIORITY, this);
}

void CSkybox::Render()
{
	LPD3DXEFFECT ptr_effect = ptr_shader_->GetEffectHandle();
	if (nullptr == ptr_effect) return;

	Matrix mat_view, mat_proj;
	ptr_device_->GetTransform(D3DTS_VIEW, &mat_view);
	ptr_device_->GetTransform(D3DTS_PROJECTION, &mat_proj);

	ptr_effect->SetMatrix("g_mat_world", &ptr_transform_->mat_world());
	ptr_effect->SetMatrix("g_mat_view", &mat_view);
	ptr_effect->SetMatrix("g_mat_projection", &mat_proj);
	ptr_effect->SetTexture("g_color_texture", ptr_texture_->GetTexture(0));

	ptr_shader_->BegineShader(0);
	ptr_buffer_->Render();
	ptr_shader_->EndShader();
}

CSkybox * CSkybox::Create(LPDIRECT3DDEVICE9 ptr_device)
{
	CSkybox* ptr_skybox = new CSkybox(ptr_device);
	if (FAILED(ptr_skybox->Initialize()))
	{
		Engine::Safe_Delete(ptr_skybox);
		return nullptr;
	}

	return ptr_skybox;
}

HRESULT CSkybox::AddComponent()
{
	HRESULT hr = E_FAIL;

	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Component_Transform"), TEXT("Transform"), ptr_transform_);
	assert(!FAILED(hr) && "ReadyComponent Failed of Skybox Tranform Component ");
	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Shader_Sky"), TEXT("Shader"), ptr_shader_);
	assert(!FAILED(hr) && "ReadyComponent Failed of Skybox Shader Component");
	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Buffer_Cube"), TEXT("Buffer"), ptr_buffer_);
	assert(!FAILED(hr) && "ReadyComponent Failed of Skybox CubeTex Component");
	hr = Ready_Component(MAINTAIN_STAGE, TEXT("Sky_Texture"), TEXT("Texture"), ptr_texture_);
	assert(!FAILED(hr) && "ReadyComponent Failed of Skybox StaticMesh Component");

	return S_OK;
}

void CSkybox::Release()
{
}
