#include "Steam.h"
#include "skse64_loader_common/LoaderError.h"
#include <tlhelp32.h>
#include <string>

std::string GetRegKey(HKEY root, const char * path, char * value)
{
	std::string	result;
	HKEY		key;

	UInt32	err = RegOpenKeyEx(root, path, 0, KEY_READ, &key);
	if(!err)
	{
		UInt32	dataLen = 0;

		// lol race condition
		err = RegQueryValueEx(key, value, NULL, NULL, NULL, &dataLen);
		if(!err)
		{
			result.resize(dataLen);

			err = RegQueryValueEx(key, value, NULL, NULL, (BYTE *)&result[0], &dataLen);
			if(!err)
			{
				// trim null terminator
				if(result.length() > 0)
					result.resize(result.length() - 1);
			}
			else
			{
				_ERROR("GetRegKey: error getting value (%08X)", err);
			}
		}
		else
		{
			_ERROR("GetRegKey: error querying value length (%08X)", err);
		}

		RegCloseKey(key);
	}
	else
	{
		_ERROR("GetRegKey: registry key not found (%08X)", err);
	}

	return result;
}

std::string GetSteamRoot(void)
{
	return GetRegKey(HKEY_LOCAL_MACHINE, "SOFTWARE\\Valve\\Steam", "InstallPath");
}

std::string GetSteamClientDLLPath(void)
{
	return GetRegKey(HKEY_CURRENT_USER, "SOFTWARE\\Valve\\Steam\\ActiveProcess", "SteamClientDll");
}

bool SteamCheckPassive(void)
{
	bool	result = false;
	HANDLE	snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(snap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32	proc;

		proc.dwSize = sizeof(proc);

		if(Process32First(snap, &proc))
		{
			do 
			{
				if(!_stricmp(proc.szExeFile, "steam.exe"))
				{
					_MESSAGE("found steam");
					result = true;
					break;
				}
			}
			while(Process32Next(snap, &proc));
		}
		else
		{
			_ERROR("SteamCheck: Module32First failed (%d)", GetLastError());
			result = true;
		}

		CloseHandle(snap);
	}
	else
	{
		_ERROR("SteamCheck: CreateToolhelp32Snapshot failed (%d)", GetLastError());
		result = true;	// if for whatever reason we can't scan, assume steam is running
	}

	return result;
}

static bool SteamCheckActive(void)
{
	bool	result = false;

	HMODULE	steamAPI = LoadLibrary("steam_api.dll");
	if(steamAPI)
	{
		typedef bool (* _SteamAPI_IsSteamRunning)(void);

		_SteamAPI_IsSteamRunning	SteamAPI_IsSteamRunning = NULL;

		// this just checks HKCU\Software\Valve\Steam\ActiveProcess\pid
		// to see if it's running, however process IDs can be reused and it doesn't clean the registry key
		// on exit
		SteamAPI_IsSteamRunning = (_SteamAPI_IsSteamRunning)GetProcAddress(steamAPI, "SteamAPI_IsSteamRunning");

		if(SteamAPI_IsSteamRunning)
		{
			UInt32	startTime = GetTickCount();
			const UInt32	kSteamTimeout = 10 * 1000;	// 10 seconds

			while((GetTickCount() - startTime) >= kSteamTimeout)
			{
				if(SteamAPI_IsSteamRunning())
				{
					result = true;
					break;
				}

				Sleep(100);
			}

			if(!result)
			{
				_ERROR("timed out waiting for steam boot");
			}
		}
		else
		{
			_ERROR("couldn't get steam API ptr");
		}

		FreeLibrary(steamAPI);
	}
	else
	{
		_ERROR("couldn't load steam API DLL (%08X)", GetLastError());
	}

	return result;
}

bool SteamLaunch(void)
{
	std::string	steamRoot = GetSteamRoot();
	_MESSAGE("steam root = %s", steamRoot.c_str());

	if(steamRoot.empty())
		return false;

	std::string	steamEXEPath = steamRoot + "\\Steam.exe";

	STARTUPINFO			startupInfo = { 0 };
	PROCESS_INFORMATION	procInfo = { 0 };

	startupInfo.cb = sizeof(startupInfo);

	if(!CreateProcess(
		steamEXEPath.c_str(),
		NULL,	// no args
		NULL,	// default process security
		NULL,	// default thread security
		FALSE,	// don't inherit handles
		0,		// no options
		NULL,	// no new environment block
		steamRoot.c_str(),	// new cwd
		&startupInfo, &procInfo))
	{
		PrintLoaderError("Launching Steam failed (%08X).", GetLastError());
		return false;
	}

	// this doesn't do anything useful
	//	bool result = SteamCheckActive();

	// this is an ugly hack. wait for steam to start pumping messages
	WaitForInputIdle(procInfo.hProcess, INFINITE);

	// and then you know some more just because even then it isn't ready
	Sleep(1000 * 5);

	// clean up
	CloseHandle(procInfo.hProcess);
	CloseHandle(procInfo.hThread);

	return true;
}
