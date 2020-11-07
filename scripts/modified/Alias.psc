; return the name of the alias
string Function GetName() native

; return the id of the alias
int Function GetID() native

; Registers for OnKeyDown and OnKeyUp events for the given keycode.
Function RegisterForKey(int keyCode) native
Function UnregisterForKey(int keyCode) native
Function UnregisterForAllKeys() native

Event OnKeyDown(int keyCode)
EndEvent

Event OnKeyUp(int keyCode, float holdTime)
EndEvent

; Registers for OnControlDown and OnControlUp events for the given control.
; For a list of valid controls, see Input.psc.
Function RegisterForControl(string control) native
Function UnregisterForControl(string control) native
Function UnregisterForAllControls() native

Event OnControlDown(string control)
EndEvent

Event OnControlUp(string control, float holdTime)
EndEvent

; Registers for OnMenuOpen and OnMenuClose events for the given menu.
; Registrations have to be refreshed after each game load.
; For a list of valid menu names, see UI.psc.
Function RegisterForMenu(string menuName) native
Function UnregisterForMenu(string menuName) native
Function UnregisterForAllMenus() native

Event OnMenuOpen(string menuName)
endEvent

Event OnMenuClose(string menuName)
endEvent

; Registers a custom event callback for given event name.
; Registrations have to be refreshed after each game load.
;
;	Examples:
;		RegisterForModEvent("myCustomEvent", "MyModEventCallback")
;
;	Event signature of custom event callbacks:
;		Event MyModEventCallback(string eventName, string strArg, float numArg, Form sender)
;		endEvent
;
Function RegisterForModEvent(string eventName, string callbackName) native
Function UnregisterForModEvent(string eventName) native
Function UnregisterForAllModEvents() native

; Sends custom event with given generic parameters.
Function SendModEvent(string eventName, string strArg = "", float numArg = 0.0) native

; See Form.psc
Function RegisterForCameraState() native
Function UnregisterForCameraState() native

Event OnPlayerCameraState(int oldState, int newState)
EndEvent

; See Form.psc
Function RegisterForCrosshairRef() native
Function UnregisterForCrosshairRef() native
	
Event OnCrosshairRefChange(ObjectReference ref)
EndEvent

; See Form.psc
Function RegisterForActorAction(int actionType) native
Function UnregisterForActorAction(int actionType) native

Event OnActorAction(int actionType, Actor akActor, Form source, int slot)
EndEvent

; Registers the script for when a QueueNiNodeUpdate is called
Function RegisterForNiNodeUpdate() native
Function UnregisterForNiNodeUpdate() native

Event OnNiNodeUpdate(ObjectReference akActor)
EndEvent