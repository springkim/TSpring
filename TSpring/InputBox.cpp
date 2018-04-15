// InputBox.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSpring.h"
#include "InputBox.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(XButton, CButton)
IMPLEMENT_DYNAMIC(XEdit, CEdit)

BEGIN_MESSAGE_MAP(XButton, CButton)
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()
BEGIN_MESSAGE_MAP(XEdit, CEdit)
	ON_WM_NCPAINT()
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()
// InputBox 대화 상자입니다.

IMPLEMENT_DYNAMIC(InputBox, CDialogEx)

InputBox::InputBox(CWnd* pParent, int& _id)
	: CDialogEx(IDD_INPUT, pParent), id(_id) {

}

InputBox::~InputBox() {
}

void InputBox::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, input_id);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Control(pDX, IDC_STATIC_ID, m_text);
}


BEGIN_MESSAGE_MAP(InputBox, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDCANCEL, &InputBox::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &InputBox::OnBnClickedOk)
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL InputBox::OnEraseBkgnd(CDC* pDC) {
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, GetTheme().ColorBK());
	return TRUE;
}


HBRUSH InputBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) {
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


BOOL InputBox::OnInitDialog() {
	CDialogEx::OnInitDialog();
	CRect rect;
	CDC* DC = this->GetDC();
	int h = 0;
	h = mspring::Font::GetRealFontHeight(GetTheme().Font(), rect.Height(), DC);
	m_font_for_fps.CreatePointFont(h, GetTheme().Font().c_str());
	input_id.SetFont(&m_font_for_fps);
	// Static 컨트롤의 크기 변경
	CSize sz;
	m_text.SetFont(&m_font_for_fps);
	CString str_fps;
	m_text.GetWindowText(str_fps);
	DC->SelectObject(&m_font_for_fps);
	GetTextExtentPoint32(DC->GetSafeHdc(), str_fps, str_fps.GetLength(), &sz);
	m_text.GetWindowRect(rect);
	ScreenToClient(rect);
	rect.right = rect.left + sz.cx;
	rect.bottom = rect.top + sz.cy;
	m_text.MoveWindow(rect);

	input_id.ModifyStyle(0, ES_NUMBER);
	input_id.SetLimitText(9);


	m_ok.ModifyStyle(1, BS_OWNERDRAW);
	m_cancel.ModifyStyle(1, BS_OWNERDRAW);

	CString tmp;
	tmp.Format(TEXT("%d"), id);
	input_id.SetWindowText(tmp);
	ReleaseDC(DC);
	this->SetTimer(INPUTBOX_TIMER_ID,100,nullptr);
	return TRUE;
}

void InputBox::OnBnClickedCancel() {
	this->KillTimer(INPUTBOX_TIMER_ID);
	TCHAR tmp[1024] = { 0 };
	input_id.GetWindowText(tmp,1024);
	if (_tcslen(tmp) == 0) {
		id = -1;
	} else {
		id = _wtoi(tmp);
	}
	CDialogEx::OnCancel();
}
void InputBox::OnBnClickedOk() {
	this->KillTimer(INPUTBOX_TIMER_ID);
	TCHAR tmp[1024] = { 0 };
	input_id.GetWindowText(tmp, 1024);
	if (_tcslen(tmp) == 0) {
		id = -1;
	} else {
		id = _wtoi(tmp);
	}
	CDialogEx::OnOK();
}


void InputBox::OnTimer(UINT_PTR nIDEvent) {
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == INPUTBOX_TIMER_ID) {
		CDC* DC = input_id.GetDC();

		CRect rect;
		input_id.GetClientRect(&rect);
		CPen pen;
		pen.CreatePen(PS_SOLID, 2, GetTheme().ColorBK());
		CPen* old_pen = DC->SelectObject(&pen);
		DC->MoveTo(rect.left, rect.top);
		DC->LineTo(rect.left, rect.bottom);
		DC->MoveTo(rect.right, rect.top);
		DC->LineTo(rect.right, rect.bottom);
		DC->SelectObject(old_pen);
		pen.DeleteObject();
		ReleaseDC(DC);
	}
	CDialogEx::OnTimer(nIDEvent);
}
