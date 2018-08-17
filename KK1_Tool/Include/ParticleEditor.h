#pragma once

#include "afxwin.h"


BEGIN(Engine)
class CParticleSystem;
class CShader;
END

// ParticleEditor 대화 상자입니다.

class ParticleEditor : public CPropertyPage
{
	DECLARE_DYNAMIC(ParticleEditor)

public:
	ParticleEditor();
	virtual ~ParticleEditor();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PARTICLEEDITOR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void Update(float delta_time);
	void Render();

private:
	virtual BOOL OnInitDialog();
	virtual BOOL OnSetActive();
	virtual void PostNcDestroy();

private:
	void InitParticle();

private:
	LPDIRECT3DDEVICE9 ptr_device_ = nullptr;
	Engine::CParticleSystem* ptr_particle_ = nullptr;
	Engine::CShader* ptr_mesh_effect_shader_ = nullptr;
	Engine::CShader* ptr_point_effect_shader_ = nullptr;
	Vector3 camera_pos;

private:
	int max_particle_;
	afx_msg void OnChangeMaxParticle();
	float duration_;
	afx_msg void OnChangeDuration();
	float life_time_;
	afx_msg void OnChangeLifeTime();
	BOOL loop_;
	afx_msg void OnClickedCheckLoop();

	int type_ = 0;
	afx_msg void OnClickedTypePoint();
	afx_msg void OnClickedTypeBillboard();
	afx_msg void OnClickedTypeMesh();

	BOOL origin_random_;
	afx_msg void OnClickedOriginRandom();
	float origin_x_;
	float origin_y_;
	float origin_z_;
	afx_msg void OnChangeOrigin();
	float origin_min_x_;
	float origin_min_y_;
	float origin_min_z_;
	float origin_max_x_;
	float origin_max_y_;
	float origin_max_z_;
	afx_msg void OnChangeOriginMin();
	afx_msg void OnChangeOriginMax();

	BOOL velocity_random_;
	afx_msg void OnChangeVelocityRandom();
	float velocity_x_;
	float velocity_y_;
	float velocity_z_;
	afx_msg void OnChangeVelocity();
	float velocity_min_x_;
	float velocity_min_y_;
	float velocity_min_z_;
	afx_msg void OnChangeVelocityMin();
	float velocity_max_x_;
	float velocity_max_y_;
	float velocity_max_z_;
	afx_msg void OnChangeVelocityMax();

	float speed_;
	afx_msg void OnChangeSpeed();

	float gravity_x_;
	float gravity_y_;
	float gravity_z_;
	afx_msg void OnChangeGravity();

	BOOL color_random_;
	afx_msg void OnChangeColorRandom();
	float color_r_;
	float color_g_;
	float color_b_;
	float color_a_;
	afx_msg void OnChangeColor();

	CListBox texture_list_;
	afx_msg void OnLbnSelchangeTextureList();

	BOOL uv_animation_;
	afx_msg void OnChangeUVAnimation();

	BOOL uv_loop_;
	afx_msg void OnChangeUVLoop();

	float max_u_;
	float max_v_;
	afx_msg void OnChangeMaxUV();

	BOOL rotation_random_;
	afx_msg void OnChangeRotationRandom();

	float scale_;
	afx_msg void OnChangeScale();
	BOOL scale_random_;
	afx_msg void OnChangeScaleRandom();
	float scale_min_;
	float scale_max_;
	afx_msg void OnChangeScaleMinMax();

	afx_msg void OnClickSave();
	afx_msg void OnClickedLoad();
};
