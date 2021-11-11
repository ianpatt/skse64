#include "skse64_common/skse_version.h"
#include "skse64_common/Utilities.h"
#include "skse64_common/SafeWrite.h"
#include "skse64_loader_common/IdentifyEXE.h"
#include <shlobj.h>
#include <intrin.h>
#include <string>

IDebugLog	gLog;
HANDLE		g_dllHandle;

static void OnAttach(void);
static void HookMain(void * retAddr);
static void HookIAT();

BOOL WINAPI DllMain(HANDLE procHandle, DWORD reason, LPVOID reserved)
{
	if(reason == DLL_PROCESS_ATTACH)
	{
		g_dllHandle = procHandle;
	}

	return TRUE;
}

static void OnAttach(void)
{
	gLog.OpenRelative(CSIDL_MYDOCUMENTS, "\\My Games\\Skyrim Special Edition\\SKSE\\skse64_steam_loader.log");
	gLog.SetPrintLevel(IDebugLog::kLevel_Error);
	gLog.SetLogLevel(IDebugLog::kLevel_DebugMessage);

	FILETIME	now;
	GetSystemTimeAsFileTime(&now);

	_MESSAGE("skse64 loader %08X (steam) %08X%08X %s", PACKED_SKSE_VERSION, now.dwHighDateTime, now.dwLowDateTime, GetOSInfoStr().c_str());
	_MESSAGE("loader base addr = %016I64X", g_dllHandle);
	_MESSAGE("exe base addr = %016I64X", GetModuleHandle(NULL));

	// hook an imported function early so we can inject our code 
	HookIAT();
}

// api-ms-win-crt-runtime-l1-1-0.dll
typedef char * (*__get_narrow_winmain_command_line)();
__get_narrow_winmain_command_line _get_narrow_winmain_command_line_Original = NULL;

static char * _get_narrow_winmain_command_line_Hook()
{
	HookMain(_ReturnAddress());

	return _get_narrow_winmain_command_line_Original();
}

static void HookIAT()
{
	__get_narrow_winmain_command_line * iat = (__get_narrow_winmain_command_line *)GetIATAddr(GetModuleHandle(NULL), "api-ms-win-crt-runtime-l1-1-0.dll", "_get_narrow_winmain_command_line");
	if(iat)
	{
		_MESSAGE("found iat at %016I64X", iat);

		_get_narrow_winmain_command_line_Original = *iat;
		_MESSAGE("original thunk %016I64X", _get_narrow_winmain_command_line_Original);

		SafeWrite64(uintptr_t(iat), (UInt64)_get_narrow_winmain_command_line_Hook);
		_MESSAGE("patched iat");
	}
	else
	{
		_MESSAGE("couldn't find _get_narrow_winmain_command_line");
	}
}

bool hookInstalled = false;
std::string g_dllPath;

static void HookMain(void * retAddr)
{
	if(hookInstalled)
		return;
	else
		hookInstalled = true;

	_MESSAGE("HookMain: thread = %d retaddr = %016I64X", GetCurrentThreadId(), retAddr);

	std::string runtimePath = GetRuntimePath();
	_MESSAGE("runtimePath = %s", runtimePath.c_str());

	bool isEditor = false;

	// check version etc
	std::string		dllSuffix;
	ProcHookInfo	procHookInfo;

	if(!IdentifyEXE(runtimePath.c_str(), isEditor, &dllSuffix, &procHookInfo))
	{
		_ERROR("unknown exe");
		return;
	}

	const char	* dllPrefix = (isEditor == false) ? "\\skse64_" : "\\skse64_editor_";

	g_dllPath = GetRuntimeDirectory() + dllPrefix + dllSuffix + ".dll";
	_MESSAGE("dll = %s", g_dllPath.c_str());

	HMODULE dll = LoadLibrary(g_dllPath.c_str());
	if(dll)
	{
		typedef void (* EntryPoint)(void);
		EntryPoint entryPoint = (EntryPoint)GetProcAddress(dll, "StartSKSE");
		if(entryPoint)
		{
			entryPoint();
		}
		else
		{
			_ERROR("entry point not found");
		}
	}
	else
	{
		_ERROR("couldn't load DLL");
	}
}

extern "C"
{
	void InitSKSESteamLoader()
	{
		OnAttach();
	}
}
