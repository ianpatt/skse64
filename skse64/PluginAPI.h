#pragma once

typedef UInt32	PluginHandle;	// treat this as an opaque type
class GFxMovieView;
class GFxValue;
class TaskDelegate;
class UIDelegate_v1;
class InventoryEntryData;
class SKSEDelayFunctorManager;
class SKSEObjectRegistry;
class SKSEPersistentObjectStorage;
class BranchTrampoline;

struct PluginInfo
{
	enum
	{
		kInfoVersion = 1
	};

	UInt32			infoVersion;
	const char *	name;
	UInt32			version;
};

enum
{
	kPluginHandle_Invalid = 0xFFFFFFFF
};

enum
{
	kInterface_Invalid = 0,
	kInterface_Scaleform,
	kInterface_Papyrus,
	kInterface_Serialization,
	kInterface_Task,
	kInterface_Messaging,
	kInterface_Object,
	kInterface_Trampoline,
	kInterface_Max,
};

struct SKSEInterface
{
	UInt32	skseVersion;
	UInt32	runtimeVersion;
	UInt32	editorVersion;
	UInt32	isEditor;
	void *	(* QueryInterface)(UInt32 id);

	// call during your Query or Load functions to get a PluginHandle uniquely identifying your plugin
	// invalid if called at any other time, so call it once and save the result
	PluginHandle		(* GetPluginHandle)(void);
	
	// returns the SKSE build's release index
	UInt32				(* GetReleaseIndex)(void);

	// Minimum SKSE version 2.0.18
	// returns the plugin info structure for a plugin by name, only valid to be called after PostLoad message
	const PluginInfo*	(* GetPluginInfo)(const char* name);
};

struct SKSEScaleformInterface
{
	enum
	{
		kInterfaceVersion = 2
	};

	UInt32	interfaceVersion;

	// This callback will be called once for every new menu that is created.
	// Create your objects relative to the 'root' GFxValue parameter.
	typedef bool (* RegisterCallback)(GFxMovieView * view, GFxValue * root);
	typedef void (* RegisterInventoryCallback)(GFxMovieView * view, GFxValue * object, InventoryEntryData * item);

	// Register your plugin's scaleform API creation callback here.
	// The "name" parameter will be used to create an object with the path:
	// "skse.plugins.name" that will be passed to the callback.
	// Make sure that the memory it points to is valid from the point the callback
	// is registered until the game exits.
	bool	(* Register)(const char * name, RegisterCallback callback);

	// Registers your plugin for when item data is extended to the UI
	// either favorites menu, or inventory menu
	void    (* RegisterForInventory)(RegisterInventoryCallback callback);
};

struct SKSESerializationInterface
{
	enum
	{
		kVersion = 4,
	};
	
	typedef void (* EventCallback)(SKSESerializationInterface * intfc);

	typedef void (* FormDeleteCallback)(UInt64 handle);

	UInt32	version;

	void	(* SetUniqueID)(PluginHandle plugin, UInt32 uid);

	void	(* SetRevertCallback)(PluginHandle plugin, EventCallback callback);
	void	(* SetSaveCallback)(PluginHandle plugin, EventCallback callback);
	void	(* SetLoadCallback)(PluginHandle plugin, EventCallback callback);
	void	(* SetFormDeleteCallback)(PluginHandle plugin, FormDeleteCallback callback);

	bool	(* WriteRecord)(UInt32 type, UInt32 version, const void * buf, UInt32 length);
	bool	(* OpenRecord)(UInt32 type, UInt32 version);
	bool	(* WriteRecordData)(const void * buf, UInt32 length);

	bool	(* GetNextRecordInfo)(UInt32 * type, UInt32 * version, UInt32 * length);
	UInt32	(* ReadRecordData)(void * buf, UInt32 length);
	bool	(* ResolveHandle)(UInt64 handle, UInt64 * handleOut);
	bool	(* ResolveFormId)(UInt32 formId, UInt32 * formIdOut);
};

struct SKSETaskInterface
{
	enum
	{
		kInterfaceVersion = 2
	};

	UInt32	interfaceVersion;

	// Derive your type from TaskDelegate or UIDelegate
	// Allocate before adding
	// Define your Run function
	// Delete your object in the Dispose call
	void	(* AddTask)(TaskDelegate * task);
	void	(* AddUITask)(UIDelegate_v1 * task);
};

class VMClassRegistry;

struct SKSEPapyrusInterface
{
	enum
	{
		kInterfaceVersion = 1
	};
	UInt32	interfaceVersion;
	typedef bool (* RegisterFunctions)(VMClassRegistry * registry);
	bool	(* Register)(RegisterFunctions callback);
};

/**** Messaging API docs ********************************************************************
 *
 *	Messaging API allows inter-plugin communication at run-time. A plugin may register
 *	one callback for each plugin from which it wishes to receive messages, specifying
 *	the sender by name in the call to RegisterListener(). RegisterListener returns false
 *	if the specified plugin is not loaded, true otherwise. Any messages dispatched by
 *	the specified plugin will then be forwarded to the listener as they occur. Passing NULL as 
 *	the sender registers the calling plugin as a listener to every loaded plugin.
 *
 *	Messages may be dispatched via Dispatch() to either a specific listener (specified
 *	by name) or to all listeners (with NULL passed as the receiver). The contents and format of
 *	messageData are left up to the sender, and the receiver is responsible for casting the message
 *	to the expected type. If no plugins are registered as listeners for the sender, 
 *	Dispatch() returns false, otherwise it returns true.
 *
 *	Calling RegisterListener() or Dispatch() during plugin load is not advised as the requested plugin
 *	may not yet be loaded at that point. Instead, if you wish to register as a listener or dispatch a
 *	message immediately after plugin load, use RegisterListener() during load to register to receive
 *	messages from SKSE (sender name: "SKSE"). You will then receive a message from SKSE once 
 *	all plugins have been loaded, at which point it is safe to establish communications between
 *	plugins.
 *
 *	Some plugin authors may wish to use strings instead of integers to denote message type. In
 *	that case the receiver can pass the address of the string as an integer and require the receiver
 *	to cast it back to a char* on receipt.
 *
 *********************************************************************************************/

struct SKSEMessagingInterface
{
	struct Message {
		const char	* sender;
		UInt32		type;
		UInt32		dataLen;
		void		* data;
	};

	typedef void (* EventCallback)(Message* msg);

	enum {
		kInterfaceVersion = 2
	};

	// SKSE messages
	enum {
		kMessage_PostLoad,		// sent to registered plugins once all plugins have been loaded (no data)
		kMessage_PostPostLoad,	// sent right after kMessage_PostLoad to facilitate the correct dispatching/registering of messages/listeners
		kMessage_PreLoadGame,	// dispatched immediately before savegame is read by Fallout
								// dataLen: length of file path, data: char* file path of .ess savegame file
		kMessage_PostLoadGame,	//dispatched after an attempt to load a saved game has finished (the game's LoadGame() routine
								//has returned). You will probably want to handle this event if your plugin uses a Preload callback
								//as there is a chance that after that callback is invoked the game will encounter an error
								//while loading the saved game (eg. corrupted save) which may require you to reset some of your
								//plugin state.
								//data: bool, true if game successfully loaded, false otherwise
								// plugins may register as listeners during the first callback while deferring dispatches until the next
		kMessage_SaveGame,
		kMessage_DeleteGame,	// sent right before deleting the .skse cosave and the .ess save.
								// dataLen: length of file path, data: char* file path of .ess savegame file
		kMessage_InputLoaded,	// sent right after game input is loaded, right before the main menu initializes
		kMessage_NewGame,		// sent after a new game is created, before the game has loaded (Sends CharGen TESQuest pointer)
		kMessage_DataLoaded		// send after the data handler has loaded all its forms
	};

	UInt32	interfaceVersion;
	bool	(* RegisterListener)(PluginHandle listener, const char* sender, EventCallback handler);
	bool	(* Dispatch)(PluginHandle sender, UInt32 messageType, void * data, UInt32 dataLen, const char* receiver);

	enum
	{
		kDispatcher_ModEvent = 0,
		kDispatcher_CameraEvent,
		kDispatcher_CrosshairEvent,
		kDispatcher_ActionEvent,
		kDispatcher_NiNodeUpdateEvent
	};

	// Use this to acquire SKSE's internal EventDispatchers so that you can sink to them
	void	* (* GetEventDispatcher)(UInt32 dispatcherId);
};

struct SKSEObjectInterface
{
	enum
	{
		kInterfaceVersion = 1
	};

	UInt32	interfaceVersion;

	SKSEDelayFunctorManager & (* GetDelayFunctorManager)();
	SKSEObjectRegistry & (* GetObjectRegistry)();
	SKSEPersistentObjectStorage & (* GetPersistentObjectStorage)();
};

struct SKSETrampolineInterface
{
	enum
	{
		kInterfaceVersion = 1
	};

	UInt32	interfaceVersion;

	void* (*AllocateFromBranchPool)(PluginHandle plugin, size_t size);
	void* (*AllocateFromLocalPool)(PluginHandle plugin, size_t size);
};

typedef bool(*_SKSEPlugin_Load)(const SKSEInterface * skse);

/**** plugin versioning ********************************************************
 *
 *	The AE version of Skyrim SE broke plugin versioning as many were written
 *	with the assumption that the Address Library would always be valid.
 *	These always report that they are compatible, then exit on startup because
 *	they cannot find their address library file.
 *	
 *	To work around this problem, version checking has been reimplemented and
 *	no longer calls any code. Plugins declare their compatibility, and SKSE
 *	determines whether to load the plugin. Setting this up is simple, just
 *	add something like this to your project:
 *	

extern "C" {
__declspec(dllexport) SKSEPluginVersionData SKSEPlugin_Version =
{
	SKSEPluginVersionData::kVersion,
	
	1,
	"my awesome plugin",

	"my name",
	"support@example.com",

	0,	// not version independent
	{ RUNTIME_VERSION_1_6_318, 0 },	// compatible with 1.6.318

	0,	// works with any version of the script extender. you probably do not need to put anything here
};
};

 *	
 ******************************************************************************/

struct SKSEPluginVersionData
{
	enum
	{
		kVersion = 1,
	};

	enum
	{
		// set this if you are using a (potential at this time of writing) post-AE version of the Address Library
		kVersionIndependent_AddressLibraryPostAE = 1 << 0,
		// set this if you exclusively use signature matching to find your addresses and have NO HARDCODED ADDRESSES
		kVersionIndependent_Signatures = 1 << 1,
	};

	UInt32	dataVersion;			// set to kVersion

	UInt32	pluginVersion;			// version number of your plugin
	char	name[256];				// null-terminated ASCII plugin name

	char	author[256];			// null-terminated ASCII plugin author name (can be empty)
	char	supportEmail[256];		// null-terminated ASCII support email address (can be empty)

	// version compatibility
	UInt32	versionIndependence;	// set to one of the kVersionIndependent_ enums or zero
	UInt32	compatibleVersions[16];	// zero-terminated list of RUNTIME_VERSION_ defines your plugin is compatible with

	UInt32	seVersionRequired;		// minimum version of the script extender required, compared against PACKED_SKSE_VERSION
									// you probably should just set this to 0 unless you know what you are doing
};

/**** plugin API docs **********************************************************
 *	
 *	The base API is pretty simple. Add version data as shown in the
 *	SKSEPluginVersionData docs above, and export this function:
 *	
 *	bool SKSEPlugin_Load(const SKSEInterface * skse)
 *	
 *	In this function, use the interfaces above to register your commands, patch
 *	memory, generally do whatever you need to for integration with the runtime.
 *	
 *	At this time, or at any point forward you can call the QueryInterface
 *	callback to retrieve an interface structure for the base services provided
 *	by the SKSE core.
 *	
 *	You may optionally return false from this function to unload your plugin,
 *	but make sure that you DO NOT register any commands if you do.
 *	
 *	Note that all structure versions are backwards-compatible, so you only need
 *	to check against the latest version that you need. New fields will be only
 *	added to the end, and all old fields will remain compatible with their
 *	previous implementations.
 *	
 ******************************************************************************/
