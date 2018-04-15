#pragma once
#include "afxwin.h"
#include"InputBox.h"

// TSpringMsgBox ��ȭ �����Դϴ�.

class TSpringMsgBox : public CDialogEx
{
	DECLARE_DYNAMIC(TSpringMsgBox)

public:
	TSpringMsgBox(TString msg,CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~TSpringMsgBox();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MSGBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
