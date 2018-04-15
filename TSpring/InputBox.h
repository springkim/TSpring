#pragma once
#include "afxwin.h"
#include"MSpring/MSpring.h"
#include"Theme.h"
// InputBox 대화 상자입니다.
class XButton : public CButton {
protected:
	DECLARE_DYNAMIC(XButton)
	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) {
		CDC dc;
		dc.Attach(lpDrawItemStruct->hDC);     
		CRect rt;
		rt = lpDrawItemStruct->rcItem;       
		dc.FillSolidRect(rt, GetTheme().ColorBK());
		CPen pen_null; pen_null.CreatePen(PS_NULL, 0, RGB(0, 0, 0));
		CPen* old_pen = dc.SelectObject(&pen_null);
		CBrush brush;
		UINT state = lpDrawItemStruct->itemState; 
		if ((state & ODS_SELECTED))            
		{
			//dc.DrawEdge(rt, EDGE_SUNKEN, BF_RECT);
			brush.CreateSolidBrush(MControlObject::GetBrightColor(*GetTheme().ButtonColorFR()));
		} else {
			//dc.DrawEdge(rt, EDGE_RAISED, BF_RECT); 
			brush.CreateSolidBrush(*GetTheme().ButtonColorFR());
		}
		CBrush* old_brush = dc.SelectObject(&brush);
		dc.RoundRect(rt, CPoint(5, 5));
		dc.SelectObject(old_brush);
		brush.DeleteObject();
		dc.SelectObject(pen_null);
		pen_null.DeleteObject();

		dc.SetTextColor(*GetTheme().ButtonColorText());
		CString strTemp;
		GetWindowText(strTemp);
		dc.DrawText(strTemp, rt, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		if ((state & ODS_FOCUS)){
			int iChange = 1;
			rt.top += iChange;
			rt.left += iChange;
			rt.right -= iChange;
			rt.bottom -= iChange;
			dc.DrawFocusRect(rt);
		}
		dc.Detach();
	}
};
class XEdit : public CEdit {
protected:
	DECLARE_DYNAMIC(XEdit)
	DECLARE_MESSAGE_MAP()
public:
protected:
	afx_msg void OnNcPaint() {
		CDC* pDC = GetWindowDC(); 
		CRect rect; GetWindowRect( &rect);
		rect.OffsetRect( -rect.left, -rect.top);
		CBrush brush(GetTheme().ColorBK());
		pDC->FillRect(&rect,&brush);
		brush.DeleteObject();
		ReleaseDC( pDC);
	}
};
class InputBox : public CDialogEx {
	DECLARE_DYNAMIC(InputBox)

public:
	InputBox(CWnd* pParent, int& _id);   // 표준 생성자입니다.
	virtual ~InputBox();
	int& id;
	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum {
		IDD = IDD_INPUT
	};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	CFont m_font_for_fps;
	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
public:
	XEdit input_id;
	XButton m_ok;
	XButton m_cancel;
	CStatic m_text;
	const UINT_PTR INPUTBOX_TIMER_ID = (UINT_PTR)"INPUTBOX_TIMER_ID";
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
