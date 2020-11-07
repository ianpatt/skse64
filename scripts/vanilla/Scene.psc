Scriptname Scene extends Form Hidden

; Forces a scene to start and kills the current scenes running on any ref in that scene
Function ForceStart() native

; Starts this scene
Function Start() native

; Stops the scene
Function Stop() native

; Is this scene currently playing?
bool Function IsPlaying() native

; Returns the quest that owns this scene
Quest Function GetOwningQuest() native

; Returns whether the specified action is complete or not
bool Function IsActionComplete(int aiActionID) native