// KK1Sheet.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MapTool.h"
#include "KK1Sheet.h"


// KK1Sheet

IMPLEMENT_DYNAMIC(KK1Sheet, CPropertySheet)

KK1Sheet::KK1Sheet()
{
	AddPage(&stage_editor_);
	AddPage(&navmesh_editor_);
	AddPage(&Terrain_editor_);
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


// KK1Sheet �޽��� ó�����Դϴ�.


void KK1Sheet::PostNcDestroy()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	RemovePage(&stage_editor_);
	RemovePage(&Terrain_editor_);
	RemovePage(&navmesh_editor_);

	CPropertySheet::PostNcDestroy();
}