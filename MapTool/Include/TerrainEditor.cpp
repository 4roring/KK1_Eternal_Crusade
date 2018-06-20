// TerrainEditor.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MapTool.h"
#include "TerrainEditor.h"
#include "afxdialogex.h"


// TerrainEditor 대화 상자입니다.

IMPLEMENT_DYNAMIC(TerrainEditor, CPropertyPage)

TerrainEditor::TerrainEditor()
	: CPropertyPage(IDD_TERRAINEDITOR)
{

}

TerrainEditor::~TerrainEditor()
{
}

void TerrainEditor::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(TerrainEditor, CPropertyPage)
END_MESSAGE_MAP()


// TerrainEditor 메시지 처리기입니다.
