
// iTunesServiceSampleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "iTunesServiceSample.h"
#include "iTunesServiceSampleDlg.h"
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


// CiTunesServiceSampleDlg dialog



CiTunesServiceSampleDlg::CiTunesServiceSampleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_ITUNESSERVICESAMPLE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CiTunesServiceSampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CiTunesServiceSampleDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CiTunesServiceSampleDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CiTunesServiceSampleDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CiTunesServiceSampleDlg message handlers

BOOL CiTunesServiceSampleDlg::OnInitDialog()
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

void CiTunesServiceSampleDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CiTunesServiceSampleDlg::OnPaint()
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
HCURSOR CiTunesServiceSampleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CiTunesServiceSampleDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here

	CString strPath = _T("");

	TCHAR path[MAX_PATH];

	// CSIDL_PROGRAM_FILES    => C:\Program Files (x86)?
	// CSIDL_PROGRAM_FILESX86 => C:\Program Files (x86)

	if (::SHGetSpecialFolderPath(0, path, CSIDL_PROGRAM_FILES, FALSE)) {

		AfxMessageBox(path);
	}

}

#include <Psapi.h>

void PrintProcessNameAndID(DWORD processID)
{
	TCHAR szProcessName[MAX_PATH] = _T("<unknown>");
	TCHAR szFileName[MAX_PATH] = _T("<unknown>");

	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);

	if (hProcess != NULL) {

		HMODULE hModule;
		DWORD cbNeeded;

		if (EnumProcessModules(hProcess, &hModule, sizeof(hModule), &cbNeeded)) {

			GetModuleBaseName(hProcess, hModule, szProcessName, sizeof(szProcessName) / sizeof(TCHAR));
			GetModuleFileNameEx(hProcess, NULL, szFileName, MAX_PATH);
			/*
			TRACE(_T("%s (PID: %u)\n"), szProcessName, processID);
			TRACE(_T("%s"), szFileName);
			*/
			OutputDebugString(szFileName);
			OutputDebugString(_T("\n"));

			// iTunes, AppleMobileDevice는 목록에 없네 ㅠ 
		}
	}

	CloseHandle(hProcess);
}

void CiTunesServiceSampleDlg::OnBnClickedButton2()
{
	//HANDLE processHandle = nullptr;

	//processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, 1234);
	
	DWORD aProcesses[1024], cbNeeded, cProcesses;
	unsigned int i;

	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) {
		return;
	}

	cProcesses = cbNeeded / sizeof(DWORD);

	for (i = 0; i < cProcesses; i++) {

		if (aProcesses[i] != 0) {

			PrintProcessNameAndID(aProcesses[i]);
		}
	}
}




