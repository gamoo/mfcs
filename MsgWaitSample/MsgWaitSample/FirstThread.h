#pragma once

class FirstThread : public CWinThread
{
public:
	FirstThread();
	virtual ~FirstThread();

	virtual BOOL InitInstance();
	virtual int Run();

	BOOL m_bWaitFlag;
	CEvent *m_pEvent;
};

