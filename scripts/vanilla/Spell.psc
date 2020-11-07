Scriptname Spell extends Form Hidden

; Cast this spell from an ObjectReference, optionally toward another.
Function Cast(ObjectReference akSource, ObjectReference akTarget=NONE) native

; Cast this spell from an ObjectReference, optionally toward another, and blame it on a particular actor.
Function RemoteCast(ObjectReference akSource, Actor akBlameActor, ObjectReference akTarget=NONE) native

; Is this spell classified as hostile?
bool Function IsHostile() native

; Preload the art for this spell. Useful for spells you equip & unequip on the player.
; Warning: Misuse of this function can lead to erroneous behavior as well as excessive
; memory consumption. It's best to avoid using this. This function will likely be
; deprecated in the future.
Function Preload() native

; Unload the art for this spell. Call this only if you've previously called Preload.
; Warning: Misuse of this function can lead to erroneous behavior including spell art
; being unloaded while in use, and excessive memory consumption. It's best to avoid using this.
; This function will likely be deprecated in the future.
Function Unload() native
