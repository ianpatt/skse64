#pragma once

class VMClassRegistry;
struct StaticFunctionTag;

#include "skse64/GameTypes.h"

namespace papyrusSKSE
{
	void RegisterFuncs(VMClassRegistry* registry);

	UInt32 GetVersion(StaticFunctionTag* base);
	UInt32 GetVersionMinor(StaticFunctionTag* base);
	UInt32 GetVersionBeta(StaticFunctionTag* base);
	UInt32 GetVersionRelease(StaticFunctionTag* base);
}
