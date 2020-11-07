#include "PapyrusPotion.h"
#include "PapyrusSpell.h"
#include "GameObjects.h"

namespace papyrusPotion
{
	bool IsFood(AlchemyItem* thisPotion)
	{
		return (thisPotion) ? thisPotion->IsFood() : false;
	}

	bool IsPoison(AlchemyItem* thisPotion)
	{
		return (thisPotion) ? thisPotion->IsPoison() : false;
	}

	BGSSoundDescriptorForm* GetUseSound(AlchemyItem* thisPotion)
	{
		return (thisPotion) ? thisPotion->itemData.useSound : NULL;
	}

	UInt32 GetNumEffects(AlchemyItem* thisMagic)
	{ return magicItemUtils::GetNumEffects(thisMagic); }

	float GetNthEffectMagnitude(AlchemyItem* thisMagic, UInt32 index)
	{ return magicItemUtils::GetNthEffectMagnitude(thisMagic, index); }

	UInt32 GetNthEffectArea(AlchemyItem* thisMagic, UInt32 index)
	{ return magicItemUtils::GetNthEffectArea(thisMagic, index); }

	UInt32 GetNthEffectDuration(AlchemyItem* thisMagic, UInt32 index)
	{ return magicItemUtils::GetNthEffectDuration(thisMagic, index); }

	EffectSetting* GetNthEffectMagicEffect(AlchemyItem* thisMagic, UInt32 index)
	{ return magicItemUtils::GetNthEffectMagicEffect(thisMagic, index); }

	UInt32 GetCostliestEffectIndex(AlchemyItem* thisMagic)
	{ return magicItemUtils::GetCostliestEffectIndex(thisMagic); }

	void SetNthEffectMagnitude(AlchemyItem* thisMagic, UInt32 index, float value)
	{ magicItemUtils::SetNthEffectMagnitude(thisMagic, index, value); }

	void SetNthEffectArea(AlchemyItem* thisMagic, UInt32 index, UInt32 value)
	{ magicItemUtils::SetNthEffectArea(thisMagic, index, value); }

	void SetNthEffectDuration(AlchemyItem* thisMagic, UInt32 index, UInt32 value)
	{ magicItemUtils::SetNthEffectDuration(thisMagic, index, value); }

	VMResultArray<float> GetEffectMagnitudes(AlchemyItem* thisMagic) { return magicItemUtils::GetEffectMagnitudes(thisMagic); }
	VMResultArray<UInt32> GetEffectAreas(AlchemyItem* thisMagic) { return magicItemUtils::GetEffectAreas(thisMagic); }
	VMResultArray<UInt32> GetEffectDurations(AlchemyItem* thisMagic) { return magicItemUtils::GetEffectDurations(thisMagic); }
	VMResultArray<EffectSetting*> GetMagicEffects(AlchemyItem* thisMagic) { return magicItemUtils::GetMagicEffects(thisMagic); }
}

#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

void papyrusPotion::RegisterFuncs(VMClassRegistry* registry)
{
	registry->RegisterFunction(
		new NativeFunction0 <AlchemyItem, bool>("IsFood", "Potion", papyrusPotion::IsFood, registry));

	registry->RegisterFunction(
		new NativeFunction0 <AlchemyItem, bool>("IsPoison", "Potion", papyrusPotion::IsPoison, registry));

	registry->RegisterFunction(
		new NativeFunction0<AlchemyItem, UInt32>("GetNumEffects", "Potion", papyrusPotion::GetNumEffects, registry));

	registry->RegisterFunction(
		new NativeFunction1<AlchemyItem, float, UInt32>("GetNthEffectMagnitude", "Potion", papyrusPotion::GetNthEffectMagnitude, registry));

	registry->RegisterFunction(
		new NativeFunction1<AlchemyItem, UInt32, UInt32>("GetNthEffectArea", "Potion", papyrusPotion::GetNthEffectArea, registry));

	registry->RegisterFunction(
		new NativeFunction1<AlchemyItem, UInt32, UInt32>("GetNthEffectDuration", "Potion", papyrusPotion::GetNthEffectDuration, registry));

	registry->RegisterFunction(
		new NativeFunction1<AlchemyItem, EffectSetting*, UInt32>("GetNthEffectMagicEffect", "Potion", papyrusPotion::GetNthEffectMagicEffect, registry));

	registry->RegisterFunction(
		new NativeFunction0<AlchemyItem, UInt32>("GetCostliestEffectIndex", "Potion", papyrusPotion::GetCostliestEffectIndex, registry));

	registry->RegisterFunction(
		new NativeFunction0<AlchemyItem, BGSSoundDescriptorForm*>("GetUseSound", "Potion", papyrusPotion::GetUseSound, registry));

	// Sets
	registry->RegisterFunction(
		new NativeFunction2<AlchemyItem, void, UInt32, float>("SetNthEffectMagnitude", "Potion", papyrusPotion::SetNthEffectMagnitude, registry));

	registry->RegisterFunction(
		new NativeFunction2<AlchemyItem, void, UInt32, UInt32>("SetNthEffectArea", "Potion", papyrusPotion::SetNthEffectArea, registry));

	registry->RegisterFunction(
		new NativeFunction2<AlchemyItem, void, UInt32, UInt32>("SetNthEffectDuration", "Potion", papyrusPotion::SetNthEffectDuration, registry));

	// Array gets
	registry->RegisterFunction(
		new NativeFunction0 <AlchemyItem, VMResultArray<float>>("GetEffectMagnitudes", "Potion", papyrusPotion::GetEffectMagnitudes, registry));

	registry->RegisterFunction(
		new NativeFunction0 <AlchemyItem, VMResultArray<UInt32>>("GetEffectAreas", "Potion", papyrusPotion::GetEffectAreas, registry));

	registry->RegisterFunction(
		new NativeFunction0 <AlchemyItem, VMResultArray<UInt32>>("GetEffectDurations", "Potion", papyrusPotion::GetEffectDurations, registry));

	registry->RegisterFunction(
		new NativeFunction0 <AlchemyItem, VMResultArray<EffectSetting*>>("GetMagicEffects", "Potion", papyrusPotion::GetMagicEffects, registry));

	registry->SetFunctionFlags("Potion", "IsFood", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Potion", "GetNumEffects", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Potion", "GetNthEffectMagnitude", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Potion", "GetNthEffectArea", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Potion", "GetNthEffectDuration", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Potion", "GetNthEffectMagicEffect", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Potion", "GetCostliestEffectIndex", VMClassRegistry::kFunctionFlag_NoWait);

	registry->SetFunctionFlags("Potion", "SetNthEffectMagnitude", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Potion", "SetNthEffectArea", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Potion", "SetNthEffectDuration", VMClassRegistry::kFunctionFlag_NoWait);

	registry->SetFunctionFlags("Potion", "GetEffectMagnitudes", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Potion", "GetEffectAreas", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Potion", "GetEffectDurations", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Potion", "GetMagicEffects", VMClassRegistry::kFunctionFlag_NoWait);
}
