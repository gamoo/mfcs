
// MsgWaitSampleDlg.h : header file
//

#pragma once

class FirstThread;
class SecondThread;

// CMsgWaitSampleDlg dialog
class CMsgWaitSampleDlg : public CDialog
{
// Construction
public:
	CMsgWaitSampleDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MSGWAITSAMPLE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();

	FirstThread *m_pFirstThread;
	SecondThread * m_pSecondThread;
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
};
