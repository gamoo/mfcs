#include "stdafx.h"
#include "SecondThread.h"


SecondThread::SecondThread() :
	m_bWaitFlag(TRUE)
{
	m_pEvent = new CEvent();
}


SecondThread::~SecondThread()
{
}

BOOL SecondThread::InitInstance()
{
	
	return TRUE;
}

int SecondThread::Run()
{
	while (m_bWaitFlag) {
	
		Sleep(2000);
		m_pEvent->SetEvent();
		break;
	}

	return 0;
}