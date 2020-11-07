Scriptname EffectShader extends Form Hidden

; Starts playing this effect shader on the specified object for the specified length of time in seconds. Negative values indiciate "infinite"
Function Play(ObjectReference akObject, float afDuration = -1.0) native

; Stops playing this effect shader on the specified object
Function Stop(ObjectReference akObject) native