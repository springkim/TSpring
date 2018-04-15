
// TSpring.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "TSpring.h"
#include "MainFrm.h"
#include<timeapi.h>
#include "afxwin.h"
#include"resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTSpringApp

BEGIN_MESSAGE_MAP(CTSpringApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CTSpringApp::OnAppAbout)
END_MESSAGE_MAP()


// CTSpringApp ����

CTSpringApp::CTSpringApp()
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(104);
	//_CrtMemDumpAllObjectsSince(0);
	// TODO: �Ʒ� ���� ���α׷� ID ���ڿ��� ���� ID ���ڿ��� �ٲٽʽÿ�(����).
	// ���ڿ��� ���� ����: CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("TSpring.AppID.NoVersion"));

	timeBeginPeriod(1);
	//SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);

	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
}

// ������ CTSpringApp ��ü�Դϴ�.

CTSpringApp theApp;


// CTSpringApp �ʱ�ȭ

BOOL CTSpringApp::InitInstance()
{
	AfxOleInit();
	
	CWinApp::InitInstance();


	EnableTaskbarInteraction(FALSE);

	// RichEdit ��Ʈ���� ����Ϸ���  AfxInitRichEdit2()�� �־�� �մϴ�.	
	// AfxInitRichEdit2();

	// ǥ�� �ʱ�ȭ
	// �̵� ����� ������� �ʰ� ���� ���� ������ ũ�⸦ ���̷���
	// �Ʒ����� �ʿ� ���� Ư�� �ʱ�ȭ
	// ��ƾ�� �����ؾ� �մϴ�.
	// �ش� ������ ����� ������Ʈ�� Ű�� �����Ͻʽÿ�.
	// TODO: �� ���ڿ��� ȸ�� �Ǵ� ������ �̸��� ����
	// ������ �������� �����ؾ� �մϴ�.
	SetRegistryKey(_T("���� ���� ���α׷� �����翡�� ������ ���� ���α׷�"));


	// �� â�� ����� ���� �� �ڵ忡���� �� ������ â ��ü��
	// ���� ���� �̸� ���� ���α׷��� �� â ��ü�� �����մϴ�.
	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// �������� ����� ���ҽ��� �Բ� �ε��մϴ�.
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);





	// â �ϳ��� �ʱ�ȭ�Ǿ����Ƿ� �̸� ǥ���ϰ� ������Ʈ�մϴ�.
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	return TRUE;
}

int CTSpringApp::ExitInstance()
{
	//TODO: �߰��� �߰� ���ҽ��� ó���մϴ�.
	return CWinApp::ExitInstance();
}

// CTSpringApp �޽��� ó����


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();
	~CAboutDlg() {
		m_font.DeleteObject();
	}
// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
	bool LoadBitmapFromPNG(UINT uResourceID, CBitmap& BitmapOut) {
		bool bRet = false;
		HINSTANCE hModuleInstance = AfxGetInstanceHandle();
		HRSRC hResourceHandle = ::FindResource(hModuleInstance, MAKEINTRESOURCE(uResourceID), L"PNG");
		if (0 == hResourceHandle) {
			return bRet;
		}
		DWORD nImageSize = ::SizeofResource(hModuleInstance, hResourceHandle);
		if (0 == nImageSize) {
			return bRet;
		}
		HGLOBAL hResourceInstance = ::LoadResource(hModuleInstance, hResourceHandle);
		if (0 == hResourceInstance) {
			return bRet;
		}
		const void* pResourceData = ::LockResource(hResourceInstance);
		if (0 == pResourceData) {
			FreeResource(hResourceInstance);
			return bRet;
		}
		HGLOBAL hBuffer = ::GlobalAlloc(GMEM_MOVEABLE, nImageSize);
		if (0 == hBuffer) {
			FreeResource(hResourceInstance);
			return bRet;
		}
		void* pBuffer = ::GlobalLock(hBuffer);
		if (0 != pBuffer) {
			CopyMemory(pBuffer, pResourceData, nImageSize);
			IStream* pStream = 0;
			if (S_OK == ::CreateStreamOnHGlobal(hBuffer, FALSE, &pStream)) {
				CImage ImageFromResource;
				ImageFromResource.Load(pStream);
				pStream->Release();
				BitmapOut.Attach(ImageFromResource.Detach());
				bRet = true;
			}
			::GlobalUnlock(hBuffer);
		}
		::GlobalFree(hBuffer);
		UnlockResource(hResourceInstance);
		FreeResource(hResourceInstance);
		return bRet;
	}
protected:
	DECLARE_MESSAGE_MAP()
public:
	CFont m_font;
	CStatic m_version;
	CStatic m_url;
	CStatic m_copyright;
	XButton m_close;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_VERSION, m_version);
	DDX_Control(pDX, IDC_STATIC_URL, m_url);
	DDX_Control(pDX, IDC_STATIC_COPYRIGHT, m_copyright);
	DDX_Control(pDX, IDOK, m_close);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
END_MESSAGE_MAP()

// ��ȭ ���ڸ� �����ϱ� ���� ���� ���α׷� ����Դϴ�.
void CTSpringApp::OnAppAbout()
{
	if (GetApp().g_exporting == true) {
		return;
	}
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CTSpringApp �޽��� ó����





HBRUSH CAboutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) {
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	if (nCtlColor == CTLCOLOR_STATIC) {
		pDC->SetBkColor(GetTheme().ColorBK());
		pDC->SetTextColor(GetTheme().ColorText());
	}
	if (nCtlColor == CTLCOLOR_BTN) {
		pDC->SetBkMode(TRANSPARENT);
	}
	return hbr;
}


BOOL CAboutDlg::OnInitDialog() {
	CDialogEx::OnInitDialog();
	CRect rect;
	CDC* DC = this->GetDC();
	int h = 0;
	h = mspring::Font::GetRealFontHeight(GetTheme().Font(), 20, DC);
	m_font.CreatePointFont(h, GetTheme().Font().c_str());
	CString str;
	CSize sz;
	m_version.SetFont(&m_font);
	m_version.GetWindowText(str);
	DC->SelectObject(&m_font);
	GetTextExtentPoint32(DC->GetSafeHdc(), str, str.GetLength(), &sz);
	m_version.GetWindowRect(rect);
	ScreenToClient(rect);
	rect.right = rect.left + sz.cx;
	rect.bottom = rect.top + sz.cy;
	m_version.MoveWindow(rect);


	m_url.SetFont(&m_font);
	m_url.SetFont(&m_font);
	m_url.GetWindowText(str);
	DC->SelectObject(&m_font);
	GetTextExtentPoint32(DC->GetSafeHdc(), str, str.GetLength(), &sz);
	m_url.GetWindowRect(rect);
	ScreenToClient(rect);
	rect.right = rect.left + sz.cx;
	rect.bottom = rect.top + sz.cy;
	m_url.MoveWindow(rect);

	m_copyright.SetFont(&m_font);
	m_copyright.SetFont(&m_font);
	m_copyright.SetFont(&m_font);
	m_copyright.GetWindowText(str);
	DC->SelectObject(&m_font);
	GetTextExtentPoint32(DC->GetSafeHdc(), str, str.GetLength(), &sz);
	m_copyright.GetWindowRect(rect);
	ScreenToClient(rect);
	rect.right = rect.left + sz.cx;
	rect.bottom = rect.top + sz.cy;
	m_copyright.MoveWindow(rect);

	m_close.SetFont(&m_font);
	m_close.ModifyStyle(1, BS_OWNERDRAW);
	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	ReleaseDC(DC);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


BOOL CAboutDlg::OnEraseBkgnd(CDC* pDC) {
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, GetTheme().ColorBK());
	return TRUE;
}


void CAboutDlg::OnPaint() {
	CPaintDC dc(this);
	CRect rect;
	rect.left = 300;
	rect.top = 20;
	rect.right = rect.left + 64;
	rect.bottom = rect.top + 64;
	CDC* pDC = &dc;
	CDC mDC;
	BITMAP bmp_info;
	mDC.CreateCompatibleDC(pDC);
	CBitmap bmp, *old_bmp = nullptr;
	static std::map<UINT, CBitmap*> cbmp_map;

	bmp.LoadBitmap(IDB_TSPRING);

	bmp.GetBitmap(&bmp_info);
	old_bmp = mDC.SelectObject(&bmp);
	int w = rect.Width();
	int h = rect.Height();
	if (bmp_info.bmWidth != rect.Width() || bmp_info.bmHeight != rect.Height()) {
		w = bmp_info.bmWidth*rect.Height() / bmp_info.bmHeight;
		h = rect.Height();
		if (w > rect.Width()) {
			w = rect.Width();
			h = bmp_info.bmHeight*rect.Width() / bmp_info.bmWidth;
		}
	}
	int x = (rect.Width() - w) / 2 + rect.left;
	int y = (rect.Height() - h) / 2 + rect.top;
	pDC->SetStretchBltMode(HALFTONE);
	dc.TransparentBlt(x, y, w, h, &mDC, 0, 0, bmp_info.bmWidth, bmp_info.bmHeight, RGB(255,255,255));
	mDC.SelectObject(old_bmp);
	bmp.DeleteObject();
}
