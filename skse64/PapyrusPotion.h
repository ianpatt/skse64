#pragma once

class AlchemyItem;
class BGSPerk;
class EffectSetting;
class BGSSoundDescriptorForm;
class VMClassRegistry;

namespace papyrusPotion
{
	void RegisterFuncs(VMClassRegistry* registry);

	bool IsFood(AlchemyItem* thisPotion);
	bool IsPoison(AlchemyItem* thisPotion);
	UInt32 GetNumEffects(AlchemyItem* thisMagic);
	float GetNthEffectMagnitude(AlchemyItem* thisMagic, UInt32 index);
	UInt32 GetNthEffectArea(AlchemyItem* thisMagic, UInt32 index);
	UInt32 GetNthEffectDuration(AlchemyItem* thisMagic, UInt32 index);
	EffectSetting* GetNthEffectMagicEffect(AlchemyItem* thisMagic, UInt32 index);
	UInt32 GetCostliestEffectIndex(AlchemyItem* thisMagic);
	BGSSoundDescriptorForm* GetUseSound(AlchemyItem* thisPotion);

	void SetNthEffectMagnitude(AlchemyItem* thisMagic, UInt32 index, float value);
	void SetNthEffectArea(AlchemyItem* thisMagic, UInt32 index, UInt32 value);
	void SetNthEffectDuration(AlchemyItem* thisMagic, UInt32 index, UInt32 value);
};
