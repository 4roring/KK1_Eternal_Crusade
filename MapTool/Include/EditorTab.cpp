// EditorTab.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MapTool.h"
#include "EditorTab.h"

#include "KK1Sheet.h"

// EditorTab

IMPLEMENT_DYNCREATE(EditorTab, CFormView)

EditorTab::EditorTab()
	: CFormView(IDD_EDITORTAB)
{

}

EditorTab::~EditorTab()
{
}

void EditorTab::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(EditorTab, CFormView)
END_MESSAGE_MAP()


// EditorTab 진단입니다.

#ifdef _DEBUG
void EditorTab::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void EditorTab::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// EditorTab 메시지 처리기입니다.


void EditorTab::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	ptr_sheet_ = new KK1Sheet;
	ptr_sheet_->Create(this, WS_CHILD | WS_VISIBLE);
	ptr_sheet_->MoveWindow(0, 0, 300, 300);
}


void EditorTab::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CFormView::PostNcDestroy();
}
