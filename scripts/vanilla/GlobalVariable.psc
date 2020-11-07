Scriptname GlobalVariable extends Form Hidden

; Obtains the global's current value
float Function GetValue() native

; Sets the global's current value
Function SetValue(float afNewValue) native

; convenience functions for ints
int Function GetValueInt()
	return GetValue() as int
endFunction

; Sets the global's current value
Function SetValueInt(int aiNewValue)
	SetValue(aiNewValue as float)
endFunction

; Easy access to the global's value
float Property Value Hidden
  float Function get()
    return GetValue()
  EndFunction
  
  Function set(float afValue)
    SetValue(afValue)
  EndFunction
EndProperty

; the threadsafe way to modify a global variable
float Function Mod(float afHowMuch)
                Value += afHowMuch
                Return Value
EndFunction
