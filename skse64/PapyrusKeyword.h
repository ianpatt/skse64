#pragma once

#include "GameTypes.h"

class BGSKeyword;
struct StaticFunctionTag;
class VMClassRegistry;

namespace papyrusKeyword
{
	void RegisterFuncs(VMClassRegistry* registry);

	BGSKeyword* GetKeyword(StaticFunctionTag*, BSFixedString keyword);
	BSFixedString GetString(BGSKeyword* thisKeyword);
}
