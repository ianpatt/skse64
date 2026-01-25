#include "PapyrusMisc.h"

#include "GameForms.h"
#include "GameObjects.h"

namespace papyrusSoulGem
{
	UInt32 GetSoulSize(TESSoulGem* thisGem)
	{
		if (!thisGem)
			return 0;
		return thisGem->soulSize;
	}

	UInt32 GetGemSize(TESSoulGem* thisGem)
	{
		if (!thisGem)
			return 0;
		return thisGem->gemSize;
	}
}

namespace papyrusApparatus
{
	UInt32 GetQuality(BGSApparatus* thisApparatus)
	{
		if (!thisApparatus)
			return 0;
		return thisApparatus->quality.quality;
	}

	void SetQuality(BGSApparatus* thisApparatus, UInt32 nuQuality)
	{
		if (thisApparatus) {
			thisApparatus->quality.quality = nuQuality;
		}
	}
}

namespace papyrusOutfit
{
	UInt32 GetNumParts(BGSOutfit* thisOutfit)
	{
		return thisOutfit ? thisOutfit->armorOrLeveledItemArray.count : 0;
	}

	TESForm* GetNthPart(BGSOutfit* thisOutfit, UInt32 n)
	{
		TESForm* pForm = NULL;
		if (thisOutfit) {
			thisOutfit->armorOrLeveledItemArray.GetNthItem(n, pForm);
		}
		return pForm;
	}
}

#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

void papyrusSoulGem::RegisterFuncs(VMClassRegistry* registry)
{
	registry->RegisterFunction(
		new NativeFunction0 <TESSoulGem, UInt32>("GetSoulSize", "SoulGem", papyrusSoulGem::GetGemSize, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESSoulGem, UInt32> ("GetGemSize", "SoulGem", papyrusSoulGem::GetGemSize, registry));
}

void papyrusApparatus::RegisterFuncs(VMClassRegistry* registry)
{
	registry->RegisterFunction(
		new NativeFunction0 <BGSApparatus, UInt32> ("GetQuality", "Apparatus", papyrusApparatus::GetQuality, registry));

	registry->RegisterFunction(
		new NativeFunction1 <BGSApparatus, void, UInt32> ("SetQuality", "Apparatus", papyrusApparatus::SetQuality, registry));
}

void papyrusOutfit::RegisterFuncs(VMClassRegistry* registry)
{
	registry->RegisterFunction(
		new NativeFunction0 <BGSOutfit, UInt32> ("GetNumParts", "Outfit", papyrusOutfit::GetNumParts, registry));

	registry->RegisterFunction(
		new NativeFunction1 <BGSOutfit, TESForm*, UInt32> ("GetNthPart", "Outfit", papyrusOutfit::GetNthPart, registry));
}
