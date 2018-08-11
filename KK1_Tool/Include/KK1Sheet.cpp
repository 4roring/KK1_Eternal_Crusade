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
	AddPage(&nav_n_light_editor_);
	AddPage(&particle_editor_);
}

KK1Sheet::KK1Sheet(uint32 nIDCaption, CWnd* pParentWnd, uint32 iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

}

KK1Sheet::KK1Sheet(LPCTSTR pszCaption, CWnd* pParentWnd, uint32 iSelectPage)
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
	RemovePage(&nav_n_light_editor_);
	RemovePage(&particle_editor_);

	CPropertySheet::PostNcDestroy();
}
