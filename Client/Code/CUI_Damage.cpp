#include "stdafx.h"
#include "CUI_Damage.h"

#include "Transform.h"
#include "Shader.h"
#include "RectTexture.h"
#include "Texture.h"

CUI_Damage::CUI_Damage(LPDIRECT3DDEVICE9 ptr_device)
	: CUI(ptr_device)
{
}

CUI_Damage::~CUI_Damage()
{
}

void CUI_Damage::set_aim_state(int state)
{
	Release();
}

HRESULT CUI_Damage::Initialize()
{
	HRESULT hr = Engine::CGameObject::Initialize();
	assert(!FAILED(hr) && "UI_Aim Initialize Falied");
	hr = AddComponent();
	assert(!FAILED(hr) && "UI_Aim AddComponent Falied");

	point_ = Vector2(float(kWinCx >> 1), float(kWinCy >> 1));
	width_ = float(kWinCx >> 1);
	height_ = float(kWinCy >> 1);
	alpha_ = 0.f;
	active_ = false;

	return S_OK;
}

void CUI_Damage::LateInit()
{
}

void CUI_Damage::Update(float delta_time)
{
	CUI::Update(delta_time);
	alpha_ -= 4.f * delta_time;
	if (alpha_ <= 0.f)
		active_ = false;
}

void CUI_Damage::LateUpdate()
{
	Engine::GameManager()->AddRenderLayer(Engine::LAYER_UI, this);
}

void CUI_Damage::Render()
{
	LPD3DXEFFECT ptr_effect = ptr_shader_->GetEffectHandle();
	ptr_effect->SetMatrix("g_mat_world", &ptr_transform_->mat_world());
	ptr_effect->SetMatrix("g_mat_view", &mat_view_);
	ptr_effect->SetMatrix("g_mat_projection", &Subject()->GetOrthoProjection());
	ptr_effect->SetTexture("g_color_texture", ptr_texture_->GetTexture(0));
	ptr_effect->SetFloat("g_alpha", alpha_);

	ptr_shader_->BegineShader(2);
	ptr_buffer_->Render();
	ptr_shader_->EndShader();
}

CUI_Damage * CUI_Damage::Create(LPDIRECT3DDEVICE9 ptr_device)
{
	CUI_Damage* ptr_ui = new CUI_Damage(ptr_device);
	if (FAILED(ptr_ui->Initialize()))
	{
		Safe_Delete(ptr_ui);
		assert(!"UI_Hp Create Failed");
	}

	return ptr_ui;
}

void CUI_Damage::OnEnable()
{
	CGameObject::OnDisable();
	alpha_ = 1.f;
}

HRESULT CUI_Damage::AddComponent()
{
	HRESULT hr = E_FAIL;

	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Component_Transform"), TEXT("Transform"), ptr_transform_);
	assert(!FAILED(hr) && "ReadyComponent Failed of CUI_Damage Tranform Component ");
	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Shader_UI"), TEXT("Shader"), ptr_shader_);
	assert(!FAILED(hr) && "ReadyComponent Failed of CUI_Damage Shader Component");
	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Buffer_RectTexture"), TEXT("Buffer"), ptr_buffer_);
	assert(hr == S_OK && "ReadyComponent Failed of CUI_Damage Buffer Component");
	hr = Ready_Component(MAINTAIN_STAGE, TEXT("Damage_UI_Texture"), TEXT("Texture"), ptr_texture_);
	assert(!FAILED(hr) && "ReadyComponent Failed of CUI_Damage Texture Component");

	return S_OK;

}

void CUI_Damage::Release()
{
}
