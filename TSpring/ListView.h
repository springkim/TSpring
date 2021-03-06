#pragma once
#include<afxstr.h>
#include<cctype>
#include<ctime>
#include"Mspring/MSpring.h"
#include"Theme.h"
#include"ispring/All.h"

#include"resource.h"
#include"TSpringMsgBox.h"
class ListView :public VirtualView {
	const UINT_PTR MOUSE_LEAVE_TIMER_ID = (UINT_PTR)"MOUSE_LEAVE_TIMER_ID";
public:
	MListBox m_list_image;

	MListBox m_list_class;
	MSingleEdit m_edit_class;
	MButton m_btn_add_class;
	MButton m_btn_load_class;
	MButton m_btn_export_class;

	MButtonCheck m_chk_detection;
	MStatic m_stc_detection;
	MButtonCheck m_chk_segmentation;
	MStatic m_stc_segmentation;

	MButtonCheck m_chk_rectangle;
	MStatic m_stc_rectangle;
	MButtonCheck m_chk_r_mid;
	MStatic m_stc_r_mid;
	MButtonCheck m_chk_r_2pt;
	MStatic m_stc_r_2pt;

	MButtonCheck m_chk_tracking;
	MStatic m_stc_tracking;

	MStatic m_stc_image;
	MStatic m_stc_class;
public:
	ListView(CWnd* wnd);
	~ListView();
	
	void IdentifyTagInfo();
	void GetFinalID();
public:
	int OnCreate()override;
	void OnDestroy()override;

	void OnPaint(CDC* pDC)override;
	void OnSetFocus(CWnd* pOldWnd)override;
	void OnKillFocus(CWnd* pNewWnd)override;

	void OnLButtonDown(UINT nFlags, CPoint point)override;
	void OnLButtonUp(UINT nFlags, CPoint point)override;
	void OnLButtonDblClk(UINT nFlags, CPoint point)override;
	void OnRButtonUp(UINT nFlags, CPoint point)override;
	BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)override;
	void OnMouseMove(UINT nFlags, CPoint point)override;
	void OnMouseLeave()override;
	void OnRButtonDown(UINT nFlags, CPoint point)override;
	BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)override;
	void OnSize(UINT nType, int cx, int cy)override;
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)override;
	void OnTimer(UINT_PTR nIDEvent)override;
	void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)override;
	LRESULT OnComposition(WPARAM wParam, LPARAM lParam)override;
};

