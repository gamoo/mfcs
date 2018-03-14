#include "stdafx.h"
#include "CrashDumper.h"

#include <algorithm>
#include <afxinet.h>
#include <dbghelp.h>

#pragma comment(lib, "Dbghelp.lib")

using namespace MobigoSecure;

const CString DEFAULT_SERVER_ADDRESS = _T("192.168.0.182");
const INTERNET_PORT DEFAULT_SERVER_PORT = 1337U;

const DWORD DUMP_PROCESS_TIMEOUT = 20000;
const CString SYSTEMINFO_COMMAND = _T("systeminfo");
const CString STORAGEINFO_COMMAND = _T("wmic logicaldisk get size, freespace, caption");
const CString IEINFO_COMMAND = _T("reg query \"HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Internet Explorer\" /v svcVersion");
const CString IPINFO_COMMAND = _T("ipconfig");

const CString DEFAULT_MOBIGO_PATH = _T("C:\\MobigoSecure");
const CString DEFAULT_DUMP_PATHNAME = _T("dump");

const CString DEFAULT_SYSTEM_LOG_FILE = _T("system");
const CString DEFAULT_STORAGE_LOG_FILE = _T("storage");
const CString DEFAULT_IEVERSION_LOG_FILE = _T("ieversion");
const CString DEFAULT_IPCONFIG_LOG_FILE = _T("ipconfig");
const CString DEFAULT_DUMP_FILE = _T("dump");

CrashDumper* CrashDumper::m_pInstance = nullptr;

CrashDumper* CrashDumper::GetInstance()
{
	if (m_pInstance == nullptr) {
		m_pInstance = new CrashDumper();
	}

	return m_pInstance;
}

void CrashDumper::Enable()
{
	SetUnhandledExceptionFilter(CrashDumper::ExceptionFilter);
}

void CrashDumper::Test()
{
	ExceptionFilter(nullptr);
}

CrashDumper::CrashDumper()
{
}

CrashDumper::~CrashDumper()
{
}

LONG WINAPI CrashDumper::ExceptionFilter(struct _EXCEPTION_POINTERS *pExceptionInfo)
{
	CString strPath = _T("");
	CString strDump = _T("");
	CString strDate = CTime::GetCurrentTime().Format(_T("%y%m%d"));
	CString strTime = CTime::GetCurrentTime().Format(_T("%H%M%S"));

	TCHAR buffer[MAX_PATH];
	DWORD dwResult = GetCurrentDirectory(MAX_PATH, buffer);

	// TODO: GetModuleFileName으로 바꾸어야 함

	if (dwResult == 0) {

		OutputDebugString(_T("Could not get current directory."));
		strDump.Format(_T("%s\\%s"), DEFAULT_MOBIGO_PATH, DEFAULT_DUMP_PATHNAME);
		strPath.Format(_T("%s\\%s"), strDump, strDate);
	}
	else {

		strDump.Format(_T("%s\\%s"), buffer, DEFAULT_DUMP_PATHNAME);
		strPath.Format(_T("%s\\%s"), strDump, strDate);
	}

	if (GetFileAttributes(strDump) == INVALID_FILE_ATTRIBUTES) {
		CreateDirectory(strDump, NULL);
	}

	if (GetFileAttributes(strPath) == INVALID_FILE_ATTRIBUTES) {
		CreateDirectory(strPath, NULL);
	}

	PathList pathList;
	CString strDumpFilePath;
	strDumpFilePath.Format(_T("%s\\%s_%s.dmp"), strPath, DEFAULT_DUMP_FILE, strTime);

	WriteDump(strDumpFilePath, pExceptionInfo);

	CString strSystemLogPath, strStorageLogPath, strIEVersionLogPath, strIpconfigLogPath;

	strSystemLogPath.Format(_T("%s\\%s_%s.log"), strPath, DEFAULT_SYSTEM_LOG_FILE, strTime);
	strStorageLogPath.Format(_T("%s\\%s_%s.log"), strPath, DEFAULT_STORAGE_LOG_FILE, strTime);
	strIEVersionLogPath.Format(_T("%s\\%s_%s.log"), strPath, DEFAULT_IEVERSION_LOG_FILE, strTime);
	strIpconfigLogPath.Format(_T("%s\\%s_%s.log"), strPath, DEFAULT_IPCONFIG_LOG_FILE, strTime);

	WriteCommandToLog(SYSTEMINFO_COMMAND, strSystemLogPath);
	WriteCommandToLog(STORAGEINFO_COMMAND, strStorageLogPath);
	WriteCommandToLog(IEINFO_COMMAND, strIEVersionLogPath);
	WriteCommandToLog(IPINFO_COMMAND, strIpconfigLogPath);

	pathList.emplace_back(strSystemLogPath);
	pathList.emplace_back(strStorageLogPath);
	pathList.emplace_back(strIEVersionLogPath);
	pathList.emplace_back(strIpconfigLogPath);
	pathList.emplace_back(strDumpFilePath);

	SendCrashReport(pathList);

	return EXCEPTION_CONTINUE_SEARCH;
}

BOOL CrashDumper::WriteCommandToLog(CString strCommand, CString strFilePath) 
{
	SECURITY_ATTRIBUTES sa;

	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	HANDLE hFile = CreateFile(strFilePath.GetBuffer(), GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, &sa, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE) {

		OutputDebugString(_T("Could not create file."));

		return FALSE;
	}

	PROCESS_INFORMATION pi;
	STARTUPINFO si;

	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	ZeroMemory(&si, sizeof(STARTUPINFO));

	si.cb = sizeof(STARTUPINFO);
	si.dwFlags |= STARTF_USESTDHANDLES;
	si.hStdInput = NULL;
	si.hStdError = hFile;
	si.hStdOutput = hFile;

	BOOL bResult = CreateProcess(NULL, strCommand.GetBuffer(), NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);

	if (bResult == TRUE) {

		WaitForSingleObject(pi.hProcess, DUMP_PROCESS_TIMEOUT);

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	else {

		OutputDebugString(_T("Cound not create process."));

		CloseHandle(hFile);

		return FALSE;
	}

	CloseHandle(hFile);

	return TRUE;
}

BOOL CrashDumper::WriteDump(CString strFilePath, struct _EXCEPTION_POINTERS *pExceptionInfo)
{
	_MINIDUMP_EXCEPTION_INFORMATION	stExceptInfo = { 0, };

	HANDLE hFile = CreateFile(strFilePath.GetBuffer(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile != INVALID_HANDLE_VALUE) {

		stExceptInfo.ThreadId = ::GetCurrentThreadId();
		stExceptInfo.ExceptionPointers = pExceptionInfo;
		stExceptInfo.ClientPointers = NULL;

		::MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &stExceptInfo, NULL, NULL);

		CloseHandle(hFile);

		return TRUE;
	}

	return FALSE;
}

BOOL CrashDumper::SendCrashReport(PathList& pathList)
{
	CInternetSession internetSession;

	CString strPrefix = _T("");
	CString strSuffix = _T("");

	strSuffix.Format(_T("\r\n\r\n------WebKitFormBoundary7MA4YWxkTrZu0gW--\r\n"));

	_off_t tsize = strSuffix.GetLength();
	PairList pairList;

	std::for_each(pathList.begin(), pathList.end(), [&](CString& c) {

		struct _stat buf;
		_tstat(c.GetBuffer(), &buf);

		strPrefix.Format(_T("\r\n------WebKitFormBoundary7MA4YWxkTrZu0gW\r\nContent-Disposition: form-data; name=\"file\"; filename=\"%s\"\r\nContent-Type: application/octet-stream\r\n\r\n"), c.Mid(c.ReverseFind(_T('\\')) + 1));

		pairList.emplace_back(std::make_pair(c, strPrefix));

		tsize += buf.st_size;
		tsize += strPrefix.GetLength();
	});

	CString strHeaders = _T("");

	strHeaders += _T("User-Agent: MobigoSecure\r\n");
	strHeaders += _T("Cache-Control: no-cache\r\n");
	strHeaders += _T("Content-Type: multipart/form-data; boundary=----WebKitFormBoundary7MA4YWxkTrZu0gW\r\n");
	
	try {

		CHttpConnection *pHttpConnection = internetSession.GetHttpConnection(DEFAULT_SERVER_ADDRESS, (INTERNET_PORT)DEFAULT_SERVER_PORT, NULL, NULL);

		CHttpFile *pHttpFile = pHttpConnection->OpenRequest(CHttpConnection::HTTP_VERB_POST, _T("/dump/upload"));

		pHttpFile->AddRequestHeaders(strHeaders);

		pHttpFile->SendRequestEx(DWORD(tsize), HSR_SYNC | HSR_INITIATE);

#ifdef UNICODE
		USES_CONVERSION;
#endif
		std::for_each(pairList.begin(), pairList.end(), [&](std::pair<CString, CString> m) {

#ifdef UNICODE
			pHttpFile->Write(W2A(m.second), m.second.GetLength());
#else 
			pHttpFile->Write(m.second, m.second.GetLength());
#endif

			CFile file;
			if (file.Open(m.first, CFile::modeRead | CFile::typeBinary)) {

				UINT len = 65536;
				char buf[65536];
				
				while (len > 0) {

					len = file.Read(buf, 65535);
					if (len > 0) {
						pHttpFile->Write(buf, len);
					}
					pHttpFile->Flush();
				}

				file.Close();
			}

		});

#ifdef UNICODE
		pHttpFile->Write(W2A(strSuffix.GetBuffer()), strSuffix.GetLength());
#else
		pHttpFile->Write(strSuffix.GetBuffer(), strSuffix.GetLength());
#endif

		pHttpFile->EndRequest(HSR_SYNC);

		// Response
		DWORD dwStatusCode;

		pHttpFile->QueryInfoStatusCode(dwStatusCode);

		TCHAR szResp[1024];

		while (pHttpFile->ReadString(szResp, 1024)) {
			
			// TODO: UTF8 처리
			AfxMessageBox(szResp);
			//char *szBuf = W2A(szResp);
			//OutputDebugString(A2W(szBuf));
		}
		
		pHttpFile->Close();
		pHttpConnection->Close();

		delete pHttpFile;
		delete pHttpConnection;

	}
	catch (CInternetException& ex) {
		
		ex.ReportError();
		OutputDebugString(_T("Could not send files to the server."));

		return FALSE;
	}

	return TRUE;
}