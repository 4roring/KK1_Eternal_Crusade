#pragma once



// EditorTab �� ���Դϴ�.
class StageEditor;
class NavMeshEditor;

class EditorTab : public CFormView
{
	DECLARE_DYNCREATE(EditorTab)

protected:
	EditorTab();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~EditorTab();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EDITORTAB };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	virtual void PostNcDestroy();

private:
	class KK1Sheet* ptr_sheet_ = nullptr;
};


