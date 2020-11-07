#include "InputMap.h"
#include <xinput.h>

UInt32 InputMap::GamepadMaskToKeycode(UInt32 keyMask)
{
	switch (keyMask)
	{
		case XINPUT_GAMEPAD_DPAD_UP:		return kGamepadButtonOffset_DPAD_UP;
		case XINPUT_GAMEPAD_DPAD_DOWN:		return kGamepadButtonOffset_DPAD_DOWN;
		case XINPUT_GAMEPAD_DPAD_LEFT:		return kGamepadButtonOffset_DPAD_LEFT;
		case XINPUT_GAMEPAD_DPAD_RIGHT:		return kGamepadButtonOffset_DPAD_RIGHT;
		case XINPUT_GAMEPAD_START:			return kGamepadButtonOffset_START;
		case XINPUT_GAMEPAD_BACK:			return kGamepadButtonOffset_BACK;
		case XINPUT_GAMEPAD_LEFT_THUMB:		return kGamepadButtonOffset_LEFT_THUMB;
		case XINPUT_GAMEPAD_RIGHT_THUMB:	return kGamepadButtonOffset_RIGHT_THUMB;
		case XINPUT_GAMEPAD_LEFT_SHOULDER:	return kGamepadButtonOffset_LEFT_SHOULDER;
		case XINPUT_GAMEPAD_RIGHT_SHOULDER: return kGamepadButtonOffset_RIGHT_SHOULDER;
		case XINPUT_GAMEPAD_A:				return kGamepadButtonOffset_A;
		case XINPUT_GAMEPAD_B:				return kGamepadButtonOffset_B;
		case XINPUT_GAMEPAD_X:				return kGamepadButtonOffset_X;
		case XINPUT_GAMEPAD_Y:				return kGamepadButtonOffset_Y;
		case 0x9:							return kGamepadButtonOffset_LT;
		case 0xA:							return kGamepadButtonOffset_RT;
		default:							return kMaxMacros; // Invalid
	}
}

UInt32 InputMap::GamepadKeycodeToMask(UInt32 keyCode)
{
	switch (keyCode)
	{
		case kGamepadButtonOffset_DPAD_UP:			return XINPUT_GAMEPAD_DPAD_UP;
		case kGamepadButtonOffset_DPAD_DOWN:		return XINPUT_GAMEPAD_DPAD_DOWN;
		case kGamepadButtonOffset_DPAD_LEFT:		return XINPUT_GAMEPAD_DPAD_LEFT;
		case kGamepadButtonOffset_DPAD_RIGHT:		return XINPUT_GAMEPAD_DPAD_RIGHT;
		case kGamepadButtonOffset_START:			return XINPUT_GAMEPAD_START;
		case kGamepadButtonOffset_BACK:				return XINPUT_GAMEPAD_BACK;
		case kGamepadButtonOffset_LEFT_THUMB:		return XINPUT_GAMEPAD_LEFT_THUMB;
		case kGamepadButtonOffset_RIGHT_THUMB:		return XINPUT_GAMEPAD_RIGHT_THUMB;
		case kGamepadButtonOffset_LEFT_SHOULDER:	return XINPUT_GAMEPAD_LEFT_SHOULDER;
		case kGamepadButtonOffset_RIGHT_SHOULDER:	return XINPUT_GAMEPAD_RIGHT_SHOULDER;
		case kGamepadButtonOffset_A:				return XINPUT_GAMEPAD_A;
		case kGamepadButtonOffset_B:				return XINPUT_GAMEPAD_B;
		case kGamepadButtonOffset_X:				return XINPUT_GAMEPAD_X;
		case kGamepadButtonOffset_Y:				return XINPUT_GAMEPAD_Y;
		case kGamepadButtonOffset_LT:				return 0x9;
		case kGamepadButtonOffset_RT:				return 0xA;
		default:									return 0xFF; // Invalid
	}
}
