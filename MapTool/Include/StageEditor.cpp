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


BOOL StageEditor::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	// ���ҽ� ��ü �ε� �� Resource List�� Key�����θ� ä���.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void StageEditor::PostNcDestroy()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	// ��� ���ҽ�, ������Ʈ ���� ����

	CPropertyPage::PostNcDestroy();
}
