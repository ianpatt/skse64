Scriptname ReferenceAlias extends Alias Hidden
import ObjectReference

; Add an inventory event filter to this alias. Item added/removed events matching the
; specified form (or in the specified form list) will now be let through.
Function AddInventoryEventFilter(Form akFilter) native

; Clears the alias - fails on non-optional aliases
Function Clear() native

; Get the object reference this alias refers to
ObjectReference Function GetReference() native

; Forces this alias to use the specified reference
Function ForceRefTo(ObjectReference akNewRef) native

; Remove all inventory event filters from this alias - all item added/removed events will now be received
Function RemoveAllInventoryEventFilters() native

; Remove an inventory event filter from this alias. Item added/removed events matching the
; specified form (or in the specified form list) will no longer be let through.
Function RemoveInventoryEventFilter(Form akFilter) native

; SJML -- tries to force a reference into the alias, but only if it's already empty.
;  returns true if the alias now holds the passed reference, false if it was already filled.
bool Function ForceRefIfEmpty(ObjectReference akNewRef)
	if (GetReference())
		return False
	else
		ForceRefTo(akNewRef)
		return True
	endif
EndFunction

; Autocast to Actor is applicable
Actor Function GetActorReference()
	return GetReference() as Actor
endFunction

; Convenience function
ObjectReference Function GetRef()
	return GetReference()
endFunction

; Convenience fucntion
Actor Function GetActorRef()
	return GetReference() as Actor
endFunction


; Convenience function - jduvall
bool Function TryToAddToFaction(Faction FactionToAddTo)
	Actor ActorRef = GetReference() as Actor
	
	if ActorRef
		if (!ActorRef.IsInFaction(FactionToAddTo))
			ActorRef.SetFactionRank(FactionToAddTo, 0)
		endif
		Return True
	EndIf

	Return False
EndFunction

; Convenience function - jduvall
bool Function TryToRemoveFromFaction(Faction FactionToRemoveFrom)
	Actor ActorRef = GetReference() as Actor
	
	if ActorRef
		ActorRef.RemoveFromFaction(FactionToRemoveFrom)
		Return True
	EndIf

	Return False
EndFunction

; Convenience function - jduvall
bool Function TryToStopCombat()
	Actor ActorRef = GetReference() as Actor
	
	if ActorRef
		ActorRef.StopCombat()
		Return True
	EndIf

	Return False
EndFunction

; Convenience function - jduvall
bool Function TryToDisable()
	ObjectReference Ref = GetReference()
	
	if Ref
		Ref.Disable()
		Return True
	EndIf

	Return False
EndFunction

; Convenience function - wshen
bool Function TryToDisableNoWait()
	ObjectReference Ref = GetReference()
	
	if Ref
		Ref.DisableNoWait()
		Return True
	EndIf

	Return False
EndFunction

; Convenience function - jduvall
bool Function TryToEnable()
	ObjectReference Ref = GetReference()
	
	if Ref
		Ref.Enable()
		Return True
	EndIf

	Return False
EndFunction

; Convenience function - wshen
bool Function TryToEnableNoWait()
	ObjectReference Ref = GetReference()
	
	if Ref
		Ref.EnableNoWait()
		Return True
	EndIf

	Return False
EndFunction

; Convenience function - jduvall
bool Function TryToEvaluatePackage()
	Actor ActorRef = GetReference() as Actor
	
	if ActorRef
		ActorRef.EvaluatePackage()
		Return True
	EndIf

	Return False
EndFunction

; Convenience function - jduvall
bool Function TryToKill()
	Actor ActorRef = GetReference() as Actor
	
	if ActorRef
		ActorRef.Kill()
		Return True
	EndIf

	Return False
EndFunction

; Convenience function - jduvall
bool Function TryToMoveTo(ObjectReference RefToMoveTo)
	ObjectReference Ref = GetReference()
	
	if Ref
		Ref.MoveTo(RefToMoveTo)
		Return True
	EndIf

	Return False
EndFunction

; Convenience function - jduvall
bool Function TryToReset()
	ObjectReference Ref = GetReference()
	
	if Ref
		Ref.Reset()
		Return True
	EndIf

	Return False
EndFunction

; Convenience function - sjml
bool Function TryToClear()
	if (GetReference())
		Clear()
		return True
	endif

	return False
EndFunction

; The following events are received if the object the alias points at receives the events:

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

; Event that is triggered when this actor finishes dying (only if this alias points at an actor)
Event OnDeath(Actor akKiller)
EndEvent

; Event that is triggered when this actor begins dying (only if this alias points at an actor)
Event OnDying(Actor akKiller)
EndEvent

; Event received when an actor enters bleedout. (only if this alias points at an actor)
Event OnEnterBleedout()
EndEvent

; Event that is triggered when this actor changes from one location to another (only if this alias points at an actor)
Event OnLocationChange(Location akOldLoc, Location akNewLoc)
EndEvent

; Received when the lycanthropy state of this actor changes (when SendLycanthropyStateChanged is called)
Event OnLycanthropyStateChanged(bool abIsWerewolf)
EndEvent

; Event received when this actor equips something - akReference may be None if object is not persistent (only if this alias points at an actor)
Event OnObjectEquipped(Form akBaseObject, ObjectReference akReference)
EndEvent

; Event received when this actor unequips something - akReference may be None if object is not persistent (only if this alias points at an actor)
Event OnObjectUnequipped(Form akBaseObject, ObjectReference akReference)
EndEvent

; Event received when this actor starts a new package (only if this alias points at an actor)
Event OnPackageStart(Package akNewPackage)
EndEvent

; Event received when this actor's package changes (only if this alias points at an actor)
Event OnPackageChange(Package akOldPackage)
EndEvent

; Event received when this actor's package ends (only if this alias points at an actor)
Event OnPackageEnd(Package akOldPackage)
EndEvent

; Event received when this actor finishes changing its race
Event OnRaceSwitchComplete()
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
