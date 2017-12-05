
// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "TSpring.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, MSpringFrame)

BEGIN_MESSAGE_MAP(CMainFrame, MSpringFrame)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_NCLBUTTONDOWN()
	ON_COMMAND(ID_FILE_OPENIMAGES, &CMainFrame::OnFileOpenImages)
	ON_WM_GETMINMAXINFO()
	ON_COMMAND(ID_FILE_OPENIMAGEFOLDER, &CMainFrame::OnFileOpenimagefolder)
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_DESTROY()
	ON_COMMAND(ID_FILE_CLEARIMAGES, &CMainFrame::OnFileClearimages)
END_MESSAGE_MAP()

// CMainFrame 생성/소멸

CMainFrame::CMainFrame() {
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CMainFrame::~CMainFrame() {
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (MSpringFrame::OnCreate(lpCreateStruct) == -1)
		return -1;
#ifdef _DEBUG
	if (::AllocConsole() == TRUE) {
		FILE* nfp[3];
		freopen_s(nfp + 0, "CONOUT$", "rb", stdin);
		freopen_s(nfp + 1, "CONOUT$", "wb", stdout);
		freopen_s(nfp + 2, "CONOUT$", "wb", stderr);
		std::ios::sync_with_stdio(false);
	}
#endif
	// 프레임의 클라이언트 영역을 차지하는 뷰를 만듭니다.
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
						  CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL)) {
		TRACE0("뷰 창을 만들지 못했습니다.\n");
		return -1;
	}
	this->AddSysBtn(IDB_CLOSE, MSpringFrame::ButtonEvent_Close);
	this->AddSysBtn(IDB_MAXIMIZE, MSpringFrame::ButtonEvent_MaximizeWindow);
	this->AddSysBtn(IDB_MINIMIZE, MSpringFrame::ButtonEvent_MinimizeWindow);
	this->SetIcon(IDR_MAINFRAME);

	this->SetStyle(g_font, g_color_bk, g_color_text, g_color_border);
	this->SetTitle(TEXT("TSpring"));

	m_menu_frame = new MSpringMenuFrame(this);
	m_menu_frame->SetStyle(g_font, g_color_bk, g_color_text_menu, g_color_hover, g_color_bk);
	m_menu_frame->SetMenu(IDR_MAINFRAME);
	m_menu_frame->SetPosition(0);
	this->AddExpansionClass(m_menu_frame);

	m_tab_frame = new MSpringTabFrame(this);
	m_tab_frame->SetStyle(g_font, g_color_activate, g_color_deactivate, g_color_bk, g_color_text_tab);
	m_tab_frame->SetPosition(0);
	m_tab_frame->AddTab(TEXT("List"));
	m_tab_frame->AddTab(TEXT("Tag"));
	m_tab_frame->AddTab(TEXT("Export"));
	this->AddExpansionClass(m_tab_frame);

	m_wndView.SetStyle(g_color_bk);

	m_list_view = new ListView(&m_wndView);
	g_image_data = &m_list_view->m_list_image->m_data;
	g_class_data = &m_list_view->m_list_class->m_data;

	g_is_detection = &m_list_view->m_chk_detection->check;
	g_is_segmentation = &m_list_view->m_chk_segmentation->check;
	g_is_square = &m_list_view->m_chk_square->check;
	g_is_rectangle = &m_list_view->m_chk_rectangle->check;
	g_is_rmid = &m_list_view->m_chk_r_mid->check;
	g_is_r2pt = &m_list_view->m_chk_r_2pt->check;
	g_is_tracking = &m_list_view->m_chk_tracking->check;


	m_tag_view = new TagView(&m_wndView);
	m_export_view = new ExportView(&m_wndView);
	m_wndView.m_view = m_list_view;

	ReadSettingFile();
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs) {
	if (!MSpringFrame::PreCreateWindow(cs))
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const {
	MSpringFrame::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const {
	MSpringFrame::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/) {
	// 뷰 창으로 포커스를 이동합니다.
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) {
	// 뷰에서 첫째 크랙이 해당 명령에 나타나도록 합니다.
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// 그렇지 않으면 기본 처리합니다.
	return MSpringFrame::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}



void CMainFrame::OnNcLButtonDown(UINT nHitTest, CPoint point) {
	MSpringFrame::OnNcLButtonDown(nHitTest, point);
	int idx = m_tab_frame->GetCurrentTab();
	switch (idx) {
		case 0: {
			m_wndView.m_view = m_list_view;
		}break;
		case 1: {
			m_wndView.m_view = m_tag_view;
		}break;
		case 2: {
			m_wndView.m_view = m_export_view;
		}break;
	}
	

	m_wndView.Invalidate();
	this->Invalidate();
}


void CMainFrame::OnFileOpenImages() {
	//m_image_data
	const TCHAR* IMAGE_FILTER = TEXT("Image File(*bmp;*.jpg;*.jpeg;*.png)|*.bmp;*.jpg;*.jpeg;*.png|BMP File(*.bmp)|*.bmp|JPG File(*.jpg)|*.jpg|JPEG File(*.jpeg)|*.jpeg|PNG File(*.png)|*.png|");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_ALLOWMULTISELECT, IMAGE_FILTER);
	CString strFileList;
	const unsigned int c_cbBuffSize = (10000 * (MAX_PATH + 1)) + 1;
	dlg.GetOFN().lpstrFile = strFileList.GetBuffer(c_cbBuffSize);
	dlg.GetOFN().nMaxFile = c_cbBuffSize;
	if (IDOK == dlg.DoModal()) {
		std::set<CString> overlab_set;
		for (auto&e : *g_image_data) {
			overlab_set.insert(e.first);
		}
		for (POSITION pos = dlg.GetStartPosition(); pos != NULL;) {
			CString path = dlg.GetNextPathName(pos);
			if (overlab_set.find(path) == overlab_set.end()) {
				g_image_data->push_back(std::make_pair(path, false));
			}
		}
		m_wndView.Invalidate();
		this->Invalidate();
	}
}


void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI) {
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	lpMMI->ptMinTrackSize.x = 1200;
	lpMMI->ptMinTrackSize.y = 740;
	lpMMI->ptMaxTrackSize.x = 3840;
	lpMMI->ptMaxTrackSize.y = 2160;
	MSpringFrame::OnGetMinMaxInfo(lpMMI);
}


void CMainFrame::OnFileOpenimagefolder() {
	CFolderPickerDialog fpd(nullptr, OFN_ALLOWMULTISELECT | OFN_FILEMUSTEXIST);
	if (IDOK == fpd.DoModal()) {
		std::set<CString> overlab_set;
		for (auto&e : *g_image_data) {
			overlab_set.insert(e.first);
		}
		for (POSITION pos = fpd.GetStartPosition(); pos != NULL;) {
			CString path = fpd.GetNextPathName(pos);
			std::string cpath = mspring::String::ToString(path);
			std::vector<std::string> cfiles = ispring::File::FileList(cpath, "*.bmp;*.jpg;*.jpeg;*.png", false);
			for (auto& cfile : cfiles) {
				if (overlab_set.find(mspring::String::ToCString(cfile)) == overlab_set.end()) {
					g_image_data->push_back(std::make_pair(mspring::String::ToCString(cfile),false));
				}
			}
		}
		this->Invalidate();
	}
}


void CMainFrame::OnLButtonDown(UINT nFlags, CPoint point) {
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (g_command_go2tagging != -1) {
		m_tab_frame->SetCurrentTab(1);
		m_wndView.m_view = m_tag_view;
		g_tag_idx = g_command_go2tagging;
		g_command_go2tagging = -1;
		m_wndView.Invalidate();
		this->Invalidate();
		this->OnNcPaint();
	}
	MSpringFrame::OnLButtonDown(nFlags, point);
}


BOOL CMainFrame::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) {
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (g_degree != -1) {
		m_cursor = AfxGetApp()->LoadCursor(IDC_CURSOR1 + (g_degree + 90) % 180);
		m_cursor_prev = SetCursor(m_cursor);
		return TRUE;
	}
	return MSpringFrame::OnSetCursor(pWnd, nHitTest, message);
}


void CMainFrame::OnDestroy() {
	WriteSettingFile();
	MSpringFrame::OnDestroy();
}


void CMainFrame::OnFileClearimages() {
	g_image_data->clear();
	this->Invalidate();
}
