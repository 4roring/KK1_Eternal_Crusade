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


BOOL StageEditor::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	// 리소스 전체 로드 후 Resource List를 Key값으로만 채우기.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void StageEditor::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	// 모든 리소스, 오브젝트 안전 제거

	CPropertyPage::PostNcDestroy();
}
