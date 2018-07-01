#pragma once


// StageEditor 대화 상자입니다.

class StageEditor : public CPropertyPage
{
	DECLARE_DYNAMIC(StageEditor)

public:
	StageEditor();
	virtual ~StageEditor();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STAGEEDITOR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:

public:
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
};
