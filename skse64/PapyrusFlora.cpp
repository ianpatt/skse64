#include "PapyrusFlora.h"
#include "GameObjects.h"

namespace papyrusFlora
{
	TESForm * GetIngredient(TESFlora* thisFlora)
	{
		return (thisFlora) ? thisFlora->produce.produce : NULL;
	}

	void SetIngredient(TESFlora* thisFlora, TESForm* ingredient)
	{
		if (thisFlora) {
			thisFlora->produce.produce = ingredient;
		}
	}

	BGSSoundDescriptorForm * GetHarvestSound(TESFlora* thisFlora)
	{
		return (thisFlora) ? thisFlora->produce.harvestSound : NULL;
	}

	void SetHarvestSound(TESFlora* thisFlora, BGSSoundDescriptorForm * sound)
	{
		if (thisFlora) {
			thisFlora->produce.harvestSound = sound;
		}
	}
}

#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

void papyrusFlora::RegisterFuncs(VMClassRegistry* registry)
{
	registry->RegisterFunction(
		new NativeFunction0<TESFlora, TESForm*>("GetIngredient", "Flora", papyrusFlora::GetIngredient, registry));

	registry->RegisterFunction(
		new NativeFunction1<TESFlora, void, TESForm*>("SetIngredient", "Flora", papyrusFlora::SetIngredient, registry));

	registry->RegisterFunction(
		new NativeFunction0<TESFlora, BGSSoundDescriptorForm*>("GetHarvestSound", "Flora", papyrusFlora::GetHarvestSound, registry));

	registry->RegisterFunction(
		new NativeFunction1<TESFlora, void, BGSSoundDescriptorForm*>("SetHarvestSound", "Flora", papyrusFlora::SetHarvestSound, registry));
}
