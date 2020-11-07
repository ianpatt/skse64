#include "PapyrusSpell.h"

#include "GameObjects.h"
#include "GameReferences.h"

namespace papyrusSpell
{
	float GetCastTime(SpellItem* thisSpell)
	{
		return (thisSpell) ? thisSpell->data.castTime : 0.0;
	}

	BGSPerk* GetPerk(SpellItem* thisSpell)
	{
		return (thisSpell) ? thisSpell->data.spellPerk : NULL;
	}

	UInt32 GetNumEffects(SpellItem* thisMagic)
	{ return magicItemUtils::GetNumEffects(thisMagic); }

	float GetNthEffectMagnitude(SpellItem* thisMagic, UInt32 index)
	{ return magicItemUtils::GetNthEffectMagnitude(thisMagic, index); }

	UInt32 GetNthEffectArea(SpellItem* thisMagic, UInt32 index)
	{ return magicItemUtils::GetNthEffectArea(thisMagic, index); }

	UInt32 GetNthEffectDuration(SpellItem* thisMagic, UInt32 index)
	{ return magicItemUtils::GetNthEffectDuration(thisMagic, index); }

	EffectSetting* GetNthEffectMagicEffect(SpellItem* thisMagic, UInt32 index)
	{ return magicItemUtils::GetNthEffectMagicEffect(thisMagic, index); }

	UInt32 GetCostliestEffectIndex(SpellItem* thisMagic)
	{ return magicItemUtils::GetCostliestEffectIndex(thisMagic); }

	UInt32 GetMagickaCost(SpellItem* thisSpell)
	{
		return thisSpell ? thisSpell->GetMagickaCost() : 0;
	}

	UInt32 GetEffectiveMagickaCost(SpellItem* thisSpell, Character* caster)
	{
		return thisSpell ? CALL_MEMBER_FN(thisSpell,GetEffectiveMagickaCost)(caster) : 0;
	}

	void SetNthEffectMagnitude(SpellItem* thisMagic, UInt32 index, float value)
	{ magicItemUtils::SetNthEffectMagnitude(thisMagic, index, value); }

	void SetNthEffectArea(SpellItem* thisMagic, UInt32 index, UInt32 value)
	{ magicItemUtils::SetNthEffectArea(thisMagic, index, value); }

	void SetNthEffectDuration(SpellItem* thisMagic, UInt32 index, UInt32 value)
	{ magicItemUtils::SetNthEffectDuration(thisMagic, index, value); }

	BGSEquipSlot * GetEquipType(SpellItem* thisMagic)
	{
		if (thisMagic) {
			return thisMagic->equipType.GetEquipSlot();
		}

		// Invalid EquipSlot
		return NULL;
	}

	void SetEquipType(SpellItem* thisMagic, BGSEquipSlot * slot)
	{
		if (thisMagic && slot) {
			thisMagic->equipType.SetEquipSlot(slot);
		}
	}

	VMResultArray<float> GetEffectMagnitudes(SpellItem* thisMagic) { return magicItemUtils::GetEffectMagnitudes(thisMagic); }
	VMResultArray<UInt32> GetEffectAreas(SpellItem* thisMagic) { return magicItemUtils::GetEffectAreas(thisMagic); }
	VMResultArray<UInt32> GetEffectDurations(SpellItem* thisMagic) { return magicItemUtils::GetEffectDurations(thisMagic); }
	VMResultArray<EffectSetting*> GetMagicEffects(SpellItem* thisMagic) { return magicItemUtils::GetMagicEffects(thisMagic); }
}

namespace magicItemUtils
{
	UInt32 GetNumEffects(MagicItem* thisMagic)
	{
		return thisMagic ? thisMagic->effectItemList.count : 0;
	}

	float GetNthEffectMagnitude(MagicItem* thisMagic, UInt32 index)
	{
		if (!thisMagic)
			return 0.0;

		MagicItem::EffectItem* pEI = NULL;
		thisMagic->effectItemList.GetNthItem(index, pEI);
		return (pEI) ? pEI->magnitude : 0.0;
	}

	UInt32 GetNthEffectArea(MagicItem* thisMagic, UInt32 index)
	{
		if (!thisMagic)
			return 0;

		MagicItem::EffectItem* pEI = NULL;
		thisMagic->effectItemList.GetNthItem(index, pEI);
		return (pEI) ? pEI->area : 0;
	}

	UInt32 GetNthEffectDuration(MagicItem* thisMagic, UInt32 index)
	{
		if (!thisMagic)
			return 0;

		MagicItem::EffectItem* pEI = NULL;
		thisMagic->effectItemList.GetNthItem(index, pEI);
		return (pEI) ? pEI->duration : 0;
	}

	EffectSetting* GetNthEffectMagicEffect(MagicItem* thisMagic, UInt32 index)
	{
		if (!thisMagic)
			return NULL;

		MagicItem::EffectItem* pEI = NULL;
		thisMagic->effectItemList.GetNthItem(index, pEI);
		return (pEI) ? pEI->mgef : NULL;
	}

	void SetNthEffectMagnitude(MagicItem* thisMagic, UInt32 index, float value)
	{
		if(thisMagic)
		{
			MagicItem::EffectItem* pEI = NULL;
			thisMagic->effectItemList.GetNthItem(index, pEI);
			if(pEI)
				pEI->magnitude = value;
		}
	}

	void SetNthEffectArea(MagicItem* thisMagic, UInt32 index, UInt32 value)
	{
		if (thisMagic) {
			MagicItem::EffectItem* pEI = NULL;
			thisMagic->effectItemList.GetNthItem(index, pEI);
			if(pEI)
				pEI->area = value;
		}		
	}

	void SetNthEffectDuration(MagicItem* thisMagic, UInt32 index, UInt32 value)
	{
		if (thisMagic) {
			MagicItem::EffectItem* pEI = NULL;
			thisMagic->effectItemList.GetNthItem(index, pEI);
			if(pEI)
				pEI->duration = value;
		}
	}

	UInt32 GetCostliestEffectIndex(MagicItem* thisMagic)
	{
		if (!thisMagic)
			return 0;

		MagicItem::EffectItem * pEI = CALL_MEMBER_FN(thisMagic, GetCostliestEffectItem)(5, false);
		return pEI ? thisMagic->effectItemList.GetItemIndex(pEI) : 0;
	}

	VMResultArray<float> GetEffectMagnitudes(MagicItem* thisMagic)
	{
		VMResultArray<float> result;
		for(UInt32 i = 0; i < thisMagic->effectItemList.count; i++)
		{
			MagicItem::EffectItem* pEI = NULL;
			thisMagic->effectItemList.GetNthItem(i, pEI);
			if(pEI)
				result.push_back(pEI->magnitude);
			else
				result.push_back(0.0f);
		}
		return result;
	}
	VMResultArray<UInt32> GetEffectAreas(MagicItem* thisMagic)
	{
		VMResultArray<UInt32> result;
		for(UInt32 i = 0; i < thisMagic->effectItemList.count; i++)
		{
			MagicItem::EffectItem* pEI = NULL;
			thisMagic->effectItemList.GetNthItem(i, pEI);
			if(pEI)
				result.push_back(pEI->area);
			else
				result.push_back(0);
		}
		return result;
	}
	VMResultArray<UInt32> GetEffectDurations(MagicItem* thisMagic)
	{
		VMResultArray<UInt32> result;
		for(UInt32 i = 0; i < thisMagic->effectItemList.count; i++)
		{
			MagicItem::EffectItem* pEI = NULL;
			thisMagic->effectItemList.GetNthItem(i, pEI);
			if(pEI)
				result.push_back(pEI->duration);
			else
				result.push_back(0);
		}
		return result;
	}
	VMResultArray<EffectSetting*> GetMagicEffects(MagicItem* thisMagic)
	{
		VMResultArray<EffectSetting*> result;
		for(UInt32 i = 0; i < thisMagic->effectItemList.count; i++)
		{
			MagicItem::EffectItem* pEI = NULL;
			thisMagic->effectItemList.GetNthItem(i, pEI);
			if(pEI)
				result.push_back(pEI->mgef);
			else
				result.push_back(NULL);
		}
		return result;
	}
}

#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

void papyrusSpell::RegisterFuncs(VMClassRegistry* registry)
{
	registry->RegisterFunction(
		new NativeFunction0<SpellItem, float>("GetCastTime", "Spell", papyrusSpell::GetCastTime, registry));

	registry->RegisterFunction(
		new NativeFunction0<SpellItem, BGSPerk*>("GetPerk", "Spell", papyrusSpell::GetPerk, registry));

	registry->RegisterFunction(
		new NativeFunction0<SpellItem, UInt32>("GetNumEffects", "Spell", papyrusSpell::GetNumEffects, registry));

	registry->RegisterFunction(
		new NativeFunction1<SpellItem, float, UInt32>("GetNthEffectMagnitude", "Spell", papyrusSpell::GetNthEffectMagnitude, registry));

	registry->RegisterFunction(
		new NativeFunction1<SpellItem, UInt32, UInt32>("GetNthEffectArea", "Spell", papyrusSpell::GetNthEffectArea, registry));

	registry->RegisterFunction(
		new NativeFunction1<SpellItem, UInt32, UInt32>("GetNthEffectDuration", "Spell", papyrusSpell::GetNthEffectDuration, registry));

	registry->RegisterFunction(
		new NativeFunction1<SpellItem, EffectSetting*, UInt32>("GetNthEffectMagicEffect", "Spell", papyrusSpell::GetNthEffectMagicEffect, registry));

	registry->RegisterFunction(
		new NativeFunction0<SpellItem, UInt32>("GetCostliestEffectIndex", "Spell", papyrusSpell::GetCostliestEffectIndex, registry));

	registry->RegisterFunction(
		new NativeFunction0<SpellItem, UInt32>("GetMagickaCost", "Spell", papyrusSpell::GetMagickaCost, registry));

	registry->RegisterFunction(
		new NativeFunction1<SpellItem, UInt32, Character*>("GetEffectiveMagickaCost", "Spell", papyrusSpell::GetEffectiveMagickaCost, registry));

	// Sets
	registry->RegisterFunction(
		new NativeFunction2<SpellItem, void, UInt32, float>("SetNthEffectMagnitude", "Spell", papyrusSpell::SetNthEffectMagnitude, registry));

	registry->RegisterFunction(
		new NativeFunction2<SpellItem, void, UInt32, UInt32>("SetNthEffectArea", "Spell", papyrusSpell::SetNthEffectArea, registry));

	registry->RegisterFunction(
		new NativeFunction2<SpellItem, void, UInt32, UInt32>("SetNthEffectDuration", "Spell", papyrusSpell::SetNthEffectDuration, registry));

	// EquipType
	registry->RegisterFunction(
		new NativeFunction0 <SpellItem, BGSEquipSlot*>("GetEquipType", "Spell", papyrusSpell::GetEquipType, registry));

	registry->RegisterFunction(
		new NativeFunction1 <SpellItem, void, BGSEquipSlot*>("SetEquipType", "Spell", papyrusSpell::SetEquipType, registry));

	// Array gets
	registry->RegisterFunction(
		new NativeFunction0 <SpellItem, VMResultArray<float>>("GetEffectMagnitudes", "Spell", papyrusSpell::GetEffectMagnitudes, registry));

	registry->RegisterFunction(
		new NativeFunction0 <SpellItem, VMResultArray<UInt32>>("GetEffectAreas", "Spell", papyrusSpell::GetEffectAreas, registry));

	registry->RegisterFunction(
		new NativeFunction0 <SpellItem, VMResultArray<UInt32>>("GetEffectDurations", "Spell", papyrusSpell::GetEffectDurations, registry));

	registry->RegisterFunction(
		new NativeFunction0 <SpellItem, VMResultArray<EffectSetting*>>("GetMagicEffects", "Spell", papyrusSpell::GetMagicEffects, registry));

	registry->SetFunctionFlags("Spell", "GetCastTime", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Spell", "GetPerk", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Spell", "GetNumEffects", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Spell", "GetNthEffectMagnitude", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Spell", "GetNthEffectArea", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Spell", "GetNthEffectDuration", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Spell", "GetNthEffectMagicEffect", VMClassRegistry::kFunctionFlag_NoWait);
//	registry->SetFunctionFlags("Spell", "GetCostliestEffectIndex", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Spell", "GetMagickaCost", VMClassRegistry::kFunctionFlag_NoWait);
//	registry->SetFunctionFlags("Spell", "GetEffectiveMagickaCost", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Spell", "GetEquipType", VMClassRegistry::kFunctionFlag_NoWait);

	registry->SetFunctionFlags("Spell", "SetNthEffectMagnitude", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Spell", "SetNthEffectArea", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Spell", "SetNthEffectDuration", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Spell", "SetEquipType", VMClassRegistry::kFunctionFlag_NoWait);

	registry->SetFunctionFlags("Spell", "GetEffectMagnitudes", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Spell", "GetEffectAreas", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Spell", "GetEffectDurations", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Spell", "GetMagicEffects", VMClassRegistry::kFunctionFlag_NoWait);
}