// TerrainEditor.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MapTool.h"
#include "TerrainEditor.h"
#include "afxdialogex.h"


// TerrainEditor ��ȭ �����Դϴ�.

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


// TerrainEditor �޽��� ó�����Դϴ�.
