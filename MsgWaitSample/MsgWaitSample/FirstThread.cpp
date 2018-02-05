#include "stdafx.h"
#include "FirstThread.h"


FirstThread::FirstThread() :
	m_bWaitFlag(TRUE)
{
	m_pEvent = new CEvent();
}


FirstThread::~FirstThread()
{
}

BOOL FirstThread::InitInstance()
{
	

	return TRUE;
}

int FirstThread::Run()
{
	while (m_bWaitFlag) {

		Sleep(1000);
		m_pEvent->SetEvent();
		break;
	}

	return 0;
}