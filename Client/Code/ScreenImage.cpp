#include "stdafx.h"
#include "ScreenImage.h"

#include "Texture.h"
#include "VIBuffer.h"
#include "Shader.h"

CScreenImage::CScreenImage(LPDIRECT3DDEVICE9 ptr_device)
	: Engine::CGameObject(ptr_device)
{
}

CScreenImage::~CScreenImage()
{
	Release();
}

HRESULT CScreenImage::Initialize()
{
	HRESULT hr = CGameObject::Initialize();
	assert(!FAILED(hr) && "CGameObject::Initialize call failed in ScreenImage");

	hr = AddComponent();
	assert(!FAILED(hr) && "AddComponent call failed in ScreenImage");

	return S_OK;
}

void CScreenImage::Update(float delta_time)
{
	Engine::GameManager()->AddRenderLayer(Engine::RENDERLAYER::LAYER_NONEALPHA, this);
	Engine::CGameObject::Update(delta_time);
}

void CScreenImage::Render()
{
	LPD3DXEFFECT ptr_effect = ptr_shader_->GetEffectHandle();
	assert(nullptr != ptr_effect && "Shader Effect Handle is nullptr");

	D3DXMATRIX mat_identity;
	D3DXMatrixIdentity(&mat_identity);

	ptr_effect->SetMatrix("g_mat_world", &mat_identity);
	ptr_effect->SetMatrix("g_mat_view", &mat_identity);
	ptr_effect->SetMatrix("g_mat_projection", &mat_identity);
	ptr_effect->SetTexture("g_color_texture", ptr_texture_->GetTexture(0));

	ptr_shader_->BegineShader(0);
	ptr_buffer_->Render();
	ptr_shader_->EndShader();
}

CScreenImage * CScreenImage::Create(LPDIRECT3DDEVICE9 ptr_device)
{
	CScreenImage* ptr_object = new CScreenImage(ptr_device);
	if (FAILED(ptr_object->Initialize()))
	{
		Safe_Delete(ptr_object);
		assert(!"ScreenImage Object Create Failed");
	}
	return ptr_object;
}

HRESULT CScreenImage::AddComponent()
{
	HRESULT hr = E_FAIL;

	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Buffer_RectTexture"), TEXT("Buffer"), ptr_buffer_);
	assert(hr == S_OK && "ScreenImage Buffer ReadyComponent Failed");
	hr = Ready_Component(MAINTAIN_LOGO, TEXT("Logo_Texture"), TEXT("Texture"), ptr_texture_);
	assert(hr == S_OK && "ScreenImage Texture ReadyComponent Failed");
	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Shader_Default"), TEXT("Shader"), ptr_shader_);
	assert(hr == S_OK && "ScreenImage Shader ReadyComponent Failed");

	return S_OK;
}

void CScreenImage::Release()
{
}
