// SplashDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Pivovarna.h"
#include "SplashDialog.h"


// CSplashDialog dialog

IMPLEMENT_DYNAMIC(CSplashDialog, CDialog)

CSplashDialog::CSplashDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSplashDialog::IDD, pParent)
{
}

CSplashDialog::~CSplashDialog()
{
}

void CSplashDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESSTEXT, m_staticStep);
	DDX_Control(pDX, IDC_PROGRESSTEXTSTATUS, m_staticStepStatus);
}

BEGIN_MESSAGE_MAP(CSplashDialog, CDialog)
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CSplashDialog message handlers
BOOL CSplashDialog::OnInitDialog() {
	return CDialog::OnInitDialog();
}

void CSplashDialog::OnPaint() {
	CDialog::OnPaint();
}

void CSplashDialog::OnSize(UINT nType, int cx, int cy) {
	UNREFERENCED_PARAMETER(nType);
	UNREFERENCED_PARAMETER(cx);
	UNREFERENCED_PARAMETER(cy);
	this->GetWindowRect(m_rectWindow);
	this->SetBackground();
	this->SetProgresBarAndLabel();
	CDialog::OnSize(nType, cx, cy);
}

// CSplashDialog public methods
void CSplashDialog::SetStep(int nStepProgress, TCHAR* lpszStepText) {
	CProgressCtrl* pCtrlProgress = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS1);
	pCtrlProgress->SetPos(nStepProgress);
	m_staticStep.AppendWindowText(_T("\n"));
	m_staticStep.AppendWindowText(lpszStepText);
	m_staticStep.AppendWindowText(_T("..."));
	m_staticStepStatus.AppendWindowText(_T("\n"));
	Sleep(1200);
	m_staticStepStatus.AppendWindowText(_T("OK"));
	Sleep(300);
}

// CSplashDialog private methods
void CSplashDialog::CalculateOffset() {
	m_sizeOffset.cx = (m_rectWindow.Width() - m_sizeBackground.cx) / 2;
	m_sizeOffset.cy = (m_rectWindow.Height() - m_sizeBackground.cy) / 2;
}

void CSplashDialog::SetBackground() {
	CDC MemDC;
	CBitmap bmp;
	CPaintDC dc(this);
	CRect rct;
	this->GetClientRect(&rct);
	MemDC.CreateCompatibleDC(&dc);
	m_imgOzadje.Load(_T("res\\splash_logo.png"));
	m_sizeBackground.cx = m_imgBackground.GetWidth();
	m_sizeBackground.cy = m_imgBackground.GetHeight();
	this->CalculateOffset();
	bmp.Attach(m_imgBackground.Detach());
	MemDC.SelectObject(&bmp);
	dc.BitBlt(m_sizeOffset.cx, m_sizeOffset.cy, rct.Width(), rct.Height(), &MemDC, 0, 0, SRCCOPY);
}

void CSplashDialog::SetProgresBarAndLabel() {
	CProgressCtrl* pCtrlProgress = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS1);
	pCtrlProgress->SetRange(0, 100);
	pCtrlProgress->MoveWindow(m_sizeOffset.cx + 35, m_sizeOffset.cy + 740, 954, 15, FALSE);
	pCtrlProgress->RedrawWindow();
	m_staticStep.MoveWindow(m_sizeOffset.cx + 35, m_sizeOffset.cy + 535, 200, 200, FALSE);
	m_staticStep.SetWindowTextW(_T(""));
	m_staticStepStatus.SetTextColor(RGB(0, 120, 0));
	m_staticStepStatus.MoveWindow(m_sizeOffset.cx+235, m_sizeOffset.cy + 535, 50, 200, FALSE);
	m_staticStepStatus.SetWindowTextW(_T(""));
}


// CTransparentStatic

IMPLEMENT_DYNAMIC(CTransparentStatic, CStatic)
CTransparentStatic::CTransparentStatic() {
	m_colorBesedilo = RGB(38, 54, 72);
}

CTransparentStatic::~CTransparentStatic() {
}

BEGIN_MESSAGE_MAP(CTransparentStatic, CStatic)
	ON_MESSAGE(WM_SETTEXT,OnSetText)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()

// CTransparentStatic message handlers

void CTransparentStatic::OnPaint() {
	CPaintDC dc(this);
	CRect rect;
	GetClientRect(rect);
	dc.SelectObject(GetParent()->GetFont());
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(m_colorBesedilo);
	CString strOutput;
	GetWindowText(strOutput);
	CRect rectText(rect);
	rectText.bottom = 0;
	dc.DrawText(strOutput, strOutput.GetLength(), rectText, DT_WORDBREAK | DT_CALCRECT);
	rectText.OffsetRect(0, rect.Height() - rectText.Height());
	dc.DrawText(strOutput, strOutput.GetLength(), rectText, DT_WORDBREAK);
}

LRESULT CTransparentStatic::OnSetText(WPARAM wParam,LPARAM lParam) {
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	LRESULT Result = Default();
	Invalidate();
	UpdateWindow();
	RedrawWindow();
	return Result;
}

HBRUSH CTransparentStatic::CtlColor(CDC* pDC, UINT nCtlColor) {
	UNREFERENCED_PARAMETER(nCtlColor);
	pDC->SetBkMode(TRANSPARENT);
	return (HBRUSH)GetStockObject(NULL_BRUSH);
}

BOOL CTransparentStatic::OnEraseBkgnd(CDC* pDC) {
	if (m_Bmp.GetSafeHandle() == NULL) {
		CRect Rect;
		GetWindowRect(&Rect);
		CWnd *pParent = GetParent();
		pParent->ScreenToClient(&Rect);
		CDC *pDC = pParent->GetDC();
		CDC MemDC;
		MemDC.CreateCompatibleDC(pDC);
		m_Bmp.CreateCompatibleBitmap(pDC, Rect.Width(), Rect.Height());
		CBitmap *pOldBmp = MemDC.SelectObject(&m_Bmp);
		MemDC.BitBlt(0, 0, Rect.Width(), Rect.Height(), pDC, Rect.left, Rect.top, SRCCOPY);
		MemDC.SelectObject(pOldBmp);
		pParent->ReleaseDC(pDC);
	}
	else {
		CRect Rect;
		GetClientRect(Rect);
		CDC MemDC;
		MemDC.CreateCompatibleDC(pDC);
		CBitmap *pOldBmp = MemDC.SelectObject(&m_Bmp);
		pDC->BitBlt(0, 0, Rect.Width(), Rect.Height(), &MemDC, 0, 0, SRCCOPY);
		MemDC.SelectObject(pOldBmp);
	}
	return TRUE;
}

// CTransparentStatic public methods
void CTransparentStatic::AppendWindowText(TCHAR *lpszText) {
	CString sCurrentText;
	GetWindowText(sCurrentText);
	sObstojeceBesedilo += lpszText;
	SetWindowText(sCurrentText);
}

void CTransparentStatic::SetTextColor(COLORREF color) {
	m_colorText = color;
}
