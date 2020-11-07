#pragma once
#include "GameTypes.h"

struct StaticFunctionTag;

class VMClassRegistry;

namespace papyrusUtility {

	void RegisterFuncs(VMClassRegistry* registry);

	float GetINIFloat(StaticFunctionTag* base, BSFixedString ini);
	UInt32 GetINIInt(StaticFunctionTag* base, BSFixedString ini);
	bool GetINIBool(StaticFunctionTag* base, BSFixedString ini);
	BSFixedString GetINIString(StaticFunctionTag* base, BSFixedString ini);
}
