#include "PapyrusLeveledItem.h"

#include "GameObjects.h"
#include "GameReferences.h"

namespace papyrusLeveledItem
{
	UInt32 GetChanceNone(TESLevItem * leveledItem)
	{ return leveledItemUtils::GetChanceNone(&leveledItem->leveledList); }

	void SetChanceNone(TESLevItem * leveledItem, UInt32 chanceNone)
	{ leveledItemUtils::SetChanceNone(&leveledItem->leveledList, chanceNone); }

	TESGlobal * GetChanceGlobal(TESLevItem * leveledItem)
	{ return leveledItemUtils::GetChanceGlobal(&leveledItem->leveledList); }

	void SetChanceGlobal(TESLevItem * leveledItem, TESGlobal * globalVar)
	{ leveledItemUtils::SetChanceGlobal(&leveledItem->leveledList, globalVar); }

	UInt32 GetNumForms(TESLevItem * leveledItem)
	{ return leveledItemUtils::GetNumForms(&leveledItem->leveledList); }

	TESForm * GetNthForm(TESLevItem * leveledItem, UInt32 n)
	{ return leveledItemUtils::GetNthForm(&leveledItem->leveledList, n); }

	UInt32 GetNthLevel(TESLevItem * leveledItem, UInt32 n)
	{ return leveledItemUtils::GetNthLevel(&leveledItem->leveledList, n); }

	void SetNthLevel(TESLevItem * leveledItem, UInt32 n, UInt32 level)
	{ return leveledItemUtils::SetNthLevel(&leveledItem->leveledList, n, level); }

	UInt32 GetNthCount(TESLevItem * leveledItem, UInt32 n)
	{ return leveledItemUtils::GetNthCount(&leveledItem->leveledList, n); }

	void SetNthCount(TESLevItem * leveledItem, UInt32 n, UInt32 count)
	{ return leveledItemUtils::SetNthCount(&leveledItem->leveledList, n, count); }
}

namespace leveledItemUtils
{
	UInt32 GetChanceNone(TESLeveledList * leveledList)
	{
		return leveledList->chanceValue;
	}

	void SetChanceNone(TESLeveledList * leveledList, UInt32 chanceNone)
	{
		leveledList->chanceValue = chanceNone;
	}

	TESGlobal * GetChanceGlobal(TESLeveledList * leveledList)
	{
		return leveledList->chanceGlobal;
	}

	void SetChanceGlobal(TESLeveledList * leveledList, TESGlobal * globalVar)
	{
		leveledList->chanceGlobal = globalVar;
	}

	UInt32 GetNumForms(TESLeveledList * leveledList)
	{
		return leveledList->length;
	}

	TESForm * GetNthForm(TESLeveledList * leveledList, UInt32 n)
	{
		return (n < leveledList->length) ? leveledList->entries[n].form : NULL;
	}

	UInt32 GetNthLevel(TESLeveledList * leveledList, UInt32 n)
	{
		return (n < leveledList->length) ? leveledList->entries[n].level : 0;
	}

	void SetNthLevel(TESLeveledList * leveledList, UInt32 n, UInt32 level)
	{
		if(n < leveledList->length) {
			if(level > 0xFFFF)
				level = 0xFFFF;

			leveledList->entries[n].level = level;
		}
	}

	UInt32 GetNthCount(TESLeveledList * leveledList, UInt32 n)
	{
		return (n < leveledList->length) ? leveledList->entries[n].count : 0;
	}

	void SetNthCount(TESLeveledList * leveledList, UInt32 n, UInt32 count)
	{
		if(n < leveledList->length) {
			if(count > 0xFFFF)
				count = 0xFFFF;

			leveledList->entries[n].count = count;
		}
	}
}

#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

void papyrusLeveledItem::RegisterFuncs(VMClassRegistry* registry)
{
	registry->RegisterFunction(
		new NativeFunction0<TESLevItem, UInt32>("GetChanceNone", "LeveledItem", papyrusLeveledItem::GetChanceNone, registry));

	registry->RegisterFunction(
		new NativeFunction1<TESLevItem, void, UInt32>("SetChanceNone", "LeveledItem", papyrusLeveledItem::SetChanceNone, registry));

	registry->RegisterFunction(
		new NativeFunction0<TESLevItem, TESGlobal*>("GetChanceGlobal", "LeveledItem", papyrusLeveledItem::GetChanceGlobal, registry));

	registry->RegisterFunction(
		new NativeFunction1<TESLevItem, void, TESGlobal*>("SetChanceGlobal", "LeveledItem", papyrusLeveledItem::SetChanceGlobal, registry));


	registry->RegisterFunction(
		new NativeFunction0<TESLevItem, UInt32>("GetNumForms", "LeveledItem", papyrusLeveledItem::GetNumForms, registry));

	registry->RegisterFunction(
		new NativeFunction1<TESLevItem, TESForm*, UInt32>("GetNthForm", "LeveledItem", papyrusLeveledItem::GetNthForm, registry));

	registry->RegisterFunction(
		new NativeFunction1<TESLevItem, UInt32, UInt32>("GetNthLevel", "LeveledItem", papyrusLeveledItem::GetNthLevel, registry));

	registry->RegisterFunction(
		new NativeFunction2<TESLevItem, void, UInt32, UInt32>("SetNthLevel", "LeveledItem", papyrusLeveledItem::SetNthLevel, registry));

	registry->RegisterFunction(
		new NativeFunction1<TESLevItem, UInt32, UInt32>("GetNthCount", "LeveledItem", papyrusLeveledItem::GetNthCount, registry));

	registry->RegisterFunction(
		new NativeFunction2<TESLevItem, void, UInt32, UInt32>("SetNthCount", "LeveledItem", papyrusLeveledItem::SetNthCount, registry));

	registry->SetFunctionFlags("LeveledItem", "GetChanceNone", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("LeveledItem", "SetChanceNone", VMClassRegistry::kFunctionFlag_NoWait);

	registry->SetFunctionFlags("LeveledItem", "GetChanceGlobal", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("LeveledItem", "SetChanceGlobal", VMClassRegistry::kFunctionFlag_NoWait);

	registry->SetFunctionFlags("LeveledItem", "GetNumForms", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("LeveledItem", "GetNthForm", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("LeveledItem", "GetNthLevel", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("LeveledItem", "SetNthLevel", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("LeveledItem", "GetNthCount", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("LeveledItem", "SetNthCount", VMClassRegistry::kFunctionFlag_NoWait);
}