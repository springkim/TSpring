
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
};


