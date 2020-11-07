#include "PapyrusMagicEffect.h"
#include "PapyrusArgs.h"

#include "GameData.h"
#include "GameObjects.h"
#include "GameReferences.h"

namespace papyrusMagicEffect
{
	bool IsEffectFlagSet(EffectSetting* thisEffect, UInt32 flag)
	{
		return (thisEffect) ? (thisEffect->properties.flags & flag) == flag : false;
	}

	void SetEffectFlag(EffectSetting* thisEffect, UInt32 flag)
	{
		if (thisEffect)
			thisEffect->properties.flags |= flag;
	}

	void ClearEffectFlag(EffectSetting* thisEffect, UInt32 flag)
	{
		if (thisEffect)
			thisEffect->properties.flags &= ~flag;
	}

	float GetCastTime(EffectSetting* thisEffect)
	{
		return (thisEffect) ? thisEffect->properties.castingTime : 0.0;
	}

	void SetCastTime(EffectSetting* thisEffect, float castTime)
	{
		if(thisEffect)
			thisEffect->properties.castingTime = castTime;
	}

	UInt32 GetSkillLevel(EffectSetting* thisEffect)
	{
		return (thisEffect) ? thisEffect->properties.level : 0;
	}

	void SetSkillLevel(EffectSetting* thisEffect, UInt32 level)
	{
		if(thisEffect)
			thisEffect->properties.level = level;
	}

	UInt32 GetArea(EffectSetting* thisEffect)
	{
		return (thisEffect) ? thisEffect->properties.area : 0;
	}

	void SetArea(EffectSetting* thisEffect, UInt32 area)
	{
		if(thisEffect)
			thisEffect->properties.area = area;
	}

	float GetSkillUsageMult(EffectSetting* thisEffect)
	{
		return (thisEffect) ? thisEffect->properties.skillUsageMult : 0.0;
	}

	void SetSkillUsageMult(EffectSetting* thisEffect, float skillMult)
	{
		if(thisEffect)
			thisEffect->properties.skillUsageMult = skillMult;
	}

	float GetBaseCost(EffectSetting* thisEffect)
	{
		return (thisEffect) ? thisEffect->properties.baseCost : 0.0;
	}

	void SetBaseCost(EffectSetting* thisEffect, float baseCost)
	{
		if(thisEffect)
			thisEffect->properties.baseCost = baseCost;
	}

	void SetAssociatedSkill(EffectSetting* thisEffect, BSFixedString school)
	{
		if (thisEffect) {
			UInt32 actorValue = ActorValueList::ResolveActorValueByName(school.data);
			thisEffect->properties.school = actorValue;
		}
	}

	BSFixedString GetResistance(EffectSetting* thisEffect)
	{
		if(!thisEffect)
			return "";

		ActorValueList * avList = ActorValueList::GetSingleton();
		if(!avList)
			return "";

		ActorValueInfo * info = avList->GetActorValue(thisEffect->properties.resistance);
		if(!info)
			return "";

		return info->name;
	}

	void SetResistance(EffectSetting* thisEffect, BSFixedString resistance)
	{
		if (thisEffect) {
			UInt32 actorValue = ActorValueList::ResolveActorValueByName(resistance.data);
			thisEffect->properties.resistance = actorValue;
		}
	}

	TESObjectLIGH* GetLight(EffectSetting* thisEffect)
	{
		return (thisEffect) ? thisEffect->properties.light : NULL;
	}

	void SetLight(EffectSetting* thisEffect, TESObjectLIGH * obj)
	{
		if(thisEffect)
			thisEffect->properties.light = obj;
	}

	TESEffectShader* GetHitShader(EffectSetting* thisEffect)
	{
		return (thisEffect) ? thisEffect->properties.hitShader : NULL;
	}

	void SetHitShader(EffectSetting* thisEffect, TESEffectShader * obj)
	{
		if(thisEffect)
			thisEffect->properties.hitShader = obj;
	}

	TESEffectShader* GetEnchantShader(EffectSetting* thisEffect)
	{
		return (thisEffect) ? thisEffect->properties.enchantShader : NULL;
	}

	void SetEnchantShader(EffectSetting* thisEffect, TESEffectShader * obj)
	{
		if(thisEffect)
			thisEffect->properties.enchantShader = obj;
	}

	BGSProjectile* GetProjectile(EffectSetting* thisEffect)
	{
		return (thisEffect) ? thisEffect->properties.projectile : NULL;
	}

	void SetProjectile(EffectSetting* thisEffect, BGSProjectile * obj)
	{
		if(thisEffect)
			thisEffect->properties.projectile = obj;
	}

	BGSExplosion* GetExplosion(EffectSetting* thisEffect)
	{
		return (thisEffect) ? thisEffect->properties.explosion : NULL;
	}

	void SetExplosion(EffectSetting* thisEffect, BGSExplosion * obj)
	{
		if(thisEffect)
			thisEffect->properties.explosion = obj;
	}

	BGSArtObject* GetCastingArt(EffectSetting* thisEffect)
	{
		return (thisEffect) ? thisEffect->properties.castingArt : NULL;
	}

	void SetCastingArt(EffectSetting* thisEffect, BGSArtObject * obj)
	{
		if(thisEffect)
			thisEffect->properties.castingArt = obj;
	}

	BGSArtObject* GetHitEffectArt(EffectSetting* thisEffect)
	{
		return (thisEffect) ? thisEffect->properties.hitEffectArt : NULL;
	}

	void SetHitEffectArt(EffectSetting* thisEffect, BGSArtObject * obj)
	{
		if(thisEffect)
			thisEffect->properties.hitEffectArt = obj;
	}

	BGSImpactDataSet* GetImpactDataSet(EffectSetting* thisEffect)
	{
		return (thisEffect) ? thisEffect->properties.impactDataSet : NULL;
	}

	void SetImpactDataSet(EffectSetting* thisEffect, BGSImpactDataSet * obj)
	{
		if(thisEffect)
			thisEffect->properties.impactDataSet = obj;
	}

	BGSArtObject* GetEnchantArt(EffectSetting* thisEffect)
	{
		return (thisEffect) ? thisEffect->properties.enchantArt : NULL;
	}

	void SetEnchantArt(EffectSetting* thisEffect, BGSArtObject * obj)
	{
		if(thisEffect)
			thisEffect->properties.enchantArt = obj;
	}

	SpellItem* GetEquipAbility(EffectSetting* thisEffect)
	{
		return (thisEffect) ? thisEffect->properties.equipAbility : NULL;
	}

	void SetEquipAbility(EffectSetting* thisEffect, SpellItem * obj)
	{
		if(thisEffect)
			thisEffect->properties.equipAbility = obj;
	}

	TESImageSpaceModifier* GetImageSpaceMod(EffectSetting* thisEffect)
	{
		return (thisEffect) ? thisEffect->properties.imageSpaceMod : NULL;
	}

	void SetImageSpaceMod(EffectSetting* thisEffect, TESImageSpaceModifier * obj)
	{
		if(thisEffect)
			thisEffect->properties.imageSpaceMod = obj;
	}

	BGSPerk* GetPerk(EffectSetting* thisEffect)
	{
		return (thisEffect) ? thisEffect->properties.perk : NULL;
	}

	void SetPerk(EffectSetting* thisEffect, BGSPerk * obj)
	{
		if(thisEffect)
			thisEffect->properties.perk = obj;
	}

	UInt32 GetCastingType(EffectSetting* thisEffect)
	{
		return (thisEffect) ? thisEffect->properties.castType : 0;
	}

	UInt32 GetDeliveryType(EffectSetting* thisEffect)
	{
		return (thisEffect) ? thisEffect->properties.deliveryType : 0;
	}

	VMResultArray<TESSound*> GetSounds(EffectSetting* thisEffect)
	{
		VMResultArray<TESSound*> sounds;
		sounds.resize(EffectSetting::SoundInfo::kNumSounds, NULL);
		if(thisEffect)
		{
			for(UInt32 i = 0; i < thisEffect->sounds.count; i++)
			{
				EffectSetting::SoundInfo sInfo;
				thisEffect->sounds.GetNthItem(i, sInfo);
				if(sInfo.index < sounds.size())
					sounds.at(sInfo.index) = sInfo.sound;
			}
		}

		return sounds;
	}
}

#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

void papyrusMagicEffect::RegisterFuncs(VMClassRegistry* registry)
{
	registry->RegisterFunction(
		new NativeFunction1 <EffectSetting, bool, UInt32>("IsEffectFlagSet", "MagicEffect", papyrusMagicEffect::IsEffectFlagSet, registry));

	registry->RegisterFunction(
		new NativeFunction1 <EffectSetting, void, UInt32>("SetEffectFlag", "MagicEffect", papyrusMagicEffect::SetEffectFlag, registry));

	registry->RegisterFunction(
		new NativeFunction1 <EffectSetting, void, UInt32>("ClearEffectFlag", "MagicEffect", papyrusMagicEffect::ClearEffectFlag, registry));


	registry->RegisterFunction(
		new NativeFunction0<EffectSetting, float>("GetCastTime", "MagicEffect", papyrusMagicEffect::GetCastTime, registry));

	registry->RegisterFunction(
		new NativeFunction1<EffectSetting, void, float>("SetCastTime", "MagicEffect", papyrusMagicEffect::SetCastTime, registry));


	registry->RegisterFunction(
		new NativeFunction0<EffectSetting, UInt32>("GetSkillLevel", "MagicEffect", papyrusMagicEffect::GetSkillLevel, registry));

	registry->RegisterFunction(
		new NativeFunction1<EffectSetting, void, UInt32>("SetSkillLevel", "MagicEffect", papyrusMagicEffect::SetSkillLevel, registry));


	registry->RegisterFunction(
		new NativeFunction0<EffectSetting, UInt32>("GetArea", "MagicEffect", papyrusMagicEffect::GetArea, registry));

	registry->RegisterFunction(
		new NativeFunction1<EffectSetting, void, UInt32>("SetArea", "MagicEffect", papyrusMagicEffect::SetArea, registry));


	registry->RegisterFunction(
		new NativeFunction0<EffectSetting, float>("GetSkillUsageMult", "MagicEffect", papyrusMagicEffect::GetSkillUsageMult, registry));

	registry->RegisterFunction(
		new NativeFunction1<EffectSetting, void, float>("SetSkillUsageMult", "MagicEffect", papyrusMagicEffect::SetSkillUsageMult, registry));


	registry->RegisterFunction(
		new NativeFunction0<EffectSetting, float>("GetBaseCost", "MagicEffect", papyrusMagicEffect::GetBaseCost, registry));

	registry->RegisterFunction(
		new NativeFunction1<EffectSetting, void, float>("SetBaseCost", "MagicEffect", papyrusMagicEffect::SetBaseCost, registry));


	registry->RegisterFunction(
		new NativeFunction1<EffectSetting, void, BSFixedString>("SetAssociatedSkill", "MagicEffect", papyrusMagicEffect::SetAssociatedSkill, registry));


	registry->RegisterFunction(
		new NativeFunction0<EffectSetting, BSFixedString>("GetResistance", "MagicEffect", papyrusMagicEffect::GetResistance, registry));

	registry->RegisterFunction(
		new NativeFunction1<EffectSetting, void, BSFixedString>("SetResistance", "MagicEffect", papyrusMagicEffect::SetResistance, registry));


	registry->RegisterFunction(
		new NativeFunction0<EffectSetting, TESObjectLIGH*>("GetLight", "MagicEffect", papyrusMagicEffect::GetLight, registry));

	registry->RegisterFunction(
		new NativeFunction1<EffectSetting, void, TESObjectLIGH*>("SetLight", "MagicEffect", papyrusMagicEffect::SetLight, registry));


	registry->RegisterFunction(
		new NativeFunction0<EffectSetting, TESEffectShader*>("GetHitShader", "MagicEffect", papyrusMagicEffect::GetHitShader, registry));

	registry->RegisterFunction(
		new NativeFunction1<EffectSetting, void, TESEffectShader*>("SetHitShader", "MagicEffect", papyrusMagicEffect::SetHitShader, registry));


	registry->RegisterFunction(
		new NativeFunction0<EffectSetting, TESEffectShader*>("GetEnchantShader", "MagicEffect", papyrusMagicEffect::GetEnchantShader, registry));

	registry->RegisterFunction(
		new NativeFunction1<EffectSetting, void, TESEffectShader*>("SetEnchantShader", "MagicEffect", papyrusMagicEffect::SetEnchantShader, registry));


	registry->RegisterFunction(
		new NativeFunction0<EffectSetting, BGSProjectile*>("GetProjectile", "MagicEffect", papyrusMagicEffect::GetProjectile, registry));

	registry->RegisterFunction(
		new NativeFunction1<EffectSetting, void, BGSProjectile*>("SetProjectile", "MagicEffect", papyrusMagicEffect::SetProjectile, registry));


	registry->RegisterFunction(
		new NativeFunction0<EffectSetting, BGSExplosion*>("GetExplosion", "MagicEffect", papyrusMagicEffect::GetExplosion, registry));

	registry->RegisterFunction(
		new NativeFunction1<EffectSetting, void, BGSExplosion*>("SetExplosion", "MagicEffect", papyrusMagicEffect::SetExplosion, registry));


	registry->RegisterFunction(
		new NativeFunction0<EffectSetting, BGSArtObject*>("GetCastingArt", "MagicEffect", papyrusMagicEffect::GetCastingArt, registry));

	registry->RegisterFunction(
		new NativeFunction1<EffectSetting, void, BGSArtObject*>("SetCastingArt", "MagicEffect", papyrusMagicEffect::SetCastingArt, registry));


	registry->RegisterFunction(
		new NativeFunction0<EffectSetting, BGSArtObject*>("GetHitEffectArt", "MagicEffect", papyrusMagicEffect::GetHitEffectArt, registry));

	registry->RegisterFunction(
		new NativeFunction1<EffectSetting, void, BGSArtObject*>("SetHitEffectArt", "MagicEffect", papyrusMagicEffect::SetHitEffectArt, registry));


	registry->RegisterFunction(
		new NativeFunction0<EffectSetting, BGSArtObject*>("GetEnchantArt", "MagicEffect", papyrusMagicEffect::GetEnchantArt, registry));

	registry->RegisterFunction(
		new NativeFunction1<EffectSetting, void, BGSArtObject*>("SetEnchantArt", "MagicEffect", papyrusMagicEffect::SetEnchantArt, registry));


	registry->RegisterFunction(
		new NativeFunction0<EffectSetting, BGSImpactDataSet*>("GetImpactDataSet", "MagicEffect", papyrusMagicEffect::GetImpactDataSet, registry));

	registry->RegisterFunction(
		new NativeFunction1<EffectSetting, void, BGSImpactDataSet*>("SetImpactDataSet", "MagicEffect", papyrusMagicEffect::SetImpactDataSet, registry));


	registry->RegisterFunction(
		new NativeFunction0<EffectSetting, SpellItem*>("GetEquipAbility", "MagicEffect", papyrusMagicEffect::GetEquipAbility, registry));

	registry->RegisterFunction(
		new NativeFunction1<EffectSetting, void, SpellItem*>("SetEquipAbility", "MagicEffect", papyrusMagicEffect::SetEquipAbility, registry));


	registry->RegisterFunction(
		new NativeFunction0<EffectSetting, TESImageSpaceModifier*>("GetImageSpaceMod", "MagicEffect", papyrusMagicEffect::GetImageSpaceMod, registry));

	registry->RegisterFunction(
		new NativeFunction1<EffectSetting, void, TESImageSpaceModifier*>("SetImageSpaceMod", "MagicEffect", papyrusMagicEffect::SetImageSpaceMod, registry));


	registry->RegisterFunction(
		new NativeFunction0<EffectSetting, BGSPerk*>("GetPerk", "MagicEffect", papyrusMagicEffect::GetPerk, registry));

	registry->RegisterFunction(
		new NativeFunction1<EffectSetting, void, BGSPerk*>("SetPerk", "MagicEffect", papyrusMagicEffect::SetPerk, registry));

	registry->RegisterFunction(
		new NativeFunction0<EffectSetting, UInt32>("GetCastingType", "MagicEffect", papyrusMagicEffect::GetCastingType, registry));

	registry->RegisterFunction(
		new NativeFunction0<EffectSetting, UInt32>("GetDeliveryType", "MagicEffect", papyrusMagicEffect::GetDeliveryType, registry));

	registry->RegisterFunction(
		new NativeFunction0<EffectSetting, VMResultArray<TESSound*>>("GetSounds", "MagicEffect", papyrusMagicEffect::GetSounds, registry));


	registry->SetFunctionFlags("MagicEffect", "IsEffectFlagSet", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("MagicEffect", "SetEffectFlag", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("MagicEffect", "ClearEffectFlag", VMClassRegistry::kFunctionFlag_NoWait);

	registry->SetFunctionFlags("MagicEffect", "GetCastTime", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("MagicEffect", "SetCastTime", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("MagicEffect", "GetSkillLevel", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("MagicEffect", "SetSkillLevel", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("MagicEffect", "GetArea", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("MagicEffect", "SetArea", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("MagicEffect", "GetSkillUsageMult", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("MagicEffect", "SetSkillUsageMult", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("MagicEffect", "GetBaseCost", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("MagicEffect", "SetBaseCost", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("MagicEffect", "SetAssociatedSkill", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("MagicEffect", "GetResistance", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("MagicEffect", "SetResistance", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("MagicEffect", "GetLight", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("MagicEffect", "SetLight", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("MagicEffect", "GetHitShader", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("MagicEffect", "SetHitShader", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("MagicEffect", "GetEnchantShader", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("MagicEffect", "SetEnchantShader", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("MagicEffect", "GetProjectile", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("MagicEffect", "SetProjectile", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("MagicEffect", "GetExplosion", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("MagicEffect", "SetExplosion", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("MagicEffect", "GetCastingArt", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("MagicEffect", "SetCastingArt", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("MagicEffect", "GetHitEffectArt", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("MagicEffect", "SetHitEffectArt", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("MagicEffect", "GetImpactDataSet", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("MagicEffect", "SetImpactDataSet", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("MagicEffect", "GetEnchantArt", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("MagicEffect", "SetEnchantArt", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("MagicEffect", "GetEquipAbility", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("MagicEffect", "SetEquipAbility", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("MagicEffect", "GetImageSpaceMod", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("MagicEffect", "SetImageSpaceMod", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("MagicEffect", "GetPerk", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("MagicEffect", "SetPerk", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("MagicEffect", "GetCastingType", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("MagicEffect", "GetDeliveryType", VMClassRegistry::kFunctionFlag_NoWait);
}