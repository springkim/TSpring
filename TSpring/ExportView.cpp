#include "stdafx.h"
#include "ExportView.h"
#include<opencv2/opencv.hpp>
ExportView::ExportView(CWnd* wnd) : VirtualView(wnd), m_yolov2(wnd){
	m_list_class = new MListBox(wnd, MRect(MRectPosition::R, 380, 10, 10, 300));
	m_list_class->m_color_bk = &g_lv_color_bk;
	m_list_class->m_color_text = &g_lv_color_text;
	m_list_class->is_check = true;

	int base = 10;
	m_stc_architecture = new MStatic(wnd, MRect(MRectPosition::LT, base, 10, 100 + 30, 30));
	m_stc_architecture->m_color_text = &g_solarized_yellow;
	m_stc_architecture->m_text = TEXT("Architecture");

	m_chk_yolov2 = new MButtonCheck(wnd, MRect(MRectPosition::LT, base, 70, 25, 25));
	m_chk_yolov2->m_color_text= &g_lv_color_text_white;
	m_stc_yolov2 = new MStatic(wnd, MRect(MRectPosition::LT, base+30, 70, 100, 25));
	m_stc_yolov2->m_color_text = &g_lv_color_text;
	m_stc_yolov2->m_text = TEXT("YOLO v2");


	m_chk_fasterrcnn = new MButtonCheck(wnd, MRect(MRectPosition::LT, base, 130, 25, 25));
	m_chk_fasterrcnn->m_color_text = &g_lv_color_text_white;
	m_stc_fasterrcnn = new MStatic(wnd, MRect(MRectPosition::LT, base+30, 130, 100, 25));
	m_stc_fasterrcnn->m_color_text = &g_lv_color_text;
	m_stc_fasterrcnn->m_text = TEXT("FasterRCNN");

	m_chk_dlibcnn = new MButtonCheck(wnd, MRect(MRectPosition::LT, base, 190, 25, 25));
	m_chk_dlibcnn->m_color_text = &g_lv_color_text_white;
	m_stc_dlibcnn = new MStatic(wnd, MRect(MRectPosition::LT, base+30, 190, 100, 25));
	m_stc_dlibcnn->m_color_text = &g_lv_color_text;
	m_stc_dlibcnn->m_text = TEXT("dlib cnn");

	m_chk_rotate = new MButtonCheck(wnd, MRect(MRectPosition::RB, 380-25, 270, 25, 25));
	m_chk_rotate->m_color_text = &g_lv_color_text_white;
	m_stc_rotate = new MStatic(wnd, MRect(MRectPosition::RB, 380-25-200-10, 270, 200, 25));
	m_stc_rotate->m_color_text = &g_lv_color_text;
	m_stc_rotate->m_text = TEXT("Enable Rotate");

	m_chk_boundingbox = new MButtonCheck(wnd, MRect(MRectPosition::RB, 380 - 25, 240, 25, 25));
	m_chk_boundingbox->m_color_text = &g_lv_color_text_white;
	m_stc_boundingbox = new MStatic(wnd, MRect(MRectPosition::RB, 380 - 25 - 200 - 10, 240, 200, 25));
	m_stc_boundingbox->m_color_text = &g_lv_color_text;
	m_stc_boundingbox->m_text = TEXT("BoundingBox");
	m_chk_ignore_rbox = new MButtonCheck(wnd, MRect(MRectPosition::RB, 380 - 25, 210, 25, 25));
	m_chk_ignore_rbox->m_color_text = &g_lv_color_text_white;
	m_stc_ignore_rbox = new MStatic(wnd, MRect(MRectPosition::RB, 380 - 25 - 200 - 10, 210, 200, 25));
	m_stc_ignore_rbox->m_color_text = &g_lv_color_text;
	m_stc_ignore_rbox->m_text = TEXT("Ignore RotatedRect");

	m_stc_name=new MStatic(wnd, MRect(MRectPosition::RB, 310, 150, 80, 30));
	m_stc_name->m_color_text = &g_lv_color_text;
	m_stc_name->m_text = TEXT("Data : ");

	m_edit_name = new MEdit(wnd, MRect(MRectPosition::RB, 10, 150, 300, 30));
	m_edit_name->m_color_text = &g_lv_color_text_white;
	
	m_btn_export = new MButton(wnd, MRect(MRectPosition::RB, 10, 90, 380, 30));
	m_btn_export->m_text = TEXT("Export data");
	m_btn_export->m_color_text = &g_lv_color_text_white;

	for (size_t i = 2; 25 * i<300; i++) {
		MStatic* doc = new MStatic(wnd, MRect(MRectPosition::B, 10,25, 400, 25*i));
		doc->m_color_text = &g_lv_color_text;
		doc->m_text = TEXT("");
		m_doc.push_front(doc);
		m_objs.push_back(doc);
	}

	m_objs.push_back(m_edit_name);
	m_objs.push_back(m_btn_export);
	m_objs.push_back(m_stc_name);

	m_objs.push_back(m_stc_architecture);
	m_objs.push_back(m_chk_yolov2);
	m_objs.push_back(m_stc_yolov2);
	m_objs.push_back(m_chk_fasterrcnn);
	m_objs.push_back(m_stc_fasterrcnn);
	m_objs.push_back(m_chk_dlibcnn);
	m_objs.push_back(m_stc_dlibcnn);
	m_objs.push_back(m_chk_rotate);
	m_objs.push_back(m_stc_rotate);

	m_objs.push_back(m_chk_boundingbox);
	m_objs.push_back(m_stc_boundingbox);
	m_objs.push_back(m_chk_ignore_rbox);
	m_objs.push_back(m_stc_ignore_rbox);

	m_chk_yolov2->check = true;
	m_chk_ignore_rbox->check = true;
}


ExportView::~ExportView() {
}
int ExportView::OnCreate() {
	VirtualView::OnCreate();
	return 1;
}
void ExportView::OnDestroy() {
	VirtualView::OnDestroy();
}

void ExportView::OnPaint(CDC* pDC) {
	VirtualView::OnPaint(pDC);
	if (this->isSameClassList() == false) {
		m_list_class->m_data.assign(g_class_data->size(), std::pair<CString, bool>());
		std::copy(g_class_data->begin(), g_class_data->end(), m_list_class->m_data.begin());
		for (auto&e : m_list_class->m_data) {
			e.second = true;
		}
	}
	m_list_class->m_select = -1;
	m_list_class->OnPaint(pDC);
	for (auto&obj : m_objs) {
		obj->OnPaint(pDC);
	}
	if (m_chk_yolov2->check == true) {
		m_yolov2.OnPaint(pDC);
	}
	WriteDocument();
}
void ExportView::OnSetFocus(CWnd* pOldWnd) {
	VirtualView::OnSetFocus(pOldWnd);
}
void ExportView::OnKillFocus(CWnd* pNewWnd) {
	VirtualView::OnKillFocus(pNewWnd);
}

void ExportView::OnLButtonDown(UINT nFlags, CPoint point) {
	VirtualView::OnLButtonDown(nFlags, point);
	m_list_class->OnLButtonDown();
	m_edit_name->OnLButtonDown();
	if (m_btn_export->OnLButtonDown() == M_CLICKED) {
		///Do...
	}
	CPoint rpoint = this->GetMousePoint();
	int sel=m_list_class->GetElementByPoint(rpoint);
	if (sel != -1) {
		m_list_class->m_data[sel].second = !m_list_class->m_data[sel].second;
	}
	if (m_chk_yolov2->OnLButtonDown() == M_CLICKED) {
		m_chk_yolov2->check = true;
		m_chk_fasterrcnn->check = false;
		m_chk_dlibcnn->check = false;
	}
	if (m_chk_fasterrcnn->OnLButtonDown() == M_CLICKED) {
		m_chk_fasterrcnn->check = true;
		m_chk_yolov2->check = false;
		m_chk_dlibcnn->check = false;
	}
	if (m_chk_dlibcnn->OnLButtonDown() == M_CLICKED ) {
		m_chk_dlibcnn->check = true;
		m_chk_yolov2->check = false;
		m_chk_fasterrcnn->check = false;
	}
	if (m_chk_yolov2->check == true) {
		m_yolov2.OnLButtonDown(nFlags,point);
	}
	m_chk_rotate->OnLButtonDown();
	if (m_chk_rotate->check == true) {
		m_chk_boundingbox->disable = true;
		m_chk_ignore_rbox->disable = true;
	} else {
		m_chk_boundingbox->disable = false;
		m_chk_ignore_rbox->disable = false;
		if (m_chk_boundingbox->OnLButtonDown() == M_CLICKED) {
			m_chk_boundingbox->check = true;
			m_chk_ignore_rbox->check = false;
		}
		if (m_chk_ignore_rbox->OnLButtonDown() == M_CLICKED) {
			m_chk_boundingbox->check = false;
			m_chk_ignore_rbox->check = true;
		}
	}
}
void ExportView::OnLButtonUp(UINT nFlags, CPoint point) {
	VirtualView::OnLButtonUp(nFlags, point);
	m_list_class->OnLButtonUp();
	m_edit_name->OnLButtonUp();
}
void ExportView::OnLButtonDblClk(UINT nFlags, CPoint point) {
	VirtualView::OnLButtonDblClk(nFlags, point);
}
void ExportView::OnRButtonUp(UINT nFlags, CPoint point) {
	VirtualView::OnRButtonUp(nFlags, point);
}
BOOL ExportView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) {
	VirtualView::OnMouseWheel(nFlags, zDelta, pt);
	m_list_class->OnMouseWheel(zDelta);
	return TRUE;
}
void ExportView::OnMouseMove(UINT nFlags, CPoint point) {
	VirtualView::OnMouseMove(nFlags, point);
	m_list_class->OnMouseMove();
	for (auto&obj : m_objs) {
		obj->OnMouseMove();
	}
	if (m_chk_yolov2->check == true) {
		m_yolov2.OnMouseMove(nFlags, point);
	}
}
void ExportView::OnMouseLeave() {
	VirtualView::OnMouseLeave();
}

BOOL ExportView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) {
	VirtualView::OnSetCursor(pWnd, nHitTest, message);
	return TRUE;
}
void ExportView::OnSize(UINT nType, int cx, int cy) {
	VirtualView::OnSize(nType, cx, cy);
}

void ExportView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	VirtualView::OnKeyDown(nChar, nRepCnt, nFlags);
	m_edit_name->OnKeyDown(nChar);
}

void ExportView::OnTimer(UINT_PTR nIDEvent) {
	VirtualView::OnTimer(nIDEvent);
	m_list_class->OnTimer(nIDEvent);
	m_edit_name->OnTimer(nIDEvent);
}
void ExportView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) {
	VirtualView::OnChar(nChar, nRepCnt, nFlags);
	if (nChar != VK_RETURN) {
		if (std::isspace(nChar) == false && nChar!='\\' && nChar!='/' && nChar!=':' && nChar!='?' && nChar!='\"' && nChar!='<' && nChar!='>' && nChar!='|') {
			m_edit_name->OnChar(nChar);
		}
	}
}
LRESULT ExportView::OnComposition(WPARAM wParam, LPARAM lParam) {
	VirtualView::OnComposition(wParam, lParam);
	m_edit_name->OnComposition(wParam, lParam);
	return 1;
}