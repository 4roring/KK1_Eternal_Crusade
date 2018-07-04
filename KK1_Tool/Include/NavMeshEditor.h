#pragma once


// NavMeshEditor 대화 상자입니다.

class NavMeshEditor : public CPropertyPage
{
	DECLARE_DYNAMIC(NavMeshEditor)

public:
	NavMeshEditor();
	virtual ~NavMeshEditor();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NAVMESHEDITOR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
