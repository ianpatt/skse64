#include "skse64/PluginManager.h"
#include "common/IDirectoryIterator.h"
#include "common/IFileStream.h"
#include "skse64/GameAPI.h"
#include "skse64_common/Utilities.h"
#include "skse64/Serialization.h"
#include "skse64_common/skse_version.h"
#include "skse64/PapyrusEvents.h"
#include "skse64_common/BranchTrampoline.h"
#include "resource.h"

PluginManager	g_pluginManager;

PluginManager::LoadedPlugin *	PluginManager::s_currentLoadingPlugin = NULL;
PluginHandle					PluginManager::s_currentPluginHandle = 0;
UInt32							s_trampolineLog = 1;

extern EventDispatcher<SKSEModCallbackEvent>	g_modCallbackEventDispatcher;
extern EventDispatcher<SKSECameraEvent>			g_cameraEventDispatcher;
extern EventDispatcher<SKSECrosshairRefEvent>	g_crosshairRefEventDispatcher;
extern EventDispatcher<SKSEActionEvent>			g_actionEventDispatcher;

BranchTrampolineManager g_branchTrampolineManager(g_branchTrampoline);
BranchTrampolineManager g_localTrampolineManager(g_localTrampoline);

static const SKSEInterface g_SKSEInterface =
{
	PACKED_SKSE_VERSION,

#ifdef RUNTIME
	RUNTIME_VERSION,
	0,
	0,
#else
	0,
	EDITOR_VERSION,
	1,
#endif

	PluginManager::QueryInterface,
	PluginManager::GetPluginHandle,
	PluginManager::GetReleaseIndex,
	PluginManager::GetPluginInfo
};

#ifdef RUNTIME

#include "Hooks_Scaleform.h"

static const SKSEScaleformInterface g_SKSEScaleformInterface =
{
	SKSEScaleformInterface::kInterfaceVersion,

	RegisterScaleformPlugin,
	RegisterScaleformInventory
};

#include "Hooks_Threads.h"

static const SKSETaskInterface g_SKSETaskInterface =
{
	SKSETaskInterface::kInterfaceVersion,

	TaskInterface::AddTask,
	TaskInterface::AddUITask
};

#include "skse64/Hooks_Papyrus.h"
#include "skse64/PapyrusVM.h"

static const SKSEPapyrusInterface g_SKSEPapyrusInterface =
{
	SKSEPapyrusInterface::kInterfaceVersion,
	RegisterPapyrusPlugin
};

static SKSEMessagingInterface g_SKSEMessagingInterface =
{
	SKSEMessagingInterface::kInterfaceVersion,
	PluginManager::RegisterListener,
	PluginManager::Dispatch_Message,
	PluginManager::GetEventDispatcher,
};

#include "skse64/PapyrusDelayFunctors.h"
#include "skse64/PapyrusObjects.h"

static const SKSEObjectInterface g_SKSEObjectInterface =
{
	SKSEObjectInterface::kInterfaceVersion,
	SKSEDelayFunctorManagerInstance,
	SKSEObjectRegistryInstance,
	SKSEObjectStorageInstance
};

static const SKSETrampolineInterface g_SKSETrampolineInterface =
{
	SKSETrampolineInterface::kInterfaceVersion,
	AllocateFromSKSEBranchPool,
	AllocateFromSKSELocalPool
};
#endif

PluginManager::PluginManager()
{
	//
}

PluginManager::~PluginManager()
{
	DeInit();
}

PluginManager::LoadedPlugin::LoadedPlugin()
	:handle(0)
	,load(nullptr)
{
	memset(&info, 0, sizeof(info));
	memset(&version, 0, sizeof(version));
}

bool PluginManager::Init(void)
{
	bool	result = false;

	if(FindPluginDirectory())
	{
		_MESSAGE("plugin directory = %s", m_pluginDirectory.c_str());

		// avoid realloc
		m_plugins.reserve(5);

		__try
		{
			ScanPlugins();
			InstallPlugins();

			result = true;
		}
		__except(EXCEPTION_EXECUTE_HANDLER)
		{
			// something very bad happened
			_ERROR("exception occurred while loading plugins");
		}
	}

	ReportPluginErrors();

	return result;
}

void PluginManager::DeInit(void)
{
	for(LoadedPluginList::iterator iter = m_plugins.begin(); iter != m_plugins.end(); ++iter)
	{
		LoadedPlugin	* plugin = &(*iter);

		if(plugin->handle)
		{
			FreeLibrary(plugin->handle);
		}
	}

	m_plugins.clear();
}

UInt32 PluginManager::GetNumPlugins(void)
{
	UInt32	numPlugins = m_plugins.size();

	// is one currently loading?
	if(s_currentLoadingPlugin) numPlugins++;

	return numPlugins;
}

PluginInfo * PluginManager::GetInfoByName(const char * name)
{
	for(LoadedPluginList::iterator iter = m_plugins.begin(); iter != m_plugins.end(); ++iter)
	{
		LoadedPlugin	* plugin = &(*iter);

		if(plugin->info.name && !_stricmp(name, plugin->info.name))
			return &plugin->info;
	}

	return NULL;
}

void * PluginManager::QueryInterface(UInt32 id)
{
	void	* result = NULL;

#ifdef RUNTIME
	switch(id)
	{
	case kInterface_Papyrus:
		result = (void *)&g_SKSEPapyrusInterface;
		break;
	case kInterface_Serialization:
		result = (void *)&g_SKSESerializationInterface;
		break;
	case kInterface_Messaging:
		result = (void *)&g_SKSEMessagingInterface;
		break;
	case kInterface_Scaleform:
		result = (void *)&g_SKSEScaleformInterface;
		break;
	case kInterface_Task:
		result = (void *)&g_SKSETaskInterface;
		break;
	case kInterface_Object:
		result = (void *)&g_SKSEObjectInterface;
		break;
	case kInterface_Trampoline:
		result = (void *)&g_SKSETrampolineInterface;
		break;

	default:
		_WARNING("unknown QueryInterface %08X", id);
		break;
	}
#else
	_WARNING("unknown QueryInterface %08X", id);
#endif

	return result;
}

PluginHandle PluginManager::GetPluginHandle(void)
{
	ASSERT_STR(s_currentPluginHandle, "A plugin has called SKSEInterface::GetPluginHandle outside of its Query/Load handlers");

	return s_currentPluginHandle;
}

UInt32 PluginManager::GetReleaseIndex( void )
{
	return SKSE_VERSION_RELEASEIDX;
}

const PluginInfo*	PluginManager::GetPluginInfo(const char* name)
{
	return g_pluginManager.GetInfoByName(name);
}

bool PluginManager::FindPluginDirectory(void)
{
	bool	result = false;

	// find the path <runtime directory>/data/skse/
	std::string	runtimeDirectory = GetRuntimeDirectory();

	if(!runtimeDirectory.empty())
	{
		m_pluginDirectory = runtimeDirectory + "Data\\SKSE\\Plugins\\";
		result = true;
	}

	return result;
}

void PluginManager::ScanPlugins(void)
{
	_MESSAGE("scanning plugin directory %s", m_pluginDirectory.c_str());

	UInt32 handleIdx = 1;	// start at 1, 0 is reserved for internal use

	for(IDirectoryIterator iter(m_pluginDirectory.c_str(), "*.dll"); !iter.Done(); iter.Next())
	{
		std::string	pluginPath = iter.GetFullPath();

		LoadedPlugin	plugin;
		plugin.dllName = iter.Get()->cFileName;

		_MESSAGE("checking plugin %s", plugin.dllName.c_str());

		HMODULE resourceHandle = (HMODULE)LoadLibraryEx(pluginPath.c_str(), nullptr, LOAD_LIBRARY_AS_IMAGE_RESOURCE);
		if(resourceHandle)
		{
			if(Is64BitDLL(resourceHandle))
			{
				auto * version = (const SKSEPluginVersionData *)GetResourceLibraryProcAddress(resourceHandle, "SKSEPlugin_Version");
				if(version)
				{
					plugin.version = *version;
					Sanitize(&plugin.version);

					auto * loadStatus = CheckPluginCompatibility(plugin.version);
					if(!loadStatus)
					{
						// compatible, add to list

						plugin.internalHandle = handleIdx;
						handleIdx++;

						m_plugins.push_back(plugin);
					}
					else
					{
						LogPluginLoadError(plugin, loadStatus);
					}
				}
				else
				{
					LogPluginLoadError(plugin, "no version data", 0, false);
				}
			}
			else
			{
				LogPluginLoadError(plugin, "LE plugin cannot be used with SE");
			}

			FreeLibrary(resourceHandle);
		}
		else
		{
			LogPluginLoadError(plugin, "couldn't load plugin", GetLastError());
		}
	}
}

const char * PluginManager::CheckAddressLibrary(void)
{
	static bool s_checked = false;
	static const char * s_status = nullptr;

	if(s_checked)
	{
		return s_status;
	}

	char fileName[256];
	_snprintf_s(fileName, 256, "Data\\SKSE\\Plugins\\versionlib-%d-%d-%d-%d.bin",
		GET_EXE_VERSION_MAJOR(RUNTIME_VERSION),
		GET_EXE_VERSION_MINOR(RUNTIME_VERSION),
		GET_EXE_VERSION_BUILD(RUNTIME_VERSION),
		0);

	IFileStream versionLib;
	if(!versionLib.Open(fileName))
	{
		m_oldAddressLibrary = true;
		s_status = "disabled, address library needs to be updated";
	}

	s_checked = true;

	return s_status;
}

void PluginManager::InstallPlugins(void)
{
	for(size_t i = 0; i < m_plugins.size(); i++)
	{
		auto & plugin = m_plugins[i];

		_MESSAGE("loading plugin \"%s\"", plugin.version.name);

		s_currentLoadingPlugin = &plugin;
		s_currentPluginHandle = plugin.internalHandle;

		std::string pluginPath = m_pluginDirectory + plugin.dllName;

		plugin.handle = (HMODULE)LoadLibrary(pluginPath.c_str());
		if(plugin.handle)
		{
			bool		success = false;

			plugin.load = (_SKSEPlugin_Load)GetProcAddress(plugin.handle, "SKSEPlugin_Load");
			if(plugin.load)
			{
				const char * loadStatus = NULL;

				loadStatus = SafeCallLoadPlugin(&plugin, &g_SKSEInterface);

				if(!loadStatus)
				{
					success = true;
					loadStatus = "loaded correctly";
				}

				ASSERT(loadStatus);

				if(success)
				{
					_MESSAGE("plugin %s (%08X %s %08X) %s (handle %d)",
						plugin.dllName.c_str(),
						plugin.version.dataVersion,
						plugin.version.name,
						plugin.version.pluginVersion,
						loadStatus,
						s_currentPluginHandle);
				}
				else
				{
					LogPluginLoadError(plugin, loadStatus);
				}
			}
			else
			{
				LogPluginLoadError(plugin, "does not appear to be an SKSE plugin");
			}

			if(!success)
			{
				// failed, unload the library
				FreeLibrary(plugin.handle);

				// and remove from plugins list
				m_plugins.erase(m_plugins.begin() + i);

				// fix iterator
				i--;
			}
		}
		else
		{
			LogPluginLoadError(plugin, "couldn't load plugin", GetLastError());
		}
	}

	s_currentLoadingPlugin = NULL;
	s_currentPluginHandle = 0;

	// make fake PluginInfo structs after m_plugins is locked
	for(auto & plugin : m_plugins)
	{
		plugin.info.infoVersion = PluginInfo::kInfoVersion;
		plugin.info.name = plugin.version.name;
		plugin.info.version = plugin.version.pluginVersion;
	}

	// alert any listeners that plugin load has finished
	Dispatch_Message(0, SKSEMessagingInterface::kMessage_PostLoad, NULL, 0, NULL);
	// second post-load dispatch
	Dispatch_Message(0, SKSEMessagingInterface::kMessage_PostPostLoad, NULL, 0, NULL);
}

const char * PluginManager::SafeCallLoadPlugin(LoadedPlugin * plugin, const SKSEInterface * skse)
{
	__try
	{
		if(!plugin->load(skse))
		{
			return "reported as incompatible during load";
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		// something very bad happened
		return "disabled, fatal error occurred while loading plugin";
	}

	return NULL;
}

void PluginManager::Sanitize(SKSEPluginVersionData * version)
{
	version->name[sizeof(version->name) - 1] = 0;
	version->author[sizeof(version->author) - 1] = 0;
	version->supportEmail[sizeof(version->supportEmail) - 1] = 0;
}

enum
{
	kCompat_BlockFromRuntime =	1 << 0,
	kCompat_BlockFromEditor =	1 << 1,
};

struct MinVersionEntry
{
	const char	* name;
	UInt32		minVersion;
	const char	* reason;
	UInt32		compatFlags;
};

static const MinVersionEntry	kMinVersionList[] =
{
	{	NULL, 0, NULL }
};

const char * PluginManager::CheckPluginCompatibility(const SKSEPluginVersionData & version)
{
	__try
	{
		// basic validity
		if(!version.dataVersion)
		{
			return "disabled, bad version data";
		}

		if(!version.name[0])
		{
			return "disabled, no name specified";
		}

		// check for 'known bad' versions of plugins
		for(const MinVersionEntry * iter = kMinVersionList; iter->name; ++iter)
		{
			if(!strcmp(iter->name, version.name))
			{
				if(version.pluginVersion < iter->minVersion)
				{
#ifdef RUNTIME
					if(iter->compatFlags & kCompat_BlockFromRuntime)
					{
						return iter->reason;
					}
#endif

#ifdef EDITOR
					if(iter->compatFlags & kCompat_BlockFromEditor)
					{
						return iter->reason;
					}
#endif
				}

				break;
			}
		}

		// version compatibility

		// any claim of version independence?
		bool versionIndependent = version.versionIndependence & (SKSEPluginVersionData::kVersionIndependent_AddressLibraryPostAE | SKSEPluginVersionData::kVersionIndependent_Signatures);

		// verify the address library is there to centralize error message
		if(version.versionIndependence & SKSEPluginVersionData::kVersionIndependent_AddressLibraryPostAE)
		{
			const char * result = CheckAddressLibrary();
			if(result) return result;
		}
		
		// 1.6.629+ has different structure sizes, make sure the plugin specifies which is used
		if(
			versionIndependent &&
			(RUNTIME_VERSION >= RUNTIME_VERSION_1_6_629) &&
			!(version.versionIndependence & SKSEPluginVersionData::kVersionIndependent_StructsPost629))
		{
			// plugins that don't use gameplay structures or that use EXTREME EFFORT can specify that they are truly generic
			if(!(version.versionIndependenceEx & SKSEPluginVersionData::kVersionIndependentEx_NoStructUse))
				return "disabled, only compatible with versions earlier than 1.6.629";
		}
		
		// simple version list
		if(!versionIndependent)
		{
			bool found = false;

			for(UInt32 i = 0; i < _countof(version.compatibleVersions); i++)
			{
				UInt32 compatibleVersion = version.compatibleVersions[i];

				if(compatibleVersion == RUNTIME_VERSION)
				{
					found = true;
					break;
				}
				else if(!compatibleVersion)
				{
					break;
				}
			}

			if(!found)
			{
				return "disabled, incompatible with current version of the game";
			}
		}

		// SE version compatibility
		if(version.seVersionRequired > PACKED_SKSE_VERSION)
		{
			return "disabled, requires newer script extender";
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		// paranoia
		return "disabled, fatal error occurred while checking plugin compatibility";
	}

	return nullptr;
}

void PluginManager::LogPluginLoadError(const LoadedPlugin & pluginSrc, const char * errStr, UInt32 errCode, bool isError)
{
	LoadedPlugin plugin = pluginSrc;

	plugin.errorState = errStr;
	plugin.errorCode = errCode;

	if(isError)
		m_erroredPlugins.push_back(plugin);

	_MESSAGE("plugin %s (%08X %s %08X) %s %d (handle %d)",
		plugin.dllName.c_str(),
		plugin.version.dataVersion,
		plugin.version.name,
		plugin.version.pluginVersion,
		plugin.errorState,
		plugin.errorCode,
		s_currentPluginHandle);
}

void PluginManager::ReportPluginErrors()
{
#if 0
	PluginErrorDialogBox dialog(*this);
	dialog.Show();

	return;
#endif

	if(m_erroredPlugins.empty())
		return;

	if(m_oldAddressLibrary)
		UpdateAddressLibraryPrompt();

	// With this plugin DLL load error, the thread of prophecy is severed. Update your plugins to restore the weave of fate, or persist in the doomed world you have created

	std::string message = "A DLL plugin has failed to load correctly. If a new version of Skyrim was just released, the plugin needs to be updated. Please check the mod's webpage for updates.\n";

	for(auto & plugin : m_erroredPlugins)
	{
		message += "\n";
		message += plugin.dllName + ": " + plugin.errorState;

		if(plugin.errorCode)
		{
			char codeStr[128];
			sprintf_s(codeStr, "%08X", plugin.errorCode);

			message += " (";
			message += codeStr;
			message += ")";
		}
	}

	message += "\n\nContinuing to load may result in lost save data or other undesired behavior.";
	message += "\nExit game? (yes highly suggested)";

	int result = MessageBox(0, message.c_str(), "SKSE Plugin Loader", MB_YESNO);

	if(result == IDYES)
	{
		TerminateProcess(GetCurrentProcess(), 0);
	}
}

void PluginManager::UpdateAddressLibraryPrompt()
{
	int result = MessageBox(0,
		"DLL plugins you have installed require a new version of the Address Library. Either this is a new install, or Skyrim was just updated. Visit the Address Library webpage for updates?",
		"SKSE Plugin Loader", MB_YESNO);

	if(result == IDYES)
	{
		ShellExecute(0, nullptr, "https://www.nexusmods.com/skyrimspecialedition/mods/32444", nullptr, nullptr, 0);
		TerminateProcess(GetCurrentProcess(), 0);
	}
}

void * PluginManager::GetEventDispatcher(UInt32 dispatcherId)
{
	void	* result = nullptr;

#ifdef RUNTIME
	switch(dispatcherId)
	{
	case SKSEMessagingInterface::kDispatcher_ModEvent:
		result = (void *)&g_modCallbackEventDispatcher;
		break;
	case SKSEMessagingInterface::kDispatcher_CameraEvent:
		result = (void *)&g_cameraEventDispatcher;
		break;
	case SKSEMessagingInterface::kDispatcher_CrosshairEvent:
		result = (void *)&g_crosshairRefEventDispatcher;
		break;
	case SKSEMessagingInterface::kDispatcher_ActionEvent:
		result = (void *)&g_actionEventDispatcher;
		break;
	case SKSEMessagingInterface::kDispatcher_NiNodeUpdateEvent:
		result = (void *)&g_ninodeUpdateEventDispatcher;
		break;

	default:
		_WARNING("unknown EventDispatcher %08X", dispatcherId);
		break;
	}
#else
	_WARNING("unknown EventDispatcher %08X", id);
#endif

	return result;
}

// Plugin communication interface
struct PluginListener {
	PluginHandle	listener;
	SKSEMessagingInterface::EventCallback	handleMessage;
};

typedef std::vector<std::vector<PluginListener> > PluginListeners;
static PluginListeners s_pluginListeners;

bool PluginManager::RegisterListener(PluginHandle listener, const char* sender, SKSEMessagingInterface::EventCallback handler)
{
	// because this can be called while plugins are loading, gotta make sure number of plugins hasn't increased
	UInt32 numPlugins = g_pluginManager.GetNumPlugins() + 1;
	if (s_pluginListeners.size() < numPlugins)
	{
		s_pluginListeners.resize(numPlugins + 5);	// add some extra room to avoid unnecessary re-alloc
	}

	_MESSAGE("registering plugin listener for %s at %u of %u", sender, listener, numPlugins);

	// handle > num plugins = invalid
	if (listener > g_pluginManager.GetNumPlugins() || !handler) 
	{
		return false;
	}

	if (sender)
	{
		// is target loaded?
		PluginHandle target = g_pluginManager.LookupHandleFromName(sender);
		if (target == kPluginHandle_Invalid)
		{
			return false;
		}
		// is listener already registered?
		for (std::vector<PluginListener>::iterator iter = s_pluginListeners[target].begin(); iter != s_pluginListeners[target].end(); ++iter)
		{
			if (iter->listener == listener)
			{
				return true;
			}
		}

		// register new listener
		PluginListener newListener;
		newListener.handleMessage = handler;
		newListener.listener = listener;

		s_pluginListeners[target].push_back(newListener);
	}
	else
	{
		// register listener to every loaded plugin
		UInt32 idx = 0;
		for(PluginListeners::iterator iter = s_pluginListeners.begin(); iter != s_pluginListeners.end(); ++iter)
		{
			// don't add the listener to its own list
			if (idx && idx != listener)
			{
				bool skipCurrentList = false;
				for (std::vector<PluginListener>::iterator iterEx = iter->begin(); iterEx != iter->end(); ++iterEx)
				{
					// already registered with this plugin, skip it
					if (iterEx->listener == listener)
					{
						skipCurrentList = true;
						break;
					}
				}
				if (skipCurrentList)
				{
					continue;
				}
				PluginListener newListener;
				newListener.handleMessage = handler;
				newListener.listener = listener;

				iter->push_back(newListener);
			}
			idx++;
		}
	}

	return true;
}

bool PluginManager::Dispatch_Message(PluginHandle sender, UInt32 messageType, void * data, UInt32 dataLen, const char* receiver)
{
	_MESSAGE("dispatch message (%d) to plugin listeners", messageType);
	UInt32 numRespondents = 0;
	PluginHandle target = kPluginHandle_Invalid;

	if (!s_pluginListeners.size())	// no listeners yet registered
	{
		_MESSAGE("no listeners registered");
		return false;
	}
	else if (sender >= s_pluginListeners.size())
	{
		_MESSAGE("sender is not in the list");
		return false;
	}

	if (receiver)
	{
		target = g_pluginManager.LookupHandleFromName(receiver);
		if (target == kPluginHandle_Invalid)
			return false;
	}

	const char* senderName = g_pluginManager.GetPluginNameFromHandle(sender);
	if (!senderName)
		return false;
	for (std::vector<PluginListener>::iterator iter = s_pluginListeners[sender].begin(); iter != s_pluginListeners[sender].end(); ++iter)
	{
		SKSEMessagingInterface::Message msg;
		msg.data = data;
		msg.type = messageType;
		msg.sender = senderName;
		msg.dataLen = dataLen;

		if (target != kPluginHandle_Invalid)	// sending message to specific plugin
		{
			if (iter->listener == target)
			{
				iter->handleMessage(&msg);
				return true;
			}
		}
		else
		{
			_DMESSAGE("sending message type %u to plugin %u", messageType, iter->listener);
			iter->handleMessage(&msg);
			numRespondents++;
		}
	}
	_DMESSAGE("dispatched message.");
	return numRespondents ? true : false;
}

const char * PluginManager::GetPluginNameFromHandle(PluginHandle handle)
{
	if (handle > 0 && handle <= m_plugins.size())
		return (m_plugins[handle - 1].version.name);
	else if (handle == 0)
		return "SKSE";

	return NULL;
}

PluginHandle PluginManager::LookupHandleFromName(const char* pluginName)
{
	if (!_stricmp("SKSE", pluginName))
		return 0;

	UInt32	idx = 1;
	for(LoadedPluginList::iterator iter = m_plugins.begin(); iter != m_plugins.end(); ++iter)
	{
		LoadedPlugin	* plugin = &(*iter);
		if(!_stricmp(plugin->version.name, pluginName))
		{
			return idx;
		}
		idx++;
	}
	return kPluginHandle_Invalid;
}

void PluginErrorDialogBox::Show()
{
	extern HINSTANCE g_moduleHandle;

	CreateDialogParam(g_moduleHandle, MAKEINTRESOURCE(IDD_PLUGINERROR), NULL, _DialogProc, (LPARAM)this);
	UInt32 err = GetLastError();
}

INT_PTR PluginErrorDialogBox::_DialogProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam)
{
	INT_PTR result = 0;
	PluginErrorDialogBox * context = nullptr;

	if(msg == WM_INITDIALOG)
	{
		context = (PluginErrorDialogBox *)lParam;
		context->m_window = window;
		SetWindowLongPtr(window, GWLP_USERDATA, lParam);
	}
	else
	{
		context = (PluginErrorDialogBox *)GetWindowLongPtr(window, GWLP_USERDATA);
	}

	if(context)
		result = context->DialogProc(msg, wParam, lParam);

	return result;
}

INT_PTR PluginErrorDialogBox::DialogProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	INT_PTR result = FALSE;

	switch(msg)
	{
		case WM_INITDIALOG:
			result = TRUE;
			break;

		case WM_COMMAND:
		{
			bool done = false;

			switch(LOWORD(wParam))
			{
				case IDCANCEL:
					done = true;
					m_exitGame = true;
					break;

				case IDOK:
					done = true;
					break;
			}

			if(done)
			{
				DestroyWindow(m_window);
			}
		}
		break;

		default:
			result = FALSE;
			break;
	}

	return result;
}

inline void * BranchTrampolineManager::Allocate(PluginHandle plugin, size_t size)
{
	auto mem = m_trampoline.Allocate(size);
	if (mem) {
		std::lock_guard<decltype(m_lock)> locker(m_lock);

		auto findIt = m_stats.find(plugin);
		if (findIt != m_stats.end()) {
			findIt->second += size;
		}
		else {
			auto insIt = m_stats.insert(std::make_pair(plugin, size));
			ASSERT(insIt.second);   // insertion failed
		}
	}
	else {
		ASSERT(false);  // alloc failed
	}
	return mem;
}


void * AllocateFromSKSEBranchPool(PluginHandle plugin, size_t size)
{
	if (s_trampolineLog) {
		_DMESSAGE("plugin %d allocated %lld bytes from branch pool", plugin, size);
	}
	return g_branchTrampolineManager.Allocate(plugin, size);
}

void * AllocateFromSKSELocalPool(PluginHandle plugin, size_t size)
{
	if (s_trampolineLog) {
		_DMESSAGE("plugin %d allocated %lld bytes from local pool", plugin, size);
	}
	return g_localTrampolineManager.Allocate(plugin, size);
}
