#include "skse64_common/skse_version.h"
#include "skse64_common/Utilities.h"
#include "skse64_loader_common/IdentifyEXE.h"
#include "skse64_loader_common/LoaderError.h"
#include "skse64_loader_common/Inject.h"
#include <ShlObj.h>

IDebugLog	gLog;
HANDLE		g_dllHandle;

static void OnAttach(void);

BOOL WINAPI DllMain(HANDLE procHandle, DWORD reason, LPVOID reserved)
{
	if(reason == DLL_PROCESS_ATTACH)
	{
		// at this point we are racing the main thread because we can't launch suspended
		// however we are now inside the app sandbox
		// time to do all kinds of things that you shouldn't in DLL_PROCESS_ATTACH
		g_dllHandle = procHandle;

		OnAttach();

		// kill the launcher
		TerminateProcess(GetCurrentProcess(), 0);
	}

	return TRUE;
}

static void OnAttach(void)
{
	gLog.OpenRelative(CSIDL_MYDOCUMENTS, "\\My Games\\Skyrim Special Edition MS\\SKSE\\skse64_msstore_stub.log");
	gLog.SetPrintLevel(IDebugLog::kLevel_Error);
	gLog.SetLogLevel(IDebugLog::kLevel_DebugMessage);

	FILETIME	now;
	GetSystemTimeAsFileTime(&now);

	_MESSAGE("skse64 loader %08X (msstore) %08X%08X %s", PACKED_SKSE_VERSION, now.dwHighDateTime, now.dwLowDateTime, GetOSInfoStr().c_str());

	std::string runtimeDir = GetRuntimeDirectory();
	std::string procPath = runtimeDir + "SkyrimSE.exe";

	std::string dllSuffix;
	ProcHookInfo info;

	if(!IdentifyEXE(procPath.c_str(), false, &dllSuffix, &info))
		return;

	if(info.procType != kProcType_WinStore)
	{
		PrintLoaderError("Expected windows store app.");
		return;
	}

	std::string	dllPath;

	dllPath = runtimeDir + "skse64_" + dllSuffix + ".dll";

	STARTUPINFO			startupInfo = { 0 };
	PROCESS_INFORMATION	procInfo = { 0 };

	startupInfo.cb = sizeof(startupInfo);

	if(!CreateProcess(
		procPath.c_str(),
		NULL,	// no args
		NULL,	// default process security
		NULL,	// default thread security
		FALSE,	// don't inherit handles
		CREATE_SUSPENDED,
		NULL,	// no new environment
		NULL,	// no new cwd
		&startupInfo, &procInfo))
	{
		PrintLoaderError("Launching %s failed (%d).", procPath.c_str(), GetLastError());

		return;
	}

	_MESSAGE("main thread id = %d", procInfo.dwThreadId);

	if(!InjectDLLThread(&procInfo, dllPath.c_str(), true, false))
	{
		PrintLoaderError("Couldn't inject DLL.");

		_ERROR("terminating process");

		TerminateProcess(procInfo.hProcess, 0);
	}
	else
	{
		_MESSAGE("launching");

		// get out asap
	}
}
