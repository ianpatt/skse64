#pragma once

class VMClassRegistry;
class TESForm;
class BaseExtraList;
class TESObjectREFR;
class BGSRefAlias;
class EnchantmentItem;
class AlchemyItem;
class Actor;
class EffectSetting;

#include "GameExtraData.h"

#include "PapyrusArgs.h"
#include "GameTypes.h"

namespace referenceUtils
{
	EquipData ResolveEquippedObject(Actor * actor, UInt32 weaponSlot, UInt32 slotMask);

	float GetItemHealthPercent(TESForm * baseForm, BaseExtraList* extraData);
	void SetItemHealthPercent(TESForm * baseForm, BaseExtraList* extraData, float value);

	float GetItemMaxCharge(TESForm * baseForm, BaseExtraList* extraData);
	void SetItemMaxCharge(TESForm * baseForm, BaseExtraList* extraData, float maxCharge);

	float GetItemCharge(TESForm * baseForm, BaseExtraList* extraData);
	void SetItemCharge(TESForm * baseForm, BaseExtraList* extraData, float value);

	BSFixedString GetDisplayName(TESForm * baseForm, BaseExtraList* extraData);
	bool SetDisplayName(BaseExtraList* extraData, BSFixedString value, bool force);

	EnchantmentItem * GetEnchantment(BaseExtraList * extraData);
	void CreateEnchantment(TESForm* baseForm, BaseExtraList * extraData, float maxCharge, VMArray<EffectSetting*> effects, VMArray<float> magnitudes, VMArray<UInt32> areas, VMArray<UInt32> durations);
	void SetEnchantment(TESForm* baseForm, BaseExtraList * extraData, EnchantmentItem * enchantment, float maxCharge);

	AlchemyItem * GetPoison(BaseExtraList * extraData);

	UInt32 GetNumReferenceAliases(BaseExtraList * extraData);
	BGSRefAlias * GetNthReferenceAlias(BaseExtraList * extraData, UInt32 n);

	VMResultArray<BGSRefAlias*> GetReferenceAliases(BaseExtraList* extraData);

	bool HasItemAbility(Actor * actor, TESForm* baseForm, BaseExtraList * extraData);
	void UpdateItemAbility(Actor * actor, TESForm* baseForm, BaseExtraList * extraData, bool bLeftHand);
};

#define WORNOBJECT_PARAMS StaticFunctionTag*, Actor * actor, UInt32 weaponSlot, UInt32 slotMask
#define WORNOBJECT_TEMPLATE Actor*, UInt32, UInt32

namespace papyrusWornObject
{
	void RegisterFuncs(VMClassRegistry* registry);

	float GetItemHealthPercent(WORNOBJECT_PARAMS);
	void SetItemHealthPercent(WORNOBJECT_PARAMS, float value);
	float GetItemMaxCharge(WORNOBJECT_PARAMS);
	void SetItemMaxCharge(WORNOBJECT_PARAMS, float maxCharge);
	float GetItemCharge(WORNOBJECT_PARAMS);

	BSFixedString GetDisplayName(WORNOBJECT_PARAMS);
	bool SetDisplayName(WORNOBJECT_PARAMS, BSFixedString value, bool force);
	EnchantmentItem * GetEnchantment(WORNOBJECT_PARAMS);
	void CreateEnchantment(WORNOBJECT_PARAMS, float maxCharge, VMArray<EffectSetting*> effects, VMArray<float> magnitudes, VMArray<UInt32> areas, VMArray<UInt32> durations);
	void SetEnchantment(WORNOBJECT_PARAMS, EnchantmentItem * enchantment, float maxCharge);
	UInt32 GetNumReferenceAliases(WORNOBJECT_PARAMS);
	BGSRefAlias * GetNthReferenceAlias(WORNOBJECT_PARAMS, UInt32 n);
}