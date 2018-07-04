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
	KK1Sheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	KK1Sheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~KK1Sheet();

protected:
	DECLARE_MESSAGE_MAP()

public:
	StageEditor stage_editor_;
	TerrainEditor Terrain_editor_;
	NavMeshEditor navmesh_editor_;
	virtual void PostNcDestroy();
};


