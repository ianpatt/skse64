#include "PapyrusForm.h"

#include "GameForms.h"
#include "GameObjects.h"
#include "GameRTTI.h"
#include "PapyrusVM.h"
#include "PapyrusEvents.h"
#include "PapyrusArgs.h"
//#include "GameMenus.h"

#include <map>
#include <set>

namespace papyrusForm
{
	UInt32 GetType(TESForm* thisForm)
	{
		return (thisForm) ? thisForm->GetFormType() : 0;
	}

	BSFixedString GetName(TESForm* thisForm)
	{
		if (!thisForm)
			return NULL;
		
		TESFullName* pFullName = DYNAMIC_CAST(thisForm, TESForm, TESFullName);
		if (pFullName)
			return pFullName->name.data;
		return NULL;
	}

	void SetName(TESForm* thisForm, BSFixedString nuName)
	{
		TESFullName* pFullName = DYNAMIC_CAST(thisForm, TESForm, TESFullName);
		// is a const string, so have to just reassign it.
		if (pFullName) {
			pFullName->name = nuName;
		}
	}

	float GetWeight(TESForm* thisForm)
	{
		if (!thisForm)
			return 0.0;

		TESWeightForm* pWeight = DYNAMIC_CAST(thisForm, TESForm, TESWeightForm);
		if (pWeight)
			return pWeight->weight;
		return 0.0;
	}

	void SetWeight(TESForm* thisForm, float nuWeight)
	{
		if (!thisForm)
			return;

		TESWeightForm* pWeight = DYNAMIC_CAST(thisForm, TESForm, TESWeightForm);
		if (pWeight)
			pWeight->weight = nuWeight;
	}

	UInt32 GetBaseDamage(TESForm* thisForm)
	{
		if (!thisForm)
			return 0;
		TESAttackDamageForm* pAttack = DYNAMIC_CAST(thisForm, TESForm, TESAttackDamageForm);
		if (pAttack)
			return pAttack->GetAttackDamage();
		return 0;
	}

	UInt32 GetValue(TESForm* thisForm)
	{
		if (!thisForm)
			return 0;
		TESValueForm* pValue = DYNAMIC_CAST(thisForm, TESForm, TESValueForm);
		if (pValue)
			return pValue->value;
		else {
			AlchemyItem* alchemyItem = DYNAMIC_CAST(thisForm, TESForm, AlchemyItem);
			if(alchemyItem && (alchemyItem->itemData.flags & AlchemyItem::kFlag_ManualCalc) == AlchemyItem::kFlag_ManualCalc)
				return alchemyItem->itemData.value;
		}
		return 0;
	}

	void SetGoldValue(TESForm* thisForm, UInt32 value)
	{
		if (!thisForm)
			return;
		TESValueForm* pValue = DYNAMIC_CAST(thisForm, TESForm, TESValueForm);
		if (pValue)
			pValue->value = value;
		else {
			AlchemyItem* alchemyItem = DYNAMIC_CAST(thisForm, TESForm, AlchemyItem);
			if(alchemyItem && (alchemyItem->itemData.flags & AlchemyItem::kFlag_ManualCalc) == AlchemyItem::kFlag_ManualCalc)
				alchemyItem->itemData.value = value;
		}
	}

	UInt32 GetNumKeywords(TESForm* thisForm)
	{
		if (!thisForm)
			return 0;
		BGSKeywordForm* pKeywords = DYNAMIC_CAST(thisForm, TESForm, BGSKeywordForm);
		if (pKeywords)
			return pKeywords->numKeywords;
		return 0;
	}

	BGSKeyword* GetNthKeyword(TESForm* thisForm, UInt32 index)
	{
		if (!thisForm)
			return NULL;
		BGSKeywordForm* pKeywords = DYNAMIC_CAST(thisForm, TESForm, BGSKeywordForm);
		if (pKeywords && index < pKeywords->numKeywords) {
			return pKeywords->keywords[index];
		}
		return NULL;
	}

	typedef std::set <UInt64>				HandleList;
	typedef std::map <UInt32, HandleList>	RegisteredKeyMap;

	VMResultArray<BGSKeyword*> GetKeywords(TESForm* thisForm)
	{
		VMResultArray<BGSKeyword*> result;
		if (!thisForm)
			return result;

		BGSKeywordForm* pKeywords = DYNAMIC_CAST(thisForm, TESForm, BGSKeywordForm);
		if (pKeywords) {
			for(UInt32 i = 0; i < pKeywords->numKeywords; i++)
			{
				if(pKeywords->keywords[i])
					result.push_back(pKeywords->keywords[i]);
			}
		}
		return result;
	}

	void SetPlayerKnows(TESForm * thisForm, bool knows)
	{
		if(!thisForm)
			return;

		knows ? thisForm->flags |= TESForm::kFlagPlayerKnows : thisForm->flags &= ~TESForm::kFlagPlayerKnows;
	}

	void RegisterForKey(TESForm * thisForm, UInt32 key)
	{
		if(!thisForm)
			return;
		g_inputKeyEventRegs.Register<TESForm>(key, thisForm->GetFormType(), thisForm);
	}

	void UnregisterForKey(TESForm * thisForm, UInt32 key)
	{
		if(!thisForm)
			return;
		g_inputKeyEventRegs.Unregister<TESForm>(key, thisForm->GetFormType(), thisForm);
	}

	void UnregisterForAllKeys(TESForm * thisForm)
	{
		if(!thisForm)
			return;
		g_inputKeyEventRegs.UnregisterAll<TESForm>(thisForm->GetFormType(), thisForm);
	}


	void RegisterForControl(TESForm * thisForm, BSFixedString control)
	{
		if(!thisForm)
			return;
		g_inputControlEventRegs.Register<TESForm>(control, thisForm->GetFormType(), thisForm);
	}

	void UnregisterForControl(TESForm * thisForm, BSFixedString control)
		{
		if(!thisForm)
			return;
		g_inputControlEventRegs.Unregister<TESForm>(control, thisForm->GetFormType(), thisForm);
	}

	void UnregisterForAllControls(TESForm * thisForm)
	{
		if(!thisForm)
			return;
		g_inputControlEventRegs.UnregisterAll<TESForm>(thisForm->GetFormType(), thisForm);
	}

#if 0
	void UpdateKeys(UInt8 * data)
	{
		/*static UInt8	oldState[0x100] = { 0 };

		if(!*g_skyrimVM) return;

		// ### this is off because it's a super temp hack

		BSFixedString		eventName("OnKeyDown");
		VMClassRegistry		* registry =	(*g_skyrimVM)->GetClassRegistry();

		for(UInt32 i = 0; i < 0x100; i++)
		{
			if(!oldState[i] && data[i])
			{
				RegisteredKeyMap::iterator	registeredHandles = g_regKeyMap.find(i);
				if(registeredHandles != g_regKeyMap.end())
				{
					for(HandleList::iterator iter = registeredHandles->second.begin(); iter != registeredHandles->second.end(); ++iter)
					{
						OneIntArg	args(i);

						registry->QueueEvent(*iter, &eventName, &args);
					}
				}
			}

			oldState[i] = data[i];
		}*/
	}
#endif

	void RegisterForMenu(TESForm * thisForm, BSFixedString menuName)
	{
		if(!thisForm || !menuName.data)
			return;
		
		g_menuOpenCloseRegs.Register<TESForm>(menuName, thisForm->GetFormType(), thisForm);
	}

	void UnregisterForMenu(TESForm * thisForm, BSFixedString menuName)
	{
		if(!thisForm || !menuName.data)
			return;

		g_menuOpenCloseRegs.Unregister<TESForm>(menuName, thisForm->GetFormType(), thisForm);
	}

	void UnregisterForAllMenus(TESForm * thisForm)
	{
		if(!thisForm)
			return;

		g_menuOpenCloseRegs.UnregisterAll<TESForm>(thisForm->GetFormType(), thisForm);
	}

	void RegisterForModEvent(TESForm * thisForm, BSFixedString eventName, BSFixedString callbackName)
	{
		if(!thisForm || !eventName.data || !callbackName.data)
			return;

		ModCallbackParameters params;
		params.callbackName = callbackName;

		g_modCallbackRegs.Register<TESForm>(eventName, thisForm->GetFormType(), thisForm, &params);
	}

	void UnregisterForModEvent(TESForm * thisForm, BSFixedString eventName)
	{
		if(!thisForm || !eventName.data)
			return;

		g_modCallbackRegs.Unregister<TESForm>(eventName, thisForm->GetFormType(), thisForm);
	}

	void UnregisterForAllModEvents(TESForm * thisForm)
	{
		g_modCallbackRegs.UnregisterAll<TESForm>(thisForm->GetFormType(), thisForm);
	}

	void SendModEvent(TESForm * thisForm, BSFixedString eventName, BSFixedString strArg, float numArg)
	{
		if (!eventName.data)
			return;

		SKSEModCallbackEvent evn(eventName, strArg, numArg, thisForm);
		g_modCallbackEventDispatcher.SendEvent(&evn);
	}

	void RegisterForCameraState(TESForm * thisForm)
	{
		if(!thisForm)
			return;

		g_cameraEventRegs.Register<TESForm>(thisForm->GetFormType(), thisForm);
	}

	void UnregisterForCameraState(TESForm * thisForm)
	{
		if(!thisForm)
			return;

		g_cameraEventRegs.Unregister<TESForm>(thisForm->GetFormType(), thisForm);
	}

	void RegisterForCrosshairRef(TESForm * thisForm)
	{
		if(!thisForm)
			return;

		g_crosshairRefEventRegs.Register<TESForm>(thisForm->GetFormType(), thisForm);
	}

	void UnregisterForCrosshairRef(TESForm * thisForm)
	{
		if(!thisForm)
			return;

		g_crosshairRefEventRegs.Unregister<TESForm>(thisForm->GetFormType(), thisForm);
	}

	void RegisterForActorAction(TESForm * thisForm, UInt32 actionType)
	{
		if(!thisForm)
			return;

		g_actionEventRegs.Register<TESForm>(actionType, thisForm->GetFormType(), thisForm);
	}

	void UnregisterForActorAction(TESForm * thisForm, UInt32 actionType)
	{
		if(!thisForm)
			return;

		g_actionEventRegs.Unregister<TESForm>(actionType, thisForm->GetFormType(), thisForm);
	}


	TESForm * TempClone(TESForm * thisForm)
	{
		TESForm	* result = NULL;

		if(thisForm)
		{
			IFormFactory	* factory = IFormFactory::GetFactoryForType(thisForm->formType);
			if(factory)
			{
				result = factory->Create();
				if(result)
				{
					result->Init();
					result->CopyFrom(thisForm);
					result->CopyFromEx(thisForm);
				}
				else
				{
					_ERROR("Form::TempClone: factory for type %02X failed", thisForm->formType);
				}
			}
			else
			{
				_MESSAGE("Form::TempClone: tried to clone form %08X (type %02X), no factory found", thisForm->formID, thisForm->formType);
			}
		}

		return result;
	}

	bool HasWorldModel(TESForm * thisForm)
	{
		if(thisForm) {
			TESModel* pWorldModel = DYNAMIC_CAST(thisForm, TESForm, TESModel);
			if (pWorldModel)
				return true;
		}

		return false;
	}

	BSFixedString GetWorldModelPath(TESForm * thisForm)
	{
		if(!thisForm)
			return NULL;

		TESModel* pWorldModel = DYNAMIC_CAST(thisForm, TESForm, TESModel);
		if (!pWorldModel)
			return NULL;

		return pWorldModel->GetModelName();
	}

	void SetWorldModelPath(TESForm * thisForm, BSFixedString nuPath)
	{
		if(thisForm) {
			TESModel* pWorldModel = DYNAMIC_CAST(thisForm, TESForm, TESModel);
			if(pWorldModel)
				pWorldModel->SetModelName(nuPath.data);
		}
	}

	UInt32 GetWorldModelNumTextureSets(TESForm * thisForm)
	{
		if(!thisForm)
			return 0;

		TESModelTextureSwap* pTextSwap = DYNAMIC_CAST(thisForm, TESForm, TESModelTextureSwap);
		if (!pTextSwap)
			return 0;

		return pTextSwap->count;
	}

	BGSTextureSet* GetWorldModelNthTextureSet(TESForm * thisForm, UInt32 n)
	{
		if(!thisForm)
			return NULL;

		TESModelTextureSwap* pTextSwap = DYNAMIC_CAST(thisForm, TESForm, TESModelTextureSwap);
		if (!pTextSwap)
			return NULL;

		if(!pTextSwap->swaps || n >= pTextSwap->count)
			return NULL;

		return pTextSwap->swaps[n].textureSet;
	}

	void SetWorldModelNthTextureSet(TESForm * thisForm, BGSTextureSet* textureSet, UInt32 n)
	{
		if(thisForm && thisForm) {
			TESModelTextureSwap* pTextSwap = DYNAMIC_CAST(thisForm, TESForm, TESModelTextureSwap);
			if (pTextSwap) {
				if(pTextSwap->swaps && n < pTextSwap->count) {
					pTextSwap->swaps[n].textureSet = textureSet;
				}
			}
		}
	}

	bool IsPlayable(TESForm * thisForm)
	{
		return (thisForm) ? thisForm->IsPlayable() : false;
	}

	void RegisterForNiNodeUpdate(TESForm * thisForm)
	{
		if(!thisForm)
			return;

		g_ninodeUpdateEventRegs.Register<TESForm>(thisForm->GetFormType(), thisForm);
	}

	void UnregisterForNiNodeUpdate(TESForm * thisForm)
	{
		if(!thisForm)
			return;

		g_ninodeUpdateEventRegs.Unregister<TESForm>(thisForm->GetFormType(), thisForm);
	}
}

#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

void papyrusForm::RegisterFuncs(VMClassRegistry* registry)
{
	registry->RegisterFunction(
		new NativeFunction0 <TESForm, UInt32> ("GetType", "Form", papyrusForm::GetType, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESForm, BSFixedString> ("GetName", "Form", papyrusForm::GetName, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESForm, void, BSFixedString> ("SetName", "Form", papyrusForm::SetName, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESForm, float> ("GetWeight", "Form", papyrusForm::GetWeight, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESForm, void, float> ("SetWeight", "Form", papyrusForm::SetWeight, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESForm, void, UInt32> ("SetGoldValue", "Form", papyrusForm::SetGoldValue, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESForm, UInt32> ("GetNumKeywords", "Form", papyrusForm::GetNumKeywords, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESForm, BGSKeyword *, UInt32> ("GetNthKeyword", "Form", papyrusForm::GetNthKeyword, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESForm, VMResultArray<BGSKeyword*>> ("GetKeywords", "Form", papyrusForm::GetKeywords, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESForm, void, bool> ("SetPlayerKnows", "Form", papyrusForm::SetPlayerKnows, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESForm, void, UInt32> ("RegisterForKey", "Form", papyrusForm::RegisterForKey, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESForm, void, UInt32> ("UnregisterForKey", "Form", papyrusForm::UnregisterForKey, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESForm, void> ("UnregisterForAllKeys", "Form", papyrusForm::UnregisterForAllKeys, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESForm, void, BSFixedString> ("RegisterForMenu", "Form", papyrusForm::RegisterForMenu, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESForm, void, BSFixedString> ("UnregisterForMenu", "Form", papyrusForm::UnregisterForMenu, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESForm, void> ("UnregisterForAllMenus", "Form", papyrusForm::UnregisterForAllMenus, registry));

	registry->RegisterFunction(
		new NativeFunction2 <TESForm, void, BSFixedString, BSFixedString> ("RegisterForModEvent", "Form", papyrusForm::RegisterForModEvent, registry));

	registry->RegisterFunction(
		new NativeFunction3 <TESForm, void, BSFixedString, BSFixedString, float> ("SendModEvent", "Form", papyrusForm::SendModEvent, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESForm, void, BSFixedString> ("UnregisterForModEvent", "Form", papyrusForm::UnregisterForModEvent, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESForm, void> ("UnregisterForAllModEvents", "Form", papyrusForm::UnregisterForAllModEvents, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESForm, TESForm *> ("TempClone", "Form", papyrusForm::TempClone, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESForm, void, BSFixedString> ("RegisterForControl", "Form", papyrusForm::RegisterForControl, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESForm, void, BSFixedString> ("UnregisterForControl", "Form", papyrusForm::UnregisterForControl, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESForm, void> ("UnregisterForAllControls", "Form", papyrusForm::UnregisterForAllControls, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESForm, void> ("RegisterForCameraState", "Form", papyrusForm::RegisterForCameraState, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESForm, void> ("UnregisterForCameraState", "Form", papyrusForm::UnregisterForCameraState, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESForm, void> ("RegisterForCrosshairRef", "Form", papyrusForm::RegisterForCrosshairRef, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESForm, void> ("UnregisterForCrosshairRef", "Form", papyrusForm::UnregisterForCrosshairRef, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESForm, void, UInt32> ("RegisterForActorAction", "Form", papyrusForm::RegisterForActorAction, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESForm, void, UInt32> ("UnregisterForActorAction", "Form", papyrusForm::UnregisterForActorAction, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESForm, void> ("RegisterForNiNodeUpdate", "Form", papyrusForm::RegisterForNiNodeUpdate, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESForm, void> ("UnregisterForNiNodeUpdate", "Form", papyrusForm::UnregisterForNiNodeUpdate, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESForm, bool> ("HasWorldModel", "Form", papyrusForm::HasWorldModel, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESForm, BSFixedString> ("GetWorldModelPath", "Form", papyrusForm::GetWorldModelPath, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESForm, void, BSFixedString> ("SetWorldModelPath", "Form", papyrusForm::SetWorldModelPath, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESForm, UInt32> ("GetWorldModelNumTextureSets", "Form", papyrusForm::GetWorldModelNumTextureSets, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESForm, BGSTextureSet*, UInt32> ("GetWorldModelNthTextureSet", "Form", papyrusForm::GetWorldModelNthTextureSet, registry));

	registry->RegisterFunction(
		new NativeFunction2 <TESForm, void, BGSTextureSet*, UInt32> ("SetWorldModelNthTextureSet", "Form", papyrusForm::SetWorldModelNthTextureSet, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESForm, bool> ("IsPlayable", "Form", papyrusForm::IsPlayable, registry));

	registry->SetFunctionFlags("Form", "SetPlayerKnows", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Form", "RegisterForKey", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Form", "UnregisterForKey", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Form", "UnregisterForAllKeys", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Form", "RegisterForMenu", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Form", "UnregisterForMenu", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Form", "UnregisterForAllMenus", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Form", "RegisterForModEvent", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Form", "SendModEvent", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Form", "UnregisterForAllModEvents", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Form", "RegisterForControl", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Form", "UnregisterForControl", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Form", "UnregisterForAllControls", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Form", "RegisterForCameraState", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Form", "UnregisterForCameraState", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Form", "RegisterForCrosshairRef", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Form", "UnregisterForCrosshairRef", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Form", "RegisterForActorAction", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Form", "UnregisterForActorAction", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Form", "HasWorldModel", VMClassRegistry::kFunctionFlag_NoWait);
}
