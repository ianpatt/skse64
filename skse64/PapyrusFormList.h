#pragma once

#include "skse64/GameTypes.h"
#include "skse64/PapyrusArgs.h"

class BGSListForm;
class VMClassRegistry;

namespace papyrusFormList
{
	void RegisterFuncs(VMClassRegistry* registry);

	VMResultArray<TESForm*> ToArray(BGSListForm * list);
}