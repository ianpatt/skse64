#pragma once

#include "PapyrusArgs.h"

class SpellItem;
class MagicItem;
class BGSPerk;
class EffectSetting;
class VMClassRegistry;
class Character;
class BGSEquipSlot;

namespace papyrusSpell
{
	void RegisterFuncs(VMClassRegistry* registry);

	float GetCastTime(SpellItem* thisSpell);
	BGSPerk* GetPerk(SpellItem* thisSpell);

	// MagicItem funcs
	UInt32 GetNumEffects(SpellItem* thisMagic);
	float GetNthEffectMagnitude(SpellItem* thisMagic, UInt32 index);
	UInt32 GetNthEffectArea(SpellItem* thisMagic, UInt32 index);
	UInt32 GetNthEffectDuration(SpellItem* thisMagic, UInt32 index);
	EffectSetting* GetNthEffectMagicEffect(SpellItem* thisMagic, UInt32 index);
	UInt32 GetCostliestEffectIndex(SpellItem* thisMagic);
	UInt32 GetMagickaCost(SpellItem* thisMagic);
	UInt32 GetEffectiveMagickaCost(SpellItem* thisSpell, Character* caster);

	void SetNthEffectMagnitude(SpellItem* thisSpell, UInt32 index, float value);
	void SetNthEffectArea(SpellItem* thisSpell, UInt32 index, UInt32 value);
	void SetNthEffectDuration(SpellItem* thisSpell, UInt32 index, UInt32 value);

	BGSEquipSlot * GetEquipType(SpellItem* thisMagic);
	void SetEquipType(SpellItem* thisMagic, BGSEquipSlot * n);
};

namespace magicItemUtils {
	UInt32 GetNumEffects(MagicItem* thisMagic);
	float GetNthEffectMagnitude(MagicItem* thisMagic, UInt32 index);
	UInt32 GetNthEffectArea(MagicItem* thisMagic, UInt32 index);
	UInt32 GetNthEffectDuration(MagicItem* thisMagic, UInt32 index);
	EffectSetting* GetNthEffectMagicEffect(MagicItem* thisMagic, UInt32 index);
	UInt32 GetCostliestEffectIndex(MagicItem* thisMagic);

	void SetNthEffectMagnitude(MagicItem* thisMagic, UInt32 index, float value);
	void SetNthEffectArea(MagicItem* thisMagic, UInt32 index, UInt32 value);
	void SetNthEffectDuration(MagicItem* thisMagic, UInt32 index, UInt32 value);

	VMResultArray<float> GetEffectMagnitudes(MagicItem* thisMagic);
	VMResultArray<UInt32> GetEffectAreas(MagicItem* thisMagic);
	VMResultArray<UInt32> GetEffectDurations(MagicItem* thisMagic);
	VMResultArray<EffectSetting*> GetMagicEffects(MagicItem* thisMagic);
}
