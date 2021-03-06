
// CaptureScreenSampleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CaptureScreenSample.h"
#include "CaptureScreenSampleDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCaptureScreenSampleDlg dialog



CCaptureScreenSampleDlg::CCaptureScreenSampleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_CAPTURESCREENSAMPLE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCaptureScreenSampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCaptureScreenSampleDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CCaptureScreenSampleDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CCaptureScreenSampleDlg message handlers

BOOL CCaptureScreenSampleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCaptureScreenSampleDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCaptureScreenSampleDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCaptureScreenSampleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// http://www.tipssoft.com/bulletin/board.php?bo_table=FAQ&wr_id=1849


void CCaptureScreenSampleDlg::OnBnClickedButton1()
{
	// 화면 전체를 캡쳐하기 위해서 Window DC 를 사용합니다. ::GetDC(NULL) 이라고 해도 결과는 동일합니다.
	HDC h_dc = ::GetWindowDC(NULL);
	// 캡쳐에 사용할 CImage 객체를 선언한다.
	CImage tips_image;
	// 수평 해상도를 얻는다.
	int cx = ::GetSystemMetrics(SM_CXSCREEN);
	// 수직 해상도를 얻는다.
	int cy = ::GetSystemMetrics(SM_CYSCREEN);
	// 화면의 색상 수를 얻는다.
	int color_depth = ::GetDeviceCaps(h_dc, BITSPIXEL);
	// 캡쳐에 사용할 이미지를 생성한다.
	tips_image.Create(cx, cy, color_depth, 0);

	// 화면 전체 이미지를 m_tips_image 객체에 복사한다. 
	::BitBlt(tips_image.GetDC(), 0, 0, cx, cy, h_dc, 0, 0, SRCCOPY);
	// 캡쳐한 이미지를 "test.png" 라는 이름으로 저장한다.
	tips_image.Save(L"test.png", Gdiplus::ImageFormatPNG);

	// 화면 캡쳐에 사용했던 DC를 해제한다.
	::ReleaseDC(NULL, h_dc);
	tips_image.ReleaseDC();
}
