Scriptname UICallback Hidden

; UICallback allows passing arguments of different types to UI functions, unlike UI.Invoke*
;
; Example:
; 	int handle = UICallback.Create("InventoryMenu", "_global.MyClass.initData")
;	if (handle)
;		UICallback.PushBool(handle, true)
;		UICallback.PushInt(handle, 1000)
;		UICallback.PushString(handle, "Hello")
;		UICallback.PushFloat(handle, 1.234)
;		UIDelegate.Send(handle)
;	endIf
;
; Any UICallback allocated by Create must be released later.
; That happens automatically when passing it to send.
; Otherwise the handle must be manually released by passing it to Release.
;
; Internally, UICallback objects only persist for the duration of the current
; game session. They are also cleared after each reload.
;
; This means that in very rare cases, the execution sequence of several operations
; on one UICallback might get interrupted, the handle turns invalid and the final Send
; will fail. If its necessary to detect this, check the return value of Send.

; Creates a new UICallback and returns the handle.
int Function Create(string menuName, string target) global native

; Invokes the UICallback and releases it.
; Returns true, if it was executed, false if an error happened.
bool Function Send(int handle) global native

; Releases the UICallback without sending it.
Function Release(int handle) global native

; Push single parameter. Maximum number of parameters per callback is 128.
Function PushBool(int handle, bool value) global native
Function PushInt(int handle, int value) global native
Function PushFloat(int handle, float value) global native
Function PushString(int handle, string value) global native

; Push parameters from array. Maximum number of parameters per callback is 128.
Function PushBoolA(int handle, bool[] args) global native
Function PushIntA(int handle, int[] args) global native
Function PushFloatA(int handle, float[] args) global native
Function PushStringA(int handle, string[] args) global native