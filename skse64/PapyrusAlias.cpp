#include "PapyrusAlias.h"

#include "GameForms.h"
#include "PapyrusEvents.h"

namespace papyrusAlias
{
	BSFixedString GetName(BGSBaseAlias* thisAlias)
	{
		return (thisAlias) ? thisAlias->name.data : NULL;
	}

	UInt32 GetId(BGSBaseAlias* thisAlias)
	{
		return (thisAlias) ? thisAlias->aliasId : -1;
	}

	void RegisterForKey(BGSBaseAlias * thisAlias, UInt32 key)
	{
		if(!thisAlias)
			return;
		g_inputKeyEventRegs.Register<BGSBaseAlias>(key, BGSBaseAlias::kTypeID, thisAlias);
	}

	void UnregisterForKey(BGSBaseAlias * thisAlias, UInt32 key)
	{
		if(!thisAlias)
			return;
		g_inputKeyEventRegs.Unregister<BGSBaseAlias>(key, BGSBaseAlias::kTypeID, thisAlias);
	}

	void UnregisterForAllKeys(BGSBaseAlias * thisAlias)
	{
		if(!thisAlias)
			return;
		g_inputKeyEventRegs.UnregisterAll<BGSBaseAlias>(BGSBaseAlias::kTypeID, thisAlias);
	}

	void RegisterForControl(BGSBaseAlias * thisAlias, BSFixedString control)
	{
		if(!thisAlias)
			return;
		g_inputControlEventRegs.Register<BGSBaseAlias>(control, BGSBaseAlias::kTypeID, thisAlias);
	}

	void UnregisterForControl(BGSBaseAlias * thisAlias, BSFixedString control)
	{
		if(!thisAlias)
			return;
		g_inputControlEventRegs.Unregister<BGSBaseAlias>(control, BGSBaseAlias::kTypeID, thisAlias);
	}

	void UnregisterForAllControls(BGSBaseAlias * thisAlias)
	{
		if(!thisAlias)
			return;
		g_inputControlEventRegs.UnregisterAll<BGSBaseAlias>(BGSBaseAlias::kTypeID, thisAlias);
	}

	void RegisterForMenu(BGSBaseAlias * thisAlias, BSFixedString menuName)
	{
		if(!thisAlias || !menuName.data)
			return;

		g_menuOpenCloseRegs.Register<BGSBaseAlias>(menuName, BGSBaseAlias::kTypeID, thisAlias);
	}

	void UnregisterForMenu(BGSBaseAlias * thisAlias, BSFixedString menuName)
	{
		if(!thisAlias || !menuName.data)
			return;

		g_menuOpenCloseRegs.Unregister<BGSBaseAlias>(menuName, BGSBaseAlias::kTypeID, thisAlias);
	}

	void UnregisterForAllMenus(BGSBaseAlias * thisAlias)
	{
		if(!thisAlias)
			return;

		g_menuOpenCloseRegs.UnregisterAll<BGSBaseAlias>(BGSBaseAlias::kTypeID, thisAlias);
	}

	void RegisterForModEvent(BGSBaseAlias * thisAlias, BSFixedString eventName, BSFixedString callbackName)
	{
		if(!thisAlias || !eventName.data || !callbackName.data)
			return;

		ModCallbackParameters params;
		params.callbackName = callbackName;

		g_modCallbackRegs.Register<BGSBaseAlias>(eventName, BGSBaseAlias::kTypeID, thisAlias, &params);
	}

	void UnregisterForModEvent(BGSBaseAlias * thisAlias, BSFixedString eventName)
	{
		if(!thisAlias || !eventName.data)
			return;

		g_modCallbackRegs.Unregister<BGSBaseAlias>(eventName, BGSBaseAlias::kTypeID, thisAlias);
	}

	void UnregisterForAllModEvents(BGSBaseAlias * thisAlias)
	{
		g_modCallbackRegs.UnregisterAll<BGSBaseAlias>(BGSBaseAlias::kTypeID, thisAlias);
	}

	void SendModEvent(BGSBaseAlias * thisAlias, BSFixedString eventName, BSFixedString strArg, float numArg)
	{
		if (!thisAlias || !eventName.data)
			return;

		SKSEModCallbackEvent evn(eventName, strArg, numArg, thisAlias->owner);
		g_modCallbackEventDispatcher.SendEvent(&evn);
	}

	void RegisterForCameraState(BGSBaseAlias * thisForm)
	{
		if(!thisForm)
			return;

		g_cameraEventRegs.Register<BGSBaseAlias>(BGSBaseAlias::kTypeID, thisForm);
	}

	void UnregisterForCameraState(BGSBaseAlias * thisForm)
	{
		if(!thisForm)
			return;

		g_cameraEventRegs.Unregister<BGSBaseAlias>(BGSBaseAlias::kTypeID, thisForm);
	}

	void RegisterForCrosshairRef(BGSBaseAlias * thisForm)
	{
		if(!thisForm)
			return;

		g_crosshairRefEventRegs.Register<BGSBaseAlias>(BGSBaseAlias::kTypeID, thisForm);
	}

	void UnregisterForCrosshairRef(BGSBaseAlias * thisForm)
	{
		if(!thisForm)
			return;

		g_crosshairRefEventRegs.Unregister<BGSBaseAlias>(BGSBaseAlias::kTypeID, thisForm);
	}

	void RegisterForActorAction(BGSBaseAlias * thisForm, UInt32 actionType)
	{
		if(!thisForm)
			return;

		g_actionEventRegs.Register<BGSBaseAlias>(actionType, BGSBaseAlias::kTypeID, thisForm);
	}

	void UnregisterForActorAction(BGSBaseAlias * thisForm, UInt32 actionType)
	{
		if(!thisForm)
			return;

		g_actionEventRegs.Unregister<BGSBaseAlias>(actionType, BGSBaseAlias::kTypeID, thisForm);
	}

	void UnregisterForNiNodeUpdate(BGSBaseAlias * thisForm)
	{
		if(!thisForm)
			return;

		g_ninodeUpdateEventRegs.Unregister<BGSBaseAlias>(BGSBaseAlias::kTypeID, thisForm);
	}

	void RegisterForNiNodeUpdate(BGSBaseAlias * thisForm)
	{
		if(!thisForm)
			return;

		g_ninodeUpdateEventRegs.Register<BGSBaseAlias>(BGSBaseAlias::kTypeID, thisForm);
	}
}


#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

void papyrusAlias::RegisterFuncs(VMClassRegistry* registry)
{
	registry->RegisterFunction(
		new NativeFunction0<BGSBaseAlias, BSFixedString> ("GetName", "Alias", papyrusAlias::GetName, registry));

	registry->RegisterFunction(
		new NativeFunction0<BGSBaseAlias, UInt32> ("GetId", "Alias", papyrusAlias::GetId, registry));

	registry->RegisterFunction(
		new NativeFunction1 <BGSBaseAlias, void, UInt32> ("RegisterForKey", "Alias", papyrusAlias::RegisterForKey, registry));

	registry->RegisterFunction(
		new NativeFunction1 <BGSBaseAlias, void, UInt32> ("UnregisterForKey", "Alias", papyrusAlias::UnregisterForKey, registry));

	registry->RegisterFunction(
		new NativeFunction0 <BGSBaseAlias, void> ("UnregisterForAllKeys", "Alias", papyrusAlias::UnregisterForAllKeys, registry));

	registry->RegisterFunction(
		new NativeFunction1 <BGSBaseAlias, void, BSFixedString> ("RegisterForMenu", "Alias", papyrusAlias::RegisterForMenu, registry));

	registry->RegisterFunction(
		new NativeFunction1 <BGSBaseAlias, void, BSFixedString> ("UnregisterForMenu", "Alias", papyrusAlias::UnregisterForMenu, registry));

	registry->RegisterFunction(
		new NativeFunction0 <BGSBaseAlias, void> ("UnregisterForAllMenus", "Alias", papyrusAlias::UnregisterForAllMenus, registry));

	registry->RegisterFunction(
		new NativeFunction2 <BGSBaseAlias, void, BSFixedString, BSFixedString> ("RegisterForModEvent", "Alias", papyrusAlias::RegisterForModEvent, registry));

	registry->RegisterFunction(
		new NativeFunction1 <BGSBaseAlias, void, BSFixedString> ("UnregisterForModEvent", "Alias", papyrusAlias::UnregisterForModEvent, registry));

	registry->RegisterFunction(
		new NativeFunction0 <BGSBaseAlias, void> ("UnregisterForAllModEvents", "Alias", papyrusAlias::UnregisterForAllModEvents, registry));

	registry->RegisterFunction(
		new NativeFunction3 <BGSBaseAlias, void, BSFixedString, BSFixedString, float> ("SendModEvent", "Alias", papyrusAlias::SendModEvent, registry));

	registry->RegisterFunction(
		new NativeFunction1 <BGSBaseAlias, void, BSFixedString> ("RegisterForControl", "Alias", papyrusAlias::RegisterForControl, registry));

	registry->RegisterFunction(
		new NativeFunction1 <BGSBaseAlias, void, BSFixedString> ("UnregisterForControl", "Alias", papyrusAlias::UnregisterForControl, registry));

	registry->RegisterFunction(
		new NativeFunction0 <BGSBaseAlias, void> ("UnregisterForAllControls", "Alias", papyrusAlias::UnregisterForAllControls, registry));

	registry->RegisterFunction(
		new NativeFunction0 <BGSBaseAlias, void> ("RegisterForCameraState", "Alias", papyrusAlias::RegisterForCameraState, registry));

	registry->RegisterFunction(
		new NativeFunction0 <BGSBaseAlias, void> ("UnregisterForCameraState", "Alias", papyrusAlias::UnregisterForCameraState, registry));

	registry->RegisterFunction(
		new NativeFunction0 <BGSBaseAlias, void> ("RegisterForCrosshairRef", "Alias", papyrusAlias::RegisterForCrosshairRef, registry));

	registry->RegisterFunction(
		new NativeFunction0 <BGSBaseAlias, void> ("UnregisterForCrosshairRef", "Alias", papyrusAlias::UnregisterForCrosshairRef, registry));

	registry->RegisterFunction(
		new NativeFunction1 <BGSBaseAlias, void, UInt32> ("RegisterForActorAction", "Alias", papyrusAlias::RegisterForActorAction, registry));

	registry->RegisterFunction(
		new NativeFunction1 <BGSBaseAlias, void, UInt32> ("UnregisterForActorAction", "Alias", papyrusAlias::UnregisterForActorAction, registry));

	registry->RegisterFunction(
		new NativeFunction0 <BGSBaseAlias, void> ("RegisterForNiNodeUpdate", "Alias", papyrusAlias::RegisterForNiNodeUpdate, registry));

	registry->RegisterFunction(
		new NativeFunction0 <BGSBaseAlias, void> ("UnregisterForNiNodeUpdate", "Alias", papyrusAlias::UnregisterForNiNodeUpdate, registry));

	registry->SetFunctionFlags("Alias", "RegisterForKey", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Alias", "UnregisterForKey", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Alias", "UnregisterForAllKeys", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Alias", "RegisterForMenu", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Alias", "UnregisterForMenu", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Alias", "UnregisterForAllMenus", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Alias", "RegisterForModEvent", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Alias", "UnregisterForAllModEvents", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Alias", "SendModEvent", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Alias", "RegisterForControl", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Alias", "UnregisterForControl", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Alias", "UnregisterForAllControls", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Alias", "RegisterForCameraState", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Alias", "UnregisterForCameraState", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Alias", "RegisterForCrosshairRef", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Alias", "UnregisterForCrosshairRef", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Alias", "RegisterForActorAction", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Alias", "UnregisterForActorAction", VMClassRegistry::kFunctionFlag_NoWait);
}