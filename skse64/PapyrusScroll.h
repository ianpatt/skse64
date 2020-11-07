#pragma once

class ScrollItem;
class BGSPerk;
class EffectSetting;
class VMClassRegistry;
class BGSEquipSlot;

namespace papyrusScroll
{
	void RegisterFuncs(VMClassRegistry* registry);

	float GetCastTime(ScrollItem* thisSpell);
	BGSPerk* GetPerk(ScrollItem* thisSpell);

	UInt32 GetNumEffects(ScrollItem* thisMagic);
	float GetNthEffectMagnitude(ScrollItem* thisMagic, UInt32 index);
	UInt32 GetNthEffectArea(ScrollItem* thisMagic, UInt32 index);
	UInt32 GetNthEffectDuration(ScrollItem* thisMagic, UInt32 index);
	EffectSetting* GetNthEffectMagicEffect(ScrollItem* thisMagic, UInt32 index);
	UInt32 GetCostliestEffectIndex(ScrollItem* thisMagic);

	void SetNthEffectMagnitude(ScrollItem* thisMagic, UInt32 index, float value);
	void SetNthEffectArea(ScrollItem* thisMagic, UInt32 index, UInt32 value);
	void SetNthEffectDuration(ScrollItem* thisMagic, UInt32 index, UInt32 value);

	BGSEquipSlot * GetEquipType(ScrollItem* thisMagic);
	void SetEquipType(ScrollItem* thisMagic, BGSEquipSlot * slot);
};
