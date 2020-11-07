#pragma once
#include "GameTypes.h"

struct StaticFunctionTag;
class VMClassRegistry;


namespace papyrusStringUtil
{
	void RegisterFuncs(VMClassRegistry* registry);


	UInt32 GetLength(StaticFunctionTag* thisInput, BSFixedString s);
	BSFixedString GetNthChar(StaticFunctionTag* thisInput, BSFixedString s, UInt32 index);

	bool IsUppercase(StaticFunctionTag* thisInput, BSFixedString s);
	bool IsLowercase(StaticFunctionTag* thisInput, BSFixedString s);
	bool IsLetter(StaticFunctionTag* thisInput, BSFixedString s);
	bool IsDigit(StaticFunctionTag* thisInput, BSFixedString s);
	bool IsPunctuation(StaticFunctionTag* thisInput, BSFixedString s);
	bool IsPrintable(StaticFunctionTag* thisInput, BSFixedString s);
	BSFixedString ToUpper(StaticFunctionTag* thisInput, BSFixedString s);
	BSFixedString ToLower(StaticFunctionTag* thisInput, BSFixedString s);

	SInt32 Find(StaticFunctionTag* thisInput, BSFixedString theString, BSFixedString toFind, UInt32 startIndex = 0);
	BSFixedString Substring(StaticFunctionTag* thisInput, BSFixedString s, UInt32 startIndex, UInt32 len = 0);

	UInt32 AsOrd(StaticFunctionTag* thisInput, BSFixedString s);
	BSFixedString AsChar(StaticFunctionTag* thisInput, UInt32 c);
}
