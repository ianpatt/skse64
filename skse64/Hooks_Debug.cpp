#include "Hooks_Debug.h"
#include "common/IInterlockedLong.h"
#include "common/IFileStream.h"
#include "skse64_common/Utilities.h"
#include "skse64_common/SafeWrite.h"
#include "skse64_common/skse_version.h"
#include <dbghelp.h>
#include <shlobj.h>

static HMODULE	s_dbgHelpDLL = NULL;

typedef BOOL (__stdcall * _MiniDumpWriteDump)(HANDLE hProcess, DWORD ProcessId, HANDLE hFile, MINIDUMP_TYPE DumpType, CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam, CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam, CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam);
static _MiniDumpWriteDump s_dbgHelpWriteDump = NULL;

static LPTOP_LEVEL_EXCEPTION_FILTER s_oldExceptionFilter = NULL;
static SYSTEMTIME					s_launchTime;
static char							s_crashDumpPath[MAX_PATH];

static IInterlockedLong				s_inExceptionFilter;

// ignore these crash points to make dumps less spammy
const char * IsKnownCrash(EXCEPTION_POINTERS * info)
{
	return NULL;
}

LONG WINAPI ExceptionFilter(EXCEPTION_POINTERS * info)
{
	if(s_inExceptionFilter.Claim())
	{
		bool		writeDump = true;
		const char	* crashReason = IsKnownCrash(info);

		if(crashReason)
		{
			_ERROR("Skyrim has crashed in a known crash location (%s). No crashdump will be written in release builds.", crashReason);

#if NDEBUG
			// skip writing crashdumps for these in release
			writeDump = false;
#endif
		}
		
		if(writeDump)
		{
			_ERROR("Skyrim has crashed. A minidump containing debugging information is being written to %s.", s_crashDumpPath);

			HANDLE	dumpFile = CreateFile(s_crashDumpPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if(dumpFile != INVALID_HANDLE_VALUE)
			{
				MINIDUMP_EXCEPTION_INFORMATION	exceptionInfo;

				exceptionInfo.ThreadId = GetCurrentThreadId();
				exceptionInfo.ExceptionPointers = info;
				exceptionInfo.ClientPointers = FALSE;

				BOOL	result = s_dbgHelpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), dumpFile, MiniDumpNormal, &exceptionInfo, NULL, NULL);
				if(result)
				{
					_ERROR("Minidump written.");
				}
				else
				{
					_ERROR("Unable to write minidump.");
				}

				CloseHandle(dumpFile);
			}
			else
			{
				_ERROR("Unable to open minidump. (%08X)", GetLastError());
			}
		}

		s_inExceptionFilter.Release();
	}
	else
	{
		_ERROR("top-level exception filter hit multiple times");
	}

	return EXCEPTION_EXECUTE_HANDLER;
}

LPTOP_LEVEL_EXCEPTION_FILTER WINAPI SetUnhandledExceptionFilter_Hook(__in LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter)
{
	_MESSAGE("SetUnhandledExceptionFilter_Hook: %p", lpTopLevelExceptionFilter);

	return NULL;
}

void Hooks_Debug_Init(void)
{
	GetSystemTime(&s_launchTime);

	UInt32	enableMiniDump = 0;
	GetConfigOption_UInt32("Debug", "WriteMinidumps", &enableMiniDump);

	if(enableMiniDump)
	{
		_MESSAGE("minidumps enabled");

		// try to get dbghelp
		s_dbgHelpDLL = LoadLibrary("dbghelp.dll");
		if(s_dbgHelpDLL)
		{
			s_dbgHelpWriteDump = (_MiniDumpWriteDump)GetProcAddress(s_dbgHelpDLL, "MiniDumpWriteDump");
			if(!s_dbgHelpWriteDump)
				_WARNING("dbghelp missing MiniDumpWriteDump, upgrade to dbghelp 5.1 or later");
		}
		else
		{
			_MESSAGE("no dbghelp");
		}

		// we want to catch crashes from hook commit, apply exception filter in Init function
		if(s_dbgHelpDLL && s_dbgHelpWriteDump)
		{
			// precalculate as much as possible
			char	myDocumentsPath[MAX_PATH];
			ASSERT(SUCCEEDED(SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, myDocumentsPath)));

			sprintf_s(s_crashDumpPath, sizeof(s_crashDumpPath), "%s\\My Games\\" SAVE_FOLDER_NAME "\\SKSE\\Crashdumps\\%04d-%02d-%02d_%02d.%02d.%02d.dmp", myDocumentsPath,
				s_launchTime.wYear, s_launchTime.wMonth, s_launchTime.wDay,
				s_launchTime.wHour, s_launchTime.wMinute, s_launchTime.wSecond);

			IFileStream::MakeAllDirs(s_crashDumpPath);

			// replace previous exception filter
			s_oldExceptionFilter = SetUnhandledExceptionFilter(ExceptionFilter);
			_MESSAGE("old exception filter = %p", s_oldExceptionFilter);

			// disable game overwriting exception filter
			uintptr_t	thunkAddress = (uintptr_t)GetIATAddr((UInt8 *)GetModuleHandle(NULL), "kernel32.dll", "SetUnhandledExceptionFilter");
			SafeWrite64(thunkAddress, (uintptr_t)SetUnhandledExceptionFilter_Hook);
		}
	}
}

void Hooks_Debug_Commit(void)
{
	//
}
