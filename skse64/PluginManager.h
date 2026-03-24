#pragma once

#include <string>
#include <vector>
#include <mutex>
#include <unordered_map>

#include "skse64/PluginAPI.h"

class PluginManager
{
public:
	PluginManager();
	~PluginManager();

	enum
	{
		kPhase_Preload = 0,
		kPhase_Load,

		kPhase_Num,
	};

	void	Init(void);
	void	InstallPlugins(UInt32 phase);
	void	LoadComplete();
	void	DeInit(void);

	PluginInfo *	GetInfoByName(const char * name);
	const char *	GetPluginNameFromHandle(PluginHandle handle);

	PluginHandle	LookupHandleFromName(const char* pluginName);

	UInt32			GetNumPlugins(void);

	static void *				QueryInterface(UInt32 id);
	static PluginHandle			GetPluginHandle(void);
	static UInt32				GetReleaseIndex(void);
	static const PluginInfo*	GetPluginInfo(const char* name);

	static void * GetEventDispatcher(UInt32 dispatcherId);

	static bool Dispatch_Message(PluginHandle sender, UInt32 messageType, void * data, UInt32 dataLen, const char* receiver);
	static bool	RegisterListener(PluginHandle listener, const char* sender, SKSEMessagingInterface::EventCallback handler);

private:
	struct LoadedPlugin
	{
		LoadedPlugin();

		std::string dllName;

		HMODULE		handle = 0;
		PluginInfo	info;
		UInt32		internalHandle = 0;

		SKSEPluginVersionData	version;

		_SKSEPlugin_Load	load[kPhase_Num] = { nullptr };

		const char			* errorState = nullptr;
		UInt32				errorCode = 0;

		bool	hasLoad = false;
		bool	hasPreload = false;
	};

	bool	FindPluginDirectory(void);
	void	ScanPlugins(void);

	const char *	SafeCallLoadPlugin(LoadedPlugin * plugin, const SKSEInterface * skse, UInt32 phase);

	void			Sanitize(SKSEPluginVersionData * version);
	const char *	CheckPluginCompatibility(const SKSEPluginVersionData & version);
	const char *	CheckAddressLibrary(void);

	void			LogPluginLoadError(const LoadedPlugin & plugin, const char * errStr, UInt32 errCode = 0, bool isError = true);
	void			ReportPluginErrors();
	void			UpdateAddressLibraryPrompt();

	void			CallPostLoad();

	typedef std::vector <LoadedPlugin>	LoadedPluginList;

	std::string			m_pluginDirectory;
	LoadedPluginList	m_plugins;

	LoadedPluginList	m_erroredPlugins;

	bool				m_oldAddressLibrary = false;

	static LoadedPlugin		* s_currentLoadingPlugin;
	static PluginHandle		s_currentPluginHandle;

	static PluginHandle		s_dispatchingPluginHandle;

	// Plugin communication interface
	struct PluginListener {
		PluginHandle	listener;
		SKSEMessagingInterface::EventCallback	handleMessage;
	};

	typedef std::vector<std::vector<PluginListener> > PluginListeners;
	static PluginListeners s_pluginListeners;
};

class BranchTrampolineManager
{
public:
	inline BranchTrampolineManager(BranchTrampoline& trampoline) :
		m_trampoline(trampoline)
	{}

	inline void* Allocate(PluginHandle plugin, size_t size);

private:
	BranchTrampoline& m_trampoline;
	std::mutex m_lock;
	std::unordered_map<PluginHandle, size_t> m_stats;
};

extern BranchTrampolineManager g_branchTrampolineManager;
extern BranchTrampolineManager g_localTrampolineManager;

void* AllocateFromSKSEBranchPool(PluginHandle plugin, size_t size);
void* AllocateFromSKSELocalPool(PluginHandle plugin, size_t size);

extern PluginManager	g_pluginManager;
