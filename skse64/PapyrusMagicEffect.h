#pragma once

#include "GameTypes.h"

class EffectSetting;
class VMClassRegistry;
class TESObjectLIGH;
class TESEffectShader;
class BGSProjectile;
class BGSArtObject;
class BGSImpactDataSet;
class SpellItem;
class TESImageSpaceModifier;
class BGSPerk;
class BGSExplosion;

namespace papyrusMagicEffect
{
	void RegisterFuncs(VMClassRegistry* registry);

	bool IsEffectFlagSet(EffectSetting* thisEffect, UInt32 flag);
	void SetEffectFlag(EffectSetting* thisEffect, UInt32 flag);
	void ClearEffectFlag(EffectSetting* thisEffect, UInt32 flag);

	float GetCastTime(EffectSetting* thisEffect);
	void SetCastTime(EffectSetting* thisEffect, float castTime);
	UInt32 GetSkillLevel(EffectSetting* thisEffect);
	void SetSkillLevel(EffectSetting* thisEffect, UInt32 level);
	UInt32 GetArea(EffectSetting* thisEffect);
	void SetArea(EffectSetting* thisEffect, UInt32 area);
	float GetSkillUsageMult(EffectSetting* thisEffect);
	void SetSkillUsageMult(EffectSetting* thisEffect, float skillMult);
	float GetBaseCost(EffectSetting* thisEffect);
	void SetBaseCost(EffectSetting* thisEffect, float baseCost);
	//void SetAssociatedSkill(EffectSetting* thisEffect, BSFixedString school);
	//BSFixedString GetResistance(EffectSetting* thisEffect);
	//void SetResistance(EffectSetting* thisEffect, BSFixedString resistance);
	TESObjectLIGH* GetLight(EffectSetting* thisEffect);
	void SetLight(EffectSetting* thisEffect, TESObjectLIGH * obj);
	TESEffectShader* GetHitShader(EffectSetting* thisEffect);
	void SetHitShader(EffectSetting* thisEffect, TESEffectShader * obj);
	TESEffectShader* GetEnchantShader(EffectSetting* thisEffect);
	void SetEnchantShader(EffectSetting* thisEffect, TESEffectShader * obj);
	BGSProjectile* GetProjectile(EffectSetting* thisEffect);
	void SetProjectile(EffectSetting* thisEffect, BGSProjectile * obj);
	BGSExplosion* GetExplosion(EffectSetting* thisEffect);
	void SetExplosion(EffectSetting* thisEffect, BGSExplosion * obj);
	BGSArtObject* GetCastingArt(EffectSetting* thisEffect);
	void SetCastingArt(EffectSetting* thisEffect, BGSArtObject * obj);
	BGSArtObject* GetHitEffectArt(EffectSetting* thisEffect);
	void SetHitEffectArt(EffectSetting* thisEffect, BGSArtObject * obj);
	BGSImpactDataSet* GetImpactDataSet(EffectSetting* thisEffect);
	void SetImpactDataSet(EffectSetting* thisEffect, BGSImpactDataSet * obj);
	BGSArtObject* GetEnchantArt(EffectSetting* thisEffect);
	void SetEnchantArt(EffectSetting* thisEffect, BGSArtObject * obj);
	SpellItem* GetEquipAbility(EffectSetting* thisEffect);
	void SetEquipAbility(EffectSetting* thisEffect, SpellItem * obj);
	//TESImageSpaceModifier* GetImageSpaceMod(EffectSetting* thisEffect);
	//void SetImageSpaceMod(EffectSetting* thisEffect, TESImageSpaceModifier * obj);
	BGSPerk* GetPerk(EffectSetting* thisEffect);
	void SetPerk(EffectSetting* thisEffect, BGSPerk * obj);
	UInt32 GetCastingType(EffectSetting* thisEffect);
	UInt32 GetDeliveryType(EffectSetting* thisEffect);
};
