#pragma once
#include<array>

#include"MSpring/MSpring.h"
#include"ispring/All.h"
#include"resource.h"
#include<opencv2/opencv.hpp>

#include<opencv2/tracking.hpp>
#include<wykobi/wykobi.hpp>
#include<wykobi/wykobi_math.hpp>
#include<wykobi/wykobi_algorithm.hpp>
#include"Theme.h"
#include"InputBox.h"
#include"TSpringMsgBox.h"
class TagView : public VirtualView {
	const int MOUSE_LEAVE_TIMER_ID = 0x11110004;
public:
	MListBox m_list_class;
	
	MStatic m_stc_path;
	MStatic m_stc_tag_info;

	MStatic m_stc_box_class;
	MStatic m_stc_box_coord;
	MStatic m_stc_box_angle;

	MButton m_btn_pen_style;
	//enum PenStyle {
	//	Light, Heavy, Thin
	//}
	int m_pen_style = 0;

	MButtonCheck m_chk_crosshair;
	MStatic m_stc_crosshair;

	MButtonCheck m_chk_magnifier;
	MStatic m_stc_magnifier;

	MButtonCheck m_chk_showid;
	MStatic m_stc_showid;

	MButtonCheck m_chk_edit;
	MStatic m_stc_edit;

	MButtonCheck m_chk_tracking;
	MStatic m_stc_tracking;

	cv::Mat m_img;
	CRect m_img_rect;
	std::vector<TagInfo> m_tag_data;

protected:
	CPoint m_drag_point;
	int m_angle = 0;
protected:
	CPoint m_r_down;
	CPoint m_l_down;
protected:
	void ClearEditState() {
		m_is_edit = false;
		m_is_side = false;
		m_edit_idx = -1;
		m_edit_idx_side = -1;
		m_move_base.x = m_move_base.y = -1;
		m_is_resize = false;
	}
	bool m_is_edit = false;
	bool m_is_resize = false;
	bool m_is_side = false;
	CPoint m_apex[2];
	int m_edit_idx = -1;

	cv::RotatedRect m_side_rect;
	wykobi::segment<float, 2> m_side;
	int m_edit_idx_side = -1;
	
	cv::Point2f m_move_base;
protected:
	void TrackObject() {
		auto& tag_idx = GetApp().g_tag_idx;
		if (tag_idx + 1 >= GetApp().g_image_data->size()) {
			return;
		}
		std::vector<TagInfo> prev_tag = m_tag_data;
		tag_idx = (tag_idx + 1) % GetApp().g_image_data->size();
		TString& c_img_path = GetApp().g_image_data->at(tag_idx).first;
		std::string img_path = mspring::String::ToString(c_img_path);
		std::string tsp_path = img_path.substr(0, img_path.find_last_of('.')) + ".tsp";
		this->ReadTagFile();

		cv::Mat prev;
		cv::cvtColor(m_img, prev, CV_BGR2GRAY);
		cv::Mat curr = cv::imread(mspring::String::ToString(GetApp().g_image_data->at(tag_idx).first), cv::IMREAD_GRAYSCALE);
		auto GetMaxIOU = [](cv::Rect2d a, std::vector<TagInfo> dst)->double {
			double iou = 0.0F;
			for (auto&e : dst) {
				cv::Rect2f r(e.m_rect.center.x - e.m_rect.size.width / 2, e.m_rect.center.y - e.m_rect.size.height / 2, e.m_rect.size.width, e.m_rect.size.height);
				double iou_local = ispring::CVEval::IOU(a, r);
				if (iou < iou_local) {
					iou = iou_local;
				}
			}
			return iou;
		};
		auto ToInnerRect = [](cv::Rect2d a, cv::Mat& img) ->cv::Rect2d{
			cv::Point2d p1(a.x, a.y);
			cv::Point2d p2(a.x+a.width, a.y+a.height);
			mspring::SetRange(p1.x,0.0, img.cols-1.0);
			mspring::SetRange(p1.y, 0.0, img.rows-1.0);
			mspring::SetRange(p2.x, 0.0, img.cols-1.0);
			mspring::SetRange(p2.y, 0.0, img.rows-1.0);
			return cv::Rect2d(p1.x, p1.y, p2.x - p1.x, p2.y - p1.y);
		};
		for(size_t i=0;i<prev_tag.size();i++){
			auto&e = prev_tag[i];
			cv::Rect2d rect;
			rect.x = e.m_rect.center.x - e.m_rect.size.width / 2;
			rect.y = e.m_rect.center.y - e.m_rect.size.height / 2;
			rect.width = e.m_rect.size.width;
			rect.height = e.m_rect.size.height;
			cv::Ptr<cv::Tracker> tracker = cv::TrackerMedianFlow::create();
			tracker->init(prev, rect);
			if (e.m_rect.angle == 0) {
				if (tracker->update(curr, rect) == true) {
					if (GetMaxIOU(rect, m_tag_data) < 0.5) {
						rect=ToInnerRect(rect, curr);
						cv::RotatedRect rrect;
						rrect.center.x = static_cast<float>(rect.x + rect.width / 2);
						rrect.center.y = static_cast<float>(rect.y + rect.height / 2);
						rrect.size.width = static_cast<float>(rect.width);
						rrect.size.height = static_cast<float>(rect.height);
						rrect.angle = 0;
						m_tag_data.push_back(TagInfo(e.m_class, rrect,e.m_id));
					}
				}
			}
		}
		
		std::fstream fout;
		fout.open(tsp_path, std::ios::out);
		if (fout.is_open() == false) {
			ISPRING_VERIFY("Can't open " + tsp_path);
			return;
		}
		for (auto& data : m_tag_data) {
			fout << data.m_class << "\t"
				<< data.m_rect.center.x << "\t"
				<< data.m_rect.center.y << "\t"
				<< data.m_rect.size.width << "\t"
				<< data.m_rect.size.height << "\t"
				<< data.m_rect.angle << "\t" 
				<< data.m_id << std::endl;
		}
		fout.close();
		if (m_tag_data.size() == 0) {
			ispring::File::FileErase(tsp_path);
		}

	}
	std::vector<int> GetPreviousIDs() {
		std::set<int> ids;
		std::vector<int> idv;
		if (GetApp().g_tag_idx - 1 < 0) {
			return idv;
		}
		TString& c_img_path = GetApp().g_image_data->at(GetApp().g_tag_idx - 1).first;
		std::string img_path = mspring::String::ToString(c_img_path);
		std::string tsp_path = img_path.substr(0, img_path.find_last_of('.')) + ".tsp";
		std::fstream fin;
		fin.open(tsp_path, std::ios::in);
		if (fin.is_open() == false) {
			return idv;
		}
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
			int id;
			iss >> _class >> rr.center.x >> rr.center.y >> rr.size.width >> rr.size.height >> rr.angle >> id;
			ids.insert(id);
		}
		for (auto&e : ids) {
			idv.push_back(e);
		}
		return idv;
	}
	int GetFocusedTag(bool id=false) {
		if (GetApp().g_exporting == true)return -1;
		if (m_tag_data.size() == 0) {
			return -1;
		}
		if (m_img.empty() == true) {
			return -1;
		}
		if (m_img_rect.left == -1) {
			return -1;
		}
		if (m_drag_point.x != -1 && m_drag_point.y != -1) {
			return static_cast<int>(m_tag_data.size()) - 1;
		}
		if (m_edit_idx != -1) {
			return m_edit_idx;
		}
		CPoint point = this->GetMousePoint();
		cv::Point2f rpt;
		if (id == false) {
			rpt.x = static_cast<float>((point.x - m_img_rect.left)*m_img.cols / m_img_rect.Width());
			rpt.y = static_cast<float>((point.y - m_img_rect.top)* m_img.rows / m_img_rect.Height());
		} else {
			rpt.x = static_cast<float>(point.x);
			rpt.y = static_cast<float>(point.y);
		}
		float min_area = std::numeric_limits<float>::max();
		int ret = -1;
		for (int i = 0; i < m_tag_data.size(); i++) {
			cv::Point2f pt[4];
			if (id==false) {
				m_tag_data[i].m_rect.points(pt);
			} else {
				m_tag_data[i].m_id_rect.points(pt);
			}
			if (ispring::CVGeometry::PtInRectangle(rpt, pt[0], pt[1], pt[2], pt[3]) == true) {
				if (min_area > m_tag_data[i].m_rect.size.width*m_tag_data[i].m_rect.size.height) {
					min_area = m_tag_data[i].m_rect.size.width*m_tag_data[i].m_rect.size.height;
					ret = i;
				}
			}
		}
		return ret;
	}
	void UpdateTagInfo() {
		CPoint point = this->GetMousePoint();
		cv::RotatedRect rrect;
		rrect = this->GetRotatedRect(m_drag_point, point, m_angle);
		cv::Point2f pt[4];
		rrect.points(pt);
		bool contain = true;
		for (auto&p : pt) {
			contain &= cv::Rect(0, 0, m_img.cols, m_img.rows).contains(p);
		}
		if (contain == true) {
			m_tag_data.back().m_rect = rrect;
		}
	}
	cv::RotatedRect GetRotatedRect(CPoint p1, CPoint p2, int angle = 0) {
		p1.x -= m_img_rect.left;
		p1.y -= m_img_rect.top;
		p2.x -= m_img_rect.left;
		p2.y -= m_img_rect.top;
		mspring::SetRange(p1.x, 0L, static_cast<LONG>(m_img_rect.Width() - 1));
		mspring::SetRange(p1.y, 0L, static_cast<LONG>(m_img_rect.Height() - 1));
		mspring::SetRange(p2.x, 0L, static_cast<LONG>(m_img_rect.Width() - 1));
		mspring::SetRange(p2.y, 0L, static_cast<LONG>(m_img_rect.Height() - 1));
		cv::Point2f cvp1, cvp2;	//실제 이미지의 두 대각 점
		cvp1.x = static_cast<float>(p1.x *m_img.cols / m_img_rect.Width());
		cvp1.y = static_cast<float>(p1.y *m_img.rows / m_img_rect.Height());
		cvp2.x = static_cast<float>(p2.x *m_img.cols / m_img_rect.Width());
		cvp2.y = static_cast<float>(p2.y *m_img.rows / m_img_rect.Height());
		if (m_is_edit == false) {
			if (*GetApp().g_is_rectangle == true) {
				angle = 0;
			}
		}
		cv::RotatedRect rrect;
		rrect.angle = -static_cast<float>(angle);
		rrect.center.x = (cvp1.x + cvp2.x) / 2.0F;
		rrect.center.y = (cvp1.y + cvp2.y) / 2.0F;

		if (*GetApp().g_is_r2pt == false && m_is_edit == false) {
			rrect.size.width = std::fabs(cvp1.x - cvp2.x);
			rrect.size.height = std::fabs(cvp1.y - cvp2.y);
		} else {
			float rad = -static_cast<float>(angle*CV_PI / 180);
			cv::Point2f pt(cvp2.x, cvp1.y);
			cv::Point2f af1;
			af1.x = (pt.x - cvp1.x)*cos(rad) - (pt.y - cvp1.y)*sin(rad) + cvp1.x;
			af1.y = (pt.x - cvp1.x)*sin(rad) + (pt.y - cvp1.y)*cos(rad) + cvp1.y;
			cv::Point2f af2;
			af2.x = (pt.x - cvp2.x)*cos(rad) - (pt.y - cvp2.y)*sin(rad) + cvp2.x;
			af2.y = (pt.x - cvp2.x)*sin(rad) + (pt.y - cvp2.y)*cos(rad) + cvp2.y;

			std::vector<wykobi::line<float, 2>> lines(2);
			lines[0][0].x = cvp1.x;
			lines[0][0].y = cvp1.y;
			lines[0][1].x = af1.x;
			lines[0][1].y = af1.y;
			lines[1][0].x = cvp2.x;
			lines[1][0].y = cvp2.y;
			lines[1][1].x = af2.x;
			lines[1][1].y = af2.y;

			std::vector<wykobi::point2d<float>> intersection_list;
			wykobi::algorithm::naive_group_intersections<wykobi::segment<wykobi::point2d<float>, 2>>
				(lines.begin(), lines.end(), std::back_inserter(intersection_list));
			if (intersection_list.size() > 0) {
				cv::Point2f intersect(intersection_list[0].x, intersection_list[0].y);
				float W = std::hypot(intersect.x - cvp1.x, intersect.y - cvp1.y);
				float H = std::hypot(intersect.x - cvp2.x, intersect.y - cvp2.y);
				rrect.size.width = W;
				rrect.size.height = H;
			}
		}
		return rrect;
	}
	bool isBoundaryViolation(cv::RotatedRect rrect) {
		cv::Point2f pt[4];
		rrect.points(pt);
		bool contain = true;
		for (auto&p : pt) {
			contain &= cv::Rect(0, 0, m_img.cols, m_img.rows).contains(p);
		}
		return !contain;
	}
	cv::RotatedRect SetRectInBoundary(cv::RotatedRect rrect) {
		cv::Point2f pts[4];
		rrect.points(pts);
		//4방향 검사
		float minx = FLT_MAX;
		float miny = FLT_MAX;
		float maxx = -FLT_MAX;
		float maxy = -FLT_MAX;
		for (int i = 0; i < 4; i++) {
			minx = mspring::Min(minx,pts[i].x);
			miny = mspring::Min(miny, pts[i].y);
			maxx = mspring::Max(maxx, pts[i].x);
			maxy = mspring::Max(maxy, pts[i].y);
		}
		if (minx < 0.0F) {
			rrect.center.x += -minx;
		}
		if (miny < 0.0F) {
			rrect.center.y += -miny;
		}
		if (maxx >= m_img.cols) {
			rrect.center.x -= (maxx - m_img.cols-1);
		}
		if (maxy >= m_img.rows) {
			rrect.center.y -= (maxy - m_img.rows - 1);
		}
		return rrect;
	}
	void DrawCrosshair(CDC* pDC,CRect rect) {
		CPoint point = this->GetMousePoint();
		if (rect.PtInRect(point)) {
			CPen pen;
			pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
			CPen* old_pen = pDC->SelectObject(&pen);
			
			pDC->MoveTo(rect.left, point.y);
			pDC->LineTo(rect.right, point.y);
			pDC->MoveTo(point.x, rect.top);
			pDC->LineTo(point.x, rect.bottom);

			pDC->SelectObject(old_pen);
			pen.DeleteObject();

			pen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
			old_pen = pDC->SelectObject(&pen);

			pDC->MoveTo(mspring::Max(point.x-100,rect.left), point.y);
			pDC->LineTo(mspring::Min(point.x+100,rect.right), point.y);
			pDC->MoveTo(point.x, mspring::Max(point.y-100,rect.top));
			pDC->LineTo(point.x, mspring::Min(point.y+100,rect.bottom));

			pDC->SelectObject(old_pen);
			pen.DeleteObject();
		}
	}
	void DrawLens(CDC* pDC, CRect rect) {
		auto DrawMat = [](CDC* pDC, cv::Mat img,CRect rect)->void {
			cv::flip(img, img, 0);
			//흑백이면 채널을3으로
			if (img.channels() == 1) {
				cv::cvtColor(img, img, CV_GRAY2BGR);
			}
			if (img.cols != rect.Width() || img.rows != rect.Height()) {
				int newWidth = static_cast<int>(img.cols*(rect.Height() / (double)img.rows));
				int newHeight = rect.Height();
				if (newWidth > rect.Width()) {
					newWidth = rect.Width();
					newHeight = static_cast<int>(img.rows*(rect.Width() / (double)img.cols));
				}
				cv::resize(img, img, cv::Size(newWidth, newHeight), 0, 0, CV_INTER_NN);
			}
			CImage mfcImg;
			mfcImg.Create(img.cols, img.rows, 24);
			BITMAPINFO bitInfo = { { sizeof(BITMAPINFOHEADER),img.cols,img.rows,1,24 },0 };

			void* vp = img.data;
			IplImage* iplimage = nullptr;
			if (img.cols % 4 != 0 && img.isContinuous()) {
				iplimage = cvCreateImage(img.size(), 8, img.channels());
				img.copyTo(cv::cvarrToMat(iplimage));
				vp = iplimage->imageData;
			}
			pDC->SetStretchBltMode(HALFTONE);
			StretchDIBits(mfcImg.GetDC(), 0, 0, img.cols, img.rows, 0, 0, img.cols, img.rows, vp, &bitInfo, DIB_RGB_COLORS, SRCCOPY);
			if (iplimage != nullptr) {
				cvReleaseImage(&iplimage);
			}
			mfcImg.BitBlt(*pDC, (rect.Width() - img.cols) / 2 + rect.left, (rect.Height() - img.rows) / 2 + rect.top);
			mfcImg.ReleaseDC();
		};
		CPoint point = this->GetMousePoint();
		if (rect.PtInRect(point)) {
			CPoint rpoint = point;
			cv::Mat img = m_img.clone();
			cv::resize(img, img, cv::Size(rect.Width(), rect.Height()));
			rpoint.x -= rect.left;
			rpoint.y -= rect.top;
			cv::line(img, cv::Point(0, rpoint.y), cv::Point(img.cols, rpoint.y), cv::Scalar(0, 0, 255), 1);
			cv::line(img, cv::Point(rpoint.x, 0), cv::Point(rpoint.x, img.rows), cv::Scalar(0, 0, 255), 1);

			int size = 50;
			if (point.x-size < rect.left) {
				point.x = rect.left+size;
			}
			if (point.x + size >= rect.right) {
				point.x = rect.right - size - 1;
			}
			if (point.y - size < rect.top) {
				point.y = rect.top+size;
			}
			if (point.y + size >= rect.bottom) {
				point.y = rect.bottom - size - 1;
			}
			point.x -= rect.left;
			point.y -= rect.top;
			
			cv::Mat crop = img(cv::Rect(point.x - size, point.y - size, size * 2, size * 2));
			CRect drawrect;
			size *= 2;
			if (point.x < (rect.left + rect.right) / 2) {	//좌측
				if (point.y < (rect.top + rect.bottom) / 2) {		//좌측상단->우측 하단
					drawrect = CRect(rect.right - size * 2, rect.bottom - size * 2, rect.right, rect.bottom);
				} else {												//좌측하단->우측상단
					drawrect = CRect(rect.right - size * 2, rect.top, rect.right, rect.top + size * 2);
				}
			} else {											//우측
				if (point.y < (rect.top + rect.bottom) / 2) {		//우측상단->좌측하단
					drawrect=CRect(rect.left, rect.bottom - size * 2, rect.left+size*2, rect.bottom);
				} else {												//우측하단->좌측상단
					drawrect= CRect(rect.left, rect.top, rect.left + size * 2, rect.top + size * 2);
				}
			}
			cv::Mat draw = crop.clone();
			DrawMat(pDC, draw, drawrect);
		}
	}
	void WriteTagFile() {
		if (GetApp().g_tag_idx < 0 || GetApp().g_image_data->empty() == true) {
			return;
		}
		TString& c_img_path = GetApp().g_image_data->at(GetApp().g_tag_idx).first;
		std::string img_path = mspring::String::ToString(c_img_path);
		std::string tsp_path = img_path.substr(0, img_path.find_last_of('.')) + ".tsp";
		std::fstream fout;
		fout.open(tsp_path, std::ios::out);
		if (fout.is_open() == false) {
			ISPRING_VERIFY("Can't open " + tsp_path);
			return;
		}
		for (auto& data : m_tag_data) {
			fout << data.m_class << "\t"
				<< data.m_rect.center.x << "\t"
				<< data.m_rect.center.y << "\t"
				<< data.m_rect.size.width << "\t"
				<< data.m_rect.size.height << "\t"
				<< data.m_rect.angle << "\t"
				<< data.m_id << std::endl;
		}
		fout.close();
		if (m_tag_data.size() == 0) {
			ispring::File::FileErase(tsp_path);
		}
	}
	void ReadTagFile() {
		TString& c_img_path = GetApp().g_image_data->at(GetApp().g_tag_idx).first;
		std::string img_path = mspring::String::ToString(c_img_path);
		std::string tsp_path = img_path.substr(0, img_path.find_last_of('.')) + ".tsp";
		std::fstream fin;
		fin.open(tsp_path, std::ios::in);
		m_tag_data.clear();
		if (fin.is_open() == false) {
			return;
		}
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
			int _id = -1;
			iss >> _class >> rr.center.x >> rr.center.y >> rr.size.width >> rr.size.height >> rr.angle >> _id;
			m_tag_data.push_back(TagInfo(_class, rr,_id));
		}
	}

public:
	TagView(CWnd* wnd);
	~TagView();
public:
	int OnCreate()override;
	void OnDestroy()override;
	void DrawAlphaBlend(CDC* pDC) {

	}
	void OnPaint(CDC* pDC)override;
	void OnSetFocus(CWnd* pOldWnd)override;
	void OnKillFocus(CWnd* pNewWnd)override;

	void OnLButtonDown(UINT nFlags, CPoint point)override;
	void OnLButtonUp(UINT nFlags, CPoint point)override;
	void OnLButtonDblClk(UINT nFlags, CPoint point)override;
	void OnRButtonUp(UINT nFlags, CPoint point)override;
	void OnRButtonDown(UINT nFlags, CPoint point)override;
	BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)override;
	void OnMouseMove(UINT nFlags, CPoint point)override;
	void OnMouseLeave()override;

	BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)override;
	void OnSize(UINT nType, int cx, int cy)override;
	
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)override;
	void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)override;
	void OnTimer(UINT_PTR nIDEvent)override;
	void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)override;
	LRESULT OnComposition(WPARAM wParam, LPARAM lParam)override;
};