// GetSystemInfoConsoleSample.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>

#include <iostream>

int main()
{
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	HANDLE h = CreateFile(_T("D:\\out.log"),
		GENERIC_WRITE,
		FILE_SHARE_WRITE | FILE_SHARE_READ,
		&sa,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	BOOL ret = FALSE;
	DWORD flags = CREATE_NO_WINDOW;
	//DWORD flags = CREATE_NEW_CONSOLE;

	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	ZeroMemory(&si, sizeof(STARTUPINFO));

	si.cb = sizeof(STARTUPINFO);
	si.dwFlags |= STARTF_USESTDHANDLES;
	si.hStdInput = NULL;
	si.hStdError = h;
	si.hStdOutput = h;

	TCHAR cmd[] = TEXT("systeminfo");
	TCHAR cmd2[] = TEXT("wmic logicaldisk get size, freespace, caption"); // console로는 안보이네
	TCHAR cmd3[] = TEXT("reg query \"HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Internet Explorer\" /v svcVersion");
	TCHAR cmd4[] = TEXT("ipconfig");

	// chrome 버전은 필요할까? 동작 안되네...
	TCHAR cmd5[] = TEXT("wmic datafile where name=\"C:\\Program Files (x86)\\Google\\Chrome\\Application\\chrome.exe\" get Version /value"); 

	ret = CreateProcess(NULL, cmd4, NULL, NULL, TRUE, flags, NULL, NULL, &si, &pi);

	if (ret)
	{
		// 20초 정도 20000
		WaitForSingleObject(pi.hProcess, INFINITE); 

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

		CloseHandle(h);
		
		h = CreateFile(_T("D:\\out.log"),
			GENERIC_READ,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		
		SetFilePointer(h, 0, NULL, FILE_BEGIN);

		char buf[1024];
		DWORD readn;
		while (true) {

			ZeroMemory(buf, 1024);
			BOOL r = ReadFile(h, buf, 1023, &readn, NULL);

			DWORD dwError = GetLastError();
			if (readn != 0) {
				std::cout << buf << std::endl;
			}
			
			if (r && readn == 0) {
				break;
			}
		}

		return 0;
	}

	return -1;
}