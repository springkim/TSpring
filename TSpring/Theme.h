#pragma once
#include<vector>
#include<opencv2/opencv.hpp>
#include"stdafx.h"
#include"MSpring/MSpring.h"
#include"resource.h"
class Theme {
public:
	int g_idx = 0;
private:
	std::vector<TString> g_font;						//폰트입니다.
	std::vector<COLORREF> g_color_bk;				//배경색 입니다.
	std::vector<COLORREF> g_color_hover;			//마우스가 올라갔을떄 색 입니다.
	std::vector<COLORREF> g_color_text;				//글자색 입니다.	
	std::vector<COLORREF> g_color_title;				//제목 글자색 입니다.	
	std::vector<COLORREF> g_color_border;			//테두리 색 입니다.
	std::vector<COLORREF> g_color_activate;			//활성화 색 입니다.
	std::vector<COLORREF> g_color_deactivate;		//비활성화 색 입니다.
	std::vector<COLORREF> g_color_menu_border;			//메뉴 테두리 색 입니다.
	std::vector<UINT> g_close_btn;
	std::vector<UINT> g_maximize_btn;
	std::vector<UINT> g_minimize_btn;
	std::vector<UINT> g_usage;
	std::vector<UINT> g_nolist;
	//ListBox
	std::vector<COLORREF> g_color_list_scroll;	//스크롤의 색 입니다.
	//Button
	std::vector<COLORREF> g_color_button_fr;	//버튼의 배경 색 입니다.
	std::vector<COLORREF> g_color_button_text;	//버튼의 글자 색 입니다.
public:
	Theme() {
		///AshSky
		g_font.push_back(TEXT("Calibri"));
		g_color_bk.push_back(RGB(32,37,48));
		g_color_hover.push_back(RGB(58, 69, 90));
		g_color_text.push_back(RGB(217,229,255));
		g_color_title.push_back(RGB(0, 143, 231));
		g_color_border.push_back(RGB(178,204,255));
		g_color_activate.push_back(RGB(32, 37, 48));
		g_color_deactivate.push_back(RGB(58, 69, 90));
		g_color_menu_border.push_back(RGB(0, 143, 231));
		g_close_btn.push_back(IDB_CLOSE_ASHSKY);
		g_maximize_btn.push_back(IDB_MAXIMIZE_ASHSKY);
		g_minimize_btn.push_back(IDB_MINIMIZE_ASHSKY);
		g_usage.push_back(IDB_USAGE_ASHSKY);
		g_nolist.push_back(IDB_NOLIST_ASHSKY);
		//button
		g_color_button_fr.push_back(RGB(0, 143, 231));
		g_color_button_text.push_back(RGB(255, 255, 255));
		//list
		g_color_list_scroll.push_back(RGB(0, 143, 231));
		///Solarized Dark
		g_font.push_back(TEXT("Arial"));
		g_color_bk.push_back(RGB(0,43,54));
		g_color_hover.push_back(RGB(203, 75, 22));
		g_color_text.push_back(RGB(238,232,213));
		g_color_title.push_back(RGB(203, 75, 22));
		g_color_border.push_back(RGB(181, 137, 0));
		g_color_activate.push_back(RGB(133, 153, 0));
		g_color_deactivate.push_back(RGB(88, 110, 117));
		g_color_menu_border.push_back(RGB(133, 153, 0));
		g_close_btn.push_back(IDB_CLOSE);
		g_maximize_btn.push_back(IDB_MAXIMIZE);
		g_minimize_btn.push_back(IDB_MINIMIZE);
		g_usage.push_back(IDB_USAGE_SOLARIZED_DARK);
		g_nolist.push_back(IDB_NOLIST_SOLARIZED_DARK);
		//button
		g_color_button_fr.push_back(RGB(38,139,210));
		g_color_button_text.push_back(RGB(238, 232, 213));
		//list
		g_color_list_scroll.push_back(RGB(38, 139, 210));

	}
	void SetIdx(int idx) {
		g_idx = idx;
	}
	int GetIdx() {
		return g_idx;
	}
public:
	TString Font() {
		return g_font[g_idx];
	}
	COLORREF ColorBK() {
		return g_color_bk[g_idx];
	}
	COLORREF ColorHover() {
		return g_color_hover[g_idx];
	}
	COLORREF ColorText() {
		return g_color_text[g_idx];
	}
	COLORREF ColorTitle() {
		return g_color_title[g_idx];
	}
	COLORREF ColorBorder() {
		return g_color_border[g_idx];
	}
	COLORREF ColorDeactivate() {
		return g_color_deactivate[g_idx];
	}
	COLORREF ColorActivate() {
		return g_color_activate[g_idx];
	}
	COLORREF ColorMenuBorder() {
		return g_color_menu_border[g_idx];
	}
	UINT ResourceClose() {
		return g_close_btn[g_idx];
	}
	UINT ResourceMaximize() {
		return g_maximize_btn[g_idx];
	}
	UINT ResourceMinimize() {
		return g_minimize_btn[g_idx];
	}
	UINT ResourceUsage() {
		return g_usage[g_idx];
	}
	UINT ResourceNoList() {
		return g_nolist[g_idx];
	}
	//==LISTVIEW
	std::atomic<COLORREF>* ListViewColorBK() {
		static std::atomic<COLORREF> color;
		color = ColorBK();
		return &color;
	}
	std::atomic<COLORREF>* ListViewColorText() {
		static std::atomic<COLORREF> color;
		color = ColorText();
		return &color;
	}
	std::atomic<COLORREF>* ButtonColorFR() {
		static std::atomic<COLORREF> color;
		color = g_color_button_fr[g_idx];
		return &color;
	}
	std::atomic<COLORREF>* ButtonColorText() {
		static std::atomic<COLORREF> color;
		color = g_color_button_text[g_idx];
		return &color;
	}
	std::atomic<COLORREF>* ListColorScroll() {
		static std::atomic<COLORREF> color;
		color = g_color_list_scroll[g_idx];
		return &color;
	}
	void Update() {
		ListViewColorBK();
		ListViewColorText();
		ButtonColorFR();
		ButtonColorText();
		ListColorScroll();
	}
};
__declspec(selectany) Theme* ptheme = nullptr;
inline void ThemeRelease() {
	if (ptheme) {
		delete ptheme;
		ptheme = nullptr;
	}
}
inline Theme& GetTheme() {
	if (ptheme == nullptr) {
		ptheme = new Theme();
		atexit(ThemeRelease);
	}
	return *ptheme;
}
class AppStatus {
public:
	std::atomic<bool> g_exporting;
	std::atomic<bool> g_analysing;
	std::atomic<int> g_analysing_count = 0;
	std::atomic<int> g_command_go2tagging;
	std::vector<std::pair<TString, bool>>* g_image_data;
	std::atomic<int> g_tag_idx = 0;
	std::atomic<int> g_id = 0;
	std::vector<std::pair<TString, bool>>* g_class_data;

	bool* g_is_detection;
	bool* g_is_segmentation;
	bool* g_is_rectangle;
	bool* g_is_rmid;
	bool* g_is_r2pt;
	bool* g_is_tracking;

	int* g_pen_weight;
	bool* g_is_magnifier;
	bool* g_is_crosshair;
	bool* g_is_showid;

	std::atomic<int> g_progress_total;
	std::atomic<int> g_progress_current;
	//ExportView.cpp
	bool* g_is_rotate;
	bool* g_is_ignore;
	std::vector<std::pair<TString, bool>>* g_export_class_data;
	bool* g_is_dbg_info;
	TString g_project_name;
	TString g_export_dir;
	TString g_export_str;

	int g_degree = -1;
	UINT g_buffer_size;
protected:
	TCHAR* g_buffer = nullptr;
public:
	AppStatus() {
		g_exporting = false;
		g_command_go2tagging = -1;
		g_buffer_size = 100000 * (MAX_PATH + 1) + 1;
		g_buffer =new TCHAR[g_buffer_size];
	}
	~AppStatus() {
		delete[] g_buffer;
	}
	TCHAR* GetBuffer() {
		memset(g_buffer, 0, g_buffer_size);
		return g_buffer;
	}
};
__declspec(selectany) AppStatus* papp = nullptr;
inline void AppRelease() {
	if (papp) {
		delete papp;
		papp = nullptr;
	}
}
inline AppStatus& GetApp() {
	if (papp == nullptr) {
		papp = new AppStatus();
		atexit(AppRelease);
	}
	return *papp;
}
///UTils
void DrawBitmap(CDC* pDC, CRect rect, UINT id);
class TagInfo {
public:
	int m_class;
	cv::RotatedRect m_rect;
	int m_id;
	cv::RotatedRect m_id_rect;
	TagInfo(int _class, cv::RotatedRect _rect,int _id) {
		m_class = _class;
		m_rect = _rect;
		m_id = _id;
	}
};
inline cv::Rect GetSafeRect(cv::Rect rect, cv::Size size) {
	mspring::SetRange(rect.x, 0, size.width);
	mspring::SetRange(rect.y, 0, size.height);
	if (rect.x + rect.width >= size.width) {
		rect.width = size.width - rect.x ;
	}
	if (rect.y + rect.height >= size.height) {
		rect.height = size.height - rect.y ;
	}
	return rect;
}
void ShowProgressBar(CDC* pDC, CRect rect_window);