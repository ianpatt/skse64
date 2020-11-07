#pragma once

#include "GameTypes.h"

struct StaticFunctionTag;
class VMClassRegistry;
template <typename T> class VMArray;

namespace papyrusUICallback
{
	SInt32	Create(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag* thisInput, BSFixedString menuName, BSFixedString targetStr);
	bool	Send(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag* thisInput, SInt32 handle);
	void	Release(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag* thisInput, SInt32 handle);

	template <typename T>
	void	Push(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag* thisInput, SInt32 handle, T arg);

	template <typename T>
	void	PushArray(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag* thisInput, SInt32 handle, VMArray<T> args);

	void	RegisterFuncs(VMClassRegistry* registry);
}