#include "PapyrusRace.h"

#include "GameObjects.h"
#include "GameFormComponents.h"
#include "GameForms.h"
#include "GameData.h"
#include <vector>

#include "common/ICriticalSection.h"

UInt32 g_invalidatePlayableRaceCache = 0;
UInt32 g_invalidateRaceCache = 0;

namespace papyrusRace
{
	typedef std::map<BSFixedString, TESRace*> RaceCache;
	static ICriticalSection	s_raceCacheLock;
	static RaceCache s_raceCache;

	TESRace* GetRace(StaticFunctionTag*, BSFixedString editorID)
	{
		s_raceCacheLock.Enter();

		if(g_invalidateRaceCache == 1) {
			s_raceCache.clear();
			g_invalidateRaceCache = 0;
		}

		if (s_raceCache.empty()) {
			DataHandler* pDataHandler = DataHandler::GetSingleton();
			tArray<TESRace*>& races = pDataHandler->races;
			for (UInt32 n = 0; n < 	races.count; n++) {
				TESRace* pRace = NULL;
				races.GetNthItem(n, pRace);
				if (pRace) {
					s_raceCache.insert(RaceCache::value_type(pRace->editorId, pRace));
				}
			}
		}

		s_raceCacheLock.Leave();

		RaceCache::iterator it = s_raceCache.find(editorID);
		TESRace* pRace = (it != s_raceCache.end()) ? it->second : NULL;
		return pRace;
	}

	typedef std::vector<TESRace*> PlayableRaceCache;
	static ICriticalSection	s_playableRaceCacheLock;
	static PlayableRaceCache s_playableRaceCache;

	UInt32 GetNumPlayableRaces(StaticFunctionTag * base)
	{
		s_playableRaceCacheLock.Enter();

		if(g_invalidatePlayableRaceCache == 1) {
			s_playableRaceCache.clear();
			g_invalidatePlayableRaceCache = 0;
		}

		if (s_playableRaceCache.empty()) {
			DataHandler* pDataHandler = DataHandler::GetSingleton();
			tArray<TESRace*>& races = pDataHandler->races;
			for (UInt32 n = 0; n < 	races.count; n++) {
				TESRace* pRace = NULL;
				races.GetNthItem(n, pRace);
				if (pRace && IsRaceFlagSet(pRace, TESRace::kRace_Playable)) {
					s_playableRaceCache.push_back(pRace);
				}
			}
		}

		s_playableRaceCacheLock.Leave();
		return s_playableRaceCache.size();
	}

	TESRace * GetNthPlayableRace(StaticFunctionTag * base, UInt32 n)
	{
		if(n < 0) {
			return NULL;
		}

		s_playableRaceCacheLock.Enter();

		if(g_invalidatePlayableRaceCache == 1) {
			s_playableRaceCache.clear();
			g_invalidatePlayableRaceCache = 0;
		}

		if (s_playableRaceCache.empty()) {
			DataHandler* pDataHandler = DataHandler::GetSingleton();
			tArray<TESRace*>& races = pDataHandler->races;
			for (UInt32 n = 0; n < 	races.count; n++) {
				TESRace* pRace = NULL;
				races.GetNthItem(n, pRace);
				if (pRace && IsRaceFlagSet(pRace, TESRace::kRace_Playable)) {
					s_playableRaceCache.push_back(pRace);
				}
			}
		}

		s_playableRaceCacheLock.Leave();
		return (n < s_playableRaceCache.size()) ? s_playableRaceCache.at(n) : NULL;
	}

	UInt32 GetSpellCount(TESRace* thisRace)
	{
		return (thisRace)? thisRace->spellList.GetSpellCount() : 0;
	}

	SpellItem* GetNthSpell(TESRace* thisRace, UInt32 n)
	{
		return (thisRace) ? thisRace->spellList.GetNthSpell(n) : NULL;
	}

	bool IsRaceFlagSet(TESRace* thisRace, UInt32 flag)
	{
		return (thisRace) ? (thisRace->data.raceFlags & flag) == flag : false;
	}

	void SetRaceFlag(TESRace* thisRace, UInt32 flag)
	{
		if (thisRace) {
			thisRace->data.raceFlags |= flag;
			if((flag & TESRace::kRace_Playable) == TESRace::kRace_Playable)
				g_invalidatePlayableRaceCache = 1;
		}
	}

	void ClearRaceFlag(TESRace* thisRace, UInt32 flag)
	{
		if (thisRace) {
			thisRace->data.raceFlags &= ~flag;
			if((flag & TESRace::kRace_Playable) == TESRace::kRace_Playable)
				g_invalidatePlayableRaceCache = 1;
		}
	}

	BGSVoiceType* GetDefaultVoiceType(TESRace* thisRace, bool female)
	{
		return (thisRace) ? thisRace->voiceTypes[female] : NULL;
	}

	void SetDefaultVoiceType(TESRace* thisRace, bool female, BGSVoiceType* voiceType)
	{
		if(thisRace) {
			thisRace->voiceTypes[female] = voiceType;
		}
	}

	TESObjectARMO * GetSkin(TESRace* thisRace)
	{
		return (thisRace) ? thisRace->skin.skin : NULL;
	}

	void SetSkin(TESRace* thisRace, TESObjectARMO * skin)
	{
		if(thisRace) {
			thisRace->skin.skin = skin;
		}
	}
}

#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

void papyrusRace::RegisterFuncs(VMClassRegistry* registry)
{

	registry->RegisterFunction(
		new NativeFunction0 <TESRace, UInt32>("GetSpellCount", "Race", papyrusRace::GetSpellCount, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESRace, SpellItem*, UInt32>("GetNthSpell", "Race", papyrusRace::GetNthSpell, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESRace, bool, UInt32>("IsRaceFlagSet", "Race", papyrusRace::IsRaceFlagSet, registry));
	
	registry->RegisterFunction(
		new NativeFunction1 <TESRace, void, UInt32>("SetRaceFlag", "Race", papyrusRace::SetRaceFlag, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESRace, void, UInt32>("ClearRaceFlag", "Race", papyrusRace::ClearRaceFlag, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESRace, BGSVoiceType*, bool>("GetDefaultVoiceType", "Race", papyrusRace::GetDefaultVoiceType, registry));

	registry->RegisterFunction(
		new NativeFunction2 <TESRace, void, bool, BGSVoiceType*>("SetDefaultVoiceType", "Race", papyrusRace::SetDefaultVoiceType, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, TESRace*, BSFixedString>("GetRace", "Race", papyrusRace::GetRace, registry));

	registry->RegisterFunction(
		new NativeFunction0 <StaticFunctionTag, UInt32>("GetNumPlayableRaces", "Race", papyrusRace::GetNumPlayableRaces, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, TESRace*, UInt32>("GetNthPlayableRace", "Race", papyrusRace::GetNthPlayableRace, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESRace, TESObjectARMO*>("GetSkin", "Race", papyrusRace::GetSkin, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESRace, void, TESObjectARMO*>("SetSkin", "Race", papyrusRace::SetSkin, registry));
}
