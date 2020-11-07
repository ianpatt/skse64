#include "PapyrusShout.h"
#include "GameObjects.h"

namespace papyrusShout
{
	TESWordOfPower * GetNthWordOfPower(TESShout * thisShout, UInt32 n)
	{
		return (thisShout && n < TESShout::Words::kNumWords) ? thisShout->words[n].word : NULL;
	}
	SpellItem * GetNthSpell(TESShout * thisShout, UInt32 n)
	{
		return (thisShout && n < TESShout::Words::kNumWords) ? thisShout->words[n].spell : NULL;
	}
	float GetNthRecoveryTime(TESShout * thisShout, UInt32 n)
	{
		return (thisShout && n < TESShout::Words::kNumWords) ? thisShout->words[n].recoverytime : 0.0;
	}

	void SetNthWordOfPower(TESShout * thisShout, UInt32 n, TESWordOfPower * woop)
	{
		if(thisShout && n < TESShout::Words::kNumWords) {
			thisShout->words[n].word = woop;
		}
	}
	void SetNthSpell(TESShout * thisShout, UInt32 n, SpellItem * spell)
	{
		if(thisShout && n < TESShout::Words::kNumWords) {
			thisShout->words[n].spell = spell;
		}
	}
	void SetNthRecoveryTime(TESShout * thisShout, UInt32 n, float time)
	{
		if(thisShout && n < TESShout::Words::kNumWords) {
			thisShout->words[n].recoverytime = time;
		}
	}
}

#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

void papyrusShout::RegisterFuncs(VMClassRegistry* registry)
{
	registry->RegisterFunction(
		new NativeFunction1<TESShout, TESWordOfPower*, UInt32>("GetNthWordOfPower", "Shout", papyrusShout::GetNthWordOfPower, registry));

	registry->RegisterFunction(
		new NativeFunction1<TESShout, SpellItem*, UInt32>("GetNthSpell", "Shout", papyrusShout::GetNthSpell, registry));

	registry->RegisterFunction(
		new NativeFunction1<TESShout, float, UInt32>("GetNthRecoveryTime", "Shout", papyrusShout::GetNthRecoveryTime, registry));

	registry->RegisterFunction(
		new NativeFunction2<TESShout, void, UInt32, TESWordOfPower*>("SetNthWordOfPower", "Shout", papyrusShout::SetNthWordOfPower, registry));

	registry->RegisterFunction(
		new NativeFunction2<TESShout, void, UInt32, SpellItem*>("SetNthSpell", "Shout", papyrusShout::SetNthSpell, registry));

	registry->RegisterFunction(
		new NativeFunction2<TESShout, void, UInt32, float>("SetNthRecoveryTime", "Shout", papyrusShout::SetNthRecoveryTime, registry));

	registry->SetFunctionFlags("Shout", "GetNthWordOfPower", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Shout", "GetNthSpell", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Shout", "GetNthRecoveryTime", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Shout", "SetNthWordOfPower", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Shout", "SetNthSpell", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Shout", "SetNthRecoveryTime", VMClassRegistry::kFunctionFlag_NoWait);
}
