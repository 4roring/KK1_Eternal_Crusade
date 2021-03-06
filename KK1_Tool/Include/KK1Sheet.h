#pragma once



// KK1Sheet

#include "StageEditor.h"
#include "NavMeshEditor.h"
#include "ParticleEditor.h"


class KK1Sheet : public CPropertySheet
{
	DECLARE_DYNAMIC(KK1Sheet)

public:
	KK1Sheet();
	KK1Sheet(uint32 nIDCaption, CWnd* pParentWnd = NULL, uint32 iSelectPage = 0);
	KK1Sheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, uint32 iSelectPage = 0);
	virtual ~KK1Sheet();

protected:
	DECLARE_MESSAGE_MAP()

public:
	StageEditor stage_editor_;
	NavMeshEditor nav_n_light_editor_;
	ParticleEditor particle_editor_;
	virtual void PostNcDestroy();
};


