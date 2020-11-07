#pragma once

class TESForm;
class BGSConstructibleObject;
class BGSKeyword;
class VMClassRegistry;

namespace papyrusConstructibleObject
{
	void RegisterFuncs(VMClassRegistry* registry);

	TESForm* GetResult(BGSConstructibleObject* recipe);
	void SetResult(BGSConstructibleObject* recipe, TESForm* result);
	UInt32 GetResultQuantity(BGSConstructibleObject* recipe);
	void SetResultQuantity(BGSConstructibleObject* recipe, UInt32 quantity);
	UInt32 GetNumIngredients(BGSConstructibleObject* recipe);
	TESForm* GetNthIngredient(BGSConstructibleObject* recipe, UInt32 n);
	void SetNthIngredient(BGSConstructibleObject* recipe, TESForm* form, UInt32 n);
	UInt32 GetNthIngredientQuantity(BGSConstructibleObject* recipe, UInt32 n);
	void SetNthIngredientQuantity(BGSConstructibleObject* recipe, UInt32 value, UInt32 n);
	BGSKeyword* GetWorkbenchKeyword(BGSConstructibleObject* recipe);
	void SetWorkbenchKeyword(BGSConstructibleObject* recipe, BGSKeyword* keyword);
}
