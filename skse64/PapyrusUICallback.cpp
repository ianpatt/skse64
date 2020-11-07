#include "PapyrusUICallback.h"

#include "common/ICriticalSection.h"

#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

#include "PapyrusArgs.h"
#include "PapyrusUI.h"
#include "PapyrusObjects.h"

///
/// Native functions
///

namespace papyrusUICallback
{
	SInt32 Create(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag* thisInput, BSFixedString menuName, BSFixedString targetStr)
	{
		ERROR_AND_RETURN_0_IF(!menuName.data, "Menu name must not be empty.", registry, stackId)
		ERROR_AND_RETURN_0_IF(!targetStr.data, "Target name must not be empty.", registry, stackId)
		
		UIInvokeDelegate* cmd = new UIInvokeDelegate(menuName.data, targetStr.data);

		UInt32 handle = SKSEObjectStorageInstance().StoreObject(cmd, stackId);
		return handle;
	}

	bool Send(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag* thisInput, SInt32 handle)
	{
		ERROR_AND_RETURN_0_IF(handle <= 0, "Invalid handle.", registry, stackId)

		UIManager* uiManager = UIManager::GetSingleton();
		if (!uiManager)
			return false;

		UIInvokeDelegate* cmd = SKSEObjectStorageInstance().TakeObject<UIInvokeDelegate>(handle);
		
		ERROR_AND_RETURN_0_IF(cmd == NULL, "Failed to lookup object for given handle.", registry, stackId)

		// Transfers ownership to UIManager
		uiManager->QueueCommand(cmd);

		return true;
	}

	void Release(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag* thisInput, SInt32 handle)
	{
		ERROR_AND_RETURN_IF(handle <= 0, "Invalid handle.", registry, stackId)

		UIInvokeDelegate* cmd = SKSEObjectStorageInstance().TakeObject<UIInvokeDelegate>(handle);
		
		ERROR_AND_RETURN_IF(cmd == NULL, "Failed to lookup object for given handle.", registry, stackId)
		
		delete cmd;
	}

	template <typename T>
	void Push(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag* thisInput, SInt32 handle, T arg)
	{
		ERROR_AND_RETURN_IF(handle <= 0, "Invalid handle.", registry, stackId)

		UIInvokeDelegate* cmd = SKSEObjectStorageInstance().AccessObject<UIInvokeDelegate>(handle);
		
		ERROR_AND_RETURN_IF(cmd == NULL, "Failed to lookup object for given handle.", registry, stackId)

		GFxValue value;
		SetGFxValue<T>(&value, arg);
		cmd->args.push_back(value);
	}

	template <typename T>
	void PushArray(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag* thisInput, SInt32 handle, VMArray<T> args)
	{
		ERROR_AND_RETURN_IF(handle <= 0, "Invalid handle.", registry, stackId)

		UIInvokeDelegate* cmd = SKSEObjectStorageInstance().AccessObject<UIInvokeDelegate>(handle);
		
		ERROR_AND_RETURN_IF(cmd == NULL, "Failed to lookup object for given handle.", registry, stackId)

		UInt32 argCount = args.Length();

		UInt32 offset = cmd->args.size();
		UInt32 newArgCount = offset + argCount;
		
		cmd->args.resize(newArgCount);
		for (UInt32 i=0; i<argCount; i++, offset++)
		{
			T arg;
			args.Get(&arg, i);
			SetGFxValue<T>(&cmd->args[offset], arg);
		}
	}
}

void papyrusUICallback::RegisterFuncs(VMClassRegistry* registry)
{
	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, SInt32, BSFixedString, BSFixedString> ("Create", "UICallback", papyrusUICallback::Create, registry));
	
	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, bool, SInt32> ("Send", "UICallback", papyrusUICallback::Send, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, void, SInt32> ("Release", "UICallback", papyrusUICallback::Release, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, void, SInt32, bool> ("PushBool", "UICallback", papyrusUICallback::Push<bool>, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, void, SInt32, SInt32> ("PushInt", "UICallback", papyrusUICallback::Push<SInt32>, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, void, SInt32, float> ("PushFloat", "UICallback", papyrusUICallback::Push<float>, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, void, SInt32, BSFixedString> ("PushString", "UICallback", papyrusUICallback::Push<BSFixedString>, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, void, SInt32, VMArray<bool>> ("PushBoolA", "UICallback", papyrusUICallback::PushArray<bool>, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, void, SInt32, VMArray<SInt32>> ("PushIntA", "UICallback", papyrusUICallback::PushArray<SInt32>, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, void, SInt32, VMArray<float>> ("PushFloatA", "UICallback", papyrusUICallback::PushArray<float>, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, void, SInt32, VMArray<BSFixedString>> ("PushStringA", "UICallback", papyrusUICallback::PushArray<BSFixedString>, registry));

	registry->SetFunctionFlags("UICallback", "Create", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("UICallback", "Send", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("UICallback", "Release", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("UICallback", "PushBool", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("UICallback", "PushInt", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("UICallback", "PushFloat", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("UICallback", "PushString", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("UICallback", "PushBoolA", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("UICallback", "PushIntA", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("UICallback", "PushFloatA", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("UICallback", "PushStringA", VMClassRegistry::kFunctionFlag_NoWait);
}