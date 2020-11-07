#pragma once

#include "GameTypes.h"

class BGSBaseAlias;
class VMClassRegistry;

namespace papyrusAlias
{
	void RegisterFuncs(VMClassRegistry* registry);

	BSFixedString GetName(BGSBaseAlias* thisAlias);
	UInt32 GetId(BGSBaseAlias* thisAlias);

	void RegisterForKey(BGSBaseAlias * thisAlias, UInt32 key);
	void UnregisterForKey(BGSBaseAlias * thisAlias, UInt32 key);
	void UnregisterForAllKeys(BGSBaseAlias * thisAlias);

	void RegisterForControl(BGSBaseAlias * thisAlias, BSFixedString control);
	void UnregisterForControl(BGSBaseAlias * thisAlias, BSFixedString control);
	void UnregisterForAllControls(BGSBaseAlias * thisAlias);

	void RegisterForMenu(BGSBaseAlias * thisAlias, BSFixedString menuName);
	void UnregisterForMenu(BGSBaseAlias * thisAlias, BSFixedString menuName);
	void UnregisterForAllMenus(BGSBaseAlias * thisAlias);

	void RegisterForModEvent(BGSBaseAlias * thisAlias, BSFixedString eventName, BSFixedString callbackName);
	void UnregisterForModEvent(BGSBaseAlias * thisAlias, BSFixedString eventName);
	void UnregisterForAllModEvents(BGSBaseAlias * thisAlias);

	void SendModEvent(BGSBaseAlias * thisAlias, BSFixedString eventName, BSFixedString strArg, float numArg);

	void RegisterForCameraState(BGSBaseAlias * thisForm);
	void UnregisterForCameraState(BGSBaseAlias * thisForm);

	void RegisterForCrosshairRef(BGSBaseAlias * thisForm);
	void UnregisterForCrosshairRef(BGSBaseAlias * thisForm);
}
