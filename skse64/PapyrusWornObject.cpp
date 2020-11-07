#include "PapyrusWornObject.h"

#include "GameForms.h"
#include "GameObjects.h"
#include "GameFormComponents.h"
#include "GameExtraData.h"
#include "GameBSExtraData.h"
#include "GameRTTI.h"
#include "GameData.h"

class MatchByEquipSlot : public FormMatcher
{
	UInt32 m_mask;
	UInt32 m_hand;
	Actor * m_actor;
public:
	MatchByEquipSlot(Actor * actor, UInt32 hand, UInt32 slot) : 
	  m_hand(hand),
	  m_mask(slot),
	  m_actor(actor)
	  {

	  }

	  enum
	  {
		  kSlotID_Left = 0,
		  kSlotID_Right
	  };

	  bool Matches(TESForm* pForm) const {
		  if (pForm) {
			  if(pForm->formType != TESObjectWEAP::kTypeID) { // If not a weapon use mask
				  BGSBipedObjectForm* pBip = DYNAMIC_CAST(pForm, TESForm, BGSBipedObjectForm);
				  if (pBip)
					  return (pBip->data.parts & m_mask) != 0;
			  } else if(m_mask == 0) { // Use hand if no mask specified
				  TESForm * equippedForm = m_actor->GetEquippedObject(m_hand == kSlotID_Left);
				  return (equippedForm && equippedForm == pForm);
			  }
		  }
		  return false;
	  }
};

namespace referenceUtils
{

	bool HasItemAbility(Actor * actor, TESForm* baseForm, BaseExtraList * extraData)
	{
		if(actor && baseForm) {
			tList<ActiveEffect> * effects = actor->magicTarget.GetActiveEffects();
			for(UInt32 i = 0; i < effects->Count(); i++) {
				ActiveEffect* effect = effects->GetNthItem(i);
				if(effect->sourceItem == baseForm) { // Check the item
					EnchantmentItem * enchantment = NULL;
					TESEnchantableForm * enchantable = DYNAMIC_CAST(baseForm, TESForm, TESEnchantableForm);
					if(enchantable) { // Check the item for a base enchantment
						enchantment = enchantable->enchantment;
					}
					if(extraData) { // Check the extra data for enchantment
						ExtraEnchantment* extraEnchant = static_cast<ExtraEnchantment*>(extraData->GetByType(kExtraData_Enchantment));
						if(extraEnchant) {
							enchantment = extraEnchant->enchant;
						}
					}

					if(effect->item == enchantment) {
						return true;
					}
				}
			}
		}

		return false;
	}

	void UpdateItemAbility(Actor * actor, TESForm* baseForm, BaseExtraList * extraData, bool bLeftHand)
	{
		if(actor && baseForm) {
			if(baseForm->formType == TESObjectWEAP::kTypeID)
				CALL_MEMBER_FN(actor, UpdateWeaponAbility)(baseForm, extraData, bLeftHand);
			else if(baseForm->formType == TESObjectARMO::kTypeID)
				CALL_MEMBER_FN(actor, UpdateArmorAbility)(baseForm, extraData);
		}
	}

	float GetItemHealthPercent(TESForm * baseForm, BaseExtraList* extraData)
	{
		// Object must be a weapon, or armor
		if(baseForm) {
			if(baseForm->formType == TESObjectWEAP::kTypeID || baseForm->formType == TESObjectARMO::kTypeID) {
				ExtraHealth* xHealth = static_cast<ExtraHealth*>(extraData->GetByType(kExtraData_Health));
				if(xHealth) {
					return xHealth->health;
				}
			}
		}

		return 1.0;
	}

	void SetItemHealthPercent(TESForm * baseForm, BaseExtraList* extraData, float value)
	{
		// Object must be a weapon, or armor
		if(baseForm) {
			if(baseForm->formType == TESObjectWEAP::kTypeID || baseForm->formType == TESObjectARMO::kTypeID) {
				ExtraHealth* xHealth = static_cast<ExtraHealth*>(extraData->GetByType(kExtraData_Health));
				if(xHealth) {
					xHealth->health = value;
				} else  {
					ExtraHealth* newHealth = ExtraHealth::Create();
					newHealth->health = value;
					extraData->Add(kExtraData_Health, newHealth);
				}
			}
		}
	}

	float GetItemMaxCharge(TESForm * baseForm, BaseExtraList* extraData)
	{
		if (!baseForm)
			return 0.0;
		TESObjectWEAP * weapon = DYNAMIC_CAST(baseForm, TESForm, TESObjectWEAP);
		if(!weapon) // Object is not a weapon
			return 0.0;
		float maxCharge = 0;
		if(weapon->enchantable.enchantment != NULL) // Base enchant
			maxCharge = (float)weapon->enchantable.maxCharge;
		else if(ExtraEnchantment* extraEnchant = static_cast<ExtraEnchantment*>(extraData->GetByType(kExtraData_Enchantment))) // Enchanted
			maxCharge = (float)extraEnchant->maxCharge;
		return maxCharge;
	}

	void SetItemMaxCharge(TESForm * baseForm, BaseExtraList* extraData, float maxCharge)
	{
		if (baseForm) {
			TESObjectWEAP * weapon = DYNAMIC_CAST(baseForm, TESForm, TESObjectWEAP);
			if(weapon) {
				if(maxCharge > USHRT_MAX)
					maxCharge = USHRT_MAX;

				if(ExtraEnchantment* extraEnchant = static_cast<ExtraEnchantment*>(extraData->GetByType(kExtraData_Enchantment))) // Enchanted
					extraEnchant->maxCharge = (UInt16)maxCharge;
			}
		}
	}

	float GetItemCharge(TESForm * baseForm, BaseExtraList* extraData)
	{
		if (!baseForm)
			return 0.0;

		if(baseForm->formType != TESObjectWEAP::kTypeID) // Object is not a weapon
			return 0.0;

		ExtraCharge* xCharge = static_cast<ExtraCharge*>(extraData->GetByType(kExtraData_Charge));
		return (xCharge) ? xCharge->charge : GetItemMaxCharge(baseForm, extraData); // When charge value is not present on an enchanted weapon, maximum charge is assumed
	}

	void SetItemCharge(TESForm * baseForm, BaseExtraList* extraData, float value)
	{
		// Object must be an enchanted weapon
		if(baseForm) {
			TESObjectWEAP * weapon = DYNAMIC_CAST(baseForm, TESForm, TESObjectWEAP);
			if(weapon && ((extraData->GetByType(kExtraData_Enchantment) || weapon->enchantable.enchantment != NULL))) {
				ExtraCharge* xCharge = static_cast<ExtraCharge*>(extraData->GetByType(kExtraData_Charge));
				if(xCharge) {
					xCharge->charge = value;
				} else {
					ExtraCharge* newCharge = ExtraCharge::Create();
					newCharge->charge = value;
					extraData->Add(kExtraData_Charge, newCharge);
				}
			}
		}
	}

	BSFixedString GetDisplayName(TESForm * baseForm, BaseExtraList* extraData)
	{
		const char * displayName = NULL;
		if(baseForm) {
			displayName = extraData->GetDisplayName(baseForm);
			if(!displayName) {
				TESFullName* pFullName = DYNAMIC_CAST(baseForm, TESForm, TESFullName);
				if (pFullName)
					displayName = pFullName->name.data;
			}
		}

		return displayName;
	}

	bool SetDisplayName(BaseExtraList* extraData, BSFixedString value, bool force)
	{
		bool renamed = false;
		ExtraTextDisplayData* xTextData = static_cast<ExtraTextDisplayData*>(extraData->GetByType(kExtraData_TextDisplayData));
		if(xTextData) {
			bool inUse = xTextData->message || xTextData->owner;
			if(inUse && force) {
				xTextData->message = NULL;
				xTextData->owner = NULL;
			}
			renamed = (inUse == false || force);
			CALL_MEMBER_FN(xTextData, SetName_Internal)(value.data);
		} else {
			ExtraTextDisplayData* newTextData = ExtraTextDisplayData::Create();
			CALL_MEMBER_FN(newTextData, SetName_Internal)(value.data);
			extraData->Add(kExtraData_TextDisplayData, newTextData);
			renamed = true;
		}

		return renamed;
	}

	EnchantmentItem * GetEnchantment(BaseExtraList * extraData)
	{
		ExtraEnchantment* extraEnchant = static_cast<ExtraEnchantment*>(extraData->GetByType(kExtraData_Enchantment));
		return extraEnchant ? extraEnchant->enchant : NULL;
	}

	void CreateEnchantment(TESForm* baseForm, BaseExtraList * extraData, float maxCharge, VMArray<EffectSetting*> effects, VMArray<float> magnitudes, VMArray<UInt32> areas, VMArray<UInt32> durations)
	{
		if(baseForm && (baseForm->formType == TESObjectWEAP::kTypeID || baseForm->formType == TESObjectARMO::kTypeID)) {
			EnchantmentItem * enchantment = NULL;
			if(effects.Length() > 0 && magnitudes.Length() == effects.Length() && areas.Length() == effects.Length() && durations.Length() == effects.Length()) {
				tArray<MagicItem::EffectItem> effectItems;
				effectItems.Allocate(effects.Length());

				UInt32 j = 0;
				for(UInt32 i = 0; i < effects.Length(); i++) {
					EffectSetting * magicEffect = NULL;
					effects.Get(&magicEffect, i);
					if(magicEffect) { // Only add effects that actually exist
						magnitudes.Get(&effectItems[j].magnitude, i);
						areas.Get(&effectItems[j].area, i);
						durations.Get(&effectItems[j].duration, i);
						effectItems[j].mgef = magicEffect;
						j++;
					}
				}
				effectItems.count = j; // Set count to existing count

				if(baseForm->formType == TESObjectWEAP::kTypeID)
					enchantment = CALL_MEMBER_FN(PersistentFormManager::GetSingleton(), CreateOffensiveEnchantment)(&effectItems);
				else
					enchantment = CALL_MEMBER_FN(PersistentFormManager::GetSingleton(), CreateDefensiveEnchantment)(&effectItems);

				Heap_Free(effectItems.entries);
			}

			if(enchantment) {
				if(maxCharge > 0xFFFF) // Charge exceeds uint16 clip it
					maxCharge = 0xFFFF;

				ExtraEnchantment* extraEnchant = static_cast<ExtraEnchantment*>(extraData->GetByType(kExtraData_Enchantment));
				if(extraEnchant) {
					PersistentFormManager::GetSingleton()->DecRefEnchantment(extraEnchant->enchant);
					extraEnchant->enchant = enchantment;
					PersistentFormManager::GetSingleton()->IncRefEnchantment(extraEnchant->enchant);

					extraEnchant->maxCharge = (UInt16)maxCharge;
				} else {
					ExtraEnchantment* extraEnchant = ExtraEnchantment::Create();
					extraEnchant->enchant = enchantment;
					extraEnchant->maxCharge = (UInt16)maxCharge;
					extraData->Add(kExtraData_Enchantment, extraEnchant);
				}
			}
		}
	}

	void SetEnchantment(TESForm* baseForm, BaseExtraList * extraData, EnchantmentItem * enchantment, float maxCharge)
	{
		if(baseForm && (baseForm->formType == TESObjectWEAP::kTypeID || baseForm->formType == TESObjectARMO::kTypeID)) {
			if(!enchantment) { // Null form, remove enchantment and it's extraData
				ExtraEnchantment* extraEnchant = static_cast<ExtraEnchantment*>(extraData->GetByType(kExtraData_Enchantment));
				if(extraEnchant)
					extraData->Remove(kExtraData_Enchantment, extraEnchant);

				PersistentFormManager::GetSingleton()->DecRefEnchantment(extraEnchant->enchant);
				Heap_Free(extraEnchant);
			} else { // Non-null form, accept Enchantment
				if(maxCharge > 0xFFFF) // Charge exceeds uint16 clip it
					maxCharge = 0xFFFF;

				ExtraEnchantment* extraEnchant = static_cast<ExtraEnchantment*>(extraData->GetByType(kExtraData_Enchantment));
				if(extraEnchant) {
					PersistentFormManager::GetSingleton()->DecRefEnchantment(extraEnchant->enchant);
					extraEnchant->enchant = enchantment;
					PersistentFormManager::GetSingleton()->IncRefEnchantment(extraEnchant->enchant);

					extraEnchant->maxCharge = (UInt16)maxCharge;
				} else {
					ExtraEnchantment* extraEnchant = ExtraEnchantment::Create();
					extraEnchant->enchant = enchantment;
					extraEnchant->maxCharge = (UInt16)maxCharge;
					extraData->Add(kExtraData_Enchantment, extraEnchant);
				}
			}
		}
	}

	AlchemyItem * GetPoison(BaseExtraList * extraData)
	{
		ExtraPoison* extraPoison = static_cast<ExtraPoison*>(extraData->GetByType(kExtraData_Poison));
		return extraPoison ? extraPoison->poison : NULL;
	}

	UInt32 GetNumReferenceAliases(BaseExtraList * extraData)
	{
		ExtraAliasInstanceArray* xAliases = static_cast<ExtraAliasInstanceArray*>(extraData->GetByType(kExtraData_AliasInstanceArray));
		if(!xAliases)
			return NULL;

		return xAliases->aliases.count;
	}

	VMResultArray<BGSRefAlias*> GetReferenceAliases(BaseExtraList* extraData)
	{
		VMResultArray<BGSRefAlias*> result;
		ExtraAliasInstanceArray* xAliases = static_cast<ExtraAliasInstanceArray*>(extraData->GetByType(kExtraData_AliasInstanceArray));
		if(!xAliases)
			return result;

		ExtraAliasInstanceArray::AliasInfo * info = NULL;
		for(UInt32 n = 0; n < xAliases->aliases.count; n++)
		{
			BGSRefAlias * alias = NULL;
			if(xAliases->aliases.GetNthItem(n, info))
				alias = DYNAMIC_CAST(info->alias, BGSBaseAlias, BGSRefAlias);

			result.push_back(alias);
		}
		
		return result;
	}

	BGSRefAlias * GetNthReferenceAlias(BaseExtraList * extraData, UInt32 n)
	{
		ExtraAliasInstanceArray* xAliases = static_cast<ExtraAliasInstanceArray*>(extraData->GetByType(kExtraData_AliasInstanceArray));
		if(!xAliases)
			return NULL;

		ExtraAliasInstanceArray::AliasInfo * info = NULL;
		xAliases->aliases.GetNthItem(n, info);
		if(!info)
			return NULL;

		return (BGSRefAlias*)info->alias;
	}

	EquipData ResolveEquippedObject(Actor * actor, UInt32 weaponSlot, UInt32 slotMask)
	{
		EquipData foundData;
		foundData.pForm = NULL;
		foundData.pExtraData = NULL;
		if(!actor)
			return foundData;

		MatchByEquipSlot matcher(actor, weaponSlot, slotMask);
		ExtraContainerChanges* pContainerChanges = static_cast<ExtraContainerChanges*>(actor->extraData.GetByType(kExtraData_ContainerChanges));
		if (pContainerChanges) {
			foundData = pContainerChanges->FindEquipped(matcher, weaponSlot == MatchByEquipSlot::kSlotID_Right || slotMask != 0, weaponSlot == MatchByEquipSlot::kSlotID_Left);
			return foundData;
		}

		return foundData;
	}
};

namespace papyrusWornObject
{
	float GetItemHealthPercent(WORNOBJECT_PARAMS)
	{
		EquipData equipData = referenceUtils::ResolveEquippedObject(actor, weaponSlot, slotMask);
		if(equipData.pForm && equipData.pExtraData) {
			return referenceUtils::GetItemHealthPercent(equipData.pForm, equipData.pExtraData);
		}

		return 1.0;
	}

	void SetItemHealthPercent(WORNOBJECT_PARAMS, float value)
	{
		EquipData equipData = referenceUtils::ResolveEquippedObject(actor, weaponSlot, slotMask);
		if(equipData.pForm && equipData.pExtraData) {
			referenceUtils::SetItemHealthPercent(equipData.pForm, equipData.pExtraData, value);
		}
	}
	float GetItemMaxCharge(WORNOBJECT_PARAMS)
	{
		EquipData equipData = referenceUtils::ResolveEquippedObject(actor, weaponSlot, slotMask);
		if(equipData.pForm && equipData.pExtraData) {
			return referenceUtils::GetItemMaxCharge(equipData.pForm, equipData.pExtraData);
		}

		return 0.0;
	}

	void SetItemMaxCharge(WORNOBJECT_PARAMS, float maxCharge)
	{
		EquipData equipData = referenceUtils::ResolveEquippedObject(actor, weaponSlot, slotMask);
		if(equipData.pForm && equipData.pExtraData) {
			referenceUtils::SetItemMaxCharge(equipData.pForm, equipData.pExtraData, maxCharge);
		}
	}

	float GetItemCharge(WORNOBJECT_PARAMS)
	{
		EquipData equipData = referenceUtils::ResolveEquippedObject(actor, weaponSlot, slotMask);
		if(equipData.pForm && equipData.pExtraData) {
			return referenceUtils::GetItemCharge(equipData.pForm, equipData.pExtraData);
		}

		return 0.0;
	}
	/*void SetItemCharge(WORNOBJECT_PARAMS, float value)
	{
		EquipData equipData = referenceUtils::ResolveEquippedObject(actor, weaponSlot, slotMask);
		if(equipData.pForm && equipData.pExtraData) {
			referenceUtils::SetItemCharge(equipData.pForm, equipData.pExtraData, value);
		}
	}*/

	BSFixedString GetDisplayName(WORNOBJECT_PARAMS)
	{
		EquipData equipData = referenceUtils::ResolveEquippedObject(actor, weaponSlot, slotMask);
		if(equipData.pForm && equipData.pExtraData) {
			return referenceUtils::GetDisplayName(equipData.pForm, equipData.pExtraData);
		}

		return "";
	}
	bool SetDisplayName(WORNOBJECT_PARAMS, BSFixedString value, bool force)
	{
		EquipData equipData = referenceUtils::ResolveEquippedObject(actor, weaponSlot, slotMask);
		if(equipData.pForm && equipData.pExtraData) {
			return referenceUtils::SetDisplayName(equipData.pExtraData, value, force);
		}

		return false;
	}

	EnchantmentItem * GetEnchantment(WORNOBJECT_PARAMS)
	{
		EquipData equipData = referenceUtils::ResolveEquippedObject(actor, weaponSlot, slotMask);
		if(equipData.pForm && equipData.pExtraData) {
			return referenceUtils::GetEnchantment(equipData.pExtraData);
		}

		return NULL;
	}

	void CreateEnchantment(WORNOBJECT_PARAMS, float maxCharge, VMArray<EffectSetting*> effects, VMArray<float> magnitudes, VMArray<UInt32> areas, VMArray<UInt32> durations)
	{
		EquipData equipData = referenceUtils::ResolveEquippedObject(actor, weaponSlot, slotMask);
		if(equipData.pForm && equipData.pExtraData) {
			referenceUtils::CreateEnchantment(equipData.pForm, equipData.pExtraData, maxCharge, effects, magnitudes, areas, durations);
			if(actor)
				referenceUtils::UpdateItemAbility(actor, equipData.pForm, equipData.pExtraData, weaponSlot == MatchByEquipSlot::kSlotID_Left);
		}
	}

	void SetEnchantment(WORNOBJECT_PARAMS, EnchantmentItem * enchantment, float maxCharge)
	{
		EquipData equipData = referenceUtils::ResolveEquippedObject(actor, weaponSlot, slotMask);
		if(equipData.pForm && equipData.pExtraData) {
			referenceUtils::SetEnchantment(equipData.pForm, equipData.pExtraData, enchantment, maxCharge);
			if(actor)
				referenceUtils::UpdateItemAbility(actor, equipData.pForm, equipData.pExtraData, weaponSlot == MatchByEquipSlot::kSlotID_Left);
		}
	}

	AlchemyItem * GetPoison(WORNOBJECT_PARAMS)
	{
		EquipData equipData = referenceUtils::ResolveEquippedObject(actor, weaponSlot, slotMask);
		if(equipData.pForm && equipData.pExtraData) {
			return referenceUtils::GetPoison(equipData.pExtraData);
		}

		return NULL;
	}

	UInt32 GetNumReferenceAliases(WORNOBJECT_PARAMS)
	{
		EquipData equipData = referenceUtils::ResolveEquippedObject(actor, weaponSlot, slotMask);
		if(equipData.pForm && equipData.pExtraData) {
			return referenceUtils::GetNumReferenceAliases(equipData.pExtraData);
		}

		return 0;
	}
	BGSRefAlias * GetNthReferenceAlias(WORNOBJECT_PARAMS, UInt32 n)
	{
		EquipData equipData = referenceUtils::ResolveEquippedObject(actor, weaponSlot, slotMask);
		if(equipData.pForm && equipData.pExtraData) {
			return referenceUtils::GetNthReferenceAlias(equipData.pExtraData, n);
		}

		return NULL;
	}

	VMResultArray<BGSRefAlias*> GetReferenceAliases(WORNOBJECT_PARAMS)
	{
		VMResultArray<BGSRefAlias*> result;
		EquipData equipData = referenceUtils::ResolveEquippedObject(actor, weaponSlot, slotMask);
		if(equipData.pForm && equipData.pExtraData) {
			result = referenceUtils::GetReferenceAliases(equipData.pExtraData);
		}

		return result;
	}
};

#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

void papyrusWornObject::RegisterFuncs(VMClassRegistry* registry)
{
	// Item modifications, Tempering/Charges
	registry->RegisterFunction(
		new NativeFunction3<StaticFunctionTag, float, WORNOBJECT_TEMPLATE>("GetItemHealthPercent", "WornObject", papyrusWornObject::GetItemHealthPercent, registry));

	registry->RegisterFunction(
		new NativeFunction4<StaticFunctionTag, void, WORNOBJECT_TEMPLATE, float>("SetItemHealthPercent", "WornObject", papyrusWornObject::SetItemHealthPercent, registry));

	registry->RegisterFunction(
		new NativeFunction3<StaticFunctionTag, float, WORNOBJECT_TEMPLATE>("GetItemMaxCharge", "WornObject", papyrusWornObject::GetItemMaxCharge, registry));

	registry->RegisterFunction(
		new NativeFunction4<StaticFunctionTag, void, WORNOBJECT_TEMPLATE, float>("SetItemMaxCharge", "WornObject", papyrusWornObject::SetItemMaxCharge, registry));

	registry->RegisterFunction(
		new NativeFunction3<StaticFunctionTag, float, WORNOBJECT_TEMPLATE>("GetItemCharge", "WornObject", papyrusWornObject::GetItemCharge, registry));

	/*registry->RegisterFunction(
		new NativeFunction4<StaticFunctionTag, void, WORNOBJECT_TEMPLATE, float>("SetItemCharge", "WornObject", papyrusWornObject::SetItemCharge, registry));*/

	registry->RegisterFunction(
		new NativeFunction3<StaticFunctionTag, EnchantmentItem*, WORNOBJECT_TEMPLATE>("GetEnchantment", "WornObject", papyrusWornObject::GetEnchantment, registry));

	registry->RegisterFunction(
		new NativeFunction5<StaticFunctionTag, void, WORNOBJECT_TEMPLATE, EnchantmentItem*, float>("SetEnchantment", "WornObject", papyrusWornObject::SetEnchantment, registry));

	registry->RegisterFunction(
		new NativeFunction8<StaticFunctionTag, void, WORNOBJECT_TEMPLATE, float, VMArray<EffectSetting*>, VMArray<float>, VMArray<UInt32>, VMArray<UInt32>>("CreateEnchantment", "WornObject", papyrusWornObject::CreateEnchantment, registry));

	registry->RegisterFunction(
		new NativeFunction3<StaticFunctionTag, AlchemyItem*, WORNOBJECT_TEMPLATE>("GetPoison", "WornObject", papyrusWornObject::GetPoison, registry));

	registry->RegisterFunction(
		new NativeFunction3<StaticFunctionTag, BSFixedString, WORNOBJECT_TEMPLATE>("GetDisplayName", "WornObject", papyrusWornObject::GetDisplayName, registry));

	registry->RegisterFunction(
		new NativeFunction5<StaticFunctionTag, bool, WORNOBJECT_TEMPLATE, BSFixedString, bool>("SetDisplayName", "WornObject", papyrusWornObject::SetDisplayName, registry));

	registry->RegisterFunction(
		new NativeFunction3<StaticFunctionTag, UInt32, WORNOBJECT_TEMPLATE>("GetNumReferenceAliases", "WornObject", papyrusWornObject::GetNumReferenceAliases, registry));

	registry->RegisterFunction(
		new NativeFunction4<StaticFunctionTag, BGSRefAlias*, WORNOBJECT_TEMPLATE, UInt32>("GetNthReferenceAlias", "WornObject", papyrusWornObject::GetNthReferenceAlias, registry));

	registry->RegisterFunction(
		new NativeFunction3<StaticFunctionTag, VMResultArray<BGSRefAlias*>, WORNOBJECT_TEMPLATE>("GetReferenceAliases", "WornObject", papyrusWornObject::GetReferenceAliases, registry));
}
