#pragma once


// TerrainEditor ��ȭ �����Դϴ�.

class TerrainEditor : public CPropertyPage
{
	DECLARE_DYNAMIC(TerrainEditor)

public:
	TerrainEditor();
	virtual ~TerrainEditor();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TERRAINEDITOR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
