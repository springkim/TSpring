
#include "stdafx.h"
#include "TagView.h"
void DrawColorBoxSE(cv::Mat& img, cv::Rect rect, cv::Scalar c) {
	cv::Mat crop = img(rect);
	cv::Mat box = cv::Mat::zeros(rect.height, rect.width, CV_8UC3) + c;
	crop = 0.3*crop + 0.7*box;
}
CRect DrawCvMat(CDC* pDC, std::vector<TagInfo>& tag_data, cv::Mat& origin, CRect rect, int alphablend = 0, cv::RotatedRect rrect = cv::RotatedRect()) {

	auto MakeColorBox = [](int w, int h, COLORREF c)->cv::Mat {
		cv::Mat box = cv::Mat::zeros(h, w, CV_8UC3) + cv::Scalar(GetBValue(c), GetGValue(c), GetRValue(c));
		return box;
	};
	CImage mfcImg;
	cv::Mat outImg = origin.clone();
	cv::Rect img_rect(0, 0, origin.cols, origin.rows);

	for (auto&td : tag_data) {
		if (td.m_class == -100) {	//ignore
			cv::Rect cvrect = GetSafeRect(td.m_rect.boundingRect(), outImg.size());
			outImg(cvrect) = 0.3*outImg(cvrect) + 0.7* cv::Scalar(33, 33, 33);
		}
	}

	if (alphablend != 0) {
		int r, g, b;
		float d = 0.3F;
		switch (alphablend) {
			case 1: {
				d = 1.0F;
				COLORREF lv_color_bk = *GetTheme().ListViewColorBK();
				r = GetRValue(lv_color_bk);
				g = GetGValue(lv_color_bk);
				b = GetBValue(lv_color_bk);
				outImg = 0.3*outImg + 0.7*cv::Scalar(b, g, r);
			}break;
			case 2: {
				r = 220; g = 50; b = 47;
			}break;
			case 3: {
				r = 38; g = 139; b = 210;
			}break;
			case 4: {
				r = 181; g = 137; b = 0;
			}break;
		}
		cv::Vec3b color(b, g, r);
		cv::Rect cvrect = rrect.boundingRect();
		mspring::SetRange(cvrect.x, 0, outImg.cols - 1);
		mspring::SetRange(cvrect.y, 0, outImg.rows - 1);
		if (cvrect.x + cvrect.width >= outImg.cols) {
			cvrect.width = outImg.cols - cvrect.x - 1;
		}
		if (cvrect.y + cvrect.height >= outImg.rows) {
			cvrect.height = outImg.rows - cvrect.y - 1;
		}
		if (rrect.angle != 0) {
			cv::Point2f pt[4];
			rrect.points(pt);
			for (int y = cvrect.y; y < cvrect.y + cvrect.height; y++) {
				for (int x = cvrect.x; x < cvrect.x + cvrect.width; x++) {
					if (ispring::CVGeometry::PtInRectangle(cv::Point(x, y), pt[0], pt[1], pt[2], pt[3]) == true) {
						if (img_rect.contains(cv::Point(x, y)) == true) {
							outImg.at<cv::Vec3b>(y, x) = origin.at<cv::Vec3b>(y, x)*d + color*(1.0F - d);
						}
					}
				}
			}
		} else {
			cvrect.width--;
			cvrect.height--;
			outImg(cvrect) = origin(cvrect)*d + cv::Scalar(b,g,r)*(1.0F-d);
		}
	}

	cv::flip(outImg, outImg, 0);

	//흑백이면 채널을3으로
	if (outImg.channels() == 1) {
		cv::cvtColor(outImg, outImg, CV_GRAY2BGR);
	}
	//이미지의 크기를 rect에 들어가게 수정합니다.
	if (outImg.cols != rect.Width() || outImg.rows != rect.Height()) {
		int newWidth = static_cast<int>(outImg.cols*(rect.Height() / (double)outImg.rows));
		int newHeight = rect.Height();
		if (newWidth > rect.Width()) {
			newWidth = rect.Width();
			newHeight = static_cast<int>(outImg.rows*(rect.Width() / (double)outImg.cols));
		}
		cv::resize(outImg, outImg, cv::Size(newWidth, newHeight), 0, 0, CV_INTER_NN);
	}
	mfcImg.Create(outImg.cols, outImg.rows, 24);
	BITMAPINFO bitInfo = { { sizeof(BITMAPINFOHEADER),outImg.cols,outImg.rows,1,24 },0 };

	void* vp = outImg.data;
	IplImage* iplimage = nullptr;
	if (outImg.cols % 4 != 0 && outImg.isContinuous()) {
		iplimage = cvCreateImage(outImg.size(), 8, outImg.channels());
		outImg.copyTo(cv::cvarrToMat(iplimage));
		vp = iplimage->imageData;
	}
	pDC->SetStretchBltMode(HALFTONE);
	StretchDIBits(mfcImg.GetDC(), 0, 0, outImg.cols, outImg.rows, 0, 0, outImg.cols, outImg.rows, vp, &bitInfo, DIB_RGB_COLORS, SRCCOPY);
	if (iplimage != nullptr) {
		cvReleaseImage(&iplimage);
	}
	mfcImg.BitBlt(*pDC, (rect.Width() - outImg.cols) / 2 + rect.left, (rect.Height() - outImg.rows) / 2 + rect.top);

	CRect crect;
	crect.left = (rect.Width() - outImg.cols) / 2 + rect.left;
	crect.top = (rect.Height() - outImg.rows) / 2 + rect.top;
	crect.right = crect.left + outImg.cols;
	crect.bottom = crect.top + outImg.rows;

	mfcImg.ReleaseDC();
	outImg.release();

	return crect;
}


TagView::TagView(CWnd* wnd) : VirtualView(wnd) {
	m_list_class = CreateControl<MListBox>(wnd, MRect(MRectPosition::R, 380, 10, 10, 350));
	m_list_class->m_color_bk = GetTheme().ListViewColorBK();
	m_list_class->m_color_text = GetTheme().ListViewColorText();
	m_list_class->is_numbering = true;
	m_list_class->m_color_fr = GetTheme().ListColorScroll();

	m_stc_path = CreateControl<MStatic>(wnd, MRect(MRectPosition::B, 10, 30, 380, 5));
	m_stc_path->m_color_text = GetTheme().ListViewColorText();
	m_stc_path->m_text = TEXT("");

	m_stc_tag_info = CreateControl<MStatic>(wnd, MRect(MRectPosition::RB, 10, 150, 380, 30));
	m_stc_tag_info->m_color_text = GetTheme().ListViewColorText();
	m_stc_tag_info->m_text = TEXT("");

	m_stc_box_angle = CreateControl<MStatic>(wnd, MRect(MRectPosition::RB, 10, 200, 380, 30));
	m_stc_box_angle->m_color_text = GetTheme().ListViewColorText();
	m_stc_box_angle->m_text = TEXT("");

	m_stc_box_coord = CreateControl<MStatic>(wnd, MRect(MRectPosition::RB, 10, 250, 380, 30));
	m_stc_box_coord->m_color_text = GetTheme().ListViewColorText();
	m_stc_box_coord->m_text = TEXT("");

	m_stc_box_class = CreateControl<MStatic>(wnd, MRect(MRectPosition::RB, 10, 300, 380, 30));
	m_stc_box_class->m_color_text = GetTheme().ListViewColorText();
	m_stc_box_class->m_text = TEXT("");

	m_btn_pen_style = CreateControl<MButton>(wnd, MRect(MRectPosition::RB, 390 - 190+40, 110, 150, 25));
	m_btn_pen_style->m_color_text = GetTheme().ButtonColorText();
	m_btn_pen_style->m_color_fr= GetTheme().ButtonColorFR();
	m_btn_pen_style->m_text = TEXT("");

	m_chk_magnifier = CreateControl<MButtonCheck>(wnd, MRect(MRectPosition::RB, 390 - 25 - 200, 110, 25, 25));
	m_chk_magnifier->m_color_text = GetTheme().ButtonColorText();
	m_chk_magnifier->m_color_fr = GetTheme().ButtonColorFR();
	m_stc_magnifier = CreateControl<MStatic>(wnd, MRect(MRectPosition::RB, 10, 110, 200 - 25 - 25 - 5, 25));
	m_stc_magnifier->m_color_text = GetTheme().ListViewColorText();
	m_stc_magnifier->m_text = TEXT("Magnifier(&M)");

	m_chk_crosshair = CreateControl<MButtonCheck>(wnd, MRect(MRectPosition::RB, 390 - 25, 60, 25, 25));
	m_chk_crosshair->m_color_text = GetTheme().ButtonColorText();
	m_chk_crosshair->m_color_fr = GetTheme().ButtonColorFR();
	m_stc_crosshair = CreateControl<MStatic>(wnd, MRect(MRectPosition::RB, 190 - 25 - 10, 60, 200, 25));
	m_stc_crosshair->m_color_text = GetTheme().ListViewColorText();
	m_stc_crosshair->m_text = TEXT("Crosshair(&C)");

	m_chk_showid = CreateControl<MButtonCheck>(wnd, MRect(MRectPosition::RB, 390 - 25 - 200, 60, 25, 25));
	m_chk_showid->m_color_text = GetTheme().ButtonColorText();
	m_chk_showid->m_color_fr = GetTheme().ButtonColorFR();
	m_stc_showid = CreateControl<MStatic>(wnd, MRect(MRectPosition::RB, 10, 60, 200 - 25 - 25 - 5, 25));
	m_stc_showid->m_color_text = GetTheme().ListViewColorText();
	m_stc_showid->m_text = TEXT("Show ID(&I)");

	m_chk_edit = CreateControl<MButtonCheck>(wnd, MRect(MRectPosition::RB, 390 - 25, 10, 25, 25));
	m_chk_edit->m_color_text = GetTheme().ButtonColorText();
	m_chk_edit->m_color_fr = GetTheme().ButtonColorFR();
	m_stc_edit = CreateControl<MStatic>(wnd, MRect(MRectPosition::RB, 190 - 25 - 10, 10, 200, 25));
	m_stc_edit->m_color_text = GetTheme().ListViewColorText();
	m_stc_edit->m_text = TEXT("Edit mode(&E)");

	m_chk_tracking = CreateControl<MButtonCheck>(wnd, MRect(MRectPosition::RB, 390 - 25 - 200, 10, 25, 25));
	m_chk_tracking->m_color_text = GetTheme().ButtonColorText();
	m_chk_tracking->m_color_fr = GetTheme().ButtonColorFR();
	m_stc_tracking = CreateControl<MStatic>(wnd, MRect(MRectPosition::RB, 10, 10, 200 - 25 - 25 - 5, 25));
	m_stc_tracking->m_color_text = GetTheme().ListViewColorText();
	m_stc_tracking->m_text = TEXT("Tracking mode(&T)");

	m_drag_point.x = -1;
	m_drag_point.y = -1;
	m_img_rect.left = -1;

	GetApp().g_pen_weight = &m_pen_style;
	GetApp().g_is_magnifier = &m_chk_magnifier->check;
	GetApp().g_is_crosshair = &m_chk_crosshair->check;
	GetApp().g_is_showid = &m_chk_showid->check;

	this->AddControl(m_list_class);
	this->AddControl(m_stc_path);
	this->AddControl(m_stc_tag_info);
	this->AddControl(m_stc_box_class);
	this->AddControl(m_stc_box_coord);
	this->AddControl(m_stc_box_angle);
	this->AddControl(m_chk_magnifier);
	this->AddControl(m_stc_magnifier);
	this->AddControl(m_chk_showid);
	this->AddControl(m_stc_showid);
	this->AddControl(m_chk_crosshair);
	this->AddControl(m_stc_crosshair);
	this->AddControl(m_chk_edit);
	this->AddControl(m_stc_edit);
	this->AddControl(m_chk_tracking);
	this->AddControl(m_stc_tracking);
	this->AddControl(m_btn_pen_style);
}


TagView::~TagView() {

}
int TagView::OnCreate() {
	VirtualView::OnCreate();
	return 1;
}
void TagView::OnDestroy() {
	VirtualView::OnDestroy();
}

void TagView::OnPaint(CDC* pDC) {
	m_chk_tracking->check = *GetApp().g_is_tracking;
	if (*GetApp().g_is_r2pt || *GetApp().g_is_rmid) {
		m_chk_tracking->disable = true;
	} else {
		m_chk_tracking->disable = false;
	}
	if (GetApp().g_image_data->size() > 0 && GetApp().g_tag_idx >= 0) {
		OStringStream oss;
		oss << TEXT("[") << GetApp().g_tag_idx + 1 << TEXT("]/[") << static_cast<int>(GetApp().g_image_data->size()) << TEXT("] ") << GetApp().g_image_data->at(GetApp().g_tag_idx).first;
		m_stc_path->m_text = oss.str();
	}
	{	//풀지마셈
		OStringStream oss;
		oss << m_tag_data.size() << TEXT(" box") << TEXT("\0es") + (m_tag_data.size() > 1) << TEXT(" tagged");
		m_stc_tag_info->m_text = oss.str();
	}
	if (m_list_class->m_select == -1 && GetApp().g_class_data->size() > 0) {
		m_list_class->m_select = 0;
	}
	switch (m_pen_style) {
		case 0:m_btn_pen_style->m_text = TEXT("Light pen"); break;
		case 1:m_btn_pen_style->m_text = TEXT("Heavy pen"); break;
		case 2:m_btn_pen_style->m_text = TEXT("Thin pen"); break;
	}
	///Bottle nect #1
	if (m_list_class->m_data.size() != GetApp().g_class_data->size()) {
		m_list_class->m_data.assign(GetApp().g_class_data->begin(), GetApp().g_class_data->end());
	}
	m_stc_box_class->m_text.clear();
	m_stc_box_coord->m_text.clear();
	m_stc_box_angle->m_text.clear();

	CRect rect = this->GetViewRect();
	//Draw image
	CRect rect_image;
	rect_image.left = 10;
	rect_image.top = 10;
	rect_image.bottom = rect.bottom - 40;
	rect_image.right = rect.right - m_list_class->m_rect.GetRect(rect).Width() - 20;

	CBrush brush_bk; brush_bk.CreateSolidBrush(*GetTheme().ListViewColorBK());
	CPen pen_null; pen_null.CreatePen(PS_NULL, 0, RGB(0, 0, 0));
	CBrush* old_brush = pDC->SelectObject(&brush_bk);
	CPen* old_pen = pDC->SelectObject(&pen_null);

	static TString path = TEXT("");
	if (GetApp().g_image_data->size() > GetApp().g_tag_idx && path != GetApp().g_image_data->at(GetApp().g_tag_idx).first) {
		path = GetApp().g_image_data->at(GetApp().g_tag_idx).first;
		std::string cpp_path = mspring::String::ToString(path);
		m_img.release();
		m_img = cv::imread(cpp_path);
		m_img_rect.left = -1;
		ReadTagFile();
	}
	if (GetApp().g_image_data->size() == 0) {
		m_img.release();
	}
	pDC->Rectangle(rect_image);



	int focused = -1;
	if (m_img.empty() == false) {		//Draw image		
		focused = GetFocusedTag();
		if (m_chk_edit->check == false) {	//Tag mode
			int will_remove = -1;
			if ((GetAsyncKeyState(VK_RBUTTON) & 0x8000) && !(GetAsyncKeyState(VK_LBUTTON) & 0x8000)) {
				will_remove = this->GetFocusedTag();
				if (will_remove != -1) {
					m_img_rect = DrawCvMat(pDC, m_tag_data, m_img, rect_image, 2, m_tag_data[will_remove].m_rect);
				} else {
					m_img_rect = DrawCvMat(pDC, m_tag_data, m_img, rect_image);
				}
			} else if (m_drag_point.x != -1 && m_drag_point.y != -1) {
				m_img_rect = DrawCvMat(pDC, m_tag_data, m_img, rect_image, 1, m_tag_data.back().m_rect);
			} else {
				if (focused != -1) {
					m_img_rect = DrawCvMat(pDC, m_tag_data, m_img, rect_image, 3, m_tag_data[focused].m_rect);
				} else {
					m_img_rect = DrawCvMat(pDC, m_tag_data, m_img, rect_image);
				}
			}
			//Draw tag info
			cv::Point2f pt[4];
			for (int i = 0; i < m_tag_data.size(); i++) {
				auto e = m_tag_data[i];
				e.m_rect.points(pt);
				for (int j = 0; j < 4; j++) {
					pt[j].x = pt[j].x*m_img_rect.Width() / m_img.cols + m_img_rect.left;
					pt[j].y = pt[j].y*m_img_rect.Height() / m_img.rows + m_img_rect.top;
				}
				auto c = ispring::CV::GetRGB(e.m_class);
				CPen pen;
				switch (m_pen_style) {
					case 0:pen.CreatePen(PS_SOLID, 2, RGB(c[0], c[1], c[2])); break;
					case 1:pen.CreatePen(PS_SOLID, 4, RGB(c[0], c[1], c[2])); break;
					case 2:pen.CreatePen(PS_SOLID, 1, RGB(c[0], c[1], c[2])); break;
				}
				CPen* old_pen = pDC->SelectObject(&pen);
				pDC->MoveTo(static_cast<int>(pt[3].x), static_cast<int>(pt[3].y));
				for (int j = 0; j < 4; j++) {
					pDC->LineTo(static_cast<int>(pt[j].x), static_cast<int>(pt[j].y));
				}
				if(m_chk_showid->check==true){
					CFont font;
					font.CreateFont(30, 0, static_cast<int>(-e.m_rect.angle*10),0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS,GetTheme().Font().c_str());
					CFont* old_font=pDC->SelectObject(&font);
					pDC->SetTextColor(RGB(255,255,255));
					pDC->SetBkMode(OPAQUE);
					pDC->SetBkColor(RGB(c[0], c[1], c[2]));

					OStringStream oss;
					oss << TEXT(" ") << e.m_id << TEXT(" ");
					pDC->TextOut(static_cast<int>(pt[0].x), static_cast<int>(pt[0].y), oss.str().c_str(), static_cast<int>(oss.str().length()));
					CSize sz;
					GetTextExtentPoint32(pDC->GetSafeHdc(), oss.str().c_str(), static_cast<int>(oss.str().length()), &sz);
					cv::RotatedRect rrect;
					rrect.center.x = pt[0].x + sz.cx / 2;
					rrect.center.y = pt[0].y + sz.cy / 2;
					rrect.size.width = static_cast<float>( sz.cx);
					rrect.size.height = static_cast<float>(sz.cy);
					rrect.angle = e.m_rect.angle;
					m_tag_data[i].m_id_rect = rrect;
					pDC->SelectObject(old_font);
					font.DeleteObject();
				}
				CPoint ground1, ground2;
				ground1.x = static_cast<LONG>((pt[0].x + pt[1].x) / 2);
				ground1.y = static_cast<LONG>((pt[0].y + pt[1].y) / 2);
				ground2.x = static_cast<LONG>((pt[2].x + pt[3].x) / 2);
				ground2.y = static_cast<LONG>((pt[2].y + pt[3].y) / 2);
				if (*GetApp().g_is_r2pt == true || *GetApp().g_is_rmid == true) {
					CPen pen_dot;

					pen_dot.CreatePen(PS_DASH, 1, RGB(c[0], c[1], c[2]));
					pDC->SelectObject(&pen_dot);

					pDC->SetBkMode(TRANSPARENT);
					pDC->MoveTo(ground1);
					pDC->LineTo(ground2);
					pDC->SelectObject(&pen);
					pen_dot.DeleteObject();
				}
				if (e.m_class >= static_cast<int>(GetApp().g_class_data->size())) {
					CPoint cpt[4];
					for (int j = 0; j < 4; j++) {
						cpt[j].x = static_cast<LONG>(pt[j].x);
						cpt[j].y = static_cast<LONG>(pt[j].y);
					}
					CBitmap bmp;
					CBrush brush;
					if (will_remove == i) {
						bmp.LoadBitmap(IDB_UNKNOWN_RED);
					} else if (focused == i) {
						bmp.LoadBitmap(IDB_UNKNOWN_BLUE);
					} else {
						bmp.LoadBitmap(IDB_UNKNOWN);
					}
					brush.CreatePatternBrush(&bmp);
					CBrush* old_brush = pDC->SelectObject(&brush);

					pDC->Polygon(cpt, 4);
					pDC->SelectObject(old_brush);
					bmp.DeleteObject();
					brush.DeleteObject();
				}
				pDC->SelectObject(old_pen);
				pen.DeleteObject();
			}
		} else {

			int focused = this->GetFocusedTag();
			if (focused != -1) {
				m_img_rect = DrawCvMat(pDC, m_tag_data, m_img, rect_image, 4, m_tag_data[focused].m_rect);
			} else {
				m_img_rect = DrawCvMat(pDC, m_tag_data, m_img, rect_image);
			}
			for (int i = 0; i < m_tag_data.size(); i++) {
				auto e = m_tag_data[i];
				cv::Point2f pt[4];
				e.m_rect.points(pt);
				for (int j = 0; j < 4; j++) {
					pt[j].x = pt[j].x*m_img_rect.Width() / m_img.cols + m_img_rect.left;
					pt[j].y = pt[j].y*m_img_rect.Height() / m_img.rows + m_img_rect.top;
				}
				auto c = ispring::CV::GetRGB(e.m_class);
				CPen pen; 
				switch (m_pen_style) {
					case 0:pen.CreatePen(PS_SOLID, 2, RGB(c[0], c[1], c[2])); break;
					case 1:pen.CreatePen(PS_SOLID, 4, RGB(c[0], c[1], c[2])); break;
					case 2:pen.CreatePen(PS_SOLID, 1, RGB(c[0], c[1], c[2])); break;
				}
				CPen* old_pen = pDC->SelectObject(&pen);
				pDC->MoveTo(static_cast<int>(pt[3].x), static_cast<int>(pt[3].y));
				for (int j = 0; j < 4; j++) {
					pDC->LineTo(static_cast<int>(pt[j].x), static_cast<int>(pt[j].y));
				}
				if (m_chk_showid->check == true) {
					CFont font;
					font.CreateFont(30, 0, static_cast<int>(-e.m_rect.angle * 10), 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, GetTheme().Font().c_str());
					CFont* old_font = pDC->SelectObject(&font);
					pDC->SetTextColor(RGB(255, 255, 255));
					pDC->SetBkMode(OPAQUE);
					pDC->SetBkColor(RGB(c[0], c[1], c[2]));

					OStringStream oss;
					oss << TEXT(" ") << e.m_id << TEXT(" ");
					pDC->TextOut(static_cast<int>(pt[0].x), static_cast<int>(pt[0].y), oss.str().c_str(), static_cast<int>(oss.str().length()));
					CSize sz;
					GetTextExtentPoint32(pDC->GetSafeHdc(), oss.str().c_str(), static_cast<int>(oss.str().length()), &sz);
					cv::RotatedRect rrect;
					rrect.center.x = pt[0].x + sz.cx / 2;
					rrect.center.y = pt[0].y + sz.cy / 2;
					rrect.size.width = static_cast<float>(sz.cx);
					rrect.size.height = static_cast<float>(sz.cy);
					rrect.angle =e.m_rect.angle;
					m_tag_data[i].m_id_rect = rrect;
					pDC->SelectObject(old_font);
					font.DeleteObject();
				}
				pDC->SelectObject(old_pen);
				pen.DeleteObject();
			}

		}
	} else {
		m_img_rect = CRect(0, 0, 0, 0);
		DrawBitmap(pDC, rect_image, GetTheme().ResourceUsage());
	}
	
	pDC->SelectObject(old_pen);
	pDC->SelectObject(old_brush);
	if (focused != -1) {
		if (m_tag_data[focused].m_class >= static_cast<int>(m_list_class->m_data.size())) {
			OStringStream oss;
			oss << TEXT("class : Unknown(") << m_tag_data[focused].m_class << TEXT(")");
			m_stc_box_class->m_text = oss.str();
			///m_stc_box_class->m_color_text = &g_tag_color_text_red;
		} else {
			if (m_tag_data[focused].m_class != -100) {
				OStringStream oss;
				oss << TEXT("class : ") << m_list_class->m_data[m_tag_data[focused].m_class].first << TEXT("(") << m_tag_data[focused].m_class << TEXT(")");
				m_stc_box_class->m_text = oss.str();
			} else {
				m_stc_box_class->m_text=TEXT("class : Ignore(Ignore)");
			}
			///m_stc_box_class->m_color_text = &g_tag_color_text_purple;
		}
		{
			OStringStream oss;
			oss << TEXT("X(") << m_tag_data[focused].m_rect.center.x << TEXT("), Y(") << m_tag_data[focused].m_rect.center.y
				<< TEXT("), W(") << m_tag_data[focused].m_rect.size.width << TEXT("), H(") << m_tag_data[focused].m_rect.size.height << TEXT(")");
			m_stc_box_coord->m_text = oss.str();
		}
		{
			OStringStream oss;
			oss << TEXT("Angle(") << -m_tag_data[focused].m_rect.angle << TEXT(")");
			if (m_chk_showid->check == true) {
				oss << TEXT(", ID(") << m_tag_data[focused].m_id << TEXT(")");
			}
			m_stc_box_angle->m_text = oss.str();
		}
	}
	MCALL;
	if (m_chk_crosshair->check == true) {
		DrawCrosshair(pDC, m_img_rect);
	}
	if (m_chk_magnifier->check == true) {
		DrawLens(pDC, m_img_rect);
	}
	if (m_list_class->m_data.size() == 0) {
		CRect rect = m_list_class->m_rect.GetRect(this->GetViewRect());
		DrawBitmap(pDC, rect, GetTheme().ResourceNoList());
	}
	if (GetApp().g_exporting == true) {
		ShowProgressBar(pDC, this->GetViewRect());
	}
}
void TagView::OnSetFocus(CWnd* pOldWnd) {
	VirtualView::OnSetFocus(pOldWnd);
}
void TagView::OnKillFocus(CWnd* pNewWnd) {
	VirtualView::OnKillFocus(pNewWnd);
}

void TagView::OnLButtonDown(UINT nFlags, CPoint point) {
	if (GetApp().g_exporting == true)return;
	AfxGetMainWnd()->SendMessage(MSPRING_DISABLE_HTTEST, 1);
	MCALL;
	if (m_btn_pen_style->isClicked()) {
		int n = m_pen_style;
		n = (n + 1) % 3;
		m_pen_style = (n);
	}
	point = this->GetMousePoint();
	::SetFocus(this->m_parent->GetSafeHwnd());
	if ((GetAsyncKeyState(VK_RBUTTON) & 0x8000)) {
		return;		//R button 이 작업중이면 무시
	}
	m_l_down = this->GetMousePoint();
	*GetApp().g_is_tracking = m_chk_tracking->check;
	
	if (m_list_class->m_select == -1 && m_img_rect.PtInRect(point)) {
		TSpringMsgBox msgbox(TEXT("Please, make class list before tagging"));
		msgbox.DoModal();
		AfxGetMainWnd()->SendMessage(MSPRING_DISABLE_HTTEST, 0);
		return;
	}
	if (m_chk_edit->isChanged() && m_chk_edit->check == true) {
		if (m_chk_edit->check == false) {	//에디트 체크를 해지하면 커서를 화살표(기본) 으로 변경
			m_is_edit = false;
			m_edit_idx = -1;
			GetApp().g_degree = -1;
			AfxGetMainWnd()->SendMessage(WM_SETCURSOR);
		}
		return;
	}
	if (m_chk_edit->check == false) {	//Tag start
		if (m_list_class->m_select != -1 && m_img_rect.PtInRect(point)) {
			m_drag_point = point;
			m_angle = 0;
			cv::RotatedRect rrect = this->GetRotatedRect(m_drag_point, m_drag_point, m_angle);
			if ((GetAsyncKeyState('q') & 0x8000) || (GetAsyncKeyState('Q') & 0x8000)) {
				m_tag_data.push_back(TagInfo(-100, rrect,-1));
			} else {
				m_tag_data.push_back(TagInfo(m_list_class->m_select, rrect,-1));
			}
			this->m_parent->SetTimer(MOUSE_LEAVE_TIMER_ID, 10, nullptr);
		}
	} else {		//Edit start
		m_is_edit = true;
		if (m_chk_edit->check == true) {
			m_edit_idx = this->GetFocusedTag();
			if (m_edit_idx != -1) {
				m_move_base = m_tag_data[m_edit_idx].m_rect.center;
			}
		}
		this->m_parent->SetTimer(MOUSE_LEAVE_TIMER_ID, 10, nullptr);
	}
}
void TagView::OnLButtonUp(UINT nFlags, CPoint point) {
	if (GetApp().g_exporting == true)return;
	AfxGetMainWnd()->SendMessage(MSPRING_DISABLE_HTTEST, 0);
	MCALL;
	this->m_parent->KillTimer(MOUSE_LEAVE_TIMER_ID);
	if (m_chk_edit->check == false) {	//Tag end
		m_drag_point.x = -1;
		m_drag_point.y = -1;
		if (m_tag_data.empty() == false) {
			if (m_tag_data.back().m_rect.size.width < 32 && m_tag_data.back().m_rect.size.height < 32) {
				m_tag_data.pop_back();
			}
		}
	} else {		//edit end
		this->ClearEditState();
	}

	WriteTagFile();
}
void TagView::OnLButtonDblClk(UINT nFlags, CPoint point) {
	if (GetApp().g_exporting == true)return;
	MCALL;
	VirtualView::OnLButtonDblClk(nFlags, point);
}
void TagView::OnRButtonDown(UINT nFlags, CPoint point) {
	if (GetApp().g_exporting == true)return;

	if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000)) {
		return;		//L 버튼이 작업중이면 무시
	}
	MCALL;
	m_r_down = this->GetMousePoint();
	this->m_parent->Invalidate();
}
void TagView::OnRButtonUp(UINT nFlags, CPoint point) {
	if (GetApp().g_exporting == true)return;
	if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000)) {
		return;
	}
	MCALL;
	if (m_tag_data.size() == 0) {
		return;
	}
	if(m_chk_edit->check==false && m_chk_showid->check==true){
		int idx = this->GetFocusedTag(true);
		if (idx != -1) {
			std::vector<int> previd = this->GetPreviousIDs();
			MSpringMenu popup(this->m_parent);
			popup.CreatePopupMenu();
			for (int i = 0; i<previd.size(); i++) {
				CString str;
				str.Format(TEXT("ID : %d"), previd[i]);
				popup.AppendMenu(MF_STRING, i + 1, str);
			}
			popup.AppendMenu(MF_STRING, previd.size() + 1, TEXT("New ID"));
			popup.AppendMenu(MF_STRING, previd.size() + 2, TEXT("Remove Tag"));
			popup.AppendMenu(MF_STRING, previd.size() + 3, TEXT("Input"));
			popup.MakeItemsOwnDraw();
			GetCursorPos(&point);
			int menu_id = popup.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD, point.x, point.y, this->m_parent);
			popup.DestroyMenu();
			if (menu_id == 0) {

			} else if (menu_id < previd.size() + 1) {
				m_tag_data[idx].m_id = previd[menu_id - 1];
			} else if (menu_id == previd.size() + 1) {
				m_tag_data[idx].m_id = GetApp().g_id++;
			} else if (menu_id == previd.size() + 2) {
				m_tag_data.erase(m_tag_data.begin() + idx);
			} else {
				int id = 0;
				InputBox dlg(NULL, id);
				if (dlg.DoModal() == IDOK) {
					m_tag_data[idx].m_id = id;
				}
			}
			WriteTagFile();
			return;
		}
	}

	if (m_chk_edit->check == false) {
		int idx = this->GetFocusedTag();
		if (idx != -1) {
			m_tag_data.erase(m_tag_data.begin() + idx);
		}
		m_r_down.x = -1;
		m_r_down.y = -1;
	} else {
		this->ClearEditState();
		int idx = this->GetFocusedTag();
		if (idx != -1) {
			m_tag_data[idx].m_class = m_list_class->m_select;
		}
	}
	WriteTagFile();
}
BOOL TagView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) {
	if (GetApp().g_exporting == true)return FALSE;
	MCALL;
	CPoint point = this->GetMousePoint();
	if (m_chk_edit->check == false) {	//Tag mode
		if (m_drag_point.x != -1 && m_drag_point.y != -1) {	//if Draging
			if (m_tag_data.back().m_class != -100) {
				if (*GetApp().g_is_rmid == true || *GetApp().g_is_r2pt == true) {
					int val = 3;
					if ((::GetKeyState(VK_CONTROL) & 0x8000) == 0x8000) {
						val = 1;
					}
					if ((::GetKeyState(VK_SHIFT) & 0x8000) == 0x8000) {
						val = 10;
					}
					if (zDelta > 0) {
						m_angle = (m_angle + val) % 360;
					} else {
						m_angle = (m_angle - val + 360) % 360;
					}
					UpdateTagInfo();
				}
			}
		}
	}
	return TRUE;
}
void TagView::OnMouseMove(UINT nFlags, CPoint point) {
	if (GetApp().g_exporting == true)return;
	
	if (m_chk_edit->check == false) {	//Tag mode
		if (m_drag_point.x != -1 && m_drag_point.y != -1) {
			UpdateTagInfo();
		}
	} else {
		if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) && m_edit_idx != -1 && m_is_resize == false) {
			//이동
			point = this->GetMousePoint();
			cv::RotatedRect rrect = m_tag_data[m_edit_idx].m_rect;
			rrect.center.x = m_move_base.x + (point.x - m_l_down.x)*m_img.cols / m_img_rect.Width();
			rrect.center.y = m_move_base.y + (point.y - m_l_down.y)*m_img.rows / m_img_rect.Height();
			if (this->isBoundaryViolation(rrect) == false) {
				m_tag_data[m_edit_idx].m_rect = rrect;
			} else {
				m_tag_data[m_edit_idx].m_rect = SetRectInBoundary(rrect);
			}
			return;
		}
		if (m_is_edit == true) {
			if (m_edit_idx == -1) return;
			if (m_is_resize == false)return;
			if (m_is_side == false) {	//꼭지점 변경
				m_apex[1] = this->GetMousePoint();
				cv::RotatedRect rrect = this->GetRotatedRect(m_apex[0], m_apex[1], -static_cast<int>(m_tag_data[m_edit_idx].m_rect.angle));
				cv::Point2f pt[4];
				rrect.points(pt);
				bool contain = true;
				for (auto&p : pt) {
					contain &= cv::Rect(0, 0, m_img.cols, m_img.rows).contains(p);
				}
				if (contain == true) {
					m_tag_data[m_edit_idx].m_rect = rrect;
				}
			} else {					//변 변경
				std::array<int, 4> idx;
				int beg = m_edit_idx_side;
				for (size_t i = 0; i < 4; i++) {
					idx[i] = (beg - 1 + 4) % 4;
					beg = idx[i];
				}
				cv::Point2f rpts[4];
				m_side_rect.points(rpts);
				CPoint point = this->GetMousePoint();
				wykobi::point2d<float> wpoint;
				wpoint.x = static_cast<float>(point.x);
				wpoint.y = static_cast<float>(point.y);
				float dist = wykobi::minimum_distance_from_point_to_segment<float>(wpoint, m_side);
				wykobi::line<float, 2> line;
				line[0] = m_side[0];
				line[1] = m_side[1];
				wykobi::point2d<float> wpoint_base = wykobi::closest_point_on_line_from_point(line, wpoint);
				rpts[idx[2]].x -= (wpoint_base.x - wpoint.x)*m_img.cols / m_img_rect.Width();
				rpts[idx[2]].y -= (wpoint_base.y - wpoint.y)*m_img.rows / m_img_rect.Height();
				rpts[idx[3]].x -= (wpoint_base.x - wpoint.x)*m_img.cols / m_img_rect.Width();
				rpts[idx[3]].y -= (wpoint_base.y - wpoint.y)*m_img.rows / m_img_rect.Height();

				std::vector<wykobi::segment<float, 2>> lines(2);
				lines[0][0].x = rpts[idx[0]].x;
				lines[0][0].y = rpts[idx[0]].y;
				lines[0][1].x = rpts[idx[2]].x;
				lines[0][1].y = rpts[idx[2]].y;
				lines[1][0].x = rpts[idx[1]].x;
				lines[1][0].y = rpts[idx[1]].y;
				lines[1][1].x = rpts[idx[3]].x;
				lines[1][1].y = rpts[idx[3]].y;
				std::vector<wykobi::point2d<float>> intersection_list;
				wykobi::algorithm::naive_group_intersections<wykobi::segment<wykobi::point2d<float>, 2>>(lines.begin(), lines.end(), std::back_inserter(intersection_list));

				cv::RotatedRect rrect;
				rrect.center.x = intersection_list[0].x;
				rrect.center.y = intersection_list[0].y;
				rrect.angle = m_tag_data[m_edit_idx].m_rect.angle;
				rrect.size.width = std::hypot(rpts[idx[0]].x - rpts[idx[1]].x, rpts[idx[0]].y - rpts[idx[1]].y);
				rrect.size.height = std::hypot(rpts[idx[1]].x - rpts[idx[2]].x, rpts[idx[1]].y - rpts[idx[2]].y);
				if (m_edit_idx_side % 2 == 0) {
					std::swap(rrect.size.width, rrect.size.height);
				}
				if (this->isBoundaryViolation(rrect) == false) {
					m_tag_data[m_edit_idx].m_rect = rrect;
				} else if(m_tag_data[m_edit_idx].m_rect.angle==0){
					float x1=-1, y1=-1, x2=-1, y2=-1;
					if (rrect.center.x + rrect.size.width / 2 >= m_img.cols) {	//Right Side
						x1 = m_tag_data[m_edit_idx].m_rect.center.x - m_tag_data[m_edit_idx].m_rect.size.width / 2;
						y1 = m_tag_data[m_edit_idx].m_rect.center.y - m_tag_data[m_edit_idx].m_rect.size.height / 2;
						x2 = static_cast<float>(m_img.cols - 1);
						y2 = m_tag_data[m_edit_idx].m_rect.center.y + m_tag_data[m_edit_idx].m_rect.size.height / 2;
					} else if (rrect.center.x - rrect.size.width / 2 < 0) {		//Left Side
						x1 = 0;
						y1 = m_tag_data[m_edit_idx].m_rect.center.y - m_tag_data[m_edit_idx].m_rect.size.height / 2;
						x2 = m_tag_data[m_edit_idx].m_rect.center.x + m_tag_data[m_edit_idx].m_rect.size.width / 2;
						y2 = m_tag_data[m_edit_idx].m_rect.center.y + m_tag_data[m_edit_idx].m_rect.size.height / 2;
					} else if (rrect.center.y + rrect.size.height / 2 >= m_img.rows) {	//Bottom Side
						x1 = m_tag_data[m_edit_idx].m_rect.center.x - m_tag_data[m_edit_idx].m_rect.size.width / 2;
						x2 = m_tag_data[m_edit_idx].m_rect.center.x + m_tag_data[m_edit_idx].m_rect.size.width / 2;
						y1 = m_tag_data[m_edit_idx].m_rect.center.y - m_tag_data[m_edit_idx].m_rect.size.height / 2;
						y2 = static_cast<float>(m_img.rows - 1);
					} else if (rrect.center.y - rrect.size.height / 2 < 0) {	//Top Side
						x1 = m_tag_data[m_edit_idx].m_rect.center.x - m_tag_data[m_edit_idx].m_rect.size.width / 2;
						x2 = m_tag_data[m_edit_idx].m_rect.center.x + m_tag_data[m_edit_idx].m_rect.size.width / 2;
						y1 = 0;
						y2 = m_tag_data[m_edit_idx].m_rect.center.y + m_tag_data[m_edit_idx].m_rect.size.height / 2;
					}
					if (x1 != -1 && y1 != -1 && x2 != -1 && y2 != -1) {
						m_tag_data[m_edit_idx].m_rect.center.x=(x1+x2)/2;
						m_tag_data[m_edit_idx].m_rect.center.y = (y1 + y2) / 2;
						m_tag_data[m_edit_idx].m_rect.size.width = std::fabs(x2 - x1);
						m_tag_data[m_edit_idx].m_rect.size.height = std::fabs(y2 - y1);
					}
				}
				
			}
			return;
		} else {
			//1. 꼭지점과 거리를 먼저 검사하고, 대응되는 꼭지점이 없으면 가까운 선분을 찾는다.
			this->ClearEditState();
			wykobi::segment<float, 2> min_segment_pt[2];
			float min_dist_pt = std::numeric_limits<float>::max();
			wykobi::segment<float, 2> min_segment;
			float min_dist = std::numeric_limits<float>::max();
			CPoint point = this->GetMousePoint();
			wykobi::point2d<float> wpoint;
			wpoint.x = static_cast<float>(point.x);
			wpoint.y = static_cast<float>(point.y);
			for (int i = 0; i < m_tag_data.size(); i++) {
				auto e = m_tag_data[i];
				cv::Point2f pt[4];
				e.m_rect.points(pt);
				for (int j = 0; j < 4; j++) {
					pt[j].x = pt[j].x*m_img_rect.Width() / m_img.cols + m_img_rect.left;
					pt[j].y = pt[j].y*m_img_rect.Height() / m_img.rows + m_img_rect.top;
				}
				wykobi::segment<float, 2> segments[4];
				for (int j = 0; j < 4; j++) {
					segments[j][0].x = pt[j].x;
					segments[j][0].y = pt[j].y;
					segments[j][1].x = pt[(j + 1) % 4].x;
					segments[j][1].y = pt[(j + 1) % 4].y;
				}
				// 꼭지점을 먼저 검사한다.
				for (int j = 0; j < 4; j++) {
					float dist = std::hypot(pt[j].x - point.x, pt[j].y - point.y);
					if (dist < min_dist_pt) {
						min_dist_pt = dist;
						min_segment_pt[0] = segments[j];
						min_segment_pt[1] = segments[(j + 1) % 4];
						if (dist < 10) {
							m_apex[0].x = static_cast<LONG>(pt[(j + 2) % 4].x);
							m_apex[0].y = static_cast<LONG>(pt[(j + 2) % 4].y);
							m_apex[1].x = static_cast<LONG>(pt[j].x);
							m_apex[1].y = static_cast<LONG>(pt[j].y);
							m_edit_idx = i;
						}
					}
				}
				// 그 다음 변을 검사한다.
				for (int j = 0; j < 4; j++) {
					float dist = wykobi::minimum_distance_from_point_to_segment<float>(wpoint, segments[j]);
					if (dist < min_dist) {
						min_dist = dist;
						min_segment = segments[j];
						if (dist < 5) {
							m_edit_idx = i;
							m_edit_idx_side = j;
							m_side = segments[j];
							m_side_rect = e.m_rect;
						}
					}
				}
			}
			if (min_dist_pt < 10) {
				auto GetDegree360 = [](float x1, float y1, float x2, float y2)->int {
					int d = static_cast<int>(floor((atan2(y1 - y2, x2 - x1) * 180 / acos(-1)) + 0.5));
					return (d + 180);//각도 양수화
				};
				m_is_side = false;
				int d1 = 359 - GetDegree360(min_segment_pt[0][0].x, min_segment_pt[0][0].y, min_segment_pt[0][1].x, min_segment_pt[0][1].y);
				int d2 = 359 - GetDegree360(min_segment_pt[1][0].x, min_segment_pt[1][0].y, min_segment_pt[1][1].x, min_segment_pt[1][1].y);
				GetApp().g_degree = ((d1 + d2) / 2 + 90) % 180;
				m_is_resize = true;
			} else if (min_dist < 5) {
				m_is_side = true;
				m_is_resize = true;
				GetApp().g_degree = 179 - ispring::CVGeometry::GetDegree(cv::Point2f(min_segment[0].x, min_segment[0].y), cv::Point2f(min_segment[1].x, min_segment[1].y));
			} else {
				GetApp().g_degree = -1;
			}
			AfxGetMainWnd()->SendMessage(WM_SETCURSOR);
		}
	}
	MCALL;
}
void TagView::OnMouseLeave() {
	if (GetApp().g_exporting == true)return;
	MCALL;
}

BOOL TagView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) {
	VirtualView::OnSetCursor(pWnd, nHitTest, message);
	return TRUE;
}
void TagView::OnSize(UINT nType, int cx, int cy) {
	VirtualView::OnSize(nType, cx, cy);
}
void TagView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if (GetApp().g_exporting == true)return;
	MCALL;
	if (nChar=='d' || nChar=='D') {
		if (GetApp().g_image_data->size() != 0) {
			if (*GetApp().g_is_tracking == true) {
				this->TrackObject();
			}
		}
	}
}
void TagView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	auto IsKeyDown = [](char key)->bool {
		return ((::GetKeyState(key) & 0x8000) == 0x8000);
	};
	if (GetApp().g_exporting == true)return;
	MCALL;
	if (m_chk_edit->check == false) {	//Tag mode
		if (m_drag_point.x != -1 && m_drag_point.y != -1) {
			switch (nChar) {
				case VK_LEFT: {
					m_drag_point.x--;
				}break;
				case VK_RIGHT: {
					m_drag_point.x++;
				}break;
				case VK_UP: {
					m_drag_point.y--;
				}break;
				case VK_DOWN: {
					m_drag_point.y++;
				}break;
			}
			mspring::SetRange(m_drag_point.x, 0L, (LONG)m_img_rect.Width() - 1L);
			mspring::SetRange(m_drag_point.y, 0L, (LONG)m_img_rect.Height() - 1L);
			UpdateTagInfo();
			this->m_parent->Invalidate();
		} else {
			switch (nChar) {
				case VK_LEFT: {
					CPoint cursor;
					GetCursorPos(&cursor);
					SetCursorPos(cursor.x - 1, cursor.y);
				}break;
				case VK_RIGHT: {
					CPoint cursor;
					GetCursorPos(&cursor);
					SetCursorPos(cursor.x + 1, cursor.y);
				}break;
				case VK_UP: {
					CPoint cursor;
					GetCursorPos(&cursor);
					SetCursorPos(cursor.x, cursor.y - 1);
				}break;
				case VK_DOWN: {
					CPoint cursor;
					GetCursorPos(&cursor);
					SetCursorPos(cursor.x, cursor.y + 1);
				}break;
			}
		}
	} 
	if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000)) {
		return;
	}
	//==
	if (IsKeyDown('A')) {
		if (GetApp().g_image_data->size() != 0) {
			GetApp().g_tag_idx = static_cast<int>((GetApp().g_tag_idx - 1 + GetApp().g_image_data->size()) % GetApp().g_image_data->size());
		}
	} else if (IsKeyDown('D')) {
		if (GetApp().g_image_data->size() != 0) {
			if (*GetApp().g_is_tracking == false) {
				GetApp().g_tag_idx = (GetApp().g_tag_idx + 1) % GetApp().g_image_data->size();
			}
		}
	} else if (IsKeyDown('W')) {
		if (m_list_class->m_data.size() > 0) {
			m_list_class->m_select = static_cast<int>((m_list_class->m_select - 1 + m_list_class->m_data.size()) % m_list_class->m_data.size());
		}
	} else if (IsKeyDown('S')) {
		if (m_list_class->m_data.size() > 0) {
			m_list_class->m_select = (m_list_class->m_select + 1) % m_list_class->m_data.size();
		}
	} else if (IsKeyDown('E')) {
		m_chk_edit->check = !m_chk_edit->check;
		if (m_chk_edit->check == false) {
			m_is_edit = false;
			m_edit_idx = -1;
			GetApp().g_degree = -1;
			AfxGetMainWnd()->SendMessage(WM_SETCURSOR);
		}
	} else if (IsKeyDown('T')) {
		*GetApp().g_is_tracking = !*GetApp().g_is_tracking;
	} else if (IsKeyDown('R')) {
		int focused = this->GetFocusedTag();
		if (focused != -1) {
			int angle = -static_cast<int>(m_tag_data[focused].m_rect.angle);
			m_tag_data[focused].m_rect.angle = -static_cast<float>((angle + 90) % 360);
			std::swap(m_tag_data[focused].m_rect.size.width, m_tag_data[focused].m_rect.size.height);
			this->WriteTagFile();
		}
	} else if (IsKeyDown('C')) {
		m_chk_crosshair->check = !m_chk_crosshair->check;
	} else if (IsKeyDown('I')) {
		m_chk_showid->check = !m_chk_showid->check;
	} else if (IsKeyDown('M')) {
		m_chk_magnifier->check = !m_chk_magnifier->check;
	}
}

void TagView::OnTimer(UINT_PTR nIDEvent) {
	if (GetApp().g_exporting == true)return;
	MCALL;

	if (nIDEvent == MOUSE_LEAVE_TIMER_ID) {
		if (!(GetAsyncKeyState(VK_LBUTTON) & 0x8000)) {	//LBtn이 끝났으면
			this->OnLButtonUp(0, this->GetMousePoint());
		} else {
			this->OnMouseMove(0, this->GetMousePoint());
		}
	}

}
void TagView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if (GetApp().g_exporting == true)return;
	MCALL;
	if (m_drag_point.x != -1 && m_drag_point.y != -1) {
		return;
	}
}
LRESULT TagView::OnComposition(WPARAM wParam, LPARAM lParam) {
	if (GetApp().g_exporting == true)return 0;
	MCALL;
	return 1;
}