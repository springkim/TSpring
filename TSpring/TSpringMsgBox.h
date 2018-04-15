#pragma once
#include "afxwin.h"
#include"InputBox.h"

// TSpringMsgBox 대화 상자입니다.

class TSpringMsgBox : public CDialogEx
{
	DECLARE_DYNAMIC(TSpringMsgBox)

public:
	TSpringMsgBox(TString msg,CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~TSpringMsgBox();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MSGBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	TString m_msg;
	CFont m_font;
	CStatic m_text;
	XButton m_ok;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
};
