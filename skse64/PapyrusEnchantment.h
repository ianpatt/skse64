#pragma once

class EnchantmentItem;
class MagicItem;
class EffectSetting;
class VMClassRegistry;

namespace papyrusEnchantment
{
	void RegisterFuncs(VMClassRegistry* registry);

	// MagicItem funcs
	UInt32 GetNumEffects(EnchantmentItem* thisMagic);
	float GetNthEffectMagnitude(EnchantmentItem* thisMagic, UInt32 index);
	UInt32 GetNthEffectArea(EnchantmentItem* thisMagic, UInt32 index);
	UInt32 GetNthEffectDuration(EnchantmentItem* thisMagic, UInt32 index);
	EffectSetting* GetNthEffectMagicEffect(EnchantmentItem* thisMagic, UInt32 index);
	//UInt32 GetCostliestEffectIndex(EnchantmentItem* thisMagic);
	EnchantmentItem * GetBaseEnchantment(EnchantmentItem* thisMagic);

	void SetNthEffectMagnitude(EnchantmentItem* thisSpell, UInt32 index, float value);
	void SetNthEffectArea(EnchantmentItem* thisSpell, UInt32 index, UInt32 value);
	void SetNthEffectDuration(EnchantmentItem* thisSpell, UInt32 index, UInt32 value);
};
