
// TSpring.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
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


// CTSpringApp 생성

CTSpringApp::CTSpringApp()
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(104);
	//_CrtMemDumpAllObjectsSince(0);
	// TODO: 아래 응용 프로그램 ID 문자열을 고유 ID 문자열로 바꾸십시오(권장).
	// 문자열에 대한 서식: CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("TSpring.AppID.NoVersion"));

	timeBeginPeriod(1);
	//SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);

	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}

// 유일한 CTSpringApp 개체입니다.

CTSpringApp theApp;


// CTSpringApp 초기화

BOOL CTSpringApp::InitInstance()
{
	AfxOleInit();
	
	CWinApp::InitInstance();


	EnableTaskbarInteraction(FALSE);

	// RichEdit 컨트롤을 사용하려면  AfxInitRichEdit2()가 있어야 합니다.	
	// AfxInitRichEdit2();

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("로컬 응용 프로그램 마법사에서 생성된 응용 프로그램"));


	// 주 창을 만들기 위해 이 코드에서는 새 프레임 창 개체를
	// 만든 다음 이를 응용 프로그램의 주 창 개체로 설정합니다.
	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// 프레임을 만들어 리소스와 함께 로드합니다.
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);





	// 창 하나만 초기화되었으므로 이를 표시하고 업데이트합니다.
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	return TRUE;
}

int CTSpringApp::ExitInstance()
{
	//TODO: 추가한 추가 리소스를 처리합니다.
	return CWinApp::ExitInstance();
}

// CTSpringApp 메시지 처리기


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();
	~CAboutDlg() {
		m_font.DeleteObject();
	}
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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

// 대화 상자를 실행하기 위한 응용 프로그램 명령입니다.
void CTSpringApp::OnAppAbout()
{
	if (GetApp().g_exporting == true) {
		return;
	}
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CTSpringApp 메시지 처리기





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
	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	ReleaseDC(DC);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
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
