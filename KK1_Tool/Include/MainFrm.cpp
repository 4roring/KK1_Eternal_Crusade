
// MainFrm.cpp : CMainFrame Ŭ������ ����
//

#include "stdafx.h"
#include "MapTool.h"

#include "MainFrm.h"

#include "MapToolView.h"
#include "EditorTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const uint32 uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static uint32 indicators[] =
{
	ID_SEPARATOR,           // ���� �� ǥ�ñ�
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame ����/�Ҹ�

CMainFrame::CMainFrame()
{
	// TODO: ���⿡ ��� �ʱ�ȭ �ڵ带 �߰��մϴ�.
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return TRUE;
}

// CMainFrame ����

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame �޽��� ó����



BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	constexpr int edit_tap_size_x = 300;
	
	main_splitter_.CreateStatic(this, 1, 2);
	main_splitter_.CreateView(0, 0, RUNTIME_CLASS(CMapToolView), CSize(g_kWinCx, g_kWinCy), pContext);
	main_splitter_.CreateView(0, 1, RUNTIME_CLASS(EditorTab), CSize(edit_tap_size_x, g_kWinCy), pContext);

	ptr_main_view_ = static_cast<CMapToolView*>(main_splitter_.GetPane(0, 0));
	ptr_edit_tab_ = static_cast<EditorTab*>(main_splitter_.GetPane(0, 1));

	RECT rc_frame_window;
	GetWindowRect(&rc_frame_window);
	SetRect(&rc_frame_window, 0, 0
		, rc_frame_window.right - rc_frame_window.left
		, rc_frame_window.bottom - rc_frame_window.top);

	RECT rc_main_view;
	GetClientRect(&rc_main_view);

	int row_frame = rc_frame_window.right - rc_main_view.right;
	int col_frame = rc_frame_window.bottom - rc_main_view.bottom;

	SetWindowPos(nullptr, 100, 100
		, int(g_kWinCx + edit_tap_size_x + row_frame), int(g_kWinCy + col_frame), SWP_NOZORDER);

	g_hwnd = m_hWnd;
	Tool()->Init_ToolManager();
	Engine::Input()->InitInputDevice(AfxGetInstanceHandle(), m_hWnd);

	return TRUE;
}

void CMainFrame::PostNcDestroy()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	Tool()->DestroyInstance();

	CFrameWnd::PostNcDestroy();
}
