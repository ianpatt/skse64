#include "PapyrusWeapon.h"

#include "GameObjects.h"
#include "GameData.h"

namespace papyrusWeapon
{
	UInt32 GetBaseDamage(TESObjectWEAP* thisWeapon)
	{
		return (thisWeapon) ? thisWeapon->damage.GetAttackDamage() : 0;
	}

	void SetBaseDamage(TESObjectWEAP* thisWeapon, UInt32 nuDamage)
	{
		if (thisWeapon) {
			thisWeapon->damage.attackDamage = nuDamage;
		}
	}

	UInt32 GetCritDamage(TESObjectWEAP* thisWeapon)
	{
		return (thisWeapon) ? thisWeapon->critDamage() : 0;
	}

	void SetCritDamage(TESObjectWEAP* thisWeapon, UInt32 critDamage)
	{
		if (thisWeapon) {
			thisWeapon->critData.critDamage = critDamage;
		}
	}

	float GetReach(TESObjectWEAP* thisWeapon)
	{
		return (thisWeapon) ? thisWeapon->reach() : 0;
	}

	void SetReach(TESObjectWEAP* thisWeapon, float nuReach)
	{
		if(thisWeapon) {
			thisWeapon->gameData.reach = nuReach;
		}
	}

	float GetSpeed(TESObjectWEAP* thisWeapon)
	{
		return (thisWeapon) ? thisWeapon->speed() : 0.0;
	}

	void SetSpeed(TESObjectWEAP* thisWeapon, float speed)
	{
		if (thisWeapon) {
			thisWeapon->gameData.speed = speed;
		}
	}

	float GetStagger(TESObjectWEAP* thisWeapon)
	{
		return (thisWeapon) ? thisWeapon->stagger() : 0.0;
	}

	void SetStagger(TESObjectWEAP* thisWeapon, float stagger)
	{
		if (thisWeapon) {
			thisWeapon->gameData.stagger = stagger;
		}
	}

	float GetMinRange(TESObjectWEAP* thisWeapon)
	{
		return (thisWeapon) ? thisWeapon->minRange() : 0.0;
	}

	void SetMinRange(TESObjectWEAP* thisWeapon, float minRange)
	{
		if (thisWeapon) {
			thisWeapon->gameData.minRange = minRange;
		}
	}

	float GetMaxRange(TESObjectWEAP* thisWeapon)
	{
		return (thisWeapon) ? thisWeapon->maxRange() : 0.0;
	}

	void SetMaxRange(TESObjectWEAP* thisWeapon, float maxRange)
	{
		if (thisWeapon) {
			thisWeapon->gameData.maxRange = maxRange;
		}
	}

	UInt32 GetWeaponType(TESObjectWEAP* thisWeapon)
	{
		return (thisWeapon) ? thisWeapon->type() : 0;
	}

	void SetWeaponType(TESObjectWEAP* thisWeapon, UInt32 nuType)
	{
		if(thisWeapon) {
			thisWeapon->gameData.type = nuType;
	}
	}


	BSFixedString GetModelPath(TESObjectWEAP* thisWeapon)
	{
		return (thisWeapon) ? thisWeapon->texSwap.GetModelName() : NULL;
	}

	void SetModelPath(TESObjectWEAP* thisWeapon, BSFixedString nuPath)
	{
		if(thisWeapon) {
		thisWeapon->texSwap.SetModelName(nuPath.data);
	}
	}

	BSFixedString GetIconPath(TESObjectWEAP* thisWeapon)
	{
		return (thisWeapon) ? thisWeapon->icon.str : NULL;
	}

	void SetIconPath(TESObjectWEAP* thisWeapon, BSFixedString nuPath)
	{
		if(thisWeapon) {
		thisWeapon->icon.str = nuPath.data;
	}
	}

	BSFixedString GetMessageIconPath(TESObjectWEAP* thisWeapon)
	{
		return (thisWeapon) ? thisWeapon->messageIcon.icon.str : NULL;
	}

	void SetMessageIconPath(TESObjectWEAP* thisWeapon, BSFixedString nuPath)
	{
		if(thisWeapon) {
		thisWeapon->messageIcon.icon.str = nuPath.data;
	}
	}

	EnchantmentItem* GetEnchantment(TESObjectWEAP* thisWeapon)
	{
		return (thisWeapon) ? thisWeapon->enchantable.enchantment : NULL;
	}

	void SetEnchantment(TESObjectWEAP* thisWeapon, EnchantmentItem* enchantment)
	{
		if (thisWeapon) {
			thisWeapon->enchantable.enchantment = enchantment;
		}
	}

	UInt32 GetEnchantmentValue(TESObjectWEAP* thisWeapon)
	{
		return (thisWeapon && thisWeapon->enchantable.enchantment) ? thisWeapon->enchantable.maxCharge : 0;
	}

	void SetEnchantmentValue(TESObjectWEAP* thisWeapon, UInt32 value)
	{
		if (thisWeapon && thisWeapon->enchantable.enchantment) {
			thisWeapon->enchantable.maxCharge = value;
		}
	}

	TESObjectSTAT * GetEquippedModel(TESObjectWEAP* thisWeapon)
	{
		return (thisWeapon) ? thisWeapon->model : NULL;
	}

	void SetEquippedModel(TESObjectWEAP* thisWeapon, TESObjectSTAT * model)
	{
		if (thisWeapon) {
			thisWeapon->model = model;
		}
	}

	BGSEquipSlot * GetEquipType(TESObjectWEAP* thisWeapon)
	{
		if (thisWeapon) {
			return thisWeapon->equipType.equipSlot;
//			return thisWeapon->equipType.GetEquipSlot();
		}

		// Invalid EquipSlot
		return NULL;
	}

	void SetEquipType(TESObjectWEAP* thisWeapon, BGSEquipSlot * slot)
	{
		if (thisWeapon && slot) {
			thisWeapon->equipType.SetEquipSlot(slot);
		}
	}


	BSFixedString GetSkill(TESObjectWEAP* thisWeapon)
	{
		if(!thisWeapon)
			return "";

		ActorValueList * avList = ActorValueList::GetSingleton();
		if(!avList)
			return "";
		
		ActorValueInfo * info = avList->GetActorValue(thisWeapon->gameData.skill);
		if(!info)
			return "";

		return info->name;
	}

	void SetSkill(TESObjectWEAP* thisWeapon, BSFixedString skill)
	{
		if(thisWeapon) {
			UInt32 actorValue = ActorValueList::ResolveActorValueByName(skill.data);
			thisWeapon->gameData.skill = actorValue;
		}
	}

	BSFixedString GetResist(TESObjectWEAP* thisWeapon)
	{
		if(!thisWeapon)
			return "";

		ActorValueList * avList = ActorValueList::GetSingleton();
		if(!avList)
			return "";

		ActorValueInfo * info = avList->GetActorValue(thisWeapon->gameData.resist);
		if(!info)
			return "";

		return info->name;
	}

	void SetResist(TESObjectWEAP* thisWeapon, BSFixedString resist)
	{
		if(thisWeapon) {
			UInt32 actorValue = ActorValueList::ResolveActorValueByName(resist.data);
			thisWeapon->gameData.resist = actorValue;
		}
	}

	SpellItem * GetCritEffect(TESObjectWEAP* thisWeapon)
	{
		return (thisWeapon) ? thisWeapon->critData.critEffect : NULL;
	}

	void SetCritEffect(TESObjectWEAP* thisWeapon, SpellItem * spell)
	{
		if(thisWeapon) {
			thisWeapon->critData.critEffect = spell;
		}
	}

	bool GetCritEffectOnDeath(TESObjectWEAP* thisWeapon)
	{
		return (thisWeapon) ? (thisWeapon->critData.effectOnDeath != 0) : false;
	}

	void SetCritEffectOnDeath(TESObjectWEAP* thisWeapon, bool critOnDeath)
	{
		if(thisWeapon) {
			thisWeapon->critData.effectOnDeath = (UInt8)critOnDeath;
		}
	}

	float GetCritMultiplier(TESObjectWEAP* thisWeapon)
	{
		return (thisWeapon) ? thisWeapon->critData.critMult : 0.0;
	}

	void SetCritMultiplier(TESObjectWEAP* thisWeapon, float critMult)
	{
		if(thisWeapon) {
			thisWeapon->critData.critMult = critMult;
		}
	}

	TESObjectWEAP* GetTemplate(TESObjectWEAP* thisWeapon)
	{
		return (thisWeapon) ? thisWeapon->templateForm : NULL;
	}
}

#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

void papyrusWeapon::RegisterFuncs(VMClassRegistry* registry)
{
	registry->RegisterFunction(
		new NativeFunction0 <TESObjectWEAP, TESObjectWEAP*>("GetTemplate", "Weapon", papyrusWeapon::GetTemplate, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESObjectWEAP, UInt32>("GetWeaponType", "Weapon", papyrusWeapon::GetWeaponType, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESObjectWEAP, void, UInt32>("SetWeaponType", "Weapon", papyrusWeapon::SetWeaponType, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESObjectWEAP, UInt32>("GetBaseDamage", "Weapon", papyrusWeapon::GetBaseDamage, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESObjectWEAP, void, UInt32>("SetBaseDamage", "Weapon", papyrusWeapon::SetBaseDamage, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESObjectWEAP, UInt32>("GetCritDamage", "Weapon", papyrusWeapon::GetCritDamage, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESObjectWEAP, void, UInt32>("SetCritDamage", "Weapon", papyrusWeapon::SetCritDamage, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESObjectWEAP, float>("GetReach", "Weapon", papyrusWeapon::GetReach, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESObjectWEAP, void, float>("SetReach", "Weapon", papyrusWeapon::SetReach, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESObjectWEAP, float>("GetMinRange", "Weapon", papyrusWeapon::GetMinRange, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESObjectWEAP, void, float>("SetMinRange", "Weapon", papyrusWeapon::SetMinRange, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESObjectWEAP, float>("GetMaxRange", "Weapon", papyrusWeapon::GetMaxRange, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESObjectWEAP, void, float>("SetMaxRange", "Weapon", papyrusWeapon::SetMaxRange, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESObjectWEAP, float>("GetSpeed", "Weapon", papyrusWeapon::GetSpeed, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESObjectWEAP, void, float>("SetSpeed", "Weapon", papyrusWeapon::SetSpeed, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESObjectWEAP, float>("GetStagger", "Weapon", papyrusWeapon::GetStagger, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESObjectWEAP, void, float>("SetStagger", "Weapon", papyrusWeapon::SetStagger, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESObjectWEAP, BSFixedString>("GetModelPath", "Weapon", papyrusWeapon::GetModelPath, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESObjectWEAP, void, BSFixedString>("SetModelPath", "Weapon", papyrusWeapon::SetModelPath, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESObjectWEAP, BSFixedString>("GetIconPath", "Weapon", papyrusWeapon::GetIconPath, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESObjectWEAP, void, BSFixedString>("SetIconPath", "Weapon", papyrusWeapon::SetIconPath, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESObjectWEAP, BSFixedString>("GetMessageIconPath", "Weapon", papyrusWeapon::GetMessageIconPath, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESObjectWEAP, void, BSFixedString>("SetMessageIconPath", "Weapon", papyrusWeapon::SetMessageIconPath, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESObjectWEAP, EnchantmentItem*>("GetEnchantment", "Weapon", papyrusWeapon::GetEnchantment, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESObjectWEAP, void, EnchantmentItem*>("SetEnchantment", "Weapon", papyrusWeapon::SetEnchantment, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESObjectWEAP, UInt32>("GetEnchantmentValue", "Weapon", papyrusWeapon::GetEnchantmentValue, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESObjectWEAP, void, UInt32>("SetEnchantmentValue", "Weapon", papyrusWeapon::SetEnchantmentValue, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESObjectWEAP, TESObjectSTAT*>("GetEquippedModel", "Weapon", papyrusWeapon::GetEquippedModel, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESObjectWEAP, void, TESObjectSTAT*>("SetEquippedModel", "Weapon", papyrusWeapon::SetEquippedModel, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESObjectWEAP, BGSEquipSlot*>("GetEquipType", "Weapon", papyrusWeapon::GetEquipType, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESObjectWEAP, void, BGSEquipSlot*>("SetEquipType", "Weapon", papyrusWeapon::SetEquipType, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESObjectWEAP, BSFixedString>("GetSkill", "Weapon", papyrusWeapon::GetSkill, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESObjectWEAP, void, BSFixedString>("SetSkill", "Weapon", papyrusWeapon::SetSkill, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESObjectWEAP, BSFixedString>("GetResist", "Weapon", papyrusWeapon::GetResist, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESObjectWEAP, void, BSFixedString>("SetResist", "Weapon", papyrusWeapon::SetResist, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESObjectWEAP, SpellItem*>("GetCritEffect", "Weapon", papyrusWeapon::GetCritEffect, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESObjectWEAP, void, SpellItem*>("SetCritEffect", "Weapon", papyrusWeapon::SetCritEffect, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESObjectWEAP, bool>("GetCritEffectOnDeath", "Weapon", papyrusWeapon::GetCritEffectOnDeath, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESObjectWEAP, void, bool>("SetCritEffectOnDeath", "Weapon", papyrusWeapon::SetCritEffectOnDeath, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESObjectWEAP, float>("GetCritMultiplier", "Weapon", papyrusWeapon::GetCritMultiplier, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESObjectWEAP, void, float>("SetCritMultiplier", "Weapon", papyrusWeapon::SetCritMultiplier, registry));
}
