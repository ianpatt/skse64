#include "GameInput.h"

bool InputEventDispatcher::IsGamepadEnabled(void)
{
	return (gamepad != NULL) && gamepad->IsEnabled();
}

InputEventDispatcher* InputEventDispatcher::GetSingleton()
{
	// 5202C5E930BB4CD8F477F91C9434AB37DBDB10B3+7A
	static RelocPtr<InputEventDispatcher*> g_inputEventDispatcher(0x02F257A8);
	return *g_inputEventDispatcher;
}

InputManager * InputManager::GetSingleton(void)
{
	// 61FAE6E8975F0FA7B3DD4D5A410A240E86A58F7B+E
	static RelocPtr<InputManager*> g_inputManager(0x02EC5BD0);
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
	// F1E82AFF2615653A5A14A2E7C229B4B0466688EF+19
	static RelocPtr<PlayerControls*> g_playerControls(0x02EC5BD8);
	return *g_playerControls;
}

MenuControls * MenuControls::GetSingleton(void)
{
	// DC378767BEB0312EBDE098BC7E0CE53FCC296377+D9
	static RelocPtr<MenuControls*> g_menuControls(0x02F003F8);
	return *g_menuControls;
}
