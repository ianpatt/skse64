#include "PapyrusLeveledActor.h"
#include "PapyrusLeveledItem.h"
#include "GameObjects.h"

namespace papyrusLeveledActor
{
	UInt32 GetNumForms(TESLevCharacter * leveledItem)
	{ return leveledItemUtils::GetNumForms(&leveledItem->leveledList); }

	TESForm * GetNthForm(TESLevCharacter * leveledItem, UInt32 n)
	{ return leveledItemUtils::GetNthForm(&leveledItem->leveledList, n); }

	UInt32 GetNthLevel(TESLevCharacter * leveledItem, UInt32 n)
	{ return leveledItemUtils::GetNthLevel(&leveledItem->leveledList, n); }

	void SetNthLevel(TESLevCharacter * leveledItem, UInt32 n, UInt32 level)
	{ return leveledItemUtils::SetNthLevel(&leveledItem->leveledList, n, level); }

	UInt32 GetNthCount(TESLevCharacter * leveledItem, UInt32 n)
	{ return leveledItemUtils::GetNthCount(&leveledItem->leveledList, n); }

	void SetNthCount(TESLevCharacter * leveledItem, UInt32 n, UInt32 count)
	{ return leveledItemUtils::SetNthCount(&leveledItem->leveledList, n, count); }
}

#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

void papyrusLeveledActor::RegisterFuncs(VMClassRegistry* registry)
{
	registry->RegisterFunction(
		new NativeFunction0<TESLevCharacter, UInt32>("GetNumForms", "LeveledActor", papyrusLeveledActor::GetNumForms, registry));

	registry->RegisterFunction(
		new NativeFunction1<TESLevCharacter, TESForm*, UInt32>("GetNthForm", "LeveledActor", papyrusLeveledActor::GetNthForm, registry));

	registry->RegisterFunction(
		new NativeFunction1<TESLevCharacter, UInt32, UInt32>("GetNthLevel", "LeveledActor", papyrusLeveledActor::GetNthLevel, registry));

	registry->RegisterFunction(
		new NativeFunction2<TESLevCharacter, void, UInt32, UInt32>("SetNthLevel", "LeveledActor", papyrusLeveledActor::SetNthLevel, registry));

	registry->RegisterFunction(
		new NativeFunction1<TESLevCharacter, UInt32, UInt32>("GetNthCount", "LeveledActor", papyrusLeveledActor::GetNthCount, registry));

	registry->RegisterFunction(
		new NativeFunction2<TESLevCharacter, void, UInt32, UInt32>("SetNthCount", "LeveledActor", papyrusLeveledActor::SetNthCount, registry));

	registry->SetFunctionFlags("LeveledActor", "GetNumForms", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("LeveledActor", "GetNthForm", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("LeveledActor", "GetNthLevel", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("LeveledActor", "SetNthLevel", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("LeveledActor", "GetNthCount", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("LeveledActor", "SetNthCount", VMClassRegistry::kFunctionFlag_NoWait);
}
