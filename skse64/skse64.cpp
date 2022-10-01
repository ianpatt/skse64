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

IDebugLog gLog;
HINSTANCE g_moduleHandle = nullptr;

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

//	_MESSAGE("pre global init");

	return _initterm_e_Original(a, b);
}

// runs after global initializers
char * __get_narrow_winmain_command_line_Hook()
{
	// the usual load time
	
//	_MESSAGE("post global init");

	SKSE64_Initialize();

	return _get_narrow_winmain_command_line_Original();
}

void SKSE64_PreInit(void)
{
	gLog.OpenRelative(CSIDL_MYDOCUMENTS, "\\My Games\\" SAVE_FOLDER_NAME "\\SKSE\\skse64.log");

	HANDLE exe = GetModuleHandle(nullptr);

	// fetch functions to hook
	auto * initterm = (__initterm_e *)GetIATAddr(exe, "api-ms-win-crt-runtime-l1-1-0.dll", "_initterm_e");
	auto * cmdline = (__get_narrow_winmain_command_line *)GetIATAddr(exe, "api-ms-win-crt-runtime-l1-1-0.dll", "_get_narrow_winmain_command_line");

	// hook them
	if(initterm)
	{
		_initterm_e_Original = *initterm;
		SafeWrite64(uintptr_t(initterm), UInt64(__initterm_e_Hook));
	}
	else
	{
		_ERROR("couldn't find _initterm_e");
	}

	if(cmdline)
	{
		_get_narrow_winmain_command_line_Original = *cmdline;
		SafeWrite64(uintptr_t(cmdline), UInt64(__get_narrow_winmain_command_line_Hook));
	}
	else
	{
		_ERROR("couldn't find _get_narrow_winmain_command_line");
	}
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
	if(isInit) return;
	isInit = true;

#ifndef _DEBUG
	__try {
#endif

		FILETIME	now;
		GetSystemTimeAsFileTime(&now);

		_MESSAGE("SKSE64 runtime: initialize (version = %d.%d.%d %08X %08X%08X, os = %s)",
			SKSE_VERSION_INTEGER, SKSE_VERSION_INTEGER_MINOR, SKSE_VERSION_INTEGER_BETA, RUNTIME_VERSION,
			now.dwHighDateTime, now.dwLowDateTime, GetOSInfoStr().c_str());

		_MESSAGE("imagebase = %016I64X", GetModuleHandle(NULL));
		_MESSAGE("reloc mgr imagebase = %016I64X", RelocationManager::s_baseAddr);

#ifdef _DEBUG
		SetPriorityClass(GetCurrentProcess(), IDLE_PRIORITY_CLASS);

		WaitForDebugger();
#endif

		if(!g_branchTrampoline.Create(1024 * 64))
		{
			_ERROR("couldn't create branch trampoline. this is fatal. skipping remainder of init process.");
			return;
		}

		if(!g_localTrampoline.Create(1024 * 64, g_moduleHandle))
		{
			_ERROR("couldn't create codegen buffer. this is fatal. skipping remainder of init process.");
			return;
		}

		// Add Hooks_XXX_Init calls here
		Hooks_Debug_Init();
		Hooks_ObScript_Init();
		Hooks_Papyrus_Init();
		Hooks_NetImmerse_Init();
		Hooks_Threads_Init();
		Hooks_Handlers_Init();

		g_pluginManager.Init();

		// Add Hooks_XXX_Commit calls here in the same order
		Hooks_Debug_Commit();
		Hooks_ObScript_Commit();
		Hooks_Papyrus_Commit();
		Hooks_UI_Commit();
		Hooks_Camera_Commit();
		Hooks_NetImmerse_Commit();
		Hooks_Threads_Commit();
		Hooks_Handlers_Commit();
		Hooks_Scaleform_Commit();
		Hooks_Gameplay_Commit();
		Hooks_Event_Commit();
		Hooks_SaveLoad_Commit();
		Hooks_Data_Commit();
		Init_CoreSerialization_Callbacks();
		Hooks_DirectInput_Commit();
		
		FlushInstructionCache(GetCurrentProcess(), NULL, 0);

#ifndef _DEBUG
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		_ERROR("exception thrown during startup");
	}
#endif

	_MESSAGE("init complete");
}

extern "C" {

	void StartSKSE(void)
	{
		SKSE64_PreInit();
	}

	BOOL WINAPI DllMain(HANDLE hDllHandle, DWORD dwReason, LPVOID lpreserved)
	{
		switch(dwReason)
		{
		case DLL_PROCESS_ATTACH:
			g_moduleHandle = (HINSTANCE)hDllHandle;
			break;

		case DLL_PROCESS_DETACH:
			break;
		};

		return TRUE;
	}

};
