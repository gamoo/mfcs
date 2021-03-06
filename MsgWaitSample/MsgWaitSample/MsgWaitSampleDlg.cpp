
// MsgWaitSampleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MsgWaitSample.h"
#include "MsgWaitSampleDlg.h"
#include "afxdialogex.h"

#include "FirstThread.h"
#include "SecondThread.h"

#include "resource.h"

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


// CMsgWaitSampleDlg dialog



CMsgWaitSampleDlg::CMsgWaitSampleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MSGWAITSAMPLE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pFirstThread = new FirstThread();
	m_pSecondThread = new SecondThread();
}

void CMsgWaitSampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMsgWaitSampleDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMsgWaitSampleDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMsgWaitSampleDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CMsgWaitSampleDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CMsgWaitSampleDlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// CMsgWaitSampleDlg message handlers

BOOL CMsgWaitSampleDlg::OnInitDialog()
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

void CMsgWaitSampleDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMsgWaitSampleDlg::OnPaint()
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
HCURSOR CMsgWaitSampleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMsgWaitSampleDlg::OnBnClickedButton1()
{
	
	m_pFirstThread->CreateThread();
	m_pSecondThread->CreateThread();

	BOOL bWaitFlag = TRUE;
	BOOL bSignaled[2] = { FALSE, FALSE };
	HANDLE handles[2] = { m_pFirstThread->m_pEvent->m_hObject, m_pSecondThread->m_pEvent->m_hObject };

	while (bWaitFlag) {
		/*
		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {

			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		*/

		///DWORD dwResult = ::MsgWaitForMultipleObjects(2, handles, FALSE, INFINITE, QS_INPUT);
		DWORD dwResult = ::MsgWaitForMultipleObjectsEx(2, handles, INFINITE, QS_INPUT, 0);

		DWORD dwError;
		CString msg2;
		/*	msg2.Format(_T("%d\n"), dwResult);
		::OutputDebugString(msg2);*/

		switch (dwResult) {

		case WAIT_OBJECT_0 :
			::OutputDebugString(_T("WAIT_OBJECT_0\n"));
			bSignaled[0] = TRUE;
			break;
		case WAIT_OBJECT_0 + 1:
			::OutputDebugString(_T("WAIT_OBJECT_0 + 1\n"));
			bSignaled[1] = TRUE;
			break;
		case WAIT_ABANDONED_0:
			::OutputDebugString(_T("WAIT_ABANDONED_0\n"));
			break;
		case WAIT_ABANDONED_0 + 1:
			::OutputDebugString(_T("WAIT_ABANDONED_1\n"));
			break;

		case WAIT_TIMEOUT:
			::OutputDebugString(_T("WAIT_TIMEOUT\n"));
			break;

		case WAIT_FAILED:
			/*
			::OutputDebugString(_T("WAIT_FAILED\n"));

			dwError = ::GetLastError();

			msg2.Format(_T("%d\n"), dwError);
			::OutputDebugString(msg2);
			*/
			break;
		
		default:
			//::OutputDebugString(_T("default"));
			AfxPumpMessage();
			break;
		}

		if (bSignaled[0] && bSignaled[1]) {
			break;
		}
	}
	
	AfxMessageBox(_T("Terminated!"));
}


// Exit Test
void CMsgWaitSampleDlg::OnBnClickedButton2()
{
	m_pFirstThread->m_bWaitFlag = FALSE;
}


void CMsgWaitSampleDlg::OnBnClickedButton3()
{
	m_pSecondThread->m_bWaitFlag = FALSE;
}

#include <Windows.h>
#include <winioctl.h>
void CMsgWaitSampleDlg::OnBnClickedButton4()
{
	int code = IOCTL_DISK_REASSIGN_BLOCKS_EX;

	AfxMessageBox(_T("Test"));
}
