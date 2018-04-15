
// MainFrm.h : CMainFrame 클래스의 인터페이스
//

#pragma once

#include"MSpring/MSpring.h"
#include"Theme.h"
#include"ListView.h"
#include"TagView.h"
#include"ExportView.h"

class CMainFrame : public MSpringFrame{
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	MSpringView    m_wndView;
	
	ListView* m_list_view;
	TagView* m_tag_view;
	ExportView* m_export_view;
	
	MSpringMenuFrame m_menu_frame;
	MSpringTabFrame m_tab_frame;

protected:
	HCURSOR m_cursor;
	HCURSOR m_cursor_prev;
public:
	const std::string setting_file = "TSpring_settings.txt";
	const std::string class_file = "TSpring_class.txt";
	void ReadSettingFile() {
		char c_temp[MAX_PATH + 1] = { 0 };
		SHGetSpecialFolderPathA(this->GetSafeHwnd(), c_temp, CSIDL_COMMON_APPDATA, TRUE);

		std::string temp = c_temp;
		if (temp.back() != '\\' || temp.back() != '/') {
			temp += '\\';
		}
		std::string setting = temp + setting_file;
		std::ifstream fin;
		fin.open(setting, std::ios::in);
		fin >> GetTheme().g_idx;
		int value;
		fin >> value;
		if (value == 0) {
			*GetApp().g_is_detection = true;
			*GetApp().g_is_segmentation = false;
		} else if (value == 1) {
			*GetApp().g_is_detection = false;
			*GetApp().g_is_segmentation = true;
		}
		fin >> value;
		*GetApp().g_is_rectangle = false;
		*GetApp().g_is_rmid = false;
		*GetApp().g_is_r2pt = false;
		if (value == 0) {
			*GetApp().g_is_rectangle = true;
		} else if (value == 1) {
			*GetApp().g_is_rmid = true;
		} else if (value == 2) {
			*GetApp().g_is_r2pt = true;
		}
		fin >> value;
		if (value >= 0 && value < 3) {
			*GetApp().g_pen_weight = value;
		}
		fin >> value;
		*GetApp().g_is_magnifier = value != 0;
		fin >> value;
		*GetApp().g_is_crosshair = value != 0;
		fin >> value;
		*GetApp().g_is_showid = value != 0;
		fin.close();

		std::string class_filename = temp + class_file;
		fin.open(class_filename, std::ios::in);
		std::vector<std::string> lists;
		while (fin.eof() == false) {
			std::string line;
			std::getline(fin, line);
			if (line.length() == 0)break;
			lists.push_back(line);
		}
		fin.close();

		char path[_MAX_PATH];
		GetModuleFileNameA(NULL, path, sizeof path);
		auto it = std::find(lists.begin(), lists.end(), path);
		if (it != lists.end()) {
			ptrdiff_t idx = it - lists.begin();
			int cnt = atoi(lists[idx + 1].c_str());
			for (auto it2 = it + 2; it2 != it + cnt + 2; it2++) {
				GetApp().g_class_data->push_back(std::make_pair(TString((*it2).begin(),(*it2).end()), false));
			}
		}
		OnThemeUpdate();
	}
	void WriteSettingFile() {
		char c_temp[MAX_PATH + 1] = { 0 };
		SHGetSpecialFolderPathA(this->GetSafeHwnd(), c_temp, CSIDL_COMMON_APPDATA, TRUE);

		std::string temp = c_temp;
		if (temp.back() != '\\' || temp.back() != '/') {
			temp += '\\';
		}
		std::string setting = temp+setting_file;
		std::fstream fout;
		fout.open(setting,std::ios::out);
		if (fout.is_open() == false) {
			return;
		}
		fout << GetTheme().g_idx << std::endl;
		//Obj detection or Segmentation
		if (*GetApp().g_is_detection == true) {
			fout << 0 << std::endl;
		} else if(*GetApp().g_is_segmentation==true) {
			fout << 1 << std::endl;
		} else {
			fout << -1 << std::endl;
		}
		//Type of tagging rule
		 if (*GetApp().g_is_rectangle == true) {
			fout << 0 << std::endl;
		} else if (*GetApp().g_is_rmid == true) {
			fout << 1 << std::endl;
		} else if (*GetApp().g_is_r2pt == true) {
			fout << 2 << std::endl;
		} else {
			fout << -1 << std::endl;
		}

		fout << *GetApp().g_pen_weight << std::endl;
		fout << (int)*GetApp().g_is_magnifier << std::endl;
		fout << (int)*GetApp().g_is_crosshair << std::endl;
		fout << (int)*GetApp().g_is_showid << std::endl;


		fout.close();

		//Class List
		std::string class_filename = temp + class_file;
		fout.open(class_filename, std::ios::in);
		std::vector<std::string> lists;
		while (fout.eof() == false) {
			std::string line;
			std::getline(fout, line);
			if (line.length() == 0)break;
			lists.push_back(line);
		}
		fout.close();
		
		char path[_MAX_PATH];
		GetModuleFileNameA(NULL, path, sizeof path);
		auto it = std::find(lists.begin(), lists.end(), path);
		if (it != lists.end()) {
			ptrdiff_t idx = it - lists.begin();
			int cnt = atoi(lists[idx + 1].c_str());
			auto it2 = it + cnt + 2;
			std::vector<std::string> lists2(it2, lists.end());
			lists.resize(idx);
			lists.push_back(path);
			std::ostringstream oss;
			oss << GetApp().g_class_data->size();
			lists.push_back(oss.str());
			for (auto&e : *GetApp().g_class_data) {
				lists.push_back(mspring::String::ToString(e.first));
			}
			for (auto&e : lists2) {
				lists.push_back(e);
			}
		} else {
			lists.push_back(path);
			std::ostringstream oss;
			oss << GetApp().g_class_data->size();
			lists.push_back(oss.str());
			for (auto&e : *GetApp().g_class_data) {
				lists.push_back(mspring::String::ToString(e.first));
			}
		}
		//경로
		//개수
		//클래스들 ...
		fout.open(class_filename,std::ios::out);
		for (auto&e : lists) {
			fout << e << std::endl;
		}
		fout.close();
	}
	void UpdateProgram();
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
	void OnThemeUpdate();
	afx_msg void OnThemeAshsky();
	afx_msg void OnThemeSolarizeddark();
	afx_msg LRESULT OnDisableHtTest(WPARAM wParam, LPARAM lParam);
};


