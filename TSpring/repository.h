#pragma once
#include<vector>
#include<afxstr.h>
#include<opencv2/opencv.hpp>
#include"stdafx.h"
///List View
__declspec(selectany) std::vector<std::pair<CString, bool>>* g_image_data;
__declspec(selectany) std::vector<std::pair<CString, bool>>* g_class_data;
__declspec(selectany) bool* g_is_detection;
__declspec(selectany) bool* g_is_segmentation;
__declspec(selectany) bool* g_is_square;
__declspec(selectany) bool* g_is_rectangle;
__declspec(selectany) bool* g_is_rmid;
__declspec(selectany) bool* g_is_r2pt;
__declspec(selectany) bool* g_is_tracking;


__declspec(selectany) std::atomic<COLORREF> g_lv_color_bk = RGB(0, 43, 54);
__declspec(selectany) std::atomic<COLORREF> g_lv_color_text = RGB(199, 199, 199);
__declspec(selectany) std::atomic<COLORREF> g_lv_color_text_white = RGB(255, 255, 255);
__declspec(selectany) std::atomic<COLORREF> g_lv_color_border = RGB(211, 54, 130);

__declspec(selectany) int g_command_go2tagging = -1;

class TagInfo {
public:
	int m_class;
	cv::RotatedRect m_rect;
	TagInfo(int _class, cv::RotatedRect _rect) {
		m_class = _class;
		m_rect = _rect;
	}
};
inline void DrawBitmap(CDC* pDC, CRect rect, UINT id) {
	CDC mDC;
	BITMAP bmp_info;
	mDC.CreateCompatibleDC(pDC);
	CBitmap bmp, *old_bmp = nullptr;
	bmp.LoadBitmap(id);
	bmp.GetBitmap(&bmp_info);
	old_bmp = mDC.SelectObject(&bmp);
	int w = rect.Width();
	int h = rect.Height();
	if (bmp_info.bmWidth != rect.Width() || bmp_info.bmHeight != rect.Height()) {
		w = bmp_info.bmWidth*rect.Height() / bmp_info.bmHeight;
		h = rect.Height();
		if (w > rect.Width()) {
			w = rect.Width();
			h = bmp_info.bmHeight*rect.Width() / bmp_info.bmWidth;
		}
	}
	int x = (rect.Width() - w) / 2 + rect.left;
	int y = (rect.Height() - h) / 2 + rect.top;
	pDC->SetStretchBltMode(HALFTONE);
	pDC->StretchBlt(x, y, w, h, &mDC, 0, 0, bmp_info.bmWidth, bmp_info.bmHeight, SRCCOPY);
	mDC.SelectObject(old_bmp);
}
///Tag View
__declspec(selectany) int g_tag_idx = 0;
__declspec(selectany) std::atomic<COLORREF> g_tag_color_text_purple = RGB(108,113,196);
__declspec(selectany) std::atomic<COLORREF> g_tag_color_text_red = RGB(220,50,47);
__declspec(selectany) int g_degree = -1;

///Export View
__declspec(selectany) std::atomic<COLORREF> g_solarized_yellow = RGB(181,137,0);
__declspec(selectany) bool* g_is_rotate;
__declspec(selectany) bool* g_is_ignore;
__declspec(selectany) bool* g_is_dbg_info;
__declspec(selectany) std::vector<std::pair<CString, bool>>* g_export_class_data;
__declspec(selectany) CString g_export_dir;
__declspec(selectany) CString g_project_name;
__declspec(selectany) int g_train_ratio;

__declspec(selectany) std::atomic<bool> g_exporting = false;
__declspec(selectany) std::atomic<size_t> g_progress = 0;

__declspec(selectany) int g_maximum_size = 0;
__declspec(selectany) size_t g_total_images = 0;
__declspec(selectany) size_t g_complete_images = 0;
inline void ShowProgressBar(CDC* pDC, CRect rect_window) {
	CPen null_pen;
	null_pen.CreatePen(PS_NULL, 0, RGB(0, 0, 0));
	
	//Draw status window
	CBrush bk_brush;
	bk_brush.CreateSolidBrush(RGB(7, 54, 66));
	CBrush* old_brush = pDC->SelectObject(&bk_brush);
	CRect rect;

	rect.left = (rect_window.left + rect_window.right) / 2 - 250;
	rect.right = rect.left + 500;
	rect.top = (rect_window.top + rect_window.bottom) / 2 - 50;
	rect.bottom = rect.top + 100;
	CPen pen;
	pen.CreatePen(PS_SOLID, 4, RGB(108, 113, 196));
	pDC->SelectObject(&pen);
	pDC->RoundRect(rect, CPoint(10, 10));
	pDC->SelectObject(&null_pen);
	pDC->SelectObject(old_brush);
	//Draw Progressbar
	CBrush probk_brush;
	probk_brush.CreateSolidBrush(RGB(88, 110, 117));
	old_brush = pDC->SelectObject(&probk_brush);
	CRect progress_rect = rect;
	progress_rect.top += static_cast<LONG>(rect.Height()*0.1);
	progress_rect.bottom = static_cast<LONG>(progress_rect.top + rect.Height()*0.3);
	progress_rect.left = static_cast<LONG>(rect.left + rect.Width()*0.05);
	progress_rect.right = static_cast<LONG>(rect.right - rect.Width()*0.05);
	pDC->RoundRect(progress_rect, CPoint(10, 10));
	pDC->SelectObject(old_brush);
	//Draw progress thumb
	CBrush proth_brush;
	proth_brush.CreateSolidBrush(RGB(38, 139, 210));
	old_brush = pDC->SelectObject(&proth_brush);
	CRect thumb_rect = progress_rect;
	if (g_complete_images != 0) {
		thumb_rect.right = static_cast<LONG>(thumb_rect.left + progress_rect.Width()*g_progress/100);
	} else {
		thumb_rect.right = thumb_rect.left;
	}
	pDC->RoundRect(thumb_rect, CPoint(10, 10));
	pDC->SelectObject(old_brush);
	//Draw status text
	CRect status_rect = rect;
	status_rect.bottom -= static_cast<LONG>(rect.Height()*0.1);
	status_rect.top = static_cast<LONG>(status_rect.bottom - rect.Height()*0.3);
	status_rect.left = static_cast<LONG>(rect.left + rect.Width()*0.05);
	status_rect.right = static_cast<LONG>(rect.right - rect.Width()*0.05);
	CString str;
	if (g_progress < 3 ) {
		str.Format(TEXT("Downloading modules...."));
	} else {
		str.Format(TEXT("completed images ... (%d/%d)"), g_complete_images, g_total_images);
	}
	int font_height = mspring::Font::GetRealFontHeight(TEXT("Arial"), status_rect.Height(), pDC, str, true);

	int font_height2 = mspring::Font::GetRealFontHeight(TEXT("Arial"), status_rect.Width(), pDC, str, false);
	font_height = mspring::Min(font_height, font_height2);
	CFont font;
	font.CreatePointFont(font_height, TEXT("Arial"));
	CFont* old_font = pDC->SelectObject(&font);
	pDC->SetTextColor(RGB(253, 246, 227));
	pDC->SetBkMode(TRANSPARENT);

	pDC->TextOutW(status_rect.left, status_rect.top, str);
	//dbb.getDC().RoundRect(status_rect, CPoint(10, 10));
	pDC->SelectObject(old_brush);


	pDC->SelectObject(old_font);
	font.DeleteObject();
	bk_brush.DeleteObject();
}
inline cv::Rect GetSafeRect(cv::Rect rect,cv::Size size) {
	mspring::SetRange(rect.x, 0, size.width - 1);
	mspring::SetRange(rect.y, 0,size.height - 1);
	if (rect.x + rect.width >= size.width) {
		rect.width = size.width - rect.x - 1;
	}
	if (rect.y + rect.height >= size.height) {
		rect.height = size.height - rect.y - 1;
	}
	return rect;
}