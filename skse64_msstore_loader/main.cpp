#include <ShObjIdl_core.h>
#include <ShlObj_core.h>
#include <ObjIdl.h>
#include "skse64_loader_common/Inject.h"
#include "skse64_loader_common/LoaderError.h"

IDebugLog gLog;

int main(int argc, char ** argv)
{
	gLog.OpenRelative(CSIDL_MYDOCUMENTS, "\\My Games\\Skyrim Special Edition MS\\SKSE\\skse64_msstore_loader.log");
	gLog.SetPrintLevel(IDebugLog::kLevel_FatalError);
	gLog.SetLogLevel(IDebugLog::kLevel_DebugMessage);
	
	int err;
	
	// appears to be constant and independent of version
	std::wstring userModelID = L"BethesdaSoftworks.SkyrimSE-PC_3275kfvn8vcwc!Game";
	
	HANDLE exclusiveLaunchMutex = CreateMutex(nullptr, FALSE, "__SCRIPT_EXTENDER_MSSTORE_LOADER_MUTEX__");
	err = GetLastError();
	if(err == ERROR_ALREADY_EXISTS)
	{
		PrintLoaderError("The loader is already running.");
		return 1;
	}

	err = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
	if(err)
	{
		PrintLoaderError("Unable to initialize COM (%d).", GetLastError());
		return 1;
	}
	
	IApplicationActivationManager * activationManager = nullptr;
	err = CoCreateInstance(CLSID_ApplicationActivationManager, nullptr, CLSCTX_ALL, __uuidof(IApplicationActivationManager), (void **)&activationManager);
	if(err)
	{
		PrintLoaderError("Unable to create ApplicationActivationManager (%d).", GetLastError());
		return 1;
	}
	
	DWORD procID;
	err = activationManager->ActivateApplication(userModelID.c_str(), nullptr, AO_NONE, &procID);
	if(err)
	{
		PrintLoaderError("Couldn't launch app (%d).", GetLastError());
	}
	else
	{
		_MESSAGE("procID %d", procID);
		
		PROCESS_INFORMATION procInfo = { 0 };
		procInfo.dwProcessId = procID;	// InjectDLLThread only cares about this
		
		if(InjectDLLThread(&procInfo, "skse64_msstore_stub.dll", true, false))
		{
			_MESSAGE("injection succeeded");
		}
		else
		{
			PrintLoaderError("Injection failed.");
		}
	}
	
	activationManager->Release();
	CoUninitialize();
}
