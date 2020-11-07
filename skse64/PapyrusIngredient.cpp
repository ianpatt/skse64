#include "PapyrusIngredient.h"
#include "PapyrusSpell.h"
#include "GameObjects.h"

namespace papyrusIngredient
{

	UInt32 GetNumEffects(IngredientItem* thisMagic)
	{ return magicItemUtils::GetNumEffects(thisMagic); }

	float GetNthEffectMagnitude(IngredientItem* thisMagic, UInt32 index)
	{ return magicItemUtils::GetNthEffectMagnitude(thisMagic, index); }

	UInt32 GetNthEffectArea(IngredientItem* thisMagic, UInt32 index)
	{ return magicItemUtils::GetNthEffectArea(thisMagic, index); }

	UInt32 GetNthEffectDuration(IngredientItem* thisMagic, UInt32 index)
	{ return magicItemUtils::GetNthEffectDuration(thisMagic, index); }

	EffectSetting* GetNthEffectMagicEffect(IngredientItem* thisMagic, UInt32 index)
	{ return magicItemUtils::GetNthEffectMagicEffect(thisMagic, index); }

	UInt32 GetCostliestEffectIndex(IngredientItem* thisMagic)
	{ return magicItemUtils::GetCostliestEffectIndex(thisMagic); }

	void SetNthEffectMagnitude(IngredientItem* thisMagic, UInt32 index, float value)
	{ magicItemUtils::SetNthEffectMagnitude(thisMagic, index, value); }

	void SetNthEffectArea(IngredientItem* thisMagic, UInt32 index, UInt32 value)
	{ magicItemUtils::SetNthEffectArea(thisMagic, index, value); }

	void SetNthEffectDuration(IngredientItem* thisMagic, UInt32 index, UInt32 value)
	{ magicItemUtils::SetNthEffectDuration(thisMagic, index, value); }

	// ahzaab 8-25-13
	bool GetIsNthEffectKnown(IngredientItem* thisMagic, UInt32 index)
	{
		bool isKnown = false;
		if (!thisMagic)
			return false;
		switch (index)
		{
		case 0:
			isKnown = ((thisMagic->knownEffects & IngredientItem::kType_FirstEffect) == IngredientItem::kType_FirstEffect);
			break;
		case 1:
			isKnown = ((thisMagic->knownEffects & IngredientItem::kType_SecondEffect) == IngredientItem::kType_SecondEffect);
			break;
		case 2:
			isKnown = ((thisMagic->knownEffects & IngredientItem::kType_ThirdEffect) == IngredientItem::kType_ThirdEffect);
			break;
		case 3:
			isKnown = ((thisMagic->knownEffects & IngredientItem::kType_FourthEffect) == IngredientItem::kType_FourthEffect);
			break;
		default:
			break;
		}
		return isKnown;
	}

	VMResultArray<float> GetEffectMagnitudes(IngredientItem* thisMagic) { return magicItemUtils::GetEffectMagnitudes(thisMagic); }
	VMResultArray<UInt32> GetEffectAreas(IngredientItem* thisMagic) { return magicItemUtils::GetEffectAreas(thisMagic); }
	VMResultArray<UInt32> GetEffectDurations(IngredientItem* thisMagic) { return magicItemUtils::GetEffectDurations(thisMagic); }
	VMResultArray<EffectSetting*> GetMagicEffects(IngredientItem* thisMagic) { return magicItemUtils::GetMagicEffects(thisMagic); }
}

#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

void papyrusIngredient::RegisterFuncs(VMClassRegistry* registry)
{
	registry->RegisterFunction(
		new NativeFunction0<IngredientItem, UInt32>("GetNumEffects", "Ingredient", papyrusIngredient::GetNumEffects, registry));

	registry->RegisterFunction(
		new NativeFunction1<IngredientItem, float, UInt32>("GetNthEffectMagnitude", "Ingredient", papyrusIngredient::GetNthEffectMagnitude, registry));

	registry->RegisterFunction(
		new NativeFunction1<IngredientItem, UInt32, UInt32>("GetNthEffectArea", "Ingredient", papyrusIngredient::GetNthEffectArea, registry));

	registry->RegisterFunction(
		new NativeFunction1<IngredientItem, UInt32, UInt32>("GetNthEffectDuration", "Ingredient", papyrusIngredient::GetNthEffectDuration, registry));

	registry->RegisterFunction(
		new NativeFunction1<IngredientItem, EffectSetting*, UInt32>("GetNthEffectMagicEffect", "Ingredient", papyrusIngredient::GetNthEffectMagicEffect, registry));

	registry->RegisterFunction(
		new NativeFunction0<IngredientItem, UInt32>("GetCostliestEffectIndex", "Ingredient", papyrusIngredient::GetCostliestEffectIndex, registry));

	registry->RegisterFunction(
		new NativeFunction1<IngredientItem, bool, UInt32>("GetIsNthEffectKnown", "Ingredient", papyrusIngredient::GetIsNthEffectKnown, registry));

	// Sets
	registry->RegisterFunction(
		new NativeFunction2<IngredientItem, void, UInt32, float>("SetNthEffectMagnitude", "Ingredient", papyrusIngredient::SetNthEffectMagnitude, registry));

	registry->RegisterFunction(
		new NativeFunction2<IngredientItem, void, UInt32, UInt32>("SetNthEffectArea", "Ingredient", papyrusIngredient::SetNthEffectArea, registry));

	registry->RegisterFunction(
		new NativeFunction2<IngredientItem, void, UInt32, UInt32>("SetNthEffectDuration", "Ingredient", papyrusIngredient::SetNthEffectDuration, registry));

	// Array gets
	registry->RegisterFunction(
		new NativeFunction0 <IngredientItem, VMResultArray<float>>("GetEffectMagnitudes", "Ingredient", papyrusIngredient::GetEffectMagnitudes, registry));

	registry->RegisterFunction(
		new NativeFunction0 <IngredientItem, VMResultArray<UInt32>>("GetEffectAreas", "Ingredient", papyrusIngredient::GetEffectAreas, registry));

	registry->RegisterFunction(
		new NativeFunction0 <IngredientItem, VMResultArray<UInt32>>("GetEffectDurations", "Ingredient", papyrusIngredient::GetEffectDurations, registry));

	registry->RegisterFunction(
		new NativeFunction0 <IngredientItem, VMResultArray<EffectSetting*>>("GetMagicEffects", "Ingredient", papyrusIngredient::GetMagicEffects, registry));

	registry->SetFunctionFlags("Ingredient", "GetNumEffects", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Ingredient", "GetNthEffectMagnitude", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Ingredient", "GetNthEffectArea", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Ingredient", "GetNthEffectDuration", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Ingredient", "GetNthEffectMagicEffect", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Ingredient", "GetCostliestEffectIndex", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Ingredient", "GetIsNthEffectKnown", VMClassRegistry::kFunctionFlag_NoWait);

	registry->SetFunctionFlags("Ingredient", "SetNthEffectMagnitude", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Ingredient", "SetNthEffectArea", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Ingredient", "SetNthEffectDuration", VMClassRegistry::kFunctionFlag_NoWait);

	registry->SetFunctionFlags("Ingredient", "GetEffectMagnitudes", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Ingredient", "GetEffectAreas", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Ingredient", "GetEffectDurations", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Ingredient", "GetMagicEffects", VMClassRegistry::kFunctionFlag_NoWait);
}
