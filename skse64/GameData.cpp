#include "skse64/GameData.h"

// 9615953AF64D4A2EB76F7BDE0F38D30B311B1351+40
RelocAddr <UInt32*> g_gameTime(0x03275610);

// B8860AA646A4AA64DB8D5C6EDA306FC6B59F1B9B+5B
RelocPtr <DataHandler *> g_dataHandler(0x0219DED8);

// 5F417CF4D2EB33C7D6903EA38BB5CDDEF48A6F83+4B
RelocPtr <PersistentFormManager *> g_persistentFormManager(0x0219E588);

// EC410889481EE7B9DC4B6D7A707FA41B72CDCE48+4
RelocPtr <FaceGen *> g_faceGen(0x0219E5E0);

// F22C9BC54C962ED86B36585C0EC0E729DCA18000+26
RelocPtr<MagicFavorites *>	g_MagicFavorites(0x03223E50);

// 6C8AEF51A6996BA6DC317E92014FCC446B5DBC17+25
RelocPtr<MenuTopicManager *> g_MenuTopicManager(0x031DF408);

// 5ED03BBC1845C7394493F8FAA1DE8C7AEB3E2350+2A
RelocPtr <ActorValueList *> g_actorValueList(0x0219DEC8);

// aWerewolfSpell
RelocPtr <DefaultObjectList> g_defaultObjectList(0x0207BF40);

// 68BF756333EF751059E935236F381FA91F62C373+22
RelocPtr <BGSSaveLoadManager *> g_saveLoadManager(0x03223E60);

// 3B74AA860AC42F1080FAF792E4BE1D3BF0E0EFEE+47
RelocPtr <MiscStatManager>	g_MiscStatManager(0x031A4F98);

// A9D0A72CC9E5F85E2169118F999943FD43AF51EA+95
RelocPtr <EquipManager *> g_equipManager(0x031A42F8);

// FBC6C0FB3A858D20081F5FF9CB3E780E59DCE52F+50
RelocPtr <RelationshipRanks> g_relationshipRanks(0x0209A548);

RelocAddr<_ChangeActorHeadPart> ChangeActorHeadPart(0x0043D6E0);
// 
RelocAddr<_GetEitherHandSlot> GetEitherHandSlot(0x0038FA50);
// 
RelocAddr<_GetRightHandSlot> GetRightHandSlot(0x0038FA40);
// 
RelocAddr<_GetLeftHandSlot> GetLeftHandSlot(0x0038FA30);
RelocAddr<_LookupActorValueByName> LookupActorValueByName(0x00443320);
RelocAddr<_UpdatePlayerTints> UpdatePlayerTints(0x0096BE70);
RelocAddr<_GetActorBaseOverlays> GetActorBaseOverlays(0x003C8D00);
RelocAddr<_GetNumActorBaseOverlays> GetNumActorBaseOverlays(0x003C8D90);

RelocAddr<_ApplyMasksToRenderTarget> ApplyMasksToRenderTarget(0x0043C530);

RelocAddr<_UpdateModelSkin> UpdateModelSkin(0x0043DA10); // Applies tint to ShaderType 5 nodes
RelocAddr<_UpdateModelHair> UpdateModelHair(0x0043DB20); // Applies tint to ShaderType 6 nodes
RelocAddr<_UpdateModelFace> UpdateModelFace(0x0043D080);
RelocAddr<_UpdateHarvestModel> UpdateHarvestModel(0x001EEB10);

RelocAddr<_GetRelationshipIndex> GetRelationshipIndex(0x003A4E00);

// 63503D87B68DB4A4C8A2BE534A75246035A76798+82
RelocPtr<FacePresetList> g_facePresetList(0x0209C860);	// aNosetype

RelocAddr<_HasLOS> HasLOS(0x009D19F0);

class LoadedModFinder
{
	const char * m_stringToFind;

public:
	LoadedModFinder(const char * str) : m_stringToFind(str) { }

	bool Accept(ModInfo* modInfo)
	{
		return _stricmp(modInfo->name, m_stringToFind) == 0;
	}
};

const ModInfo * DataHandler::LookupModByName(const char * modName)
{
	return modList.modInfoList.Find(LoadedModFinder(modName));
}

DataHandler* DataHandler::GetSingleton()
{
	return *(g_dataHandler.GetPtr());
}

MiscStatManager * MiscStatManager::GetSingleton(void)
{
	return g_MiscStatManager.GetPtr(); 
}

MiscStatManager::MiscStat * MiscStatManager::Get(const char * name)
{
	for (UInt32 i = 0; i < m_stats.count; i++)
	{
		MiscStat	* stat = &m_stats[i];

		if (!_stricmp(name, stat->name))
		{
			return stat;
		}
	}

	return NULL;
}

EquipManager * EquipManager::GetSingleton(void)
{
	return *g_equipManager;
}

PersistentFormManager * PersistentFormManager::GetSingleton(void)
{
	return *g_persistentFormManager;
}

FaceGen * FaceGen::GetSingleton(void)
{
	return *g_faceGen;
}

MagicFavorites * MagicFavorites::GetSingleton()
{
	return *(g_MagicFavorites.GetPtr());
}

void MagicFavorites::ClearHotkey(SInt8 idx)
{
	if (idx < 0 || idx >= hotkeys.count)
		return;

	hotkeys[idx] = NULL;
}

void MagicFavorites::SetHotkey(TESForm * form, SInt8 idx)
{
	if (idx < 0 || idx >= hotkeys.count)
		return;

	SInt8 oldIdx = hotkeys.GetItemIndex(form);
	if (idx == oldIdx)
		return;

	if (IsFavorited(form))
	{
		hotkeys[oldIdx] = NULL;
		hotkeys[idx] = form;
	}
}

TESForm * MagicFavorites::GetSpell(SInt8 idx)
{
	TESForm * form = NULL;
	hotkeys.GetNthItem(idx, form);
	return form;
}

bool MagicFavorites::IsFavorited(TESForm * form)
{
	SInt32 indexOut = -1;
	if (GetSortIndex(spells, form, indexOut) && indexOut != -1)
		return true;
	else
		return false;
}

MenuTopicManager * MenuTopicManager::GetSingleton(void)
{
	return *(g_MenuTopicManager.GetPtr());
}

NiPointer<TESObjectREFR> MenuTopicManager::GetDialogueTarget()
{
	
	NiPointer<TESObjectREFR> refr;
	if (talkingHandle == (*g_invalidRefHandle) || talkingHandle == 0)
		return NULL;

	LookupREFRByHandle(talkingHandle, refr);
	return refr;
}

ActorValueList * ActorValueList::GetSingleton(void)
{
	return *g_actorValueList;
}

ActorValueInfo * ActorValueList::GetActorValue(UInt32 id)
{
	return (id < kNumActorValues) ? actorValues[id] : NULL;
}

UInt32 ActorValueList::ResolveActorValueByName(const char * name)
{
	UInt32 id = LookupActorValueByName(name);
	if (id >= kNumActorValues)
		return 255;

	return id;
}

DefaultObjectList * DefaultObjectList::GetSingleton(void)
{
	return g_defaultObjectList.GetPtr();
}

void BGSSaveLoadManager::Save(const char * name)
{
	CALL_MEMBER_FN(this, Save_Internal)(2, 0, name);
}

void BGSSaveLoadManager::Load(const char * name)
{
	CALL_MEMBER_FN(this, Load_Internal)(name, -1, 0, 1);
}

BGSSaveLoadManager * BGSSaveLoadManager::GetSingleton(void)
{
	return *g_saveLoadManager;
}

RelationshipRanks * RelationshipRanks::GetSingleton(void)
{
	return g_relationshipRanks;
}

SInt32 RelationshipRanks::GetRelationshipRank(TESForm * form1, TESForm * form2)
{
	RelationshipRanks * ranks = RelationshipRanks::GetSingleton();
	if (form1 && form2) {
		return ranks->value[GetRelationshipIndex(form1, form2)];
	}

	return ranks->value[kRelationshipAcquaintance];
}

FacePresetList * FacePresetList::GetSingleton(void)
{
	return g_facePresetList;
}
