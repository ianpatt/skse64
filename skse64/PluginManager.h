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
		LoadedPlugin();

		std::string dllName;

		HMODULE		handle = 0;
		PluginInfo	info;
		UInt32		internalHandle = 0;

		SKSEPluginVersionData	version;

		_SKSEPlugin_Load	load = nullptr;

		const char			* errorState = nullptr;
		UInt32				errorCode = 0;
	};

	bool	FindPluginDirectory(void);
	void	ScanPlugins(void);
	void	InstallPlugins(void);

	const char *	SafeCallLoadPlugin(LoadedPlugin * plugin, const SKSEInterface * skse);

	void			Sanitize(SKSEPluginVersionData * version);
	const char *	CheckPluginCompatibility(const SKSEPluginVersionData & version);
	const char *	CheckAddressLibrary(void);

	void			LogPluginLoadError(const LoadedPlugin & plugin, const char * errStr, UInt32 errCode = 0, bool isError = true);
	void			ReportPluginErrors();
	void			UpdateAddressLibraryPrompt();

	typedef std::vector <LoadedPlugin>	LoadedPluginList;

	std::string			m_pluginDirectory;
	LoadedPluginList	m_plugins;

	LoadedPluginList	m_erroredPlugins;

	bool				m_oldAddressLibrary = false;

	static LoadedPlugin		* s_currentLoadingPlugin;
	static PluginHandle		s_currentPluginHandle;
};

class PluginErrorDialogBox
{
public:
	PluginErrorDialogBox() = delete;
	PluginErrorDialogBox(const PluginManager & mgr)
		:m_owner(mgr) { }

	void	Show();

	bool	ShouldExitGame() { return m_exitGame; }

private:
	static INT_PTR _DialogProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam);
	INT_PTR DialogProc(UINT msg, WPARAM wParam, LPARAM lParam);

	HWND m_window = 0;
	bool m_exitGame = false;

	const PluginManager & m_owner;
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
