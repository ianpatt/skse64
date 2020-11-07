#pragma once

#include "GameTypes.h"
#include "PapyrusArgs.h"

class TESObjectREFR;
class TESForm;
class EnchantmentItem;
class VMClassRegistry;
class EffectSetting;
class BGSListForm;

namespace papyrusObjectReference
{
	void RegisterFuncs(VMClassRegistry* registry);
	UInt32 GetNumItems(TESObjectREFR* pContainerRef);
	TESForm* GetNthForm(TESObjectREFR* pContainerRef, UInt32 n);
	float GetTotalItemWeight(TESObjectREFR* pContainerRef);
	float GetTotalArmorWeight(TESObjectREFR* pContainerRef);

	void SetItemHealthPercent(TESObjectREFR* object, float value);
	float GetItemCharge(TESObjectREFR* object);
	float GetItemMaxCharge(TESObjectREFR* object);
	void SetItemCharge(TESObjectREFR* object, float value);
	EnchantmentItem * GetEnchantment(TESObjectREFR* object);

	void CreateEnchantment(TESObjectREFR* object, float maxCharge, VMArray<EffectSetting*> effects, VMArray<float> magnitudes, VMArray<UInt32> areas, VMArray<UInt32> durations);
	void SetEnchantment(TESObjectREFR* object, EnchantmentItem * form, float maxCharge);

	void ResetInventory(TESObjectREFR * obj);
	bool IsOffLimits(TESObjectREFR * obj);
	BSFixedString GetDisplayName(TESObjectREFR* object);
	bool SetDisplayName(TESObjectREFR* object, BSFixedString value, bool force);
	TESObjectREFR * GetEnableParent(TESObjectREFR* object);

	bool IsHarvested(TESObjectREFR* pProduceRef);

	bool HasNiNode(TESObjectREFR * obj, BSFixedString nodeName);
	float GetNiNodePositionX(TESObjectREFR * obj, BSFixedString nodeName);
	float GetNiNodePositionY(TESObjectREFR * obj, BSFixedString nodeName);
	float GetNiNodePositionZ(TESObjectREFR * obj, BSFixedString nodeName);
	float GetNiNodeScale(TESObjectREFR * obj, BSFixedString nodeName);
	void SetNiNodeScale(TESObjectREFR * obj, BSFixedString nodeName, float value);

	void GetAllForms(TESObjectREFR* pContainerRef, BGSListForm * list);
}
