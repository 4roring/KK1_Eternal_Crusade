// EditorTab.cpp : ���� �����Դϴ�.
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


// EditorTab �����Դϴ�.

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


// EditorTab �޽��� ó�����Դϴ�.


void EditorTab::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	ptr_sheet_ = new KK1Sheet;
	ptr_sheet_->Create(this, WS_CHILD | WS_VISIBLE);
	ptr_sheet_->MoveWindow(0, 0, 300, 300);
}


void EditorTab::PostNcDestroy()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	CFormView::PostNcDestroy();
}
