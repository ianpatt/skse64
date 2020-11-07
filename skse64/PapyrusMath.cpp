#include "PapyrusMath.h"
#include <math.h>

namespace papyrusMath
{
	UInt32 LeftShift(StaticFunctionTag* base, UInt32 value, UInt32 shiftBy)
	{
		return (shiftBy >= 32) ? 0 : value << shiftBy;
	}

	UInt32 RightShift(StaticFunctionTag* base, UInt32 value, UInt32 shiftBy)
	{
		return (shiftBy >= 32) ? 0 : value >> shiftBy;
	}

	UInt32 LogicalAnd(StaticFunctionTag* base, UInt32 arg1, UInt32 arg2)
	{
		return arg1 & arg2;
	}

	UInt32 LogicalOr(StaticFunctionTag* base, UInt32 arg1, UInt32 arg2)
	{
		return arg1 | arg2;
	}

	UInt32 LogicalXor(StaticFunctionTag* base, UInt32 arg1, UInt32 arg2)
	{
		return arg1 ^ arg2;
	}

	UInt32 LogicalNot(StaticFunctionTag* base, UInt32 arg1)
	{
		return ~arg1;
	}

	float Log(StaticFunctionTag* base, float arg1)
	{
		return log(arg1);
	}
}

#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

void papyrusMath::RegisterFuncs(VMClassRegistry* registry)
{
	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, UInt32, UInt32, UInt32>("LeftShift", "Math", papyrusMath::LeftShift, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, UInt32, UInt32, UInt32>("RightShift", "Math", papyrusMath::RightShift, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, UInt32, UInt32, UInt32>("LogicalAnd", "Math", papyrusMath::LogicalAnd, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, UInt32, UInt32, UInt32>("LogicalOr", "Math", papyrusMath::LogicalOr, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, UInt32, UInt32, UInt32>("LogicalXor", "Math", papyrusMath::LogicalXor, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, UInt32, UInt32>("LogicalNot", "Math", papyrusMath::LogicalNot, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, float, float>("Log", "Math", papyrusMath::Log, registry));

	registry->SetFunctionFlags("Math", "LeftShift", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Math", "RightShift", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Math", "LogicalAnd", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Math", "LogicalOr", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Math", "LogicalXor", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Math", "LogicalNot", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Math", "Log", VMClassRegistry::kFunctionFlag_NoWait);
}
