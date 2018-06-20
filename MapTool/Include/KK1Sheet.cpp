// KK1Sheet.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MapTool.h"
#include "KK1Sheet.h"


// KK1Sheet

IMPLEMENT_DYNAMIC(KK1Sheet, CPropertySheet)

KK1Sheet::KK1Sheet()
{
	AddPage(&stage_editor_);
	AddPage(&Terrain_editor_);
	AddPage(&navmesh_editor_);
}

KK1Sheet::KK1Sheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

}

KK1Sheet::KK1Sheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{

}

KK1Sheet::~KK1Sheet()
{
}


BEGIN_MESSAGE_MAP(KK1Sheet, CPropertySheet)
END_MESSAGE_MAP()


// KK1Sheet 메시지 처리기입니다.
