#include "PapyrusConstructibleObject.h"

#include "GameForms.h"
#include "GameObjects.h"
#include "GameRTTI.h"

namespace papyrusConstructibleObject
{
	TESForm* GetResult(BGSConstructibleObject* recipe)
	{
		if(!recipe || !recipe->createdObject)
			return NULL;

		return recipe->createdObject;
	}

	void SetResult(BGSConstructibleObject* recipe, TESForm* form)
	{
		if(recipe && form) {
			TESBoundObject* result = DYNAMIC_CAST(form, TESForm, TESBoundObject); // Only Base Objects allowed
			if(result) {
				recipe->createdObject = result;
			}
		}
	}

	UInt32 GetResultQuantity(BGSConstructibleObject* recipe)
	{
		if(!recipe || !recipe->createdObject)
			return 0;

		return (UInt32)recipe->quantity;
	}

	void SetResultQuantity(BGSConstructibleObject* recipe, UInt32 quantity)
	{
		if(recipe && quantity < USHRT_MAX) {
			recipe->quantity = quantity;
		}
	}

	UInt32 GetNumIngredients(BGSConstructibleObject* recipe)
	{
		if(!recipe)
			return 0;

		return recipe->container.numEntries;
	}

	TESForm* GetNthIngredient(BGSConstructibleObject* recipe, UInt32 n)
	{
		if(!recipe || n > recipe->container.numEntries)
			return NULL;

		return recipe->container.entries[n]->form;
	}

	void SetNthIngredient(BGSConstructibleObject* recipe, TESForm* form, UInt32 n)
	{
		if(recipe && form && n <= recipe->container.numEntries) {
			TESBoundObject* result = DYNAMIC_CAST(form, TESForm, TESBoundObject); // Only Base Objects allowed
			if(result) {
				recipe->container.entries[n]->form = result;
			}
		}
	}

	UInt32 GetNthIngredientQuantity(BGSConstructibleObject* recipe, UInt32 n)
	{
		if(!recipe || n > recipe->container.numEntries)
			return 0;

		return recipe->container.entries[n]->count;
	}

	void SetNthIngredientQuantity(BGSConstructibleObject* recipe, UInt32 value, UInt32 n)
	{
		if(recipe && n <= recipe->container.numEntries) {
			recipe->container.entries[n]->count = value;
		}
	}

	BGSKeyword* GetWorkbenchKeyword(BGSConstructibleObject* recipe)
	{
		if(!recipe)
			return NULL;

		return recipe->wbKeyword;
	}

	void SetWorkbenchKeyword(BGSConstructibleObject* recipe, BGSKeyword* keyword)
	{
		if(recipe && keyword) {
			recipe->wbKeyword = keyword;
		}
	}
}

#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

void papyrusConstructibleObject::RegisterFuncs(VMClassRegistry* registry)
{
	registry->RegisterFunction(
		new NativeFunction0 <BGSConstructibleObject, TESForm*>("GetResult", "ConstructibleObject", papyrusConstructibleObject::GetResult, registry));

	registry->RegisterFunction(
		new NativeFunction1 <BGSConstructibleObject, void, TESForm*>("SetResult", "ConstructibleObject", papyrusConstructibleObject::SetResult, registry));

	registry->RegisterFunction(
		new NativeFunction0 <BGSConstructibleObject, UInt32>("GetResultQuantity", "ConstructibleObject", papyrusConstructibleObject::GetResultQuantity, registry));

	registry->RegisterFunction(
		new NativeFunction1 <BGSConstructibleObject, void, UInt32>("SetResultQuantity", "ConstructibleObject", papyrusConstructibleObject::SetResultQuantity, registry));

	registry->RegisterFunction(
		new NativeFunction0 <BGSConstructibleObject, UInt32>("GetNumIngredients", "ConstructibleObject", papyrusConstructibleObject::GetNumIngredients, registry));

	registry->RegisterFunction(
		new NativeFunction1 <BGSConstructibleObject, TESForm*, UInt32>("GetNthIngredient", "ConstructibleObject", papyrusConstructibleObject::GetNthIngredient, registry));

	registry->RegisterFunction(
		new NativeFunction2 <BGSConstructibleObject, void, TESForm*, UInt32>("SetNthIngredient", "ConstructibleObject", papyrusConstructibleObject::SetNthIngredient, registry));

	registry->RegisterFunction(
		new NativeFunction1 <BGSConstructibleObject, UInt32, UInt32>("GetNthIngredientQuantity", "ConstructibleObject", papyrusConstructibleObject::GetNthIngredientQuantity, registry));

	registry->RegisterFunction(
		new NativeFunction2 <BGSConstructibleObject, void, UInt32, UInt32>("SetNthIngredientQuantity", "ConstructibleObject", papyrusConstructibleObject::SetNthIngredientQuantity, registry));

	registry->RegisterFunction(
		new NativeFunction0 <BGSConstructibleObject, BGSKeyword*>("GetWorkbenchKeyword", "ConstructibleObject", papyrusConstructibleObject::GetWorkbenchKeyword, registry));

	registry->RegisterFunction(
		new NativeFunction1 <BGSConstructibleObject, void, BGSKeyword*>("SetWorkbenchKeyword", "ConstructibleObject", papyrusConstructibleObject::SetWorkbenchKeyword, registry));
}
