// TSpringMsgBox.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSpring.h"
#include "TSpringMsgBox.h"
#include "afxdialogex.h"


// TSpringMsgBox 대화 상자입니다.

IMPLEMENT_DYNAMIC(TSpringMsgBox, CDialogEx)

TSpringMsgBox::TSpringMsgBox(TString msg, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MSGBOX, pParent)
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif
	m_msg = msg;
}

TSpringMsgBox::~TSpringMsgBox()
{
	m_font.DeleteObject();
}

void TSpringMsgBox::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSG, m_text);
	DDX_Control(pDX, IDOK, m_ok);
}


BEGIN_MESSAGE_MAP(TSpringMsgBox, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// TSpringMsgBox 메시지 처리기입니다.


BOOL TSpringMsgBox::OnEraseBkgnd(CDC* pDC) {
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, GetTheme().ColorBK());
	return TRUE;
}


HBRUSH TSpringMsgBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) {
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	if (nCtlColor == CTLCOLOR_EDIT) {
		pDC->SetBkColor(MControlObject::GetBrightColor(GetTheme().ColorBK()));
		pDC->SetTextColor(GetTheme().ColorText());
	}
	if (nCtlColor == CTLCOLOR_STATIC) {
		pDC->SetBkColor(GetTheme().ColorBK());
		pDC->SetTextColor(GetTheme().ColorText());
	}
	if (nCtlColor == CTLCOLOR_BTN) {
		pDC->SetBkMode(TRANSPARENT);
	}
	return hbr;
}


BOOL TSpringMsgBox::OnInitDialog() {
	CDialogEx::OnInitDialog();
	CRect wrect;
	this->GetWindowRect(&wrect);
	CRect rect;
	CDC* DC = this->GetDC();
	int h = 0;
	h = mspring::Font::GetRealFontHeight(GetTheme().Font(), 20, DC);
	m_font.CreatePointFont(h, GetTheme().Font().c_str());
	CString str;
	CSize sz;
	m_text.SetWindowText(m_msg.c_str());
	m_text.SetFont(&m_font);
	m_text.GetWindowText(str);
	DC->SelectObject(&m_font);
	GetTextExtentPoint32(DC->GetSafeHdc(), str, str.GetLength(), &sz);
	m_text.GetWindowRect(rect);
	ScreenToClient(rect);

	int margin = (wrect.Width() - sz.cx) / 2;

	rect.left = margin;

	rect.right = rect.left + sz.cx;
	rect.bottom = rect.top + sz.cy;
	m_text.MoveWindow(rect);
	
	m_ok.SetFont(&m_font);
	m_ok.ModifyStyle(1, BS_OWNERDRAW);
	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	ReleaseDC(DC);
	return TRUE;  // return TRUE unless you set the focus to a control
}
