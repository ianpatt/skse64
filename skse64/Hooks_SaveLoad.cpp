#include "Hooks_SaveLoad.h"
#include "skse64_common/SafeWrite.h"
#include "skse64_common/Utilities.h"
#include "skse64_common/BranchTrampoline.h"
#include "Serialization.h"
#include "GlobalLocks.h"
#include "GameData.h"
#include "GameMenus.h"
#include "PapyrusVM.h"
#include "PluginManager.h"

void BGSSaveLoadManager::SaveGame_Hook(UInt64 *unk0)
{
	const char *saveName = reinterpret_cast<const char *>(unk0[0xBB0 / 8]);

	// Game actually does this, we may as well do the same
	if (!saveName)
		saveName = "";

#ifdef DEBUG
	_MESSAGE("Executing BGSSaveLoadManager::SaveGame_Hook. saveName: %s", saveName);
#endif

	Serialization::SetSaveName(saveName);
	PluginManager::Dispatch_Message(0, SKSEMessagingInterface::kMessage_SaveGame, (void*)saveName, strlen(saveName), NULL);	
	CALL_MEMBER_FN(this, SaveGame_HookTarget)(unk0);
	Serialization::SetSaveName(NULL);

#ifdef DEBUG
	_MESSAGE("Executed BGSSaveLoadManager::SaveGame_Hook.");
#endif
}

bool BGSSaveLoadManager::LoadGame_Hook(UInt64 *unk0, UInt32 unk1, UInt32 unk2, void *unk3)
{
	const char *saveName = reinterpret_cast<const char *>(unk0[0xBB0 / 8]); 

	// Game actually does this, we may as well do the same
	if (!saveName)
		saveName = ""; 

#ifdef DEBUG
	_MESSAGE("Executing BGSSaveLoadManager::LoadGame_Hook. saveName: %s", saveName);
#endif

	g_loadGameLock.Enter();

	Serialization::SetSaveName(saveName);
	PluginManager::Dispatch_Message(0, SKSEMessagingInterface::kMessage_PreLoadGame, (void*)saveName, strlen(saveName), NULL);
	bool result = CALL_MEMBER_FN(this, LoadGame_HookTarget)(unk0, unk1, unk2, unk3);
	PluginManager::Dispatch_Message(0, SKSEMessagingInterface::kMessage_PostLoadGame, (void*)result, 1, NULL);
	Serialization::SetSaveName(NULL);

	g_loadGameLock.Leave();

	// Clear invalid handles in OnUpdate event registration list
	UInt32	enableClearRegs = 0;
	if(GetConfigOption_UInt32("General", "ClearInvalidRegistrations", &enableClearRegs))
	{
		if(enableClearRegs)
		{
			UInt32 count = (*g_skyrimVM)->ClearInvalidRegistrations();
			if (count > 0)
				_MESSAGE("ClearInvalidRegistrations: Removed %d invalid OnUpdate registration(s)", count);
		}
	}

#ifdef DEBUG
	_MESSAGE("Executed BGSSaveLoadManager::LoadGame_Hook.");
#endif

	return result;
}

bool		s_requestedSave	= false;
bool		s_requestedLoad	= false;
std::string	s_reqSaveName;
std::string	s_reqLoadName;

void BGSSaveLoadManager::RequestSave(const char * name)
{
	s_requestedSave = true;
	s_reqSaveName = name;
}

void BGSSaveLoadManager::RequestLoad(const char * name)
{
	s_requestedLoad = true;
	s_reqLoadName = name;
}

void BGSSaveLoadManager::ProcessEvents_Hook(void)
{
	CALL_MEMBER_FN(this, ProcessEvents_Internal)();
	
	// wants both? gets nothing.
	if(s_requestedSave && s_requestedLoad)
		_MESSAGE("BGSSaveLoadManager: save and load requested in the same frame, ignoring both");
	else if(s_requestedSave)
		Save(s_reqSaveName.c_str());
	else if(s_requestedLoad)
		Load(s_reqLoadName.c_str());

	s_requestedSave = false;
	s_requestedLoad = false;
	s_reqSaveName.clear();
	s_reqLoadName.clear();
}

void BGSSaveLoadManager::DeleteSavegame_Hook(const char * saveNameIn, UInt32 unk1)

{
	std::string saveName = saveNameIn;

	PluginManager::Dispatch_Message(0, SKSEMessagingInterface::kMessage_DeleteGame, (void*)saveName.c_str(), strlen(saveName.c_str()), NULL);
	CALL_MEMBER_FN(this, DeleteSavegame)(saveNameIn, unk1);
	Serialization::HandleDeleteSave(saveName);
}

UInt8 TESQuest::NewGame_Hook(UInt8 * unk1, UInt8 unk2)
{
	UInt8 ret = CALL_MEMBER_FN(this, NewGame_Internal)(unk1, unk2);
	PluginManager::Dispatch_Message(0, SKSEMessagingInterface::kMessage_NewGame, (void*)this, sizeof(void*), NULL);
	return ret;
}

RelocAddr <uintptr_t> SaveGame_HookTarget_Enter(0x005A8960 + 0x2B3);
RelocAddr <uintptr_t> LoadGame_HookTarget_Enter(0x005AD550 + 0x2C5);
// 691ACD40B8430FBDA081477DEB2A9948ACC235F1+11F
RelocAddr <uintptr_t> ProcessEvents_Enter(0x005DAE80 + 0x9A);
RelocAddr <uintptr_t> NewGame_Enter(0x008D3450 + 0x59);
RelocAddr <uintptr_t> DeleteSaveGame_Enter(0x0059AC80 + 0x77);
RelocAddr <uintptr_t> DeleteSaveGame_Enter2(0x0059AD50 + 0x17);

void Hooks_SaveLoad_Commit(void)
{
	// Load & Save
	g_branchTrampoline.Write5Call(SaveGame_HookTarget_Enter, GetFnAddr(&BGSSaveLoadManager::SaveGame_Hook));
	g_branchTrampoline.Write5Call(LoadGame_HookTarget_Enter, GetFnAddr(&BGSSaveLoadManager::LoadGame_Hook));
	g_branchTrampoline.Write5Call(ProcessEvents_Enter, GetFnAddr(&BGSSaveLoadManager::ProcessEvents_Hook));

	// New Game
	g_branchTrampoline.Write5Call(NewGame_Enter, GetFnAddr(&TESQuest::NewGame_Hook));

	// Delete savegame
	g_branchTrampoline.Write5Call(DeleteSaveGame_Enter, GetFnAddr(&BGSSaveLoadManager::DeleteSavegame_Hook));
	g_branchTrampoline.Write5Call(DeleteSaveGame_Enter2, GetFnAddr(&BGSSaveLoadManager::DeleteSavegame_Hook));
}
