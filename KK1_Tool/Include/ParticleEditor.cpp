// ParticleEditor.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ParticleEditor.h"
#include "MapTool.h"
#include "afxdialogex.h"
#include "StageEditor.h"

#include "ParticleSystem.h"
#include "Shader.h"
#include "ResourceLoader.h"

// ParticleEditor 대화 상자입니다.

IMPLEMENT_DYNAMIC(ParticleEditor, CPropertyPage)

ParticleEditor::ParticleEditor()
	: CPropertyPage(IDD_PARTICLEEDITOR)
	, max_particle_(100)
	, duration_(1)
	, loop_(TRUE)
	, origin_random_(FALSE)
	, origin_x_(0)
	, origin_y_(0)
	, origin_z_(0)
	, origin_min_x_(0)
	, origin_min_y_(0)
	, origin_min_z_(0)
	, origin_max_x_(0)
	, origin_max_y_(0)
	, origin_max_z_(0)
	, velocity_random_(TRUE)
	, velocity_x_(0)
	, velocity_y_(0)
	, velocity_z_(0)
	, velocity_min_x_(-1.f)
	, velocity_min_y_(-1.f)
	, velocity_min_z_(-1.f)
	, velocity_max_x_(1.f)
	, velocity_max_y_(1.f)
	, velocity_max_z_(1.f)
	, speed_(1.f)
	, gravity_x_(0)
	, gravity_y_(1.f)
	, gravity_z_(0)
	, color_random_(FALSE)
	, color_r_(1.f)
	, color_g_(1.f)
	, color_b_(1.f)
	, color_a_(1.f)
	, uv_animation_(FALSE)
	, uv_loop_(FALSE)
	, max_u_(0)
	, max_v_(0)
	, rotation_random_(FALSE)
	, scale_(0.5f)
	, scale_random_(FALSE)
	, scale_min_(-1.f)
	, scale_max_(1.f)
	, type_(0)
	, life_time_(1.f)
{

}

ParticleEditor::~ParticleEditor()
{
}

void ParticleEditor::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, max_particle_);
	DDX_Text(pDX, IDC_EDIT13, duration_);
	DDX_Check(pDX, IDC_CHECK4, loop_);
	DDX_Check(pDX, IDC_CHECK2, origin_random_);
	DDX_Text(pDX, IDC_EDIT2, origin_x_);
	DDX_Text(pDX, IDC_EDIT3, origin_y_);
	DDX_Text(pDX, IDC_EDIT4, origin_z_);
	DDX_Text(pDX, IDC_EDIT5, origin_min_x_);
	DDX_Text(pDX, IDC_EDIT6, origin_min_y_);
	DDX_Text(pDX, IDC_EDIT7, origin_min_z_);
	DDX_Text(pDX, IDC_EDIT8, origin_max_x_);
	DDX_Text(pDX, IDC_EDIT9, origin_max_y_);
	DDX_Text(pDX, IDC_EDIT10, origin_max_z_);
	DDX_Check(pDX, IDC_CHECK3, velocity_random_);
	DDX_Text(pDX, IDC_EDIT15, velocity_x_);
	DDX_Text(pDX, IDC_EDIT16, velocity_y_);
	DDX_Text(pDX, IDC_EDIT11, velocity_z_);
	DDX_Text(pDX, IDC_EDIT12, velocity_min_x_);
	DDX_Text(pDX, IDC_EDIT17, velocity_min_y_);
	DDX_Text(pDX, IDC_EDIT18, velocity_min_z_);
	DDX_Text(pDX, IDC_EDIT19, velocity_max_x_);
	DDX_Text(pDX, IDC_EDIT20, velocity_max_y_);
	DDX_Text(pDX, IDC_EDIT21, velocity_max_z_);
	DDX_Text(pDX, IDC_EDIT32, speed_);
	DDX_Text(pDX, IDC_EDIT22, gravity_x_);
	DDX_Text(pDX, IDC_EDIT23, gravity_y_);
	DDX_Text(pDX, IDC_EDIT24, gravity_z_);
	DDX_Check(pDX, IDC_CHECK5, color_random_);
	DDX_Text(pDX, IDC_EDIT25, color_r_);
	DDV_MinMaxFloat(pDX, color_r_, 0, 1);
	DDX_Text(pDX, IDC_EDIT26, color_g_);
	DDV_MinMaxFloat(pDX, color_g_, 0, 1);
	DDX_Text(pDX, IDC_EDIT27, color_b_);
	DDV_MinMaxFloat(pDX, color_b_, 0, 1);
	DDX_Text(pDX, IDC_EDIT28, color_a_);
	DDV_MinMaxFloat(pDX, color_a_, 0, 1);
	DDX_Control(pDX, IDC_LIST1, texture_list_);
	DDX_Check(pDX, IDC_CHECK6, uv_animation_);
	DDX_Check(pDX, IDC_CHECK7, uv_loop_);
	DDX_Text(pDX, IDC_EDIT29, max_u_);
	DDX_Text(pDX, IDC_EDIT30, max_v_);
	DDX_Check(pDX, IDC_CHECK9, rotation_random_);
	DDX_Text(pDX, IDC_EDIT31, scale_);
	DDX_Check(pDX, IDC_CHECK8, scale_random_);
	DDX_Text(pDX, IDC_SCALE_MIN, scale_min_);
	DDX_Text(pDX, IDC_SCALE_MAX, scale_max_);
	DDX_Text(pDX, IDC_EDIT33, life_time_);
}

BEGIN_MESSAGE_MAP(ParticleEditor, CPropertyPage)
	ON_WM_VSCROLL()
ON_WM_SIZE()
ON_EN_CHANGE(IDC_EDIT1, &ParticleEditor::OnChangeMaxParticle)
ON_EN_CHANGE(IDC_EDIT13, &ParticleEditor::OnChangeDuration)
ON_BN_CLICKED(IDC_CHECK4, &ParticleEditor::OnClickedCheckLoop)
ON_BN_CLICKED(IDC_RADIO1, &ParticleEditor::OnClickedTypePoint)
ON_BN_CLICKED(IDC_RADIO5, &ParticleEditor::OnClickedTypeBillboard)
ON_BN_CLICKED(IDC_RADIO6, &ParticleEditor::OnClickedTypeMesh)
ON_BN_CLICKED(IDC_CHECK2, &ParticleEditor::OnClickedOriginRandom)
ON_EN_CHANGE(IDC_EDIT2, &ParticleEditor::OnChangeOrigin)
ON_EN_CHANGE(IDC_EDIT3, &ParticleEditor::OnChangeOrigin)
ON_EN_CHANGE(IDC_EDIT4, &ParticleEditor::OnChangeOrigin)
ON_EN_CHANGE(IDC_EDIT5, &ParticleEditor::OnChangeOriginMin)
ON_EN_CHANGE(IDC_EDIT6, &ParticleEditor::OnChangeOriginMin)
ON_EN_CHANGE(IDC_EDIT7, &ParticleEditor::OnChangeOriginMin)
ON_EN_CHANGE(IDC_EDIT8, &ParticleEditor::OnChangeOriginMax)
ON_EN_CHANGE(IDC_EDIT9, &ParticleEditor::OnChangeOriginMax)
ON_EN_CHANGE(IDC_EDIT10, &ParticleEditor::OnChangeOriginMax)
ON_BN_CLICKED(IDC_CHECK3, &ParticleEditor::OnChangeVelocityRandom)
ON_EN_CHANGE(IDC_EDIT15, &ParticleEditor::OnChangeVelocity)
ON_EN_CHANGE(IDC_EDIT16, &ParticleEditor::OnChangeVelocity)
ON_EN_CHANGE(IDC_EDIT11, &ParticleEditor::OnChangeVelocity)
ON_EN_CHANGE(IDC_EDIT12, &ParticleEditor::OnChangeVelocityMin)
ON_EN_CHANGE(IDC_EDIT17, &ParticleEditor::OnChangeVelocityMin)
ON_EN_CHANGE(IDC_EDIT18, &ParticleEditor::OnChangeVelocityMin)
ON_EN_CHANGE(IDC_EDIT19, &ParticleEditor::OnChangeVelocityMax)
ON_EN_CHANGE(IDC_EDIT20, &ParticleEditor::OnChangeVelocityMax)
ON_EN_CHANGE(IDC_EDIT21, &ParticleEditor::OnChangeVelocityMax)
ON_EN_CHANGE(IDC_EDIT32, &ParticleEditor::OnChangeSpeed)
ON_EN_CHANGE(IDC_EDIT22, &ParticleEditor::OnChangeGravity)
ON_EN_CHANGE(IDC_EDIT23, &ParticleEditor::OnChangeGravity)
ON_EN_CHANGE(IDC_EDIT24, &ParticleEditor::OnChangeGravity)
ON_BN_CLICKED(IDC_CHECK5, &ParticleEditor::OnChangeColorRandom)
ON_EN_CHANGE(IDC_EDIT25, &ParticleEditor::OnChangeColor)
ON_EN_CHANGE(IDC_EDIT26, &ParticleEditor::OnChangeColor)
ON_EN_CHANGE(IDC_EDIT27, &ParticleEditor::OnChangeColor)
ON_EN_CHANGE(IDC_EDIT28, &ParticleEditor::OnChangeColor)
ON_LBN_SELCHANGE(IDC_LIST1, &ParticleEditor::OnLbnSelchangeTextureList)
ON_BN_CLICKED(IDC_CHECK6, &ParticleEditor::OnChangeUVAnimation)
ON_BN_CLICKED(IDC_CHECK7, &ParticleEditor::OnChangeUVLoop)
ON_EN_CHANGE(IDC_EDIT29, &ParticleEditor::OnChangeMaxUV)
ON_EN_CHANGE(IDC_EDIT30, &ParticleEditor::OnChangeMaxUV)
ON_BN_CLICKED(IDC_CHECK9, &ParticleEditor::OnChangeRotationRandom)
ON_EN_CHANGE(IDC_EDIT31, &ParticleEditor::OnChangeScale)
ON_BN_CLICKED(IDC_CHECK8, &ParticleEditor::OnChangeScaleRandom)
ON_EN_CHANGE(IDC_SCALE_MIN, &ParticleEditor::OnChangeScaleMinMax)
ON_EN_CHANGE(IDC_SCALE_MAX, &ParticleEditor::OnChangeScaleMinMax)
ON_EN_CHANGE(IDC_EDIT33, &ParticleEditor::OnChangeLifeTime)
ON_BN_CLICKED(IDC_BUTTON5, &ParticleEditor::OnClickSave)
ON_BN_CLICKED(IDC_BUTTON6, &ParticleEditor::OnClickedLoad)
END_MESSAGE_MAP()


// ParticleEditor 메시지 처리기입니다.


BOOL ParticleEditor::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	ptr_device_ = Engine::GraphicDevice()->GetDevice();
	ptr_point_effect_shader_ = dynamic_cast<Engine::CShader*> (Engine::GameManager()->CloneComponent(0, TEXT("Shader_Particle")));
	ptr_mesh_effect_shader_ = dynamic_cast<Engine::CShader*> (Engine::GameManager()->CloneComponent(0, TEXT("Shader_Mesh_Particle")));
	//ptr_particle_ = Engine::CParticleSystem::Create(ptr_device_, 50, Engine::ParticleType::Point);
	//ptr_particle_->Reset();

	//ptr_particle_->SetParticleMeshKey(0, TEXT("Buffer_RectTexture"));
	//ptr_particle_->SetParticleTexture(0, TEXT("Texture_Blood"));
	//ptr_particle_->SetCameraPos(&camera_pos);

	OnChangeMaxParticle();

	Tool()->SetParticleEditor(this);
	
	APRESOURCELOADER resource_loader(ResourceLoader::Create(ptr_device_, TEXT("../../Client/bin/Resources/Texture/Effect"), texture_list_, ResourceLoader::ResourceType::Texture));


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void ParticleEditor::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	Engine::Safe_Release_Delete(ptr_point_effect_shader_);
	Engine::Safe_Release_Delete(ptr_mesh_effect_shader_);
	Engine::Safe_Release_Delete(ptr_particle_);

	CPropertyPage::PostNcDestroy();
}

void ParticleEditor::InitParticle()
{
	UpdateData(FALSE);

	OnChangeDuration();
	OnChangeLifeTime();
	OnClickedCheckLoop();
	OnClickedOriginRandom();
	OnChangeOrigin();
	OnChangeVelocityRandom();
	OnChangeVelocity();
	OnChangeVelocityMin();
	OnChangeVelocityMax();
	OnChangeSpeed();
	OnChangeGravity();
	OnChangeColorRandom();
	OnChangeColor();
	OnChangeUVAnimation();
	OnChangeUVLoop();
	OnChangeMaxUV();
	OnChangeRotationRandom();
	OnChangeScale();
	OnChangeScaleRandom();
	OnChangeScaleMinMax();
}


BOOL ParticleEditor::OnSetActive()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	Tool()->SetTab(CToolManager::TabID::Particle);

	return CPropertyPage::OnSetActive();
}

void ParticleEditor::Update(float delta_time)
{
	Matrix inv_mat_view;
	ptr_device_->GetTransform(D3DTS_VIEW, &inv_mat_view);
	D3DXMatrixInverse(&inv_mat_view, nullptr, &inv_mat_view);
	camera_pos = *(Vector3*)&inv_mat_view.m[3][0];

	ptr_particle_->Update(delta_time);
}

void ParticleEditor::Render()
{
	LPD3DXEFFECT ptr_effect = (type_ == 0) ? ptr_point_effect_shader_->GetEffectHandle() : ptr_mesh_effect_shader_->GetEffectHandle();

	ptr_effect->Begin(nullptr, 0);
	ptr_particle_->Render(ptr_effect, nullptr);
	ptr_effect->End();
}

void ParticleEditor::OnChangeMaxParticle()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CPropertyPage::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	Engine::Safe_Release_Delete(ptr_particle_);

	if(type_ == 0)
		ptr_particle_ = Engine::CParticleSystem::Create(ptr_device_, max_particle_, Engine::ParticleType::Point);
	else if (type_ == 1)
	{
		ptr_particle_ = Engine::CParticleSystem::Create(ptr_device_, max_particle_, Engine::ParticleType::Billboard);
		ptr_particle_->SetParticleMeshKey(0, TEXT("Buffer_RectTexture"));
		CString texture_key;
		texture_list_.GetText(0, texture_key);
		ptr_particle_->SetParticleTexture(0, texture_key.operator LPCWSTR());
		ptr_particle_->SetCameraPos(&camera_pos);
	}
	InitParticle();
	ptr_particle_->Reset();
}


void ParticleEditor::OnChangeDuration()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CPropertyPage::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	ptr_particle_->particles()->duration = duration_;
	ptr_particle_->Reset();
}

void ParticleEditor::OnChangeLifeTime()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CPropertyPage::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	ptr_particle_->particles()->life_time = life_time_;
	ptr_particle_->Reset();
}

void ParticleEditor::OnClickedCheckLoop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	ptr_particle_->particles()->loop = (loop_ == TRUE) ? true : false;
	ptr_particle_->Reset();
}


void ParticleEditor::OnClickedTypePoint()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	type_ = 0;
	OnChangeMaxParticle();
}


void ParticleEditor::OnClickedTypeBillboard()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	type_ = 1;
	OnChangeMaxParticle();
}


void ParticleEditor::OnClickedTypeMesh()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	type_ = 2;
	OnChangeMaxParticle();
}


void ParticleEditor::OnClickedOriginRandom()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	ptr_particle_->particles()->random_origin = (origin_random_ == TRUE) ? true : false;
	
	GetDlgItem(IDC_EDIT5)->EnableWindow(origin_random_);
	GetDlgItem(IDC_EDIT6)->EnableWindow(origin_random_);
	GetDlgItem(IDC_EDIT7)->EnableWindow(origin_random_);
	GetDlgItem(IDC_EDIT8)->EnableWindow(origin_random_);
	GetDlgItem(IDC_EDIT9)->EnableWindow(origin_random_);
	GetDlgItem(IDC_EDIT10)->EnableWindow(origin_random_);

	ptr_particle_->Reset();
}


void ParticleEditor::OnChangeOrigin()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CPropertyPage::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	ptr_particle_->particles()->origin = Vector3(origin_x_, origin_y_, origin_z_);
	ptr_particle_->Reset();
}


void ParticleEditor::OnChangeOriginMin()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CPropertyPage::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	ptr_particle_->particles()->origin_min = Vector3(origin_min_x_, origin_min_y_, origin_min_z_);
	ptr_particle_->Reset();
}


void ParticleEditor::OnChangeOriginMax()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CPropertyPage::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	ptr_particle_->particles()->origin_max = Vector3(origin_max_x_, origin_max_y_, origin_max_z_);
	ptr_particle_->Reset();
}

void ParticleEditor::OnChangeVelocityRandom()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	ptr_particle_->particles()->random_velocity = (velocity_random_ == TRUE) ? true : false;

	GetDlgItem(IDC_EDIT12)->EnableWindow(velocity_random_);
	GetDlgItem(IDC_EDIT17)->EnableWindow(velocity_random_);
	GetDlgItem(IDC_EDIT18)->EnableWindow(velocity_random_);
	GetDlgItem(IDC_EDIT19)->EnableWindow(velocity_random_);
	GetDlgItem(IDC_EDIT20)->EnableWindow(velocity_random_);
	GetDlgItem(IDC_EDIT21)->EnableWindow(velocity_random_);

	ptr_particle_->Reset();
}

void ParticleEditor::OnChangeVelocity()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CPropertyPage::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	ptr_particle_->particles()->velocity = Vector3(velocity_x_, velocity_y_, velocity_z_);
	ptr_particle_->Reset();
}




void ParticleEditor::OnChangeVelocityMin()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CPropertyPage::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	ptr_particle_->particles()->velocity_min = Vector3(velocity_min_x_, velocity_min_y_, velocity_min_z_);
	ptr_particle_->Reset();
}


void ParticleEditor::OnChangeVelocityMax()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CPropertyPage::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	ptr_particle_->particles()->velocity_max = Vector3(velocity_max_x_, velocity_max_y_, velocity_max_z_);
	ptr_particle_->Reset();
}


void ParticleEditor::OnChangeSpeed()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CPropertyPage::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	ptr_particle_->particles()->speed = speed_;
	ptr_particle_->Reset();
}


void ParticleEditor::OnChangeGravity()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CPropertyPage::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	ptr_particle_->particles()->gravity = Vector3(gravity_x_, gravity_y_, gravity_z_);
	ptr_particle_->Reset();
}


void ParticleEditor::OnChangeColorRandom()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	ptr_particle_->particles()->random_color = (color_random_ == TRUE) ? true : false;
	ptr_particle_->Reset();
}


void ParticleEditor::OnChangeColor()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CPropertyPage::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	ptr_particle_->particles()->color = D3DXCOLOR(color_r_, color_g_, color_b_, color_a_);
	ptr_particle_->Reset();
}


void ParticleEditor::OnLbnSelchangeTextureList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (type_ == 0) return;

	UpdateData(TRUE);
	int select = texture_list_.GetCurSel();
	CString tex_key;
	texture_list_.GetText(select, tex_key);
	ptr_particle_->SetParticleTexture(0, tex_key.operator LPCWSTR());
}


void ParticleEditor::OnChangeUVAnimation()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	ptr_particle_->particles()->uv_animation = (uv_animation_ == TRUE) ? true : false;
	ptr_particle_->Reset();
}


void ParticleEditor::OnChangeUVLoop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	ptr_particle_->particles()->uv_loop = (uv_loop_ == TRUE) ? true : false;
	ptr_particle_->Reset();
}


void ParticleEditor::OnChangeMaxUV()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CPropertyPage::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	ptr_particle_->particles()->max_u = max_u_;
	ptr_particle_->particles()->max_v = max_v_;
	ptr_particle_->Reset();
}


void ParticleEditor::OnChangeRotationRandom()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	ptr_particle_->particles()->random_rotation = (rotation_random_ == TRUE) ? true : false;
	ptr_particle_->Reset();
}


void ParticleEditor::OnChangeScale()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CPropertyPage::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	ptr_particle_->particles()->scale = scale_;
	ptr_particle_->Reset();

}


void ParticleEditor::OnChangeScaleRandom()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	ptr_particle_->particles()->random_scale = (scale_random_ == TRUE) ? true : false;

	GetDlgItem(IDC_SCALE_MIN)->EnableWindow(scale_random_);
	GetDlgItem(IDC_SCALE_MAX)->EnableWindow(scale_random_);
	ptr_particle_->Reset();
}


void ParticleEditor::OnChangeScaleMinMax()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CPropertyPage::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	ptr_particle_->particles()->scale_min = scale_min_;
	ptr_particle_->particles()->scale_max = scale_max_;
	ptr_particle_->Reset();
}

void ParticleEditor::OnClickSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Tool()->SetFileMode(true);

	CString path;
	Tool()->ptr_stage_editor()->GetPathFromDialog(FALSE, path);

	if (path.IsEmpty())
	{
		Tool()->SetFileMode(false);
		return;
	}

	HANDLE file = CreateFile(path, GENERIC_WRITE, 0, nullptr
		, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	DWORD byte = 0;
	TCHAR text[MAX_PATH] = TEXT("");

	WriteFile(file, &ptr_particle_->particles()->type, sizeof(unsigned int), &byte, nullptr);
	WriteFile(file, &ptr_particle_->particles()->max_particle, sizeof(int), &byte, nullptr);
	
	WriteFile(file, ptr_particle_->particles()->origin, sizeof(Vector3), &byte, nullptr);
	WriteFile(file, ptr_particle_->particles()->origin_min, sizeof(Vector3), &byte, nullptr);
	WriteFile(file, ptr_particle_->particles()->origin_max, sizeof(Vector3), &byte, nullptr);
	
	WriteFile(file, ptr_particle_->particles()->velocity, sizeof(Vector3), &byte, nullptr);
	WriteFile(file, ptr_particle_->particles()->velocity_min, sizeof(Vector3), &byte, nullptr);
	WriteFile(file, ptr_particle_->particles()->velocity_max, sizeof(Vector3), &byte, nullptr);
	
	WriteFile(file, ptr_particle_->particles()->gravity, sizeof(Vector3), &byte, nullptr);
	
	WriteFile(file, &ptr_particle_->particles()->scale, sizeof(float), &byte, nullptr);
	WriteFile(file, &ptr_particle_->particles()->scale_min, sizeof(float), &byte, nullptr);
	WriteFile(file, &ptr_particle_->particles()->scale_max, sizeof(float), &byte, nullptr);
	WriteFile(file, &ptr_particle_->particles()->speed, sizeof(float), &byte, nullptr);
	WriteFile(file, &ptr_particle_->particles()->life_time, sizeof(float), &byte, nullptr);
	WriteFile(file, &ptr_particle_->particles()->color.r, sizeof(float), &byte, nullptr);
	WriteFile(file, &ptr_particle_->particles()->color.g, sizeof(float), &byte, nullptr);
	WriteFile(file, &ptr_particle_->particles()->color.b, sizeof(float), &byte, nullptr);
	WriteFile(file, &ptr_particle_->particles()->color.a, sizeof(float), &byte, nullptr);

	wsprintf(text, ptr_particle_->particles()->texture_key.c_str());
	WriteFile(file, text, sizeof(text), &byte, nullptr);

	WriteFile(file, &ptr_particle_->particles()->duration, sizeof(float), &byte, nullptr);
	WriteFile(file, &ptr_particle_->particles()->max_u, sizeof(float), &byte, nullptr);
	WriteFile(file, &ptr_particle_->particles()->max_v, sizeof(float), &byte, nullptr);
	WriteFile(file, &ptr_particle_->particles()->loop, sizeof(bool), &byte, nullptr);
	WriteFile(file, &ptr_particle_->particles()->uv_animation, sizeof(bool), &byte, nullptr);
	WriteFile(file, &ptr_particle_->particles()->uv_loop, sizeof(bool), &byte, nullptr);
	WriteFile(file, &ptr_particle_->particles()->random_origin, sizeof(bool), &byte, nullptr);
	WriteFile(file, &ptr_particle_->particles()->random_velocity, sizeof(bool), &byte, nullptr);
	WriteFile(file, &ptr_particle_->particles()->random_color, sizeof(bool), &byte, nullptr);
	WriteFile(file, &ptr_particle_->particles()->random_rotation, sizeof(bool), &byte, nullptr);
	WriteFile(file, &ptr_particle_->particles()->random_scale, sizeof(bool), &byte, nullptr);

	CloseHandle(file);
	Tool()->SetFileMode(false);
}


void ParticleEditor::OnClickedLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Tool()->SetFileMode(true);

	CString path;
	Tool()->ptr_stage_editor()->GetPathFromDialog(TRUE, path);

	if (path.IsEmpty() || nullptr == ptr_particle_)
	{
		Tool()->SetFileMode(false);
		return;
	}

	HANDLE file = CreateFile(path, GENERIC_READ, 0, nullptr
		, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	DWORD byte = 0;
	TCHAR text[MAX_PATH] = TEXT("");
	Vector3 vec3_buffer;

	ReadFile(file, &type_, sizeof(unsigned int), &byte, nullptr);
	ptr_particle_->particles()->type = (Engine::ParticleType)type_;
	ReadFile(file, &max_particle_, sizeof(int), &byte, nullptr);

	ReadFile(file, vec3_buffer, sizeof(Vector3), &byte, nullptr);
	origin_x_ = vec3_buffer.x; origin_y_ = vec3_buffer.y; origin_z_ = vec3_buffer.z;
	ReadFile(file, vec3_buffer, sizeof(Vector3), &byte, nullptr);
	origin_min_x_ = vec3_buffer.x; origin_min_y_ = vec3_buffer.y; origin_min_z_ = vec3_buffer.z;
	ReadFile(file, vec3_buffer, sizeof(Vector3), &byte, nullptr);
	origin_max_x_ = vec3_buffer.x; origin_max_y_ = vec3_buffer.y; origin_max_z_ = vec3_buffer.z;

	ReadFile(file, vec3_buffer, sizeof(Vector3), &byte, nullptr);
	velocity_x_ = vec3_buffer.x; velocity_y_ = vec3_buffer.y; velocity_z_ = vec3_buffer.z;
	ReadFile(file, vec3_buffer, sizeof(Vector3), &byte, nullptr);
	velocity_min_x_ = vec3_buffer.x; velocity_min_y_ = vec3_buffer.y; velocity_min_z_ = vec3_buffer.z;
	ReadFile(file, vec3_buffer, sizeof(Vector3), &byte, nullptr);
	velocity_max_x_ = vec3_buffer.x; velocity_max_y_ = vec3_buffer.y; velocity_max_z_ = vec3_buffer.z;

	ReadFile(file, vec3_buffer, sizeof(Vector3), &byte, nullptr);
	gravity_x_ = vec3_buffer.x; gravity_y_ = vec3_buffer.y; gravity_z_ = vec3_buffer.z;

	ReadFile(file, &scale_, sizeof(float), &byte, nullptr);
	ReadFile(file, &scale_min_, sizeof(float), &byte, nullptr);
	ReadFile(file, &scale_max_, sizeof(float), &byte, nullptr);
	ReadFile(file, &speed_, sizeof(float), &byte, nullptr);
	ReadFile(file, &life_time_, sizeof(float), &byte, nullptr);
	ReadFile(file, &color_r_, sizeof(float), &byte, nullptr);
	ReadFile(file, &color_g_, sizeof(float), &byte, nullptr);
	ReadFile(file, &color_b_, sizeof(float), &byte, nullptr);
	ReadFile(file, &color_a_, sizeof(float), &byte, nullptr);

	ReadFile(file, &text, sizeof(text), &byte, nullptr);

	ReadFile(file, &duration_, sizeof(float), &byte, nullptr);

	ReadFile(file, &max_u_, sizeof(float), &byte, nullptr);
	ReadFile(file, &max_v_, sizeof(float), &byte, nullptr);

	ReadFile(file, &loop_, sizeof(bool), &byte, nullptr);
	ReadFile(file, &uv_animation_, sizeof(bool), &byte, nullptr);
	ReadFile(file, &uv_loop_, sizeof(bool), &byte, nullptr);
	ReadFile(file, &origin_random_, sizeof(bool), &byte, nullptr);
	ReadFile(file, &velocity_random_, sizeof(bool), &byte, nullptr);
	ReadFile(file, &color_random_, sizeof(bool), &byte, nullptr);
	ReadFile(file, &rotation_random_, sizeof(bool), &byte, nullptr);
	ReadFile(file, &scale_random_, sizeof(bool), &byte, nullptr);

	CloseHandle(file);
	Tool()->SetFileMode(false);
	UpdateData(FALSE);
	OnChangeMaxParticle();
	InitParticle();
	if (type_ == 1)
		ptr_particle_->SetParticleTexture(0, text);
}
