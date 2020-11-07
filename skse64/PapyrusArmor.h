#pragma once

#include "GameTypes.h"
class TESObjectARMO;
struct StaticFunctionTag;
class VMClassRegistry;
class EnchantmentItem;
class TESObjectARMA;


namespace papyrusArmor
{
	void RegisterFuncs(VMClassRegistry* registry);

	UInt32 GetArmorRating(TESObjectARMO* thisArmor);
	void SetArmorRating(TESObjectARMO* thisArmor, UInt32 nuAR);
	void ModArmorRating(TESObjectARMO* thisArmor, SInt32 modBy);

	UInt32 GetSlotMask(TESObjectARMO* thisArmor);
	void SetSlotMask(TESObjectARMO* thisArmor, UInt32 slotMask);
	UInt32 AddToSlotMask(TESObjectARMO* thisArmor, UInt32 slotMask);
	UInt32 RemoveFromSlotMask(TESObjectARMO* thisArmor, UInt32 slotMask);

	UInt32 GetMaskForSlot(StaticFunctionTag*, UInt32 slot);

	BSFixedString GetModelPath(TESObjectARMO* thisArmor, bool bFemale);
	void SetModelPath(TESObjectARMO* thisArmor, BSFixedString nuPath, bool bFemale);

	BSFixedString GetIconPath(TESObjectARMO* thisArmor, bool bFemale);
	void SetIconPath(TESObjectARMO* thisArmor, BSFixedString nuPath, bool bFemale);

	BSFixedString GetMessageIconPath(TESObjectARMO* thisArmor,  bool bFemale);
	void SetMessageIconPath(TESObjectARMO* thisArmor, BSFixedString nuPath, bool bFemale);

	UInt32 GetWeightClass(TESObjectARMO* thisArmor);
	void SetWeightClass(TESObjectARMO* thisArmor, UInt32 nuWeightClass);

	EnchantmentItem* GetEnchantment(TESObjectARMO* thisArmor);
	void SetEnchantment(TESObjectARMO* thisArmor, EnchantmentItem* enchantment);

	UInt32 GetNumArmorAddons(TESObjectARMO* thisArmor);
	TESObjectARMA* GetNthArmorAddon(TESObjectARMO* thisArmor, UInt32 n);
}
