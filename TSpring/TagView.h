#pragma once
#include<array>

#include"mspring/view.h"
#include"mspring/control.h"
#include"ispring/All.h"
#include"repository.h"
#include"resource.h"
#include<opencv2/opencv.hpp>

#include<opencv2/tracking.hpp>
#include<wykobi/wykobi.hpp>
#include<wykobi/wykobi_math.hpp>
#include<wykobi/wykobi_algorithm.hpp>

class TagView : public VirtualView {
	const int MOUSE_LEAVE_TIMER_ID = 0x11110004;
public:
	MListBox* m_list_class;
	
	MStatic* m_stc_path;
	MStatic* m_stc_tag_info;

	MStatic* m_stc_box_class;
	MStatic* m_stc_box_center;
	MStatic* m_stc_box_size;
	MStatic* m_stc_box_angle;

	MButtonCheck* m_chk_edit;
	MStatic* m_stc_edit;

	MButtonCheck* m_chk_tracking;
	MStatic* m_stc_tracking;

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
		int& tag_idx = g_tag_idx;
		if (tag_idx + 1 >= g_image_data->size()) {
			return;
		}
		std::vector<TagInfo> prev_tag = m_tag_data;
		tag_idx = (tag_idx + 1) % g_image_data->size();
		CString& c_img_path = g_image_data->at(tag_idx).first;
		std::string img_path = mspring::String::ToString(c_img_path);
		std::string tsp_path = img_path.substr(0, img_path.find_last_of('.')) + ".tsp";
		/*if (ispring::File::FileExist(tsp_path) == true) {
			return;
		}*/
		this->ReadTagFile();

		cv::Mat prev;
		cv::cvtColor(m_img, prev, CV_BGR2GRAY);
		cv::Mat curr = cv::imread(mspring::String::ToString(g_image_data->at(tag_idx).first), cv::IMREAD_GRAYSCALE);
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
						m_tag_data.push_back(TagInfo(e.m_class, rrect));
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
				<< data.m_rect.angle << std::endl;
		}
		fout.close();
		if (m_tag_data.size() == 0) {
			ispring::File::FileErase(tsp_path);
		}

	}
	int GetFocusedTag() {
		if (g_exporting == true)return -1;
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
		rpt.x = static_cast<float>((point.x - m_img_rect.left)*m_img.cols / m_img_rect.Width());
		rpt.y = static_cast<float>((point.y - m_img_rect.top)* m_img.rows / m_img_rect.Height());
		float min_area = std::numeric_limits<float>::max();
		int ret = -1;
		for (int i = 0; i < m_tag_data.size(); i++) {
			cv::Point2f pt[4];
			m_tag_data[i].m_rect.points(pt);
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
			if (*g_is_square == true) {
				//Make rectangle to square
				float x_dist = std::fabs(cvp1.x - cvp2.x);
				float y_dist = std::fabs(cvp1.y - cvp2.y);
				float direct[2] = { 1.0,-1.0F };
				if (x_dist < y_dist) {
					cvp2.y = cvp1.y + (x_dist)* direct[cvp1.y > cvp2.y];
				} else {
					cvp2.x = cvp1.x + (y_dist)* direct[cvp1.x > cvp2.x];
				}
			}
			if (*g_is_square == true || *g_is_rectangle == true) {
				angle = 0;
			}
		}
		cv::RotatedRect rrect;
		rrect.angle = -static_cast<float>(angle);
		rrect.center.x = (cvp1.x + cvp2.x) / 2.0F;
		rrect.center.y = (cvp1.y + cvp2.y) / 2.0F;

		if (*g_is_r2pt == false && m_is_edit == false) {
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
	void WriteTagFile() {
		if (g_tag_idx < 0 || g_image_data->empty() == true) {
			return;
		}
		CString& c_img_path = g_image_data->at(g_tag_idx).first;
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
				<< data.m_rect.angle << std::endl;
		}
		fout.close();
		if (m_tag_data.size() == 0) {
			ispring::File::FileErase(tsp_path);
		}
	}
	void ReadTagFile() {
		CString& c_img_path = g_image_data->at(g_tag_idx).first;
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
			iss >> _class >> rr.center.x >> rr.center.y >> rr.size.width >> rr.size.height >> rr.angle;
			m_tag_data.push_back(TagInfo(_class, rr));
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

