#pragma once

#include "GameTypes.h"

class BGSRefAlias;
class TESObjectREFR;
class VMClassRegistry;

namespace papyrusReferenceAlias
{
	void RegisterFuncs(VMClassRegistry* registry);

	bool ForceRefToWornItem(BGSRefAlias * alias, TESObjectREFR * reference, UInt32 mask);
}
