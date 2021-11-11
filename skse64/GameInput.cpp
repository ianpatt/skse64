#include "GameInput.h"

bool InputEventDispatcher::IsGamepadEnabled(void)
{
	return (gamepad != NULL) && gamepad->IsEnabled();
}

InputEventDispatcher* InputEventDispatcher::GetSingleton()
{
	// 8FAAB2F3DC59885AE2A550C95F2C82BE2497789F+37
	static RelocPtr<InputEventDispatcher*> g_inputEventDispatcher(0x02FBFD88);
	return *g_inputEventDispatcher;
}

InputManager * InputManager::GetSingleton(void)
{
	// 80AE59E4FF0DC421866FA7261C1625595B0C7572+11
	static RelocPtr<InputManager*> g_inputManager(0x02F60320);
	return *g_inputManager;
}

UInt8 InputManager::AllowTextInput(bool allow)
{
	if(allow)
	{
		if(allowTextInput == 0xFF)
			_WARNING("InputManager::AllowTextInput: counter overflow");
		else
			allowTextInput++;
	}
	else
	{
		if(allowTextInput == 0)
			_WARNING("InputManager::AllowTextInput: counter underflow");
		else
			allowTextInput--;
	}

	if(IsConsoleMode())
		Console_Print("%s text input, count = %d", allow ? "allowed" : "disallowed", allowTextInput);

	return allowTextInput;
}

UInt32 InputManager::GetMappedKey(BSFixedString name, UInt32 deviceType, UInt32 contextIdx)
{
	ASSERT(contextIdx < kContextCount);

	tArray<InputContext::Mapping> * mappings;
	if (deviceType == kDeviceType_Mouse)
		mappings = &context[contextIdx]->mouseMap;
	else if (deviceType == kDeviceType_Gamepad)
		mappings = &context[contextIdx]->gamepadMap;
	else
		mappings = &context[contextIdx]->keyboardMap;

	for (UInt32 i=0; i < mappings->count; i++)
	{
		InputContext::Mapping m;
		if (!mappings->GetNthItem(i, m))
			break;
		if (m.name == name)
			return m.buttonID;
	}

	// Unbound
	return 0xFF;
}

BSFixedString InputManager::GetMappedControl(UInt32 buttonID, UInt32 deviceType, UInt32 contextIdx)
{
	ASSERT(contextIdx < kContextCount);

	// 0xFF == unbound
	if (buttonID == 0xFF)
		return BSFixedString();

	tArray<InputContext::Mapping> * mappings;
	if (deviceType == kDeviceType_Mouse)
		mappings = &context[contextIdx]->mouseMap;
	else if (deviceType == kDeviceType_Gamepad)
		mappings = &context[contextIdx]->gamepadMap;
	else
		mappings = &context[contextIdx]->keyboardMap;

	for (UInt32 i=0; i < mappings->count; i++)
	{
		InputContext::Mapping m;
		if (!mappings->GetNthItem(i, m))
			break;
		if (m.buttonID == buttonID)
			return m.name;
	}

	return BSFixedString();
}

PlayerControls * PlayerControls::GetSingleton(void)
{
	// 20AF40A6B803C21547F594CE478EAA5065E8F5A5+11B
	static RelocPtr<PlayerControls*> g_playerControls(0x02F60328);
	return *g_playerControls;
}

MenuControls * MenuControls::GetSingleton(void)
{
	// 4E9F39D1066653EF254B38406212E476F80A6C9B+F0
	static RelocPtr<MenuControls*> g_menuControls(0x02F9AB08);
	return *g_menuControls;
}
