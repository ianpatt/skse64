#include "PapyrusUtility.h"

#include "GameForms.h"
#include "GameObjects.h"
#include "GameReferences.h"
#include "GameExtraData.h"
#include "GameRTTI.h"
#include "GameSettings.h"

#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

namespace papyrusUtility
{
	float GetINIFloat(StaticFunctionTag* base, BSFixedString ini)
	{
		Setting * setting = GetINISetting(ini.data);
		if(!setting || setting->GetType() != Setting::kType_Float)
			return 0.0;

		return setting->data.f32;
	}

	UInt32 GetINIInt(StaticFunctionTag* base, BSFixedString ini)
	{
		Setting * setting = GetINISetting(ini.data);
		if(!setting || setting->GetType() != Setting::kType_Integer)
			return 0;

		return setting->data.s32;
	}

	bool GetINIBool(StaticFunctionTag* base, BSFixedString ini)
	{
		Setting * setting = GetINISetting(ini.data);
		if(!setting || setting->GetType() != Setting::kType_Bool)
			return false;

		return setting->data.u8 >= 1 ? true : false;
	}

	BSFixedString GetINIString(StaticFunctionTag* base, BSFixedString ini)
	{
		Setting * setting = GetINISetting(ini.data);
		if(!setting || setting->GetType() != Setting::kType_String)
			return NULL;

		return setting->data.s;
	}

	template<typename T>
	VMResultArray<T> CreateArray(StaticFunctionTag* base, UInt32 size, T fillValue)
	{
		VMResultArray<T> data;
		data.resize(size, fillValue);
		return data;
	}

	template<typename T>
	VMResultArray<T> ResizeArray(StaticFunctionTag* base, VMArray<T> inArray, UInt32 size, T fillValue)
	{
		VMResultArray<T> data;
		data.resize(size, fillValue);
		UInt32 arrayLength = min(inArray.Length(), size);
		for(UInt32 i = 0; i < arrayLength; i++)
		{
			T value;
			inArray.Get(&value, i);
			data[i] = value;
		}
		return data;
	}
}

void papyrusUtility::RegisterFuncs(VMClassRegistry* registry)
{
	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, float, BSFixedString>("GetINIFloat", "Utility", papyrusUtility::GetINIFloat, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, UInt32, BSFixedString>("GetINIInt", "Utility", papyrusUtility::GetINIInt, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, bool, BSFixedString>("GetINIBool", "Utility", papyrusUtility::GetINIBool, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, BSFixedString, BSFixedString>("GetINIString", "Utility", papyrusUtility::GetINIString, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, VMResultArray<float>, UInt32, float>("CreateFloatArray", "Utility", papyrusUtility::CreateArray<float>, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, VMResultArray<SInt32>, UInt32, SInt32>("CreateIntArray", "Utility", papyrusUtility::CreateArray<SInt32>, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, VMResultArray<bool>, UInt32, bool>("CreateBoolArray", "Utility", papyrusUtility::CreateArray<bool>, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, VMResultArray<BSFixedString>, UInt32, BSFixedString>("CreateStringArray", "Utility", papyrusUtility::CreateArray<BSFixedString>, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, VMResultArray<TESForm*>, UInt32, TESForm*>("CreateFormArray", "Utility", papyrusUtility::CreateArray<TESForm*>, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, VMResultArray<BGSBaseAlias*>, UInt32, BGSBaseAlias*>("CreateAliasArray", "Utility", papyrusUtility::CreateArray<BGSBaseAlias*>, registry));


	registry->RegisterFunction(
		new NativeFunction3 <StaticFunctionTag, VMResultArray<float>, VMArray<float>, UInt32, float>("ResizeFloatArray", "Utility", papyrusUtility::ResizeArray<float>, registry));

	registry->RegisterFunction(
		new NativeFunction3 <StaticFunctionTag, VMResultArray<SInt32>, VMArray<SInt32>, UInt32, SInt32>("ResizeIntArray", "Utility", papyrusUtility::ResizeArray<SInt32>, registry));

	registry->RegisterFunction(
		new NativeFunction3 <StaticFunctionTag, VMResultArray<bool>, VMArray<bool>, UInt32, bool>("ResizeBoolArray", "Utility", papyrusUtility::ResizeArray<bool>, registry));

	registry->RegisterFunction(
		new NativeFunction3 <StaticFunctionTag, VMResultArray<BSFixedString>, VMArray<BSFixedString>, UInt32, BSFixedString>("ResizeStringArray", "Utility", papyrusUtility::ResizeArray<BSFixedString>, registry));

	registry->RegisterFunction(
		new NativeFunction3 <StaticFunctionTag, VMResultArray<TESForm*>, VMArray<TESForm*>, UInt32, TESForm*>("ResizeFormArray", "Utility", papyrusUtility::ResizeArray<TESForm*>, registry));

	registry->RegisterFunction(
		new NativeFunction3 <StaticFunctionTag, VMResultArray<BGSBaseAlias*>, VMArray<BGSBaseAlias*>, UInt32, BGSBaseAlias*>("ResizeAliasArray", "Utility", papyrusUtility::ResizeArray<BGSBaseAlias*>, registry));


	registry->SetFunctionFlags("Utility", "GetINIFloat", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Utility", "GetINIInt", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Utility", "GetINIBool", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Utility", "GetINIString", VMClassRegistry::kFunctionFlag_NoWait);

	registry->SetFunctionFlags("Utility", "CreateFloatArray", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Utility", "CreateIntArray", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Utility", "CreateBoolArray", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Utility", "CreateStringArray", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Utility", "CreateFormArray", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Utility", "CreateAliasArray", VMClassRegistry::kFunctionFlag_NoWait);

	registry->SetFunctionFlags("Utility", "ResizeFloatArray", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Utility", "ResizeIntArray", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Utility", "ResizeBoolArray", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Utility", "ResizeStringArray", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Utility", "ResizeFormArray", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Utility", "ResizeAliasArray", VMClassRegistry::kFunctionFlag_NoWait);
}
