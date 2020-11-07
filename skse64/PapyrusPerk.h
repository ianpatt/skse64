#pragma once

class SpellItem;
class TESQuest;
class TESLevItem;
class BGSPerk;
class VMClassRegistry;

#include "GameTypes.h"

namespace papyrusPerk
{
	void RegisterFuncs(VMClassRegistry* registry);

	BGSPerk * GetNextPerk(BGSPerk * perk);
	UInt32 GetNumEntries(BGSPerk * perk);
	UInt32 GetNthEntryRank(BGSPerk * perk, UInt32 n);
	UInt32 GetNthEntryPriority(BGSPerk * perk, UInt32 n);
	TESQuest * GetNthEntryQuest(BGSPerk * perk, UInt32 n);
	UInt32 GetNthEntryStage(BGSPerk * perk, UInt32 n);
	SpellItem * GetNthEntrySpell(BGSPerk * perk, UInt32 n);
	TESLevItem * GetNthEntryLeveledList(BGSPerk * perk, UInt32 n);
	BSFixedString GetNthEntryText(BGSPerk * perk, UInt32 n);
	float GetNthEntryValue(BGSPerk * perk, UInt32 n, UInt32 i);

	bool SetNthEntryRank(BGSPerk * perk, UInt32 n, UInt32 value);
	bool SetNthEntryPriority(BGSPerk * perk, UInt32 n, UInt32 value);
	bool SetNthEntryQuest(BGSPerk * perk, UInt32 n, TESQuest * quest);
	bool SetNthEntryStage(BGSPerk * perk, UInt32 n, UInt32 stage);
	bool SetNthEntrySpell(BGSPerk * perk, UInt32 n, SpellItem * spell);
	bool SetNthEntryLeveledList(BGSPerk * perk, UInt32 n, TESLevItem * leveledList);
	bool SetNthEntryText(BGSPerk * perk, UInt32 n, BSFixedString str);
	bool SetNthEntryValue(BGSPerk * perk, UInt32 n, UInt32 i, float value);
};
