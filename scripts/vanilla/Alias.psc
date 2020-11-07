Scriptname Alias Hidden

; Returns the quest that owns this alias
Quest Function GetOwningQuest() native

; Register for the specified animation event from the specified object - returns true if it successfully registered
bool Function RegisterForAnimationEvent(ObjectReference akSender, string asEventName) native

; Register for LOS gain and lost events between the viewer and the target
; A loss or gain event will be sent immediately, depending on whether or not the viewer is already looking at the target or not
; If the viewer is not the player, the target must be another actor
Function RegisterForLOS(Actor akViewer, ObjectReference akTarget) native

; Register for only the first LOS gain event between the viewer and the target
; If the viewer is already looking at the target, an event will be received almost immediately
; If the viewer is not the player, the target must be another actor
Function RegisterForSingleLOSGain(Actor akViewer, ObjectReference akTarget) native

; Register for only the first LOS lost event between the viewer and the target
; If the viewer is already not looking at the target, an event will be received almost immediately
; If the viewer is not the player, the target must be another actor
Function RegisterForSingleLOSLost(Actor akViewer, ObjectReference akTarget) native

; Register for a single OnUpdate event, in afInterval seconds. All scripts attached to this alias will get the update events
; Of course, this means you don't need to call UnregisterForUpdate()
; If you find yourself doing this:
; Event OnUpdate()
;     UnregisterForUpdate()
;     {Do some stuff}
; endEvent
; Then you should use RegisterForSingleUpdate instead
Function RegisterForSingleUpdate(float afInterval) native

; Register for OnUpdate events, every X seconds, where X is the interval. All scripts attached to this alias will get the update events
Function RegisterForUpdate(float afInterval) native

; Register for OnUpdateGameTime events, every X hours of game time, where X is the interval. All scripts attached to this alias will get the update events
Function RegisterForUpdateGameTime(float afInterval) native

; Register for a single OnUpdateGameTime event, in afInterval hours of game time. All scripts attached to this alias will get the update events
Function RegisterForSingleUpdateGameTime(float afInterval) native

; Registers this alias to receive events when the player sleeps and wakes up
Function RegisterForSleep() native

; Registers this alias to receive events when tracked stats are updated
Function RegisterForTrackedStatsEvent() native

; Turns on profiling for this specific object and all scripts attached to it - setting doesn't persist across saves
; Will do nothing on release console builds, and if the Papyrus:bEnableProfiling ini setting is off
Function StartObjectProfiling() native

; Turns off profiling for this specific object and all scripts attached to it - setting doesn't persist across saves
; Will do nothing on release console builds, and if the Papyrus:bEnableProfiling ini setting is off
Function StopObjectProfiling() native

; Unregister for any LOS events between the viewer and target
Function UnregisterForLOS(Actor akViewer, ObjectReference akTarget) native

; Unregister for the specified animation event from the specified object
Function UnregisterForAnimationEvent(ObjectReference akSender, string asEventName) native

; Unregisters this alias to receive events when the player sleeps and wakes up
Function UnregisterForSleep() native

; Unregisters this alias from receiving events when tracked stats are updated
Function UnregisterForTrackedStatsEvent() native

; Unregister for OnUpdate events, all attached scripts will stop getting update events
Function UnregisterForUpdate() native

; Unregister for OnUpdateGameTime events, all attached scripts will stop getting update game time events
Function UnregisterForUpdateGameTime() native

; Animation event, sent when an object we are listening to hits one of the events we are listening for
Event OnAnimationEvent(ObjectReference akSource, string asEventName)
EndEvent

; Event sent when you have been unregistered from receiving an animation event because the target
; object's animation graph has been unloaded
Event OnAnimationEventUnregistered(ObjectReference akSource, string asEventName)
EndEvent

; LOS event, sent whenever the viewer first sees the target (after registering)
Event OnGainLOS(Actor akViewer, ObjectReference akTarget)
EndEvent

; Lost LOS event, sent whenever the viewer first loses sight of the target (after registering)
Event OnLostLOS(Actor akViewer, ObjectReference akTarget)
EndEvent

; Received when the player sleeps. Start and desired end time are in game time days (after registering)
Event OnSleepStart(float afSleepStartTime, float afDesiredSleepEndTime)
EndEvent

; Received when the player stops sleeping - whether naturally or interrupted (after registering)
Event OnSleepStop(bool abInterrupted)
EndEvent

; Event received when a tracked stat is updated for the player
Event OnTrackedStatsEvent(string arStatName, int aiStatValue)
EndEvent

; Update event, sent every X seconds while this alias is registered for them
Event OnUpdate()
EndEvent

; Update event, sent every X hours of game time while this alias is registered for them
Event OnUpdateGameTime()
EndEvent