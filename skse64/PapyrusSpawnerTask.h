#pragma once

struct StaticFunctionTag;
class TESForm;
class TESObjectREFR;
class VMClassRegistry;
template <typename T> class VMArray;

namespace papyrusSpawnerTask
{
	SInt32	Create(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*);

	void	AddSpawn(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, SInt32 handle, TESForm* form, TESObjectREFR* target,
		             VMArray<float> offset, VMArray<float> rotation, SInt32 count, bool bForcePersist, bool bInitiallyDisabled);

	// Latent - returns VMResultArray<float>
	bool	Run(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, SInt32 handle);

	void RegisterFuncs(VMClassRegistry* registry);
}