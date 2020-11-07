#pragma once

class VMClassRegistry;
class TESForm;
class BGSDefaultObjectManager;

#include "skse64/GameTypes.h"

namespace papyrusDefaultObjectManager
{
	void RegisterFuncs(VMClassRegistry* registry);

	void SetForm(BGSDefaultObjectManager * objectManager, BSFixedString keySearch, TESForm * form);
	TESForm * GetForm(BGSDefaultObjectManager * objectManager, BSFixedString keySearch);
};
