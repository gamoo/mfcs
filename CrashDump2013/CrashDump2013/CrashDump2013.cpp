// CrashDump2013.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "SelfDump.h"

#pragma comment(lib, "Dbghelp.lib")

// source
// http://greenfishblog.tistory.com/19

void crash()
{
	int *p = nullptr;

	*p = 10;
}


static LONG WINAPI UnhandledExceptionFilterFunc(_In_ struct _EXCEPTION_POINTERS *pExceptionInfo)
{

	HANDLE hFile = INVALID_HANDLE_VALUE;

	_MINIDUMP_EXCEPTION_INFORMATION	stExceptInfo = { 0, };

	hFile = CreateFile(_T("Dump.dmp"), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile != INVALID_HANDLE_VALUE) {

		stExceptInfo.ThreadId = ::GetCurrentThreadId();
		stExceptInfo.ExceptionPointers = pExceptionInfo;
		stExceptInfo.ClientPointers = NULL;

		::MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &stExceptInfo, NULL, NULL);

		CloseHandle(hFile);
	}

	return EXCEPTION_CONTINUE_SEARCH;
}

int _tmain(int argc, _TCHAR* argv[])
{
	/*
	CSelfDump dump;

	dump.RegisterExceptionFilter();
	*/

	SetUnhandledExceptionFilter(UnhandledExceptionFilterFunc);

	crash();

	return 0;
}

