#include "PapyrusActiveMagicEffect.h"
 
#include "GameForms.h"
#include "GameObjects.h"
#include "GameRTTI.h"
#include "PapyrusEvents.h"
  
namespace papyrusActiveMagicEffect
{
	float GetTimeElapsed(ActiveEffect* effect)
	{
		return (effect) ? effect->elapsed : 0.0;
	}

	float GetDuration(ActiveEffect* effect)
	{
		return (effect) ? effect->duration : 0.0;
	}

	float GetMagnitude(ActiveEffect* effect)
	{
		return (effect) ? effect->magnitude : 0.0;
	}

	void RegisterForKey(ActiveEffect * effect, UInt32 key)
	{
		if(!effect)
			return;
		g_inputKeyEventRegs.Register<ActiveEffect>(key, ActiveEffect::kTypeID, effect);
	}

	void UnregisterForKey(ActiveEffect * effect, UInt32 key)
	{
		if(!effect)
			return;
		g_inputKeyEventRegs.Unregister<ActiveEffect>(key, ActiveEffect::kTypeID, effect);
	}

	void UnregisterForAllKeys(ActiveEffect * effect)
	{
		if(!effect)
			return;
		g_inputKeyEventRegs.UnregisterAll<ActiveEffect>(ActiveEffect::kTypeID, effect);
	}

	void RegisterForControl(ActiveEffect * effect, BSFixedString control)
	{
		if(!effect)
			return;
		g_inputControlEventRegs.Register<ActiveEffect>(control, ActiveEffect::kTypeID, effect);
	}

	void UnregisterForControl(ActiveEffect * effect, BSFixedString control)
	{
		if(!effect)
			return;
		g_inputControlEventRegs.Unregister<ActiveEffect>(control, ActiveEffect::kTypeID, effect);
	}

	void UnregisterForAllControls(ActiveEffect * effect)
	{
		if(!effect)
			return;
		g_inputControlEventRegs.UnregisterAll<ActiveEffect>(ActiveEffect::kTypeID, effect);
	}

	void RegisterForMenu(ActiveEffect * effect, BSFixedString menuName)
	{
		if(!effect || !menuName.data)
			return;

		g_menuOpenCloseRegs.Register<ActiveEffect>(menuName, ActiveEffect::kTypeID, effect);
	}

	void UnregisterForMenu(ActiveEffect * effect, BSFixedString menuName)
	{
		if(!effect || !menuName.data)
			return;

		g_menuOpenCloseRegs.Unregister<ActiveEffect>(menuName, ActiveEffect::kTypeID, effect);
	}

	void UnregisterForAllMenus(ActiveEffect * effect)
	{
		if(!effect)
			return;

		g_menuOpenCloseRegs.UnregisterAll<ActiveEffect>(ActiveEffect::kTypeID, effect);
	}

	void RegisterForModEvent(ActiveEffect * effect, BSFixedString eventName, BSFixedString callbackName)
	{
		if(!effect || !eventName.data || !callbackName.data)
			return;

		ModCallbackParameters params;
		params.callbackName = callbackName;

		g_modCallbackRegs.Register<ActiveEffect>(eventName, ActiveEffect::kTypeID, effect, &params);
	}

	void UnregisterForModEvent(ActiveEffect * effect, BSFixedString eventName)
	{
		if(!effect || !eventName.data)
			return;

		g_modCallbackRegs.Unregister<ActiveEffect>(eventName, ActiveEffect::kTypeID, effect);
	}

	void UnregisterForAllModEvents(ActiveEffect * effect)
	{
		g_modCallbackRegs.UnregisterAll<ActiveEffect>(ActiveEffect::kTypeID, effect);
	}

	void SendModEvent(ActiveEffect * thisForm, BSFixedString eventName, BSFixedString strArg, float numArg)
	{
		if (!eventName.data)
			return;

		SKSEModCallbackEvent evn(eventName, strArg, numArg, NULL);
		g_modCallbackEventDispatcher.SendEvent(&evn);
	}

	void RegisterForCameraState(ActiveEffect * thisForm)
	{
		if(!thisForm)
			return;

		g_cameraEventRegs.Register<ActiveEffect>(ActiveEffect::kTypeID, thisForm);
	}

	void UnregisterForCameraState(ActiveEffect * thisForm)
	{
		if(!thisForm)
			return;

		g_cameraEventRegs.Unregister<ActiveEffect>(ActiveEffect::kTypeID, thisForm);
	}

	void RegisterForCrosshairRef(ActiveEffect * thisForm)
	{
		if(!thisForm)
			return;

		g_crosshairRefEventRegs.Register<ActiveEffect>(ActiveEffect::kTypeID, thisForm);
	}

	void UnregisterForCrosshairRef(ActiveEffect * thisForm)
	{
		if(!thisForm)
			return;

		g_crosshairRefEventRegs.Unregister<ActiveEffect>(ActiveEffect::kTypeID, thisForm);
	}

	void RegisterForActorAction(ActiveEffect * thisForm, UInt32 actionType)
	{
		if(!thisForm)
			return;

		g_actionEventRegs.Register<ActiveEffect>(actionType, ActiveEffect::kTypeID, thisForm);
	}

	void UnregisterForActorAction(ActiveEffect * thisForm, UInt32 actionType)
	{
		if(!thisForm)
			return;

		g_actionEventRegs.Unregister<ActiveEffect>(actionType, ActiveEffect::kTypeID, thisForm);
	}

	void UnregisterForNiNodeUpdate(ActiveEffect * thisForm)
	{
		if(!thisForm)
			return;

		g_ninodeUpdateEventRegs.Unregister<ActiveEffect>(ActiveEffect::kTypeID, thisForm);
	}

	void RegisterForNiNodeUpdate(ActiveEffect * thisForm)
	{
		if(!thisForm)
			return;

		g_ninodeUpdateEventRegs.Register<ActiveEffect>(ActiveEffect::kTypeID, thisForm);
	}
}
 
#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"
 
void papyrusActiveMagicEffect::RegisterFuncs(VMClassRegistry* registry)
{
	registry->RegisterFunction(
		new NativeFunction0 <ActiveEffect, float>("GetTimeElapsed", "ActiveMagicEffect", papyrusActiveMagicEffect::GetTimeElapsed, registry));

	registry->RegisterFunction(
		new NativeFunction0 <ActiveEffect, float>("GetDuration", "ActiveMagicEffect", papyrusActiveMagicEffect::GetDuration, registry));

	registry->RegisterFunction(
		new NativeFunction0 <ActiveEffect, float>("GetMagnitude", "ActiveMagicEffect", papyrusActiveMagicEffect::GetMagnitude, registry));

	registry->RegisterFunction(
		new NativeFunction1 <ActiveEffect, void, UInt32> ("RegisterForKey", "ActiveMagicEffect", papyrusActiveMagicEffect::RegisterForKey, registry));

	registry->RegisterFunction(
		new NativeFunction1 <ActiveEffect, void, UInt32> ("UnregisterForKey", "ActiveMagicEffect", papyrusActiveMagicEffect::UnregisterForKey, registry));

	registry->RegisterFunction(
		new NativeFunction0 <ActiveEffect, void> ("UnregisterForAllKeys", "ActiveMagicEffect", papyrusActiveMagicEffect::UnregisterForAllKeys, registry));

	registry->RegisterFunction(
		new NativeFunction1 <ActiveEffect, void, BSFixedString> ("RegisterForMenu", "ActiveMagicEffect", papyrusActiveMagicEffect::RegisterForMenu, registry));

	registry->RegisterFunction(
		new NativeFunction1 <ActiveEffect, void, BSFixedString> ("UnregisterForMenu", "ActiveMagicEffect", papyrusActiveMagicEffect::UnregisterForMenu, registry));

	registry->RegisterFunction(
		new NativeFunction0 <ActiveEffect, void> ("UnregisterForAllMenus", "ActiveMagicEffect", papyrusActiveMagicEffect::UnregisterForAllMenus, registry));

	registry->RegisterFunction(
		new NativeFunction2 <ActiveEffect, void, BSFixedString, BSFixedString> ("RegisterForModEvent", "ActiveMagicEffect", papyrusActiveMagicEffect::RegisterForModEvent, registry));

	registry->RegisterFunction(
		new NativeFunction1 <ActiveEffect, void, BSFixedString> ("UnregisterForModEvent", "ActiveMagicEffect", papyrusActiveMagicEffect::UnregisterForModEvent, registry));

	registry->RegisterFunction(
		new NativeFunction0 <ActiveEffect, void> ("UnregisterForAllModEvents", "ActiveMagicEffect", papyrusActiveMagicEffect::UnregisterForAllModEvents, registry));

	registry->RegisterFunction(
		new NativeFunction3 <ActiveEffect, void, BSFixedString, BSFixedString, float> ("SendModEvent", "ActiveMagicEffect", papyrusActiveMagicEffect::SendModEvent, registry));

	registry->RegisterFunction(
		new NativeFunction1 <ActiveEffect, void, BSFixedString> ("RegisterForControl", "ActiveMagicEffect", papyrusActiveMagicEffect::RegisterForControl, registry));

	registry->RegisterFunction(
		new NativeFunction1 <ActiveEffect, void, BSFixedString> ("UnregisterForControl", "ActiveMagicEffect", papyrusActiveMagicEffect::UnregisterForControl, registry));

	registry->RegisterFunction(
		new NativeFunction0 <ActiveEffect, void> ("UnregisterForAllControls", "ActiveMagicEffect", papyrusActiveMagicEffect::UnregisterForAllControls, registry));

	registry->RegisterFunction(
		new NativeFunction0 <ActiveEffect, void> ("RegisterForCameraState", "ActiveMagicEffect", papyrusActiveMagicEffect::RegisterForCameraState, registry));

	registry->RegisterFunction(
		new NativeFunction0 <ActiveEffect, void> ("UnregisterForCameraState", "ActiveMagicEffect", papyrusActiveMagicEffect::UnregisterForCameraState, registry));

	registry->RegisterFunction(
		new NativeFunction0 <ActiveEffect, void> ("RegisterForCrosshairRef", "ActiveMagicEffect", papyrusActiveMagicEffect::RegisterForCrosshairRef, registry));

	registry->RegisterFunction(
		new NativeFunction0 <ActiveEffect, void> ("UnregisterForCrosshairRef", "ActiveMagicEffect", papyrusActiveMagicEffect::UnregisterForCrosshairRef, registry));

	registry->RegisterFunction(
		new NativeFunction1 <ActiveEffect, void, UInt32> ("RegisterForActorAction", "ActiveMagicEffect", papyrusActiveMagicEffect::RegisterForActorAction, registry));

	registry->RegisterFunction(
		new NativeFunction1 <ActiveEffect, void, UInt32> ("UnregisterForActorAction", "ActiveMagicEffect", papyrusActiveMagicEffect::UnregisterForActorAction, registry));

	registry->RegisterFunction(
		new NativeFunction0 <ActiveEffect, void> ("RegisterForNiNodeUpdate", "ActiveMagicEffect", papyrusActiveMagicEffect::RegisterForNiNodeUpdate, registry));

	registry->RegisterFunction(
		new NativeFunction0 <ActiveEffect, void> ("UnregisterForNiNodeUpdate", "ActiveMagicEffect", papyrusActiveMagicEffect::UnregisterForNiNodeUpdate, registry));

	registry->SetFunctionFlags("ActiveMagicEffect", "RegisterForKey", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ActiveMagicEffect", "UnregisterForKey", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ActiveMagicEffect", "UnregisterForAllKeys", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ActiveMagicEffect", "RegisterForMenu", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ActiveMagicEffect", "UnregisterForMenu", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ActiveMagicEffect", "UnregisterForAllMenus", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ActiveMagicEffect", "RegisterForModEvent", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ActiveMagicEffect", "UnregisterForAllModEvents", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ActiveMagicEffect", "SendModEvent", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ActiveMagicEffect", "RegisterForControl", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ActiveMagicEffect", "UnregisterForControl", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ActiveMagicEffect", "UnregisterForAllControls", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ActiveMagicEffect", "RegisterForCameraState", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ActiveMagicEffect", "UnregisterForCameraState", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ActiveMagicEffect", "RegisterForCrosshairRef", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ActiveMagicEffect", "UnregisterForCrosshairRef", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ActiveMagicEffect", "RegisterForActorAction", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ActiveMagicEffect", "UnregisterForActorAction", VMClassRegistry::kFunctionFlag_NoWait);
}
