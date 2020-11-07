; return the casting time
float Function GetCastTime() native

; return the perk associated with the spell
Perk Function GetPerk() native

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

; Returns the particular equipslot type
EquipSlot Function GetEquipType() native
Function SetEquipType(EquipSlot type) native

; Returns all the magnitudes of this object in order
float[] Function GetEffectMagnitudes() native

; Returns all the areas of this object in order
int[] Function GetEffectAreas() native

; Returns all the durations of this object in order
int[] Function GetEffectDurations() native

; Returns all the magic effects of this object in order
MagicEffect[] Function GetMagicEffects() native