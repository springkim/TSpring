
// MainFrm.h : CMainFrame 클래스의 인터페이스
//

#pragma once

#include"mspring\frame.h"
#include"mspring\view.h"
#include"ListView.h"
#include"TagView.h"
#include"ExportView.h"
#include"repository.h"
__declspec(selectany) CString g_font = TEXT("Arial");
__declspec(selectany) COLORREF g_color_bk = RGB(7,54,66);
__declspec(selectany) COLORREF g_color_text = RGB(203,75,22);
__declspec(selectany) COLORREF g_color_border = RGB(181,137,0);

__declspec(selectany) COLORREF g_color_hover = RGB(203, 75, 22);
__declspec(selectany) COLORREF g_color_text_menu = RGB(199, 199, 199);


__declspec(selectany) COLORREF g_color_activate = RGB(133,153,0);
__declspec(selectany) COLORREF g_color_deactivate = RGB(88,110,117);
__declspec(selectany) COLORREF g_color_text_tab = RGB(255, 255, 255);

class CMainFrame : public MSpringFrame
{
	
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// 구현입니다.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	MSpringView    m_wndView;
	///ListView
	ListView* m_list_view;
	///========

	TagView* m_tag_view;
	ExportView* m_export_view;
	
	MSpringMenuFrame* m_menu_frame = nullptr;
	MSpringTabFrame* m_tab_frame = nullptr;

protected:
	HCURSOR m_cursor;
	HCURSOR m_cursor_prev;
public:
	const std::string temp_file = "TSpring_settings.txt";
	void ReadSettingFile() {
		char c_temp[MAX_PATH + 1] = { 0 };
		GetTempPathA(MAX_PATH,c_temp);
		std::string temp = c_temp;
		if (temp.back() != '\\' || temp.back() != '/') {
			temp += '\\';
		}
		temp += temp_file;
		std::fstream fin;
		fin.open(temp);
		if (fin.is_open() == false) {
			return;
		}
		int tag_type = -1;
		int tag_type_rule = -1;
		int is_tracking = -1;
		fin >> tag_type >> tag_type_rule >> is_tracking;
		*g_is_detection = false;
		*g_is_detection = false;
		switch (tag_type) {
			case 0:{
				*g_is_detection = true;
			}break;
			case 1: {
				*g_is_segmentation = true;
			}break;
		}
		*g_is_square = false;
		*g_is_rectangle = false;
		*g_is_rmid = false;
		*g_is_r2pt = false;
		switch (tag_type_rule ) {
			case 0: {
				*g_is_square = true;
			}break;
			case 1: {
				*g_is_rectangle = true;
			}break;
			case 2: {
				*g_is_rmid = true;
			}break;
			case 3: {
				*g_is_r2pt = true;
			}break;
		}
		if (is_tracking == 1) {
			*g_is_tracking = true;
		}
		int numOfClass = 0;
		fin >> numOfClass;
		for (int i = 0; i < numOfClass; i++) {
			std::string _class;
			fin >> _class;
			g_class_data->push_back(std::make_pair(mspring::String::ToCString(_class), false));
		}
		fin.close();
	}
	void WriteSettingFile() {
		char c_temp[MAX_PATH + 1] = { 0 };
		GetTempPathA(MAX_PATH, c_temp);
		std::string temp = c_temp;
		if (temp.back() != '\\' || temp.back() != '/') {
			temp += '\\';
		}
		temp += temp_file;
		std::fstream fout;
		fout.open(temp,std::ios::out);
		if (fout.is_open() == false) {
			return;
		}
		//Obj detection or Segmentation
		if (*g_is_detection == true) {
			fout << 0 << std::endl;
		} else if(*g_is_segmentation==true) {
			fout << 1 << std::endl;
		} else {
			fout << -1 << std::endl;
		}
		//Type of tagging rule
		if (*g_is_square == true) {
			fout << 0 << std::endl;
		} else if (*g_is_rectangle == true) {
			fout << 1 << std::endl;
		} else if (*g_is_rmid == true) {
			fout << 2 << std::endl;
		} else if (*g_is_r2pt == true) {
			fout << 3 << std::endl;
		} else {
			fout << -1 << std::endl;
		}
		//Tracking or not
		if (*g_is_tracking == true) {
			fout << 1 << std::endl;
		} else {
			fout << -1 << std::endl;
		}
		fout << g_class_data->size() << std::endl;
		for (size_t i = 0; i < g_class_data->size(); i++) {
			fout << mspring::String::ToString(g_class_data->at(i).first) << std::endl;
		}
		fout.close();
	}
// 생성된 메시지 맵 함수
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnFileOpenImages();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnFileOpenimagefolder();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnDestroy();
	afx_msg void OnFileClearimages();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnEditEditmode();
	afx_msg void OnEditTrackingmode();
};


