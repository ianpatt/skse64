#pragma once

class TESObjectWEAP;
class VMClassRegistry;
class EnchantmentItem;
class TESObjectSTAT;
class SpellItem;
class BGSEquipSlot;

#include "GameTypes.h"

namespace papyrusWeapon
{
	void RegisterFuncs(VMClassRegistry* registry);

	UInt32 GetBaseDamage(TESObjectWEAP* thisWeapon);
	void SetBaseDamage(TESObjectWEAP* thisWeapon, UInt32 nuDamage);
	UInt32 GetCritDamage(TESObjectWEAP* thisWeapon);
	void SetCritDamage(TESObjectWEAP* thisWeapon, UInt32 critDamage);
	float GetReach(TESObjectWEAP* thisWeapon);
	void SetReach(TESObjectWEAP* thisWeapon, float nuReach);
	UInt32 GetWeaponType(TESObjectWEAP* thisWeapon);
	void SetWeaponType(TESObjectWEAP* thisWeapon, UInt32 nuType);
	float GetSpeed(TESObjectWEAP* thisWeapon);
	void SetSpeed(TESObjectWEAP* thisWeapon, float speed);
	float GetStagger(TESObjectWEAP* thisWeapon);
	void SetStagger(TESObjectWEAP* thisWeapon, float stagger);
	float GetMinRange(TESObjectWEAP* thisWeapon);
	void SetMinRange(TESObjectWEAP* thisWeapon, float minRange);
	float GetMaxRange(TESObjectWEAP* thisWeapon);
	void SetMaxRange(TESObjectWEAP* thisWeapon, float maxRange);

	BSFixedString GetModelPath(TESObjectWEAP* thisWeapon);
	void SetModelPath(TESObjectWEAP* thisWeapon, BSFixedString nuPath);

	BSFixedString GetIconPath(TESObjectWEAP* thisWeapon);
	void SetIconPath(TESObjectWEAP* thisWeapon, BSFixedString nuPath);

	BSFixedString GetMessageIconPath(TESObjectWEAP* thisWeapon);
	void SetMessageIconPath(TESObjectWEAP* thisWeapon, BSFixedString nuPath);

	EnchantmentItem* GetEnchantment(TESObjectWEAP* thisWeapon);
	void SetEnchantment(TESObjectWEAP* thisWeapon, EnchantmentItem* enchantment);

	UInt32 GetEnchantmentValue(TESObjectWEAP* thisWeapon);
	void SetEnchantmentValue(TESObjectWEAP* thisWeapon, UInt32 value);

	TESObjectSTAT * GetEquippedModel(TESObjectWEAP* thisWeapon);
	void SetEquippedModel(TESObjectWEAP* thisWeapon, TESObjectSTAT * model);

	BGSEquipSlot * GetEquipType(TESObjectWEAP* thisWeapon);
	void SetEquipType(TESObjectWEAP* thisWeapon, BGSEquipSlot * slot);

	//BSFixedString GetSkill(TESObjectWEAP* thisWeapon);
	//void SetSkill(TESObjectWEAP* thisWeapon, BSFixedString skill);

	//BSFixedString GetResist(TESObjectWEAP* thisWeapon);
	//void SetResist(TESObjectWEAP* thisWeapon, BSFixedString resist);

	SpellItem * GetCritEffect(TESObjectWEAP* thisWeapon);
	void SetCritEffect(TESObjectWEAP* thisWeapon, SpellItem * spell);

	bool GetCritEffectOnDeath(TESObjectWEAP* thisWeapon);
	void SetCritEffectOnDeath(TESObjectWEAP* thisWeapon, bool critOnDeath);

	float GetCritMultiplier(TESObjectWEAP* thisWeapon);
	void SetCritMultiplier(TESObjectWEAP* thisWeapon, float critMult);
}
