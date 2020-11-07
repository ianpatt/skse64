#pragma once

class VMClassRegistry;
class TESSound;
class BGSSoundDescriptorForm;

namespace papyrusSound
{
	BGSSoundDescriptorForm * GetDescriptor(TESSound* thisSound);
	void RegisterFuncs(VMClassRegistry* registry);
};