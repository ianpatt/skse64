#pragma once

class IngredientItem;
class MagicItem;
class EffectSetting;
class VMClassRegistry;

namespace papyrusIngredient
{
	void RegisterFuncs(VMClassRegistry* registry);

	// MagicItem funcs
	UInt32 GetNumEffects(IngredientItem* thisMagic);
	float GetNthEffectMagnitude(IngredientItem* thisMagic, UInt32 index);
	UInt32 GetNthEffectArea(IngredientItem* thisMagic, UInt32 index);
	UInt32 GetNthEffectDuration(IngredientItem* thisMagic, UInt32 index);
	EffectSetting* GetNthEffectMagicEffect(IngredientItem* thisMagic, UInt32 index);
	UInt32 GetCostliestEffectIndex(IngredientItem* thisMagic);
	bool GetIsNthEffectKnown(IngredientItem* thisMagic, UInt32 index);

	void SetNthEffectMagnitude(IngredientItem* thisMagic, UInt32 index, float value);
	void SetNthEffectArea(IngredientItem* thisMagic, UInt32 index, UInt32 value);
	void SetNthEffectDuration(IngredientItem* thisMagic, UInt32 index, UInt32 value);
};
