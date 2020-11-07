#pragma once

class TESShout;
class SpellItem;
class TESWordOfPower;
class VMClassRegistry;

namespace papyrusShout
{
	void RegisterFuncs(VMClassRegistry* registry);

	TESWordOfPower * GetNthWordOfPower(TESShout * thisShout, UInt32 n);
	SpellItem * GetNthSpell(TESShout * thisShout, UInt32 n);
	float GetNthRecoveryTime(TESShout * thisShout, UInt32 n);

	void SetNthWordOfPower(TESShout * thisShout, UInt32 n, TESWordOfPower * woop);
	void SetNthSpell(TESShout * thisShout, UInt32 n, SpellItem * spell);
	void SetNthRecoveryTime(TESShout * thisShout, UInt32 n, float time);
};
