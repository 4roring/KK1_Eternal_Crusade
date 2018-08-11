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


// KK1Sheet 메시지 처리기입니다.


void KK1Sheet::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	RemovePage(&stage_editor_);
	RemovePage(&nav_n_light_editor_);
	RemovePage(&particle_editor_);

	CPropertySheet::PostNcDestroy();
}
