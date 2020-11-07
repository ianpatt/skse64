#include "PapyrusPerk.h"

#include "GameForms.h"
#include "GameObjects.h"
#include "GameRTTI.h"

namespace papyrusPerk
{
	BGSPerk * GetNextPerk(BGSPerk * perk)
	{
		return (perk) ? perk->nextPerk : NULL;
	}

	UInt32 GetNumEntries(BGSPerk * perk)
	{
		return (perk) ? perk->perkEntries.count : 0;
	}

	UInt32 GetNthEntryRank(BGSPerk * perk, UInt32 n)
	{
		if(!perk)
			return 0;

		BGSPerkEntry * perkEntry;
		if(!perk->perkEntries.GetNthItem(n, perkEntry))
			return 0;

		return perkEntry->rank;
	}

	UInt32 GetNthEntryPriority(BGSPerk * perk, UInt32 n)
	{
		if(!perk)
			return 0;

		BGSPerkEntry * perkEntry;
		if(!perk->perkEntries.GetNthItem(n, perkEntry))
			return 0;

		return perkEntry->priority;
	}

	// Quest Perk Entry
	TESQuest * GetNthEntryQuest(BGSPerk * perk, UInt32 n)
	{
		if(!perk)
			return NULL;

		BGSPerkEntry * perkEntry;
		if(!perk->perkEntries.GetNthItem(n, perkEntry))
			return NULL;

		BGSQuestPerkEntry * questEntry = DYNAMIC_CAST(perkEntry, BGSPerkEntry, BGSQuestPerkEntry);
		if(!questEntry)
			return NULL;

		return questEntry->quest;
	}

	UInt32 GetNthEntryStage(BGSPerk * perk, UInt32 n)
	{
		if(!perk)
			return 0;

		BGSPerkEntry * perkEntry;
		if(!perk->perkEntries.GetNthItem(n, perkEntry))
			return 0;

		BGSQuestPerkEntry * questEntry = DYNAMIC_CAST(perkEntry, BGSPerkEntry, BGSQuestPerkEntry);
		if(!questEntry)
			return 0;

		return questEntry->stage;
	}

	// Ability Perk Entry
	// EntryPoint Perk Entry
	SpellItem * GetNthEntrySpell(BGSPerk * perk, UInt32 n)
	{
		if(!perk)
			return NULL;

		BGSPerkEntry * perkEntry;
		if(!perk->perkEntries.GetNthItem(n, perkEntry))
			return NULL;

		BGSAbilityPerkEntry * abilityEntry = DYNAMIC_CAST(perkEntry, BGSPerkEntry, BGSAbilityPerkEntry);
		if(abilityEntry)
			return abilityEntry->spellItem;

		BGSEntryPointPerkEntry * entryPointEntry = DYNAMIC_CAST(perkEntry, BGSPerkEntry, BGSEntryPointPerkEntry);
		if(entryPointEntry) {
			BGSEntryPointFunctionDataSpellItem * functionDataSpell = DYNAMIC_CAST(entryPointEntry->data, BGSEntryPointFunctionData, BGSEntryPointFunctionDataSpellItem);
			if(functionDataSpell)
				return functionDataSpell->spellItem;

			BGSEntryPointFunctionDataActivateChoice * functionDataActivation = DYNAMIC_CAST(entryPointEntry->data, BGSEntryPointFunctionData, BGSEntryPointFunctionDataActivateChoice);
			if(functionDataActivation)
				return functionDataActivation->appliedSpell;
		}

		return NULL;
	}

	TESLevItem * GetNthEntryLeveledList(BGSPerk * perk, UInt32 n)
	{
		if(!perk)
			return NULL;

		BGSPerkEntry * perkEntry;
		if(!perk->perkEntries.GetNthItem(n, perkEntry))
			return NULL;

		BGSEntryPointPerkEntry * entryPointEntry = DYNAMIC_CAST(perkEntry, BGSPerkEntry, BGSEntryPointPerkEntry);
		if(entryPointEntry) {
			BGSEntryPointFunctionDataLeveledList * functionDataLeveledList = DYNAMIC_CAST(entryPointEntry->data, BGSEntryPointFunctionData, BGSEntryPointFunctionDataLeveledList);
			if(functionDataLeveledList)
				return functionDataLeveledList->leveledList;
		}

		return NULL;
	}

	BSFixedString GetNthEntryText(BGSPerk * perk, UInt32 n)
	{
		if(!perk)
			return NULL;

		BGSPerkEntry * perkEntry;
		if(!perk->perkEntries.GetNthItem(n, perkEntry))
			return NULL;

		BGSEntryPointPerkEntry * entryPointEntry = DYNAMIC_CAST(perkEntry, BGSPerkEntry, BGSEntryPointPerkEntry);
		if(entryPointEntry) {
			BGSEntryPointFunctionDataText * functionDataText = DYNAMIC_CAST(entryPointEntry->data, BGSEntryPointFunctionData, BGSEntryPointFunctionDataText);
			if(functionDataText)
				return functionDataText->text;

			BGSEntryPointFunctionDataActivateChoice * functionDataActivation = DYNAMIC_CAST(entryPointEntry->data, BGSEntryPointFunctionData, BGSEntryPointFunctionDataActivateChoice);
			if(functionDataActivation)
				return functionDataActivation->label;

			BGSEntryPointFunctionDataBooleanGraphVariable * functionDataGraphVar = DYNAMIC_CAST(entryPointEntry->data, BGSEntryPointFunctionData, BGSEntryPointFunctionDataBooleanGraphVariable);
			if(functionDataGraphVar)
				return functionDataGraphVar->variable;
		}

		return NULL;
	}

	float GetNthEntryValue(BGSPerk * perk, UInt32 n, UInt32 i)
	{
		if(!perk || i >= BGSEntryPointFunctionDataTwoValue::kNumValues)
			return 0.0;

		BGSPerkEntry * perkEntry;
		if(!perk->perkEntries.GetNthItem(n, perkEntry))
			return 0.0;

		BGSEntryPointPerkEntry * entryPointEntry = DYNAMIC_CAST(perkEntry, BGSPerkEntry, BGSEntryPointPerkEntry);
		if(entryPointEntry) {
			BGSEntryPointFunctionDataOneValue * functionOne = DYNAMIC_CAST(entryPointEntry->data, BGSEntryPointFunctionData, BGSEntryPointFunctionDataOneValue);
			if(functionOne)
				return functionOne->value;

			BGSEntryPointFunctionDataTwoValue * functionTwo = DYNAMIC_CAST(entryPointEntry->data, BGSEntryPointFunctionData, BGSEntryPointFunctionDataTwoValue);
			if(functionTwo)
				return functionTwo->value[i];
		}

		return 0.0;
	}

	bool SetNthEntryRank(BGSPerk * perk, UInt32 n, UInt32 value)
	{
		if(!perk)
			return false;

		BGSPerkEntry * perkEntry;
		if(perk->perkEntries.GetNthItem(n, perkEntry)) {
			perkEntry->rank = value;
			return true;
		}

		return false;
	}

	bool SetNthEntryPriority(BGSPerk * perk, UInt32 n, UInt32 value)
	{
		if(!perk)
			return false;

		BGSPerkEntry * perkEntry;
		if(perk->perkEntries.GetNthItem(n, perkEntry)) {
			perkEntry->priority = value;
			return true;
		}

		return false;
	}

	// Quest Perk Entry
	bool SetNthEntryQuest(BGSPerk * perk, UInt32 n, TESQuest * quest)
	{
		if(!perk)
			return false;

		BGSPerkEntry * perkEntry;
		if(perk->perkEntries.GetNthItem(n, perkEntry)) {
			BGSQuestPerkEntry * questEntry = DYNAMIC_CAST(perkEntry, BGSPerkEntry, BGSQuestPerkEntry);
			if(questEntry) {
				questEntry->quest = quest;
				return true;
			}
		}

		return false;
	}

	bool SetNthEntryStage(BGSPerk * perk, UInt32 n, UInt32 stage)
	{
		if(!perk)
			return false;

		BGSPerkEntry * perkEntry;
		if(perk->perkEntries.GetNthItem(n, perkEntry)) {
			BGSQuestPerkEntry * questEntry = DYNAMIC_CAST(perkEntry, BGSPerkEntry, BGSQuestPerkEntry);
			if(questEntry) {
				questEntry->stage = stage;
				return true;
			}
		}

		return false;
	}

	// Ability Perk Entry
	// EntryPoint Perk Entry
	bool SetNthEntrySpell(BGSPerk * perk, UInt32 n, SpellItem * spell)
	{
		if(!perk)
			return false;

		BGSPerkEntry * perkEntry;
		if(perk->perkEntries.GetNthItem(n, perkEntry)) {
			BGSAbilityPerkEntry * abilityEntry = DYNAMIC_CAST(perkEntry, BGSPerkEntry, BGSAbilityPerkEntry);
			if(abilityEntry) {
				abilityEntry->spellItem = spell;
				return true;
			}

			BGSEntryPointPerkEntry * entryPointEntry = DYNAMIC_CAST(perkEntry, BGSPerkEntry, BGSEntryPointPerkEntry);
			if(entryPointEntry) {
				BGSEntryPointFunctionDataSpellItem * functionDataSpell = DYNAMIC_CAST(entryPointEntry->data, BGSEntryPointFunctionData, BGSEntryPointFunctionDataSpellItem);
				if(functionDataSpell) {
					functionDataSpell->spellItem = spell;
					return true;
				}

				BGSEntryPointFunctionDataActivateChoice * functionDataActivation = DYNAMIC_CAST(entryPointEntry->data, BGSEntryPointFunctionData, BGSEntryPointFunctionDataActivateChoice);
				if(functionDataActivation) {
					functionDataActivation->appliedSpell = spell;
					return true;
				}
			}
		}

		return false;
	}

	bool SetNthEntryLeveledList(BGSPerk * perk, UInt32 n, TESLevItem * leveledList)
	{
		if(!perk)
			return false;

		BGSPerkEntry * perkEntry;
		if(perk->perkEntries.GetNthItem(n, perkEntry)) {
			BGSEntryPointPerkEntry * entryPointEntry = DYNAMIC_CAST(perkEntry, BGSPerkEntry, BGSEntryPointPerkEntry);
			if(entryPointEntry) {
				BGSEntryPointFunctionDataLeveledList * functionDataLeveledList = DYNAMIC_CAST(entryPointEntry->data, BGSEntryPointFunctionData, BGSEntryPointFunctionDataLeveledList);
				if(functionDataLeveledList) {
					functionDataLeveledList->leveledList = leveledList;
					return true;
				}
			}
		}

		return false;
	}

	bool SetNthEntryText(BGSPerk * perk, UInt32 n, BSFixedString str)
	{
		if(!perk)
			return false;

		BGSPerkEntry * perkEntry;
		if(perk->perkEntries.GetNthItem(n, perkEntry)) {
			BGSEntryPointPerkEntry * entryPointEntry = DYNAMIC_CAST(perkEntry, BGSPerkEntry, BGSEntryPointPerkEntry);
			if(entryPointEntry) {
				BGSEntryPointFunctionDataText * functionDataText = DYNAMIC_CAST(entryPointEntry->data, BGSEntryPointFunctionData, BGSEntryPointFunctionDataText);
				if(functionDataText) {
					functionDataText->text = str;
					return true;
				}

				BGSEntryPointFunctionDataActivateChoice * functionDataActivation = DYNAMIC_CAST(entryPointEntry->data, BGSEntryPointFunctionData, BGSEntryPointFunctionDataActivateChoice);
				if(functionDataActivation) {
					functionDataActivation->label = str;
					return true;
				}

				BGSEntryPointFunctionDataBooleanGraphVariable * functionDataGraphVar = DYNAMIC_CAST(entryPointEntry->data, BGSEntryPointFunctionData, BGSEntryPointFunctionDataBooleanGraphVariable);
				if(functionDataGraphVar) {
					functionDataGraphVar->variable = str;
					return true;
				}
			}
		}

		return false;
	}

	bool SetNthEntryValue(BGSPerk * perk, UInt32 n, UInt32 i, float value)
	{
		if(perk && i < BGSEntryPointFunctionDataTwoValue::kNumValues) {
			BGSPerkEntry * perkEntry;
			if(perk->perkEntries.GetNthItem(n, perkEntry)) {
				BGSEntryPointPerkEntry * entryPointEntry = DYNAMIC_CAST(perkEntry, BGSPerkEntry, BGSEntryPointPerkEntry);
				if(entryPointEntry) {
					BGSEntryPointFunctionDataOneValue * functionOne = DYNAMIC_CAST(entryPointEntry->data, BGSEntryPointFunctionData, BGSEntryPointFunctionDataOneValue);
					if(functionOne) {
						functionOne->value = value;
						return true;
					}

					BGSEntryPointFunctionDataTwoValue * functionTwo = DYNAMIC_CAST(entryPointEntry->data, BGSEntryPointFunctionData, BGSEntryPointFunctionDataTwoValue);
					if(functionTwo) {
						functionTwo->value[i] = value;
						return true;
					}
				}
			}
		}

		return false;
	}
}

#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

void papyrusPerk::RegisterFuncs(VMClassRegistry* registry)
{
	registry->RegisterFunction(
		new NativeFunction0<BGSPerk, BGSPerk*>("GetNextPerk", "Perk", papyrusPerk::GetNextPerk, registry));

	registry->RegisterFunction(
		new NativeFunction0<BGSPerk, UInt32>("GetNumEntries", "Perk", papyrusPerk::GetNumEntries, registry));

	registry->RegisterFunction(
		new NativeFunction1<BGSPerk, UInt32, UInt32>("GetNthEntryRank", "Perk", papyrusPerk::GetNthEntryRank, registry));

	registry->RegisterFunction(
		new NativeFunction1<BGSPerk, UInt32, UInt32>("GetNthEntryPriority", "Perk", papyrusPerk::GetNthEntryPriority, registry));

	registry->RegisterFunction(
		new NativeFunction1<BGSPerk, TESQuest*, UInt32>("GetNthEntryQuest", "Perk", papyrusPerk::GetNthEntryQuest, registry));

	registry->RegisterFunction(
		new NativeFunction1<BGSPerk, UInt32, UInt32>("GetNthEntryStage", "Perk", papyrusPerk::GetNthEntryStage, registry));

	registry->RegisterFunction(
		new NativeFunction1<BGSPerk, SpellItem*, UInt32>("GetNthEntrySpell", "Perk", papyrusPerk::GetNthEntrySpell, registry));

	registry->RegisterFunction(
		new NativeFunction1<BGSPerk, TESLevItem*, UInt32>("GetNthEntryLeveledList", "Perk", papyrusPerk::GetNthEntryLeveledList, registry));

	registry->RegisterFunction(
		new NativeFunction1<BGSPerk, BSFixedString, UInt32>("GetNthEntryText", "Perk", papyrusPerk::GetNthEntryText, registry));

	registry->RegisterFunction(
		new NativeFunction2<BGSPerk, float, UInt32, UInt32>("GetNthEntryValue", "Perk", papyrusPerk::GetNthEntryValue, registry));

	registry->RegisterFunction(
		new NativeFunction2<BGSPerk, bool, UInt32, UInt32>("SetNthEntryRank", "Perk", papyrusPerk::SetNthEntryRank, registry));

	registry->RegisterFunction(
		new NativeFunction2<BGSPerk, bool, UInt32, UInt32>("SetNthEntryPriority", "Perk", papyrusPerk::SetNthEntryPriority, registry));

	registry->RegisterFunction(
		new NativeFunction2<BGSPerk, bool, UInt32, TESQuest*>("SetNthEntryQuest", "Perk", papyrusPerk::SetNthEntryQuest, registry));

	registry->RegisterFunction(
		new NativeFunction2<BGSPerk, bool, UInt32, UInt32>("SetNthEntryStage", "Perk", papyrusPerk::SetNthEntryStage, registry));

	registry->RegisterFunction(
		new NativeFunction2<BGSPerk, bool, UInt32, SpellItem*>("SetNthEntrySpell", "Perk", papyrusPerk::SetNthEntrySpell, registry));

	registry->RegisterFunction(
		new NativeFunction2<BGSPerk, bool, UInt32, TESLevItem*>("SetNthEntryLeveledList", "Perk", papyrusPerk::SetNthEntryLeveledList, registry));

	registry->RegisterFunction(
		new NativeFunction2<BGSPerk, bool, UInt32, BSFixedString>("SetNthEntryText", "Perk", papyrusPerk::SetNthEntryText, registry));

	registry->RegisterFunction(
		new NativeFunction3<BGSPerk, bool, UInt32, UInt32, float>("SetNthEntryValue", "Perk", papyrusPerk::SetNthEntryValue, registry));
}
