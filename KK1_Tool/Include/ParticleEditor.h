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
	LPDIRECT3DDEVICE9 ptr_device_ = nullptr;
	Engine::CParticleSystem* ptr_particle_ = nullptr;
	Engine::CShader* ptr_shader_ = nullptr;
};
