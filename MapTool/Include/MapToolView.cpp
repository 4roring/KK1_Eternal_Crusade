
// MapToolView.cpp : CMapToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MapTool.h"
#endif

#include "MapToolDoc.h"
#include "MapToolView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND g_hwnd;

// CMapToolView

IMPLEMENT_DYNCREATE(CMapToolView, CView)

BEGIN_MESSAGE_MAP(CMapToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CMapToolView 생성/소멸

CMapToolView::CMapToolView()
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CMapToolView::~CMapToolView()
{
}

BOOL CMapToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.


	return CView::PreCreateWindow(cs);
}

// CMapToolView 그리기

void CMapToolView::OnDraw(CDC* /*pDC*/)
{
	CMapToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	Tool()->BegineScene();

	Tool()->EndScene(m_hWnd);
}


// CMapToolView 인쇄

BOOL CMapToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMapToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMapToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CMapToolView 진단

#ifdef _DEBUG
void CMapToolView::AssertValid() const
{
	CView::AssertValid();
}

void CMapToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMapToolDoc* CMapToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMapToolDoc)));
	return (CMapToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CMapToolView 메시지 처리기


void CMapToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CMainFrame* ptr_mainframe = (CMainFrame*)AfxGetMainWnd();

	RECT rc_frame_window;
	ptr_mainframe->GetWindowRect(&rc_frame_window);
	SetRect(&rc_frame_window, 0, 0
		, rc_frame_window.right - rc_frame_window.left
		, rc_frame_window.bottom - rc_frame_window.top);

	RECT rc_main_view;
	GetClientRect(&rc_main_view);

	int row_frame = rc_frame_window.right - rc_main_view.right;
	int col_frame = rc_frame_window.bottom - rc_main_view.bottom;

	ptr_mainframe->SetWindowPos(nullptr, 100, 100
		, int(g_kWinCx + row_frame), int(g_kWinCy + col_frame), SWP_NOZORDER);

	g_hwnd = m_hWnd;

	Tool()->Init_ToolManager();
}
