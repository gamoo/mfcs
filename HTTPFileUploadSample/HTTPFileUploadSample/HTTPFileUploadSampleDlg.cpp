
// HTTPFileUploadSampleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HTTPFileUploadSample.h"
#include "HTTPFileUploadSampleDlg.h"
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


// CHTTPFileUploadSampleDlg dialog



CHTTPFileUploadSampleDlg::CHTTPFileUploadSampleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_HTTPFILEUPLOADSAMPLE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHTTPFileUploadSampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CHTTPFileUploadSampleDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CHTTPFileUploadSampleDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CHTTPFileUploadSampleDlg message handlers

BOOL CHTTPFileUploadSampleDlg::OnInitDialog()
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

void CHTTPFileUploadSampleDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CHTTPFileUploadSampleDlg::OnPaint()
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
HCURSOR CHTTPFileUploadSampleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

#include <afxinet.h>


void CHTTPFileUploadSampleDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE);

	INT_PTR nResult = dlg.DoModal();

	if (nResult == IDOK) {

		CString strFileName = dlg.GetPathName();

		AfxMessageBox(strFileName);

		CString strServerName = _T("127.0.0.1");
		CInternetSession internetSession;

		CHttpConnection *pHttpConnection = internetSession.GetHttpConnection(strServerName, (INTERNET_PORT)3000U, NULL, NULL);
		
		CHttpFile *pHttpFile = pHttpConnection->OpenRequest(CHttpConnection::HTTP_VERB_POST, _T("/upload/dump.dmp"));

		//pHttpFile->AddRequestHeaders(_T("Accept: text/*\r\nContent-Type: multipart/form-data; boundary=---------------------------7d13a23b368\r\nUser-Agent: VCApp\r\n"));
		pHttpFile->AddRequestHeaders(_T("Content-Type: application/x-www-form-urlencoded"));

		CString strPrefix;

		strPrefix.Format(_T("\r\n-----------------------------7d13a23b368\r\nContent-Disposition: form-data; name=\"upfile\"; filename=\"%s\"\r\nContent-Type: application/octet-stream\r\n\r\n"), strFileName);

		DWORD dwPrefix = strPrefix.GetLength();

		CString strSuffix = _T("\r\n\r\n-----------------------------7d13a23b368--\r\n");

		DWORD dwSuffix = strSuffix.GetLength();

		CFile file;
		CFileStatus status;

		file.Open(strFileName, CFile::modeRead | CFile::typeBinary);

		file.GetStatus(status);

		DWORD dwDataLength = dwPrefix + status.m_size + dwSuffix;

		BYTE *pData, *pCurrent;

		pData = pCurrent = new BYTE[status.m_size];

		pCurrent += dwPrefix;

		pCurrent += file.Read((void*)pData, status.m_size);

		//memcpy((void*)pCurrent, (const void*)strSuffix.GetBuffer(), dwSuffix);

		try {

			pHttpFile->SendRequest(NULL, 0, (LPVOID)pData, status.m_size);

			DWORD dwRet;

			pHttpFile->QueryInfoStatusCode(dwRet);

			TCHAR szResp[1024];

			while (pHttpFile->ReadString(szResp, 1024)) {

				TRACE(_T("%s\n", szResp));

				delete pData;
			}
		}
		catch (CInternetException& ex) {
			
			TRACE(_T("Caught Exception (%d)\n"), ex.m_dwError);
		}

		pHttpFile->Close();

		pHttpConnection->Close();

		delete pHttpFile;
		delete pHttpConnection;

	}
}
