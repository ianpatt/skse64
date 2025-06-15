Scriptname ObjectReference extends Form Hidden

bool FUNCTION rampRumble(float power = 0.5, float duration = 0.25, float falloff = 1600.0)
	; Function to shake cam/controller based on distance from player
	; should always be called on the source of the rumble, 
	; as final intensity is relevant to player
	if power > 1.0 || power <= 0
; 		debug.traceStack(self + " called rampRumble() but parameter 'power' was invalid.  Must be a non-zero float less than 1.0",1)
		; throw the warning, but don't return false - value gets clamped anyway
	endif
    Actor plyr = game.GetPlayer()
	float playerDist = (plyr).getDistance(self)
	; ignore if the player is too far away
	if playerDist < falloff
		float intensity = (1.0 - (playerDist / falloff))
		; ramp actual intensity down based on parameter value
		intensity = intensity*power		
		if intensity > 1.0
			; clamp to prevent invalid values
; 			debug.traceStack(self + " called for too much controller/camera shake.  Clamped to 1.0", 0)
			intensity = 1.0 
		elseif intensity <= 0.0
			; clamp to prevent invalid values
; 			debug.traceStack(self + " called for too little controller/camera shake", 0)
			intensity = 0.0
			return false
		endif
		game.shakeCamera(plyr, intensity)
		game.shakeController(intensity, intensity, duration)
		return true
	else
; 		debug.traceStack(self + "called for rampedRumble(), but player is too far away", 0)
		return False
	endif
endFUNCTION

; Function to know if I'm near the player (whether I can be safely enabled or disabled)
bool Function IsNearPlayer()
	Actor player = Game.GetPlayer()
	Cell targetCell = self.GetParentCell()
	Cell playerCell = player.GetParentCell()
	
	if (targetCell != playerCell)
		; player and target are in different cells
		if (targetCell && targetCell.IsInterior() || playerCell && playerCell.IsInterior())
			; in different cells and at least one is an interior
			;  -- we can safely enable or disable
			return false
		else
			; both in an exterior -- no means of testing 
			;  worldspace at the moment, so this will do.
			if (player.GetDistance(self) > 3000.0)
				; pretty darned far away -- safe
				return false
			else
				; too close for comfort
				return true
			endif
		endif
	else
		; in the same cell -- err on the side of caution
		return true
	endif
endFunction

;jduvall
bool Function IsInInterior()
{Returns !IsInExterior()}
  Cell parentCell = GetParentCell()
  Return parentCell && parentCell.IsInterior()
EndFunction

;kkuhlmann:
bool function MoveToIfUnloaded(ObjectReference akTarget, float afXOffset = 0.0, float afYOffset = 0.0, float afZOffset = 0.0)
{Calls MoveTo if the calling ObjectReference is currently unloaded. Doesn't do anything if it IS loaded. No waiting or while loops. Returns true if it does the moveto}
	if !Is3DLoaded()
		MoveTo(akTarget, afXOffset, afYOffset, afZOffset)
		return true
	else
		return false
	endif
endFunction

;jduvall:
function MoveToWhenUnloaded(ObjectReference akTarget, float afXOffset = 0.0, float afYOffset = 0.0, float afZOffset = 0.0)
{DEPRECATED: DO NOT USE. Calls MoveTo if both the calling ObjectReference and the akTarget ObjectReference have current locations that are not loaded.}
	while self.GetCurrentLocation().IsLoaded() || akTarget.GetCurrentLocation().IsLoaded()
		;do nothing
; 		debug.trace(self + "MoveToWhenUnloaded() waiting for current location and target location to be unloaded before moving. If called by a quest stage fragment, this may cause that quest stage to not complete until this function finishes (and if it's a startup stage, the quest will not report itself as running until the stage finishes.).", 1)
		Utility.Wait(5.0)	;when this function is threaded we can increase this wait time... I set it lower for testing purposes so it reevaluates faster when I need to purge cell buffers in the Civil War when calling moveto on the player between Civil War campaigns
	EndWhile
	self.MoveTo(akTarget, afXOffset, afYOffset, afZOffset)
EndFunction

;jduvall
Function DeleteWhenAble()
{This will become a native function... it will wait until the object is not persisting, then delete itself.}
	While GetParentCell() && GetParentCell().IsAttached()
		;do nothing
; 		debug.trace(self + "DeleteWhenAble() waiting for current location to be unloaded before deleting. If called by a quest stage fragment, this may cause that quest stage to not complete until this function finishes (and if it's a startup stage, the quest will not report itself as running until the stage finishes.).", 1)
		Utility.Wait(5.0) ;when this function is threaded we can increase this wait time... I set it lower for testing purposes so it reevaluates faster when I need to purge cell buffers in the Civil War when calling moveto on the player between Civil War campaigns
	EndWhile
	Delete()
EndFunction



;jduvall
Function AddKeyIfNeeded(ObjectReference ObjectWithNeededKey)
{Should only be called by ObjectReferences that have/are containers (ie Containers and Actors). Checks to see if self has the key to ObjectWithNeededKey, and if not, creates a copy of the key and puts it in self.}
	key NeededKey = ObjectWithNeededKey.GetKey()
	if NeededKey != None
		if GetItemCount(NeededKey) == 0
			AddItem(NeededKey)
		EndIf
	EndIf
EndFunction


; Property to obtain the current X position of the object
float Property X
  float Function get()
    return GetPositionX()
  EndFunction
EndProperty

; Property to obtain the current Y position of the object
float Property Y
  float Function get()
    return GetPositionY()
  EndFunction
EndProperty

; Property to obtain the current Z position of the object
float Property Z
  float Function get()
    return GetPositionZ()
  EndFunction
EndProperty

; Have akActivator activate this reference. If abDefaultProcessingOnly is true then any block will be bypassed
; and no OnActivate event will be sent. The function returns true if default processing ran, and succeeded. If
; default processing has been blocked, will always return false.
bool Function Activate(ObjectReference akActivator, bool abDefaultProcessingOnly = false) native

; Sets up a dependent animated object              
; This function should be used only with a coder supervision.  It is left undocumented because it can cause dangling pointers as well as very broken functionality 
; for the dependent object if used improperly.
bool Function AddDependentAnimatedObjectReference( ObjectReference akDependent ) native

; Add an inventory event filter to this reference. Item added/removed events matching the
; specified form (or in the specified form list) will now be let through.
Function AddInventoryEventFilter(Form akFilter) native

; Adds the specified base object or object reference to this object reference's container/inventory
; Note that you cannot add more then one copy of a reference to a container (a warning will be printed if you try)
Function AddItem(Form akItemToAdd, int aiCount = 1, bool abSilent = false) native

; Adds this reference (which is a map marker) to the map, optionally making it available for fast travel
Function AddToMap(bool abAllowFastTravel = false) native

; Apply an impulse to this reference
Function ApplyHavokImpulse(float afX, float afY, float afZ, float afMagnitude) native

; Turns on and off blocking of normal activation - OnActivate events will still be sent
Function BlockActivation(bool abBlocked = true) native

; Calculate's this references encounter level based on the requested difficulty level
; 0 - Easy
; 1 - Medium
; 2 - Hard
; 3 - Very Hard
; 4 - None
int Function CalculateEncounterLevel(int aiDifficulty = 4) native

; Can the map marker be fast traveled to?
bool Function CanFastTravelToMarker() native

; Clears all effects of destruction from this object
Function ClearDestruction() native

; Create a detection event at this reference, with the specified owner. Sound level is between 0 and 100

Function CreateDetectionEvent(Actor akOwner, int aiSoundLevel = 0 ) native

; Damages this object and advances the destruction stage - does not return until the object is damaged
Function DamageObject(float afDamage) native

; Delets this object
Function Delete() native

; Disables this object - fading out if requested
Function Disable(bool abFadeOut = false) native

; Disables this object - fading out if requested. Does NOT wait for the fade or disable to finish
Function DisableNoWait(bool abFadeOut = false) native

; Drops the specified object from this object's inventory
ObjectReference Function DropObject(Form akObject, int aiCount = 1) native

; Enables this object - fading in if requested
Function Enable(bool abFadeIn = false) native

; Enables the ability to fast travel to this marker - or disables it. Note that if you disable
; fast travel the player will see "You haven't discovered this location" as an error message
Function EnableFastTravel(bool abEnable = true) native

; Enables this object - fading in if requested. Does NOT wait for the fade or enable to finish
Function EnableNoWait(bool abFadeIn = false) native

; Forcibly adds / removes the ragdoll for a reference to the world
Function ForceAddRagdollToWorld() native
Function ForceRemoveRagdollFromWorld() native

; Gets the actor that owns this object (or None if not owned by an Actor)
ActorBase Function GetActorOwner() native

; Get the current X angle of this object
float Function GetAngleX() native

; Get the current Y angle of this object
float Function GetAngleY() native

; Get the current Z angle of this object
float Function GetAngleZ() native

; Get a variable from the reference's animation graph (if applicable). Bool version.
bool Function GetAnimationVariableBool(string arVariableName) native

; Get a variable from the reference's animation graph (if applicable). Int version.
int Function GetAnimationVariableInt(string arVariableName) native

; Get a variable from the reference's animation graph (if applicable). Float version.
float Function GetAnimationVariableFloat(string arVariableName) native

; Returns the base object this reference represents
Form Function GetBaseObject() native

; Returns the object's current destruction stage
int Function GetCurrentDestructionStage() native

; Returns this reference's current location
Location Function GetCurrentLocation() native

; Returns the scene this reference is currently in - if any
Scene Function GetCurrentScene() native

; Calculates the distance between this reference and another - both must either be in the same interior, or same worldspace
float Function GetDistance(ObjectReference akOther) native

; Returns this reference's editor location
Location Function GetEditorLocation() native

; Gets the faction that owns this object (or None if not owned by a Faction)
Faction Function GetFactionOwner() native

; Gets the angle between this object's heading and the other object in degrees - in the range from -180 to 180
float Function GetHeadingAngle(ObjectReference akOther) native

; Get the current height of the object
float Function GetHeight() native

; Returns how many of the specified item is in this object reference's inventory
int Function GetItemCount(Form akItem) native

; Returns the smithed health of this object reference (1.0 == 100%)
float Function GetItemHealthPercent() native

; Returns the key base object that will unlock this object
Key Function GetKey() native

; Get the current length of the object
float Function GetLength() native

; Get our linked reference
ObjectReference Function GetLinkedRef(Keyword apKeyword = NONE) native

; Get the level of the lock on this object
int Function GetLockLevel() native

;jtucker, jduvall
;This function  counts the number of linked refs that are in a linked Ref chain (ie object is linked to A, A is linked to B, etc. this then counts all the linked refs.)
;Often used in conjunction with GetNthLinkedRef()
;*** WARNING: Having a link ref chain that at any point loops back on itself and calling this function will result in very bad things. Don't do that!***
int Function countLinkedRefChain(keyword apKeyword = None, int maxExpectedLinkedRefs = 100)	
	;Don't use this on a loop of linked refs.
	ObjectReference CurrentLink = self
	ObjectReference NewLink
	int NumLinkedRefs = 0 
	
 	while(currentLink) && NumLinkedRefs <= maxExpectedLinkedRefs 
		
		NewLink = currentLink.getLinkedRef(apKeyword)
		
		if NewLink != self
			currentLink = NewLink
			NumLinkedRefs = NumLinkedRefs + 1
		Else
			currentLink = None
; 			debug.trace( self + "countLinkedRefs() found itself. This suggests it was linked back to itself. This will create an infinite loop, so we are killing the function now. NumLinkedRefs =" + NumLinkedRefs)
		EndIf
				
	endWhile
	
	if NumLinkedRefs >= maxExpectedLinkedRefs
; 		debug.trace( self + "countLinkedRefs() bailing out early because it found more linked refs than maxExpectedLinkRefs (suggesting an infinite loop). LinkedRefs found:" + NumLinkedRefs + ", maxExpectedLinkedRefs:" + maxExpectedLinkedRefs)
	EndIf
	
	
	return NumLinkedRefs
	
endFunction


; Returns the Nth linked ref from this reference (0 = self, 1 = GetLinkedRef, 2 = GetLinkedRef.GetLinkedRef, etc)
ObjectReference Function GetNthLinkedRef(int aiLinkedRef) native


; Enables all of the references that are linked, in a chain, to this one.
Function EnableLinkChain(Keyword apKeyword = None)
	ObjectReference CurrentLink = GetLinkedRef(apKeyword)
	While CurrentLink
		CurrentLink.Enable()
		CurrentLink = CurrentLink.GetLinkedRef(apKeyword)
	endWhile
endFunction


; Disables all of the references that are linked, in a chain, to this one.
Function DisableLinkChain(Keyword apKeyword = None, bool abFadeOut = false)
	ObjectReference CurrentLink = GetLinkedRef(apKeyword)
	While CurrentLink
		CurrentLink.Disable(abFadeOut)
		CurrentLink = CurrentLink.GetLinkedRef(apKeyword)
	endWhile
endFunction


; Get this object's mass
float Function GetMass() native

; Gets the open state of this object. Which can be one of the following:
; 0 - None
; 1 - Open
; 2 - Opening
; 3 - Closed
; 4 - Closing
int Function GetOpenState() native

; Gets the cell this object is in
Cell Function GetParentCell() native

; Get the current X position of the object
float Function GetPositionX() native

; Get the current Y position of the object
float Function GetPositionY() native

; Get the current Z position of the object
float Function GetPositionZ() native

; Get the current scale of the object
float Function GetScale() native

; Get the number of objects inside this trigger (throws warning if not a triggger)
int Function GetTriggerObjectCount() native

; Gets the voice type for this reference. Will return None if not an actor or a talking activator
VoiceType Function GetVoiceType() native

; Get the current width of the object
float Function GetWidth() native

; Get this objects worldspace
WorldSpace Function GetWorldSpace() native

; Returns self cast as an actor
actor Function GetSelfAsActor()
	return self as Actor
endFunction

; Returns if this reference has an active effect coming from a magic effect with the specified keyword attached
bool Function HasEffectKeyword(Keyword akKeyword) native

; Returns whether the reference has the given node
bool Function HasNode(string asNodeName) native

; Returns if this reference has the specified location ref type
bool Function HasRefType(LocationRefType akRefType) native

; Flags this reference as ignoring (or not ignoring) friendly hits
Function IgnoreFriendlyHits(bool abIgnore = true) native

; Interrupts any spell-casting this object may be doing
Function InterruptCast() native

; Checks to see if the passed in reference is the activate child of this one
bool Function IsActivateChild(ObjectReference akChild) native

; Checks to see if activation is currently blocked on this object
bool Function IsActivationBlocked() native

; Returns if the 3d for this object is loaded or not
bool Function Is3DLoaded() native

; Is this object currently flagged for delete?
bool Function IsDeleted() native

; Is this object currently disabled?
bool Function IsDisabled() native

; Because Shane got tired of remembering which way to call this
bool Function IsEnabled()
	return !IsDisabled()
EndFunction

; Is any marker on this furniture in use?
bool Function IsFurnitureInUse(bool abIgnoreReserved = false) native

; Is a particular marker on this furniture in use?
bool Function IsFurnitureMarkerInUse(int aiMarker, bool abIgnoreReserved = false) native

; Is this object ignoring friendly hits?
bool Function IsIgnoringFriendlyHits() native

; Is this actor or talking activator currently talking to the player?
bool Function IsInDialogueWithPlayer() native

; Is the lock on this object broken?
bool Function IsLockBroken() native

; Is the lock on this object locked?
bool Function IsLocked() native

; Is the map marker visible?
bool Function IsMapMarkerVisible() native

; Executes a knock effect to an area
Function KnockAreaEffect(float afMagnitude, float afRadius) native

; Lock/unlock this object. If told to lock it, it will add a lock if it doesn't have one. If locked/unlocked as the owner on a door,
; the adjoining cell will be made public/private as appropriate
Function Lock(bool abLock = true, bool abAsOwner = false) native

; Moves this object to the position of the specified object, with an offset, and optionally matching its rotation
Function MoveTo(ObjectReference akTarget, float afXOffset = 0.0, float afYOffset = 0.0, float afZOffset = 0.0, bool abMatchRotation = true) native

; Moves this object to the position (and rotation) of the specified object's interaction position
Function MoveToInteractionLocation(ObjectReference akTarget) native

; Moves this object to its editor location
Function MoveToMyEditorLocation() native

; Moves this object to the position (and rotation) of the specified node on the specified object's 3D
Function MoveToNode(ObjectReference akTarget, string asNodeName) native

; Create x copies of the passed in form (forcing them to persist if desired) and place them at our location, returning the last object created
ObjectReference Function PlaceAtMe(Form akFormToPlace, int aiCount = 1, bool abForcePersist = false, bool abInitiallyDisabled = false) native

; Create an actor at this object's location. Level mod is one of the following:
; 0 - Easy
; 1 - Medium
; 2 - Hard
; 3 - Boss
; 4 - None
Actor Function PlaceActorAtMe(ActorBase akActorToPlace, int aiLevelMod = 4, EncounterZone akZone = None) native

; Start the specified animation playing - returns true if it succeeds
bool Function PlayAnimation(string asAnimation) native

; Start the specified animation playing and wait for the specified event - returns true if succeeds
bool Function PlayAnimationAndWait(string asAnimation, string asEventName) native

; Start the specified Gamebryo animation playing - returns true if it succeeds
bool Function PlayGamebryoAnimation(string asAnimation, bool abStartOver = false, float afEaseInTime = 0.0) native

; Play the specified impact effect - returns true if it succeeds
bool Function PlayImpactEffect(ImpactDataSet akImpactEffect, string asNodeName = "", float afPickDirX = 0.0, float afPickDirY = 0.0, float afPickDirZ = -1.0, float afPickLength = 512.0, bool abApplyNodeRotation = false, bool abUseNodeLocalRotation = false) native

; Play two animations at once - one on this object, one on another object
bool Function PlaySyncedAnimationSS(string asAnimation1, ObjectReference akObj2, string asAnimation2) native

; Play two animations at once - one on this object, one on another object - and wait for both
bool Function PlaySyncedAnimationAndWaitSS(string asAnimation1, string asEvent1, ObjectReference akObj2, string asAnimation2, string asEvent2) native

; Play a terrain effect that is attached to the specified bone of this object.
Function PlayTerrainEffect(string asEffectModelName, string asAttachBoneName) native

; Tells this object to process a trap hitting it
Function ProcessTrapHit(ObjectReference akTrap, float afDamage, float afPushback, float afXVel, float afYVel, float afZVel, float afXPos, float afYPos, float afZPos, int aeMaterial, float afStagger) native

; Pushes the passed-in actor away from this object, using the passed in knockback force to determine the speed
Function PushActorAway(Actor akActorToPush, float aiKnockbackForce) native

; Remove all inventory event filters from this reference - all item added/removed events will now be received
Function RemoveAllInventoryEventFilters() native

; Removes all items from this container, transferring it to the other object if passed
Function RemoveAllItems(ObjectReference akTransferTo = None, bool abKeepOwnership = false, bool abRemoveQuestItems = false) native

; Remove an inventory event filter from this reference. Item added/removed events matching the
; specified form (or in the specified form list) will no longer be let through.
Function RemoveInventoryEventFilter(Form akFilter) native

; Removes the specified item from this object reference's inventory
Function RemoveItem(Form akItemToRemove, int aiCount = 1, bool abSilent = false, ObjectReference akOtherContainer = None) native

; Removes a previously added dependent object
; This function should be used only with a coder supervision.  It is left undocumented because it can cause dangling pointers as well as very broken functionality 
; for the dependent object if used improperly.
bool Function RemoveDependentAnimatedObjectReference( ObjectReference akDependent ) native

; Resets this object, optional place the object at the new target
Function Reset(ObjectReference akTarget = None) native

; Has this object "say" the specified topic, as if spoken by the specified actor (if one is
; provided, and potentially "speaking" in the player's head.
Function Say(Topic akTopicToSay, Actor akActorToSpeakAs = None, bool abSpeakInPlayersHead = false) native

; Has this object behave as if the specified actor attempted to steal it
Function SendStealAlarm(Actor akThief) native

; Sets this object's actor cause to the specified actor
Function SetActorCause(Actor akActor) native

; Sets this object's owner to the specified actor base - None means to remove ownership
Function SetActorOwner(ActorBase akActorBase) native

; Set the orientation of the object (angles are in degrees)
Function SetAngle(float afXAngle, float afYAngle, float afZAngle) native

; Set a variable on the reference's animation graph (if applicable). Bool version.
Function SetAnimationVariableBool(string arVariableName, bool abNewValue) native

; Set a variable on the reference's animation graph (if applicable). Int version.
Function SetAnimationVariableInt(string arVariableName, int aiNewValue) native

; Set a variable on the reference's animation graph (if applicable). Float version.
Function SetAnimationVariableFloat(string arVariableName, float afNewValue) native

; Sets this object as destroyed or not
Function SetDestroyed(bool abDestroyed = true) native

; Sets this object's owner to the specified faction
Function SetFactionOwner(Faction akFaction) native

; Sets the lock level on this object. Will add an unlocked lock to it if it doesn't have one
Function SetLockLevel(int aiLockLevel) native

; Sets the motion type of the reference
; aeMotionType: The type of motion (see properties at end of file)
; abAllowActivate: When setting to a dynamic type, allows the simulation to be activated
Function SetMotionType(int aeMotionType, bool abAllowActivate = true) native

; Sets this object reference as one that teammates will refuse to do favors on
Function SetNoFavorAllowed(bool abNoFavor = true) native

; Opens/closes this object
Function SetOpen(bool abOpen = true) native

; Set the position of the object
Function SetPosition(float afX, float afY, float afZ) native

; Set the current scale of the object
Function SetScale(float afScale) native

; Makes the reference translate to the given position/orientation
; Note: Rotation speed is entirely dependent on the length of the path and the movement speed
; that is, the rotation will happen such that the reference reaches the goal orientation at the end
; of the translation.
Function TranslateTo(float afX, float afY, float afZ, float afXAngle, float afYAngle, float afZAngle, float afSpeed, float afMaxRotationSpeed = 0.0) native

; Makes the reference translate to the given position/orientation on a spline
Function SplineTranslateTo(float afX, float afY, float afZ, float afXAngle, float afYAngle, float afZAngle, float afTangentMagnitude, float afSpeed, float afMaxRotationSpeed = 0.0) native

; Makes the reference translate to the target node's ref/orient on a spline at the given speed
Function SplineTranslateToRefNode(ObjectReference arTarget, string arNodeName, float afTangentMagnitude, float afSpeed, float afMaxRotationSpeed = 0.0) native

; Stops the reference from moving
Function StopTranslation() native

; Makes the reference translate to the target ref position/orient at the given speed
Function TranslateToRef(ObjectReference arTarget, float afSpeed, float afMaxRotationSpeed = 0.0)
	TranslateTo(arTarget.X, arTarget.Y, arTarget.Z, arTarget.GetAngleX(), arTarget.GetAngleY(), arTarget.GetAngleZ(), afSpeed, afMaxRotationSpeed)
endFunction

; Makes the reference translate to the target ref position/orient on a spline at the given speed
Function SplineTranslateToRef(ObjectReference arTarget, float afTangentMagnitude, float afSpeed, float afMaxRotationSpeed = 0.0)
	SplineTranslateTo(arTarget.X, arTarget.Y, arTarget.Z, arTarget.GetAngleX(), arTarget.GetAngleY(), arTarget.GetAngleZ(), afTangentMagnitude, afSpeed, afMaxRotationSpeed)
endFunction

; Tether a prisoner cart to the given horse.
Function TetherToHorse(ObjectReference akHorse) native

; Waits for the animation graph to send the specified event
bool Function WaitForAnimationEvent(string asEventName) native

; Convenience function to check if I'm in a location or any of its children
bool Function IsInLocation(Location akLocation)
	; cache current location to avoid changing location while this function is running (surprisingly that seems to be happening occasionally)
	Location currLoc = GetCurrentLocation()
	if currLoc == None
		return false
	else
		return akLocation.IsChild(currLoc) || currLoc == akLocation
	endif
endFunction

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

; Event received when a this trigger is tripped
Event OnTrigger(ObjectReference akActionRef)
EndEvent

; Event received when this trigger volume is entered
Event OnTriggerEnter(ObjectReference akActionRef)
EndEvent

; Event received when this trigger volume is left
Event OnTriggerLeave(ObjectReference akActionRef)
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

; Event received when translation is almost complete (from a call to TranslateTo, "almost" is determined by a gamesetting, default is 90% of the way)
Event OnTranslationAlmostComplete()
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

; Event received when this object is unequipped by an actor
Event OnUnequipped(Actor akActor)
EndEvent

; Event recieved when this object is being unloaded - will be fired every time this object is unloaded
Event OnUnload()
EndEvent

; Event received when this object's Ward is hit by a spell
Event OnWardHit(ObjectReference akCaster, Spell akSpell, int aiStatus)
EndEvent

; Set of read-only properties to essentually make a fake enum for motion types passed in to the trap hit
int Property Motion_Dynamic = 1 AutoReadOnly
int Property Motion_SphereIntertia = 2 AutoReadOnly
int Property Motion_BoxIntertia = 3 AutoReadOnly
int Property Motion_Keyframed = 4 AutoReadOnly
int Property Motion_Fixed = 5 AutoReadOnly
int Property Motion_ThinBoxIntertia = 6 AutoReadOnly
int Property Motion_Character = 7 AutoReadOnly

; added in 1.6.1126
Bool Function IsContainerEmpty() Native
Function RemoveAllStolenItems(ObjectReference akTransferTo) Native
Function SetContainerAllowStolenItems(Bool setAllowStolenItems) Native
Int Function GetAllItemsCount() Native
