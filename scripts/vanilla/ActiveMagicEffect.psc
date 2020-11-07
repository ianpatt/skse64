Scriptname ActiveMagicEffect Hidden

; Add an inventory event filter to this effect. Item added/removed events matching the
; specified form (or in the specified form list) will now be let through.
Function AddInventoryEventFilter(Form akFilter) native

; Dispel this effect
Function Dispel() native

; Get the base MagicEffect this active effect is using
MagicEffect Function GetBaseObject() native

; Get the actor that cast this spell
Actor Function GetCasterActor() native

; Get the actor this spell is targeting (is attached to)
Actor Function GetTargetActor() native

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

; Register for a single OnUpdate event, in afInterval seconds. All scripts attached to this magic effect will get the update events
; Of course, this means you don't need to call UnregisterForUpdate()
; If you find yourself doing this:
; Event OnUpdate()
;     UnregisterForUpdate()
;     {Do some stuff}
; endEvent
; Then you should use RegisterForSingleUpdate instead
Function RegisterForSingleUpdate(float afInterval) native

; Registers this magic effect to receive events when the player sleeps and wakes up
Function RegisterForSleep() native

; Registers this alias to receive events when tracked stats are updated
Function RegisterForTrackedStatsEvent() native

; Register for OnUpdate events, every X seconds, where X is the interval. All scripts attached to this magic effect will get the update events
Function RegisterForUpdate(float afInterval) native

; Register for OnUpdateGameTime events, every X hours of game time, where X is the interval. All scripts attached to this magic effect will get the update events
Function RegisterForUpdateGameTime(float afInterval) native

; Register for a single OnUpdateGameTime event, in afInterval hours of game time. All scripts attached to this magic effect will get the update events
Function RegisterForSingleUpdateGameTime(float afInterval) native

; Remove all inventory event filters from this effect - all item added/removed events will now be received
Function RemoveAllInventoryEventFilters() native

; Remove an inventory event filter from this effect. Item added/removed events matching the
; specified form (or in the specified form list) will no longer be let through.
Function RemoveInventoryEventFilter(Form akFilter) native

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

; Unregisters this magic effect to receive events when the player sleeps and wakes up
Function UnregisterForSleep() native

; Unregisters this magic effect from receiving events when tracked stats are updated
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

; Event received when this effect is first started (OnInit may not have been run yet!)
Event OnEffectStart(Actor akTarget, Actor akCaster)
EndEvent

; Event received when this effect is finished (effect may already be deleted, calling
; functions on this effect will fail)
Event OnEffectFinish(Actor akTarget, Actor akCaster)
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

; Update event, sent every X seconds while this magic effect is registered for them
Event OnUpdate()
EndEvent

; Update event, sent every X hours of game time while this magic effect is registered for them
Event OnUpdateGameTime()
EndEvent

; The following events are received from the actor this effect is attached to:

; Event received when this reference is activated
Event OnActivate(ObjectReference akActionRef)
EndEvent

; Event received when this object has moved to an attached cell from a detached one
Event OnAttachedToCell()
EndEvent

; Event received when this object's parent cell is attached
Event OnCellAttach()
EndEvent

; Event received when this object's parent cell is detached
Event OnCellDetach()
EndEvent

; Event received when every object in this object's parent cell is loaded (TODO: Find restrictions)
Event OnCellLoad()
EndEvent

; Event received when this object is closed
Event OnClose(ObjectReference akActionRef)
EndEvent

; Event received when this object enters, exits, or changes containers
Event OnContainerChanged(ObjectReference akNewContainer, ObjectReference akOldContainer)
EndEvent

; Event received when this reference's destruction stage has changed
Event OnDestructionStageChanged(int aiOldStage, int aiCurrentStage)
EndEvent

; Event recieved when this object moves to a detached cell from an attached one
Event OnDetachedFromCell()
EndEvent

; Event received when this object is equipped by an actor
Event OnEquipped(Actor akActor)
EndEvent

; Event received when this object is grabbed by the player
Event OnGrab()
EndEvent

; Event received when this object is hit by a source (weapon, spell, explosion) or projectile attack
Event OnHit(ObjectReference akAggressor, Form akSource, Projectile akProjectile, bool abPowerAttack, bool abSneakAttack, bool abBashAttack, bool abHitBlocked)
EndEvent

; Event received when an item is added to this object's inventory. If the item is a persistant reference, akItemReference will
; point at it - otherwise the parameter will be None
Event OnItemAdded(Form akBaseItem, int aiItemCount, ObjectReference akItemReference, ObjectReference akSourceContainer)
EndEvent

; Event received when an item is removed from this object's inventory. If the item is a persistant reference, akItemReference
; will point at it - otherwise the parameter will be None
Event OnItemRemoved(Form akBaseItem, int aiItemCount, ObjectReference akItemReference, ObjectReference akDestContainer)
EndEvent

; Event recieved when this object is completely loaded - will be fired every time this object is loaded
Event OnLoad()
EndEvent

; Event received when the lock on this object changes
Event OnLockStateChanged()
EndEvent

; Event received when a magic affect is being applied to this object
Event OnMagicEffectApply(ObjectReference akCaster, MagicEffect akEffect)
EndEvent

; Event received when this object is opened
Event OnOpen(ObjectReference akActionRef)
EndEvent

; Event received when this actor finishes changing its race
Event OnRaceSwitchComplete()
EndEvent

; Event received when this object, if a book, is read
Event OnRead()
EndEvent

; Event received when this object is released by the player
Event OnRelease()
EndEvent

; Event received when this reference is reset
Event OnReset()
EndEvent

; Event received when this reference is sold by an actor
Event OnSell(Actor akSeller)
EndEvent

; Event received when a spell is cast by this object
Event OnSpellCast(Form akSpell)
EndEvent

; Event received when translation is complete (from a call to TranslateTo)
Event OnTranslationComplete()
EndEvent

; Event received when translation is aborted (from a call to StopTranslateTo)
Event OnTranslationFailed()
EndEvent

; Event recieved when this reference hits a target
Event OnTrapHit(ObjectReference akTarget, float afXVel, float afYVel, float afZVel, float afXPos, float afYPos, float afZPos, \
	int aeMaterial, bool abInitialHit, int aeMotionType)
EndEvent

; Event recieved when this starts hitting a target
Event OnTrapHitStart(ObjectReference akTarget, float afXVel, float afYVel, float afZVel, float afXPos, float afYPos, float afZPos, \
	int aeMaterial, bool abInitialHit, int aeMotionType)
EndEvent

; Event recieved when this stops hitting a target
Event OnTrapHitStop(ObjectReference akTarget)
EndEvent

; Event received when a this trigger is tripped
Event OnTrigger(ObjectReference akActionRef)
EndEvent

; Event received when this trigger volume is entered
Event OnTriggerEnter(ObjectReference akActionRef)
EndEvent

; Event received when this trigger volume is left
Event OnTriggerLeave(ObjectReference akActionRef)
EndEvent

; Event received when this object is unequipped by an actor
Event OnUnequipped(Actor akActor)
EndEvent

; Event recieved when this object is being unloaded - will be fired every time this object is unloaded
Event OnUnload()
EndEvent

; Event that is triggered when this actor's combat state against the target changes
; State is as follows:
; 0 - not in combat
; 1 - in combat
; 2 - searching
Event OnCombatStateChanged(Actor akTarget, int aeCombatState)
EndEvent

; Event that is triggered when this actor sits in the furniture
Event OnSit(ObjectReference akFurniture)
EndEvent

; Event that is triggered when this actor leaves the furniture
Event OnGetUp(ObjectReference akFurniture)
EndEvent

; Event that is triggered when this actor finishes dying
Event OnDeath(Actor akKiller)
EndEvent

; Event that is triggered when this actor begins dying
Event OnDying(Actor akKiller)
EndEvent

; Event that is triggered when this actor changes from one location to another
Event OnLocationChange(Location akOldLoc, Location akNewLoc)
EndEvent

; Received when the lycanthropy state of this actor changes (when SendLycanthropyStateChanged is called)
Event OnLycanthropyStateChanged(bool abIsWerewolf)
EndEvent

; Event received when this actor equips something - akReference may be None if object is not persistent
Event OnObjectEquipped(Form akBaseObject, ObjectReference akReference)
EndEvent

; Event received when this actor unequips something - akReference may be None if object is not persistent
Event OnObjectUnequipped(Form akBaseObject, ObjectReference akReference)
EndEvent

; Event received when this actor starts a new package
Event OnPackageStart(Package akNewPackage)
EndEvent

; Event received when this actor's package changes
Event OnPackageChange(Package akOldPackage)
EndEvent

; Event received when this actor's package ends
Event OnPackageEnd(Package akOldPackage)
EndEvent

; Event received when this object's Ward is hit by a spell
Event OnWardHit(ObjectReference akCaster, Spell akSpell, int aiStatus)
EndEvent

; Received when the player fires a bow. akWeapon will be a bow, akAmmo is the ammo or None, 
; afPower will be 1.0 for a full-power shot, less for a dud, and abSunGazing will be true if the player is looking at the sun.
Event OnPlayerBowShot(Weapon akWeapon, Ammo akAmmo, float afPower, bool abSunGazing)
EndEvent

; Received when the player finishes fast travel, gives the duration of game time the travel took
Event OnPlayerFastTravelEnd(float afTravelGameTimeHours)
EndEvent

; Received immediately after the player has loaded a save game. A good time to check for additional content.
Event OnPlayerLoadGame()
EndEvent

; Received when StartVampireFeed is called on an actor
Event OnVampireFeed(Actor akTarget)
EndEvent

; Received when the vampirism state of this actor changes (when SendVampirismStateChanged is called)
Event OnVampirismStateChanged(bool abIsVampire)
EndEvent
