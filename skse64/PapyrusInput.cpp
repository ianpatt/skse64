#include "skse64/PapyrusInput.h"
#include "skse64/GameInput.h"
#include "Hooks_DirectInput8Create.h"

namespace papyrusInput
{
	bool IsKeyPressed(StaticFunctionTag* thisInput, UInt32 dxKeycode)
	{
		return DIHookControl::GetSingleton().IsKeyPressed(dxKeycode);
	}

	void TapKey(StaticFunctionTag* thisInput, UInt32 dxKeycode)
	{
		//DIHookControl::GetSingleton().TapKey(dxKeycode);
		DIHookControl::GetSingleton().BufferedKeyTap(dxKeycode);
	}

	void HoldKey(StaticFunctionTag* thisInput, UInt32 dxKeycode)
	{
		//DIHookControl::GetSingleton().SetKeyHeldState(dxKeycode, true);
		DIHookControl::GetSingleton().BufferedKeyPress(dxKeycode);
	}

	void ReleaseKey(StaticFunctionTag* thisInput, UInt32 dxKeycode)
	{
		//DIHookControl::GetSingleton().SetKeyHeldState(dxKeycode, false);
		DIHookControl::GetSingleton().BufferedKeyRelease(dxKeycode);
	}

	bool IsKeyHeld(StaticFunctionTag* thisInput, UInt32 dxKeycode)
	{
		return DIHookControl::GetSingleton().IsKeyHeld(dxKeycode);
	}

	void DisableKey(StaticFunctionTag* thisInput, UInt32 dxKeycode)
	{
		DIHookControl::GetSingleton().SetKeyDisableState(dxKeycode, true, 0);
	}

	void EnableKey(StaticFunctionTag* thisInput, UInt32 dxKeycode)
	{
		DIHookControl::GetSingleton().SetKeyDisableState(dxKeycode, false, 0);
	}
	
	bool IsKeyDisabled(StaticFunctionTag* thisInput, UInt32 dxKeycode)
	{
		return DIHookControl::GetSingleton().IsKeyDisabled(dxKeycode);
	}

	UInt32 GetNumKeysPressed(StaticFunctionTag* thisInput)
	{
		return DIHookControl::GetSingleton().GetNumKeysPressed();
	}

	SInt32 GetNthKeyPressed(StaticFunctionTag* thisInput, UInt32 which)
	{
		return DIHookControl::GetSingleton().GetNthKeyPressed(which);
	}

	//UInt32 GetNumMouseButtonsPressed(StaticFunctionTag* thisInput);
	//UInt32 GetMouseButtonPressed(StaticFunctionTag* thisInput, UInt32 which);

	//void DisableMouse(StaticFunctionTag* thisInput);
	//void EnableMouse(StaticFunctionTag* thisInput);
	//bool IsMouseDisabled(StaticFunctionTag* thisInput);

	SInt32 GetMappedKey(StaticFunctionTag* thisInput, BSFixedString name, UInt32 deviceType)
	{
		InputManager * inputManager = InputManager::GetSingleton();
		if (!inputManager)
			return -1;

		UInt32 key = 0xFF;
		
		// Manual device selection
		if (deviceType != 0xFF)
		{
			key = inputManager->GetMappedKey(name, deviceType, InputManager::kContext_Gameplay);
		}
		// Auto-selected device
		else
		{
			// Gamepad
			InputEventDispatcher *inputEventDispatcher = InputEventDispatcher::GetSingleton();
			if (inputEventDispatcher && inputEventDispatcher->IsGamepadEnabled())
			{
				deviceType = kDeviceType_Gamepad;
				key = inputManager->GetMappedKey(name, kDeviceType_Gamepad, InputManager::kContext_Gameplay);
			}
			// Mouse + Keyboard
			else
			{
				deviceType = kDeviceType_Keyboard;
				key = inputManager->GetMappedKey(name, deviceType, InputManager::kContext_Gameplay);
				if (key == 0xFF)
				{
					deviceType = kDeviceType_Mouse;
					key = inputManager->GetMappedKey(name, deviceType, InputManager::kContext_Gameplay);
				}
			}
		}

		if (key == 0xFF)
			return -1;

		// Map to common value space
		if (deviceType == kDeviceType_Mouse)
		{
			return key + InputMap::kMacro_MouseButtonOffset;
		}
		else if (deviceType == kDeviceType_Gamepad)
		{
			UInt32 mapped = InputMap::GamepadMaskToKeycode(key);
			return (mapped != InputMap::kMaxMacros ? mapped : -1);
		}
		else
		{
			return key;
		}
	}

	BSFixedString GetMappedControl(StaticFunctionTag* thisInput, SInt32 keyCode)
	{
		if (keyCode < 0 || keyCode >= InputMap::kMaxMacros)
			return BSFixedString();

		InputManager * inputManager = InputManager::GetSingleton();
		if (!inputManager)
			return BSFixedString();

		UInt32 buttonID;
		UInt32 deviceType;

		if (keyCode >= InputMap::kMacro_GamepadOffset)
		{
			buttonID = InputMap::GamepadKeycodeToMask(keyCode);
			deviceType = kDeviceType_Gamepad;
		}
		else if (keyCode >= InputMap::kMacro_MouseButtonOffset)
		{
			buttonID = keyCode - InputMap::kMacro_MouseButtonOffset;
			deviceType = kDeviceType_Mouse;
		}
		else
		{
			buttonID = keyCode;
			deviceType = kDeviceType_Keyboard;
		}

		return inputManager->GetMappedControl(buttonID, deviceType, InputManager::kContext_Gameplay);
	}
}

#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

void papyrusInput::RegisterFuncs(VMClassRegistry* registry)
{
	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, bool, UInt32> ("IsKeyPressed", "Input", papyrusInput::IsKeyPressed, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, void, UInt32> ("TapKey", "Input", papyrusInput::TapKey, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, void, UInt32> ("HoldKey", "Input", papyrusInput::HoldKey, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, void, UInt32> ("ReleaseKey", "Input", papyrusInput::ReleaseKey, registry));

	registry->RegisterFunction(
		new NativeFunction0 <StaticFunctionTag, UInt32> ("GetNumKeysPressed", "Input", papyrusInput::GetNumKeysPressed, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, SInt32, UInt32> ("GetNthKeyPressed", "Input", papyrusInput::GetNthKeyPressed, registry));


	//registry->RegisterFunction(
	//	new NativeFunction1 <StaticFunctionTag, bool, UInt32> ("IsKeyHeld", "Input", papyrusInput::IsKeyHeld, registry));

	//registry->RegisterFunction(
	//	new NativeFunction1 <StaticFunctionTag, bool, UInt32> ("IsKeyDisabeld", "Input", papyrusInput::IsKeyDisabled, registry));

	//registry->RegisterFunction(
	//	new NativeFunction1 <StaticFunctionTag, void, UInt32> ("DisableKey", "Input", papyrusInput::DisableKey, registry));

	//registry->RegisterFunction(
	//	new NativeFunction1 <StaticFunctionTag, void, UInt32> ("EnableKey", "Input", papyrusInput::EnableKey, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, SInt32, BSFixedString, UInt32> ("GetMappedKey", "Input", papyrusInput::GetMappedKey, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, BSFixedString, SInt32> ("GetMappedControl", "Input", papyrusInput::GetMappedControl, registry));

	registry->SetFunctionFlags("Input", "IsKeyPressed", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Input", "TapKey", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Input", "HoldKey", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Input", "ReleaseKey", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Input", "GetNumKeysPressed", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Input", "GetNthKeyPressed", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Input", "GetMappedKey", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Input", "GetMappedControl", VMClassRegistry::kFunctionFlag_NoWait);
}
