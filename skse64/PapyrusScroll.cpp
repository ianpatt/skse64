#include "PapyrusScroll.h"
#include "PapyrusSpell.h"
#include "GameObjects.h"

namespace papyrusScroll
{
	float GetCastTime(ScrollItem* thisSpell)
	{
		return (thisSpell)? thisSpell->data.castTime : 0.0;
	}

	BGSPerk* GetPerk(ScrollItem* thisSpell)
	{
		return (thisSpell) ? thisSpell->data.spellPerk : NULL;
	}

	UInt32 GetNumEffects(ScrollItem* thisMagic)
	{ return magicItemUtils::GetNumEffects(thisMagic); }

	float GetNthEffectMagnitude(ScrollItem* thisMagic, UInt32 index)
	{ return magicItemUtils::GetNthEffectMagnitude(thisMagic, index); }

	UInt32 GetNthEffectArea(ScrollItem* thisMagic, UInt32 index)
	{ return magicItemUtils::GetNthEffectArea(thisMagic, index); }

	UInt32 GetNthEffectDuration(ScrollItem* thisMagic, UInt32 index)
	{ return magicItemUtils::GetNthEffectDuration(thisMagic, index); }

	EffectSetting* GetNthEffectMagicEffect(ScrollItem* thisMagic, UInt32 index)
	{ return magicItemUtils::GetNthEffectMagicEffect(thisMagic, index); }

	UInt32 GetCostliestEffectIndex(ScrollItem* thisMagic)
	{ return magicItemUtils::GetCostliestEffectIndex(thisMagic); }

	void SetNthEffectMagnitude(ScrollItem* thisMagic, UInt32 index, float value)
	{ magicItemUtils::SetNthEffectMagnitude(thisMagic, index, value); }

	void SetNthEffectArea(ScrollItem* thisMagic, UInt32 index, UInt32 value)
	{ magicItemUtils::SetNthEffectArea(thisMagic, index, value); }

	void SetNthEffectDuration(ScrollItem* thisMagic, UInt32 index, UInt32 value)
	{ magicItemUtils::SetNthEffectDuration(thisMagic, index, value); }

	BGSEquipSlot * GetEquipType(ScrollItem* thisMagic)
	{
		if (thisMagic) {
			return thisMagic->equipType.GetEquipSlot();
		}

		// Invalid EquipSlot
		return NULL;
	}

	void SetEquipType(ScrollItem* thisMagic, BGSEquipSlot * slot)
	{
		if (thisMagic && slot) {
			thisMagic->equipType.SetEquipSlot(slot);
		}
	}

	VMResultArray<float> GetEffectMagnitudes(ScrollItem* thisMagic) { return magicItemUtils::GetEffectMagnitudes(thisMagic); }
	VMResultArray<UInt32> GetEffectAreas(ScrollItem* thisMagic) { return magicItemUtils::GetEffectAreas(thisMagic); }
	VMResultArray<UInt32> GetEffectDurations(ScrollItem* thisMagic) { return magicItemUtils::GetEffectDurations(thisMagic); }
	VMResultArray<EffectSetting*> GetMagicEffects(ScrollItem* thisMagic) { return magicItemUtils::GetMagicEffects(thisMagic); }
}

#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

void papyrusScroll::RegisterFuncs(VMClassRegistry* registry)
{
	registry->RegisterFunction(
		new NativeFunction0<ScrollItem, float>("GetCastTime", "Scroll", papyrusScroll::GetCastTime, registry));

	registry->RegisterFunction(
		new NativeFunction0<ScrollItem, UInt32>("GetNumEffects", "Scroll", papyrusScroll::GetNumEffects, registry));

	registry->RegisterFunction(
		new NativeFunction1<ScrollItem, float, UInt32>("GetNthEffectMagnitude", "Scroll", papyrusScroll::GetNthEffectMagnitude, registry));

	registry->RegisterFunction(
		new NativeFunction1<ScrollItem, UInt32, UInt32>("GetNthEffectArea", "Scroll", papyrusScroll::GetNthEffectArea, registry));

	registry->RegisterFunction(
		new NativeFunction1<ScrollItem, UInt32, UInt32>("GetNthEffectDuration", "Scroll", papyrusScroll::GetNthEffectDuration, registry));

	registry->RegisterFunction(
		new NativeFunction1<ScrollItem, EffectSetting*, UInt32>("GetNthEffectMagicEffect", "Scroll", papyrusScroll::GetNthEffectMagicEffect, registry));

	registry->RegisterFunction(
		new NativeFunction0<ScrollItem, UInt32>("GetCostliestEffectIndex", "Scroll", papyrusScroll::GetCostliestEffectIndex, registry));

	// Sets
	registry->RegisterFunction(
		new NativeFunction2<ScrollItem, void, UInt32, float>("SetNthEffectMagnitude", "Scroll", papyrusScroll::SetNthEffectMagnitude, registry));

	registry->RegisterFunction(
		new NativeFunction2<ScrollItem, void, UInt32, UInt32>("SetNthEffectArea", "Scroll", papyrusScroll::SetNthEffectArea, registry));

	registry->RegisterFunction(
		new NativeFunction2<ScrollItem, void, UInt32, UInt32>("SetNthEffectDuration", "Scroll", papyrusScroll::SetNthEffectDuration, registry));

	// EquipType
	registry->RegisterFunction(
		new NativeFunction0 <ScrollItem, BGSEquipSlot*>("GetEquipType", "Scroll", papyrusScroll::GetEquipType, registry));

	registry->RegisterFunction(
		new NativeFunction1 <ScrollItem, void, BGSEquipSlot*>("SetEquipType", "Scroll", papyrusScroll::SetEquipType, registry));

	// Array gets
	registry->RegisterFunction(
		new NativeFunction0 <ScrollItem, VMResultArray<float>>("GetEffectMagnitudes", "Scroll", papyrusScroll::GetEffectMagnitudes, registry));

	registry->RegisterFunction(
		new NativeFunction0 <ScrollItem, VMResultArray<UInt32>>("GetEffectAreas", "Scroll", papyrusScroll::GetEffectAreas, registry));

	registry->RegisterFunction(
		new NativeFunction0 <ScrollItem, VMResultArray<UInt32>>("GetEffectDurations", "Scroll", papyrusScroll::GetEffectDurations, registry));

	registry->RegisterFunction(
		new NativeFunction0 <ScrollItem, VMResultArray<EffectSetting*>>("GetMagicEffects", "Scroll", papyrusScroll::GetMagicEffects, registry));

	registry->SetFunctionFlags("Scroll", "GetCastTime", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Scroll", "GetNumEffects", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Scroll", "GetNthEffectMagnitude", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Scroll", "GetNthEffectArea", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Scroll", "GetNthEffectDuration", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Scroll", "GetNthEffectMagicEffect", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Scroll", "GetCostliestEffectIndex", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Scroll", "GetEquipType", VMClassRegistry::kFunctionFlag_NoWait);

	registry->SetFunctionFlags("Scroll", "SetNthEffectMagnitude", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Scroll", "SetNthEffectArea", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Scroll", "SetNthEffectDuration", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Scroll", "SetEquipType", VMClassRegistry::kFunctionFlag_NoWait);

	registry->SetFunctionFlags("Scroll", "GetEffectMagnitudes", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Scroll", "GetEffectAreas", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Scroll", "GetEffectDurations", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Scroll", "GetMagicEffects", VMClassRegistry::kFunctionFlag_NoWait);
}
