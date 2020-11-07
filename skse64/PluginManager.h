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

	bool	Init(void);
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
		// internals
		HMODULE		handle;
		PluginInfo	info;

		_SKSEPlugin_Query	query;
		_SKSEPlugin_Load	load;
	};

	bool	FindPluginDirectory(void);
	void	InstallPlugins(void);

	const char *	SafeCallQueryPlugin(LoadedPlugin * plugin, const SKSEInterface * skse);
	const char *	SafeCallLoadPlugin(LoadedPlugin * plugin, const SKSEInterface * skse);

	const char *	CheckPluginCompatibility(LoadedPlugin * plugin);

	typedef std::vector <LoadedPlugin>	LoadedPluginList;

	std::string			m_pluginDirectory;
	LoadedPluginList	m_plugins;

	static LoadedPlugin		* s_currentLoadingPlugin;
	static PluginHandle		s_currentPluginHandle;
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
