#pragma once


// StageEditor ��ȭ �����Դϴ�.

class StageEditor : public CPropertyPage
{
	DECLARE_DYNAMIC(StageEditor)

public:
	StageEditor();
	virtual ~StageEditor();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STAGEEDITOR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

private:

public:
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
};
