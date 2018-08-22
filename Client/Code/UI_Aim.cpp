#include "stdafx.h"
#include "UI_Aim.h"

#include "Transform.h"
#include "Shader.h"
#include "RectTexture.h"
#include "Texture.h"

CUI_Aim::CUI_Aim(LPDIRECT3DDEVICE9 ptr_device)
	: CUI(ptr_device)
{
}

CUI_Aim::~CUI_Aim()
{
	Release();
}

void CUI_Aim::set_aim_state(int state)
{
	aim_state_ = state;
}

HRESULT CUI_Aim::Initialize()
{
	HRESULT hr = Engine::CGameObject::Initialize();
	assert(!FAILED(hr) && "UI_Aim Initialize Falied");
	hr = AddComponent();
	assert(!FAILED(hr) && "UI_Aim AddComponent Falied");

	point_ = Vector2(float(kWinCx >> 1), float(kWinCy >> 1));
	width_ = 40.f;
	height_ = 40.f;

	return S_OK;
}

void CUI_Aim::LateInit()
{
}

void CUI_Aim::Update(float delta_time)
{
	CUI::Update(delta_time);
}

void CUI_Aim::LateUpdate()
{
	Engine::GameManager()->AddRenderLayer(Engine::LAYER_UI, this);
}

void CUI_Aim::Render()
{
	LPD3DXEFFECT ptr_effect = ptr_shader_->GetEffectHandle();
	ptr_effect->SetMatrix("g_mat_world", &ptr_transform_->mat_world());
	ptr_effect->SetMatrix("g_mat_view", &mat_view_);
	ptr_effect->SetMatrix("g_mat_projection", &Subject()->GetOrthoProjection());
	ptr_effect->SetTexture("g_color_texture", ptr_texture_->GetTexture(aim_state_));
	
	ptr_shader_->BegineShader(0);
	ptr_buffer_->Render();
	ptr_shader_->EndShader();
}

CUI_Aim * CUI_Aim::Create(LPDIRECT3DDEVICE9 ptr_device)
{
	CUI_Aim* ptr_ui = new CUI_Aim(ptr_device);
	if (FAILED(ptr_ui->Initialize()))
	{
		Safe_Delete(ptr_ui);
		assert(!"UI_Hp Create Failed");
	}

	return ptr_ui;
}

HRESULT CUI_Aim::AddComponent()
{
	HRESULT hr = E_FAIL;

	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Component_Transform"), TEXT("Transform"), ptr_transform_);
	assert(!FAILED(hr) && "ReadyComponent Failed of CUI_Aim Tranform Component ");
	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Shader_UI"), TEXT("Shader"), ptr_shader_);
	assert(!FAILED(hr) && "ReadyComponent Failed of CUI_Aim Shader Component");
	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Buffer_RectTexture"), TEXT("Buffer"), ptr_buffer_);
	assert(hr == S_OK && "ReadyComponent Failed of CUI_Aim Buffer Component");
	hr = Ready_Component(MAINTAIN_STAGE, TEXT("Aim_UI_Texture"), TEXT("Texture"), ptr_texture_);
	assert(!FAILED(hr) && "ReadyComponent Failed of CUI_Aim Texture Component");

	return S_OK;
}

void CUI_Aim::Release()
{
}
