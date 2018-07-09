#pragma once



// KK1Sheet

#include "StageEditor.h"
#include "TerrainEditor.h"
#include "NavMeshEditor.h"

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
	TerrainEditor Terrain_editor_;
	NavMeshEditor nav_n_light_editor_;
	virtual void PostNcDestroy();
};


