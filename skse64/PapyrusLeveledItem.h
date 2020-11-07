#pragma once

class VMClassRegistry;
class TESLeveledList;
class TESLevItem;
class TESGlobal;
class TESForm;

namespace papyrusLeveledItem
{
	void RegisterFuncs(VMClassRegistry* registry);

	UInt32 GetChanceNone(TESLevItem * leveledItem);
	void SetChanceNone(TESLevItem * leveledItem, UInt32 chanceNone);

	TESGlobal * GetChanceGlobal(TESLevItem * leveledItem);
	void SetChanceGlobal(TESLevItem * leveledItem, TESGlobal * globalVar);

	UInt32 GetNumForms(TESLevItem * leveledItem);
	TESForm * GetNthForm(TESLevItem * leveledItem, UInt32 n);
	UInt32 GetNthLevel(TESLevItem * leveledItem, UInt32 n);
	void SetNthLevel(TESLevItem * leveledItem, UInt32 n, UInt32 level);
	UInt32 GetNthCount(TESLevItem * leveledItem, UInt32 n);
	void SetNthCount(TESLevItem * leveledItem, UInt32 n, UInt32 count);
};

namespace leveledItemUtils {
	UInt32 GetChanceNone(TESLeveledList * leveledList);
	void SetChanceNone(TESLeveledList * leveledList, UInt32 chanceNone);
	TESGlobal * GetChanceGlobal(TESLeveledList * leveledList);
	void SetChanceGlobal(TESLeveledList * leveledList, TESGlobal * globalVar);
	UInt32 GetNumForms(TESLeveledList * leveledList);
	TESForm * GetNthForm(TESLeveledList * leveledList, UInt32 n);
	UInt32 GetNthLevel(TESLeveledList * leveledList, UInt32 n);
	void SetNthLevel(TESLeveledList * leveledList, UInt32 n, UInt32 level);
	UInt32 GetNthCount(TESLeveledList * leveledList, UInt32 n);
	void SetNthCount(TESLeveledList * leveledList, UInt32 n, UInt32 count);
}
