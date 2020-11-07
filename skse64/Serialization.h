#pragma once

#include "skse64/PluginAPI.h"
#include "skse64/GameTypes.h"

#include <string>

class GFxValue;

extern SKSESerializationInterface	g_SKSESerializationInterface;

namespace Serialization
{
	struct PluginCallbacks
	{
		PluginCallbacks()
			:revert(NULL)
			,save(NULL)
			,load(NULL)
			,formDelete(NULL)
			,uid(0)
			,hadData(false)
			,hadUID(false) { }

		SKSESerializationInterface::EventCallback	revert;
		SKSESerializationInterface::EventCallback	save;
		SKSESerializationInterface::EventCallback	load;
		SKSESerializationInterface::FormDeleteCallback	formDelete;
		
		UInt32	uid;

		bool	hadData;
		bool	hadUID;
	};

	// plugin API
	void	SetUniqueID(PluginHandle plugin, UInt32 uid);
	void	SetRevertCallback(PluginHandle plugin, SKSESerializationInterface::EventCallback callback);
	void	SetSaveCallback(PluginHandle plugin, SKSESerializationInterface::EventCallback callback);
	void	SetLoadCallback(PluginHandle plugin, SKSESerializationInterface::EventCallback callback);
	void	SetFormDeleteCallback(PluginHandle plugin, SKSESerializationInterface::FormDeleteCallback callback);

	void	SetSaveName(const char * name);
	bool	WriteRecord(UInt32 type, UInt32 version, const void * buf, UInt32 length);
	bool	OpenRecord(UInt32 type, UInt32 version);
	bool	WriteRecordData(const void * buf, UInt32 length);

	bool	GetNextRecordInfo(UInt32 * type, UInt32 * version, UInt32 * length);
	UInt32	ReadRecordData(void * buf, UInt32 length);

	bool	ResolveFormId(UInt32 formId, UInt32 * formIdOut);
	bool	ResolveHandle(UInt64 handle, UInt64 * handleOut);

	// internal event handlers
	void	HandleRevertGlobalData(void);
	void	HandleSaveGlobalData(void);
	void	HandleLoadGlobalData(void);

	void	HandleDeleteSave(std::string saveName);
	void	HandleDeletedForm(UInt64 handle);

	// template helper functions
	template <typename T>
	bool WriteData(SKSESerializationInterface * intfc, const T * data)
	{
		return intfc->WriteRecordData(data, sizeof(T));
	}

	template <typename T>
	bool ReadData(SKSESerializationInterface * intfc, T * data)
	{
		return intfc->ReadRecordData(data, sizeof(T)) > 0;
	}

	template <> bool WriteData<BSFixedString>(SKSESerializationInterface * intfc, const BSFixedString * data);
	template <> bool ReadData<BSFixedString>(SKSESerializationInterface * intfc, BSFixedString * data);

	template <> bool WriteData<std::string>(SKSESerializationInterface * intfc, const std::string * data);
	template <> bool ReadData<std::string>(SKSESerializationInterface * intfc, std::string * data);

	// Note: Read would have to allocate somehow. You have to do that manually.
	template <> bool WriteData<const char>(SKSESerializationInterface * intfc, const char* data);

	template <>
	bool WriteData<GFxValue>(SKSESerializationInterface* intfc, const GFxValue* val);
	
	template <>
	bool ReadData<GFxValue>(SKSESerializationInterface* intfc, GFxValue* val);

	template <typename T>
	bool SaveClassHelper(SKSESerializationInterface* intfc, UInt32 type, T& instance)
	{
		if (! intfc->OpenRecord(type, T::kSaveVersion))
			return false;

		return instance.Save(intfc);
	}
}
