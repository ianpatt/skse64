#include "PapyrusEnchantment.h"
#include "PapyrusSpell.h"
#include "GameObjects.h"

namespace papyrusEnchantment
{

	UInt32 GetNumEffects(EnchantmentItem* thisMagic)
	{ return magicItemUtils::GetNumEffects(thisMagic); }

	float GetNthEffectMagnitude(EnchantmentItem* thisMagic, UInt32 index)
	{ return magicItemUtils::GetNthEffectMagnitude(thisMagic, index); }

	UInt32 GetNthEffectArea(EnchantmentItem* thisMagic, UInt32 index)
	{ return magicItemUtils::GetNthEffectArea(thisMagic, index); }

	UInt32 GetNthEffectDuration(EnchantmentItem* thisMagic, UInt32 index)
	{ return magicItemUtils::GetNthEffectDuration(thisMagic, index); }

	EffectSetting* GetNthEffectMagicEffect(EnchantmentItem* thisMagic, UInt32 index)
	{ return magicItemUtils::GetNthEffectMagicEffect(thisMagic, index); }

	UInt32 GetCostliestEffectIndex(EnchantmentItem* thisMagic)
	{ return magicItemUtils::GetCostliestEffectIndex(thisMagic); }

	void SetNthEffectMagnitude(EnchantmentItem* thisMagic, UInt32 index, float value)
	{ magicItemUtils::SetNthEffectMagnitude(thisMagic, index, value); }

	void SetNthEffectArea(EnchantmentItem* thisMagic, UInt32 index, UInt32 value)
	{ magicItemUtils::SetNthEffectArea(thisMagic, index, value); }

	void SetNthEffectDuration(EnchantmentItem* thisMagic, UInt32 index, UInt32 value)
	{ magicItemUtils::SetNthEffectDuration(thisMagic, index, value); }

	EnchantmentItem * GetBaseEnchantment(EnchantmentItem* thisMagic)
	{
		return thisMagic ? thisMagic->data.baseEnchantment : NULL;
	}

	BGSListForm * GetKeywordRestrictions(EnchantmentItem* thisMagic)
	{
		return thisMagic ? thisMagic->data.restrictions : NULL;
	}

	void SetKeywordRestrictions(EnchantmentItem* thisMagic, BGSListForm * list)
	{
		if(thisMagic) {
			thisMagic->data.restrictions = list;
		}
	}
}

#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

void papyrusEnchantment::RegisterFuncs(VMClassRegistry* registry)
{
	registry->RegisterFunction(
		new NativeFunction0<EnchantmentItem, UInt32>("GetNumEffects", "Enchantment", papyrusEnchantment::GetNumEffects, registry));

	registry->RegisterFunction(
		new NativeFunction1<EnchantmentItem, float, UInt32>("GetNthEffectMagnitude", "Enchantment", papyrusEnchantment::GetNthEffectMagnitude, registry));

	registry->RegisterFunction(
		new NativeFunction1<EnchantmentItem, UInt32, UInt32>("GetNthEffectArea", "Enchantment", papyrusEnchantment::GetNthEffectArea, registry));

	registry->RegisterFunction(
		new NativeFunction1<EnchantmentItem, UInt32, UInt32>("GetNthEffectDuration", "Enchantment", papyrusEnchantment::GetNthEffectDuration, registry));

	registry->RegisterFunction(
		new NativeFunction1<EnchantmentItem, EffectSetting*, UInt32>("GetNthEffectMagicEffect", "Enchantment", papyrusEnchantment::GetNthEffectMagicEffect, registry));

	registry->RegisterFunction(
		new NativeFunction0<EnchantmentItem, UInt32>("GetCostliestEffectIndex", "Enchantment", papyrusEnchantment::GetCostliestEffectIndex, registry));

	registry->RegisterFunction(
		new NativeFunction0<EnchantmentItem, EnchantmentItem*>("GetBaseEnchantment", "Enchantment", papyrusEnchantment::GetBaseEnchantment, registry));

	registry->RegisterFunction(
		new NativeFunction0<EnchantmentItem, BGSListForm*>("GetKeywordRestrictions", "Enchantment", papyrusEnchantment::GetKeywordRestrictions, registry));

	// Sets
	registry->RegisterFunction(
		new NativeFunction2<EnchantmentItem, void, UInt32, float>("SetNthEffectMagnitude", "Enchantment", papyrusEnchantment::SetNthEffectMagnitude, registry));

	registry->RegisterFunction(
		new NativeFunction2<EnchantmentItem, void, UInt32, UInt32>("SetNthEffectArea", "Enchantment", papyrusEnchantment::SetNthEffectArea, registry));

	registry->RegisterFunction(
		new NativeFunction2<EnchantmentItem, void, UInt32, UInt32>("SetNthEffectDuration", "Enchantment", papyrusEnchantment::SetNthEffectDuration, registry));

	registry->RegisterFunction(
		new NativeFunction1<EnchantmentItem, void, BGSListForm*>("SetKeywordRestrictions", "Enchantment", papyrusEnchantment::SetKeywordRestrictions, registry));

	registry->SetFunctionFlags("Enchantment", "GetNumEffects", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Enchantment", "GetNthEffectMagnitude", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Enchantment", "GetNthEffectArea", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Enchantment", "GetNthEffectDuration", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Enchantment", "GetNthEffectMagicEffect", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Enchantment", "GetCostliestEffectIndex", VMClassRegistry::kFunctionFlag_NoWait);

	registry->SetFunctionFlags("Enchantment", "GetBaseEnchantment", VMClassRegistry::kFunctionFlag_NoWait);

	registry->SetFunctionFlags("Enchantment", "SetNthEffectMagnitude", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Enchantment", "SetNthEffectArea", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Enchantment", "SetNthEffectDuration", VMClassRegistry::kFunctionFlag_NoWait);
}
