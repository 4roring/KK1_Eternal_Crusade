// NavMeshEditor.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MapTool.h"
#include "NavMeshEditor.h"
#include "afxdialogex.h"


// NavMeshEditor ��ȭ �����Դϴ�.

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


// NavMeshEditor �޽��� ó�����Դϴ�.
