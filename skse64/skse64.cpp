#include "skse64_common/skse_version.h"
#include "skse64_common/Utilities.h"
#include "skse64_common/Relocation.h"
#include "skse64_common/BranchTrampoline.h"
#include "skse64_common/SafeWrite.h"
#include "skse64/PluginManager.h"
#include <shlobj.h>
#include "common/IFileStream.h"
#include "Hooks_ObScript.h"
#include "Hooks_Papyrus.h"
#include "Hooks_SaveLoad.h"
#include "Hooks_Gameplay.h"
#include "Hooks_Data.h"
#include "Hooks_Debug.h"
#include "Hooks_Event.h"
#include "Hooks_Camera.h"
#include "Hooks_NetImmerse.h"
#include "Hooks_Handlers.h"
#include "Hooks_Threads.h"
#include "Hooks_UI.h"
#include "Hooks_Scaleform.h"
#include "Hooks_DirectInput8Create.h"
#include "Hooks_Diagnostics.h"
#include "InternalSerialization.h"
#include <cstdio>
#include <Windows.h>

IDebugLog gLog;
HINSTANCE g_moduleHandle = nullptr;

// Console logging for debugging startup crashes
static FILE* g_consoleOut = nullptr;
static bool g_consoleAllocated = false;

void AllocateConsole()
{
	if (!g_consoleAllocated)
	{
		AllocConsole();
		freopen_s(&g_consoleOut, "CONOUT$", "w", stdout);
		freopen_s(&g_consoleOut, "CONOUT$", "w", stderr);
		g_consoleAllocated = true;
		printf("=== SKSE64 Console Debug Log ===\n");
		printf("Console allocated successfully\n");
	}
}

#define CONSOLE_LOG(fmt, ...) do { if (g_consoleAllocated) { printf("[SKSE64] " fmt "\n", ##__VA_ARGS__); fflush(stdout); } } while(0)

void SKSE64_Initialize(void);

// api-ms-win-crt-runtime-l1-1-0.dll
typedef int (*__initterm_e)(_PIFV *, _PIFV *);
__initterm_e _initterm_e_Original = nullptr;

typedef char * (*__get_narrow_winmain_command_line)();
__get_narrow_winmain_command_line _get_narrow_winmain_command_line_Original = NULL;

// runs before global initializers
int __initterm_e_Hook(_PIFV * a, _PIFV * b)
{
	// could be used for plugin optional preload
	CONSOLE_LOG("__initterm_e_Hook: Entry (pre global init)");

	int result = _initterm_e_Original(a, b);

	CONSOLE_LOG("__initterm_e_Hook: Exit (result = %d)", result);
	return result;
}

// runs after global initializers
char * __get_narrow_winmain_command_line_Hook()
{
	// the usual load time
	CONSOLE_LOG("__get_narrow_winmain_command_line_Hook: Entry (post global init)");

	SKSE64_Initialize();

	CONSOLE_LOG("__get_narrow_winmain_command_line_Hook: Exit");
	return _get_narrow_winmain_command_line_Original();
}

void SKSE64_PreInit(void)
{
	CONSOLE_LOG("SKSE64_PreInit: Entry");

	CONSOLE_LOG("SKSE64_PreInit: Opening log file");
	gLog.OpenRelative(CSIDL_MYDOCUMENTS, "\\My Games\\" SAVE_FOLDER_NAME "\\SKSE\\skse64.log");

	CONSOLE_LOG("SKSE64_PreInit: Getting module handle");
	HANDLE exe = GetModuleHandle(nullptr);

	// fetch functions to hook
	CONSOLE_LOG("SKSE64_PreInit: Fetching IAT addresses");
	auto * initterm = (__initterm_e *)GetIATAddr(exe, "api-ms-win-crt-runtime-l1-1-0.dll", "_initterm_e");
	auto * cmdline = (__get_narrow_winmain_command_line *)GetIATAddr(exe, "api-ms-win-crt-runtime-l1-1-0.dll", "_get_narrow_winmain_command_line");

	// hook them
	if(initterm)
	{
		CONSOLE_LOG("SKSE64_PreInit: Hooking _initterm_e");
		_initterm_e_Original = *initterm;
		SafeWrite64(uintptr_t(initterm), UInt64(__initterm_e_Hook));
	}
	else
	{
		CONSOLE_LOG("SKSE64_PreInit: ERROR - couldn't find _initterm_e");
		_ERROR("couldn't find _initterm_e");
	}

	if(cmdline)
	{
		CONSOLE_LOG("SKSE64_PreInit: Hooking _get_narrow_winmain_command_line");
		_get_narrow_winmain_command_line_Original = *cmdline;
		SafeWrite64(uintptr_t(cmdline), UInt64(__get_narrow_winmain_command_line_Hook));
	}
	else
	{
		CONSOLE_LOG("SKSE64_PreInit: ERROR - couldn't find _get_narrow_winmain_command_line");
		_ERROR("couldn't find _get_narrow_winmain_command_line");
	}

	CONSOLE_LOG("SKSE64_PreInit: Exit");
}

void WaitForDebugger(void)
{
	while(!IsDebuggerPresent())
	{
		Sleep(10);
	}

	Sleep(1000 * 2);
}

static bool isInit = false;

void SKSE64_Initialize(void)
{
	CONSOLE_LOG("SKSE64_Initialize: Entry");

	if(isInit)
	{
		CONSOLE_LOG("SKSE64_Initialize: Already initialized, returning");
		return;
	}
	isInit = true;

	CONSOLE_LOG("SKSE64_Initialize: Starting initialization");

#ifndef _DEBUG
	__try {
#endif

		CONSOLE_LOG("SKSE64_Initialize: Getting system time");
		FILETIME	now;
		GetSystemTimeAsFileTime(&now);

		CONSOLE_LOG("SKSE64_Initialize: Logging version info");
		_MESSAGE("SKSE64 runtime: initialize (version = %d.%d.%d %08X %08X%08X, os = %s)",
			SKSE_VERSION_INTEGER, SKSE_VERSION_INTEGER_MINOR, SKSE_VERSION_INTEGER_BETA, RUNTIME_VERSION,
			now.dwHighDateTime, now.dwLowDateTime, GetOSInfoStr().c_str());

		CONSOLE_LOG("SKSE64_Initialize: Version = %d.%d.%d", SKSE_VERSION_INTEGER, SKSE_VERSION_INTEGER_MINOR, SKSE_VERSION_INTEGER_BETA);

		_MESSAGE("imagebase = %016I64X", GetModuleHandle(NULL));
		_MESSAGE("reloc mgr imagebase = %016I64X", RelocationManager::s_baseAddr);

#ifdef _DEBUG
		SetPriorityClass(GetCurrentProcess(), IDLE_PRIORITY_CLASS);

		WaitForDebugger();
#endif

		CONSOLE_LOG("SKSE64_Initialize: Creating branch trampoline");
		if(!g_branchTrampoline.Create(1024 * 64))
		{
			CONSOLE_LOG("SKSE64_Initialize: ERROR - couldn't create branch trampoline");
			_ERROR("couldn't create branch trampoline. this is fatal. skipping remainder of init process.");
			return;
		}

		CONSOLE_LOG("SKSE64_Initialize: Creating local trampoline");
		if(!g_localTrampoline.Create(1024 * 64, g_moduleHandle))
		{
			CONSOLE_LOG("SKSE64_Initialize: ERROR - couldn't create codegen buffer");
			_ERROR("couldn't create codegen buffer. this is fatal. skipping remainder of init process.");
			return;
		}

		// Add Hooks_XXX_Init calls here
		CONSOLE_LOG("SKSE64_Initialize: Hooks_Debug_Init");
		Hooks_Debug_Init();
		CONSOLE_LOG("SKSE64_Initialize: Hooks_ObScript_Init");
		Hooks_ObScript_Init();
		CONSOLE_LOG("SKSE64_Initialize: Hooks_Papyrus_Init");
		Hooks_Papyrus_Init();
		CONSOLE_LOG("SKSE64_Initialize: Hooks_NetImmerse_Init");
		Hooks_NetImmerse_Init();
		CONSOLE_LOG("SKSE64_Initialize: Hooks_Threads_Init");
		Hooks_Threads_Init();
		CONSOLE_LOG("SKSE64_Initialize: Hooks_Handlers_Init");
		Hooks_Handlers_Init();

		CONSOLE_LOG("SKSE64_Initialize: g_pluginManager.Init()");
		g_pluginManager.Init();

		// Add Hooks_XXX_Commit calls here in the same order
		CONSOLE_LOG("SKSE64_Initialize: Hooks_Debug_Commit");
		Hooks_Debug_Commit();
		CONSOLE_LOG("SKSE64_Initialize: Hooks_ObScript_Commit");
		Hooks_ObScript_Commit();
		CONSOLE_LOG("SKSE64_Initialize: Hooks_Papyrus_Commit");
		Hooks_Papyrus_Commit();
		CONSOLE_LOG("SKSE64_Initialize: Hooks_UI_Commit");
		Hooks_UI_Commit();
		CONSOLE_LOG("SKSE64_Initialize: Hooks_Camera_Commit");
		Hooks_Camera_Commit();
		CONSOLE_LOG("SKSE64_Initialize: Hooks_NetImmerse_Commit");
		Hooks_NetImmerse_Commit();
		CONSOLE_LOG("SKSE64_Initialize: Hooks_Threads_Commit");
		Hooks_Threads_Commit();
		CONSOLE_LOG("SKSE64_Initialize: Hooks_Handlers_Commit");
		Hooks_Handlers_Commit();
		CONSOLE_LOG("SKSE64_Initialize: Hooks_Scaleform_Commit");
		Hooks_Scaleform_Commit();
		CONSOLE_LOG("SKSE64_Initialize: Hooks_Gameplay_Commit");
		Hooks_Gameplay_Commit();
		CONSOLE_LOG("SKSE64_Initialize: Hooks_Event_Commit");
		Hooks_Event_Commit();
		CONSOLE_LOG("SKSE64_Initialize: Hooks_SaveLoad_Commit");
		Hooks_SaveLoad_Commit();
		CONSOLE_LOG("SKSE64_Initialize: Hooks_Data_Commit");
		Hooks_Data_Commit();
		CONSOLE_LOG("SKSE64_Initialize: Init_CoreSerialization_Callbacks");
		Init_CoreSerialization_Callbacks();
		CONSOLE_LOG("SKSE64_Initialize: Hooks_DirectInput_Commit");
		Hooks_DirectInput_Commit();

		CONSOLE_LOG("SKSE64_Initialize: Flushing instruction cache");
		FlushInstructionCache(GetCurrentProcess(), NULL, 0);

#ifndef _DEBUG
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		CONSOLE_LOG("SKSE64_Initialize: *** EXCEPTION CAUGHT ***");
		CONSOLE_LOG("SKSE64_Initialize: Exception code: %08X", GetExceptionCode());
		_ERROR("exception thrown during startup");

		// Keep console open so user can see the error
		printf("\n\n=== EXCEPTION OCCURRED ===\n");
		printf("Press any key to exit...\n");
		getchar();
	}
#endif

	CONSOLE_LOG("SKSE64_Initialize: Init complete");
	_MESSAGE("init complete");
}

extern "C" {

	void StartSKSE(void)
	{
		AllocateConsole();
		CONSOLE_LOG("StartSKSE: Entry");
		SKSE64_PreInit();
		CONSOLE_LOG("StartSKSE: Exit");
	}

	BOOL WINAPI DllMain(HANDLE hDllHandle, DWORD dwReason, LPVOID lpreserved)
	{
		switch(dwReason)
		{
		case DLL_PROCESS_ATTACH:
			// Allocate console as early as possible to catch all logging
			AllocateConsole();
			CONSOLE_LOG("DllMain: DLL_PROCESS_ATTACH");
			g_moduleHandle = (HINSTANCE)hDllHandle;
			CONSOLE_LOG("DllMain: Module handle set to %p", g_moduleHandle);
			break;

		case DLL_PROCESS_DETACH:
			CONSOLE_LOG("DllMain: DLL_PROCESS_DETACH");
			break;
		};

		return TRUE;
	}

};
