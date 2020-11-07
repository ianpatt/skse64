#pragma once

class VMClassRegistry;
class TESLevSpell;
class TESGlobal;
class TESForm;

namespace papyrusLeveledSpell
{
	void RegisterFuncs(VMClassRegistry* registry);

	UInt32 GetChanceNone(TESLevSpell * leveledItem);
	void SetChanceNone(TESLevSpell * leveledItem, UInt32 chanceNone);

	UInt32 GetNumForms(TESLevSpell * leveledItem);
	TESForm * GetNthForm(TESLevSpell * leveledItem, UInt32 n);
	UInt32 GetNthLevel(TESLevSpell * leveledItem, UInt32 n);
	void SetNthLevel(TESLevSpell * leveledItem, UInt32 n, UInt32 level);
};