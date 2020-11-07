#pragma once

struct StaticFunctionTag;
class VMClassRegistry;

namespace papyrusMath 
{
	void RegisterFuncs(VMClassRegistry* registry);

	UInt32 LeftShift(StaticFunctionTag* base, UInt32 value, UInt32 shiftBy);
	UInt32 RightShift(StaticFunctionTag* base, UInt32 value, UInt32 shiftBy);
	UInt32 LogicalAnd(StaticFunctionTag* base, UInt32 arg1, UInt32 arg2);
	UInt32 LogicalOr(StaticFunctionTag* base, UInt32 arg1, UInt32 arg2);
	UInt32 LogicalXor(StaticFunctionTag* base, UInt32 arg1, UInt32 arg2);
	UInt32 LogicalNot(StaticFunctionTag* base, UInt32 arg1);
	float Log(StaticFunctionTag* base, float arg1);
}
