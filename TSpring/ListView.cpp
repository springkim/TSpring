#include "stdafx.h"
#include "ListView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

ListView::ListView(CWnd* wnd) : VirtualView(wnd) {
	m_list_image = CreateControl<MListBox>(wnd, MRect(MRectPosition::DYNAMIC, 10, 10, 400, 190));
	m_list_image->is_check = true;
	m_list_image->is_numbering = true;
	m_list_image->m_color_bk = GetTheme().ListViewColorBK();
	m_list_image->m_color_text = GetTheme().ListViewColorText();
	m_list_image->m_color_fr = GetTheme().ListColorScroll();

	m_list_class = CreateControl<MListBox>(wnd, MRect(MRectPosition::R, 380, 10, 10, 190));
	m_list_class->m_color_bk = GetTheme().ListViewColorBK();
	m_list_class->m_color_text = GetTheme().ListViewColorText();
	m_list_class->is_numbering = true;
	m_list_class->m_color_fr = GetTheme().ListColorScroll();

	m_edit_class = CreateControl<MSingleEdit>(wnd, MRect(MRectPosition::RB, 140, 150, 250, 30));
	m_edit_class->m_color_text = GetTheme().ListViewColorText();



	m_btn_add_class = CreateControl<MButton>(wnd, MRect(MRectPosition::RB, 10, 150, 120, 30));
	
	m_btn_add_class->m_text = TEXT("Add class");
	m_btn_add_class->m_color_fr = GetTheme().ButtonColorFR();
	m_btn_add_class->m_color_text= GetTheme().ButtonColorText();

	m_btn_load_class = CreateControl<MButton>(wnd, MRect(MRectPosition::RB, 10, 100, 120, 30));
	m_btn_load_class->m_text = TEXT("Load classes");
	m_btn_load_class->m_color_fr = GetTheme().ButtonColorFR();
	m_btn_load_class->m_color_text = GetTheme().ButtonColorText();

	m_btn_export_class = CreateControl<MButton>(wnd, MRect(MRectPosition::RB, 140, 100, 120, 30));
	m_btn_export_class->m_text = TEXT("Export classes");
	m_btn_export_class->m_color_fr = GetTheme().ButtonColorFR();
	m_btn_export_class->m_color_text = GetTheme().ButtonColorText();

	m_chk_detection = CreateControl<MButtonCheck>(wnd, MRect(MRectPosition::LB, 10, 150, 25, 25));
	m_chk_detection->m_color_fr= GetTheme().ButtonColorFR();
	m_chk_detection->m_color_text= GetTheme().ButtonColorText();

	m_stc_detection = CreateControl<MStatic>(wnd, MRect(MRectPosition::LB, 50, 150, 250, 25));
	m_stc_detection->m_color_text = GetTheme().ListViewColorText();
	m_stc_detection->m_text = TEXT("Object Detection");

	m_chk_segmentation = CreateControl<MButtonCheck>(wnd, MRect(MRectPosition::LB, 10, 100, 25, 25));
	m_chk_segmentation->m_color_text = GetTheme().ListViewColorText();
	m_chk_segmentation->m_color_fr = GetTheme().ButtonColorFR();
	m_stc_segmentation = CreateControl<MStatic>(wnd, MRect(MRectPosition::LB, 50, 100, 250, 25));
	m_stc_segmentation->m_color_text = GetTheme().ListViewColorText();
	m_stc_segmentation->m_text = TEXT("Semantic Segmentation");


	m_chk_rectangle = CreateControl<MButtonCheck>(wnd, MRect(MRectPosition::LB, 320, 150, 25, 25));
	m_chk_rectangle->m_color_text = GetTheme().ListViewColorText();
	m_chk_rectangle->m_color_fr = GetTheme().ButtonColorFR();
	m_stc_rectangle = CreateControl<MStatic>(wnd, MRect(MRectPosition::LB, 360, 150, 250, 25));
	m_stc_rectangle->m_color_text = GetTheme().ListViewColorText();
	m_stc_rectangle->m_text = TEXT("Rectangle");

	m_chk_r_mid = CreateControl<MButtonCheck>(wnd, MRect(MRectPosition::LB, 320, 100, 25, 25));
	m_chk_r_mid->m_color_text = GetTheme().ListViewColorText();
	m_chk_r_mid->m_color_fr = GetTheme().ButtonColorFR();
	m_stc_r_mid = CreateControl<MStatic>(wnd, MRect(MRectPosition::LB, 360, 100, 250, 25));
	m_stc_r_mid->m_color_text = GetTheme().ListViewColorText();
	m_stc_r_mid->m_text = TEXT("Rotate by center");

	m_chk_r_2pt = CreateControl<MButtonCheck>(wnd, MRect(MRectPosition::LB, 320, 50, 25, 25));
	m_chk_r_2pt->m_color_text = GetTheme().ListViewColorText();
	m_chk_r_2pt->m_color_fr = GetTheme().ButtonColorFR();
	m_stc_r_2pt = CreateControl<MStatic>(wnd, MRect(MRectPosition::LB, 360, 50, 250, 25));
	m_stc_r_2pt->m_color_text = GetTheme().ListViewColorText();
	m_stc_r_2pt->m_text = TEXT("Rotate by 2 points");

	m_chk_tracking = CreateControl<MButtonCheck>(wnd, MRect(MRectPosition::LB, 610, 150, 25, 25));
	m_chk_tracking->m_color_text = GetTheme().ListViewColorText();
	m_chk_tracking->m_color_fr = GetTheme().ButtonColorFR();
	m_stc_tracking = CreateControl<MStatic>(wnd, MRect(MRectPosition::LB, 650, 150, 150, 25));
	m_stc_tracking->m_color_text = GetTheme().ListViewColorText();
	m_stc_tracking->m_text = TEXT("Tracking");


	m_stc_image= CreateControl<MStatic>(wnd, MRect(MRectPosition::RB, 10, 50, 380, 30));
	m_stc_image->m_color_text = GetTheme().ListViewColorText();

	m_stc_class = CreateControl<MStatic>(wnd, MRect(MRectPosition::RB, 10, 0, 380, 30));
	m_stc_class->m_color_text = GetTheme().ListViewColorText();

	m_chk_rectangle->check = true;
	m_chk_detection->check = true;
	///DEV
	m_chk_segmentation->disable = true;

	this->AddControl(m_list_image);
	this->AddControl(m_list_class);
	this->AddControl(m_edit_class);
	this->AddControl(m_btn_add_class);
	this->AddControl(m_btn_load_class);
	this->AddControl(m_btn_export_class);
	this->AddControl(m_chk_detection);
	this->AddControl(m_stc_detection);
	this->AddControl(m_chk_segmentation);
	this->AddControl(m_stc_segmentation);
	this->AddControl(m_chk_rectangle);
	this->AddControl(m_stc_rectangle);
	this->AddControl(m_chk_r_mid);
	this->AddControl(m_stc_r_mid);
	this->AddControl(m_chk_r_2pt);
	this->AddControl(m_stc_r_2pt);
	this->AddControl(m_chk_tracking);
	this->AddControl(m_stc_tracking);
	this->AddControl(m_stc_image);
	this->AddControl(m_stc_class);

}


ListView::~ListView() {
}
int ListView::OnCreate() {
	VirtualView::OnCreate();
	
	return 1;
}
void ListView::OnDestroy() {
	VirtualView::OnDestroy();
	this->m_parent->KillTimer(MOUSE_LEAVE_TIMER_ID);
}

void ListView::OnPaint(CDC* pDC) {
	static ptrdiff_t c = 0;
	static double start1 = clock();
	static double start2 = clock();

	if ((clock()-start1)/CLOCKS_PER_SEC  > 20) {	//20초에 한번 검사
		c=std::count_if(m_list_image->m_data.begin(), m_list_image->m_data.end(), [](decltype(m_list_image->m_data[0])& e)->bool {return e.second; });
		start1 = clock();
	}
	if ((clock() - start2) / CLOCKS_PER_SEC  > 10) {	//10초에 한번 검사
		this->IdentifyTagInfo();
		start2 = clock();
	}
	size_t ratio = 0;
	if (c != 0) {
		if (m_list_image->m_data.size() != 0) {
			ratio = c * 100 / m_list_image->m_data.size();
		}
	}
	OStringStream oss;
	oss << m_list_image->m_data.size() << TEXT(" images loaded(") << ratio << TEXT("% completed)");
	m_stc_image->m_text = oss.str();
	oss.str(TEXT(""));
	oss << m_list_class->m_data.size() << TEXT(" classes are ready");
	m_stc_class->m_text = oss.str();
	if (*GetApp().g_is_rectangle == false) {
		m_chk_tracking->disable = true;
	} else {
		m_chk_tracking->disable = false;
	}
	MCALL;

	
	if (m_list_image->m_data.size() == 0) {
		CRect rect=m_list_image->m_rect.GetRect(this->GetViewRect());
		DrawBitmap(pDC, rect, GetTheme().ResourceUsage());
	} 
	if (m_list_class->m_data.size() == 0) {
		CRect rect = m_list_class->m_rect.GetRect(this->GetViewRect());
		DrawBitmap(pDC, rect, GetTheme().ResourceNoList());
	}
	if (GetApp().g_exporting == true) {
		ShowProgressBar(pDC, this->GetViewRect());
	}
	
}
void ListView::OnSetFocus(CWnd* pOldWnd) {
	VirtualView::OnSetFocus(pOldWnd);
}
void ListView::OnKillFocus(CWnd* pNewWnd) {
	VirtualView::OnKillFocus(pNewWnd);
}

void ListView::OnLButtonDown(UINT nFlags, CPoint point) {
	if (GetApp().g_exporting == true)return;
	MCALL;
	if (m_chk_detection->check && m_chk_detection->isChanged()) {
		m_chk_segmentation->check = false;
	} else if (m_chk_segmentation->check  && m_chk_segmentation->isChanged()) {
		m_chk_detection->check = false;
	}
	///Segmentation 태깅 비활성화
	m_chk_detection->check = true;

	if (m_chk_detection->check == true) {
		
		if (m_chk_rectangle->isChanged()) {
			m_chk_r_mid->check = false;
			m_chk_r_2pt->check = false;
			m_chk_rectangle->check = true;
			m_chk_r_mid->check = m_chk_r_2pt->check = false;
		}
		if (m_chk_r_mid->isChanged()) {
			m_chk_rectangle->check = false;
			m_chk_r_2pt->check = false;
			m_chk_r_mid->check = true;
			m_chk_rectangle->check = 	 m_chk_r_2pt->check = m_chk_tracking->check = false;
		}
		if (m_chk_r_2pt->isChanged()) {
			m_chk_rectangle->check = false;
			m_chk_r_mid->check = false;
			m_chk_r_2pt->check = true;
			m_chk_rectangle->check = m_chk_r_mid->check = m_chk_tracking->check = false;
		}
		
		if (m_chk_r_mid->check == true || m_chk_r_2pt->check == true) {
			//m_chk_tracking->check = false;
			m_chk_tracking->disable = true;
		} else {
			m_chk_tracking->disable = false;
		}
		
	}
	if (m_btn_add_class->isClicked()) {
		if (m_edit_class->m_text.length() > 0) {
			auto val = std::make_pair(m_edit_class->m_text, false);
			if (std::find(m_list_class->m_data.begin(), m_list_class->m_data.end(), val) == m_list_class->m_data.end()) {
				m_list_class->m_data.push_back(val);
				m_list_class->m_v_scroll_pos = 1.0F;
				m_edit_class->Reset();
			} else {
				TSpringMsgBox msgbox(TEXT("Existing class"));
				msgbox.DoModal();
			}
		}
	}
	if (m_btn_load_class->isClicked()) {
		//class 리스트 파일을 로드 합니다.
		const TCHAR* IMAGE_FILTER = TEXT("ClassList File(*txt;*.names)|*.txt;*.names|TXT File(*.txt)|*.txt|NAMES File(*.names)|*.names|");
		CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, IMAGE_FILTER);
		dlg.GetOFN().lpstrFile = GetApp().GetBuffer();
		dlg.GetOFN().nMaxFile = GetApp().g_buffer_size;
		if (IDOK == dlg.DoModal()) {
			CStdioFile file(dlg.GetPathName(), CFile::modeRead | CFile::typeText);
			TCHAR cstr[1024];
			int overlap = 0;
			while (1) {
				if (file.ReadString(cstr,1024) != nullptr) {
					if (_tcslen(cstr) > 0) {
						if (cstr[_tcslen(cstr) - 1]==TEXT('\n'))cstr[_tcslen(cstr) - 1] = TEXT('\0');
						auto val = std::make_pair(TString(cstr), false);
						if (std::find(m_list_class->m_data.begin(), m_list_class->m_data.end(), val) == m_list_class->m_data.end()) {
							m_list_class->m_data.push_back(val);
						} else {
							overlap++;
						}
					}
				} else {
					break;
				}
			}
			file.Close();
			if (overlap > 0) {
				OStringStream oss;
				oss << overlap << TEXT(" classes are overlapped!!");
				TSpringMsgBox msgbox(oss.str());
				msgbox.DoModal();
			}
		}
	}
	if (m_btn_export_class->isClicked()) {
		const TCHAR* FILTER = TEXT("All Files (*.*)|*.*|Text Files (*.txt)|*.txt|");
		CFileDialog dlg(FALSE, TEXT("txt"), TEXT("*.txt"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, FILTER, NULL);
		dlg.m_ofn.nFilterIndex = 2;
		if (IDOK == dlg.DoModal()) {
			CString cfile = dlg.GetPathName();
			std::string file(CT2A((LPCTSTR)cfile));
			std::fstream fout;
			fout.open(file, std::ios::out);
			if (fout.is_open() == false) {
				TSpringMsgBox msgbox(TEXT("Invalid file path!"));
				msgbox.DoModal();
				return;
			}
			for (auto&e : m_list_class->m_data) {
				fout << mspring::String::ToString(e.first) << std::endl;
			}
			fout.close();
			TSpringMsgBox msgbox(TEXT("Export success"));
			msgbox.DoModal();
			cfile.ReleaseBuffer();
		}
	}
}
void ListView::OnLButtonUp(UINT nFlags, CPoint point) {
	if (GetApp().g_exporting == true)return;
	MCALL;
}
void ListView::OnLButtonDblClk(UINT nFlags, CPoint point) {
	if (GetApp().g_exporting == true)return;
	MCALL;
}
void ListView::OnRButtonDown(UINT nFlags, CPoint point) {
	if (GetApp().g_exporting == true)return;
	MCALL;
}
void ListView::OnRButtonUp(UINT nFlags, CPoint point) {
	if (GetApp().g_exporting == true)return;
	MCALL;
	GetCursorPos(&point);
	int idx = m_list_image->GetElementByPoint(this->GetMousePoint());
	if (idx != -1) {
		MSpringMenu popup(this->m_parent);
		popup.CreatePopupMenu();
		popup.AppendMenu(MF_STRING, 1, TEXT("Go to tagging"));
		popup.AppendMenu(MF_STRING, 2, TEXT("Remove from list"));
		popup.AppendMenu(MF_STRING, 3, TEXT("Remove from disk"));
		popup.MakeItemsOwnDraw();
		int menu_id = popup.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD, point.x, point.y, this->m_parent);
		popup.DestroyMenu();
		switch (menu_id) {
			case 1: {
				GetApp().g_command_go2tagging = idx;
				AfxGetMainWnd()->SendMessage(WM_LBUTTONDOWN, NULL, NULL);
			}break;
			case 2: {
				m_list_image->m_data.erase(m_list_image->m_data.begin() + idx);
				if (GetApp().g_tag_idx >= GetApp().g_image_data->size()) {
					GetApp().g_tag_idx = static_cast<int>(GetApp().g_image_data->size() - 1);
				}
			}break;
			case 3: {
				std::string img_path = mspring::String::ToString(m_list_image->m_data[idx].first);
				ispring::File::FileErase(img_path);
				m_list_image->m_data.erase(m_list_image->m_data.begin() + idx);
				std::string tsp_path = img_path.substr(0, img_path.find_last_of('.')) + ".tsp";
				ispring::File::FileErase(tsp_path);
				std::string tsps_path = img_path.substr(0, img_path.find_last_of('.')) + ".tsps";
				ispring::File::FileErase(tsps_path);
				if (GetApp().g_tag_idx >= GetApp().g_image_data->size()) {
					GetApp().g_tag_idx = static_cast<int>(GetApp().g_image_data->size() - 1);
				}
			}break;
		}
	}
	idx = m_list_class->GetElementByPoint(this->GetMousePoint());
	if (idx != -1) {
		MSpringMenu popup(this->m_parent);
		popup.CreatePopupMenu();
		popup.AppendMenu(MF_STRING, 1, TEXT("Remove class"));
		popup.AppendMenu(MF_STRING, 2, TEXT("Remove entire class"));
		popup.MakeItemsOwnDraw();
		int menu_id = popup.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD, point.x, point.y, this->m_parent);
		popup.DestroyMenu();
		switch (menu_id) {
			case 1: {
				m_list_class->m_data.erase(m_list_class->m_data.begin() + idx);
			}break;
			case 2: {
				m_list_class->m_data.clear();
			}break;
		}
	}
}
BOOL ListView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) {
	if (GetApp().g_exporting == true)return FALSE;
	MCALL;
	return TRUE;
}
void ListView::OnMouseMove(UINT nFlags, CPoint point) {
	if (GetApp().g_exporting == true)return;
	MCALL;
}
void ListView::OnMouseLeave() {
	if (GetApp().g_exporting == true)return;
	MCALL;
}

BOOL ListView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) {
	VirtualView::OnSetCursor(pWnd, nHitTest, message);
	return TRUE;
}
void ListView::OnSize(UINT nType, int cx, int cy) {
	VirtualView::OnSize(nType, cx, cy);	
}

void ListView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if (GetApp().g_exporting == true)return;
	MCALL;
}

void ListView::OnTimer(UINT_PTR nIDEvent) {
	if (GetApp().g_exporting == true)return;
	MCALL;
}
void ListView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if (GetApp().g_exporting == true)return;
	if (m_edit_class->isFocused() == true && nChar == VK_RETURN) {
		if (m_edit_class->m_text.length() > 0) {
			auto val = std::make_pair(m_edit_class->m_text, false);
			if (std::find(m_list_class->m_data.begin(), m_list_class->m_data.end(), val) == m_list_class->m_data.end()) {
				m_list_class->m_data.push_back(val);
				m_list_class->m_v_scroll_pos = 1.0F;
				m_edit_class->Reset();
			} else {
				TSpringMsgBox msgbox(TEXT("Existing class"));
				msgbox.DoModal();
			}
		}
		return;
	}
	MCALL;
}
LRESULT ListView::OnComposition(WPARAM wParam, LPARAM lParam) {
	if (GetApp().g_exporting == true)return 0;
	MCALL;
	return 1;
}

static CWinThread *p1 = NULL;
static std::atomic<int> __Thread_IdentifyTagInfo = 0;
UINT _Thread_IdentifyTagInfo(LPVOID _this) {
	__Thread_IdentifyTagInfo = 1;
	ListView* lv = static_cast<ListView*>(_this);
	for (auto&e : lv->m_list_image->m_data) {
		std::string img_path = mspring::String::ToString(e.first);
		if (lv->m_chk_detection->check == true) {
			std::string tsp_path = img_path.substr(0, img_path.find_last_of('.')) + ".tsp";
			if (ispring::File::FileExist(tsp_path) == true) {
				e.second = true;
			} else {
				e.second = false;
			}
		} else if (lv->m_chk_segmentation->check == true) {
			std::string tsps_path = img_path.substr(0, img_path.find_last_of('.')) + ".tsps";
			if (ispring::File::FileExist(tsps_path) == true) {
				e.second = true;
			} else {
				e.second = false;
			}
		}
	}
	__Thread_IdentifyTagInfo = 2;
	return 1;
}
void ___Thread_IdentifyTagInfo() {
	if (__Thread_IdentifyTagInfo != 0) {
		WaitForSingleObject(p1, INFINITE);
	}
}
void ListView::IdentifyTagInfo() {
	switch (__Thread_IdentifyTagInfo) {
		case 2: __Thread_IdentifyTagInfo = 0;
			//THREAD_PRIORITY_TIME_CRITICAL
		case 0:p1 = AfxBeginThread(_Thread_IdentifyTagInfo, this); atexit(___Thread_IdentifyTagInfo); break;
		case 1:break;
		default:break;
	}
}
UINT _GetFinalID(LPVOID _this) {
	ListView* lv = static_cast<ListView*>(_this);
	int gid = -1;
	GetApp().g_id = -2;
	//OpenMP 로 변경
	int sz = static_cast<int>(GetApp().g_image_data->size());
	GetApp().g_analysing = true;
	GetApp().g_analysing_count = 0;
#pragma omp parallel for schedule(dynamic)
	for(int i=0;i<sz;i++){
		auto file = (*GetApp().g_image_data)[i];
		std::string img_path = mspring::String::ToString(file.first);
		std::string tsp_path = img_path.substr(0, img_path.find_last_of('.')) + ".tsp";
		std::fstream fin;
		fin.open(tsp_path, std::ios::in);
		if (fin.is_open() == true) {
			while (fin.eof() == false) {
				std::istringstream iss;
				std::string line;
				std::getline(fin, line);
				if (line.empty() == true) {
					continue;
				}
				iss.str(line);
				int _class;
				cv::RotatedRect rr;
				int id = -1;
				iss >> _class >> rr.center.x >> rr.center.y >> rr.size.width >> rr.size.height >> rr.angle >> id;
#pragma omp critical(gid)
				if (id + 1 > gid) {
					gid = id + 1;
				}
			}
			fin.close();
		}
		GetApp().g_analysing_count = GetApp().g_analysing_count + 1;
	}
	GetApp().g_analysing = false;
	GetApp().g_id = gid;
	return 1;
}
void ListView::GetFinalID() {
	//AfxBeginThread(_GetFinalID, this, THREAD_PRIORITY_TIME_CRITICAL);
	_GetFinalID(this);
}