#include <ShlObj.h>
#include "skse64_common/skse_version.h"
#include "skse64_common/Utilities.h"
#include "skse64_common/CoreInfo.h"
#include "skse64_loader_common/LoaderError.h"
#include "skse64_loader_common/IdentifyEXE.h"
#include "skse64_loader_common/Steam.h"
#include "skse64_loader_common/Inject.h"
#include <string>
#include "common/IFileStream.h"
#include <tlhelp32.h>
#include <Shlwapi.h>
#include "Options.h"
#include "SigCheck.h"

IDebugLog gLog;

class EarlyTerminationHandler
{
public:
	void start(HANDLE proc)
	{
		if(m_proc == INVALID_HANDLE_VALUE)
		{
			m_proc = proc;

			SetConsoleCtrlHandler(HandlerWrapper, true);
		}
	}

	void stop()
	{
		if(m_proc != INVALID_HANDLE_VALUE)
		{
			SetConsoleCtrlHandler(HandlerWrapper, false);

			m_proc = INVALID_HANDLE_VALUE;
		}
	}

private:
	HANDLE	m_proc = INVALID_HANDLE_VALUE;

	static BOOL HandlerWrapper(DWORD type)
	{
		return gEarlyTerminationHandler.Handler(type);
	}

	BOOL Handler(DWORD type)
	{
		_MESSAGE("early termination %d", type);

		TerminateProcess(m_proc, 0);
		
		return true;	// "no other handlers are called and the system terminates the process"
	}
} gEarlyTerminationHandler;

void AugmentEnvironment(const std::string& procPath, const std::string& dllPath)
{
	const auto getFilename = [](const std::string& fullPath) {
		char runtime[MAX_PATH] = { '\0' };
		if (fullPath.length() < std::extent<decltype(runtime)>::value)
		{
			std::copy(fullPath.begin(), fullPath.end(), runtime);
			PathStripPathA(runtime);
		}

		return std::string(runtime);
	};

	// breaks old commonlib
//	SetEnvironmentVariableA("SKSE_DLL", getFilename(dllPath).c_str());
//	SetEnvironmentVariableA("SKSE_RUNTIME", getFilename(procPath).c_str());
	SetEnvironmentVariableA("SKSE_WAITFORDEBUGGER", (g_options.m_waitForDebugger ? "1" : "0"));
}

int main(int argc, char ** argv)
{
	gLog.OpenRelative(CSIDL_MYDOCUMENTS, "\\My Games\\" SAVE_FOLDER_NAME "\\SKSE\\skse64_loader.log");
	gLog.SetPrintLevel(IDebugLog::kLevel_FatalError);
	gLog.SetLogLevel(IDebugLog::kLevel_DebugMessage);

	FILETIME	now;
	GetSystemTimeAsFileTime(&now);

	_MESSAGE("skse64 loader %08X %08X%08X %s",
		PACKED_SKSE_VERSION, now.dwHighDateTime, now.dwLowDateTime, GetOSInfoStr().c_str());

	if(!g_options.Read(argc, argv))
	{
		PrintLoaderError("Couldn't read arguments.");
		g_options.PrintUsage();

		return 1;
	}

	if(g_options.m_optionsOnly)
	{
		g_options.PrintUsage();
		return 0;
	}

	if(g_options.m_verbose)
		gLog.SetPrintLevel(IDebugLog::kLevel_VerboseMessage);

	if(g_options.m_launchCS)
	{
		PrintLoaderError("The editor should be launched directly.");

		return 1;
	}

	// get process/dll names
	bool		dllHasFullPath = false;
	const char	* baseDllName = g_options.m_launchCS ? "skse64_editor" : "skse64";
	bool		usedCustomRuntimeName = false;

	std::string	procName;

	if(g_options.m_launchCS)
	{
		procName = "CreationKit.exe";
	}
	else
	{
		procName = GetConfigOption("Loader", "RuntimeName");
		if(!procName.empty())
		{
			_MESSAGE("using runtime name from config: %s", procName.c_str());
			usedCustomRuntimeName = true;
		}
		else
		{
			procName = "SkyrimSE.exe";

			// simple check to see if someone kludge-patched the EXE
			// don't kludge the EXE, use the .ini file RIGHT ABOVE HERE
			UInt32	procNameCheck =
				(procName[0] <<  8) |
				(procName[1] << 24) |
				(procName[2] << 16) |
				(procName[3] <<  0);

			if(procNameCheck != 'kySr')
			{
				_ERROR("### someone kludged the default process name to (%s), don't ask me for support with your install ###", procName.c_str());
			}

			// check to see if someone screwed up their install
			std::string appName = GetRuntimeName();
			if(!_stricmp(appName.c_str(), procName.c_str()))
			{
				PrintLoaderError("You have renamed skse64_loader and have not specified the name of the runtime.");

				return 1;
			}
		}
	}

	const std::string & runtimeDir = GetRuntimeDirectory();
	std::string procPath = runtimeDir + procName;

	if(g_options.m_altEXE.size())
	{
		procPath = g_options.m_altEXE;
		_MESSAGE("launching alternate exe (%s)", procPath.c_str());
	}

	_MESSAGE("procPath = %s", procPath.c_str());

	// check if the exe exists
	{
		IFileStream	fileCheck;
		if(!fileCheck.Open(procPath.c_str()))
		{
			DWORD err = GetLastError();
			if(err)
				_MESSAGE("exe open check error = %08X", err);

			bool msStore = false;

			if(err == ERROR_ACCESS_DENIED)
			{
				// this might be ms store
				std::string manifestPath = runtimeDir + "appxmanifest.xml";

				if(fileCheck.Open(manifestPath.c_str()))
				{
					msStore = true;
				}
			}

			if(msStore)
			{
				PrintLoaderError("You have the MS Store/Gamepass version of Skyrim, which is not compatible with SKSE.");
			}
			else if(usedCustomRuntimeName)
			{
				PrintLoaderError("Couldn't find %s. You have customized the runtime name via SKSE64's .ini file, and that file does not exist. This can usually be fixed by removing the RuntimeName line from the .ini file.)", procName.c_str());
			}
			else
			{
				PrintLoaderError("Couldn't find %s. You have installed the loader to the wrong folder.", procName.c_str());
			}

			return 1;
		}
	}

	_MESSAGE("launching: %s (%s)", procName.c_str(), procPath.c_str());

	if(g_options.m_altDLL.size())
	{
		baseDllName = g_options.m_altDLL.c_str();
		_MESSAGE("launching alternate dll (%s)", baseDllName);

		dllHasFullPath = true;
	}

	std::string		dllSuffix;
	ProcHookInfo	procHookInfo;

	// check exe version
	if(!IdentifyEXE(procPath.c_str(), g_options.m_launchCS, &dllSuffix, &procHookInfo))
	{
		_ERROR("unknown exe");

		if(usedCustomRuntimeName)
		{
			// hurr durr
			PrintLoaderError("You have customized the runtime name via SKSE64's .ini file. Version errors can usually be fixed by removing the RuntimeName line from the .ini file.");
		}

		return 1;
	}

	// build dll path
	std::string	dllPath;
	if(dllHasFullPath)
	{
		dllPath = baseDllName;
	}
	else
	{
		dllPath = runtimeDir + baseDllName + "_" + dllSuffix + ".dll";
	}

	_MESSAGE("dll = %s", dllPath.c_str());

	// check to make sure the dll exists
	{
		IFileStream	tempFile;

		if(!tempFile.Open(dllPath.c_str()))
		{
			PrintLoaderError("Couldn't find SKSE64 DLL (%s). Please make sure you have installed SKSE64 correctly and are running it from your Skyrim SE folder.", dllPath.c_str());
			return 1;
		}
	}

	// check to make sure the dll makes sense
	{
		bool dllOK = false;
		UInt32 dllVersion = 0;

		HMODULE resourceHandle = (HMODULE)LoadLibraryEx(dllPath.c_str(), nullptr, LOAD_LIBRARY_AS_IMAGE_RESOURCE);
		if(resourceHandle)
		{
			if(Is64BitDLL(resourceHandle))
			{
				auto * version = (const SKSECoreVersionData *)GetResourceLibraryProcAddress(resourceHandle, "SKSECore_Version");
				if(version)
				{
					dllVersion = version->runtimeVersion;

					if(	(version->dataVersion == SKSECoreVersionData::kVersion) &&
						(version->runtimeVersion == procHookInfo.packedVersion))
					{
						dllOK = true;
					}
				}
			}

			FreeLibrary(resourceHandle);
		}

		if(dllOK)
		{
			if(!CheckDLLSignature(dllPath))
				dllOK = false;
		}

		if(!dllOK)
		{
			bool preSigning = false;

			VS_FIXEDFILEINFO info;
			std::string productName;
			std::string productVersion;

			if(GetFileVersion(dllPath.c_str(), &info, &productName, &productVersion))
			{
				_MESSAGE("DLL version");
				DumpVersionInfo(info);
				_MESSAGE("productName = %s", productName.c_str());

				UInt64 fullVersion = (UInt64(info.dwFileVersionMS) << 32) | info.dwFileVersionLS;
				UInt64 kFirstSignedVersion = 0x0000000200020007;

				if(fullVersion < kFirstSignedVersion)
					preSigning = true;
			}
			else
			{
				_MESSAGE("couldn't get file version info");
			}

			if(preSigning)
			{
				PrintLoaderError(
					"Old SKSE DLL (%s).\n"
					"Please make sure that you have replaced all files with their new versions.\n"
					"DLL version (%d.%d.%d) EXE version (%d.%d.%d)",
					dllPath.c_str(),
					(info.dwFileVersionMS >> 16) & 0xFFFF,
					(info.dwFileVersionLS >> 16) & 0xFFFF,
					info.dwFileVersionLS & 0xFFFF,
					SKSE_VERSION_INTEGER, SKSE_VERSION_INTEGER_MINOR, SKSE_VERSION_INTEGER_BETA);
			}
			else
			{
				PrintLoaderError(
					"Bad SKSE DLL (%s).\n"
					"Do not rename files; it will not magically make anything work.\n"
					"%08X %08X", dllPath.c_str(), procHookInfo.packedVersion, dllVersion);
			}

			return 1;
		}
	}

	if(g_options.m_crcOnly)
		return 0;

	// steam setup
	if(procHookInfo.procType == kProcType_Steam)
	{
		if(g_options.m_launchSteam)
		{
			// if steam isn't running, launch it
			if(!SteamCheckPassive())
			{
				_MESSAGE("steam not running, launching it");

				if(!SteamLaunch())
				{
					_WARNING("failed to launch steam");
				}
			}
		}

		// same for standard and nogore
		const char * kAppID = (g_options.m_launchCS == false ? "489830" : "???");

		// set this no matter what to work around launch issues
		SetEnvironmentVariable("SteamGameId", kAppID);

		if(g_options.m_skipLauncher)
		{
			SetEnvironmentVariable("SteamAppID", kAppID);
		}
	}

	// launch the app (suspended)
	STARTUPINFO			startupInfo = { 0 };
	PROCESS_INFORMATION	procInfo = { 0 };

	startupInfo.cb = sizeof(startupInfo);

	AugmentEnvironment(procPath, dllPath);
	
	DWORD createFlags = CREATE_SUSPENDED;
	if(g_options.m_setPriority)
		createFlags |= g_options.m_priority;

	if(!CreateProcess(
		procPath.c_str(),
		NULL,	// no args
		NULL,	// default process security
		NULL,	// default thread security
		FALSE,	// don't inherit handles
		createFlags,
		NULL,	// no new environment
		NULL,	// no new cwd
		&startupInfo, &procInfo))
	{
		if(GetLastError() == 740)
		{
			PrintLoaderError("Launching %s failed (%d). Please try running skse64_loader as an administrator.", procPath.c_str(), GetLastError());
		}
		else
		{
			PrintLoaderError("Launching %s failed (%d).", procPath.c_str(), GetLastError());
		}

		return 1;
	}

	gEarlyTerminationHandler.start(procInfo.hProcess);

	_MESSAGE("main thread id = %d", procInfo.dwThreadId);

	// set affinity if requested
	if(g_options.m_affinity)
	{
		_MESSAGE("setting affinity mask to %016I64X", g_options.m_affinity);

		if(!SetProcessAffinityMask(procInfo.hProcess, g_options.m_affinity))
		{
			_WARNING("couldn't set affinity mask (%08X)", GetLastError());
		}
	}

	bool	injectionSucceeded = false;
	UInt32	procType = procHookInfo.procType;

	// inject the dll
	switch(procType)
	{
	case kProcType_Steam:
	case kProcType_Normal:
	case kProcType_GOG:
		injectionSucceeded = InjectDLLThread(&procInfo, dllPath.c_str(), true, g_options.m_noTimeout);
		break;

	default:
		HALT("impossible");
	}

	// start the process if successful
	if(!injectionSucceeded)
	{
		PrintLoaderError("Couldn't inject DLL.");

		_ERROR("terminating process");

		TerminateProcess(procInfo.hProcess, 0);
	}
	else
	{
		_MESSAGE("launching");

		if(!ResumeThread(procInfo.hThread))
		{
			_WARNING("WARNING: something has started the runtime outside of skse64_loader's control.");
			_WARNING("SKSE64 will probably not function correctly.");
			_WARNING("Try running skse64_loader as an administrator, or check for conflicts with a virus scanner.");
		}

		if(g_options.m_waitForClose)
			WaitForSingleObject(procInfo.hProcess, INFINITE);
	}

	gEarlyTerminationHandler.stop();

	// clean up
	CloseHandle(procInfo.hProcess);
	CloseHandle(procInfo.hThread);

	return 0;
}
