#include "stdafx.h"
#include "ExportView.h"
#include<opencv2/opencv.hpp>



ExportView::ExportView(CWnd* wnd) : VirtualView(wnd){
	m_list_class = CreateControl<MListBox>(wnd, MRect(MRectPosition::R, 380, 10, 10, 400));
	m_list_class->m_color_bk = GetTheme().ListViewColorBK();
	m_list_class->m_color_text = GetTheme().ListViewColorText();
	m_list_class->is_check = true;

	int base = 10;
	m_stc_title_architecture = CreateControl<MStatic>(wnd, MRect(MRectPosition::LT, base, 10, 100 + 30, 30));
	m_stc_title_architecture->m_color_text = GetTheme().ButtonColorFR();
	m_stc_title_architecture->m_text = TEXT("Architecture");

	m_chk_yolo = CreateControl< MButtonCheck>(wnd, MRect(MRectPosition::LT, base, 70, 25, 25));
	m_chk_yolo->m_color_text = GetTheme().ButtonColorText();
	m_chk_yolo->m_color_fr = GetTheme().ButtonColorFR();
	m_stc_yolo = CreateControl< MStatic>(wnd, MRect(MRectPosition::LT, base+30, 70, 100, 25));
	m_stc_yolo->m_color_text = GetTheme().ButtonColorText();
	m_stc_yolo->m_text = TEXT("YOLO");


	m_chk_fasterrcnn = CreateControl< MButtonCheck>(wnd, MRect(MRectPosition::LT, base, 120, 25, 25));
	m_chk_fasterrcnn->m_color_text = GetTheme().ButtonColorText();
	m_chk_fasterrcnn->m_color_fr = GetTheme().ButtonColorFR();
	m_stc_fasterrcnn = CreateControl< MStatic>(wnd, MRect(MRectPosition::LT, base+30, 120, 100, 25));
	m_stc_fasterrcnn->m_color_text = GetTheme().ButtonColorText();
	m_stc_fasterrcnn->m_text = TEXT("FasterRCNN");

	m_chk_dlibcnn = CreateControl< MButtonCheck>(wnd, MRect(MRectPosition::LT, base, 170, 25, 25));
	m_chk_dlibcnn->m_color_text = GetTheme().ButtonColorText();
	m_chk_dlibcnn->m_color_fr = GetTheme().ButtonColorFR();
	m_stc_dlibcnn = CreateControl< MStatic>(wnd, MRect(MRectPosition::LT, base+30, 170, 100, 25));
	m_stc_dlibcnn->m_color_text = GetTheme().ButtonColorText();
	m_stc_dlibcnn->m_text = TEXT("MMOD-CNN");

	m_chk_dlibsvm = CreateControl< MButtonCheck>(wnd, MRect(MRectPosition::LT, base, 220, 25, 25));
	m_chk_dlibsvm->m_color_text = GetTheme().ButtonColorText();
	m_chk_dlibsvm->m_color_fr = GetTheme().ButtonColorFR();
	m_stc_dlibsvm = CreateControl< MStatic>(wnd, MRect(MRectPosition::LT, base + 30, 220, 100, 25));
	m_stc_dlibsvm->m_color_text = GetTheme().ButtonColorText();
	m_stc_dlibsvm->m_text = TEXT("HOG+SVM");
	///Base Model
	base = 160;
	int text_width = 100;
	m_stc_title_basemodel = CreateControl<MStatic>(wnd, MRect(MRectPosition::LT, base, 10, text_width + 30, 30));
	m_stc_title_basemodel->m_color_text = GetTheme().ButtonColorFR();
	m_stc_title_basemodel->m_text = TEXT("Base model");
	this->AddControl(m_stc_title_basemodel);
	m_str_models.push_back(TEXT("Alexnet"));
	m_str_models.push_back(TEXT("DarknetREF"));
	m_str_models.push_back(TEXT("Darknet19"));
	m_str_models.push_back(TEXT("Darknet53"));
	m_str_models.push_back(TEXT("Densenet201"));
	m_str_models.push_back(TEXT("Resnet50"));
	m_str_models.push_back(TEXT("VGG16"));
	m_str_models.push_back(TEXT("VGG19"));
	for (size_t i = 0; i < m_str_models.size(); i++) {
		MButtonCheck chk= CreateControl<MButtonCheck>(wnd, MRect(MRectPosition::LT, base, 70+i*50, 25, 25));
		chk->m_color_text = GetTheme().ButtonColorText();
		chk->m_color_fr = GetTheme().ButtonColorFR();
		MStatic stc = CreateControl<MStatic>(wnd, MRect(MRectPosition::LT, base + 30, 70+i*50, text_width, 25));
		stc->m_color_text = GetTheme().ButtonColorText();
		stc->m_text = m_str_models[i];
		m_chk_models.push_back(chk);
		m_stc_models.push_back(stc);
		this->AddControl(chk);
		this->AddControl(stc);
	}
	///Options
	base = 310;
	m_stc_title_option = CreateControl<MStatic>(wnd, MRect(MRectPosition::LT, base, 10, text_width + 30, 30));
	m_stc_title_option->m_color_text = GetTheme().ButtonColorFR();
	m_stc_title_option->m_text = TEXT("Options");
	this->AddControl(m_stc_title_option);
	m_str_option.push_back(TEXT("3 anchors"));
	m_str_option.push_back(TEXT("5 anchors"));
	m_str_option.push_back(TEXT("7 anchors"));
	m_str_option.push_back(TEXT("9 anchors"));
	m_str_option.push_back(TEXT("11 anchors"));
	m_str_option.push_back(TEXT("13 anchors"));
	m_str_option.push_back(TEXT("15 anchors"));
	m_str_option.push_back(TEXT("17 anchors"));
	for (size_t i = 0; i < m_str_option.size(); i++) {
		MButtonCheck chk = CreateControl<MButtonCheck>(wnd, MRect(MRectPosition::LT, base, 70 + i * 50, 25, 25));
		chk->m_color_text = GetTheme().ButtonColorText();
		chk->m_color_fr = GetTheme().ButtonColorFR();
		MStatic stc = CreateControl<MStatic>(wnd, MRect(MRectPosition::LT, base + 30, 70 + i * 50, text_width, 25));
		stc->m_color_text = GetTheme().ButtonColorText();
		stc->m_text = m_str_option[i];
		m_chk_option.push_back(chk);
		m_stc_option.push_back(stc);
		this->AddControl(chk);
		this->AddControl(stc);
	}
	///Size
	base = 460;
	m_stc_title_size = CreateControl<MStatic>(wnd, MRect(MRectPosition::LT, base, 10, text_width + 30, 30));
	m_stc_title_size->m_color_text = GetTheme().ButtonColorFR();
	m_stc_title_size->m_text = TEXT("Size");
	this->AddControl(m_stc_title_size);
	m_str_size.push_back(TEXT("352x352"));
	m_str_size.push_back(TEXT("416x416"));
	m_str_size.push_back(TEXT("480x480"));
	m_str_size.push_back(TEXT("544x544"));
	m_str_size.push_back(TEXT("608x608"));
	m_str_size.push_back(TEXT("850x850"));
	m_str_size.push_back(TEXT("1088x1088"));
	m_str_size.push_back(TEXT("Random"));
	for (size_t i = 0; i < m_str_size.size(); i++) {
		MButtonCheck chk = CreateControl<MButtonCheck>(wnd, MRect(MRectPosition::LT, base, 70 + i * 50, 25, 25));
		chk->m_color_text = GetTheme().ButtonColorText();
		chk->m_color_fr = GetTheme().ButtonColorFR();
		MStatic stc = CreateControl<MStatic>(wnd, MRect(MRectPosition::LT, base + 30, 70 + i * 50, text_width, 25));
		stc->m_color_text = GetTheme().ButtonColorText();
		stc->m_text = m_str_size[i];
		m_chk_size.push_back(chk);
		m_stc_size.push_back(stc);
		this->AddControl(chk);
		this->AddControl(stc);
	}
	///Noise
	base = 610;
	text_width = 200;
	m_stc_title_noise = CreateControl<MStatic>(wnd, MRect(MRectPosition::LT, base, 10, text_width + 30, 30));
	m_stc_title_noise->m_color_text = GetTheme().ButtonColorFR();
	m_stc_title_noise->m_text = TEXT("Noise");
	this->AddControl(m_stc_title_noise);
	m_str_noise.push_back(TEXT("Gray"));
	m_str_noise.push_back(TEXT("Salt&Pepper"));
	m_str_noise.push_back(TEXT("Flip(LR)"));
	m_str_noise.push_back(TEXT("Flip(TB)"));
	m_str_noise.push_back(TEXT("Slip"));
	m_str_noise.push_back(TEXT("Blur"));
	m_str_noise.push_back(TEXT("Gamma Correction(0.5)"));
	m_str_noise.push_back(TEXT("Gamma Correction(1.5)"));
	for (size_t i = 0; i < m_str_noise.size(); i++) {
		MButtonCheck chk = CreateControl<MButtonCheck>(wnd, MRect(MRectPosition::LT, base, 70 + i * 50, 25, 25));
		chk->m_color_text = GetTheme().ButtonColorText();
		chk->m_color_fr = GetTheme().ButtonColorFR();
		MStatic stc = CreateControl<MStatic>(wnd, MRect(MRectPosition::LT, base + 30, 70 + i * 50, text_width, 25));
		stc->m_color_text = GetTheme().ButtonColorText();
		stc->m_text = m_str_noise[i];
		m_chk_noise.push_back(chk);
		m_stc_noise.push_back(stc);
		this->AddControl(chk);
		this->AddControl(stc);
	}
	///////////////////////////////////////////////////////////////////////////////////
	m_chk_rotate = CreateControl< MButtonCheck>(wnd, MRect(MRectPosition::RB, 380-25, 370, 25, 25));
	m_chk_rotate->m_color_text = GetTheme().ButtonColorText();
	m_chk_rotate->m_color_fr = GetTheme().ButtonColorFR();
	m_stc_rotate = CreateControl< MStatic>(wnd, MRect(MRectPosition::RB, 380-25-200-10, 370, 200, 25));
	m_stc_rotate->m_color_text = GetTheme().ButtonColorText();
	m_stc_rotate->m_text = TEXT("Enable Rotate");

	m_chk_boundingbox = CreateControl< MButtonCheck>(wnd, MRect(MRectPosition::RB, 380 - 25, 340, 25, 25));
	m_chk_boundingbox->m_color_text = GetTheme().ButtonColorText();
	m_chk_boundingbox->m_color_fr = GetTheme().ButtonColorFR();
	m_stc_boundingbox = CreateControl< MStatic>(wnd, MRect(MRectPosition::RB, 380 - 25 - 200 - 10, 340, 200, 25));
	m_stc_boundingbox->m_color_text = GetTheme().ButtonColorText();
	m_stc_boundingbox->m_text = TEXT("BoundingBox");
	m_chk_ignore_rbox = CreateControl< MButtonCheck>(wnd, MRect(MRectPosition::RB, 380 - 25, 310, 25, 25));
	m_chk_ignore_rbox->m_color_text = GetTheme().ButtonColorText();
	m_chk_ignore_rbox->m_color_fr = GetTheme().ButtonColorFR();
	m_stc_ignore_rbox = CreateControl< MStatic>(wnd, MRect(MRectPosition::RB, 380 - 25 - 200 - 10, 310, 200, 25));
	m_stc_ignore_rbox->m_color_text = GetTheme().ButtonColorText();
	m_stc_ignore_rbox->m_text = TEXT("Ignore RotatedRect");

	m_chk_debug_info = CreateControl< MButtonCheck>(wnd, MRect(MRectPosition::RB, 380 - 25, 280, 25, 25));
	m_chk_debug_info->m_color_text = GetTheme().ButtonColorText();
	m_chk_debug_info->m_color_fr = GetTheme().ButtonColorFR();
	m_stc_debug_info = CreateControl< MStatic>(wnd, MRect(MRectPosition::RB, 380 - 25 - 200 - 10, 280, 200, 25));
	m_stc_debug_info->m_color_text = GetTheme().ButtonColorText();
	m_stc_debug_info->m_text = TEXT("Generate debug info");

	m_stc_name= CreateControl< MStatic>(wnd, MRect(MRectPosition::RB, 310, 235, 80, 30));
	m_stc_name->m_color_text = GetTheme().ButtonColorText();
	m_stc_name->m_text = TEXT("Name : ");

	m_edit_name = CreateControl<MSingleEdit>(wnd, MRect(MRectPosition::RB, 10, 235, 300, 30));
	m_edit_name->m_color_text = GetTheme().ButtonColorText();
	m_edit_name->m_color_fr = GetTheme().ButtonColorFR();
	
	m_btn_export = CreateControl< MButton>(wnd, MRect(MRectPosition::RB, 10, 30, 380, 30));
	m_btn_export->m_text = TEXT("Export data");
	m_btn_export->m_color_fr = GetTheme().ButtonColorFR();
	m_btn_export->m_color_text = GetTheme().ButtonColorText();

	for (int i = 1; 25 * i<225; i++) {
		MStatic doc = CreateControl<MStatic>(wnd, MRect(MRectPosition::B, 10,25, 400, 25*i));
		doc->m_color_text = GetTheme().ButtonColorText();
		doc->m_text = TEXT("");
		m_doc.push_front(doc);
		this->AddControl(doc);
	}
	this->AddControl(m_stc_title_architecture);
	this->AddControl(m_chk_yolo);
	this->AddControl(m_stc_yolo);
	this->AddControl(m_chk_fasterrcnn);
	this->AddControl(m_stc_fasterrcnn);
	this->AddControl(m_chk_dlibcnn);
	this->AddControl(m_stc_dlibcnn);
	this->AddControl(m_chk_dlibsvm);
	this->AddControl(m_stc_dlibsvm);

	this->AddControl(m_list_class);
	this->AddControl(m_stc_debug_info);
	this->AddControl(m_chk_debug_info);
	this->AddControl(m_edit_name);
	this->AddControl(m_btn_export);
	this->AddControl(m_stc_name);

	this->AddControl(m_chk_rotate);
	this->AddControl(m_stc_rotate);
	this->AddControl(m_chk_boundingbox);
	this->AddControl(m_stc_boundingbox);
	this->AddControl(m_chk_ignore_rbox);
	this->AddControl(m_stc_ignore_rbox);
	
	m_chk_yolo->check = true;
	m_chk_ignore_rbox->check = true;

	GetApp().g_is_rotate = &m_chk_rotate->check;
	GetApp().g_is_ignore = &m_chk_ignore_rbox->check;
	GetApp().g_export_class_data = &m_list_class->m_data;
	GetApp().g_is_dbg_info = &m_chk_debug_info->check;
	///DEV
	m_chk_dlibcnn->disable = true;
	m_chk_dlibsvm->disable = true;

	m_chk_fasterrcnn->check = false;
	m_chk_dlibcnn->check = false;
	m_chk_dlibsvm->check = false;
	m_chk_yolo->check = true;
	for (auto&e : m_chk_models) e->check = false;
	for (auto&e : m_chk_option) e->check = false;
	for (auto&e : m_chk_size) e->check = false;
	GetChkBtnByString(m_chk_models, m_stc_models, TEXT("Darknet53"))->check = true;
	GetChkBtnByString(m_chk_option, m_stc_option, TEXT("7 anchors"))->check = true;
	GetChkBtnByString(m_chk_size, m_stc_size, TEXT("416x416"))->check = true;
	WriteDocument();
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
	if (this->isSameClassList() == false) {
		m_list_class->m_data.assign(GetApp().g_class_data->begin(), GetApp().g_class_data->end());
		for (auto&e : m_list_class->m_data) {
			e.second = true;
		}
	}
	m_list_class->m_select = -1;
	
	for (auto&e : m_chk_models) {e->disable = false;}
	if (m_chk_yolo->check == true) {
		GetChkBtnByString(m_chk_models, m_stc_models, TEXT("Alexnet"))->disable = true;
		GetChkBtnByString(m_chk_models, m_stc_models, TEXT("VGG16"))->disable = true;
		GetChkBtnByString(m_chk_models, m_stc_models, TEXT("VGG19"))->disable = true;
		for (auto&e : m_chk_option) {
			e->disable = false;
		}
		for (auto&e : m_chk_size) {
			e->disable = false;
		}
		GetChkBtnByString(m_chk_size, m_stc_size, TEXT("850x850"))->disable = true;
	} else if (m_chk_fasterrcnn->check == true) {
		GetChkBtnByString(m_chk_models, m_stc_models, TEXT("DarknetREF"))->disable = true;
		GetChkBtnByString(m_chk_models, m_stc_models, TEXT("Darknet19"))->disable = true;
		GetChkBtnByString(m_chk_models, m_stc_models, TEXT("Darknet53"))->disable = true;
		GetChkBtnByString(m_chk_models, m_stc_models, TEXT("Densenet201"))->disable = true;
		GetChkBtnByString(m_chk_models, m_stc_models, TEXT("Resnet50"))->disable = true;
		for (auto&e : m_chk_option) {
			e->disable = true;
		}
		for (auto&e : m_chk_size) {
			e->disable = true;
		}
		GetChkBtnByString(m_chk_size, m_stc_size, TEXT("850x850"))->disable = false;
		GetChkBtnByString(m_chk_size, m_stc_size, TEXT("850x850"))->check = true;	//선택옵션이 이거밖에 없음
	}

	m_chk_boundingbox->disable = m_chk_rotate->check;
	m_chk_ignore_rbox->disable = m_chk_rotate->check;

	
	MCALL;
	WriteDocument();
	if (GetApp().g_exporting == true) {
		ShowProgressBar(pDC, this->GetViewRect());
	}
}
void ExportView::OnSetFocus(CWnd* pOldWnd) {
	VirtualView::OnSetFocus(pOldWnd);
}
void ExportView::OnKillFocus(CWnd* pNewWnd) {
	VirtualView::OnKillFocus(pNewWnd);
}

void ExportView::OnLButtonDown(UINT nFlags, CPoint point) {
	if (GetApp().g_exporting == true)return;
	MCALL;
	if (m_chk_boundingbox->isChanged()) {
		m_chk_boundingbox->check = true;
		m_chk_ignore_rbox->check = false;
	} else if (m_chk_ignore_rbox->isChanged()) {
		m_chk_boundingbox->check = false;
		m_chk_ignore_rbox->check = true;
	}
	if (m_chk_yolo->isChanged()) {
		m_chk_fasterrcnn->check = false;
		m_chk_dlibcnn->check = false;
		m_chk_yolo->check = true;
		for (auto&e : m_chk_models) e->check = false;
		for (auto&e : m_chk_option) e->check = false;
		for (auto&e : m_chk_size) e->check = false;
		GetChkBtnByString(m_chk_models, m_stc_models, TEXT("Darknet53"))->check = true;
		GetChkBtnByString(m_chk_option, m_stc_option, TEXT("7 anchors"))->check = true;
		GetChkBtnByString(m_chk_size, m_stc_size, TEXT("416x416"))->check = true;
	}
	if (m_chk_fasterrcnn->isChanged()) {
		m_chk_fasterrcnn->check = true;
		m_chk_yolo->check = false;
		m_chk_dlibcnn->check = false;
		for (auto&e : m_chk_models) e->check = false;
		for (auto&e : m_chk_option) e->check = false;
		for (auto&e : m_chk_size) e->check = false;
		GetChkBtnByString(m_chk_models, m_stc_models, TEXT("VGG16"))->check = true;
		GetChkBtnByString(m_chk_size, m_stc_size, TEXT("850x850"))->check = true;
	}
	if (m_chk_dlibcnn->isChanged()) {
		m_chk_dlibcnn->check = true;
		m_chk_yolo->check = false;
		m_chk_fasterrcnn->check = false;
		for (auto&e : m_chk_models) e->check = false;
		for (auto&e : m_chk_option) e->check = false;
		for (auto&e : m_chk_size) e->check = false;
	}
	for (auto&e : m_chk_option) {
		if (e->isChanged()) {
			for (auto&e : m_chk_option) e->check = false;
			e->check = true;
		}
	}
	for (auto&e : m_chk_models) {
		if (e->isChanged()) {
			for (auto&e : m_chk_models) e->check = false;
			e->check = true;
		}
	}
	for (auto&e : m_chk_size) {
		if (e->isChanged()) {
			for (auto&e : m_chk_size) e->check = false;
			e->check = true;
		}
	}
	if (m_btn_export->isClicked()) {
		if (m_edit_name->m_text.length() == 0) {
			TSpringMsgBox msgbox(TEXT("Please, Write project name before export"));
			msgbox.DoModal();
			return;
		}
		int c = 0;
		for (auto&e : m_list_class->m_data) {
			if (e.second == true)c++;
		}
		if (c == 0) {
			TSpringMsgBox msgbox(TEXT("You need select at least one class"));
			msgbox.DoModal();
			return;
		}
		int nOfAnchors = GetNumOfAnchors();
		if (m_chk_fasterrcnn->check == true) {
			nOfAnchors = 1;
		}
		if (IsMoreThanNTag(nOfAnchors) == false) {
			TSpringMsgBox msgbox(TEXT("Too less tag information."));
			msgbox.DoModal();
			return;
		}
		CFolderPickerDialog fpd(nullptr, OFN_PATHMUSTEXIST);
		if (IDOK == fpd.DoModal()) {
			LPCTSTR _path = (LPCTSTR)fpd.GetPathName();
			TString path = _path;
			TString type = TEXT("TSpring()");
			if (m_chk_yolo->check == true) {
				type = TEXT("TSpring(YOLO)");
			} else if (m_chk_fasterrcnn->check == true) {
				type = TEXT("TSpring(FasterRCNN)");
			} else if (m_chk_dlibcnn->check == true) {
				type = TEXT("TSpring(dlib-cnn)");
			}
			path += TEXT('\\') + type + m_edit_name->m_text + TEXT('\\');
			GetApp().g_project_name= m_edit_name->m_text;
			GetApp().g_export_dir = path;
			ispring::File::DirectoryMake(std::string(path.begin(),path.end()));
			if (m_chk_yolo->check == true) {
				Export_YOLO();
			} else if (m_chk_fasterrcnn->check == true) {
				Export_FasterRCNN();
			} else if (m_chk_dlibcnn->check == true) {
				//
			}
		}
	}
	CPoint rpoint = this->GetMousePoint();
	int sel=m_list_class->GetElementByPoint(rpoint);
	if (sel != -1) {
		m_list_class->m_data[sel].second = !m_list_class->m_data[sel].second;
	}
	
}
void ExportView::OnLButtonUp(UINT nFlags, CPoint point) {
	MCALL;
}
void ExportView::OnLButtonDblClk(UINT nFlags, CPoint point) {
	if (GetApp().g_exporting == true)return;
	MCALL;
}
void ExportView::OnRButtonUp(UINT nFlags, CPoint point) {
	if (GetApp().g_exporting == true)return;
	MCALL;
}
BOOL ExportView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) {
	if (GetApp().g_exporting == true)return FALSE;
	MCALL;
	return TRUE;
}
void ExportView::OnMouseMove(UINT nFlags, CPoint point) {
	if (GetApp().g_exporting == true)return;
	MCALL;
}
void ExportView::OnMouseLeave() {
	if (GetApp().g_exporting == true)return;
	MCALL;
}

BOOL ExportView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) {
	if (GetApp().g_exporting == true)return FALSE;
	VirtualView::OnSetCursor(pWnd, nHitTest, message);
	return TRUE;
}
void ExportView::OnSize(UINT nType, int cx, int cy) {
	VirtualView::OnSize(nType, cx, cy);
}

void ExportView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if (GetApp().g_exporting == true)return;
	MCALL;
}

void ExportView::OnTimer(UINT_PTR nIDEvent) {
	if (GetApp().g_exporting == true)return;
	MCALL;
}
void ExportView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if (GetApp().g_exporting == true)return;
	if (nChar != VK_RETURN) {
		if (!(std::isspace(nChar) == false && nChar!='\\' && nChar!='/' && nChar!=':' && nChar!='?' && nChar!='\"' && nChar!='<' && nChar!='>' && nChar!='|')) {
			return;
		}
	}
	MCALL;
}
LRESULT ExportView::OnComposition(WPARAM wParam, LPARAM lParam) {
	if (GetApp().g_exporting == true)return FALSE;
	MCALL;
	return 1;
}


///===============================Exporter=====================================
int ExportView::GetExportClass(int c) {
	if (c == -100) {
		return -100;
	}
	int classes = 0;
	for (int i = 0; i < (int)GetApp().g_export_class_data->size();i++) {
		if (GetApp().g_export_class_data->at(i).second == true) {
			if (i == c) {
				return classes;
			}
			classes++;
		}
	}
	return -1;
}

void ExportView::GenExportImage(std::string filename,std::fstream& fout_train_txt, ImageStatistics& stat, std::ostream& fout_roi_txt) {
	cv::Mat img = cv::imread(filename);
	int SIZE = GetExportSize();
	std::vector<TagInfo> taginfo = this->ReadTagInfo(filename);
	std::vector<int> real_tag;
	for (auto&e : taginfo) {
		real_tag.push_back(GetExportClass(e.m_class));
	}
	for (auto&e : taginfo) {	//ignore box 칠하기
		if (e.m_class == -100) cv::rectangle(img, e.m_rect.boundingRect(), cv::Scalar(0, 0, 0), CV_FILLED);
	}
	if (m_chk_rotate->check == true) {
		std::vector<int> appear(stat.nOfImages.size(), 0);
		for (size_t j = 0; j < 180; j++) {
			std::vector<TagInfo> rtag;
			std::vector<int> r_real_tag;
			for (size_t k = 0; k < taginfo.size(); k++) {
				if (taginfo[k].m_class != -100 && real_tag[k]!=-1) {
					if (j == static_cast<int>(360 + taginfo[k].m_rect.angle) % 180) {
						rtag.push_back(taginfo[k]);
						r_real_tag.push_back(real_tag[k]);
					}
				}
			}
			if (rtag.empty() == true)continue;
			std::ostringstream oss;
			oss << "(angle_" << ispring::String::PadNum(j,3) << ")";
			cv::Mat rimg;
			cv::Point out;
			rimg = ispring::CV::ImageRotateOuter(img, static_cast<double>(j), &out);
			cv::Point mid(img.cols / 2, img.rows / 2);
			for (size_t k = 0; k < rtag.size(); k++) {
				cv::Point2f pts[4];
				std::vector<cv::Point> ptsi(4);
				rtag[k].m_rect.points(pts);
				for (size_t t = 0; t < 4; t++) {
					cv::Point2d after;
					double rad = -static_cast<double>(j)*CV_PI / 180.0;
					after.x = (pts[t].x - mid.x)*cos(rad) - (pts[t].y - mid.y)*sin(rad) + mid.x;
					after.y = (pts[t].x - mid.x)*sin(rad) + (pts[t].y - mid.y)*cos(rad) + mid.y;
					after.x += out.x;
					after.y += out.y;
					ptsi[t].x = static_cast<int>(after.x);
					ptsi[t].y = static_cast<int>(after.y);
				}
				cv::Rect rect = cv::boundingRect(ptsi);
				if (m_chk_debug_info->check == true) {
					SaveCropImage(filename, rimg(rect), r_real_tag[k]);
				}
				stat.nOfSamples[r_real_tag[k]]++;
				appear[r_real_tag[k]] = 1;
				rtag[k].m_rect.angle = 0;
				rtag[k].m_rect.center.x = rect.x + rect.width / 2.0F;
				rtag[k].m_rect.center.y = rect.y + rect.height / 2.0F;
				rtag[k].m_rect.size.width = static_cast<float>(rect.width);
				rtag[k].m_rect.size.height = static_cast<float>(rect.height);
			}
			if (m_chk_yolo->check == true) {
				std::vector<YOLOBOX> yolobox = GetYOLOBOX(rimg, rtag);
				std::string file_noext = GetOriginalName(filename, "train\\", oss.str() + ".jpg");
				std::string file_img = file_noext + oss.str() + ".jpg";
				std::string file_txt = file_noext + oss.str() + ".txt";
				cv::resize(rimg, rimg, cv::Size(SIZE, SIZE), cv::INTER_LANCZOS4);
				cv::imwrite("train\\" + file_img, rimg);
				std::fstream fout;
				fout.open("train\\" + file_txt, std::ios::out);
				if (fout.is_open() == false) {
					ISPRING_VERIFY("file open fail");
				} else {
					for (size_t k = 0; k < yolobox.size(); k++) {
						if (k != 0) {
							fout << std::endl;
						}
						fout << yolobox[k].m_class << " " << yolobox[k].x << " " << yolobox[k].y << " " << yolobox[k].w << " " << yolobox[k].h;
					}
					fout.close();
				}
				fout_train_txt << "train/" << file_img << std::endl;
				stat.nOfTotal++;
			} else if (m_chk_fasterrcnn->check == true) {
				std::vector<RCNNBOX> rcnnbox = GetRCNNBOX(rimg, rtag, SIZE);
				if (rcnnbox.size() == 0)return;
				std::string file_noext = GetOriginalName(filename, "train\\", ".jpg");
				std::string file_img = file_noext + ".jpg";
				cv::resize(rimg, rimg, cv::Size(SIZE, SIZE), cv::INTER_LANCZOS4);
				cv::imwrite("train\\" + file_img, rimg);
				fout_train_txt << this->rcnn_cnt << "\ttrain/" << file_img << "\t" << 0 << std::endl;
				fout_roi_txt << this->rcnn_cnt << " |roiAndLabel";
				for (auto&box : rcnnbox) {
					std::ostringstream oss;
					fout_roi_txt << " " << box.x1 << " " << box.y1 << " " << box.x2 << " " << box.y2 << " " << box.m_class << ".0";
				}
				fout_roi_txt << std::endl;
				this->rcnn_cnt++;
			}
		}
		for (size_t i = 0; i < appear.size(); i++) {
			stat.nOfImages[i] += appear[i];
		}
	} else {
		std::vector<int> appear(stat.nOfImages.size(), 0);
		for (size_t j = 0; j < taginfo.size(); j++) {
			if (taginfo[j].m_class != -100 && real_tag[j] != -1) {
				if (m_chk_debug_info->check == true) {
					if (m_chk_boundingbox->check == true || taginfo[j].m_rect.angle == 0) {
						cv::Rect rect = taginfo[j].m_rect.boundingRect();
						rect = GetSafeRect(rect, img.size());
						SaveCropImage(filename, img(rect), real_tag[j]);
					}
				}
				appear[real_tag[j]] = 1;
				stat.nOfSamples[real_tag[j]]++;
			}
		}
		for (size_t i = 0; i < appear.size(); i++) {
			stat.nOfImages[i] += appear[i];
		}
		
		if (m_chk_yolo->check == true) {
			std::vector<YOLOBOX> yolobox = GetYOLOBOX(img, taginfo);
			if (yolobox.size() == 0)return;
			std::string file_noext = GetOriginalName(filename, "train\\", ".jpg");
			std::string file_img = file_noext + ".jpg";
			std::string file_txt = file_noext + ".txt";
			cv::resize(img, img, cv::Size(SIZE, SIZE), cv::INTER_LANCZOS4);
			cv::imwrite("train\\" + file_img, img);
			std::fstream fout;
			fout.open("train\\" + file_txt, std::ios::out);
			if (fout.is_open() == false) {
				ISPRING_VERIFY("file open fail");
			} else {
				for (size_t j = 0; j < yolobox.size(); j++) {
					if (j != 0) {
						fout << std::endl;
					}
					fout << yolobox[j].m_class << " " << yolobox[j].x << " " << yolobox[j].y << " " << yolobox[j].w << " " << yolobox[j].h;
				}
				fout.close();
			}
			fout_train_txt << "train/" << file_img << std::endl;
			stat.nOfTotal++;
		} else if (m_chk_fasterrcnn->check == true) {
			std::vector<RCNNBOX> rcnnbox = GetRCNNBOX(img, taginfo, SIZE);
			if (rcnnbox.size() == 0)return;
			std::string file_noext = GetOriginalName(filename, "train\\", ".jpg");
			std::string file_img = file_noext + ".jpg";
			cv::resize(img, img, cv::Size(SIZE, SIZE), cv::INTER_LANCZOS4);
			cv::imwrite("train\\" + file_img, img);
			fout_train_txt << this->rcnn_cnt << "\ttrain/" << file_img << "\t" << 0 << std::endl;
			fout_roi_txt << this->rcnn_cnt << " |roiAndLabel";
			for (auto&box : rcnnbox) {
				std::ostringstream oss;
				fout_roi_txt << " " << box.x1 << " " << box.y1 << " " << box.x2 << " " << box.y2 << " " << box.m_class << ".0";
			}
			fout_roi_txt << std::endl;
			this->rcnn_cnt++;
		}
	}
}
UINT Export_YOLO_T(void* param) {
	char CURR[MAX_PATH] = { 0 };
	GetCurrentDirectoryA(MAX_PATH, CURR);
	
	ExportView* _this = reinterpret_cast<ExportView*>(param);
	TString curr_dir = GetApp().g_export_dir;	//슬래쉬 붙어있음
	_chdir(std::string(curr_dir.begin(),curr_dir.end()).c_str());
	std::vector<std::pair<TString, bool>>& images = *GetApp().g_image_data;

	std::fstream fout_train_txt;
	fout_train_txt.open("train.txt", std::ios::out);
	ispring::File::DirectoryMake("train");
	ispring::File::DirectoryMake("bin");
	ispring::File::DirectoryMake("backup");
	if (_this->m_chk_debug_info->check == true) {
		ispring::File::DirectoryMake("Debug");
		ispring::File::DirectoryMake("Debug\\crop");
	}
	GetApp().g_export_str = TEXT("Download modules");
	GetApp().g_progress_total = 2;
	GetApp().g_progress_current = 0;
	ispring::Web::Download("https://www.dropbox.com/s/mejmd1fd3fqhc2z/YOLOv3SE_Train.exe?dl=1", "bin/YOLOv3SE_Train.exe.exe");
	GetApp().g_progress_current = 1;
	ispring::Web::Download("https://www.dropbox.com/s/kc9sd1irv95v53w/cudnn64_5.dll?dl=1", "bin/cudnn64_5.dll");
	GetApp().g_progress_current = 2;

	GetApp().g_export_str = TEXT("Generate images");
	GetApp().g_progress_total = images.size();
	GetApp().g_progress_current = 0;
	ExportView::ImageStatistics stat;
	stat.nOfTotal = 0;
	int nOfClasses = 0;
	bool is_yolo3 = _this->GetChkBtnByString(_this->m_chk_models, _this->m_stc_models, TEXT("Darknet53"))->check;
#if 1
	for (auto&e : _this->m_list_class->m_data) {
		if (e.second == true)nOfClasses++;
	}
	stat.nOfImages.assign(nOfClasses, 0);
	stat.nOfSamples.assign(nOfClasses, 0);
	for (size_t i = 0; i < images.size(); i++) {
		auto& file = images[i].first;
		_this->GenExportImage(std::string(file.begin(), file.end()), fout_train_txt,stat);

		GetApp().g_progress_current = i + 1;
	}
	///태깅정보 생성
	if (_this->m_chk_debug_info->check == true) {
		_this->GenTagInfos(stat);
	}
#endif
	///Anchor boxes 생성
	int nOfAnchors = _this->GetNumOfAnchors();
	int resolution = _this->GetResolution();
	std::string anchors;

	std::vector<std::string> images_str = ispring::File::FileList("train/", "*.jpg");
	anchors = GetAnchors(nOfAnchors, resolution, images_str,_this->m_chk_debug_info->check, is_yolo3);
	
	///CFG 생성
	int BATCH = 64;
	int SUBDIVISION = 64;
	int WIDTH = resolution;
	int HEIGHT = resolution;
	int MAX_BATCHES = 60000;
	int CLASSES = nOfClasses;
	int NUM = nOfAnchors;
	std::vector<int> FILTERS;
	std::vector<std::string> MASKS;
	if (is_yolo3) {
		int MS1 = (int)round(NUM / 3.0);
		int MS2 = MS1;
		int MS3 = NUM - (MS1 + MS2);
		FILTERS.push_back((CLASSES + 5)*MS1);
		FILTERS.push_back((CLASSES + 5)*MS2);
		FILTERS.push_back((CLASSES + 5)*MS3);
		int m = nOfAnchors - 1;
		std::string str;
		while (MS1--) {
			str += std::to_string(m--) + ",";
		}
		str.pop_back();
		MASKS.push_back(str);
		str = "";
		while (MS2--) {
			str += std::to_string(m--) + ",";
		}
		str.pop_back();
		MASKS.push_back(str);
		str = "";
		while (MS3--) {
			str += std::to_string(m--) + ",";
		}
		str.pop_back();
		MASKS.push_back(str);
	} else {
		FILTERS.push_back((CLASSES + 5)*NUM);
	}
	int RANDOM = _this->GetChkBtnByString(_this->m_chk_size, _this->m_stc_size, TEXT("Random"))->check ? 1 : 0;
	auto cfg = _this->GetCFG();
	char* cfg_format = cfg.first;
	std::string cfg_name = cfg.second;
	static char buffer[65536];
	if (is_yolo3) {
		sprintf_s(buffer, 65536, cfg_format, BATCH, SUBDIVISION, WIDTH, HEIGHT, MAX_BATCHES
				  , FILTERS[0], MASKS[0], anchors.c_str(), CLASSES, NUM, RANDOM
				  , FILTERS[1], MASKS[1], anchors.c_str(), CLASSES, NUM, RANDOM
				  , FILTERS[2], MASKS[2], anchors.c_str(), CLASSES, NUM, RANDOM);
	} else {
		sprintf_s(buffer, 65536, cfg_format, BATCH, SUBDIVISION, HEIGHT, WIDTH, MAX_BATCHES, FILTERS[0], anchors.c_str(), CLASSES, NUM, RANDOM);
	}
	std::fstream fout;
	///CFG
	fout.open(cfg_name + ".cfg", std::ios::out);
	fout << buffer;
	fout.close();
	///DATA
	std::string prj_name = mspring::String::ToString(GetApp().g_project_name);
	fout.open(prj_name + ".data", std::ios::out);
	fout << "classes=" << CLASSES << std::endl;
	fout << "train=train.txt" << std::endl;
	fout << "names=" << prj_name << ".names" << std::endl;
	fout << "backup=backup/";
	fout.close();
	///NAMES
	fout.open( prj_name + ".names", std::ios::out);
	for (auto&e : *GetApp().g_export_class_data) {
		if (e.second == true) {
			fout << mspring::String::ToString(e.first) << std::endl;
		}
	}
	fout.close();
	///BAT
	fout.open("train-" + cfg_name + ".bat", std::ios::out);
	fout << "cd \"bin\"" << std::endl;
	fout << "\"YOLOv3SE_Train.exe\" ../ " << prj_name + ".data " << cfg_name + ".cfg" << std::endl;
	fout << "cd .." << std::endl;
	fout << "pause";
	fout.close();
	///Noise 생성
	using Func=std::string(*)(std::string,DAType);
	std::vector<Func> func;
	func.push_back(Generate_Gray);
	func.push_back(Generate_SaltAndPepper);
	func.push_back(Generate_FilpLR);
	func.push_back(Generate_FilpTB);
	func.push_back(Generate_Slip);
	func.push_back(Generate_Blur);
	for (size_t i = 0; i < _this->m_str_noise.size()-2; i++) {
		TString da_tname = _this->m_str_noise[i];
		if (_this->GetChkBtnByString(_this->m_chk_noise, _this->m_stc_noise, da_tname)->check == true) {
			std::vector<std::string> files = ispring::File::FileList("train/", "*.jpg");
			GetApp().g_export_str = TEXT("Generate ")+ da_tname +TEXT(" images");
			GetApp().g_progress_current = 0;
			GetApp().g_progress_total = static_cast<int>(files.size());
			int threads = mspring::Max(omp_get_num_threads() / 2, 1); 
#pragma omp parallel for schedule(guided) num_threads(threads)
			for (int j = 0; (int)j < files.size(); j++) {
				fout_train_txt << func[i](files[j],DAType::YOLO) << std::endl;
#pragma omp critical(g_progress_current)
				GetApp().g_progress_current++;
			}
		}
	}
	{
		if (_this->GetChkBtnByString(_this->m_chk_noise, _this->m_stc_noise, _this->m_str_noise[_this->m_str_noise.size() - 1])->check == true
			|| _this->GetChkBtnByString(_this->m_chk_noise, _this->m_stc_noise, _this->m_str_noise[_this->m_str_noise.size() - 2])->check == true) {
			std::vector<std::string> files = ispring::File::FileList("train/", "*.jpg");
			GetApp().g_export_str = TEXT("Generate Gamma-Correction images");
			GetApp().g_progress_current = 0;
			GetApp().g_progress_total = static_cast<int>(files.size());
			int threads = mspring::Max(omp_get_num_threads() / 2, 1);
#pragma omp parallel for schedule(guided) num_threads(threads)
			for (int j = 0; (int)j < files.size(); j++) {
				if (_this->GetChkBtnByString(_this->m_chk_noise, _this->m_stc_noise, _this->m_str_noise[_this->m_str_noise.size() - 1])->check == true) {
					fout_train_txt << Generate_GammaCorrection1(files[j]) << std::endl;
				}
				if (_this->GetChkBtnByString(_this->m_chk_noise, _this->m_stc_noise, _this->m_str_noise[_this->m_str_noise.size() - 2])->check == true) {
					fout_train_txt << Generate_GammaCorrection2(files[j]) << std::endl;
				}
#pragma omp critical(g_progress_current)
				GetApp().g_progress_current++;
			}
		}
	}
	fout_train_txt.close();
	_chdir(CURR);
	GetApp().g_exporting = false;
	return 1;
}
void ExportView::Export_YOLO() {
	//Export_YOLO_T(this);
	GetApp().g_exporting = true;
	AfxGetMainWnd()->SetTimer(7777, 20, nullptr);
	AfxBeginThread(Export_YOLO_T, this);
}

UINT Export_FasterRCNN_T(void* param) {
	char CURR[MAX_PATH] = { 0 };
	GetCurrentDirectoryA(MAX_PATH, CURR);
	ExportView* _this = reinterpret_cast<ExportView*>(param);
	_this->rcnn_cnt = 0;
	TString curr_dir = GetApp().g_export_dir;	//슬래쉬 붙어있음
	_chdir(std::string(curr_dir.begin(), curr_dir.end()).c_str());
	std::vector<std::pair<TString, bool>>& images = *GetApp().g_image_data;
	ispring::File::DirectoryMake("bin");
	ispring::File::DirectoryMake("train");
	if (_this->m_chk_debug_info->check == true) {
		ispring::File::DirectoryMake("Debug");
		ispring::File::DirectoryMake("Debug\\crop");
	}
	GetApp().g_export_str = TEXT("Download modules");
	GetApp().g_progress_total = 23 + 1;
	GetApp().g_progress_current = 0;
	///bin 받기
	///pretrained 받기
	std::vector<std::string> urls, dsts;
	urls.push_back("https://www.dropbox.com/s/0rpr7j150vyqd4y/Cntk.Core-2.2.dll?dl=1"); dsts.push_back("bin/Cntk.Core-2.2.dll");
	urls.push_back("https://www.dropbox.com/s/ax7rt9x0tisv1ef/Cntk.Math-2.2.dll?dl=1"); dsts.push_back("bin/Cntk.Math-2.2.dll");
	urls.push_back("https://www.dropbox.com/s/oedrywx39qmyxcv/Cntk.PerformanceProfiler-2.2.dll?dl=1"); dsts.push_back("bin/Cntk.PerformanceProfiler-2.2.dll");
	urls.push_back("https://www.dropbox.com/s/bz2zxgz9x5rxvt1/concrt140.dll?dl=1"); dsts.push_back("bin/concrt140.dll");
	urls.push_back("https://www.dropbox.com/s/mqib7ytl6mxlwij/cublas64_80.dll?dl=1"); dsts.push_back("bin/cublas64_80.dll");
	urls.push_back("https://www.dropbox.com/s/z14s7b9etmo9sd4/cudart64_80.dll?dl=1"); dsts.push_back("bin/cudart64_80.dll");
	urls.push_back("https://www.dropbox.com/s/fqk0p5ghmlij6v5/cudnn64_6.dll?dl=1"); dsts.push_back("bin/cudnn64_6.dll");
	urls.push_back("https://www.dropbox.com/s/8958cceosr5tyr0/curand64_80.dll?dl=1"); dsts.push_back("bin/curand64_80.dll");
	urls.push_back("https://www.dropbox.com/s/xs2px0z5xbpprg7/cusparse64_80.dll?dl=1"); dsts.push_back("bin/cusparse64_80.dll");
	urls.push_back("https://www.dropbox.com/s/oys58g4yozmlwwc/FasterRCNN_Train_SE.exe?dl=1"); dsts.push_back("bin/FasterRCNN_Train_SE.exe");
	urls.push_back("https://www.dropbox.com/s/uzdmeqh3yua54ez/libiomp5md.dll?dl=1"); dsts.push_back("bin/libiomp5md.dll");
	urls.push_back("https://www.dropbox.com/s/snsf5aysewfrkls/mkl_cntk_p.dll?dl=1"); dsts.push_back("bin/mkl_cntk_p.dll");
	urls.push_back("https://www.dropbox.com/s/xlh8vtr9inbktvc/msvcp140.dll?dl=1"); dsts.push_back("bin/msvcp140.dll");
	urls.push_back("https://www.dropbox.com/s/wx7k8qw9mjmosym/nvml.dll?dl=1"); dsts.push_back("bin/nvml.dll");
	urls.push_back("https://www.dropbox.com/s/nmr47lpqrmqy2yz/python35.dll?dl=1"); dsts.push_back("bin/python35.dll");
	urls.push_back("https://www.dropbox.com/s/7unsh9kl9taifma/ucrtbase.dll?dl=1"); dsts.push_back("bin/ucrtbase.dll");
	urls.push_back("https://www.dropbox.com/s/jja4niuxa7sv1g2/vccorlib140.dll?dl=1"); dsts.push_back("bin/vccorlib140.dll");
	urls.push_back("https://www.dropbox.com/s/4skunqose0ablnk/vcomp140.dll?dl=1"); dsts.push_back("bin/vcomp140.dll");
	urls.push_back("https://www.dropbox.com/s/j2fosolulp7m7m3/vcruntime140.dll?dl=1"); dsts.push_back("bin/vcruntime140.dll");
	urls.push_back("https://www.dropbox.com/s/ssb1b1uhj7fjj1k/xlwings32.dll?dl=1"); dsts.push_back("bin/xlwings32.dll");
	urls.push_back("https://www.dropbox.com/s/w82s2vw09wg6sdo/xlwings64.dll?dl=1"); dsts.push_back("bin/xlwings64.dll");
	urls.push_back("https://www.dropbox.com/s/lliy3d0sx2l5re7/zip.dll?dl=1"); dsts.push_back("bin/zip.dll");
	urls.push_back("https://www.dropbox.com/s/64cztnodovhsi6x/zlib.dll?dl=1"); dsts.push_back("bin/zlib.dll");
	for (size_t i = 0; i < urls.size(); i++) {
		ispring::Web::Download(urls[i], dsts[i]);
		_this->DownloadFile(urls[i], dsts[i]);
		//powershell "[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12; (New-Object System.Net.WebClient).DownloadFile('https://github.com%url%','AtomSetup-x64.exe')"
		GetApp().g_progress_current = i + 1;
	}
	int epoch = 1000;
	///BAT , Pretrained
	if (_this->GetChkBtnByString(_this->m_chk_models, _this->m_stc_models, TEXT("Alexnet"))->check == true) {
		_this->DownloadFile("https://www.dropbox.com/s/viwt2u8frkyvxvf/AlexNet_ImageNet_Caffe.model?dl=1", "AlexNet_ImageNet_Caffe.model");
		std::fstream fout("train-Alexnet.bat",std::ios::out);
		fout << "cd \"bin\"" << std::endl;
		fout << "\"FasterRCNN_Train_SE.exe\" " << epoch << " AlexNet" << std::endl;
		fout << "cd .." << std::endl;
		fout << "pause" << std::endl;
		fout.close();
	} else if (_this->GetChkBtnByString(_this->m_chk_models, _this->m_stc_models, TEXT("VGG16"))->check == true) {
		_this->DownloadFile("https://www.dropbox.com/s/ri9z43wrxzgr910/VGG16_ImageNet_Caffe.model?dl=1", "VGG16_ImageNet_Caffe.model");
		std::fstream fout("train-VGG16.bat", std::ios::out);
		fout << "cd \"bin\"" << std::endl;
		fout << "\"FasterRCNN_Train_SE.exe\" " << epoch << " VGG16" << std::endl;
		fout << "cd .." << std::endl;
		fout << "pause" << std::endl;
		fout.close();
	} else if (_this->GetChkBtnByString(_this->m_chk_models, _this->m_stc_models, TEXT("VGG19"))->check == true) {
		_this->DownloadFile("https://www.dropbox.com/s/xr44j3x65heuy4g/VGG19_ImageNet_Caffe.model?dl=1", "VGG19_ImageNet_Caffe.model");
		std::fstream fout("train-VGG19.bat", std::ios::out);
		fout << "cd \"bin\"" << std::endl;
		fout << "\"FasterRCNN_Train_SE.exe\" " << epoch << " VGG19" << std::endl;
		fout << "cd .." << std::endl;
		fout << "pause" << std::endl;
		fout.close();
	}

	GetApp().g_export_str = TEXT("Generate images");
	GetApp().g_progress_total = images.size();
	GetApp().g_progress_current = 0;
	ExportView::ImageStatistics stat;
	stat.nOfTotal = 0;
	int nOfClasses = 0;
	std::fstream fout_class_txt;
	fout_class_txt.open("class_map.txt", std::ios::out);
	fout_class_txt << "__background__" << "\t" << 0 << std::endl;
	for (auto&e : _this->m_list_class->m_data) {
		if (e.second == true) {
			nOfClasses++;
			std::string c(e.first.begin(), e.first.end());
			std::replace(c.begin(), c.end(), ' ', '_');
			fout_class_txt << c << "\t" << nOfClasses << std::endl;
		}
	}
	fout_class_txt.close();
	stat.nOfImages.assign(nOfClasses, 0);
	stat.nOfSamples.assign(nOfClasses, 0);
	std::fstream fout_train_txt;
	fout_train_txt.open("train_img_file.txt", std::ios::out);
	std::fstream fout_roi_txt;
	fout_roi_txt.open("train_roi_file.txt", std::ios::out);
	///이미지들 생성
	for (size_t i = 0; i < images.size(); i++) {
		auto& file = images[i].first;
		_this->GenExportImage(std::string(file.begin(), file.end()), fout_train_txt, stat,fout_roi_txt);
		GetApp().g_progress_current = i + 1;
	}
	///태깅정보 생성
	if (_this->m_chk_debug_info->check == true) {
		_this->GenTagInfos(stat);
	}
	fout_train_txt.close();
	fout_roi_txt.close();
	///Noise
	using Func = std::string(*)(std::string, DAType);
	std::vector<Func> func;
	func.push_back(Generate_Gray);
	func.push_back(Generate_SaltAndPepper);
	func.push_back(Generate_FilpLR);
	func.push_back(Generate_FilpTB);
	func.push_back(Generate_Slip);
	func.push_back(Generate_Blur);
	for (size_t i = 0; i < _this->m_str_noise.size()-2; i++) {
		TString da_tname = _this->m_str_noise[i];
		if (_this->GetChkBtnByString(_this->m_chk_noise, _this->m_stc_noise, da_tname)->check == true) {
			std::vector<std::string> rcnn_images = _this->GetRCNNImages();
			std::vector<std::vector<RCNNBOX>> rcnn_rois = _this->GetRCNNBoxes();
			if (rcnn_images.size() == rcnn_rois.size()) {
				
				std::fstream fout_train_txt;
				fout_train_txt.open("train_img_file.txt", std::ios::app);
				std::fstream fout_roi_txt;
				fout_roi_txt.open("train_roi_file.txt", std::ios::app);

				GetApp().g_export_str = TEXT("Generate ") + da_tname + TEXT(" images");
				GetApp().g_progress_current = 0;
				GetApp().g_progress_total = static_cast<int>(rcnn_images.size());
				for (int j = 0; (int)j < rcnn_images.size(); j++) {
					std::string new_imgfile= func[i](rcnn_images[j], DAType::FasterRCNN);
					fout_train_txt << _this->rcnn_cnt << "\t" << new_imgfile << "\t" << 0 << std::endl;
					if (da_tname == TEXT("Flip(LP)")) {
						fout_roi_txt << _this->rcnn_cnt << " |roiAndLabel";
						for (auto&box : rcnn_rois[j]) {
							std::ostringstream oss;
							fout_roi_txt << " " << (850-1-box.x2) << " " << box.y1 << " " << (850 - 1 - box.x1) << " " << box.y2 << " " << box.m_class << ".0";
						}
						fout_roi_txt << std::endl;
					} else if (da_tname == TEXT("Flip(TB)")) {
						fout_roi_txt << _this->rcnn_cnt << " |roiAndLabel";
						for (auto&box : rcnn_rois[j]) {
							std::ostringstream oss;
							fout_roi_txt << " " << box.x1 << " " << (850 - 1 - box.y2) << " " << box.x2 << " " << (850 - 1 - box.y1) << " " << box.m_class << ".0";
						}
						fout_roi_txt << std::endl;
					} else {
						fout_roi_txt << _this->rcnn_cnt << " |roiAndLabel";
						for (auto&box : rcnn_rois[j]) {
							std::ostringstream oss;
							fout_roi_txt << " " << box.x1 << " " << box.y1 << " " << box.x2 << " " << box.y2 << " " << box.m_class << ".0";
						}
						fout_roi_txt << std::endl;
					}
					_this->rcnn_cnt++;
					GetApp().g_progress_current++;
				}
				fout_train_txt.close();
				fout_roi_txt.close();
			} else {
				TSpringMsgBox msgbox(TEXT("Incorrect images or rois."));
				msgbox.DoModal();
			}
		}
	}
	{
		if (_this->GetChkBtnByString(_this->m_chk_noise, _this->m_stc_noise, _this->m_str_noise[_this->m_str_noise.size() - 1])->check == true
			|| _this->GetChkBtnByString(_this->m_chk_noise, _this->m_stc_noise, _this->m_str_noise[_this->m_str_noise.size() - 2])->check == true) {

			std::vector<std::string> rcnn_images = _this->GetRCNNImages();
			std::vector<std::vector<RCNNBOX>> rcnn_rois = _this->GetRCNNBoxes();
			if (rcnn_images.size() == rcnn_rois.size()) {
				std::fstream fout_train_txt;
				fout_train_txt.open("train_img_file.txt", std::ios::app);
				std::fstream fout_roi_txt;
				fout_roi_txt.open("train_roi_file.txt", std::ios::app);
				GetApp().g_export_str = TEXT("Generate Gamma-Correction images");
				GetApp().g_progress_current = 0;
				GetApp().g_progress_total = static_cast<int>(rcnn_images.size());
				for (int j = 0; (int)j < rcnn_images.size(); j++) {
					if (_this->GetChkBtnByString(_this->m_chk_noise, _this->m_stc_noise, _this->m_str_noise[_this->m_str_noise.size() - 1])->check == true) {
						std::string new_imgfile = Generate_GammaCorrection1(rcnn_images[j], DAType::FasterRCNN);
						fout_train_txt << _this->rcnn_cnt << "\t" << new_imgfile << "\t" << 0 << std::endl;

						fout_roi_txt << _this->rcnn_cnt << " |roiAndLabel";
						for (auto&box : rcnn_rois[j]) {
							std::ostringstream oss;
							fout_roi_txt << " " << box.x1 << " " << box.y1 << " " << box.x2 << " " << box.y2 << " " << box.m_class << ".0";
						}
						fout_roi_txt << std::endl;

						_this->rcnn_cnt++;
					}
					if (_this->GetChkBtnByString(_this->m_chk_noise, _this->m_stc_noise, _this->m_str_noise[_this->m_str_noise.size() - 2])->check == true) {
						std::string new_imgfile = Generate_GammaCorrection2(rcnn_images[j], DAType::FasterRCNN);
						fout_train_txt << _this->rcnn_cnt << "\t" << new_imgfile << "\t" << 0 << std::endl;

						fout_roi_txt << _this->rcnn_cnt << " |roiAndLabel";
						for (auto&box : rcnn_rois[j]) {
							std::ostringstream oss;
							fout_roi_txt << " " << box.x1 << " " << box.y1 << " " << box.x2 << " " << box.y2 << " " << box.m_class << ".0";
						}
						fout_roi_txt << std::endl;

						_this->rcnn_cnt++;
					}
					
					GetApp().g_progress_current++;
				}
				fout_train_txt.close();
				fout_roi_txt.close();
			} else {
				TSpringMsgBox msgbox(TEXT("Incorrect images or rois."));
				msgbox.DoModal();
			}			
		}
	}
	_chdir(CURR);
	GetApp().g_exporting = false;
	return 1;
}
void ExportView::Export_FasterRCNN() {
	GetApp().g_exporting = true;
	AfxGetMainWnd()->SetTimer(7777, 20, nullptr);
	AfxBeginThread(Export_FasterRCNN_T, this);
}