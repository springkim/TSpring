#include "stdafx.h"
#include "ListView.h"


ListView::ListView(CWnd* wnd) : VirtualView(wnd) {
	m_list_image = new MListBox(wnd, MRect(MRectPosition::DYNAMIC, 10, 10, 400, 190));
	m_list_image->is_check = true;
	m_list_image->m_color_bk = &g_lv_color_bk;
	m_list_image->m_color_text = &g_lv_color_text;

	m_list_class = new MListBox(wnd, MRect(MRectPosition::R, 380, 10, 10, 190));
	m_list_class->m_color_bk = &g_lv_color_bk;
	m_list_class->m_color_text = &g_lv_color_text;

	m_edit_class = new MEdit(wnd, MRect(MRectPosition::RB, 140, 150, 250, 30));
	m_edit_class->m_color_text = &g_lv_color_text_white;
	m_btn_add_class = new MButton(wnd, MRect(MRectPosition::RB, 10, 150, 120, 30));
	m_btn_add_class->m_text = "Add class";
	m_btn_add_class->m_color_text= &g_lv_color_text_white;

	m_btn_load_class = new MButton(wnd, MRect(MRectPosition::RB, 10, 100, 120, 30));
	m_btn_load_class->m_text = "Load classes";
	m_btn_load_class->m_color_text = &g_lv_color_text_white;

	m_btn_export_class = new MButton(wnd, MRect(MRectPosition::RB, 140, 100, 120, 30));
	m_btn_export_class->m_text = "Export classes";
	m_btn_export_class->m_color_text = &g_lv_color_text_white;

	m_chk_detection = new MButtonCheck(wnd, MRect(MRectPosition::LB, 10, 150, 25, 25));
	m_chk_detection->m_color_text= &g_lv_color_text_white;
	m_stc_detection = new MStatic(wnd, MRect(MRectPosition::LB, 50, 150, 250, 25));
	m_stc_detection->m_color_text = &g_lv_color_text;
	m_stc_detection->m_text = TEXT("Object Detection");

	m_chk_segmentation = new MButtonCheck(wnd, MRect(MRectPosition::LB, 10, 100, 25, 25));
	m_chk_segmentation->m_color_text = &g_lv_color_text_white;
	m_stc_segmentation = new MStatic(wnd, MRect(MRectPosition::LB, 50, 100, 250, 25));
	m_stc_segmentation->m_color_text = &g_lv_color_text;
	m_stc_segmentation->m_text = TEXT("Semantic Segmentation");

	m_chk_square = new MButtonCheck(wnd, MRect(MRectPosition::LB, 320, 150, 25, 25));
	m_chk_square->m_color_text = &g_lv_color_text_white;
	m_stc_square = new MStatic(wnd, MRect(MRectPosition::LB, 360, 150, 250, 25));
	m_stc_square->m_color_text = &g_lv_color_text;
	m_stc_square->m_text = TEXT("Square");

	m_chk_rectangle = new MButtonCheck(wnd, MRect(MRectPosition::LB, 320, 100, 25, 25));
	m_chk_rectangle->m_color_text = &g_lv_color_text_white;
	m_stc_rectangle = new MStatic(wnd, MRect(MRectPosition::LB, 360, 100, 250, 25));
	m_stc_rectangle->m_color_text = &g_lv_color_text;
	m_stc_rectangle->m_text = TEXT("Rectangle");

	m_chk_r_mid = new MButtonCheck(wnd, MRect(MRectPosition::LB, 320, 50, 25, 25));
	m_chk_r_mid->m_color_text = &g_lv_color_text_white;
	m_stc_r_mid = new MStatic(wnd, MRect(MRectPosition::LB, 360, 50, 250, 25));
	m_stc_r_mid->m_color_text = &g_lv_color_text;
	m_stc_r_mid->m_text = TEXT("Rotate by center");

	m_chk_r_2pt = new MButtonCheck(wnd, MRect(MRectPosition::LB, 320, 0, 25, 25));
	m_chk_r_2pt->m_color_text = &g_lv_color_text_white;
	m_stc_r_2pt = new MStatic(wnd, MRect(MRectPosition::LB, 360, 0, 250, 25));
	m_stc_r_2pt->m_color_text = &g_lv_color_text;
	m_stc_r_2pt->m_text = TEXT("Rotate by 2 points");

	m_chk_tracking = new MButtonCheck(wnd, MRect(MRectPosition::LB, 630, 150, 25, 25));
	m_chk_tracking->m_color_text = &g_lv_color_text_white;
	m_stc_tracking = new MStatic(wnd, MRect(MRectPosition::LB, 670, 150, 250, 25));
	m_stc_tracking->m_color_text = &g_lv_color_text;
	m_stc_tracking->m_text = TEXT("Tracking");


	m_stc_image=new MStatic(wnd, MRect(MRectPosition::RB, 10, 50, 380, 30));
	m_stc_image->m_color_text = &g_lv_color_text;

	m_stc_class = new MStatic(wnd, MRect(MRectPosition::RB, 10, 0, 380, 30));
	m_stc_class->m_color_text = &g_lv_color_text;

	m_chk_rectangle->check = true;
	m_chk_detection->check = true;
	///DEV
	m_chk_segmentation->disable = true;
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
	VirtualView::OnPaint(pDC);
	static bool init = true;
	if (init == true) {
		init = false;
		this->m_parent->SetTimer(MOUSE_LEAVE_TIMER_ID, 3000, nullptr);
	}
	m_list_image->OnPaint(pDC);
	if (m_list_image->m_data.size() == 0) {
		CRect rect=m_list_image->m_rect.GetRect(this->GetViewRect());
		DrawBitmap(pDC, rect, IDB_USAGE);
	} 
	m_list_class->OnPaint(pDC);
	m_edit_class->OnPaint(pDC);
	m_btn_add_class->OnPaint(pDC);
	m_btn_load_class->OnPaint(pDC);
	m_btn_export_class->OnPaint(pDC);
	m_chk_detection->OnPaint(pDC);
	m_stc_detection->OnPaint(pDC);
	m_chk_segmentation->OnPaint(pDC);
	m_stc_segmentation->OnPaint(pDC);
	if (m_chk_detection->check == true) {
		m_chk_square->OnPaint(pDC);
		m_stc_square->OnPaint(pDC);
		m_chk_rectangle->OnPaint(pDC);
		m_stc_rectangle->OnPaint(pDC);
		m_chk_r_mid->OnPaint(pDC);
		m_stc_r_mid->OnPaint(pDC);
		m_chk_r_2pt->OnPaint(pDC);
		m_stc_r_2pt->OnPaint(pDC);
		m_chk_tracking->OnPaint(pDC);
		m_stc_tracking->OnPaint(pDC);
	}
	int c = 0;
	for (auto&e : m_list_image->m_data) {
		if (e.second == true) {
			c++;
		}
	}
	size_t ratio = 0;
	if (c != 0) {
		ratio = c * 100 / m_list_image->m_data.size();
	}
	m_stc_image->m_text.Format(TEXT("%d images loaded(%d%% completed)"),m_list_image->m_data.size(),ratio);
	m_stc_image->OnPaint(pDC);
	m_stc_class->m_text.Format(TEXT("%d classes are ready"), m_list_class->m_data.size());
	m_stc_class->OnPaint(pDC);
	if (g_exporting == true) {
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
	if (g_exporting == true)return;
	VirtualView::OnLButtonDown(nFlags, point);
	m_list_image->OnLButtonDown();
	m_list_class->OnLButtonDown();
	m_edit_class->OnLButtonDown();

	if (m_chk_detection->OnLButtonDown() == M_CLICKED) {
		m_chk_detection->check = true;
		m_chk_segmentation->check = false;
	}
	if (m_chk_segmentation->OnLButtonDown() == M_CLICKED) {
		m_chk_segmentation->check = true;
		m_chk_detection->check = false;
	}
	if (m_chk_detection->check == true) {
		if (m_chk_tracking->OnLButtonDown() == M_CLICKED) {
			if (m_chk_r_mid->check == true || m_chk_r_2pt->check == true) {
				this->m_parent->MessageBox(TEXT("Can't use tracking with rotated-rect"));
				m_chk_tracking->check = false;
			}
		}
		if (m_chk_square->OnLButtonDown() == M_CLICKED ) {
			m_chk_square->check = true;
			m_chk_rectangle->check = false;
			m_chk_r_mid->check = false;
			m_chk_r_2pt->check = false;
		}
		if (m_chk_rectangle->OnLButtonDown() == M_CLICKED ) {
			m_chk_rectangle->check = true;
			m_chk_square->check = false;
			m_chk_r_mid->check = false;
			m_chk_r_2pt->check = false;
		}
		if (m_chk_r_mid->OnLButtonDown() == M_CLICKED ) {
			m_chk_r_mid->check = true;
			m_chk_rectangle->check = false;
			m_chk_square->check = false;
			m_chk_r_2pt->check = false;
			m_chk_tracking->check = false;
		}
		if (m_chk_r_2pt->OnLButtonDown() == M_CLICKED) {
			m_chk_r_2pt->check = true;
			m_chk_rectangle->check = false;
			m_chk_square->check = false;
			m_chk_r_mid->check = false;
			m_chk_tracking->check = false;
		}
	}
	if (m_btn_add_class->OnLButtonDown() == M_CLICKED) {
		if (m_edit_class->m_text.GetLength() > 0) {
			auto val = std::make_pair(m_edit_class->m_text, false);
			if (std::find(m_list_class->m_data.begin(), m_list_class->m_data.end(), val) == m_list_class->m_data.end()) {
				m_list_class->m_data.push_back(val);
				m_edit_class->Reset();
			} else {
				this->m_parent->MessageBox(TEXT("Existing class"));
			}
			
		}
	}
	if (m_btn_load_class->OnLButtonDown() == M_CLICKED) {
		//class 리스트 파일을 로드 합니다.
		const TCHAR* IMAGE_FILTER = TEXT("ClassList File(*txt;*.names)|*.txt;*.names|TXT File(*.txt)|*.txt|NAMES File(*.names)|*.names|");
		CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, IMAGE_FILTER);
		CString strFileList;
		const unsigned int c_cbBuffSize = (1 * (MAX_PATH + 1)) + 1;
		dlg.GetOFN().lpstrFile = strFileList.GetBuffer(c_cbBuffSize);
		dlg.GetOFN().nMaxFile = c_cbBuffSize;
		if (IDOK == dlg.DoModal()) {
			CStdioFile file(dlg.GetPathName(), CFile::modeRead | CFile::typeText);
			CString cstr;
			int overlap = 0;
			while (1) {
				if (file.ReadString(cstr) == TRUE) {
					if (cstr.GetLength() > 0) {
						auto val = std::make_pair(cstr, false);
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
				CString msg;
				msg.Format(TEXT("%d classes are overlapped!!"), overlap);
				this->m_parent->MessageBox(msg);
			}
		}
	}
	if (m_btn_export_class->OnLButtonDown() == M_CLICKED) {
		const TCHAR* FILTER = TEXT("All Files (*.*)|*.*|Text Files (*.txt)|*.txt|");
		CFileDialog dlg(FALSE, TEXT("txt"), TEXT("*.txt"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, FILTER, NULL);
		dlg.m_ofn.nFilterIndex = 2;
		if (IDOK == dlg.DoModal()) {
			CString cfile=dlg.GetPathName();
			std::string file = mspring::String::ToString(cfile);
			std::fstream fout;
			fout.open(file, std::ios::out);
			if (fout.is_open() == false) {
				this->m_parent->MessageBox(TEXT("Invalid file path!"));
				return;
			}
			for (auto&e : m_list_class->m_data) {
				fout << mspring::String::ToString(e.first) << std::endl;
			}
			fout.close();
			this->m_parent->MessageBox(TEXT("Export success"));
		}
	}
}
void ListView::OnLButtonUp(UINT nFlags, CPoint point) {
	if (g_exporting == true)return;
	VirtualView::OnLButtonUp(nFlags, point);
	m_list_image->OnLButtonUp();
	m_list_class->OnLButtonUp();
	m_edit_class->OnLButtonUp();
	m_btn_add_class->OnLButtonUp();
	m_btn_load_class->OnLButtonUp();
}
void ListView::OnLButtonDblClk(UINT nFlags, CPoint point) {
	if (g_exporting == true)return;
	VirtualView::OnLButtonDblClk(nFlags, point);
}
void ListView::OnRButtonDown(UINT nFlags, CPoint point) {
	if (g_exporting == true)return;
	VirtualView::OnRButtonDown(nFlags, point);
	m_list_image->OnRButtonDown();
	m_list_class->OnRButtonDown();
	m_edit_class->OnRButtonDown();
	m_btn_add_class->OnRButtonDown();
	m_btn_load_class->OnRButtonDown();
}
void ListView::OnRButtonUp(UINT nFlags, CPoint point) {
	if (g_exporting == true)return;
	VirtualView::OnRButtonUp(nFlags, point);
	m_list_image->OnRButtonUp();
	m_list_class->OnRButtonUp();
	m_edit_class->OnRButtonUp();
	m_btn_add_class->OnRButtonUp();
	m_btn_load_class->OnRButtonUp();

	GetCursorPos(&point);
	int idx=m_list_image->GetElementByPoint(this->GetMousePoint());
	if (idx != -1) {
		MSpringMenu popup(this->m_parent);
		popup.CreatePopupMenu();
		popup.AppendMenu(MF_STRING, 1, TEXT("Go to tagging"));
		popup.AppendMenu(MF_STRING, 2, TEXT("Remove from list"));
		popup.AppendMenu(MF_STRING, 3, TEXT("Remove from disk"));
		popup.MakeItemsOwnDraw();
		int menu_id=popup.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON| TPM_RETURNCMD, point.x, point.y, this->m_parent);
		popup.DestroyMenu();
		switch (menu_id) {
			case 1: {
				//this->m_parent->MessageBox(TEXT("Go to tagging"));
				g_command_go2tagging = idx;
				AfxGetMainWnd()->SendMessage(WM_LBUTTONDOWN, NULL, NULL);
			}break;
			case 2: {
				m_list_image->m_data.erase(m_list_image->m_data.begin() + idx);
			}break;
			case 3: {
				std::string img_path = mspring::String::ToString(m_list_image->m_data[idx].first);
				ispring::File::FileErase(img_path);
				m_list_image->m_data.erase(m_list_image->m_data.begin() + idx);
				std::string tsp_path = img_path.substr(0, img_path.find_last_of('.')) + ".tsp";
				ispring::File::FileErase(tsp_path);
				std::string tsps_path = img_path.substr(0, img_path.find_last_of('.')) + ".tsps";
				ispring::File::FileErase(tsps_path);
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
	if (g_exporting == true)return TRUE;
	VirtualView::OnMouseWheel(nFlags, zDelta, pt);
	m_list_image->OnMouseWheel(zDelta);
	m_list_class->OnMouseWheel(zDelta);
	m_edit_class->OnMouseWheel(zDelta);
	m_btn_add_class->OnMouseWheel(zDelta);
	m_btn_load_class->OnMouseWheel(zDelta);
	return TRUE;
}
void ListView::OnMouseMove(UINT nFlags, CPoint point) {
	if (g_exporting == true)return;
	VirtualView::OnMouseMove(nFlags, point);
	m_list_image->OnMouseMove();
	m_list_class->OnMouseMove();
	m_edit_class->OnMouseMove();
	m_btn_add_class->OnMouseMove();
	m_btn_load_class->OnMouseMove();
	m_chk_detection->OnMouseMove();
	m_chk_segmentation->OnMouseMove();
	if (m_chk_detection->check == true) {
		m_chk_square->OnMouseMove();
		m_chk_rectangle->OnMouseMove();
		m_chk_r_mid->OnMouseMove();
		m_chk_r_2pt->OnMouseMove();
		m_chk_tracking->OnMouseMove();
	}
}
void ListView::OnMouseLeave() {
	if (g_exporting == true)return;
	VirtualView::OnMouseLeave();
	m_list_image->OnMouseLeave();
	m_list_class->OnMouseLeave();
	m_edit_class->OnMouseLeave();
	m_btn_add_class->OnMouseLeave();
	m_btn_load_class->OnMouseLeave();
}

BOOL ListView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) {
	VirtualView::OnSetCursor(pWnd, nHitTest, message);
	return TRUE;
}
void ListView::OnSize(UINT nType, int cx, int cy) {
	VirtualView::OnSize(nType, cx, cy);	
}

void ListView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if (g_exporting == true)return;
	VirtualView::OnKeyDown(nChar, nRepCnt, nFlags);
	m_list_image->OnKeyDown(nChar);
	m_list_class->OnKeyDown(nChar);
	m_edit_class->OnKeyDown(nChar);
	m_btn_add_class->OnKeyDown(nChar);
	m_btn_load_class->OnKeyDown(nChar);
}

void ListView::OnTimer(UINT_PTR nIDEvent) {
	if (g_exporting == true)return;
	VirtualView::OnTimer(nIDEvent);
	m_list_image->OnTimer(nIDEvent);
	m_list_class->OnTimer(nIDEvent);
	m_edit_class->OnTimer(nIDEvent);
	m_btn_add_class->OnTimer(nIDEvent);
	m_btn_load_class->OnTimer(nIDEvent);
	if (nIDEvent == MOUSE_LEAVE_TIMER_ID) {
		IdentifyTagInfo();
	}
}
void ListView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if (g_exporting == true)return;
	VirtualView::OnChar(nChar, nRepCnt, nFlags);
	m_list_image->OnChar(nChar);
	m_list_class->OnChar(nChar);
	if (nChar != VK_RETURN) {
		if (std::isspace(nChar) == false) {
			m_edit_class->OnChar(nChar);
		}
	} else if (m_edit_class->isFocused() == true) {
		if (m_edit_class->m_text.GetLength() > 0) {
			auto val = std::make_pair(m_edit_class->m_text, false);
			if (std::find(m_list_class->m_data.begin(), m_list_class->m_data.end(), val) == m_list_class->m_data.end()) {
				m_list_class->m_data.push_back(val);
				m_edit_class->Reset();
			} else {
				this->m_parent->MessageBox(TEXT("Existing class"));
			}
		}
	}
	m_btn_add_class->OnChar(nChar);
	m_btn_load_class->OnChar(nChar);
}
LRESULT ListView::OnComposition(WPARAM wParam, LPARAM lParam) {
	if (g_exporting == true)return FALSE;
	VirtualView::OnComposition(wParam, lParam);
	m_edit_class->OnComposition(wParam, lParam);
	return 1;
}