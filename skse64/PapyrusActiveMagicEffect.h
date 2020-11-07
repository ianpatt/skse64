#pragma once
 
#include "skse64/GameTypes.h"

class ActiveEffect;
class VMClassRegistry;
class TESForm;
 
namespace papyrusActiveMagicEffect
{
	void RegisterFuncs(VMClassRegistry* registry);

	float GetTimeElapsed(ActiveEffect* effect);
	float GetDuration(ActiveEffect* effect);
	float GetMagnitude(ActiveEffect* effect);

	void RegisterForKey(ActiveEffect * effect, UInt32 key);
	void UnregisterForKey(ActiveEffect * effect, UInt32 key);
	void UnregisterForAllKeys(ActiveEffect * effect);

	void RegisterForControl(ActiveEffect * effect, BSFixedString control);
	void UnregisterForControl(ActiveEffect * effect, BSFixedString control);
	void UnregisterForAllControls(ActiveEffect * effect);

	void RegisterForMenu(ActiveEffect * effect, BSFixedString menuName);
	void UnregisterForMenu(ActiveEffect * effect, BSFixedString menuName);
	void UnregisterForAllMenus(ActiveEffect * effect);

	void RegisterForModEvent(ActiveEffect * effect, BSFixedString eventName, BSFixedString callbackName);
	void UnregisterForModEvent(ActiveEffect * effect, BSFixedString eventName);
	void UnregisterForAllModEvents(ActiveEffect * effect);

	void SendModEvent(ActiveEffect * thisForm, BSFixedString eventName, BSFixedString strArg, float numArg);

	void RegisterForCameraState(ActiveEffect * thisForm);
	void UnregisterForCameraState(ActiveEffect * thisForm);

	void RegisterForCrosshairRef(ActiveEffect * thisForm);
	void UnregisterForCrosshairRef(ActiveEffect * thisForm);
}
