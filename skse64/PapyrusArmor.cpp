#include "PapyrusArmor.h"

#include "GameObjects.h"

namespace papyrusArmor
{
	UInt32 GetArmorRating(TESObjectARMO* thisArmor)
	{
		return (thisArmor) ? thisArmor->armorValTimes100 / 100 : 0;
	}

	void SetArmorRating(TESObjectARMO* thisArmor, UInt32 nuAR)
	{
		if (thisArmor) {
			thisArmor->armorValTimes100 = nuAR * 100;
		}
	}

	void ModArmorRating(TESObjectARMO* thisArmor, SInt32 modBy)
	{
		if (thisArmor) {
			SInt32 nuAR = thisArmor->armorValTimes100 + (modBy * 100);
			if (nuAR < 0)
				nuAR = 0;
			thisArmor->armorValTimes100 = nuAR;
		}
	}

	UInt32 GetSlotMask(TESObjectARMO* thisArmor)
	{
		return (thisArmor) ? thisArmor->bipedObject.GetSlotMask() : 0;
	}

	void SetSlotMask(TESObjectARMO* thisArmor, UInt32 slotMask)
	{
		if (thisArmor) {
			thisArmor->bipedObject.SetSlotMask(slotMask);
		}
	}

	UInt32 AddSlotToMask(TESObjectARMO* thisArmor, UInt32 slot)
	{
		return (thisArmor) ? thisArmor->bipedObject.AddSlotToMask(slot) : 0;
	
	}
	
	UInt32 RemoveSlotFromMask(TESObjectARMO* thisArmor, UInt32 slot)
	{
		return (thisArmor) ? thisArmor->bipedObject.RemoveSlotFromMask(slot) : 0;
	}

	UInt32 GetMaskForSlot(StaticFunctionTag*, UInt32 slot) 
	{
		if (slot < 29 || slot > 61)
			return 0;

		return (1 << (slot - 30));
	}

	BSFixedString GetModelPath(TESObjectARMO* thisArmor, bool bFemale)
	{
		return (thisArmor) ? thisArmor->bipedModel.textureSwap[bFemale ? 1 : 0].GetModelName() : NULL;
	}

	void SetModelPath(TESObjectARMO* thisArmor, BSFixedString nuPath, bool bFemale)
	{
		if (thisArmor) {
			thisArmor->bipedModel.textureSwap[bFemale ? 1 : 0].SetModelName(nuPath.data);
		}
	}

	BSFixedString GetIconPath(TESObjectARMO* thisArmor, bool bFemale)
	{
		return (thisArmor) ? thisArmor->bipedModel.icon[bFemale ? 1 : 0].str : NULL;
	}

	void SetIconPath(TESObjectARMO* thisArmor, BSFixedString nuPath, bool bFemale)
	{
		if (thisArmor) {
			thisArmor->bipedModel.icon[bFemale ? 1 : 0].str = nuPath.data;
		}
	}

	BSFixedString GetMessageIconPath(TESObjectARMO* thisArmor, bool bFemale)
	{
		return (thisArmor) ? thisArmor->bipedModel.messageIcon[bFemale ? 1 : 0].icon.str : NULL;
	}

	void SetMessageIconPath(TESObjectARMO* thisArmor, BSFixedString nuPath, bool bFemale)
	{
		if (thisArmor) {
			thisArmor->bipedModel.messageIcon[bFemale ? 1 : 0].icon.str = nuPath.data;
		}
	}

	UInt32 GetWeightClass(TESObjectARMO* thisArmor)
	{
		return (thisArmor) ? thisArmor->bipedObject.data.weightClass : BGSBipedObjectForm::kWeight_None;
	}

	void SetWeightClass(TESObjectARMO* thisArmor, UInt32 nuWeightClass)
	{
		if (thisArmor && nuWeightClass <=2)
			thisArmor->bipedObject.data.weightClass = nuWeightClass;
	}

	EnchantmentItem* GetEnchantment(TESObjectARMO* thisArmor)
	{
		return (thisArmor) ? thisArmor->enchantable.enchantment : NULL;
	}

	void SetEnchantment(TESObjectARMO* thisArmor, EnchantmentItem* enchantment)
	{
		if (thisArmor) {
			thisArmor->enchantable.enchantment = enchantment;
		}
	}

	UInt32 GetNumArmorAddons(TESObjectARMO* thisArmor)
	{
		return (thisArmor) ? thisArmor->armorAddons.count : 0;
	}

	TESObjectARMA* GetNthArmorAddon(TESObjectARMO* thisArmor, UInt32 n)
	{
		if(!thisArmor)
			return NULL;

		TESObjectARMA	* addon = NULL;
		if(thisArmor->armorAddons.GetNthItem(n, addon))
			return addon;

		return NULL;
	}
}

#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

void papyrusArmor::RegisterFuncs(VMClassRegistry* registry)
{
	// Armor Rating
	registry->RegisterFunction(
		new NativeFunction0 <TESObjectARMO, UInt32>("GetArmorRating", "Armor", papyrusArmor::GetArmorRating, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESObjectARMO, void, UInt32>("SetArmorRating", "Armor", papyrusArmor::SetArmorRating, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESObjectARMO, void, SInt32>("ModArmorRating", "Armor", papyrusArmor::ModArmorRating, registry));

	// Slot Mask
	registry->RegisterFunction(
		new NativeFunction0 <TESObjectARMO, UInt32>("GetSlotMask", "Armor", papyrusArmor::GetSlotMask, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESObjectARMO, void, UInt32>("SetSlotMask", "Armor", papyrusArmor::SetSlotMask, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESObjectARMO, UInt32, UInt32>("AddSlotToMask", "Armor", papyrusArmor::AddSlotToMask, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESObjectARMO, UInt32, UInt32>("RemoveSlotFromMask", "Armor", papyrusArmor::RemoveSlotFromMask, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, UInt32, UInt32> ("GetMaskForSlot", "Armor", papyrusArmor::GetMaskForSlot, registry));

	// World Model
	registry->RegisterFunction(
		new NativeFunction1 <TESObjectARMO, BSFixedString, bool>("GetModelPath", "Armor", papyrusArmor::GetModelPath, registry));

	registry->RegisterFunction(
		new NativeFunction2 <TESObjectARMO, void, BSFixedString, bool>("SetModelPath", "Armor", papyrusArmor::SetModelPath, registry));

	// Icon
	registry->RegisterFunction(
		new NativeFunction1 <TESObjectARMO, BSFixedString, bool>("GetIconPath", "Armor", papyrusArmor::GetIconPath, registry));

	registry->RegisterFunction(
		new NativeFunction2 <TESObjectARMO, void, BSFixedString, bool>("SetIconPath", "Armor", papyrusArmor::SetIconPath, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESObjectARMO, BSFixedString, bool>("GetMessageIconPath", "Armor", papyrusArmor::GetMessageIconPath, registry));

	registry->RegisterFunction(
		new NativeFunction2 <TESObjectARMO, void, BSFixedString, bool>("SetMessageIconPath", "Armor", papyrusArmor::SetMessageIconPath, registry));

	// Weight Class
	registry->RegisterFunction(
		new NativeFunction0 <TESObjectARMO, UInt32>("GetWeightClass", "Armor", papyrusArmor::GetWeightClass, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESObjectARMO, void, UInt32>("SetWeightClass", "Armor", papyrusArmor::SetWeightClass, registry));

	// Enchantments
	registry->RegisterFunction(
		new NativeFunction0 <TESObjectARMO, EnchantmentItem*>("GetEnchantment", "Armor", papyrusArmor::GetEnchantment, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESObjectARMO, void, EnchantmentItem*>("SetEnchantment", "Armor", papyrusArmor::SetEnchantment, registry));

	// Armor Addons
	registry->RegisterFunction(
		new NativeFunction0 <TESObjectARMO, UInt32>("GetNumArmorAddons", "Armor", papyrusArmor::GetNumArmorAddons, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESObjectARMO, TESObjectARMA*, UInt32>("GetNthArmorAddon", "Armor", papyrusArmor::GetNthArmorAddon, registry));
}
