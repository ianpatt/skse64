#include "Hooks_Event.h"

#include "skse64_common/Utilities.h"
#include "skse64/GameInput.h"
#include "skse64/GameMenus.h"
#include "skse64/PapyrusEvents.h"
#include "skse64/PluginManager.h"
#include "skse64_common/BranchTrampoline.h"

RelocAddr <uintptr_t> PlayerControls_ctor_Call(0x005D2940 + 0xD75);

PlayerControls * PlayerControls::ctor_Hook(void)
{
	PlayerControls	* result = CALL_MEMBER_FN(this, ctor)();

	MenuManager * mm = MenuManager::GetSingleton();
	if (mm) {
		mm->MenuOpenCloseEventDispatcher()->AddEventSink(&g_menuEventHandler);
	} else {
		_MESSAGE("Failed to register SKSE menuEventHandler!");
	}

	g_modCallbackEventDispatcher.AddEventSink(&g_modCallbackEventHandler);
	g_cameraEventDispatcher.AddEventSink(&g_cameraEventHandler);
	g_crosshairRefEventDispatcher.AddEventSink(&g_crosshairRefEventHandler);
	g_actionEventDispatcher.AddEventSink(&g_actionEventHandler);
	g_ninodeUpdateEventDispatcher.AddEventSink(&g_ninodeUpdateEventHandler);

	InputEventDispatcher *inputEventDispatcher = InputEventDispatcher::GetSingleton();
	if (inputEventDispatcher) {
		inputEventDispatcher->AddEventSink(&g_inputEventHandler);
	} else {
		_MESSAGE("Failed to register SKSE inputEventHandler");
	}

	PluginManager::Dispatch_Message(0, SKSEMessagingInterface::kMessage_InputLoaded, NULL, 0, NULL);

	return result;
}

void Hooks_Event_Commit(void)
{
	g_branchTrampoline.Write5Call(PlayerControls_ctor_Call, GetFnAddr(&PlayerControls::ctor_Hook));
}
