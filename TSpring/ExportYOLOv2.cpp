#include "stdafx.h"
#include "ExportYOLOv2.h"
void Export(bool is_rotate, bool is_ignore) {

}

ExportYOLOv2::ExportYOLOv2(CWnd* wnd):VirtualView(wnd) {
	int base = 160;
	int text_width = 100;
	m_stc_basemodel = new MStatic(wnd, MRect(MRectPosition::LT, base, 10, text_width+30, 30));
	m_stc_basemodel->m_color_text = &g_solarized_yellow;
	m_stc_basemodel->m_text = TEXT("Base model");

	m_chk_darknet_r = new MButtonCheck(wnd, MRect(MRectPosition::LT, base, 70, 25, 25));
	m_chk_darknet_r->m_color_text = &g_lv_color_text_white;
	m_stc_darknet_r = new MStatic(wnd, MRect(MRectPosition::LT, base+30, 70, text_width, 25));
	m_stc_darknet_r->m_color_text = &g_lv_color_text;
	m_stc_darknet_r->m_text = TEXT("Darknet-R");

	m_chk_darknet19 = new MButtonCheck(wnd, MRect(MRectPosition::LT, base, 130, 25, 25));
	m_chk_darknet19->m_color_text = &g_lv_color_text_white;
	m_stc_darknet19 = new MStatic(wnd, MRect(MRectPosition::LT, base + 30, 130, text_width, 25));
	m_stc_darknet19->m_color_text = &g_lv_color_text;
	m_stc_darknet19->m_text = TEXT("Darknet19");

	m_chk_resnet50 = new MButtonCheck(wnd, MRect(MRectPosition::LT, base, 190, 25, 25));
	m_chk_resnet50->m_color_text = &g_lv_color_text_white;
	m_stc_resnet50 = new MStatic(wnd, MRect(MRectPosition::LT, base + 30, 190, text_width, 25));
	m_stc_resnet50->m_color_text = &g_lv_color_text;
	m_stc_resnet50->m_text = TEXT("Resnet50");

	m_chk_densenet201 = new MButtonCheck(wnd, MRect(MRectPosition::LT, base, 250, 25, 25));
	m_chk_densenet201->m_color_text = &g_lv_color_text_white;
	m_stc_densenet201 = new MStatic(wnd, MRect(MRectPosition::LT, base + 30, 250, text_width, 25));
	m_stc_densenet201->m_color_text = &g_lv_color_text;
	m_stc_densenet201->m_text = TEXT("Densenet201");

	m_base_model.push_back(m_chk_darknet_r);
	m_base_model.push_back(m_chk_darknet19);
	m_base_model.push_back(m_chk_resnet50);
	m_base_model.push_back(m_chk_densenet201);
	
	m_objs.push_back(m_stc_basemodel);
	m_objs.push_back(m_chk_darknet_r);
	m_objs.push_back(m_stc_darknet_r);
	m_objs.push_back(m_chk_darknet19);
	m_objs.push_back(m_stc_darknet19);
	m_objs.push_back(m_chk_resnet50);
	m_objs.push_back(m_stc_resnet50);
	m_objs.push_back(m_chk_densenet201);
	m_objs.push_back(m_stc_densenet201);
	//=================ANCHORS
	base = 310;
	m_stc_anchors = new MStatic(wnd, MRect(MRectPosition::LT, base, 10, text_width+30, 30));
	m_stc_anchors->m_color_text = &g_solarized_yellow;
	m_stc_anchors->m_text = TEXT("Anchors");

	m_chk_anchor3 = new MButtonCheck(wnd, MRect(MRectPosition::LT, base, 70, 25, 25));
	m_chk_anchor3->m_color_text = &g_lv_color_text_white;
	m_stc_anchor3 = new MStatic(wnd, MRect(MRectPosition::LT, base + 30, 70, text_width, 25));
	m_stc_anchor3->m_color_text = &g_lv_color_text;
	m_stc_anchor3->m_text = TEXT("3 anchors");

	m_chk_anchor5 = new MButtonCheck(wnd, MRect(MRectPosition::LT, base, 130, 25, 25));
	m_chk_anchor5->m_color_text = &g_lv_color_text_white;
	m_stc_anchor5 = new MStatic(wnd, MRect(MRectPosition::LT, base + 30, 130, text_width, 25));
	m_stc_anchor5->m_color_text = &g_lv_color_text;
	m_stc_anchor5->m_text = TEXT("5 anchor");

	m_chk_anchor7 = new MButtonCheck(wnd, MRect(MRectPosition::LT, base, 190, 25, 25));
	m_chk_anchor7->m_color_text = &g_lv_color_text_white;
	m_stc_anchor7 = new MStatic(wnd, MRect(MRectPosition::LT, base + 30, 190, text_width, 25));
	m_stc_anchor7->m_color_text = &g_lv_color_text;
	m_stc_anchor7->m_text = TEXT("7 anchors");

	m_chk_anchor9 = new MButtonCheck(wnd, MRect(MRectPosition::LT, base, 250, 25, 25));
	m_chk_anchor9->m_color_text = &g_lv_color_text_white;
	m_stc_anchor9 = new MStatic(wnd, MRect(MRectPosition::LT, base + 30, 250, text_width, 25));
	m_stc_anchor9->m_color_text = &g_lv_color_text;
	m_stc_anchor9->m_text = TEXT("9 anchors");

	m_chk_anchor11 = new MButtonCheck(wnd, MRect(MRectPosition::LT, base, 310, 25, 25));
	m_chk_anchor11->m_color_text = &g_lv_color_text_white;
	m_stc_anchor11 = new MStatic(wnd, MRect(MRectPosition::LT, base + 30, 310, text_width, 25));
	m_stc_anchor11->m_color_text = &g_lv_color_text;
	m_stc_anchor11->m_text = TEXT("11 anchors");
	m_anchors.push_back(m_chk_anchor3);
	m_anchors.push_back(m_chk_anchor5);
	m_anchors.push_back(m_chk_anchor7);
	m_anchors.push_back(m_chk_anchor9);
	m_anchors.push_back(m_chk_anchor11);
	m_objs.push_back(m_stc_anchors);
	m_objs.push_back(m_chk_anchor3);
	m_objs.push_back(m_stc_anchor3);
	m_objs.push_back(m_chk_anchor5);
	m_objs.push_back(m_stc_anchor5);
	m_objs.push_back(m_chk_anchor7);
	m_objs.push_back(m_stc_anchor7);
	m_objs.push_back(m_chk_anchor9);
	m_objs.push_back(m_stc_anchor9);
	m_objs.push_back(m_chk_anchor11);
	m_objs.push_back(m_stc_anchor11);
	//==============RESOLUTION
	base = 460;
	m_stc_resolution = new MStatic(wnd, MRect(MRectPosition::LT, base, 10, text_width+30, 30));
	m_stc_resolution->m_color_text = &g_solarized_yellow;
	m_stc_resolution->m_text = TEXT("Resolution");
	m_chk_resolution416 = new MButtonCheck(wnd, MRect(MRectPosition::LT, base, 70, 25, 25));
	m_chk_resolution416->m_color_text = &g_lv_color_text_white;
	m_stc_resolution416 = new MStatic(wnd, MRect(MRectPosition::LT, base + 30, 70, text_width, 25));
	m_stc_resolution416->m_color_text = &g_lv_color_text;
	m_stc_resolution416->m_text = TEXT("416x416");

	m_chk_resolution480 = new MButtonCheck(wnd, MRect(MRectPosition::LT, base, 130, 25, 25));
	m_chk_resolution480->m_color_text = &g_lv_color_text_white;
	m_stc_resolution480 = new MStatic(wnd, MRect(MRectPosition::LT, base + 30, 130, text_width, 25));
	m_stc_resolution480->m_color_text = &g_lv_color_text;
	m_stc_resolution480->m_text = TEXT("480x480");

	m_chk_resolution544 = new MButtonCheck(wnd, MRect(MRectPosition::LT, base, 190, 25, 25));
	m_chk_resolution544->m_color_text = &g_lv_color_text_white;
	m_stc_resolution544 = new MStatic(wnd, MRect(MRectPosition::LT, base + 30, 190, text_width, 25));
	m_stc_resolution544->m_color_text = &g_lv_color_text;
	m_stc_resolution544->m_text = TEXT("544x544");

	m_chk_resolution608 = new MButtonCheck(wnd, MRect(MRectPosition::LT, base, 250, 25, 25));
	m_chk_resolution608->m_color_text = &g_lv_color_text_white;
	m_stc_resolution608 = new MStatic(wnd, MRect(MRectPosition::LT, base + 30, 250, text_width, 25));
	m_stc_resolution608->m_color_text = &g_lv_color_text;
	m_stc_resolution608->m_text = TEXT("608x608");

	m_chk_resolution1088 = new MButtonCheck(wnd, MRect(MRectPosition::LT, base, 310, 25, 25));
	m_chk_resolution1088->m_color_text = &g_lv_color_text_white;
	m_stc_resolution1088 = new MStatic(wnd, MRect(MRectPosition::LT, base + 30, 310, text_width, 25));
	m_stc_resolution1088->m_color_text = &g_lv_color_text;
	m_stc_resolution1088->m_text = TEXT("1088x1088");

	m_chk_resolution_random = new MButtonCheck(wnd, MRect(MRectPosition::LT, base, 370, 25, 25));
	m_chk_resolution_random->m_color_text = &g_lv_color_text_white;
	m_stc_resolution_random = new MStatic(wnd, MRect(MRectPosition::LT, base + 30, 370, text_width, 25));
	m_stc_resolution_random->m_color_text = &g_lv_color_text;
	m_stc_resolution_random->m_text = TEXT("Random size");

	m_resolution.push_back(m_chk_resolution416);
	m_resolution.push_back(m_chk_resolution480);
	m_resolution.push_back(m_chk_resolution544);
	m_resolution.push_back(m_chk_resolution608);
	m_resolution.push_back(m_chk_resolution1088);
	m_resolution.push_back(m_chk_resolution_random);
	m_objs.push_back(m_stc_resolution);
	m_objs.push_back(m_chk_resolution416);
	m_objs.push_back(m_stc_resolution416);
	m_objs.push_back(m_chk_resolution480);
	m_objs.push_back(m_stc_resolution480);
	m_objs.push_back(m_chk_resolution544);
	m_objs.push_back(m_stc_resolution544);
	m_objs.push_back(m_chk_resolution608);
	m_objs.push_back(m_stc_resolution608);
	m_objs.push_back(m_chk_resolution1088);
	m_objs.push_back(m_stc_resolution1088);
	m_objs.push_back(m_chk_resolution_random);
	m_objs.push_back(m_stc_resolution_random);
	//=====NOISE
	base = 610;
	m_stc_noise = new MStatic(wnd, MRect(MRectPosition::LT, base, 10, text_width+30, 30));
	m_stc_noise->m_color_text = &g_solarized_yellow;
	m_stc_noise->m_text = TEXT("Noise");
	m_chk_noise_gray = new MButtonCheck(wnd, MRect(MRectPosition::LT, base, 70, 25, 25));
	m_chk_noise_gray->m_color_text = &g_lv_color_text_white;
	m_stc_noise_gray = new MStatic(wnd, MRect(MRectPosition::LT, base + 30, 70, 150, 25));
	m_stc_noise_gray->m_color_text = &g_lv_color_text;
	m_stc_noise_gray->m_text = TEXT("Gray");

	m_chk_noise_dot = new MButtonCheck(wnd, MRect(MRectPosition::LT, base, 130, 25, 25));
	m_chk_noise_dot->m_color_text = &g_lv_color_text_white;
	m_stc_noise_dot = new MStatic(wnd, MRect(MRectPosition::LT, base + 30, 130, 150, 25));
	m_stc_noise_dot->m_color_text = &g_lv_color_text;
	m_stc_noise_dot->m_text = TEXT("Salt&Pepper");

	m_chk_noise_flipLR = new MButtonCheck(wnd, MRect(MRectPosition::LT, base, 190, 25, 25));
	m_chk_noise_flipLR->m_color_text = &g_lv_color_text_white;
	m_stc_noise_flipLR = new MStatic(wnd, MRect(MRectPosition::LT, base + 30, 190, 150, 25));
	m_stc_noise_flipLR->m_color_text = &g_lv_color_text;
	m_stc_noise_flipLR->m_text = TEXT("Flip(LR)");

	m_chk_noise_flipTB = new MButtonCheck(wnd, MRect(MRectPosition::LT, base, 250, 25, 25));
	m_chk_noise_flipTB->m_color_text = &g_lv_color_text_white;
	m_stc_noise_flipTB = new MStatic(wnd, MRect(MRectPosition::LT, base + 30, 250, 150, 25));
	m_stc_noise_flipTB->m_color_text = &g_lv_color_text;
	m_stc_noise_flipTB->m_text = TEXT("Flip(TB)");

	m_chk_noise_blur = new MButtonCheck(wnd, MRect(MRectPosition::LT, base, 310, 25, 25));
	m_chk_noise_blur->m_color_text = &g_lv_color_text_white;
	m_stc_noise_blur = new MStatic(wnd, MRect(MRectPosition::LT, base + 30, 310, 150, 25));
	m_stc_noise_blur->m_color_text = &g_lv_color_text;
	m_stc_noise_blur->m_text = TEXT("Blur");
	
	m_noise.push_back(m_chk_noise_gray);
	m_noise.push_back(m_chk_noise_dot);
	m_noise.push_back(m_chk_noise_flipLR);
	m_noise.push_back(m_chk_noise_flipTB);
	m_noise.push_back(m_chk_noise_blur);
	m_objs.push_back(m_stc_noise);
	m_objs.push_back(m_chk_noise_gray);
	m_objs.push_back(m_stc_noise_gray);
	m_objs.push_back(m_chk_noise_dot);
	m_objs.push_back(m_stc_noise_dot);
	m_objs.push_back(m_chk_noise_flipLR);
	m_objs.push_back(m_stc_noise_flipLR);
	m_objs.push_back(m_chk_noise_flipTB);
	m_objs.push_back(m_stc_noise_flipTB);
	m_objs.push_back(m_chk_noise_blur);
	m_objs.push_back(m_stc_noise_blur);
	//INIT
	m_chk_darknet19->check = true;
	m_chk_anchor7->check = true;
	m_chk_resolution416->check = true;
}
ExportYOLOv2::~ExportYOLOv2() {
}
int ExportYOLOv2::OnCreate() {
	VirtualView::OnCreate();
	return 1;
}
void ExportYOLOv2::OnDestroy() {
	VirtualView::OnDestroy();
}

void ExportYOLOv2::OnPaint(CDC* pDC) {
	VirtualView::OnPaint(pDC);
	for (auto&obj : m_objs) {
		obj->OnPaint(pDC);
	}

}
void ExportYOLOv2::OnSetFocus(CWnd* pOldWnd) {
	VirtualView::OnSetFocus(pOldWnd);
}
void ExportYOLOv2::OnKillFocus(CWnd* pNewWnd) {
	VirtualView::OnKillFocus(pNewWnd);
}

void ExportYOLOv2::OnLButtonDown(UINT nFlags, CPoint point) {
	VirtualView::OnLButtonDown(nFlags, point);
	for (size_t i = 0; i < m_base_model.size(); i++) {
		if (m_base_model[i]->OnLButtonDown() == M_CLICKED) {
			for (size_t j = 0; j < m_base_model.size(); j++) {
				dynamic_cast<MButtonCheck*>(m_base_model[j])->check = false;
			}
			dynamic_cast<MButtonCheck*>(m_base_model[i])->check = true;
		}
	}
	for (size_t i = 0; i < m_anchors.size(); i++) {
		if (m_anchors[i]->OnLButtonDown() == M_CLICKED) {
			for (size_t j = 0; j < m_anchors.size(); j++) {
				dynamic_cast<MButtonCheck*>(m_anchors[j])->check = false;
			}
			dynamic_cast<MButtonCheck*>(m_anchors[i])->check = true;
		}
	}
	for (size_t i = 0; i < m_resolution.size(); i++) {
		if (m_resolution[i]->OnLButtonDown() == M_CLICKED) {
			for (size_t j = 0; j < m_resolution.size(); j++) {
				dynamic_cast<MButtonCheck*>(m_resolution[j])->check = false;
			}
			dynamic_cast<MButtonCheck*>(m_resolution[i])->check = true;
		}
	}
	for (size_t i = 0; i < m_noise.size(); i++) {
		m_noise[i]->OnLButtonDown();
	}
	m_chk_resolution_random->disable = m_chk_resnet50->check;
	if (m_chk_resolution_random->disable == true) {
		m_chk_resolution_random->check = false;
		m_chk_resolution416->check = true;
	}
}
void ExportYOLOv2::OnLButtonUp(UINT nFlags, CPoint point) {
	VirtualView::OnLButtonUp(nFlags, point);
}
void ExportYOLOv2::OnLButtonDblClk(UINT nFlags, CPoint point) {
	VirtualView::OnLButtonDblClk(nFlags, point);
}
void ExportYOLOv2::OnRButtonUp(UINT nFlags, CPoint point) {
	VirtualView::OnRButtonUp(nFlags, point);
}
BOOL ExportYOLOv2::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) {
	VirtualView::OnMouseWheel(nFlags, zDelta, pt);
	return TRUE;
}
void ExportYOLOv2::OnMouseMove(UINT nFlags, CPoint point) {
	VirtualView::OnMouseMove(nFlags, point);
	for (auto&obj : m_objs) {
		obj->OnMouseMove();
	}
}
void ExportYOLOv2::OnMouseLeave() {
	VirtualView::OnMouseLeave();
}

BOOL ExportYOLOv2::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) {
	VirtualView::OnSetCursor(pWnd, nHitTest, message);
	return TRUE;
}
void ExportYOLOv2::OnSize(UINT nType, int cx, int cy) {
	VirtualView::OnSize(nType, cx, cy);
}

void ExportYOLOv2::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	VirtualView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void ExportYOLOv2::OnTimer(UINT_PTR nIDEvent) {
	VirtualView::OnTimer(nIDEvent);
}
void ExportYOLOv2::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) {
	VirtualView::OnChar(nChar, nRepCnt, nFlags);
}
LRESULT ExportYOLOv2::OnComposition(WPARAM wParam, LPARAM lParam) {
	VirtualView::OnComposition(wParam, lParam);
	return 1;
}

