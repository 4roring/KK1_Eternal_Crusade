// ParticleEditor.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ParticleEditor.h"
#include "MapTool.h"
#include "afxdialogex.h"

#include "ParticleSystem.h"
#include "Shader.h"

// ParticleEditor 대화 상자입니다.

IMPLEMENT_DYNAMIC(ParticleEditor, CPropertyPage)

ParticleEditor::ParticleEditor()
	: CPropertyPage(IDD_PARTICLEEDITOR)
{

}

ParticleEditor::~ParticleEditor()
{
}

void ParticleEditor::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}

void ParticleEditor::Update(float delta_time)
{
	ptr_particle_->Add_Particle();
	ptr_particle_->Update(delta_time);
}

void ParticleEditor::Render()
{
	LPD3DXEFFECT ptr_effect = ptr_shader_->GetEffectHandle();

	Matrix mat_world, mat_view, mat_proj;
	D3DXMatrixIdentity(&mat_world);
	ptr_device_->GetTransform(D3DTS_VIEW, &mat_view);
	ptr_device_->GetTransform(D3DTS_PROJECTION, &mat_proj);

	ptr_effect->SetMatrix("g_mat_world", &mat_world);
	ptr_effect->SetMatrix("g_mat_view", &mat_view);
	ptr_effect->SetMatrix("g_mat_projection", &mat_proj);

	ptr_shader_->BegineShader(0);
	ptr_particle_->Render();
	ptr_shader_->EndShader();
}


BEGIN_MESSAGE_MAP(ParticleEditor, CPropertyPage)
END_MESSAGE_MAP()


// ParticleEditor 메시지 처리기입니다.


BOOL ParticleEditor::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	ptr_device_ = Engine::GraphicDevice()->GetDevice();
	ptr_shader_ = Engine::CShader::Create(ptr_device_, TEXT("../../Reference/Shader/ParticleEffectShader.hlsl"));
	ptr_particle_ = Engine::CParticleSystem::Create(ptr_device_);
	ptr_particle_->Reset();

	Tool()->SetParticleEditor(this);
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void ParticleEditor::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	Engine::Safe_Release_Delete(ptr_shader_);
	Engine::Safe_Release_Delete(ptr_particle_);

	CPropertyPage::PostNcDestroy();
}


BOOL ParticleEditor::OnSetActive()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	Tool()->SetTab(CToolManager::TabID::Particle);

	return CPropertyPage::OnSetActive();
}
