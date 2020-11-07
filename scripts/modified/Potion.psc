; Is this potion classified as Food?
bool Function IsFood() native

; Is this potion classified as Poison?
bool Function IsPoison() native

; return the number of the effects
int Function GetNumEffects() native

; return the magnitude of the specified effect
float Function GetNthEffectMagnitude(int index) native

; return the area of the specified effect
int Function GetNthEffectArea(int index) native

; return the duration of the specified effect
int Function GetNthEffectDuration(int index) native

; return the magic effect of the specified effect
MagicEffect Function GetNthEffectMagicEffect(int index) native

; return the index of the costliest effect
int Function GetCostliestEffectIndex() native

; sets the magnitude of the specified effect
Function SetNthEffectMagnitude(int index, float value) native

; sets the area of the specified effect
Function SetNthEffectArea(int index, int value) native

; sets the duration of the specified effect
Function SetNthEffectDuration(int index, int value) native

; gets the use sound of this potion
SoundDescriptor Function GetUseSound() native

; Returns all the magnitudes of this object in order
float[] Function GetEffectMagnitudes() native

; Returns all the areas of this object in order
int[] Function GetEffectAreas() native

; Returns all the durations of this object in order
int[] Function GetEffectDurations() native

; Returns all the magic effects of this object in order
MagicEffect[] Function GetMagicEffects() native