
// IncreaseBuildNumberSampleDlg.h : ��� ����
//

#pragma once


// CIncreaseBuildNumberSampleDlg ��ȭ ����
class CIncreaseBuildNumberSampleDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CIncreaseBuildNumberSampleDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_INCREASEBUILDNUMBERSAMPLE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};