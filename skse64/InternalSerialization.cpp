#include <map>
#include <algorithm>
#include <string>
#include <functional>
#include <unordered_map>

#include "common/IFileStream.h"
#include "skse64/PluginAPI.h"
#include "skse64/InternalSerialization.h"
#include "skse64/GameData.h"
#include "skse64/Serialization.h"
#include "PapyrusEvents.h"
#include "PapyrusDelayFunctors.h"
#include "PapyrusObjects.h"

// Internal

static std::unordered_map<UInt32, UInt32> s_savedModIndexMap;

void LoadModList(SKSESerializationInterface * intfc)
{
	_MESSAGE("Loading mod list:");

	DataHandler * dhand = DataHandler::GetSingleton();

	char name[0x104] = { 0 };
	UInt16 nameLen = 0;

	UInt8 numSavedMods = 0;
	intfc->ReadRecordData(&numSavedMods, sizeof(numSavedMods));
	for (UInt32 i = 0; i < numSavedMods; i++)
	{
		intfc->ReadRecordData(&nameLen, sizeof(nameLen));
		intfc->ReadRecordData(&name, nameLen);
		name[nameLen] = 0;

		const ModInfo * modInfo = dhand->LookupModByName(name);
		if (modInfo) {
			UInt32 newIndex = modInfo->GetPartialIndex();
			s_savedModIndexMap[i] = newIndex;
			_MESSAGE("\t(%d -> %d)\t%s", i, newIndex, &name);
		}
		else {
			s_savedModIndexMap[i] = 0xFF;
		}
	}
}

void SavePluginsList(SKSESerializationInterface * intfc)
{
	DataHandler * dhand = DataHandler::GetSingleton();

	struct IsActiveFunctor
	{
		bool Accept(ModInfo * modInfo)
		{
			return modInfo && modInfo->IsActive();
		}
	};
	struct LoadedModVisitor
	{
		LoadedModVisitor(std::function<bool(ModInfo*)> func) : modInfoVisitor(func) { }
		bool Accept(ModInfo * modInfo)
		{
			return modInfoVisitor(modInfo);
		}
		std::function<bool(ModInfo*)> modInfoVisitor;
	};

	UInt16 modCount = dhand->modList.modInfoList.CountIf(IsActiveFunctor());

	intfc->OpenRecord('PLGN', 0);
	intfc->WriteRecordData(&modCount, sizeof(modCount));

	_MESSAGE("Saving plugin list:");


	dhand->modList.modInfoList.Visit(LoadedModVisitor([&](ModInfo* modInfo)
	{
		if (modInfo && modInfo->IsActive())
		{
			intfc->WriteRecordData(&modInfo->modIndex, sizeof(modInfo->modIndex));
			if (modInfo->modIndex == 0xFE) {
				intfc->WriteRecordData(&modInfo->lightIndex, sizeof(modInfo->lightIndex));
			}

			UInt16 nameLen = strlen(modInfo->name);
			intfc->WriteRecordData(&nameLen, sizeof(nameLen));
			intfc->WriteRecordData(modInfo->name, nameLen);
			if (modInfo->modIndex != 0xFE)
			{
				_MESSAGE("\t[%d]\t%s", modInfo->modIndex, &modInfo->name);
			}
			else
			{
				_MESSAGE("\t[FE:%d]\t%s", modInfo->lightIndex, &modInfo->name);
			}
		}
		return true; // Continue
	}));
}

void LoadPluginList(SKSESerializationInterface * intfc)
{
	DataHandler * dhand = DataHandler::GetSingleton();

	_MESSAGE("Loading plugin list:");

	char name[0x104] = { 0 };
	UInt16 nameLen = 0;

	UInt16 modCount = 0;
	intfc->ReadRecordData(&modCount, sizeof(modCount));
	for (UInt32 i = 0; i < modCount; i++)
	{
		UInt8 modIndex = 0xFF;
		UInt16 lightModIndex = 0xFFFF;
		intfc->ReadRecordData(&modIndex, sizeof(modIndex));
		if (modIndex == 0xFE) {
			intfc->ReadRecordData(&lightModIndex, sizeof(lightModIndex));
		}

		intfc->ReadRecordData(&nameLen, sizeof(nameLen));
		intfc->ReadRecordData(&name, nameLen);
		name[nameLen] = 0;

		UInt32 newIndex = 0xFF;
		UInt32 oldIndex = modIndex == 0xFE ? (0xFE000 | lightModIndex) : modIndex;

		const ModInfo * modInfo = dhand->LookupModByName(name);
		if (modInfo) {
			newIndex = modInfo->GetPartialIndex();
		}

		s_savedModIndexMap[oldIndex] = newIndex;

		_MESSAGE("\t(%d -> %d)\t%s", oldIndex, newIndex, name);
	}
}

UInt32 ResolveModIndex(UInt32 modIndex)
{
	auto it = s_savedModIndexMap.find(modIndex);
	if (it != s_savedModIndexMap.end())
	{
		return it->second;
	}

	return 0xFF;
}

enum LightModVersion
{
	kVersion1 = 1,
	kVersion2 = 2
};

void LoadLightModList(SKSESerializationInterface * intfc, UInt32 version)
{
	_MESSAGE("Loading light mod list:");

	DataHandler * dhand = DataHandler::GetSingleton();

	char name[0x104] = { 0 };
	UInt16 nameLen = 0;

	UInt16 numSavedMods = 0;
	if (version == kVersion1)
	{
		intfc->ReadRecordData(&numSavedMods, sizeof(UInt8));
	}
	else if (version == kVersion2)
	{
		intfc->ReadRecordData(&numSavedMods, sizeof(UInt16));
	}
	
	for (UInt32 i = 0; i < numSavedMods; i++)
	{
		intfc->ReadRecordData(&nameLen, sizeof(nameLen));
		intfc->ReadRecordData(&name, nameLen);
		name[nameLen] = 0;

		UInt32 lightIndex = 0xFE000 | i;

		const ModInfo * modInfo = dhand->LookupModByName(name);
		if (modInfo) {
			
			UInt32 newIndex = modInfo->GetPartialIndex();
			s_savedModIndexMap[lightIndex] = newIndex;
			_MESSAGE("\t(%d -> %d)\t%s", lightIndex, newIndex, &name);
		}
		else {
			s_savedModIndexMap[lightIndex] = 0xFF;
		}
		_MESSAGE("\t(%d -> %d)\t%s", lightIndex, s_savedModIndexMap[lightIndex], &name);
	}
}

//// Callbacks

void Core_RevertCallback(SKSESerializationInterface * intfc)
{
	s_savedModIndexMap.clear();
	g_menuOpenCloseRegs.Clear();
	g_inputKeyEventRegs.Clear();
	g_inputControlEventRegs.Clear();
	g_modCallbackRegs.Clear();
	g_crosshairRefEventRegs.Clear();
	g_cameraEventRegs.Clear();
	g_actionEventRegs.Clear();
	g_ninodeUpdateEventRegs.Clear();

	SKSEDelayFunctorManagerInstance().OnRevert();

	SKSEObjectStorageInstance().ClearAndRelease();
}

void Core_SaveCallback(SKSESerializationInterface * intfc)
{
	using Serialization::SaveClassHelper;

	SavePluginsList(intfc);

	_MESSAGE("Saving menu open/close event registrations...");
	g_menuOpenCloseRegs.Save(intfc, 'MENR', 1);

	_MESSAGE("Saving key input event registrations...");
	g_inputKeyEventRegs.Save(intfc, 'KEYR', 1);

	_MESSAGE("Saving control input event registrations...");
	g_inputControlEventRegs.Save(intfc, 'CTLR', 1);

	_MESSAGE("Saving mod callback event registrations...");
	g_modCallbackRegs.Save(intfc, 'MCBR', 1);

	_MESSAGE("Saving crosshair ref event registrations...");
	g_crosshairRefEventRegs.Save(intfc, 'CHRR', 1);

	_MESSAGE("Saving camera event registrations...");
	g_cameraEventRegs.Save(intfc, 'CAMR', 1);

	_MESSAGE("Saving actor action event registrations...");
	g_actionEventRegs.Save(intfc, 'AACT', 1);

	_MESSAGE("Saving NiNode update event registrations...");
	g_ninodeUpdateEventRegs.Save(intfc, 'NINU', 1);

	_MESSAGE("Saving SKSEPersistentObjectStorage data...");
	SaveClassHelper(intfc, 'OBMG', SKSEObjectStorageInstance());

	_MESSAGE("Saving SKSEDelayFunctorManager data...");
	SaveClassHelper(intfc, 'DFMG', SKSEDelayFunctorManagerInstance());
}

void Core_LoadCallback(SKSESerializationInterface * intfc)
{
	UInt32 type, version, length;

	while (intfc->GetNextRecordInfo(&type, &version, &length))
	{
		switch (type)
		{
		// Plugins list
		case 'PLGN':
			LoadPluginList(intfc);
			break;

		// Mod list - Deprecated
		case 'MODS':
			LoadModList(intfc);
			break;

		// Legacy Light Mod list - This only supported 255 entries
		case 'LMOD':
			LoadLightModList(intfc, kVersion1);
			break;

		// Light Mod list - Deprecated
		case 'LIMD':
			LoadLightModList(intfc, kVersion2);
			break;

		// Menu open/close events
		case 'MENR':
			_MESSAGE("Loading menu open/close event registrations...");
			g_menuOpenCloseRegs.Load(intfc, 1);
			break;

		// Key input events
		case 'KEYR':
			_MESSAGE("Loading key input event registrations...");
			g_inputKeyEventRegs.Load(intfc, 1);
			break;

		// Control input events
		case 'CTLR':
			_MESSAGE("Loading control input event registrations...");
			g_inputControlEventRegs.Load(intfc, 1);
			break;

		// Custom mod events
		case 'MCBR':
			_MESSAGE("Loading mod callback event registrations...");
			g_modCallbackRegs.Load(intfc, 1);
			break;

		// Crosshair rev events
		case 'CHRR':
			_MESSAGE("Loading crosshair ref event registrations...");
			g_crosshairRefEventRegs.Load(intfc, 1);
			break;

		// Camera events
		case 'CAMR':
			_MESSAGE("Loading camera event registrations...");
			g_cameraEventRegs.Load(intfc, 1);
			break;

		// Actor Actions events
		case 'AACT':
			_MESSAGE("Loading actor action event registrations...");
			g_actionEventRegs.Load(intfc, 1);
			break;

		// NiNode update events
		case 'NINU':
			_MESSAGE("Loading NiNode update event registrations...");
			g_ninodeUpdateEventRegs.Load(intfc, 1);
			break;

		// SKSEPersistentObjectStorage
		case 'OBMG':
			_MESSAGE("Loading SKSEPersistentObjectStorage data...");
			SKSEObjectStorageInstance().Load(intfc, version);
			break;

		// SKSEDelayFunctorManager
		case 'DFMG':
			_MESSAGE("Loading SKSEDelayFunctorManager data...");
			SKSEDelayFunctorManagerInstance().Load(intfc, version);
			break;

		default:
			_MESSAGE("Unhandled chunk type in Core_LoadCallback: %08X (%.4s)", type, &type);
			continue;
		}
	}
}

void Init_CoreSerialization_Callbacks()
{
	Serialization::SetUniqueID(0, 0);
	Serialization::SetRevertCallback(0, Core_RevertCallback);
	Serialization::SetSaveCallback(0, Core_SaveCallback);
	Serialization::SetLoadCallback(0, Core_LoadCallback);
}
