#pragma once


// NavMeshEditor ��ȭ �����Դϴ�.

class NavMeshEditor : public CPropertyPage
{
	DECLARE_DYNAMIC(NavMeshEditor)

public:
	NavMeshEditor();
	virtual ~NavMeshEditor();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NAVMESHEDITOR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
