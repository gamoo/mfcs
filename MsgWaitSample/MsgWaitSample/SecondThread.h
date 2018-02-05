#pragma once

class SecondThread : public CWinThread
{
public:
	SecondThread();
	virtual ~SecondThread();

	virtual BOOL InitInstance();
	virtual int Run();

	BOOL m_bWaitFlag;
	CEvent* m_pEvent;
};

