﻿#include "stdafx.h"
#include "TagView.h"

CRect DrawCvMat(CDC* pDC, cv::Mat& origin, CRect rect, int alphablend = 0, cv::RotatedRect rrect = cv::RotatedRect()) {
	CImage mfcImg;
	cv::Mat outImg;
	if (alphablend == 2) {
		outImg = origin.clone();
		cv::Rect cvrect = rrect.boundingRect();
		cv::Point2f pt[4];
		rrect.points(pt);
		cv::Vec3b color;
		color[0] = 47;
		color[1] = 50;
		color[2] = 220;
		for (int y = cvrect.y; y < cvrect.y + cvrect.height; y++) {
			for (int x = cvrect.x; x < cvrect.x + cvrect.width; x++) {
				if (ispring::CVGeometry::PtInRectangle(cv::Point(x, y), pt[0], pt[1], pt[2], pt[3]) == true) {
					outImg.at<cv::Vec3b>(y, x) = origin.at<cv::Vec3b>(y, x)*0.3 + color*0.7;
				}
			}
		}
		cv::flip(outImg, outImg, 0);
	} else if (alphablend == 1) {
		cv::cvtColor(origin, outImg, CV_BGR2GRAY);
		cv::cvtColor(outImg, outImg, CV_GRAY2BGR);
		auto MakeColorBox = [](int w, int h, COLORREF c)->cv::Mat {
			std::vector<cv::Mat> planes;
			planes.push_back(cv::Mat::zeros(h, w, CV_8UC1) + GetBValue(c));
			planes.push_back(cv::Mat::zeros(h, w, CV_8UC1) + GetGValue(c));
			planes.push_back(cv::Mat::zeros(h, w, CV_8UC1) + GetRValue(c));
			cv::Mat box;
			cv::merge(planes, box);
			return box;
		};
		cv::Mat box = MakeColorBox(outImg.cols, outImg.rows, g_lv_color_bk);
		outImg = 0.3*outImg + 0.7*box;

		cv::Rect cvrect = rrect.boundingRect();
		cv::Point2f pt[4];
		rrect.points(pt);
		for (int y = cvrect.y; y < cvrect.y + cvrect.height; y++) {
			for (int x = cvrect.x; x < cvrect.x + cvrect.width; x++) {
				if (ispring::CVGeometry::PtInRectangle(cv::Point(x, y), pt[0], pt[1], pt[2], pt[3]) == true) {
					outImg.at<cv::Vec3b>(y, x) = origin.at<cv::Vec3b>(y, x);
				}
			}
		}
		cv::flip(outImg, outImg, 0);
	} else if (alphablend == 3) {
		outImg = origin.clone();
		cv::Rect cvrect = rrect.boundingRect();
		cv::Point2f pt[4];
		rrect.points(pt);
		cv::Vec3b c(210, 139, 38);
		for (int y = cvrect.y; y < cvrect.y + cvrect.height; y++) {
			for (int x = cvrect.x; x < cvrect.x + cvrect.width; x++) {
				if (ispring::CVGeometry::PtInRectangle(cv::Point(x, y), pt[0], pt[1], pt[2], pt[3]) == true) {
					outImg.at<cv::Vec3b>(y, x) = origin.at<cv::Vec3b>(y, x)*0.3 + c*0.7;
				}
			}
		}
		cv::flip(outImg, outImg, 0);
	} else {
		cv::flip(origin, outImg, 0);
	}
	//흑백이면 채널을3으로
	if (outImg.channels() == 1) {
		cv::cvtColor(outImg, outImg, CV_GRAY2BGR);
	}
	//이미지의 크기를 rect에 들어가게 수정합니다.
	if (outImg.cols != rect.Width() || outImg.rows != rect.Height()) {
		int newWidth = outImg.cols*(rect.Height() / (double)outImg.rows);
		int newHeight = rect.Height();
		if (newWidth > rect.Width()) {
			newWidth = rect.Width();
			newHeight = outImg.rows*(rect.Width() / (double)outImg.cols);
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
	m_list_class = new MListBox(wnd, MRect(MRectPosition::R, 380, 10, 10, 300));
	m_list_class->m_color_bk = &g_lv_color_bk;
	m_list_class->m_color_text = &g_lv_color_text;

	m_stc_path = new MStatic(wnd, MRect(MRectPosition::B, 10, 30, 380, 5));
	m_stc_path->m_color_text = &g_lv_color_text;

	m_stc_tag_info = new MStatic(wnd, MRect(MRectPosition::RB, 10, 50, 380, 30));
	m_stc_tag_info->m_color_text = &g_lv_color_text;

	m_stc_box_angle = new MStatic(wnd, MRect(MRectPosition::RB, 10, 100, 380, 30));
	m_stc_box_angle->m_color_text = &g_tag_color_text_purple;

	m_stc_box_size = new MStatic(wnd, MRect(MRectPosition::RB, 10, 150, 380, 30));
	m_stc_box_size->m_color_text = &g_tag_color_text_purple;

	m_stc_box_center = new MStatic(wnd, MRect(MRectPosition::RB, 10, 200, 380, 30));
	m_stc_box_center->m_color_text = &g_tag_color_text_purple;

	m_stc_box_class = new MStatic(wnd, MRect(MRectPosition::RB, 10, 250, 380, 30));
	m_stc_box_class->m_color_text = &g_tag_color_text_purple;

	m_chk_edit = new MButtonCheck(wnd, MRect(MRectPosition::RB, 390 - 25, 10, 25, 25));
	m_chk_edit->m_color_text = &g_lv_color_text_white;
	m_stc_edit = new MStatic(wnd, MRect(MRectPosition::RB, 10, 10, 380 - 35, 25));
	m_stc_edit->m_color_text = &g_lv_color_text;
	m_stc_edit->m_text = TEXT("Edit mode(&E)");
	m_drag_point.x = -1;
	m_drag_point.y = -1;
	m_img_rect.left = -1;
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
	VirtualView::OnPaint(pDC);
	if (g_image_data->size() > 0 && g_tag_idx >= 0) {
		m_stc_path->m_text.Format(TEXT("[%d] %s"), g_tag_idx, g_image_data->at(g_tag_idx).first);
	}
	m_stc_path->OnPaint(pDC);
	m_stc_tag_info->m_text.Format(TEXT("%d box%s tagged"), m_tag_data.size(), TEXT("\0es") + (m_tag_data.size() > 1));
	m_stc_tag_info->OnPaint(pDC);
	if (m_list_class->m_select == -1 && g_class_data->size() > 0) {
		m_list_class->m_select = 0;
	}
	m_chk_edit->OnPaint(pDC);
	m_stc_edit->OnPaint(pDC);

	m_list_class->m_data.assign(g_class_data->size(), std::pair<CString, bool>());
	std::copy(g_class_data->begin(), g_class_data->end(), m_list_class->m_data.begin());
	m_list_class->OnPaint(pDC);
	CRect rect = this->GetViewRect();
	//Draw image
	CRect rect_image;
	rect_image.left = 10;
	rect_image.top = 10;
	rect_image.bottom = rect.bottom - 40;
	rect_image.right = rect.right - m_list_class->m_rect.GetRect(rect).Width() - 20;

	CBrush brush_bk; brush_bk.CreateSolidBrush(g_lv_color_bk);
	CPen pen_null; pen_null.CreatePen(PS_NULL, 0, RGB(0, 0, 0));
	CBrush* old_brush = pDC->SelectObject(&brush_bk);
	CPen* old_pen = pDC->SelectObject(&pen_null);

	static CString path = TEXT("");
	if (g_image_data->size() > g_tag_idx && path != g_image_data->at(g_tag_idx).first) {
		path = g_image_data->at(g_tag_idx).first;
		std::string cpp_path = mspring::String::ToString(path);
		m_img.release();
		m_img = cv::imread(cpp_path);
		m_img_rect.left = -1;
		ReadTagFile();
	}
	pDC->Rectangle(rect_image);



	int focused = -1;
	if (m_img.empty() == false) {		//Draw image		
		focused = GetFocusedTag();
		if (m_chk_edit->check == false) {	//Tag mode
			int will_remove = -1;
			if ((GetAsyncKeyState(VK_RBUTTON) & 0x8000)) {
				cv::Point2f rpt;
				rpt.x = (m_r_down.x - m_img_rect.left)*m_img.cols / m_img_rect.Width();
				rpt.y = (m_r_down.y - m_img_rect.top)* m_img.rows / m_img_rect.Height();
				float min_area = std::numeric_limits<float>::max();
				for (int i = 0; i < m_tag_data.size(); i++) {
					cv::Point2f pt[4];
					m_tag_data[i].m_rect.points(pt);
					if (ispring::CVGeometry::PtInRectangle(rpt, pt[0], pt[1], pt[2], pt[3]) == true) {
						if (min_area > m_tag_data[i].m_rect.size.width*m_tag_data[i].m_rect.size.height) {
							min_area = m_tag_data[i].m_rect.size.width*m_tag_data[i].m_rect.size.height;
							will_remove = i;
						}
					}
				}
				m_img_rect = DrawCvMat(pDC, m_img, rect_image, 2, m_tag_data[will_remove].m_rect);
			} else if (m_drag_point.x != -1 && m_drag_point.y != -1) {
				m_img_rect = DrawCvMat(pDC, m_img, rect_image, 1, m_tag_data.back().m_rect);
			} else {
				if (focused != -1) {
					m_img_rect = DrawCvMat(pDC, m_img, rect_image, 3, m_tag_data[focused].m_rect);
				} else {
					m_img_rect = DrawCvMat(pDC, m_img, rect_image);
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
				CPen pen; pen.CreatePen(PS_SOLID, 2, RGB(c[0], c[1], c[2]));
				CPen* old_pen = pDC->SelectObject(&pen);
				pDC->MoveTo(pt[3].x, pt[3].y);
				for (int j = 0; j < 4; j++) {
					pDC->LineTo(pt[j].x, pt[j].y);
				}
				if (e.m_class >= g_class_data->size()) {
					CPoint cpt[4];
					for (int j = 0; j < 4; j++) {
						cpt[j].x = pt[j].x;
						cpt[j].y = pt[j].y;
					}
					CBitmap bmp;
					CBrush brush; if (will_remove == i) {
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
			m_img_rect = DrawCvMat(pDC, m_img, rect_image);
			for (int i = 0; i < m_tag_data.size(); i++) {
				auto e = m_tag_data[i];
				cv::Point2f pt[4];
				e.m_rect.points(pt);
				for (int j = 0; j < 4; j++) {
					pt[j].x = pt[j].x*m_img_rect.Width() / m_img.cols + m_img_rect.left;
					pt[j].y = pt[j].y*m_img_rect.Height() / m_img.rows + m_img_rect.top;
				}
				auto c = ispring::CV::GetRGB(e.m_class);
				CPen pen; pen.CreatePen(PS_SOLID, 2, RGB(c[0], c[1], c[2]));
				CPen* old_pen = pDC->SelectObject(&pen);
				pDC->MoveTo(pt[3].x, pt[3].y);
				for (int j = 0; j < 4; j++) {
					pDC->LineTo(pt[j].x, pt[j].y);
				}
				pDC->SelectObject(old_pen);
				pen.DeleteObject();
			}

		}
	} else {
		m_img_rect = CRect(0, 0, 0, 0);
	}
	DrawAlphaBlend(pDC);


	pDC->SelectObject(old_pen);
	pDC->SelectObject(old_brush);
	if (focused != -1) {
		if (m_tag_data[focused].m_class >= m_list_class->m_data.size()) {
			m_stc_box_class->m_text.Format(TEXT("class : Unknown(%d)"), m_tag_data[focused].m_class);
			m_stc_box_class->m_color_text = &g_tag_color_text_red;
		} else {
			m_stc_box_class->m_text.Format(TEXT("class : %s(%d)"), m_list_class->m_data[m_tag_data[focused].m_class].first, m_tag_data[focused].m_class);
			m_stc_box_class->m_color_text = &g_tag_color_text_purple;
		}
		m_stc_box_center->m_text.Format(TEXT("X : %f , Y : %f"), m_tag_data[focused].m_rect.center.x, m_tag_data[focused].m_rect.center.y);
		m_stc_box_size->m_text.Format(TEXT("W : %f , H : %f"), m_tag_data[focused].m_rect.size.width, m_tag_data[focused].m_rect.size.height);
		m_stc_box_angle->m_text.Format(TEXT("Angle : %f"), -m_tag_data[focused].m_rect.angle);
		m_stc_box_class->OnPaint(pDC);
		m_stc_box_center->OnPaint(pDC);
		m_stc_box_size->OnPaint(pDC);
		m_stc_box_angle->OnPaint(pDC);
	}
	
}
void TagView::OnSetFocus(CWnd* pOldWnd) {
	VirtualView::OnSetFocus(pOldWnd);
}
void TagView::OnKillFocus(CWnd* pNewWnd) {
	VirtualView::OnKillFocus(pNewWnd);
}

void TagView::OnLButtonDown(UINT nFlags, CPoint point) {
	VirtualView::OnLButtonDown(nFlags, point);
	if (m_chk_edit->OnLButtonDown() == M_CLICKED) {
		return;
	}
	if (m_chk_edit->check == false) {	//Tag mode
		m_list_class->OnLButtonDown();
		point = this->GetMousePoint();
		if ((GetAsyncKeyState(VK_RBUTTON) & 0x8000)) {
			return;
		}
		if (m_list_class->m_select == -1) {
			this->m_parent->MessageBox(TEXT("Please, make class list before tagging"));
			return;
		}
		if (m_list_class->m_select != -1 && m_img_rect.PtInRect(point)) {
			m_drag_point = point;
			m_angle = 0;
			cv::RotatedRect rrect = this->GetRotatedRect(m_drag_point, m_drag_point, m_angle);

			m_tag_data.push_back(TagInfo(m_list_class->m_select, rrect));
		}
	} else {
		m_is_edit = true;
	}
}
void TagView::OnLButtonUp(UINT nFlags, CPoint point) {
	VirtualView::OnLButtonUp(nFlags, point);
	m_list_class->OnLButtonUp();
	if (m_chk_edit->check == false) {	//Tag mode
		m_drag_point.x = -1;
		m_drag_point.y = -1;
		if (m_tag_data.empty() == false) {
			if (m_tag_data.back().m_rect.size.width < 32 || m_tag_data.back().m_rect.size.height < 32) {
				m_tag_data.pop_back();
			}
		}
	} else {
		m_is_edit = false;
		m_edit_idx = -1;
	}
	WriteTagFile();
}
void TagView::OnLButtonDblClk(UINT nFlags, CPoint point) {
	VirtualView::OnLButtonDblClk(nFlags, point);

}
void TagView::OnRButtonDown(UINT nFlags, CPoint point) {
	VirtualView::OnRButtonDown(nFlags, point);
	if (m_chk_edit->check == false) {	//Tag mode
		m_r_down = this->GetMousePoint();
	} else {

	}
	this->m_parent->Invalidate();
}
void TagView::OnRButtonUp(UINT nFlags, CPoint point) {
	//SetCursor(LoadCursor(NULL, IDC_ARROW));
	VirtualView::OnRButtonUp(nFlags, point);
	m_list_class->OnRButtonUp();
	if (m_tag_data.size() == 0) {
		return;
	}
	if (m_chk_edit->check == false) {	//Tag mode
		cv::Point2f rpt;
		rpt.x = (m_r_down.x - m_img_rect.left)*m_img.cols / m_img_rect.Width();
		rpt.y = (m_r_down.y - m_img_rect.top)* m_img.rows / m_img_rect.Height();
		float min_area = std::numeric_limits<float>::max();
		int idx = -1;
		for (size_t i = 0; i < m_tag_data.size(); i++) {
			cv::Point2f pt[4];
			m_tag_data[i].m_rect.points(pt);
			if (ispring::CVGeometry::PtInRectangle(rpt, pt[0], pt[1], pt[2], pt[3]) == true) {
				if (min_area > m_tag_data[i].m_rect.size.width*m_tag_data[i].m_rect.size.height) {
					min_area = m_tag_data[i].m_rect.size.width*m_tag_data[i].m_rect.size.height;
					idx = (int)i;
				}
			}
		}
		if (idx != -1) {
			m_tag_data.erase(m_tag_data.begin() + idx);
		}
		m_r_down.x = -1;
		m_r_down.y = -1;
	} else {

	}
	WriteTagFile();
}
BOOL TagView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) {
	VirtualView::OnMouseWheel(nFlags, zDelta, pt);
	m_list_class->OnMouseWheel(zDelta);
	CPoint point = this->GetMousePoint();
	if (m_chk_edit->check == false) {	//Tag mode
		if (m_drag_point.x != -1 && m_drag_point.y != -1) {	//if Draging
			if (*g_is_rmid == true || *g_is_r2pt == true) {
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
	} else {

	}
	return TRUE;
}
void TagView::OnMouseMove(UINT nFlags, CPoint point) {
	VirtualView::OnMouseMove(nFlags, point);
	m_chk_edit->OnMouseMove();
	m_list_class->OnMouseMove();
	if (m_chk_edit->check == false) {	//Tag mode
		if (m_drag_point.x != -1 && m_drag_point.y != -1) {
			UpdateTagInfo();
		}
		if ((GetAsyncKeyState(VK_RBUTTON) & 0x8000)) {
			m_r_down = this->GetMousePoint();
		}
	} else {
		if (m_is_edit == true) {
			if (m_edit_idx == -1) {
				return;
			}
			m_apex[1] = this->GetMousePoint();
			cv::RotatedRect rrect = this->GetRotatedRect(m_apex[0], m_apex[1], -m_tag_data[m_edit_idx].m_rect.angle);
			cv::Point2f pt[4];
			rrect.points(pt);
			bool contain = true;
			for (auto&p : pt) {
				contain &= cv::Rect(0, 0, m_img.cols, m_img.rows).contains(p);
			}
			if (contain == true) {
				m_tag_data[m_edit_idx].m_rect = rrect;
			}
			return;
		} else {
			//1. 꼭지점과 거리를 먼저 검사하고, 대응되는 꼭지점이 없으면 가까운 선분을 찾는다.
			m_edit_idx = -1;
			wykobi::segment<float, 2> min_segment_pt[2];
			float min_dist_pt = std::numeric_limits<float>::max();
			wykobi::segment<float, 2> min_segment;
			float min_dist = std::numeric_limits<float>::max();
			CPoint point = this->GetMousePoint();
			wykobi::point2d<float> wpoint;
			wpoint.x = point.x;
			wpoint.y = point.y;
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
					float dist = wykobi::minimum_distance_from_point_to_segment<float>(wpoint, segments[j]);
					if (dist < min_dist) {
						min_dist = dist;
						min_segment = segments[j];
					}
				}
				for (int j = 0; j < 4; j++) {
					float dist = std::hypot(pt[j].x - point.x, pt[j].y - point.y);
					if (dist < min_dist_pt) {
						min_dist_pt = dist;
						min_segment_pt[0] = segments[j];
						min_segment_pt[1] = segments[(j + 1) % 4];
						if (dist < 10) {
							m_apex[0].x = pt[(j + 2) % 4].x;
							m_apex[0].y = pt[(j + 2) % 4].y;
							m_apex[1].x = pt[j].x;
							m_apex[1].y = pt[j].y;
							m_edit_idx = i;
						}
					}
				}
			}
			if (min_dist_pt < 10) {
				auto GetDegree360 = [](int x1, int y1, int x2, int y2)->int {
					int d = static_cast<int>(floor((atan2(y1 - y2, x2 - x1) * 180 / acos(-1)) + 0.5));
					return (d + 180);//각도 양수화
				};

				int d1 = 359 - GetDegree360(min_segment_pt[0][0].x, min_segment_pt[0][0].y, min_segment_pt[0][1].x, min_segment_pt[0][1].y);
				int d2 = 359 - GetDegree360(min_segment_pt[1][0].x, min_segment_pt[1][0].y, min_segment_pt[1][1].x, min_segment_pt[1][1].y);
				g_degree = ((d1 + d2) / 2 + 90) % 180;
			} else if (min_dist < 5) {
				g_degree = 179 - ispring::CVGeometry::GetDegree(cv::Point(min_segment[0].x, min_segment[0].y), cv::Point(min_segment[1].x, min_segment[1].y));
			} else {
				g_degree = -1;
			}
			AfxGetMainWnd()->SendMessage(WM_SETCURSOR);
		}
	}
}
void TagView::OnMouseLeave() {
	VirtualView::OnMouseLeave();
	m_list_class->OnMouseLeave();
	if (m_chk_edit->check == false) {	//Tag mode
		if (m_drag_point.x != -1 && m_drag_point.y != -1) {
			this->m_parent->SetTimer(MOUSE_LEAVE_TIMER_ID, 10, nullptr);
		}
	} else {

	}
}

BOOL TagView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) {
	VirtualView::OnSetCursor(pWnd, nHitTest, message);
	return TRUE;
}
void TagView::OnSize(UINT nType, int cx, int cy) {
	VirtualView::OnSize(nType, cx, cy);
}

void TagView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	VirtualView::OnKeyDown(nChar, nRepCnt, nFlags);
	m_list_class->OnKeyDown(nChar);
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
		}
	}

}

void TagView::OnTimer(UINT_PTR nIDEvent) {
	VirtualView::OnTimer(nIDEvent);
	m_list_class->OnTimer(nIDEvent);
	if (m_chk_edit->check == false) {	//Tag mode
		if (nIDEvent == MOUSE_LEAVE_TIMER_ID) {
			if ((GetAsyncKeyState(VK_RBUTTON) & 0x8000)) {
				m_r_down.x = -1;
				m_r_down.y = -1;
			}
			if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000)) {
				m_drag_point.x = -1;
				m_drag_point.y = -1;
				if (m_tag_data.back().m_rect.size.width < 32 || m_tag_data.back().m_rect.size.height < 32) {
					m_tag_data.pop_back();
				}
				m_parent->KillTimer(MOUSE_LEAVE_TIMER_ID);
				WriteTagFile();
				return;
			} else {
				UpdateTagInfo();
			}
		}
	}
}
void TagView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) {
	VirtualView::OnChar(nChar, nRepCnt, nFlags);
	m_list_class->OnChar(nChar);
	if (m_drag_point.x != -1 && m_drag_point.y != -1) {
		return;
	}
	if (nChar == 'a' || nChar == 'A') {
		if (g_image_data->size() != 0) {
			g_tag_idx = (g_tag_idx - 1 + g_image_data->size()) % g_image_data->size();
		}
	} else if (nChar == 'd' || nChar == 'D') {
		if (g_image_data->size() != 0) {
			g_tag_idx = (g_tag_idx + 1) % g_image_data->size();
		}
	} else if (nChar == 'w' || nChar == 'W') {
		if (m_list_class->m_data.size() > 0) {
			m_list_class->m_select = (m_list_class->m_select - 1 + m_list_class->m_data.size()) % m_list_class->m_data.size();
		}
	} else if (nChar == 's' || nChar == 'S') {
		if (m_list_class->m_data.size() > 0) {
			m_list_class->m_select = (m_list_class->m_select + 1) % m_list_class->m_data.size();
		}
	} else if (nChar == 'e' || nChar == 'E') {
		m_chk_edit->check = !m_chk_edit->check;
	}
}
LRESULT TagView::OnComposition(WPARAM wParam, LPARAM lParam) {
	VirtualView::OnComposition(wParam, lParam);
	return 1;
}