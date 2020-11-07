#pragma once

class TESObjectCELL;
class TESObjectREFR;
class VMClassRegistry;

namespace papyrusCell
{
	void RegisterFuncs(VMClassRegistry* registry);
	UInt32 GetNumRefs(TESObjectCELL* thisCell, UInt32 formType);
	TESObjectREFR* GetNthRef(TESObjectCELL* thisCell, UInt32 index, UInt32 formType);
}
