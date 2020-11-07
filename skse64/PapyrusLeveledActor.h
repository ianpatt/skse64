#pragma once

class VMClassRegistry;
class TESLevCharacter;
class TESGlobal;
class TESForm;

namespace papyrusLeveledActor
{
	void RegisterFuncs(VMClassRegistry* registry);

	UInt32 GetChanceNone(TESLevCharacter * leveledItem);
	void SetChanceNone(TESLevCharacter * leveledItem, UInt32 chanceNone);

	UInt32 GetNumForms(TESLevCharacter * leveledItem);
	TESForm * GetNthForm(TESLevCharacter * leveledItem, UInt32 n);
	UInt32 GetNthLevel(TESLevCharacter * leveledItem, UInt32 n);
	void SetNthLevel(TESLevCharacter * leveledItem, UInt32 n, UInt32 level);
};