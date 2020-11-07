#pragma once

class TESObjectBOOK;
class SpellItem;
class VMClassRegistry;

namespace papyrusBook
{
	void RegisterFuncs(VMClassRegistry * registry);

	UInt32 GetSkill(TESObjectBOOK * thisBook);
	SpellItem * GetSpell(TESObjectBOOK * thisBook);
	bool IsRead(TESObjectBOOK * thisBook);
	bool IsTakeable(TESObjectBOOK * thisBook);
};
