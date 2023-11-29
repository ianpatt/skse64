Scriptname Actor extends ObjectReference Hidden

; Relationship functions use the following values:
; 4 - Lover
; 3 - Ally
; 2 - Confidant
; 1 - Friend
; 0 - Acquaintance
; -1 - Rival
; -2 - Foe
; -3 - Enemy
; -4 - Archnemesis

; DEPRECATED - use MakePlayerFriend() instead
; replacement for ModFavorPoints
; if iFavorPoints > 0, will setRelationshipRank to 1 if 0
; otherwise, won't do anything
Function ModFavorPoints(int iFavorPoints = 1)
	if iFavorPoints > 0
		MakePlayerFriend()
	else
; 		debug.trace(self + " ModFavorPoints called with negative param. NO EFFECT.")
	endif
endFunction

; also DEPRECATED
Function ModFavorPointsWithGlobal(GlobalVariable FavorPointsGlobal)
	ModFavorPoints(FavorPointsGlobal.GetValue() as int)
endFunction

;this function will make an actor a friend of the player if allowed
Function MakePlayerFriend()
	ActorBase myBase = GetBaseObject() as ActorBase
	if myBase.IsUnique()
		Actor plyr = game.GetPlayer()
		if GetRelationshipRank(plyr) == 0
; 			debug.trace(self + " MakePlayerFriend called on neutral actor - changed to FRIEND.")
			SetRelationshipRank(plyr, 1)
		else
; 			debug.trace(self + " MakePlayerFriend called on non-neutral actor - NO EFFECT.")
		endif
	else
; 		debug.trace(self + " MakePlayerFriend called on non-Unique actor. NO EFFECT.")
	endif
endFunction

; Adds the specified perk to this actor
Function AddPerk(Perk akPerk) native

; Adds the specified shout to this actor - returns true on success
bool Function AddShout(Shout akShout) native

; Adds the specified spell to this actor - returns true on success
bool Function AddSpell(Spell akSpell, bool abVerbose=true) native

; Sets this a essential actors ability to talk when in a bleedout state
Function AllowBleedoutDialogue(bool abCanTalk ) native

; overrides the race flag on an actor and determines if he can talk to the player in dialogue menu
Function AllowPCDialogue(bool abTalk) native

; Attaches an "ash pile" to this actor, placing it at this actor's location and using the specified
; base object (or leveled item list) to represent the pile. If None is passed, it will use the
; default ash pile object
Function AttachAshPile(Form akAshPileBase = None) native

; Can this actor fly here?
bool Function CanFlyHere() native

; Clears this actor's arrested state
Function ClearArrested() native

; Clears any expression override on the actor
Function ClearExpressionOverride() native

; Clears this actor's extra arrows 3D
Function ClearExtraArrows() native

; Remove the obligation to use a particular marker when this actor has to land.
Function ClearForcedLandingMarker()
  SetForcedLandingMarker( None )
endFunction

; Clear any keep offset from actor settings
Function ClearKeepOffsetFromActor() native

; Clears this actor's look at target
Function ClearLookAt() native

; Damages the specified actor value
Function DamageActorValue(string asValueName, float afDamage) native

; Alias for DamageActorValue - damages the specified actor value
Function DamageAV(string asValueName, float afDamage)
  DamageActorValue(asValueName, afDamage)
EndFunction

; Initiates a dismount.
bool Function Dismount() native

; Dispel all spells from this actor
Function DispelAllSpells() native

; Dispel a spell from this actor
bool Function DispelSpell( Spell akSpell ) native

; Apply a spell to a target in combat
Function DoCombatSpellApply( Spell akSpell, ObjectReference akTarget ) native

; Enables or disable's this actor's AI
Function EnableAI(bool abEnable = true) native

; End the Deferred Kill state. This must only be called if StartDeferredKill was called first.
Function EndDeferredKill() native

; Forces this actor to equip the specified item, preventing removal if requested
Function EquipItem(Form akItem, bool abPreventRemoval = false, bool abSilent = false) native

; Forces this actor to equip the specified shout
Function EquipShout(Shout akShout) native

; Forces this actor to equip the specified spell. The casting source can be:
; 0 - Left hand
; 1 - Right hand
Function EquipSpell(Spell akSpell, int aiSource) native

; Forces the AI to re-evaluate its package stack
Function EvaluatePackage() native

; Force the specified actor value to a specified value
Function ForceActorValue(string asValueName, float afNewValue) native

; Alias for ForceActorValue - force the specified actor value to a specified value
Function ForceAV(string asValueName, float afNewValue)
  ForceActorValue(asValueName, afNewValue)
EndFunction

;returns the ActorBase
ActorBase function GetActorBase()
	return GetBaseObject() as ActorBase
endFunction

; Gets the specified actor value - returns 0 and logs an error if the value is unknown
float Function GetActorValue(string asValueName) native

; Gets the specified actor value's max, taking into account buffs/debuffs
float Function GetActorValueMax(string asValueName) native

; Gets the specified actor value as a percentage of its max value - from 0 to 1
float Function GetActorValuePercentage(string asValueName) native

; Alias for GetActorValue - retrives the specified actor value
float Function GetAV(string asValueName)
  return GetActorValue(asValueName)
EndFunction

; Alias of GetActorValueMax - retrives actor value's max, taking into account buffs/debuffs
float Function GetAVMax(string asValueName)
	return GetActorValueMax(asValueName)
EndFunction

; Alias for GetActorValuePercentage - gets the actor value as a percent of max
float Function GetAVPercentage(string asValueName)
  return GetActorValuePercentage(asValueName)
EndFunction

; Gets the base value of the specified actor value - returns 0 and logs an error if the value is unknown
float Function GetBaseActorValue(string asValueName) native

; Alias for GetBaseActorValue - retrieves the specified actor value's base value
float Function GetBaseAV(string asValueName)
  return GetBaseActorValue(asValueName)
EndFunction

; Obtains how much it would cost to bribe this actor
int Function GetBribeAmount() native

; Get the faction this actor reports crimes to
Faction Function GetCrimeFaction() native

; Gets this actor's current combat state
int Function GetCombatState() native

; Gets this actor's current combat target
Actor Function GetCombatTarget() native

; Gets this actor's current AI package
Package Function GetCurrentPackage() native

; Gets this actor's current dialogue target
Actor Function GetDialogueTarget() native

; Obtain the armor currently equipped in the specified slot
Armor Function GetEquippedArmorInSlot(int aiSlot) native

; Obtains the item quipped in the specified hand (0 - Left hand, 1 - Right hand)
; Return values are:
; -1 - Error
; 0 - Nothing
; 1 - One-handed sword
; 2 - One-handed dagger
; 3 - One-handed axe
; 4 - One-handed mace
; 5 - Two-handed sword
; 6 - Two-handed axe
; 7 - Bow
; 8 - Staff
; 9 - Magic spell
; 10 - Shield
; 11 - Torch
int Function GetEquippedItemType(int aiHand) native

; Gets this actor's currently equipped shout
Shout Function GetEquippedShout() native

; Gets this actor's currently equipped weapon
; false - Default - Right Hand
; true - Left Hand
Weapon Function GetEquippedWeapon(bool abLeftHand = false) native

; Gets this actor's currently equipped shield
Armor Function GetEquippedShield() native

; Gets the spell currently equipped in the specified source
; 0 - Left Hand
; 1 - Right Hand
; 2 - Other
; 3 - Instant
Spell Function GetEquippedSpell(int aiSource) native

; Obtains this actor's rank with the specified faction - returns -1 if the actor is not a member
int Function GetFactionRank(Faction akFaction) native

; Obtains this actor's faction-based reaction to the other actor
; 0 - Neutral
; 1 - Enemy
; 2 - Ally
; 3 - Friend
int Function GetFactionReaction(Actor akOther) native

; Obtains this actor's current flight state
; 0 - Not flying
; 1 - Taking off
; 2 - Cruising
; 3 - Hovering
; 4 - Landing
int Function GetFlyingState() native

; Get the ref at which this actor is obliged to land, if one is set (or none, if not).
ObjectReference Function GetForcedLandingMarker() native

; Retrieves the amount of gold this actor has
int Function GetGoldAmount() native

; Gets this actor's highest relationship rank - returns 0 if they have no relationships
int Function GetHighestRelationshipRank() native

; Returns this actor's killer - or None if this actor is still alive
Actor Function GetKiller() native

; Returns this actor's current level.
int Function GetLevel() native

; Returns this actor's current light level.
float Function GetLightLevel() native

; Gets this actor's highest relationship rank - returns 0 if they have no relationships
int Function GetLowestRelationshipRank() native

; Obtains a leveled actor's "fake" base (the one generated by the game when the
; actor is leveled. This differs from GetActorBase which will return the editor base
; object)
ActorBase Function GetLeveledActorBase() native

; Queries whether this actor has no bleedout recovery flag set.
bool Function GetNoBleedoutRecovery() native

; Queries whether this actor receives player input
bool Function GetPlayerControls() native

; Returns this actor's race
Race Function GetRace() native

; Obtains the relationship rank between this actor and another
int Function GetRelationshipRank(Actor akOther) native

; Obtains this actor's sit state, which is one of the following:
; 0 - Not sitting
; 2 - Not sitting, wants to sit
; 3 - Sitting
; 4 - Sitting, wants to stand
int Function GetSitState() native

; Obtains this actor's sleep state, which is one of the following:
; 0 - Not sleeping
; 2 - Not sleeping, wants to sleep
; 3 - Sleeping
; 4 - Sleeping, wants to wake
int Function GetSleepState() native

; Gets the voice recovery timer from the actor
float Function GetVoiceRecoveryTime() native

; Gets the total "warmth rating" for this actor
float Function GetWarmthRating() native

; Checks to see if this actor has the specified association with the other actor - or anyone (if no actor is passed)
bool Function HasAssociation(AssociationType akAssociation, Actor akOther = None) native

; Checks to see if this actor has a family relationship with the other actor - or anyone (if no actor is passed)
bool Function HasFamilyRelationship(Actor akOther = None) native

; Sees if this actor has line-of-sight to another object. Only the player can check LOS to a non-actor
bool Function HasLOS(ObjectReference akOther) native

; Checks to see if this actor is currently being affected by the given Magic Effect
bool Function HasMagicEffect(MagicEffect akEffect) native

; Checks to see if this actor is currently being affected by a Magic Effect with the given Keyword
bool Function HasMagicEffectWithKeyword(Keyword akKeyword) native

; Checks to see if this actor has a parent relationship with the other actor
bool Function HasParentRelationship(Actor akOther) native

; Checks to see if this actor has the given Perk
bool Function HasPerk(Perk akPerk) native

; Checks to see if this actor has the given Spell or Shout
bool Function HasSpell(Form akForm) native

; Returns if this actor is alarmed or not
bool Function IsAlarmed() native

; Returns if this actor is alerted or not
bool Function IsAlerted() native

; Is this actor allowed to fly?
bool Function IsAllowedToFly() native

; Is this actor currently arrested?
bool Function IsArrested() native

; Is this actor currently arresting his target? (Must be a guard and alarmed)
bool Function IsArrestingTarget() native

; Is the actor being ridden?
bool Function IsBeingRidden() native

; Is this actor currently bleeding out?
bool Function IsBleedingOut() native

; Queries whether this actor has player bribe flag set.
bool Function IsBribed() native

; Is this actor a child?
bool Function IsChild() native

; Is this actor a commanded by another?
bool Function IsCommandedActor() native

; Returns if this actor is dead or not
bool Function IsDead() native

; Returns if this actor is detected by the other one
bool Function IsDetectedBy(Actor akOther) native

; Is this actor doing a favor for the player?
bool Function IsDoingFavor() native

; Returns if the specified object is equipped on this actor
bool Function IsEquipped(Form akItem) native

; Is this actor essential?
bool Function IsEssential() native

; Returns if this actor is flying or not
bool Function IsFlying() native

; Returns if this actor is a guard or not
bool Function IsGuard() native

; Is this actor flagged as a ghost?
bool Function IsGhost() native

; Is this actor hostile to another actor?
bool Function IsHostileToActor(Actor akActor) native

; Returns if this actor is currently in combat
bool Function IsInCombat() native

; Checks to see if this actor is a member of the specified faction
bool Function IsInFaction(Faction akFaction) native

; Returns if this actor is in a kill move or not
bool Function IsInKillMove() native

; Queries whether this actor has player intimidated flag set.
bool Function IsIntimidated() native

; Is the actor on a mount?
bool Function IsOnMount() native

; Is the actor over-encumbered?
bool Function IsOverEncumbered() native

; Checks to see if this actor the last ridden horse of the player
bool Function IsPlayersLastRiddenHorse() native

; Is this actor currently a teammate of the player?
bool Function IsPlayerTeammate() native

; Is this actor currently running?
bool Function IsRunning() native

; Is this actor currently sneaking?
bool Function IsSneaking() native

; Is this actor currently sprinting?
bool Function IsSprinting() native

; Is this actor trespassing?
bool Function IsTrespassing() native

; Is this actor unconscious?
bool Function IsUnconscious() native

; Does this actor have his weapon and/or magic drawn?
bool Function IsWeaponDrawn() native

; Sets the actor to a mode where it will keep a given offset from another actor
Function KeepOffsetFromActor(Actor arTarget, float afOffsetX, float afOffsetY, float afOffsetZ, float afOffsetAngleX = 0.0,  float afOffsetAngleY = 0.0,  float afOffsetAngleZ = 0.0, float afCatchUpRadius = 20.0, float afFollowRadius = 5.0) native

; Kills this actor with the killer being the guilty party
Function Kill(Actor akKiller = None) native

; Kills this actor even if essential
Function KillEssential(Actor akKiller = None)
	ActorBase akActorBase = GetBaseObject() as ActorBase
	if akActorBase.IsUnique()
		akActorBase.SetEssential(0)
	endif
	Kill(akKiller)
endFunction

; Kills this actor without a kill event with the killer being the guilty party
Function KillSilent(Actor akKiller = None) native

; Modifies the specified actor value
Function ModActorValue(string asValueName, float afAmount) native

; Alias for ModActorValue - modifies the specified actor value
Function ModAV(string asValueName, float afAmount)
  ModActorValue(asValueName, afAmount)
EndFunction

; Modifies this actor's rank in the faction
Function ModFactionRank(Faction akFaction, int aiMod) native

; Pop this actor to the initial location for a package. Mainly for use on 
; disabled actors, since they would normally start at their editor locations.
Function MoveToPackageLocation( ) native

; Opens this actor's inventory, as if you were pick-pocketing them. Only works on teammates, or anyone if forced.
Function OpenInventory(bool abForceOpen = false) native

; Make the actor path to a reference, latent version
; Note: this method doesn't return until the goal is reached or pathing
; failed or was interrupted (by another request for instance)
bool Function PathToReference(ObjectReference aTarget, float afWalkRunPercent) native

; Send an idle to the actor to load in and play.
bool Function PlayIdle(Idle akIdle) native

; Send an idle to the actor to play, overriding its target with the specified reference
bool Function PlayIdleWithTarget(Idle akIdle, ObjectReference akTarget) native

; Send an event to the subgraphs of an actor.
Function PlaySubGraphAnimation(string asEventName) native

; Removes this actor from the specified faction
Function RemoveFromFaction(Faction akFaction) native

; Removes this actor from all factions
Function RemoveFromAllFactions() native

; Removes the specified perk from this actor
Function RemovePerk(Perk akPerk) native

; Removes the specified shout from this actor - returns true on success
bool Function RemoveShout(Shout akShout) native

; Removes the specified spell from this actor - returns true on success
bool Function RemoveSpell(Spell akSpell) native

; Resets this actor's health and limb state
Function ResetHealthAndLimbs() native

; Restores damage done to the actor value (up to 0 damage)
Function RestoreActorValue(string asValueName, float afAmount) native

; Resurrects this actor
Function Resurrect() native

; Alias for RestoreActorValue - restores damage done to the actor value
Function RestoreAV(string asValueName, float afAmount)
  RestoreActorValue(asValueName, afAmount)
EndFunction

; Has this actor behave as if assaulted
Function SendAssaultAlarm() native

; Tell anyone who cares that the lycanthropy state of this actor has changed
Function SendLycanthropyStateChanged(bool abIsWerewolf) native

; Has this actor behave as if they caught the target trespassing
Function SendTrespassAlarm(Actor akCriminal) native

; Tell anyone who cares that the vampirism state of this actor has changed
Function SendVampirismStateChanged(bool abIsVampire) native

; Sets the specified actor value
Function SetActorValue(string asValueName, float afValue) native

; Sets the actor in an alerted state
Function SetAlert(bool abAlerted = true) native

; Sets whether this actor is allowed to fly or not - if not, will land the actor
Function SetAllowFlying(bool abAllowed = true) native

; Sets whether this actor is allowed to fly or not - if not, will land the actor
Function SetAllowFlyingEx(bool abAllowed = true, bool abAllowCrash = true, bool abAllowSearch = false) native

; Sets this actor's alpha - with an optional fade to that alpha
; The alpha will be clamped between 0 and 1
Function SetAlpha(float afTargetAlpha, bool abFade = false) native

; Sets this actor to be attacked by all other actors on sight
Function SetAttackActorOnSight(bool abAttackOnSight = true) native

; Alias for SetActorValue - sets the specified actor value
Function SetAV(string asValueName, float afValue)
  SetActorValue(asValueName, afValue)
EndFunction

; Flags/unflags this actor as bribed by the player
Function SetBribed(bool abBribe = true) native

; Sets the faction this actor reports crimes to
Function SetCrimeFaction(Faction akFaction) native

; Sets this actor's critical stage, which is one of the following (properties below also match this)
; 0 - None
; 1 - Goo start
; 2 - Goo end
; 3 - Disintegrate start
; 4 - Disintegrate end
Function SetCriticalStage(int aiStage) native

; Flag this actor as currently doing a favor for the player
Function SetDoingFavor(bool abDoingFavor = true) native

; Sets this actor as "don't move" or not
Function SetDontMove(bool abDontMove = true) native

; Sets an expression to override any other expression other systems may give this actor.
;							7 - Mood Neutral
; 0 - Dialogue Anger		8 - Mood Anger		15 - Combat Anger
; 1 - Dialogue Fear			9 - Mood Fear		16 - Combat Shout
; 2 - Dialogue Happy		10 - Mood Happy
; 3 - Dialogue Sad			11 - Mood Sad
; 4 - Dialogue Surprise		12 - Mood Surprise
; 5 - Dialogue Puzzled		13 - Mood Puzzled
; 6 - Dialogue Disgusted	14 - Mood Disgusted
; aiStrength is from 0 to 100 (percent)
Function SetExpressionOverride(int aiMood, int aiStrength = 100) native

;forces the eye texture for this actor to the give texture set
Function SetEyeTexture(TextureSet akNewTexture) native

; Sets this actor's rank with the specified faction
Function SetFactionRank(Faction akFaction, int aiRank) native

; Set a specific marker as the place at which this actor must land from flight.
; params: 
; - aMarker:  The ObjectReference to set as this actor's landing marker
Function SetForcedLandingMarker( ObjectReference aMarker ) native

; Flags/unflags this actor as a ghost
Function SetGhost(bool abIsGhost = true) native

; Adds this actor to a faction at rank 0 if they aren't already in it
Function AddToFaction(Faction akFaction)
	if (!IsInFaction(akFaction))
		SetFactionRank(akFaction, 0)
	endif
EndFunction

; Turns on/off headtracking on this actor
Function SetHeadTracking(bool abEnable = true) native

; Flags/unflags this actor as intimidated by the player
Function SetIntimidated(bool abIntimidate = true) native

; Sets this actor's head tracking target, optionally forcing it as their pathing look-at target
Function SetLookAt(ObjectReference akTarget, bool abPathingLookAt = false) native

; Set the no bleedout recovery flag on this actor
Function SetNoBleedoutRecovery(bool abAllowed) native

; Sets this actor to not effect the detection level on the stealth meter if he is not hostile to the player 
Function SetNotShowOnStealthMeter(bool abNotShow) native

; Sets the actors outfit and makes him wear it
Function SetOutfit( Outfit akOutfit, bool abSleepOutfit = false ) native

; Set/reset whether player input being sent to the actor
Function SetPlayerControls(bool abControls) native

; Sets the player as resisting arrest from this actor's faction
Function SetPlayerResistingArrest() native

; Sets or clears this actor as a teammate of the player
; abCanDoFavor - OPTIONAL default is true the teammate can do favors
Function SetPlayerTeammate(bool abTeammate = true, bool abCanDoFavor=true) native

; Sets the actors race
; akRace - OPTIONAL (Def=None) New race for this actor. Default, no race, to switch back to the original race.
Function SetRace( Race akRace = None ) native

; Sets the relationship rank between this actor and another (See GetRelationshipRank for the ranks)
Function SetRelationshipRank(Actor akOther, int aiRank) native

; Sets this actor as restrained or not
Function SetRestrained(bool abRestrained = true) native

; Set a variable on all of an actor's subgraphs
Function SetSubGraphFloatVariable(string asVariableName, float afValue) native

; Sets this actor as unconscious or not
Function SetUnconscious(bool abUnconscious = true) native

; Attach the actor to (or detach it from) a horse, cart, or other vehicle.
; akVehicle is the vehicle ref.  To detach the actor from its current vehicle, set akVehicle to None (or to the Actor itself). 
Function SetVehicle( ObjectReference akVehicle ) native

; Sets the voice recovery timer on the actor
; afTime is recovery time in seconds
Function SetVoiceRecoveryTime( float afTime ) native

; Opens the Barter menu
Function ShowBarterMenu() native

; Opens the Gift menu
; Params:
; - abGivingGift: True if we're giving a gift to this Actor, false if the player is taking a gift from this Actor
; - apFilterList: OPTIONAL (Def=None) -- If present, this form list is used to filter the item list.  Only items
; that match keywords / items in the list will get shown
; - abShowStolenItems: OPTIONAL (Def=false) -- If true, stolen items are shown
; - abUseFavorPoints: OPTIONAL (Def=true) -- If true, favor points are added / subtracted with each transaction.  If false, FPs aren't used at all.
; Returns: The number of favor points spent / gained while in the menu.
int Function ShowGiftMenu( bool abGivingGift, FormList apFilterList = None, bool abShowStolenItems = false, bool abUseFavorPoints = true ) native

; Starts Cannibal with the target 
Function StartCannibal(Actor akTarget) native

; Starts combat with the target
Function StartCombat(Actor akTarget) native

; Start the Deferred Kill state. Be sure to call EndDeferredKill or the actor will be invulnerable.
Function StartDeferredKill() native

; Starts vampire feed with the target
Function StartVampireFeed(Actor akTarget) native

; Removes this actor from combat
Function StopCombat() native

; Stops all combat and alarms against this actor
Function StopCombatAlarm() native

; Returns whether the actor can trap the soul of the given actor.
bool Function TrapSoul(Actor akTarget) native

; Unequips the all items from this actor
Function UnequipAll() native

; Unequips the specified item from this actor
Function UnequipItem(Form akItem, bool abPreventEquip = false, bool abSilent = false) native

; Unequips the all items in this slot for the actor
Function UnequipItemSlot(int aiSlot) native

; Forces this actor to unequip the specified shout
Function UnequipShout(Shout akShout) native

; Forces this actor to unequip the specified spell. The casting source can be:
; 0 - Left hand
; 1 - Right hand
Function UnequipSpell(Spell akSpell, int aiSource) native

; This actor will unlock all the doors that he qualifies for ownership in his current parentcell
Function UnLockOwnedDoorsInCell() native

; Returns whether intimidate will succeed against this actor or not
bool Function WillIntimidateSucceed() native

; Returns whether anything the actor is wearing has the specified keyword
bool Function WornHasKeyword(Keyword akKeyword) native

; Makes this actor start sneaking
Function StartSneaking() native

; Makes this actor draw his weapon
Function DrawWeapon() native

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

; Event that is triggered when this actor begins to die
Event OnDying(Actor akKiller)
EndEvent

; Event received when an actor enters bleedout.
Event OnEnterBleedout()
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

; Event received when this actor finishes changing its race
Event OnRaceSwitchComplete()
EndEvent

; Received when the player fires a bow. akWeapon will be a bow, akAmmo is the ammo or None, 
; afPower will be 1.0 for a full-power shot, less for a dud, and abSunGazing will be true if the player is looking at the sun.
Event OnPlayerBowShot(Weapon akWeapon, Ammo akAmmo, float afPower, bool abSunGazing)
EndEvent


; Received immediately after the player has loaded a save game. A good time to check for additional content.
Event OnPlayerLoadGame()
EndEvent

; Received when the player finishes fast travel, gives the duration of game time the travel took
Event OnPlayerFastTravelEnd(float afTravelGameTimeHours)
EndEvent

; Received when StartVampireFeed is called on an actor
Event OnVampireFeed(Actor akTarget)
EndEvent

; Received when the vampirism state of this actor changes (when SendVampirismStateChanged is called)
Event OnVampirismStateChanged(bool abIsVampire)
EndEvent

; Set of read-only properties to essentually make a fake enum for critical stages
int Property CritStage_None = 0 AutoReadOnly
int Property CritStage_GooStart = 1 AutoReadOnly
int Property CritStage_GooEnd = 2 AutoReadOnly
int Property CritStage_DisintegrateStart = 3 AutoReadOnly
int Property CritStage_DisintegrateEnd = 4 AutoReadOnly

; **** For Debugging Movement Animations (not in release builds) ****
; Forces the movement direction on the actor
; afXAngle, afYAngle and afZAngle are in degrees
Function ForceMovementDirection(float afXAngle = 0.0, float afYAngle = 0.0, float afZAngle = 0.0) native

; Forces the movement speed on the actor
; afSpeedMult is a speed multiplier based on the current max speeds
; - 0 -> 1 Scales between 0 and the Walk speed
; - 1 -> 2 Scales between Walk speed and Run Speed
; - 2 and above is a multiplier of the run speed (less 1.0 since Run is 2.0)
Function ForceMovementSpeed(float afSpeedMult) native

; Forces the movement rotation speed on the actor
; Each component of the rotation speed is a multiplier following these rules:
; - 0 -> 1 Scales between 0 and the Walk speed
; - 1 -> 2 Scales between Walk speed and Run Speed
; - 2 and above is a multiplier of the run speed (less 1.0 since Run is 2.0)
Function ForceMovementRotationSpeed(float afXMult = 0.0, float afYMult = 0.0, float afZMult = 0.0) native

; Ramps the movement direction on the actor to the passed in value over the passed in time
; afXAngle, afYAngle and afZAngle are in degrees
; afRampTime is in seconds
Function ForceMovementDirectionRamp(float afXAngle = 0.0, float afYAngle = 0.0, float afZAngle = 0.0, float afRampTime = 0.1) native

; Ramps the movement speed on the actor to the passed in value over the passed in time
; afSpeedMult is a speed multiplier based on the current max speeds
; - 0 -> 1 Scales between 0 and the Walk speed
; - 1 -> 2 Scales between Walk speed and Run Speed
; - 2 and above is a multiplier of the run speed (less 1.0 since Run is 2.0)
; afRampTime is in seconds
Function ForceMovementSpeedRamp(float afSpeedMult, float afRampTime = 0.1) native

; Ramps the movement rotation speed on the actor to the passed in value over the passed in time
; Each component of the rotation speed is a multiplier following these rules:
; - 0 -> 1 Scales between 0 and the Walk speed
; - 1 -> 2 Scales between Walk speed and Run Speed
; - 2 and above is a multiplier of the run speed (less 1.0 since Run is 2.0)
; afRampTime is in seconds
Function ForceMovementRotationSpeedRamp(float afXMult = 0.0, float afYMult = 0.0, float afZMult = 0.0, float afRampTime = 0.1) native

; Sets the target movement direction on the actor
; afXAngle, afYAngle and afZAngle are in degrees
Function ForceTargetDirection(float afXAngle = 0.0, float afYAngle = 0.0, float afZAngle = 0.0) native

; Sets the target movement speed on the actor
; afSpeedMult is a speed multiplier based on the current max speeds
; - 0 -> 1 Scales between 0 and the Walk speed
; - 1 -> 2 Scales between Walk speed and Run Speed
; - 2 and above is a multiplier of the run speed (less 1.0 since Run is 2.0)
Function ForceTargetSpeed(float afSpeed) native

; Sets the target facing angle on the actor
; afXAngle, afYAngle and afZAngle are in degrees
Function ForceTargetAngle(float afXAngle = 0.0, float afYAngle = 0.0, float afZAngle = 0.0) native

; Clears any forced movement on the actor and return it to its standard state
Function ClearForcedMovement() native
