Scriptname ModEvent Hidden

; ModEvent allows sending mod events with any number/type of arguments, unlike the more limited Form.SendModEvent.
;
; Example:
;
;	(Sender)
;
;	 	int handle = ModEvent.Create("MYPREFIX_myCustomEvent")
;		if (handle)
;			ModEvent.PushForm(handle, self)
;			ModEvent.PushForm(handle, someOtherForm)
;			ModEvent.PushInt(handle, 1000)
;			ModEvent.PushString(handle, "It worked!")
;			UIDelegate.Send(handle)
;		endIf
;
;	(Receiver)
;
;		function OnInit()
;			RegisterForModEvent("MYPREFIX_myCustomEvent", "OnMyCustomEvent")
;		endFunction
;	
;		event OnMyCustomEvent(Form sender, Form theForm, int theInt, string theString)
;			; sender == (Sender)
;			; theForm == someOtherForm
;			; theInt == 1000
;			; theString == "It worked!"
;		endEvent

; Creates a new ModEvent and returns the handle.
int Function Create(string eventName) global native

; Sends the ModEvent and releases it.
; Returns true, if it was sent successfully, false if an error happened.
bool Function Send(int handle) global native
	
; Releases the ModEvent without sending it.
Function Release(int handle) global native

; Push single parameter.
;
; For arguments 1 .. N, the signature of the receiving event callback has to look like this:
;
;	event MyCallback(TYPE_1 PARAM_1, ... , TYPE_N PARAM_N)
;
Function PushBool(int handle, bool value) global native
Function PushInt(int handle, int value) global native
Function PushFloat(int handle, float value) global native
Function PushString(int handle, string value) global native
Function PushForm(int handle, Form value) global native
