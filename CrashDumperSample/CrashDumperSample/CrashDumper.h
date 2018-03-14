#pragma once

#include <vector>
#include <map>

namespace MobigoSecure {

	using PathPair = std::pair<CString, CString>;

	using PathList = std::vector<CString>;
	using PairList = std::vector<PathPair>;

	class CrashDumper {

	public:

		static CrashDumper* GetInstance();

		void Enable();
		static void Test();

	protected:

		CrashDumper();
		virtual ~CrashDumper();

		static LONG WINAPI ExceptionFilter(_In_ struct _EXCEPTION_POINTERS *pExceptionInfo);

		static BOOL WriteCommandToLog(CString strCommand, CString strFilePath);
		static BOOL WriteDump(CString strFilePath, struct _EXCEPTION_POINTERS *pExceptionInfo);

		static BOOL SendCrashReport(PathList& pathList);

		static CrashDumper* m_pInstance;

	};

} // namespace MobigoSecure
