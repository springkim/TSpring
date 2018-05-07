
// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "TSpring.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
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
	ON_WM_TIMER()
	ON_COMMAND(ID_EDIT_EDITMODE, &CMainFrame::OnEditEditmode)
	ON_COMMAND(ID_EDIT_TRACKINGMODE, &CMainFrame::OnEditTrackingmode)
	ON_COMMAND(ID_THEME_ASHSKY, &CMainFrame::OnThemeAshsky)
	ON_COMMAND(ID_THEME_SOLARIZEDDARK, &CMainFrame::OnThemeSolarizeddark)
	ON_MESSAGE(MSPRING_DISABLE_HTTEST, &CMainFrame::OnDisableHtTest)
END_MESSAGE_MAP()



CMainFrame::CMainFrame() {}
CMainFrame::~CMainFrame() {}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (MSpringFrame::OnCreate(lpCreateStruct) == -1)return -1;
#if defined(_DEBUG)
	if (::AllocConsole() == TRUE) {
		FILE* nfp[3];
		freopen_s(nfp + 0, "CONOUT$", "rb", stdin);
		freopen_s(nfp + 1, "CONOUT$", "wb", stdout);
		freopen_s(nfp + 2, "CONOUT$", "wb", stderr);
		std::ios::sync_with_stdio(false);
	}
#endif
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
						  CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL)) {
		TRACE0("뷰 창을 만들지 못했습니다.\n");
		return -1;
	}
	this->AddSysBtn(GetTheme().ResourceClose(), MSpringFrame::ButtonEvent_Close);
	this->AddSysBtn(GetTheme().ResourceMaximize(), MSpringFrame::ButtonEvent_MaximizeWindow);
	this->AddSysBtn(GetTheme().ResourceMinimize(), MSpringFrame::ButtonEvent_MinimizeWindow);
	this->SetIcon(IDR_MAINFRAME);
	
	this->SetStyle(GetTheme().Font(), GetTheme().ColorBK(), GetTheme().ColorText(), GetTheme().ColorBorder());
	this->SetTitle(TEXT("TSpring 2.2"));
	this->SetTitleColor(GetTheme().ColorTitle());
	m_menu_frame = CreateFrame<MSpringMenuFrame>(this);
	m_menu_frame->SetStyle(GetTheme().Font(), GetTheme().ColorBK(), GetTheme().ColorText(), GetTheme().ColorHover(), GetTheme().ColorBK());
	m_menu_frame->SetMenu(IDR_MAINFRAME);
	m_menu_frame->SetPosition(0);
	this->AddExpansionClass(m_menu_frame);

	m_tab_frame = CreateFrame<MSpringTabFrame>(this);
	m_tab_frame->SetStyle(GetTheme().Font(), GetTheme().ColorActivate(), GetTheme().ColorDeactivate(), GetTheme().ColorBK(), GetTheme().ColorText());
	m_tab_frame->SetActivateBorder(GetTheme().ColorTitle());
	m_tab_frame->SetPosition(0);
	m_tab_frame->AddTab(TEXT("List"));
	m_tab_frame->AddTab(TEXT("Tag"));
	m_tab_frame->AddTab(TEXT("Export"));
	this->AddExpansionClass(m_tab_frame);

	m_wndView.SetStyle(GetTheme().ColorBK());

	static ListView list_view(&m_wndView);
	m_list_view = &list_view;
	GetApp().g_image_data = &m_list_view->m_list_image->m_data;
	GetApp().g_class_data = &m_list_view->m_list_class->m_data;

	GetApp().g_is_detection = &m_list_view->m_chk_detection->check;
	GetApp().g_is_segmentation = &m_list_view->m_chk_segmentation->check;
	GetApp().g_is_rectangle = &m_list_view->m_chk_rectangle->check;
	GetApp().g_is_rmid = &m_list_view->m_chk_r_mid->check;
	GetApp().g_is_r2pt = &m_list_view->m_chk_r_2pt->check;
	GetApp().g_is_tracking = &m_list_view->m_chk_tracking->check;

	static TagView tab_view(&m_wndView);
	m_tag_view = &tab_view;
	static ExportView export_view(&m_wndView);
	m_export_view = &export_view;
	m_wndView.m_view = m_list_view;

	ReadSettingFile();
	UpdateProgram();
	this->SetFocus();
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs) {
	if (!MSpringFrame::PreCreateWindow(cs))
		return FALSE;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}
#ifdef _DEBUG
void CMainFrame::AssertValid() const {
	MSpringFrame::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const {
	MSpringFrame::Dump(dc);
}
#endif


void CMainFrame::OnSetFocus(CWnd* pOldWnd) {
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) {
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;
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
	this->SetFocus();
}


void CMainFrame::OnFileOpenImages() {
	if (GetApp().g_exporting == true)return;
	const TCHAR* IMAGE_FILTER = TEXT("Image File(*bmp;*.jpg;*.jpeg;*.png)|*.bmp;*.jpg;*.jpeg;*.png|BMP File(*.bmp)|*.bmp|JPG File(*.jpg)|*.jpg|JPEG File(*.jpeg)|*.jpeg|PNG File(*.png)|*.png|");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_ALLOWMULTISELECT, IMAGE_FILTER);
	dlg.GetOFN().lpstrFile = GetApp().GetBuffer();
	dlg.GetOFN().nMaxFile = GetApp().g_buffer_size;
	if (IDOK == dlg.DoModal()) {
		std::set<TString> overlab_set;
		for (auto&e : *GetApp().g_image_data) {
			overlab_set.insert(e.first);
		}
		for (POSITION pos = dlg.GetStartPosition(); pos != NULL;) {
			CString path = dlg.GetNextPathName(pos);
			TString tpath = TString(path);
			if (overlab_set.find(tpath) == overlab_set.end()) {
				GetApp().g_image_data->push_back(std::make_pair(tpath, false));
			}
			path.ReleaseBuffer();
		}
		if (dynamic_cast<ListView*>(this->m_wndView.m_view) != nullptr) {
			dynamic_cast<ListView*>(this->m_wndView.m_view)->IdentifyTagInfo();
			dynamic_cast<ListView*>(this->m_wndView.m_view)->GetFinalID();
		}
		m_wndView.Invalidate();
		this->Invalidate();
	}
}


void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI) {
	lpMMI->ptMinTrackSize.x = 1280;
	lpMMI->ptMinTrackSize.y = 720;
	lpMMI->ptMaxTrackSize.x = 3840;
	lpMMI->ptMaxTrackSize.y = 2160;
	MSpringFrame::OnGetMinMaxInfo(lpMMI);
}


void CMainFrame::OnFileOpenimagefolder() {
	if (GetApp().g_exporting == true)return;
	CFolderPickerDialog fpd(nullptr, OFN_ALLOWMULTISELECT | OFN_FILEMUSTEXIST);
	fpd.GetOFN().lpstrFile = GetApp().GetBuffer();
	fpd.GetOFN().nMaxFile = GetApp().g_buffer_size;
	if (IDOK == fpd.DoModal()) {
		std::set<TString> overlab_set;
		for (auto&e : *GetApp().g_image_data) {
			overlab_set.insert(e.first);
		}
		std::vector<TString> dirs;
		for (POSITION pos = fpd.GetStartPosition(); pos != NULL;) {
			CString path = fpd.GetNextPathName(pos);
			dirs.push_back(TString(path));
		}
		std::vector<std::vector<std::pair<TString, bool>>> tmps(dirs.size());
#pragma omp parallel for schedule(dynamic)
		for (int i = 0; i < dirs.size(); i++) {
			std::string cpath = mspring::String::ToString(dirs[i]);
			std::vector<std::string> cfiles = ispring::File::FileList(cpath, "*.bmp;*.jpg;*.jpeg;*.png", false);
			for (auto& cfile : cfiles) {
				TString tstr = mspring::String::ToWString(cfile);
				if (overlab_set.find(tstr) == overlab_set.end()) {
					tmps[i].push_back(std::make_pair(tstr, false));
				}
			}
		}
		for (int i = 0; i < dirs.size(); i++) {
			GetApp().g_image_data->reserve(GetApp().g_image_data->size() + tmps[i].size());
			GetApp().g_image_data->insert(GetApp().g_image_data->end(), tmps[i].begin(), tmps[i].end());
		}
		if (dynamic_cast<ListView*>(this->m_wndView.m_view) != nullptr) {
			dynamic_cast<ListView*>(this->m_wndView.m_view)->IdentifyTagInfo();
			dynamic_cast<ListView*>(this->m_wndView.m_view)->GetFinalID();
		}
		m_wndView.Invalidate();
		this->Invalidate();
	}
}


void CMainFrame::OnLButtonDown(UINT nFlags, CPoint point) {
	if (GetApp().g_command_go2tagging != -1) {
		m_tab_frame->SetCurrentTab(1);
		m_wndView.m_view = m_tag_view;
		int value= GetApp().g_command_go2tagging;
		GetApp().g_tag_idx = value;
		GetApp().g_command_go2tagging = -1;
		m_wndView.Invalidate();
		this->Invalidate();
		this->OnNcPaint();
	}
	MSpringFrame::OnLButtonDown(nFlags, point);
}


BOOL CMainFrame::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) {
	if (GetApp().g_degree != -1) {
		m_cursor = AfxGetApp()->LoadCursor(IDC_CURSOR1 + (GetApp().g_degree + 90) % 180);
		m_cursor_prev = SetCursor(m_cursor);
		return TRUE;
	}
	return MSpringFrame::OnSetCursor(pWnd, nHitTest, message);
}


void CMainFrame::OnDestroy() {
	WriteSettingFile();
#ifdef _DEBUG
	::FreeConsole();
#endif
	MSpringFrame::OnDestroy();
}


void CMainFrame::OnFileClearimages() {
	if (GetApp().g_exporting == true)return;
	GetApp().g_image_data->clear();
	GetApp().g_tag_idx = 0;
	GetApp().g_id = -2;
	this->Invalidate();
}


void CMainFrame::OnTimer(UINT_PTR nIDEvent) {
	/// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == 7777) {
		m_wndView.Invalidate();
		if (GetApp().g_exporting == false) {
			this->KillTimer(7777);
		}
	}
	MSpringFrame::OnTimer(nIDEvent);
}


void CMainFrame::OnEditEditmode() {
	if (GetApp().g_exporting == true)return;
	this->m_tag_view->m_chk_edit->check = !this->m_tag_view->m_chk_edit->check;
	m_wndView.Invalidate();
	this->Invalidate();
}


void CMainFrame::OnEditTrackingmode() {
	if (GetApp().g_exporting == true)return;
	*GetApp().g_is_tracking = !*GetApp().g_is_tracking;
	m_wndView.Invalidate();
	this->Invalidate();
}

void CMainFrame::OnThemeUpdate() {
	this->ClearSysBtn();
	this->AddSysBtn(GetTheme().ResourceClose() , MSpringFrame::ButtonEvent_Close);
	this->AddSysBtn(GetTheme().ResourceMaximize() , MSpringFrame::ButtonEvent_MaximizeWindow);
	this->AddSysBtn(GetTheme().ResourceMinimize(), MSpringFrame::ButtonEvent_MinimizeWindow);
	this->SetIcon(IDR_MAINFRAME);
	this->SetStyle(GetTheme().Font(), GetTheme().ColorBK(), GetTheme().ColorText(), GetTheme().ColorBorder());
	this->SetTitleColor(GetTheme().ColorTitle());
	m_menu_frame->SetStyle(GetTheme().Font(), GetTheme().ColorBK(), GetTheme().ColorText(), GetTheme().ColorHover(), GetTheme().ColorBK());


	m_tab_frame->SetStyle(GetTheme().Font(), GetTheme().ColorActivate(), GetTheme().ColorDeactivate(), GetTheme().ColorBK(), GetTheme().ColorText());
	m_tab_frame->SetActivateBorder(GetTheme().ColorMenuBorder());
	
	m_wndView.SetStyle(GetTheme().ColorBK());
	GetTheme().Update();
	this->OnNcPaint();
	this->Invalidate();
}

void CMainFrame::OnThemeAshsky() {
	GetTheme().SetIdx(0);
	OnThemeUpdate();
}


void CMainFrame::OnThemeSolarizeddark() {
	GetTheme().SetIdx(1);
	OnThemeUpdate();
}

LRESULT CMainFrame::OnDisableHtTest(WPARAM wParam, LPARAM lParam) {
	int b = (int)wParam;
	this->SetHtTest(b != 0);
	return 1;
}
UINT _UpdateProgram(LPVOID param) {
	std::string version = "2.2";
	char c_temp[MAX_PATH + 1] = { 0 };
	SHGetSpecialFolderPathA(AfxGetMainWnd()->GetSafeHwnd(), c_temp, CSIDL_COMMON_APPDATA, TRUE);
	std::string temp = c_temp;
	if (temp.back() != '\\' || temp.back() != '/') {
		temp += '\\';
	}
	std::string verfile =temp+"TSpring_version.txt";
	ExportView::DownloadFile("https://www.dropbox.com/s/nymwmtwiz0ak093/version.txt?dl=1", verfile);
	std::fstream fin(verfile, std::ios::in);
	if (fin.is_open() == false) {
		return 0;
	}
	std::string new_version;
	fin >> new_version;
	fin.close();
	
	std::fstream noupdate("noupdate.txt", std::ios::in);
	if (noupdate.is_open() == true) {
		noupdate.close();
	}
	else if (std::atof(version.c_str()) < std::atof(new_version.c_str())) {
		std::string newfile = temp + "TSpring.exe";
		//Do update
		//최신버전 먼저 다운로드
		std::ostringstream oss;
		ExportView::DownloadFile("https://www.dropbox.com/s/npv7xwrtapwcpmi/TSpring.exe?dl=1", newfile);

		std::string updatefile = temp + "TSpring_update.bat";
		char olddir[MAX_PATH] = { 0 };
		GetCurrentDirectoryA(MAX_PATH,olddir);

		char oldfile[MAX_PATH];
		GetModuleFileNameA(NULL, oldfile, MAX_PATH);

		char* oldexe = oldfile + strlen(oldfile);
		while (*oldexe != '\\')oldexe--;oldexe++;
		//메시지 박스 띄움(강제 업데이트)
		std::fstream fout(updatefile, std::ios::out);
		fout << "@echo off" << std::endl;
		fout << "set olddir=\"" << olddir << "\\\"" << std::endl;
		fout << "set oldexe=" << oldexe << std::endl;
		fout << "set oldfile=\"" << oldfile << "\"" << std::endl;
		fout << "set newfile=\"" << newfile << "\"" << std::endl;
		fout << ":loop" << std::endl;
		fout << "DEL %oldfile%" << std::endl;
		fout << "timeout /t 1 /nobreak > NUL" << std::endl;
		fout << "if exist %oldfile% goto loop" << std::endl;
		fout << "move %newfile% %oldfile%" << std::endl;
		fout << "timeout /t 1 /nobreak > NUL" << std::endl;
		fout << "start /D %olddir% %oldexe%" << std::endl;
		fout << "exit /b" << std::endl;
		fout.close();
		std::string msg = "Update available(" + version + "->" + new_version + ")";
		TSpringMsgBox msgbox(TString(msg.begin(), msg.end()));
		msgbox.DoModal();
		WinExec(updatefile.c_str(), SW_HIDE);
		::AfxGetMainWnd()->PostMessage(WM_COMMAND, ID_APP_EXIT, 0);
	}
	return 1;
}
void CMainFrame::UpdateProgram() {
	AfxBeginThread(_UpdateProgram,nullptr);
	//_UpdateProgram(nullptr);
}
