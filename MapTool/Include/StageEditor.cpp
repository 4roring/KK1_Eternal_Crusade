// StageEditor.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MapTool.h"
#include "StageEditor.h"
#include "afxdialogex.h"


// StageEditor ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(StageEditor, CPropertyPage)

StageEditor::StageEditor()
	: CPropertyPage(IDD_STAGEEDITOR)
{

}

StageEditor::~StageEditor()
{
}

void StageEditor::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(StageEditor, CPropertyPage)
END_MESSAGE_MAP()


// StageEditor �޽��� ó�����Դϴ�.
