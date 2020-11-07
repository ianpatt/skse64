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

; returns the base enchantment of this enchantment
Enchantment Function GetBaseEnchantment() native

; Returns a Formlist of Keywords 
FormList Function GetKeywordRestrictions() native

; Sets the FormList of keywords
Function SetKeywordRestrictions(FormList newKeywordList) native