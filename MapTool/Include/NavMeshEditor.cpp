// NavMeshEditor.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MapTool.h"
#include "NavMeshEditor.h"
#include "afxdialogex.h"


// NavMeshEditor 대화 상자입니다.

IMPLEMENT_DYNAMIC(NavMeshEditor, CPropertyPage)

NavMeshEditor::NavMeshEditor()
	: CPropertyPage(IDD_NAVMESHEDITOR)
{

}

NavMeshEditor::~NavMeshEditor()
{
}

void NavMeshEditor::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(NavMeshEditor, CPropertyPage)
END_MESSAGE_MAP()


// NavMeshEditor 메시지 처리기입니다.
