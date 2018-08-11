// ParticleEditor.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ParticleEditor.h"
#include "MapTool.h"
#include "afxdialogex.h"

#include "ParticleSystem.h"
#include "Shader.h"

// ParticleEditor ��ȭ �����Դϴ�.

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


// ParticleEditor �޽��� ó�����Դϴ�.


BOOL ParticleEditor::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	ptr_device_ = Engine::GraphicDevice()->GetDevice();
	ptr_shader_ = Engine::CShader::Create(ptr_device_, TEXT("../../Reference/Shader/ParticleEffectShader.hlsl"));
	ptr_particle_ = Engine::CParticleSystem::Create(ptr_device_);
	ptr_particle_->Reset();

	Tool()->SetParticleEditor(this);
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void ParticleEditor::PostNcDestroy()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	Engine::Safe_Release_Delete(ptr_shader_);
	Engine::Safe_Release_Delete(ptr_particle_);

	CPropertyPage::PostNcDestroy();
}


BOOL ParticleEditor::OnSetActive()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	Tool()->SetTab(CToolManager::TabID::Particle);

	return CPropertyPage::OnSetActive();
}
