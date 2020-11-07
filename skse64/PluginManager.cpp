#include "skse64/PluginManager.h"
#include "common/IDirectoryIterator.h"
#include "skse64/GameAPI.h"
#include "skse64_common/Utilities.h"
#include "skse64/Serialization.h"
#include "skse64_common/skse_version.h"
#include "skse64/PapyrusEvents.h"
#include "skse64_common/BranchTrampoline.h"

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

bool PluginManager::Init(void)
{
	bool	result = false;

	if(FindPluginDirectory())
	{
		_MESSAGE("plugin directory = %s", m_pluginDirectory.c_str());

		__try
		{
			InstallPlugins();

			result = true;
		}
		__except(EXCEPTION_EXECUTE_HANDLER)
		{
			// something very bad happened
			_ERROR("exception occurred while loading plugins");
		}
	}

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

void PluginManager::InstallPlugins(void)
{
	// avoid realloc
	m_plugins.reserve(5);

	for(IDirectoryIterator iter(m_pluginDirectory.c_str(), "*.dll"); !iter.Done(); iter.Next())
	{
		std::string	pluginPath = iter.GetFullPath();

		_MESSAGE("checking plugin %s", pluginPath.c_str());

		LoadedPlugin	plugin;
		memset(&plugin, 0, sizeof(plugin));

		s_currentLoadingPlugin = &plugin;
		s_currentPluginHandle = m_plugins.size() + 1;	// +1 because 0 is reserved for internal use

		plugin.handle = (HMODULE)LoadLibrary(pluginPath.c_str());
		if(plugin.handle)
		{
			bool		success = false;

			plugin.query = (_SKSEPlugin_Query)GetProcAddress(plugin.handle, "SKSEPlugin_Query");
			plugin.load = (_SKSEPlugin_Load)GetProcAddress(plugin.handle, "SKSEPlugin_Load");

			if(plugin.query && plugin.load)
			{
				const char	* loadStatus = NULL;

				loadStatus = SafeCallQueryPlugin(&plugin, &g_SKSEInterface);

				if(!loadStatus)
				{
					loadStatus = CheckPluginCompatibility(&plugin);

					if(!loadStatus)
					{
						loadStatus = SafeCallLoadPlugin(&plugin, &g_SKSEInterface);

						if(!loadStatus)
						{
							loadStatus = "loaded correctly";
							success = true;
						}
					}
				}
				else
				{
					loadStatus = "reported as incompatible during query";
				}

				ASSERT(loadStatus);

				_MESSAGE("plugin %s (%08X %s %08X) %s (handle %d)",
					pluginPath.c_str(),
					plugin.info.infoVersion,
					plugin.info.name ? plugin.info.name : "<NULL>",
					plugin.info.version,
					loadStatus,
					s_currentPluginHandle);
			}
			else
			{
				_MESSAGE("plugin %s does not appear to be an SKSE plugin", pluginPath.c_str());
			}

			if(success)
			{
				// succeeded, add it to the list
				m_plugins.push_back(plugin);
			}
			else
			{
				// failed, unload the library
				FreeLibrary(plugin.handle);
			}
		}
		else
		{
			_ERROR("couldn't load plugin %s (Error %d)", pluginPath.c_str(), GetLastError());
		}
	}

	s_currentLoadingPlugin = NULL;
	s_currentPluginHandle = 0;

	// alert any listeners that plugin load has finished
	Dispatch_Message(0, SKSEMessagingInterface::kMessage_PostLoad, NULL, 0, NULL);
	// second post-load dispatch
	Dispatch_Message(0, SKSEMessagingInterface::kMessage_PostPostLoad, NULL, 0, NULL);
}

// SEH-wrapped calls to plugin API functions to avoid bugs from bringing down the core
const char * PluginManager::SafeCallQueryPlugin(LoadedPlugin * plugin, const SKSEInterface * skse)
{
	__try
	{
		if(!plugin->query(skse, &plugin->info))
		{
			return "reported as incompatible during query";
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		// something very bad happened
		return "disabled, fatal error occurred while querying plugin";
	}

	return NULL;
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
	// returns true for multiple versions of the runtime
	{	"papyrusutil plugin",	2,	"broken version check, will crash the game", kCompat_BlockFromRuntime },

	{	NULL, 0, NULL }
};

// see if we have a plugin that we know causes problems
const char * PluginManager::CheckPluginCompatibility(LoadedPlugin * plugin)
{
	__try
	{
		// stupid plugin check
		if(!plugin->info.name)
		{
			return "disabled, no name specified";
		}

		// check for 'known bad' versions of plugins
		for(const MinVersionEntry * iter = kMinVersionList; iter->name; ++iter)
		{
			if(!strcmp(iter->name, plugin->info.name))
			{
				if(plugin->info.version < iter->minVersion)
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
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		// paranoia
		return "disabled, fatal error occurred while checking plugin compatibility";
	}

	return NULL;
}

void * PluginManager::GetEventDispatcher(UInt32 dispatcherId)
{
	void	* result = NULL;

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
		return (m_plugins[handle - 1].info.name);
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
		if(!_stricmp(plugin->info.name, pluginName))
		{
			return idx;
		}
		idx++;
	}
	return kPluginHandle_Invalid;
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
