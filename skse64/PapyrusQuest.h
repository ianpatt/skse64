#pragma once

#include "GameTypes.h"

class TESQuest;
class BGSBaseAlias;
class VMClassRegistry;
struct StaticFunctionTag;

namespace papyrusQuest
{
	void RegisterFuncs(VMClassRegistry* registry);

	TESQuest* GetQuest(StaticFunctionTag* thisInput, BSFixedString editorID);

	BSFixedString GetId(TESQuest* thisQuest);
	UInt32 GetPriority(TESQuest* thisQuest);
	UInt32 GetNumAliases(TESQuest* thisQuest);
	BGSBaseAlias* GetNthAlias(TESQuest* thisQuest, UInt32 n);
	BGSBaseAlias* GetAliasByName(TESQuest* thisQuest, BSFixedString s);
}
