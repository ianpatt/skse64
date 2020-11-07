
; Returns the typecode for this form object
Int Function GetType() native

; returns the form's name, full name if possible
string Function GetName() native

; sets the name of the form
Function SetName(string name) native

; returns the weight of the form
float Function GetWeight() native

; sets the weight of the form
Function SetWeight(float weight) native

; sets the gold value of the form
Function SetGoldValue(int value) native

; returns the number of keywords on the form
int Function GetNumKeywords() native

; returns the keyword at the specified index
Keyword Function GetNthKeyword(int index) native

; returns all keywords of the form
Keyword[] Function GetKeywords() native

bool Function HasKeywordString(string s)
	Keyword k = Keyword.GetKeyword(s)
	if k == None
		return false
	endif
	return HasKeyword(k)
endFunction

; Sets whether the player knows this form
; Should only be used for Magic Effects,
; Words of Power, and Enchantments
Function SetPlayerKnows(bool knows) native

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

; Registers for OnPlayerCameraState events
Function RegisterForCameraState() native
Function UnregisterForCameraState() native

Event OnPlayerCameraState(int oldState, int newState)
EndEvent

; Registers for OnCrosshairRefChange events
Function RegisterForCrosshairRef() native
Function UnregisterForCrosshairRef() native

; Note: ref is none for no target
Event OnCrosshairRefChange(ObjectReference ref)
EndEvent

Function RegisterForActorAction(int actionType) native
Function UnregisterForActorAction(int actionType) native

; ActionTypes
; 0 - Weapon Swing (Melee weapons that are swung, also barehand)
; 1 - Spell Cast (Spells and staves)
; 2 - Spell Fire (Spells and staves)
; 3 - Voice Cast
; 4 - Voice Fire
; 5 - Bow Draw
; 6 - Bow Release
; 7 - Unsheathe Begin
; 8 - Unsheathe End
; 9 - Sheathe Begin
; 10 - Sheathe End
; Slots
; 0 - Left Hand
; 1 - Right Hand
; 2 - Voice
Event OnActorAction(int actionType, Actor akActor, Form source, int slot)
EndEvent

; Registers the script for when a QueueNiNodeUpdate is called
Function RegisterForNiNodeUpdate() native
Function UnregisterForNiNodeUpdate() native

Event OnNiNodeUpdate(ObjectReference akActor)
EndEvent

; Returns a temporary clone of this form
Form Function TempClone() native

; Returns whether this Form has a World Model (fast)
bool Function HasWorldModel() native

; Returns the world model path of this Form, if it has a world model
string Function GetWorldModelPath() native
Function SetWorldModelPath(string path) native

; Returns the number of texture sets the world model has, if its textures can be swapped
int Function GetWorldModelNumTextureSets() native

; Returns the Nth texture set of the world model, if the textures can be swapped
TextureSet Function GetWorldModelNthTextureSet(int n) native

; Sets the world models Nth texture set, if the textures can be set
Function SetWorldModelNthTextureSet(TextureSet nSet, int n) native

; Returns whether this Form is playable, only applied to Forms with the playable flag
bool Function IsPlayable() native