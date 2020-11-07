Scriptname UI Hidden

; For functions that require a menuName, potential values are
;	"InventoryMenu"
;	"Console"
;	"Dialogue Menu"
;	"HUD Menu"
;	"Main Menu"
;	"MessageBoxMenu"
;	"Cursor Menu"
;	"Fader Menu"
;	"MagicMenu"
;	"Top Menu"
;	"Overlay Menu"
;	"Overlay Interaction Menu"
;	"Loading Menu"
;	"TweenMenu"
;	"BarterMenu"
;	"GiftMenu"
;	"Debug Text Menu"
;	"MapMenu"
;	"Lockpicking Menu"
;	"Quantity Menu"
;	"StatsMenu"
;	"ContainerMenu"
;	"Sleep/Wait Menu"
;	"LevelUp Menu"
;	"Journal Menu"
;	"Book Menu"
;	"FavoritesMenu"
;	"RaceSex Menu"
;	"Crafting Menu"
;	"Training Menu"
;	"Mist Menu"
;	"Tutorial Menu"
;	"Credits Menu"
;	"TitleSequence Menu"
;	"Console Native UI Menu"
;	"Kinect Menu"
;
; The target parameter requires one the following prefixes:
;	_global		, for the default namespace;
;	_root		, for the movie root.


; Returns if the menu is currently open.
bool Function IsMenuOpen(string menuName) global native


; Sets bool/number/string value at target location.
; Target value must already exist.
;
;	Examples:
;		UI.SetBool("InventoryMenu", "_root.Menu_mc._visible", false)
;		UI.SetString("FavoritesMenu", "_root.Menu_mc.panel.message.text", "My Text")
;
Function SetBool(string menuName, string target, bool value) global native
Function SetInt(string menuName, string target, int value) global native
Function SetFloat(string menuName, string target, float value) global native
Function SetString(string menuName, string target, string value) global native
Function SetNumber(string menuName, string target, float value) global ; DEPRECIATED
	SetFloat(menuName, target, value)
EndFunction

; Gets bool/number/string from target location, or false/0/none if the value doesn't exist.
;
;	Examples:
;		bool	visible	= UI.GetBool("Inventory Menu", "_root.Menu_mc._visible")
;		float	height	= UI.GetNumber("Magic Menu", "_root.Menu_mc._height")
;
bool	Function GetBool(string menuName, string target) global native
int		Function GetInt(string menuName, string target) global native
float	Function GetFloat(string menuName, string target) global native
string	Function GetString(string menuName, string target) global native
float	Function GetNumber(string menuName, string target) global ; DEPRECIATED
	return GetFloat(menuName, target)
EndFunction


; Invokes the ActionScript function at given target location.
;
;	Examples:
;		UI.InvokeString("InventoryMenu", "_global.skse.Log", "Printed to logfile")
;		UI.InvokeStringA("InventoryMenu", "_global.myFunction", myArray)
;
Function Invoke(string menuName, string target) global
	InvokeBool(menuName, target, false)
EndFunction

Function InvokeBool(string menuName, string target, bool arg) global native
Function InvokeInt(string menuName, string target, int arg) global native
Function InvokeFloat(string menuName, string target, float arg) global native
Function InvokeString(string menuName, string target, string arg) global native
Function InvokeNumber(string menuName, string target, float arg) global ; DEPRECIATED
	InvokeFloat(menuName, target, arg)
EndFunction

Function InvokeBoolA(string menuName, string target, bool[] args) global native
Function InvokeIntA(string menuName, string target, int[] args) global native
Function InvokeFloatA(string menuName, string target, float[] args) global native
Function InvokeStringA(string menuName, string target, string[] args) global native
Function InvokeNumberA(string menuName, string target, float[] args) global ; DEPRECIATED
	InvokeFloatA(menuName, target, args)
EndFunction

; Sends Form data to Scaleform as a Flash object, FormLists included.
Function InvokeForm(string menuName, string target, Form arg) global native

; returns if scaleform is in 'text input' mode
; this is useful for ignoring keys that should get swallowed by an editable text box
bool Function IsTextInputEnabled() global native

; open a custom menu named "CustomMenu" by loading the given swf from the interface folder
; (filename without extension)
; there can only be a single custom menu open at the same time
Function OpenCustomMenu(string swfPath, int flags = 0) global native

; close the custom menu if it's currently open.
Function CloseCustomMenu() global native