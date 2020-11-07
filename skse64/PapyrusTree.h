#pragma once

class VMClassRegistry;
class BGSSoundDescriptorForm;
class TESObjectTREE;
class TESForm;

namespace papyrusTree
{
	void RegisterFuncs(VMClassRegistry* registry);

	TESForm * GetIngredient(TESObjectTREE* thisTree);
	void SetIngredient(TESObjectTREE* thisTree, TESForm* ingredient);

	BGSSoundDescriptorForm * GetHarvestSound(TESObjectTREE* thisTree);
	void SetHarvestSound(TESObjectTREE* thisTree, BGSSoundDescriptorForm * sound);
};
