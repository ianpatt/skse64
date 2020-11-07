#pragma once

class VMClassRegistry;
class BGSSoundDescriptorForm;
class TESFlora;
class TESForm;

namespace papyrusFlora
{
	void RegisterFuncs(VMClassRegistry* registry);

	TESForm * GetIngredient(TESFlora* thisFlora);
	void SetIngredient(TESFlora* thisFlora, TESForm* ingredient);

	BGSSoundDescriptorForm * GetHarvestSound(TESFlora* thisFlora);
	void SetHarvestSound(TESFlora* thisFlora, BGSSoundDescriptorForm * sound);
};
