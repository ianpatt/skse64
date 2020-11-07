#include "PapyrusArmorAddon.h"

#include "GameObjects.h"

namespace papyrusArmorAddon
{
	BSFixedString GetModelPath(TESObjectARMA* thisArmorAddon, bool bFirst, bool bFemale)
	{
		return (thisArmorAddon) ? thisArmorAddon->models[bFirst][bFemale].GetModelName() : NULL;
	}

	void SetModelPath(TESObjectARMA* thisArmorAddon, BSFixedString nuPath, bool bFirst, bool bFemale)
	{
		if(thisArmorAddon) {
			thisArmorAddon->models[bFirst][bFemale].SetModelName(nuPath.data);
		}
	}

	UInt32 GetModelNumTextureSets(TESObjectARMA* thisArmorAddon, bool bFirst, bool bFemale)
	{
		return (thisArmorAddon) ? thisArmorAddon->models[bFirst][bFemale].count : 0;
	}

	BGSTextureSet* GetModelNthTextureSet(TESObjectARMA* thisArmorAddon, UInt32 n, bool bFirst, bool bFemale)
	{
		if(!thisArmorAddon)
			return NULL;

		TESModelTextureSwap	* tex = &thisArmorAddon->models[bFirst][bFemale];

		if(!tex->swaps || n >= tex->count)
			return NULL;

		return tex->swaps[n].textureSet;
	}

	void SetModelNthTextureSet(TESObjectARMA* thisArmorAddon, BGSTextureSet* textureSet, UInt32 n, bool bFirst, bool bFemale)
	{
		if(!thisArmorAddon || !textureSet)
			return;

		TESModelTextureSwap	* tex = &thisArmorAddon->models[bFirst][bFemale];

		if(!tex->swaps || n >= tex->count)
			return;

		tex->swaps[n].textureSet = textureSet;
	}

	UInt32 GetNumAdditionalRaces(TESObjectARMA* thisArmorAddon)
	{
		return (thisArmorAddon) ? thisArmorAddon->additionalRaces.count : 0;
	}

	TESRace* GetNthAdditionalRace(TESObjectARMA* thisArmorAddon, UInt32 n)
	{
		if(!thisArmorAddon || (n >= thisArmorAddon->additionalRaces.count))
			return NULL;

		TESRace * race = NULL;
		if(!thisArmorAddon->additionalRaces.GetNthItem(n, race))
			return NULL;

		return race;
	}

	UInt32 GetSlotMask(TESObjectARMA* thisArmorAddon)
	{
		return (thisArmorAddon) ? thisArmorAddon->biped.GetSlotMask() : 0;
	}

	void SetSlotMask(TESObjectARMA* thisArmorAddon, UInt32 slotMask)
	{
		if (thisArmorAddon) {
			thisArmorAddon->biped.SetSlotMask(slotMask);
		}
	}

	UInt32 AddSlotToMask(TESObjectARMA* thisArmorAddon, UInt32 slot)
	{
		return (thisArmorAddon) ? thisArmorAddon->biped.AddSlotToMask(slot) : 0;

	}

	UInt32 RemoveSlotFromMask(TESObjectARMA* thisArmorAddon, UInt32 slot)
	{
		return (thisArmorAddon) ? thisArmorAddon->biped.RemoveSlotFromMask(slot) : 0;
	}
}

#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

void papyrusArmorAddon::RegisterFuncs(VMClassRegistry* registry)
{
	registry->RegisterForm(TESObjectARMA::kTypeID, "ArmorAddon");

	// Model
	registry->RegisterFunction(
		new NativeFunction2 <TESObjectARMA, BSFixedString, bool, bool>("GetModelPath", "ArmorAddon", papyrusArmorAddon::GetModelPath, registry));

	registry->RegisterFunction(
		new NativeFunction3 <TESObjectARMA, void, BSFixedString, bool, bool>("SetModelPath", "ArmorAddon", papyrusArmorAddon::SetModelPath, registry));

	// Textures
	registry->RegisterFunction(
		new NativeFunction2 <TESObjectARMA, UInt32, bool, bool>("GetModelNumTextureSets", "ArmorAddon", papyrusArmorAddon::GetModelNumTextureSets, registry));

	registry->RegisterFunction(
		new NativeFunction3 <TESObjectARMA, BGSTextureSet*, UInt32, bool, bool>("GetModelNthTextureSet", "ArmorAddon", papyrusArmorAddon::GetModelNthTextureSet, registry));

	registry->RegisterFunction(
		new NativeFunction4 <TESObjectARMA, void, BGSTextureSet*, UInt32, bool, bool>("SetModelNthTextureSet", "ArmorAddon", papyrusArmorAddon::SetModelNthTextureSet, registry));

	// Races
	registry->RegisterFunction(
		new NativeFunction0 <TESObjectARMA, UInt32>("GetNumAdditionalRaces", "ArmorAddon", papyrusArmorAddon::GetNumAdditionalRaces, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESObjectARMA, TESRace*, UInt32>("GetNthAdditionalRace", "ArmorAddon", papyrusArmorAddon::GetNthAdditionalRace, registry));

	// Slot Mask
	registry->RegisterFunction(
		new NativeFunction0 <TESObjectARMA, UInt32>("GetSlotMask", "ArmorAddon", papyrusArmorAddon::GetSlotMask, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESObjectARMA, void, UInt32>("SetSlotMask", "ArmorAddon", papyrusArmorAddon::SetSlotMask, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESObjectARMA, UInt32, UInt32>("AddSlotToMask", "ArmorAddon", papyrusArmorAddon::AddSlotToMask, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESObjectARMA, UInt32, UInt32>("RemoveSlotFromMask", "ArmorAddon", papyrusArmorAddon::RemoveSlotFromMask, registry));
}
