#pragma once
#include<array>

#include"mspring/view.h"
#include"mspring/control.h"
#include"ispring/All.h"
#include"repository.h"

#include<wykobi/wykobi.hpp>
#include<wykobi/wykobi_algorithm.hpp>
class TagInfo {
public:
	int m_class;
	cv::RotatedRect m_rect;
	TagInfo(int _class, cv::RotatedRect _rect) {
		m_class = _class;
		m_rect = _rect;
	}
};
class TagView : public VirtualView {
	const int MOUSE_LEAVE_TIMER_ID = 0x11110004;
public:
	MListBox* m_list_class;
	MStatic* m_stc_path;
	cv::Mat m_img;
	CRect m_img_rect;
	std::vector<TagInfo> m_tag_data;
protected:
	CPoint m_drag_point;
	int m_angle = 0;
protected:
	CPoint m_r_down;

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
		cvp1.x = p1.x *m_img.cols / m_img_rect.Width();
		cvp1.y = p1.y *m_img.rows / m_img_rect.Height();
		cvp2.x = p2.x *m_img.cols / m_img_rect.Width();
		cvp2.y = p2.y *m_img.rows / m_img_rect.Height();
		if (*g_is_square == true) {
			float x_dist = std::fabs(cvp1.x - cvp2.x);
			float y_dist = std::fabs(cvp1.y - cvp2.y);
			if (x_dist < y_dist) {
				cvp2.y = cvp1.y + (cvp2.x - cvp1.x);

			} else {
				cvp2.x = cvp1.x + (cvp2.y - cvp1.y);
			}
		}
		if (*g_is_square == true || *g_is_rectangle == true) {
			angle = 0;
		}
		cv::RotatedRect rrect;
		rrect.angle = -angle;
		rrect.center.x = (cvp1.x + cvp2.x) / 2.0F;
		rrect.center.y = (cvp1.y + cvp2.y) / 2.0F;
		if (*g_is_r2pt == false) {
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

			cv::Point intersect(intersection_list[0].x, intersection_list[0].y);
			float W = std::hypot(intersect.x - cvp1.x, intersect.y - cvp1.y);
			float H = std::hypot(intersect.x - cvp2.x, intersect.y - cvp2.y);
			rrect.size.width = W;
			rrect.size.height = H;
		}
		return rrect;
	}
	void WriteTagFile() {
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
	void OnTimer(UINT_PTR nIDEvent)override;
	void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)override;
	LRESULT OnComposition(WPARAM wParam, LPARAM lParam)override;
};

