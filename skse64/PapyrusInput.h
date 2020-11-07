#pragma once

#include "GameTypes.h"

struct StaticFunctionTag;

class VMClassRegistry;

namespace papyrusInput
{
	void RegisterFuncs(VMClassRegistry* registry);

	bool IsKeyPressed(StaticFunctionTag* thisInput, UInt32 dxKeycode);

	void TapKey(StaticFunctionTag* thisInput, UInt32 dxKeycode);

	void HoldKey(StaticFunctionTag* thisInput, UInt32 dxKeycode);
	void ReleaseKey(StaticFunctionTag* thisInput, UInt32 dxKeycode);
	bool IsKeyHeld(StaticFunctionTag* thisInput, UInt32 dxKeycode);

	void DisableKey(StaticFunctionTag* thisInput, UInt32 dxKeyode);
	void EnableKey(StaticFunctionTag* thisInput, UInt32 dxKeycode);
	bool IsKeyDisabled(StaticFunctionTag* thisInput, UInt32 dxKeycode);

	UInt32 GetNumKeysPressed(StaticFunctionTag* thisInput);
	SInt32 GetNthKeyPressed(StaticFunctionTag* thisInput, UInt32 which);

	//UInt32 GetNumMouseButtonsPressed(Input* thisInput);
	//UInt32 GetMouseButtonPressed(Input* thisInput, UInt32 which);

	//void DisableMouse(Input* thisInput);
	//void EnableMouse(Input* thisInput);
	//bool IsMouseDisabled(Input* thisInput);

	SInt32			GetMappedKey(StaticFunctionTag* thisInput, BSFixedString name, UInt32 deviceType);
	BSFixedString	GetMappedControl(StaticFunctionTag* thisInput, SInt32 keyCode);
}
