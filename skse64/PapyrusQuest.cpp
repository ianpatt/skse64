#include "PapyrusQuest.h"
#include "PapyrusArgs.h"

#include "GameForms.h"
#include "GameData.h"

#include <algorithm>

#include "common/ICriticalSection.h"

// needs to be extern and set appropriately when a Quest is constructed
UInt32 g_invalidateQuestCache;

namespace papyrusQuest
{
	ICriticalSection	s_questCacheLock;
	typedef std::map<BSFixedString, TESQuest*> QuestCache;
	static QuestCache s_questCache;

	TESQuest* GetQuest(StaticFunctionTag*, BSFixedString editorID)
	{
		s_questCacheLock.Enter();

		if(g_invalidateQuestCache == 1) {
			g_invalidateQuestCache = 0;
			s_questCache.clear();
		}

		if (s_questCache.empty()) {
			DataHandler* pDataHandler = DataHandler::GetSingleton();
			tArray<TESQuest*>& quests = pDataHandler->quests;
			for (UInt32 n = 0; n < 	quests.count; n++) {
				TESQuest* pQuest = NULL;
				quests.GetNthItem(n, pQuest);
				if (pQuest) {
					s_questCache.insert(QuestCache::value_type(BSFixedString(pQuest->questID.Get()), pQuest));
				}
			}
		}

		s_questCacheLock.Leave();

		QuestCache::iterator it = s_questCache.find(editorID);
		TESQuest* pQuest = (it != s_questCache.end()) ? it->second : NULL;
		return pQuest;
	}

	BSFixedString GetId(TESQuest* thisQuest)
	{
		return (thisQuest) ? thisQuest->questID.Get() : NULL;
	}

	UInt32 GetPriority(TESQuest* thisQuest)
	{
		return (thisQuest) ? thisQuest->unk0D8.priority : 0;
	}

	UInt32 GetNumAliases(TESQuest* thisQuest)
	{
		return (thisQuest) ? thisQuest->aliases.count : 0;
	}

	BGSBaseAlias* GetNthAlias(TESQuest* thisQuest, UInt32 n)
	{
		if(!thisQuest || n > thisQuest->aliases.count)
			return NULL;
			
		BGSBaseAlias* alias = NULL;
		thisQuest->aliases.GetNthItem(n, alias);
		if(alias) {
			return alias;
		}

		return NULL;
	}

	BGSBaseAlias* GetAliasByName(TESQuest* thisQuest, BSFixedString s)
	{
		if(!thisQuest || !s.data)
			return NULL;

		for(UInt32 n = 0; n < thisQuest->aliases.count; n++) {
			BGSBaseAlias* alias = NULL;
			thisQuest->aliases.GetNthItem(n, alias);
			if(alias && alias->name == s) {
				return alias;
			}
		}
		
		return NULL;
	}

	VMResultArray<BGSBaseAlias*> GetAliases(TESQuest* thisQuest)
	{
		VMResultArray<BGSBaseAlias*> foundAliases;
		if(!thisQuest)
			return foundAliases;

		for(UInt32 n = 0; n < thisQuest->aliases.count; n++) {
			BGSBaseAlias* alias = NULL;
			thisQuest->aliases.GetNthItem(n, alias);
			if(alias) {
				foundAliases.push_back(alias);
			}
		}

		return foundAliases;
	}

	BGSBaseAlias* GetAliasById(TESQuest* thisQuest, UInt32 aliasId)
	{
		if(!thisQuest)
			return NULL;

		for(UInt32 n = 0; n < thisQuest->aliases.count; n++) {
			BGSBaseAlias* alias = NULL;
			thisQuest->aliases.GetNthItem(n, alias);
			if(alias && alias->aliasId == aliasId) {
				return alias;
			}
		}

		return NULL;
	}
}

#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

void papyrusQuest::RegisterFuncs(VMClassRegistry* registry)
{
	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, TESQuest*, BSFixedString> ("GetQuest", "Quest", papyrusQuest::GetQuest, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESQuest, BSFixedString> ("GetId", "Quest", papyrusQuest::GetId, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESQuest, UInt32> ("GetPriority", "Quest", papyrusQuest::GetPriority, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESQuest, UInt32> ("GetNumAliases", "Quest", papyrusQuest::GetNumAliases, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESQuest, BGSBaseAlias*, UInt32> ("GetNthAlias", "Quest", papyrusQuest::GetNthAlias, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESQuest, BGSBaseAlias*, BSFixedString> ("GetAliasByName", "Quest", papyrusQuest::GetAliasByName, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESQuest, BGSBaseAlias*, UInt32> ("GetAliasById", "Quest", papyrusQuest::GetAliasById, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESQuest, VMResultArray<BGSBaseAlias*>> ("GetAliases", "Quest", papyrusQuest::GetAliases, registry));
}