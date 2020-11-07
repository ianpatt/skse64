#include "skse64/Serialization.h"
#include "common/IFileStream.h"
#include "skse64/PluginManager.h"
#include "GameAPI.h"
#include "skse64_common/skse_version.h"
#include <vector>
#include <shlobj.h>
#include "GameData.h"
#include "skse64/InternalSerialization.h"
#include "skse64/GameSettings.h"
#include "skse64/ScaleformCallbacks.h"

namespace Serialization
{
	const char * kSavegamePath = "\\My Games\\Skyrim Special Edition\\";

	// file format internals

	//	general format:
	//	Header			header
	//		PluginHeader	plugin[header.numPlugins]
	//			ChunkHeader		chunk[plugin.numChunks]
	//				UInt8			data[chunk.length]

	struct Header
	{
		enum
		{
			kSignature =		MACRO_SWAP32('SKSE'),	// endian-swapping so the order matches
			kVersion =			1,

			kVersion_Invalid =	0
		};

		UInt32	signature;
		UInt32	formatVersion;
		UInt32	skseVersion;
		UInt32	runtimeVersion;
		UInt32	numPlugins;
	};

	struct PluginHeader
	{
		UInt32	signature;
		UInt32	numChunks;
		UInt32	length;		// length of following data including ChunkHeader
	};

	struct ChunkHeader
	{
		UInt32	type;
		UInt32	version;
		UInt32	length;
	};

	// locals

	std::string		s_savePath;
	IFileStream		s_currentFile;

	typedef std::vector <PluginCallbacks>	PluginCallbackList;
	PluginCallbackList	s_pluginCallbacks;

	PluginHandle	s_currentPlugin = 0;

	Header			s_fileHeader = { 0 };

	UInt64			s_pluginHeaderOffset = 0;
	PluginHeader	s_pluginHeader = { 0 };

	bool			s_chunkOpen = false;
	UInt64			s_chunkHeaderOffset = 0;
	ChunkHeader		s_chunkHeader = { 0 };

	// utilities

	// make full path from save name
	std::string MakeSavePath(std::string name, const char * extension)
	{
		char	path[MAX_PATH];
		ASSERT(SUCCEEDED(SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, path)));

		std::string	result = path;
		result += kSavegamePath;
		Setting* localSavePath = GetINISetting("sLocalSavePath:General");
		if(localSavePath && (localSavePath->GetType() == Setting::kType_String))
			result += localSavePath->data.s;
		else
			result += "Saves\\";

		result += "\\";
		result += name;
		if (extension)
			result += extension;
		return result;
	}

	PluginCallbacks * GetPluginInfo(PluginHandle plugin)
	{
		if(plugin >= s_pluginCallbacks.size())
			s_pluginCallbacks.resize(plugin + 1);

		return &s_pluginCallbacks[plugin];
	}

	// plugin API
	void SetUniqueID(PluginHandle plugin, UInt32 uid)
	{
		// check existing plugins
		for(PluginCallbackList::iterator iter = s_pluginCallbacks.begin(); iter != s_pluginCallbacks.end(); ++iter)
		{
			if(iter->hadUID && (iter->uid == uid))
			{
				UInt32	collidingID = iter - s_pluginCallbacks.begin();

				_ERROR("plugin serialization UID collision (uid = %08X, plugins = %d %d)", plugin, uid, collidingID);
			}
		}

		PluginCallbacks * info = GetPluginInfo(plugin);

		ASSERT(!info->hadUID);

		info->uid = uid;
		info->hadUID = true;
	}

	void SetRevertCallback(PluginHandle plugin, SKSESerializationInterface::EventCallback callback)
	{
		GetPluginInfo(plugin)->revert = callback;
	}

	void SetSaveCallback(PluginHandle plugin, SKSESerializationInterface::EventCallback callback)
	{
		GetPluginInfo(plugin)->save = callback;
	}

	void SetLoadCallback(PluginHandle plugin, SKSESerializationInterface::EventCallback callback)
	{
		GetPluginInfo(plugin)->load = callback;
	}

	void SetFormDeleteCallback(PluginHandle plugin, SKSESerializationInterface::FormDeleteCallback callback)
	{
		GetPluginInfo(plugin)->formDelete = callback;
	}

	void SetSaveName(const char * name)
	{
		if(name)
		{
			std::string save_name(name);

			if (save_name.length() >= 4 && _stricmp(name+save_name.length()-4, ".ess") == 0)
				save_name = save_name.substr(0, save_name.length() - 4);								
			
			_MESSAGE("save name is %s", save_name.c_str());
			s_savePath = MakeSavePath(save_name, ".skse");
			_MESSAGE("full save path: %s", s_savePath.c_str());
		}
		else
		{
			_MESSAGE("cleared save path");
			s_savePath.clear();
		}
	}

	bool WriteRecord(UInt32 type, UInt32 version, const void * buf, UInt32 length)
	{
		if(!OpenRecord(type, version))
			return false;

		return WriteRecordData(buf, length);
	}

	// flush a chunk header to the file if one is currently open
	static void FlushWriteChunk(void)
	{
		if(!s_chunkOpen)
			return;

		UInt64	curOffset = s_currentFile.GetOffset();
		UInt64	chunkSize = curOffset - s_chunkHeaderOffset - sizeof(s_chunkHeader);

		ASSERT(chunkSize < 0x80000000);	// stupidity check

		s_chunkHeader.length = (UInt32)chunkSize;

		s_currentFile.SetOffset(s_chunkHeaderOffset);
		s_currentFile.WriteBuf(&s_chunkHeader, sizeof(s_chunkHeader));

		s_currentFile.SetOffset(curOffset);

		s_pluginHeader.length += chunkSize + sizeof(s_chunkHeader);

		s_chunkOpen = false;
	}

	bool OpenRecord(UInt32 type, UInt32 version)
	{
		if(!s_pluginHeader.numChunks)
		{
			ASSERT(!s_chunkOpen);

			s_pluginHeaderOffset = s_currentFile.GetOffset();
			s_currentFile.Skip(sizeof(s_pluginHeader));
		}

		FlushWriteChunk();

		s_chunkHeaderOffset = s_currentFile.GetOffset();
		s_currentFile.Skip(sizeof(s_chunkHeader));

		s_pluginHeader.numChunks++;

		s_chunkHeader.type = type;
		s_chunkHeader.version = version;
		s_chunkHeader.length = 0;

		s_chunkOpen = true;

		return true;
	}

	bool WriteRecordData(const void * buf, UInt32 length)
	{
		s_currentFile.WriteBuf(buf, length);

		return true;
	}

	static void FlushReadRecord(void)
	{
		if(s_chunkOpen)
		{
			if(s_chunkHeader.length)
			{
				// _WARNING("plugin didn't finish reading chunk");
				s_currentFile.Skip(s_chunkHeader.length);
			}

			s_chunkOpen = false;
		}
	}

	bool GetNextRecordInfo(UInt32 * type, UInt32 * version, UInt32 * length)
	{
		FlushReadRecord();

		if(!s_pluginHeader.numChunks)
			return false;

		s_pluginHeader.numChunks--;

		s_currentFile.ReadBuf(&s_chunkHeader, sizeof(s_chunkHeader));

		*type =		s_chunkHeader.type;
		*version =	s_chunkHeader.version;
		*length =	s_chunkHeader.length;

		s_chunkOpen = true;

		return true;
	}

	UInt32 ReadRecordData(void * buf, UInt32 length)
	{
		ASSERT(s_chunkOpen);

		if(length > s_chunkHeader.length)
			length = s_chunkHeader.length;

		s_currentFile.ReadBuf(buf, length);

		s_chunkHeader.length -= length;

		return length;
	}

	bool ResolveFormId(UInt32 formId, UInt32 * formIdOut)
	{
		UInt32	modID = formId >> 24;
		if (modID == 0xFF)
		{
			*formIdOut = formId;
			return true;
		}

		if (modID == 0xFE)
		{
			modID = formId >> 12;
		}

		UInt32	loadedModID = ResolveModIndex(modID);
		if (loadedModID < 0xFF)
		{
			*formIdOut = (formId & 0x00FFFFFF) | (((UInt32)loadedModID) << 24);
			return true;
		}
		else if (loadedModID > 0xFF)
		{
			*formIdOut = (loadedModID << 12) | (formId & 0x00000FFF);
			return true;
		}
		return false;
	}

	bool ResolveHandle(UInt64 handle, UInt64 * handleOut)
	{
		UInt32	modID = (handle & 0xFF000000) >> 24;
		if (modID == 0xFF)
		{
			*handleOut = handle;
			return true;
		}

		if (modID == 0xFE)
		{
			modID = (handle >> 12) & 0xFFFFF;
		}

		UInt64	loadedModID = (UInt64)ResolveModIndex(modID);
		if (loadedModID < 0xFF)
		{
			*handleOut = (handle & 0xFFFFFFFF00FFFFFF) | (((UInt64)loadedModID) << 24);
			return true;
		}
		else if (loadedModID > 0xFF)
		{
			*handleOut = (handle & 0xFFFFFFFF00000FFF) | (loadedModID << 12);
			return true;
		}
		return false;
	}

	// internal event handlers
	void HandleRevertGlobalData(void)
	{
		for(UInt32 i = 0; i < s_pluginCallbacks.size(); i++)
			if(s_pluginCallbacks[i].revert)
				s_pluginCallbacks[i].revert(&g_SKSESerializationInterface);
	}

	void HandleSaveGlobalData(void)
	{
		_MESSAGE("creating co-save");

		DeleteFile(s_savePath.c_str());

		if(!s_currentFile.Create(s_savePath.c_str()))
		{
			_ERROR("HandleSaveGlobalData: couldn't create save file (%s)", s_savePath.c_str());
			return;
		}

		try
		{
			// init header
			s_fileHeader.signature =		Header::kSignature;
			s_fileHeader.formatVersion =	Header::kVersion;
			s_fileHeader.skseVersion =		PACKED_SKSE_VERSION;
			s_fileHeader.runtimeVersion =	RUNTIME_VERSION;
			s_fileHeader.numPlugins =		0;

			s_currentFile.Skip(sizeof(s_fileHeader));

			// iterate through plugins
			for(UInt32 i = 0; i < s_pluginCallbacks.size(); i++)
			{
				PluginCallbacks	* info = &s_pluginCallbacks[i];

				if(info->save && info->hadUID)
				{
					// set up header info
					s_currentPlugin = i;

					s_pluginHeader.signature = info->uid;
					s_pluginHeader.numChunks = 0;
					s_pluginHeader.length = 0;

					s_chunkOpen = false;

					// call the plugin
					try
					{
						info->save(&g_SKSESerializationInterface);
					}
					catch( ... )
					{
						_ERROR("HandleSaveGlobalData: exception occurred saving %08X at %016I64X data may be corrupt.", s_pluginHeader.signature, s_currentFile.GetOffset());
					}

					// flush the remaining chunk data
					FlushWriteChunk();

					if(s_pluginHeader.numChunks)
					{
						UInt64	curOffset = s_currentFile.GetOffset();

						s_currentFile.SetOffset(s_pluginHeaderOffset);
						s_currentFile.WriteBuf(&s_pluginHeader, sizeof(s_pluginHeader));

						s_currentFile.SetOffset(curOffset);

						s_fileHeader.numPlugins++;
					}
				}
			}

			// write header
			s_currentFile.SetOffset(0);
			s_currentFile.WriteBuf(&s_fileHeader, sizeof(s_fileHeader));
		}
		catch(...)
		{
			_ERROR("HandleSaveGame: exception during save");
		}

		s_currentFile.Close();
	}

	void HandleLoadGlobalData(void)
	{
		_MESSAGE("loading co-save");

		if(!s_currentFile.Open(s_savePath.c_str()))
		{
			return;
		}

		try
		{
			Header	header;

			s_currentFile.ReadBuf(&header, sizeof(header));

			if(header.signature != Header::kSignature)
			{
				_ERROR("HandleLoadGame: invalid file signature (found %08X expected %08X)", header.signature, Header::kSignature);
				goto done;
			}

			if(header.formatVersion <= Header::kVersion_Invalid)
			{
				_ERROR("HandleLoadGame: version invalid (%08X)", header.formatVersion);
				goto done;
			}

			if(header.formatVersion > Header::kVersion)
			{
				_ERROR("HandleLoadGame: version too new (found %08X current %08X)", header.formatVersion, Header::kVersion);
				goto done;
			}

			// reset flags
			for(PluginCallbackList::iterator iter = s_pluginCallbacks.begin(); iter != s_pluginCallbacks.end(); ++iter)
				iter->hadData = false;
			
			// iterate through plugin data chunks
			while(s_currentFile.GetRemain() >= sizeof(PluginHeader))
			{
				s_currentFile.ReadBuf(&s_pluginHeader, sizeof(s_pluginHeader));

				UInt64	pluginChunkStart = s_currentFile.GetOffset();

				UInt32	pluginIdx = kPluginHandle_Invalid;

				for(PluginCallbackList::iterator iter = s_pluginCallbacks.begin(); iter != s_pluginCallbacks.end(); ++iter)
					if(iter->hadUID && (iter->uid == s_pluginHeader.signature))
						pluginIdx = iter - s_pluginCallbacks.begin();

				try
				{
					if(pluginIdx != kPluginHandle_Invalid)
					{
						PluginCallbacks	* info = &s_pluginCallbacks[pluginIdx];

						info->hadData = true;

						if(info->load)
						{
							s_chunkOpen = false;
							info->load(&g_SKSESerializationInterface);
						}
					}
					else
					{
						_WARNING("HandleLoadGame: plugin with signature %08X not loaded", s_pluginHeader.signature);
					}
				}
				catch( ... )
				{
					_ERROR("HandleLoadGame: exception occurred loading %08X", s_pluginHeader.signature);
				}

				// if plugin failed to read all its data or threw exception, jump to the next chunk
				UInt64	expectedOffset = pluginChunkStart + s_pluginHeader.length;
				if(s_currentFile.GetOffset() != expectedOffset)
				{
					_WARNING("HandleLoadGame: plugin did not read all of its data (at %016I64X expected %016I64X)", s_currentFile.GetOffset(), expectedOffset);
					s_currentFile.SetOffset(expectedOffset);
				}
			}

			// call load on plugins that had no data
			for(PluginCallbackList::iterator iter = s_pluginCallbacks.begin(); iter != s_pluginCallbacks.end(); ++iter) {
				if(!iter->hadData && iter->load) {
					iter->load(&g_SKSESerializationInterface);
				}
			}
		}
		catch(...)
		{
			_ERROR("HandleLoadGame: exception during load");

			// ### this could be handled better, individually catch around each plugin so one plugin can't mess things up for everyone else
		}

	done:
		s_currentFile.Close();
	}

	void HandleDeleteSave(std::string saveName)
	{
		std::string savePath = MakeSavePath(saveName, NULL);
		std::string coSavePath = savePath;
		savePath += ".ess";
		coSavePath += ".skse";

		// Old save file really gone?
		IFileStream	saveFile;
		if (!saveFile.Open(savePath.c_str()))
		{
			_MESSAGE("deleting co-save %s", coSavePath.c_str());	
			DeleteFile(coSavePath.c_str());
		}
		else
		{
			_MESSAGE("skipped delete of co-save %s", coSavePath.c_str());	
		}
	}

	void HandleDeletedForm(UInt64 handle)
	{
		for(UInt32 i = 0; i < s_pluginCallbacks.size(); i++)
			if(s_pluginCallbacks[i].formDelete)
				s_pluginCallbacks[i].formDelete(handle);
	}

	template <>
	bool WriteData<BSFixedString>(SKSESerializationInterface * intfc, const BSFixedString * str)
	{
		return WriteData<const char>(intfc, str->data);
	}

	template <>
	bool ReadData<BSFixedString>(SKSESerializationInterface * intfc, BSFixedString * str)
	{
		char buf[257] = { 0 };
		UInt16 len = 0;

		if (! intfc->ReadRecordData(&len, sizeof(len)))
			return false;

		if (len > 256)
			return false;

		if (! intfc->ReadRecordData(buf, len))
			return false;

		*str = BSFixedString(buf);
		return true;
	}

	template <>
	bool WriteData<std::string>(SKSESerializationInterface * intfc, const std::string * str)
	{
		UInt16 len = str->length();
		if (len > 256)
			return false;

		if (! intfc->WriteRecordData(&len, sizeof(len)))
			return false;
		if (! intfc->WriteRecordData(str->data(), len))
			return false;
		return true;
	}

	template <>
	bool ReadData<std::string>(SKSESerializationInterface * intfc, std::string * str)
	{
		char buf[257] = { 0 };
		UInt16 len = 0;

		if (! intfc->ReadRecordData(&len, sizeof(len)))
			return false;

		if (len > 256)
			return false;

		if (! intfc->ReadRecordData(buf, len))
			return false;

		*str = std::string(buf);
		return true;
	}

	template <>
	bool WriteData<const char>(SKSESerializationInterface * intfc, const char* str)
	{
		UInt16 len = strlen(str);
		if (len > 256)
			return false;

		if (! intfc->WriteRecordData(&len, sizeof(len)))
			return false;
		if (! intfc->WriteRecordData(str, len))
			return false;
		return true;
	}

	template <>
	bool WriteData<GFxValue>(SKSESerializationInterface* intfc, const GFxValue* val)
	{
		UInt32 type = val->GetType();
		if (! WriteData(intfc, &type))
			return false;

		switch (type)
		{
		case GFxValue::kType_Bool:
		{
			bool t = val->GetBool();
			return WriteData(intfc, &t);
		}
		case GFxValue::kType_Number:
		{
			double t = val->GetNumber();
			return WriteData(intfc, &t);
		}
		case GFxValue::kType_String:
		{
			const char* t = val->GetString();
			return WriteData(intfc, &t);
		}
		default:
			// Unsupported
			return false;
		}

		return false;
	}

	template <>
	bool ReadData<GFxValue>(SKSESerializationInterface* intfc, GFxValue* val)
	{
		UInt32 type;
		if (! ReadData(intfc, &type))
			return false;

		switch (type)
		{
		case GFxValue::kType_Bool:
		{
			bool t;
			if (! ReadData(intfc, &t))
				return false;
			val->SetBool(t);
			return true;
		}
		case GFxValue::kType_Number:
		{
			double t;
			if (! ReadData(intfc, &t))
				return false;
			val->SetNumber(t);
			return true;
		}
		case GFxValue::kType_String:
		{
			// As usual, using string cache to manage strings
			BSFixedString t;
			if (! ReadData(intfc, &t))
				return false;
			val->SetString(t.data);
			return true;
		}
		default:
			// Unsupported
			return false;
		}

		return false;
	}
}

SKSESerializationInterface	g_SKSESerializationInterface =
{
	SKSESerializationInterface::kVersion,

	Serialization::SetUniqueID,

	Serialization::SetRevertCallback,
	Serialization::SetSaveCallback,
	Serialization::SetLoadCallback,
	Serialization::SetFormDeleteCallback,

	Serialization::WriteRecord,
	Serialization::OpenRecord,
	Serialization::WriteRecordData,

	Serialization::GetNextRecordInfo,
	Serialization::ReadRecordData,
	Serialization::ResolveHandle,
	Serialization::ResolveFormId
};
