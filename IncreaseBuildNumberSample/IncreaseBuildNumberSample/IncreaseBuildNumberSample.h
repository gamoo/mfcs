
// IncreaseBuildNumberSample.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CIncreaseBuildNumberSampleApp:
// �� Ŭ������ ������ ���ؼ��� IncreaseBuildNumberSample.cpp�� �����Ͻʽÿ�.
//

class CIncreaseBuildNumberSampleApp : public CWinApp
{
public:
	CIncreaseBuildNumberSampleApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CIncreaseBuildNumberSampleApp theApp;