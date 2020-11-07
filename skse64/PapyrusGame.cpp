#include "PapyrusGame.h"
#include "PapyrusColorForm.h"

#include "GameRTTI.h"
#include "GameAPI.h"
#include "GameReferences.h"
#include "GameData.h"
#include "GameSettings.h"
#include "GameForms.h"
#include "GameCamera.h"
#include "GameMenus.h"
#include "GameThreads.h"
#include "GameExtraData.h"

#include "NiNodes.h"
#include "Colors.h"
#include "InternalTasks.h"

#include "Hooks_Gameplay.h"
#include "Hooks_UI.h"

namespace papyrusGame
{
	UInt32 GetPerkPoints(StaticFunctionTag*)
	{
		PlayerCharacter* pPC = (*g_thePlayer);
		return pPC->numPerkPoints;
	}

	void SetPerkPoints(StaticFunctionTag*, UInt32 points)
	{
		if(points > 255) // Not sure if signed or unsigned but this was only a byte in size
			points = 255;

		PlayerCharacter* pPC = (*g_thePlayer);
		pPC->numPerkPoints = points;
	}

	void ModPerkPoints(StaticFunctionTag*, SInt32 points)
	{
		PlayerCharacter* pPC = (*g_thePlayer);
		SInt32 nuPerkPoints = pPC->numPerkPoints + points;
		if (nuPerkPoints < 0)
			nuPerkPoints = 0;
		else if (nuPerkPoints > 255)
			nuPerkPoints = 255;

		pPC->numPerkPoints = nuPerkPoints;
	}

	void SetPlayerExperience(StaticFunctionTag*, float points)
	{
		PlayerCharacter* pPC = (*g_thePlayer);
		if(pPC->skills) {
			pPC->skills->data->levelPoints = points;
		}
	}

	void SetPlayerLevel(StaticFunctionTag*, SInt32 level)
	{
		PlayerCharacter* pc = *g_thePlayer;
		if (pc && pc->skills)
			CALL_MEMBER_FN(pc->skills, SetLevel)(level);
	}

	float GetPlayerExperience(StaticFunctionTag*)
	{
		PlayerCharacter* pPC = (*g_thePlayer);
		if(pPC->skills)
			return pPC->skills->data->levelPoints;

		return 0.0;
	}

	float GetExperienceForLevel(StaticFunctionTag*, UInt32 level)
	{
		// fXPLevelUpMult * level + fXPLevelUpBase
		double fXPLevelUpMult = 0.0;
		double fXPLevelUpBase = 0.0;
		SettingCollectionMap	* settings = *g_gameSettingCollection;
		if(settings)
		{
			Setting	* levelUpBase = settings->Get("fXPLevelUpBase");
			if(levelUpBase)
				levelUpBase->GetDouble(&fXPLevelUpBase);
			Setting	* levelUpMult = settings->Get("fXPLevelUpMult");
			if(levelUpMult)
				levelUpMult->GetDouble(&fXPLevelUpMult);
		}

		return fXPLevelUpBase + fXPLevelUpMult * level;
	}

	UInt32 GetModCount(StaticFunctionTag*)
	{
		DataHandler* pDataHandler = DataHandler::GetSingleton();
		return pDataHandler->modList.loadedMods.count;
	}

	static const int LIGHT_MOD_OFFSET = 0x100;

	UInt32 GetModByName(StaticFunctionTag*, BSFixedString name)
	{
		DataHandler* pDataHandler = DataHandler::GetSingleton();
		const ModInfo * modInfo = pDataHandler->LookupModByName(name.data);
		if (!modInfo || !modInfo->IsActive())
			return 0xFF;

		if (modInfo->IsLight())
			return modInfo->lightIndex + LIGHT_MOD_OFFSET;

		return modInfo->modIndex;
	}

	BSFixedString GetModName(StaticFunctionTag*, UInt32 index)
	{
		DataHandler* pDataHandler = DataHandler::GetSingleton();
		ModInfo* modInfo = nullptr;
		if (index > 0xFF) {
			UInt32 adjusted = index - LIGHT_MOD_OFFSET;
			if (adjusted >= pDataHandler->modList.loadedCCMods.count)
				return "";
			pDataHandler->modList.loadedCCMods.GetNthItem(adjusted, modInfo);
			
		} else {
			if (index >= pDataHandler->modList.loadedMods.count)
				return "";
			pDataHandler->modList.loadedMods.GetNthItem(index, modInfo);
		}

		return (modInfo) ? modInfo->name : "";
	}

	BSFixedString GetModAuthor(StaticFunctionTag*, UInt32 index)
	{
		DataHandler* pDataHandler = DataHandler::GetSingleton();
		ModInfo* modInfo = nullptr;
		if (index > 0xFF) {
			UInt32 adjusted = index - LIGHT_MOD_OFFSET;
			if (adjusted >= pDataHandler->modList.loadedCCMods.count)
				return "";
			pDataHandler->modList.loadedCCMods.GetNthItem(adjusted, modInfo);

		}
		else {
			if (index >= pDataHandler->modList.loadedMods.count)
				return "";
			pDataHandler->modList.loadedMods.GetNthItem(index, modInfo);
		}

		return (modInfo) ? modInfo->author.Get() : "";
	}

	BSFixedString GetModDescription(StaticFunctionTag*, UInt32 index)
	{
		DataHandler* pDataHandler = DataHandler::GetSingleton();
		ModInfo* modInfo = nullptr;
		if (index > 0xFF) {
			UInt32 adjusted = index - LIGHT_MOD_OFFSET;
			if (adjusted >= pDataHandler->modList.loadedCCMods.count)
				return "";
			pDataHandler->modList.loadedCCMods.GetNthItem(adjusted, modInfo);

		}
		else {
			if (index >= pDataHandler->modList.loadedMods.count)
				return "";
			pDataHandler->modList.loadedMods.GetNthItem(index, modInfo);
		}
		return (modInfo) ? modInfo->description.Get() : "";
	}

	UInt32 GetModDependencyCount(StaticFunctionTag*, UInt32 index)
	{
		DataHandler* pDataHandler = DataHandler::GetSingleton();
		ModInfo* modInfo = nullptr;
		if (index > 0xFF) {
			UInt32 adjusted = index - LIGHT_MOD_OFFSET;
			if (adjusted >= pDataHandler->modList.loadedCCMods.count)
				return 0;
			pDataHandler->modList.loadedCCMods.GetNthItem(adjusted, modInfo);

		}
		else {
			if (index >= pDataHandler->modList.loadedMods.count)
				return 0;
			pDataHandler->modList.loadedMods.GetNthItem(index, modInfo);
		}
		return (modInfo) ? modInfo->numRefMods : 0;
	}

	UInt32 GetNthModDependency(StaticFunctionTag*, UInt32 index, UInt32 dep_index)
	{
		DataHandler* pDataHandler = DataHandler::GetSingleton();
		ModInfo* modInfo = nullptr;
		if (index > 0xFF) {
			UInt32 adjusted = index - LIGHT_MOD_OFFSET;
			if (adjusted >= pDataHandler->modList.loadedCCMods.count)
				return 0;
			pDataHandler->modList.loadedCCMods.GetNthItem(adjusted, modInfo);

		}
		else {
			if (index >= pDataHandler->modList.loadedMods.count)
				return 0;
			pDataHandler->modList.loadedMods.GetNthItem(index, modInfo);
		}
		return (modInfo && dep_index < modInfo->numRefMods) ? modInfo->refModInfo[dep_index]->modIndex : 0;
	}

	UInt32 GetLightModCount(StaticFunctionTag*)
	{
		DataHandler* pDataHandler = DataHandler::GetSingleton();
		return pDataHandler->modList.loadedCCMods.count;
	}

	UInt32 GetLightModByName(StaticFunctionTag*, BSFixedString name)
	{
		DataHandler* pDataHandler = DataHandler::GetSingleton();
		const ModInfo * modInfo = pDataHandler->LookupModByName(name.data);
		if (!modInfo || !modInfo->IsActive() || !modInfo->IsLight())
			return 0xFFFF;

		return modInfo->lightIndex;
	}

	BSFixedString GetLightModName(StaticFunctionTag*, UInt32 index)
	{
		DataHandler* pDataHandler = DataHandler::GetSingleton();
		if (index >= pDataHandler->modList.loadedCCMods.count)
			return "";

		ModInfo* modInfo = nullptr;
		pDataHandler->modList.loadedCCMods.GetNthItem(index, modInfo);
		return (modInfo) ? modInfo->name : NULL;
	}

	BSFixedString GetLightModAuthor(StaticFunctionTag*, UInt32 index)
	{
		DataHandler* pDataHandler = DataHandler::GetSingleton();
		if (index >= pDataHandler->modList.loadedCCMods.count)
			return "";

		ModInfo* modInfo = NULL;
		pDataHandler->modList.loadedCCMods.GetNthItem(index, modInfo);
		return (modInfo) ? modInfo->author.Get() : NULL;
	}

	BSFixedString GetLightModDescription(StaticFunctionTag*, UInt32 index)
	{
		DataHandler* pDataHandler = DataHandler::GetSingleton();
		if (index >= pDataHandler->modList.loadedCCMods.count)
			return "";

		ModInfo* modInfo = NULL;
		pDataHandler->modList.loadedCCMods.GetNthItem(index, modInfo);
		return (modInfo) ? modInfo->description.Get() : NULL;
	}

	UInt32 GetLightModDependencyCount(StaticFunctionTag*, UInt32 index)
	{
		DataHandler* pDataHandler = DataHandler::GetSingleton();
		if (index >= pDataHandler->modList.loadedCCMods.count)
			return 0;

		ModInfo* modInfo = NULL;
		pDataHandler->modList.loadedCCMods.GetNthItem(index, modInfo);
		return (modInfo) ? modInfo->numRefMods : 0;
	}

	UInt32 GetNthLightModDependency(StaticFunctionTag*, UInt32 index, UInt32 dep_index)
	{
		DataHandler* pDataHandler = DataHandler::GetSingleton();
		if (index >= pDataHandler->modList.loadedCCMods.count)
			return 0;

		ModInfo* modInfo = NULL;
		pDataHandler->modList.loadedCCMods.GetNthItem(index, modInfo);
		return (modInfo && dep_index < modInfo->numRefMods) ? modInfo->refModInfo[dep_index]->modIndex : 0;
	}

	bool IsPluginInstalled(StaticFunctionTag*, BSFixedString name)
	{
		DataHandler* pDataHandler = DataHandler::GetSingleton();

		const ModInfo * modInfo = pDataHandler->LookupModByName(name.data);
		if (modInfo)
			return modInfo->IsActive();

		return false;
	}

	void SetGameSettingFloat(StaticFunctionTag * base, BSFixedString name, float value)
	{
		SettingCollectionMap	* settings = *g_gameSettingCollection;

		if(settings)
		{
			Setting	* setting = settings->Get(name.data);
			if(setting)
			{
				if(!setting->SetDouble(value))
				{
					_WARNING("SetGameSettingFloat: %s is not a float", name.data);
				}
			}
			else
			{
				_WARNING("SetGameSettingFloat: %s not found", name.data);
			}
		}
	}

	void SetGameSettingInt(StaticFunctionTag * base, BSFixedString name, UInt32 value)
	{
		SettingCollectionMap	* settings = *g_gameSettingCollection;

		if(settings)
		{
			Setting	* setting = settings->Get(name.data);
			if(setting)
			{
				if(setting->GetType() == Setting::kType_Integer)
				{
					setting->data.u32 = value;
				}
				else
				{
					_WARNING("SetGameSettingInt: %s is not an int", name.data);
				}	
			}
			else
			{
				_WARNING("SetGameSettingInt: %s not found", name.data);
			}
		}
	}

	void SetGameSettingBool(StaticFunctionTag * base, BSFixedString name, bool value)
	{
		SettingCollectionMap	* settings = *g_gameSettingCollection;

		if(settings)
		{
			Setting	* setting = settings->Get(name.data);
			if(setting)
			{
				if(setting->GetType() == Setting::kType_Bool)
				{
					setting->data.u8 = value;
				}
				else
				{
					_WARNING("SetGameSettingBool: %s is not a bool", name.data);
				}
			}
			else
			{
				_WARNING("SetGameSettingBool: %s not found", name.data);
			}
		}
	}

	void SetGameSettingString(StaticFunctionTag * base, BSFixedString name, BSFixedString value)
	{
		SettingCollectionMap	* settings = *g_gameSettingCollection;

		if(settings)
		{
			Setting	* setting = settings->Get(name.data);
			if(setting)
			{
				if(!setting->SetString(value.data))
				{
					_WARNING("SetGameSettingString: %s is not a string", name.data);
				}
			}
			else
			{
				_WARNING("SetGameSettingString: %s not found", name.data);
			}
		}
	}

	void SaveGame(StaticFunctionTag * base, BSFixedString name)
	{
		BGSSaveLoadManager	* mgr = BGSSaveLoadManager::GetSingleton();
		if(!mgr)
			return;

		mgr->RequestSave(name.data);
	}

	void LoadGame(StaticFunctionTag * base, BSFixedString name)
	{
		BGSSaveLoadManager	* mgr = BGSSaveLoadManager::GetSingleton();
		if(!mgr)
			return;

		mgr->RequestLoad(name.data);
	}

	UInt32 GetNumTintMasks(StaticFunctionTag * base)
	{
		PlayerCharacter* pPC = (*g_thePlayer);
		if(!pPC) {
			return 0;
		}
		return pPC->tintMasks.count;
	}

	UInt32 GetNthTintMaskColor(StaticFunctionTag * base, UInt32 n)
	{
		PlayerCharacter* pPC = (*g_thePlayer);
		if(!pPC) {
			return 0;
		}
		TintMask * tintMask = NULL;
		if(pPC->tintMasks.GetNthItem(n, tintMask)) {
			return tintMask->ToARGB();
		}
		return 0;
	}

	UInt32 GetNthTintMaskType(StaticFunctionTag * base, UInt32 n)
	{
		PlayerCharacter* pPC = (*g_thePlayer);
		if(!pPC) {
			return 0;
		}
		TintMask * tintMask = NULL;
		if(pPC->tintMasks.GetNthItem(n, tintMask)) {
			return tintMask->tintType;
		}
		return 0;
	}

	BSFixedString GetNthTintMaskTexturePath(StaticFunctionTag * base, UInt32 n)
	{
		PlayerCharacter* pPC = (*g_thePlayer);
		if(!pPC) {
			return NULL;
		}
		TintMask * tintMask = NULL;
		if(pPC->tintMasks.GetNthItem(n, tintMask)) {
			if(tintMask->texture) {
				return tintMask->texture->str;
			}
		}
	
		return NULL;
	}

	void SetNthTintMaskTexturePath(StaticFunctionTag * base, BSFixedString path, UInt32 n)
	{
		PlayerCharacter* pPC = (*g_thePlayer);
		if(pPC) {
			TintMask * tintMask = NULL;
			if(pPC->tintMasks.GetNthItem(n, tintMask)) {
				if(tintMask->texture) {
					tintMask->texture->str = path;
				}
			}
			if(pPC->overlayTintMasks) {
				if(pPC->overlayTintMasks->GetNthItem(n, tintMask)) {
					if(tintMask->texture) {
						tintMask->texture->str = path;
					}
				}
			}
		}
	}

	void SetPlayerTintMaskColor(TESNPC* actorBase, TintMask * tintMask, UInt32 argb)
	{
		ARGBColor color(argb);
		float alpha = color.GetAlpha() / 255.0;
		if(alpha > 1.0)
			alpha = 1.0;
		if(alpha < 0.0)
			alpha = 0.0;

		tintMask->color.red = color.GetRed();
		tintMask->color.green = color.GetGreen();
		tintMask->color.blue = color.GetBlue();
		tintMask->alpha = alpha;

		// Compute skintone
		if(tintMask->tintType == TintMask::kMaskType_SkinTone) {
			NiColorA colorResult;
			CALL_MEMBER_FN(actorBase, SetSkinFromTint)(&colorResult, tintMask, true);
		}
	}

	void SetNthTintMaskColor(StaticFunctionTag * base, UInt32 n, UInt32 color)
	{
		PlayerCharacter* pPC = (*g_thePlayer);
		if(pPC) {
			TESNPC* npc = DYNAMIC_CAST(pPC->baseForm, TESForm, TESNPC);
			if(npc) {
				TintMask * tintMask = NULL;
				if(pPC->tintMasks.GetNthItem(n, tintMask)) {
					SetPlayerTintMaskColor(npc, tintMask, color);
				}
				if(pPC->overlayTintMasks) {
					if(pPC->overlayTintMasks->GetNthItem(n, tintMask)) {
						SetPlayerTintMaskColor(npc, tintMask, color);
					}
				}
			}
		}
	}

	UInt32 GetNumTintMasksByType(StaticFunctionTag * base, UInt32 tintType)
	{
		PlayerCharacter* pPC = (*g_thePlayer);
		if(!pPC) {
			return 0;
		}
		return CALL_MEMBER_FN(pPC, GetNumTints)(tintType);
	}

	UInt32 GetTintMaskColor(StaticFunctionTag * base, UInt32 tintType, UInt32 index)
	{
		PlayerCharacter* pPC = (*g_thePlayer);
		if(!pPC) {
			return 0;
		}

		UInt32 color = 0;
		TintMask * tintMask = CALL_MEMBER_FN(pPC, GetTintMask)(tintType, index);
		if(tintMask) {
			color = tintMask->ToARGB();
		}


		return color;
	}

	void SetTintMaskColor(StaticFunctionTag * base, UInt32 color, UInt32 tintType, UInt32 index)
	{
		PlayerCharacter* pPC = (*g_thePlayer);
		if(pPC) {
			TESNPC* npc = DYNAMIC_CAST(pPC->baseForm, TESForm, TESNPC);
			if(npc) {
				TintMask * tintMask = CALL_MEMBER_FN(pPC, GetTintMask)(tintType, index);
				if(tintMask) {
					SetPlayerTintMaskColor(npc, tintMask, color);
				}
				if(pPC->overlayTintMasks) { // Overlays don't always have types for some dumb reason
					TintMask * overlayMask = pPC->GetOverlayTintMask(tintMask);
					if(overlayMask) {
						SetPlayerTintMaskColor(npc, overlayMask, color);
					}
				}
			}
		}
	}

	BSFixedString GetTintMaskTexturePath(StaticFunctionTag * base, UInt32 tintType, UInt32 index)
	{
		PlayerCharacter* pPC = (*g_thePlayer);
		if(!pPC) {
			return NULL;
		}
		TintMask * tintMask = CALL_MEMBER_FN(pPC, GetTintMask)(tintType, index);
		if(tintMask && tintMask->texture) {
			return tintMask->texture->str;
		}

		return NULL;
	}

	void SetTintMaskTexturePath(StaticFunctionTag * base, BSFixedString path, UInt32 tintType, UInt32 index)
	{
		PlayerCharacter* pPC = (*g_thePlayer);
		if(pPC) {
			TintMask * tintMask = CALL_MEMBER_FN(pPC, GetTintMask)(tintType, index);
			if(tintMask && tintMask->texture) {
				tintMask->texture->str = path;
			}
			if(pPC->overlayTintMasks) {
				TintMask * overlayMask = pPC->GetOverlayTintMask(tintMask);
				if(overlayMask && overlayMask->texture) {
					overlayMask->texture->str = path;
				}
			}
		}
	}

	void UpdateTintMaskColors(StaticFunctionTag * base)
	{
		TaskInterface::UpdateTintMasks();
	}

	void UpdateHairColor(StaticFunctionTag * base)
	{
		TaskInterface::UpdateHairColor();
	}

	SInt32 GetCameraState(StaticFunctionTag * base)
	{
		PlayerCamera * playerCamera = PlayerCamera::GetSingleton();
		if (playerCamera) {
			for (int i = 0; i < PlayerCamera::kNumCameraStates; i++) {
				if (playerCamera->cameraState == playerCamera->cameraStates[i])
					return i;
			}
		}

		return -1;
	}

	void SetMiscStat(StaticFunctionTag * base, BSFixedString name, UInt32 value)
	{
		MiscStatManager::MiscStat	* stat = MiscStatManager::GetSingleton()->Get(name.data);

		if(stat)
			stat->value = value;
		else
			_MESSAGE("SetMiscStat: could not find stat (%s)", name.data);
	}

	void SetPlayersLastRiddenHorse(StaticFunctionTag * base, Actor* actor)
	{
		PlayerCharacter* pPC = (*g_thePlayer);
		if(pPC) {
			if(!actor) {
				pPC->lastRiddenHorseHandle = 0;
			} else {
				pPC->lastRiddenHorseHandle = actor->CreateRefHandle();
			}
		}
	}

	bool GetPlayerMovementMode(StaticFunctionTag * base)
	{
		PlayerControls * controls = PlayerControls::GetSingleton();
		if(controls) {
			return controls->runMode == 1;
		}

		return false;
	}

	void UpdateThirdPerson(StaticFunctionTag * base)
	{
		PlayerCharacter* pPC = (*g_thePlayer);
		PlayerCamera * camera = PlayerCamera::GetSingleton();
		if (pPC && camera) {
			CALL_MEMBER_FN(camera, UpdateThirdPerson)(pPC->actorState.IsWeaponDrawn());
		}
	}

	void UnbindObjectHotkey(StaticFunctionTag * base, SInt32 hotkey)
	{
		MagicFavorites * magicFavorites = MagicFavorites::GetSingleton();
		if(magicFavorites) {
			magicFavorites->ClearHotkey(hotkey);
		}

		PlayerCharacter* pPC = (*g_thePlayer);
		if(pPC && hotkey >= 0) {
			ExtraContainerChanges* pContainerChanges = static_cast<ExtraContainerChanges*>(pPC->extraData.GetByType(kExtraData_ContainerChanges));
			if (pContainerChanges) {
				HotkeyData data = pContainerChanges->FindHotkey(hotkey);
				if(data.pHotkey)
					data.pHotkey->hotkey = -1;
			}
		}		
	}

	TESForm * GetHotkeyBoundObject(StaticFunctionTag * base, SInt32 hotkey)
	{
		PlayerCharacter* pPC = (*g_thePlayer);
		if(!pPC || hotkey < 0)
			return NULL;

		MagicFavorites * magicFavorites = MagicFavorites::GetSingleton();
		if(magicFavorites) {
			TESForm * form = magicFavorites->GetSpell(hotkey);
			if(form)
				return form;
		}

		ExtraContainerChanges* pContainerChanges = static_cast<ExtraContainerChanges*>(pPC->extraData.GetByType(kExtraData_ContainerChanges));
		if (pContainerChanges) {
			HotkeyData data = pContainerChanges->FindHotkey(hotkey);
			if(data.pForm)
				return data.pForm;
		}

		return NULL;
	}

	bool IsObjectFavorited(StaticFunctionTag * base, TESForm * form)
	{
		PlayerCharacter* player = (*g_thePlayer);
		if (!player || !form)
			return false;

		UInt8 formType = form->formType;

		// Spell or shout - check MagicFavorites
		if (formType == kFormType_Spell || formType == kFormType_Shout)
		{
			MagicFavorites * magicFavorites = MagicFavorites::GetSingleton();

			return magicFavorites && magicFavorites->IsFavorited(form);
		}
		// Other - check ExtraHotkey. Any hotkey data (including -1) means favorited
		else
		{
			bool result = false;
			
			ExtraContainerChanges* pContainerChanges = static_cast<ExtraContainerChanges*>(player->extraData.GetByType(kExtraData_ContainerChanges));
			if (pContainerChanges) {
				HotkeyData data = pContainerChanges->FindHotkey(form);
				if (data.pHotkey)
					result = true;
			}

			return result;
		}
	}

	TESForm* GetFormEx(StaticFunctionTag * base, UInt32 formId)
	{
		return LookupFormByID(formId);
	}

	TESObjectREFR * GetDialogueTarget(StaticFunctionTag * base)
	{
		MenuTopicManager * topicManager = MenuTopicManager::GetSingleton();
		return topicManager ? topicManager->GetDialogueTarget() : NULL;
	}

	TESObjectREFR * GetCurrentCrosshairRef(StaticFunctionTag * base)
	{
		return Hooks_Gameplay_GetCrosshairRef();
	}

	TESObjectREFR * GetCurrentConsoleRef(StaticFunctionTag * base)
	{
		UInt32 handle = (*g_consoleHandle);
		NiPointer<TESObjectREFR> refr;
		if(handle != 0 && handle != (*g_invalidRefHandle)) {
			LookupREFRByHandle(handle, refr);
			return refr;
		}

		return NULL;
	}
}

#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

void papyrusGame::RegisterFuncs(VMClassRegistry* registry)
{
	// Perk Points
	registry->RegisterFunction(
		new NativeFunction0 <StaticFunctionTag, UInt32>("GetPerkPoints", "Game", papyrusGame::GetPerkPoints, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, void, UInt32>("SetPerkPoints", "Game", papyrusGame::SetPerkPoints, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, void, SInt32>("ModPerkPoints", "Game", papyrusGame::ModPerkPoints, registry));


	registry->RegisterFunction(
		new NativeFunction0 <StaticFunctionTag, float>("GetPlayerExperience", "Game", papyrusGame::GetPlayerExperience, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, void, float>("SetPlayerExperience", "Game", papyrusGame::SetPlayerExperience, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, float, UInt32>("GetExperienceForLevel", "Game", papyrusGame::GetExperienceForLevel, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, void, SInt32>("SetPlayerLevel", "Game", papyrusGame::SetPlayerLevel, registry));


	// Mods
	registry->RegisterFunction(
		new NativeFunction0 <StaticFunctionTag, UInt32>("GetModCount", "Game", papyrusGame::GetModCount, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, UInt32, BSFixedString>("GetModByName", "Game", papyrusGame::GetModByName, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, BSFixedString, UInt32>("GetModName", "Game", papyrusGame::GetModName, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, BSFixedString, UInt32>("GetModAuthor", "Game", papyrusGame::GetModAuthor, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, BSFixedString, UInt32>("GetModDescription", "Game", papyrusGame::GetModDescription, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, UInt32, UInt32>("GetModDependencyCount", "Game", papyrusGame::GetModDependencyCount, registry));

	// Light Mods
	registry->RegisterFunction(
		new NativeFunction0 <StaticFunctionTag, UInt32>("GetLightModCount", "Game", papyrusGame::GetLightModCount, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, UInt32, BSFixedString>("GetLightModByName", "Game", papyrusGame::GetLightModByName, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, BSFixedString, UInt32>("GetLightModName", "Game", papyrusGame::GetLightModName, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, BSFixedString, UInt32>("GetLightModAuthor", "Game", papyrusGame::GetLightModAuthor, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, BSFixedString, UInt32>("GetLightModDescription", "Game", papyrusGame::GetLightModDescription, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, UInt32, UInt32>("GetLightModDependencyCount", "Game", papyrusGame::GetLightModDependencyCount, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, bool, BSFixedString>("IsPluginInstalled", "Game", papyrusGame::IsPluginInstalled, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, UInt32, UInt32, UInt32>("GetNthLightModDependency", "Game", papyrusGame::GetNthLightModDependency, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, void, BSFixedString, float>("SetGameSettingFloat", "Game", papyrusGame::SetGameSettingFloat, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, void, BSFixedString, UInt32>("SetGameSettingInt", "Game", papyrusGame::SetGameSettingInt, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, void, BSFixedString, bool>("SetGameSettingBool", "Game", papyrusGame::SetGameSettingBool, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, void, BSFixedString, BSFixedString>("SetGameSettingString", "Game", papyrusGame::SetGameSettingString, registry));

	// Save/Load
	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, void, BSFixedString>("SaveGame", "Game", papyrusGame::SaveGame, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, void, BSFixedString>("LoadGame", "Game", papyrusGame::LoadGame, registry));

	// Tintmasks
	registry->RegisterFunction(
		new NativeFunction0 <StaticFunctionTag, UInt32>("GetNumTintMasks", "Game", papyrusGame::GetNumTintMasks, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, UInt32, UInt32>("GetNthTintMaskColor", "Game", papyrusGame::GetNthTintMaskColor, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, void, UInt32, UInt32>("SetNthTintMaskColor", "Game", papyrusGame::SetNthTintMaskColor, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, UInt32, UInt32>("GetNthTintMaskType", "Game", papyrusGame::GetNthTintMaskType, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, BSFixedString, UInt32>("GetNthTintMaskTexturePath", "Game", papyrusGame::GetNthTintMaskTexturePath, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, void, BSFixedString, UInt32>("SetNthTintMaskTexturePath", "Game", papyrusGame::SetNthTintMaskTexturePath, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, UInt32, UInt32>("GetNumTintsByType", "Game", papyrusGame::GetNumTintMasksByType, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, UInt32, UInt32, UInt32>("GetTintMaskColor", "Game", papyrusGame::GetTintMaskColor, registry));

	registry->RegisterFunction(
		new NativeFunction3 <StaticFunctionTag, void, UInt32, UInt32, UInt32>("SetTintMaskColor", "Game", papyrusGame::SetTintMaskColor, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, BSFixedString, UInt32, UInt32>("GetTintMaskTexturePath", "Game", papyrusGame::GetTintMaskTexturePath, registry));

	registry->RegisterFunction(
		new NativeFunction3 <StaticFunctionTag, void, BSFixedString, UInt32, UInt32>("SetTintMaskTexturePath", "Game", papyrusGame::SetTintMaskTexturePath, registry));

	// Misc
	registry->RegisterFunction(
		new NativeFunction0 <StaticFunctionTag, SInt32>("GetCameraState", "Game", papyrusGame::GetCameraState, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, void, BSFixedString, UInt32>("SetMiscStat", "Game", papyrusGame::SetMiscStat, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, void, Actor*>("SetPlayersLastRiddenHorse", "Game", papyrusGame::SetPlayersLastRiddenHorse, registry));

	registry->RegisterFunction(
		new NativeFunction0 <StaticFunctionTag, bool>("GetPlayerMovementMode", "Game", papyrusGame::GetPlayerMovementMode, registry));

	registry->RegisterFunction(
		new NativeFunction0 <StaticFunctionTag, void>("UpdateThirdPerson", "Game", papyrusGame::UpdateThirdPerson, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, TESForm*, UInt32>("GetFormEx", "Game", papyrusGame::GetFormEx, registry));

	registry->RegisterFunction(
		new NativeFunction0 <StaticFunctionTag, void>("UpdateTintMaskColors", "Game", papyrusGame::UpdateTintMaskColors, registry));

	registry->RegisterFunction(
		new NativeFunction0 <StaticFunctionTag, void>("UpdateHairColor", "Game", papyrusGame::UpdateHairColor, registry));

	// Hotkeys
	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, void, SInt32>("UnbindObjectHotkey", "Game", papyrusGame::UnbindObjectHotkey, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, TESForm*, SInt32>("GetHotkeyBoundObject", "Game", papyrusGame::GetHotkeyBoundObject, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, bool, TESForm*>("IsObjectFavorited", "Game", papyrusGame::IsObjectFavorited, registry));

	// Dialogue
	registry->RegisterFunction(
		new NativeFunction0 <StaticFunctionTag, TESObjectREFR*>("GetDialogueTarget", "Game", papyrusGame::GetDialogueTarget, registry));

	// Crosshair ref
	registry->RegisterFunction(
		new NativeFunction0 <StaticFunctionTag, TESObjectREFR*>("GetCurrentCrosshairRef", "Game", papyrusGame::GetCurrentCrosshairRef, registry));

	// Console ref
	registry->RegisterFunction(
		new NativeFunction0 <StaticFunctionTag, TESObjectREFR*>("GetCurrentConsoleRef", "Game", papyrusGame::GetCurrentConsoleRef, registry));

	// Mod
	registry->SetFunctionFlags("Game", "GetModCount", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Game", "GetModByName", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Game", "GetModName", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Game", "GetModAuthor", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Game", "GetModDescription", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Game", "GetModDependencyCount", VMClassRegistry::kFunctionFlag_NoWait);
	//registry->SetFunctionFlags("Game", "GetNthModDependency", VMClassRegistry::kFunctionFlag_NoWait);

	// GameSettings
	registry->SetFunctionFlags("Game", "SetGameSettingFloat", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Game", "SetGameSettingInt", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Game", "SetGameSettingBool", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Game", "SetGameSettingString", VMClassRegistry::kFunctionFlag_NoWait);

	// Save/Load
	registry->SetFunctionFlags("Game", "SaveGame", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Game", "LoadGame", VMClassRegistry::kFunctionFlag_NoWait);

	// Tintmasks
	registry->SetFunctionFlags("Game", "GetNumTintMasks", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Game", "GetNthTintMaskColor", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Game", "SetNthTintMaskColor", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Game", "GetNthTintMaskType", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Game", "GetNthTintMaskTexturePath", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Game", "SetNthTintMaskTexturePath", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Game", "GetNumTintsByType", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Game", "GetTintMaskColor", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Game", "SetTintMaskColor", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Game", "GetTintMaskTexturePath", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Game", "SetTintMaskTexturePath", VMClassRegistry::kFunctionFlag_NoWait);

	// Misc
	registry->SetFunctionFlags("Game", "SetMiscStat", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Game", "GetCameraState", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Game", "SetPlayersLastRiddenHorse", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Game", "GetFormEx", VMClassRegistry::kFunctionFlag_NoWait);

	registry->SetFunctionFlags("Game", "GetPlayerExperience", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Game", "SetPlayerExperience", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Game", "GetExperienceForLevel", VMClassRegistry::kFunctionFlag_NoWait);

	registry->SetFunctionFlags("Game", "GetDialogueTarget", VMClassRegistry::kFunctionFlag_NoWait);

	//registry->SetFunctionFlags("Game", "UpdateTintMaskColors", VMClassRegistry::kFunctionFlag_NoWait);
	//registry->SetFunctionFlags("Game", "UpdateHairColor", VMClassRegistry::kFunctionFlag_NoWait);
}
