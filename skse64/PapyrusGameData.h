#pragma once

class VMClassRegistry;
struct StaticFunctionTag;

#include "skse64/GameTypes.h"

namespace papyrusGameData
{
	void RegisterFuncs(VMClassRegistry* registry);
}