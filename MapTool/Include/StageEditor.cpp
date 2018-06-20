// StageEditor.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MapTool.h"
#include "StageEditor.h"
#include "afxdialogex.h"


// StageEditor 대화 상자입니다.

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


// StageEditor 메시지 처리기입니다.
