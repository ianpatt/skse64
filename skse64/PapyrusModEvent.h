#pragma once

#include "GameTypes.h"

struct StaticFunctionTag;
class VMClassRegistry;

namespace papyrusModEvent
{
	SInt32	Create(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag* thisInput, BSFixedString eventName);
	bool	Send(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag* thisInput, SInt32 handle);
	void	Release(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag* thisInput, SInt32 handle);

	template <typename T>
	void	Push(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag* thisInput, SInt32 handle, T arg);

	void	RegisterFuncs(VMClassRegistry* registry);
	
}
