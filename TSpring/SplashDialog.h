#pragma once

#include "atlimage.h"


// CTransparentStatic

class CTransparentStatic : public CStatic
{
	DECLARE_DYNAMIC(CTransparentStatic)

public:
	CTransparentStatic();
	virtual ~CTransparentStatic();
	void AppendWindowText(TCHAR* lpszText);
	void SetTextColor(COLORREF color);

protected:
	afx_msg void OnPaint();
	afx_msg LRESULT OnSetText(WPARAM,LPARAM);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()
private:
	CBitmap m_Bmp;
	COLORREF m_colorText;
};

// CSplashDialog dialog

class CSplashDialog : public CDialog
{
	DECLARE_DYNAMIC(CSplashDialog)

// private members
private: 
	CRect m_rectWindow;
	CSize m_sizeOffset;
	CSize m_sizeBackground;
	CImage m_imgBackground;
	CTransparentStatic m_staticStep;
	CTransparentStatic m_staticStepStatus;

public:
	CSplashDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSplashDialog();
	void SetStep(int nStepProgress, TCHAR* lpszStepText);

// Dialog Data
	enum { IDD = IDD_SPLASHDIALOG };

private:
	void CalculateOffset();
	void SetBackground();
	void SetProgresBarAndLabel();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()

};