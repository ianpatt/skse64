#include "PapyrusLeveledSpell.h"
#include "PapyrusLeveledItem.h"
#include "GameObjects.h"

namespace papyrusLeveledSpell
{
	UInt32 GetChanceNone(TESLevSpell * leveledItem)
	{ return leveledItemUtils::GetChanceNone(&leveledItem->leveledList); }

	void SetChanceNone(TESLevSpell * leveledItem, UInt32 chanceNone)
	{ leveledItemUtils::SetChanceNone(&leveledItem->leveledList, chanceNone); }

	UInt32 GetNumForms(TESLevSpell * leveledItem)
	{ return leveledItemUtils::GetNumForms(&leveledItem->leveledList); }

	TESForm * GetNthForm(TESLevSpell * leveledItem, UInt32 n)
	{ return leveledItemUtils::GetNthForm(&leveledItem->leveledList, n); }

	UInt32 GetNthLevel(TESLevSpell * leveledItem, UInt32 n)
	{ return leveledItemUtils::GetNthLevel(&leveledItem->leveledList, n); }

	void SetNthLevel(TESLevSpell * leveledItem, UInt32 n, UInt32 level)
	{ return leveledItemUtils::SetNthLevel(&leveledItem->leveledList, n, level); }
}

#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

void papyrusLeveledSpell::RegisterFuncs(VMClassRegistry* registry)
{
	registry->RegisterFunction(
		new NativeFunction0<TESLevSpell, UInt32>("GetChanceNone", "LeveledSpell", papyrusLeveledSpell::GetChanceNone, registry));

	registry->RegisterFunction(
		new NativeFunction1<TESLevSpell, void, UInt32>("SetChanceNone", "LeveledSpell", papyrusLeveledSpell::SetChanceNone, registry));

	registry->RegisterFunction(
		new NativeFunction0<TESLevSpell, UInt32>("GetNumForms", "LeveledSpell", papyrusLeveledSpell::GetNumForms, registry));

	registry->RegisterFunction(
		new NativeFunction1<TESLevSpell, TESForm*, UInt32>("GetNthForm", "LeveledSpell", papyrusLeveledSpell::GetNthForm, registry));

	registry->RegisterFunction(
		new NativeFunction1<TESLevSpell, UInt32, UInt32>("GetNthLevel", "LeveledSpell", papyrusLeveledSpell::GetNthLevel, registry));

	registry->RegisterFunction(
		new NativeFunction2<TESLevSpell, void, UInt32, UInt32>("SetNthLevel", "LeveledSpell", papyrusLeveledSpell::SetNthLevel, registry));

	registry->SetFunctionFlags("LeveledSpell", "GetChanceNone", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("LeveledSpell", "SetChanceNone", VMClassRegistry::kFunctionFlag_NoWait);

	registry->SetFunctionFlags("LeveledSpell", "GetNumForms", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("LeveledSpell", "GetNthForm", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("LeveledSpell", "GetNthLevel", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("LeveledSpell", "SetNthLevel", VMClassRegistry::kFunctionFlag_NoWait);
}
