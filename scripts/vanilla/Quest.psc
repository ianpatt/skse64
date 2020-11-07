Scriptname Quest extends Form Hidden

; non-native functions

; thread-safe way to modify a global value
; optional parameters:
; aiObjectiveID = objective ID to redisplay
; afTargetValue = value you're counting up (or down) towards -- if included, function will return TRUE when the global reaches the target value
; abCountingUp = by default, function assumes you're counting up towards the target value; make this false to count DOWN towards target value
; abCompleteObjective = by default, function assumes you're completing the objective once you reach the target value; make this false to FAIL the objective
; abRedisplayObjective = by default, function asssume you want to redisplay the objective every time the global is incremeneted; make this FALSE to only display the objectives on complete or failure
bool Function ModObjectiveGlobal(float afModValue, GlobalVariable aModGlobal, int aiObjectiveID = -1, float afTargetValue = -1.0, bool abCountingUp = true, bool abCompleteObjective = true, bool abRedisplayObjective = true)
	aModGlobal.Mod(afModValue)
	UpdateCurrentInstanceGlobal(aModGlobal)
	if aiObjectiveID >= 0
		; display/complete objectives automatically
		if afTargetValue > -1
			if (abCountingUp && aModGlobal.value >= afTargetValue) || (!abCountingUp && aModGlobal.value <= afTargetValue)
				if (abCompleteObjective)
					; complete objective
					SetObjectiveCompleted(aiObjectiveID)
					return true
				Else
					; fail objective
					SetObjectiveFailed(aiObjectiveID)
					return true
				Endif
			elseIf (abRedisplayObjective)
				; redisplay objective
				SetObjectiveDisplayed(aiObjectiveID, true, true)
			Else
				SetObjectiveDisplayed(aiObjectiveID, true, false)
			endif
		elseIf (abRedisplayObjective)
			; no target value, always redisplay objective
			SetObjectiveDisplayed(aiObjectiveID, true, true)
		Else
			SetObjectiveDisplayed(aiObjectiveID, true, false)
		endif
	endif
	return false
endFunction


; native functions

; Flags all objectives as complete
Function CompleteAllObjectives() native

; Flags this quest as completed
Function CompleteQuest() native

; Flags all objectives as failed
Function FailAllObjectives() native

; Obtains the specified alias on the quest
Alias Function GetAlias(int aiAliasID) native

; Obtains the id of the highest completed stage on this quest
int Function GetCurrentStageID() native

; Alias for GetCurrentStage - obtains the highest completed stage on this quest
int Function GetStage()
  return GetCurrentStageID()
EndFunction

; Alias for IsStageDone - checks to see whether the given stage is done or not
bool Function GetStageDone(int aiStage)
  return IsStageDone(aiStage)
EndFunction

; Is this quest "active" (tracked by the player)?
bool Function IsActive() native

; Checks to see if the quest is completed
bool Function IsCompleted() native

; Checks to see if the specified objective is completed
bool Function IsObjectiveCompleted(int aiObjective) native

; Checks to see if the specified objective is displayed
bool Function IsObjectiveDisplayed(int aiObjective) native

; Checks to see if the specified objective is failed
bool Function IsObjectiveFailed(int aiObjective) native

; Checks to see if the quest is running
bool Function IsRunning() native

; Obtains whether the specified stage is done or not
bool Function IsStageDone(int aiStage) native

; Checks to see if the quest is enabled but not running yet
bool Function IsStarting() native

; Checks to see if the quest is not enabled anymore but still shutting down
bool Function IsStopping() native

; Checks to see if the quest is no longer enabled or running
bool Function IsStopped() native

; Resets the quest
Function Reset() native

; Flags this quest as "active" (tracked by the player)
Function SetActive(bool abActive = true) native

; Set the quest to the requested stage ID - returns true if stage exists and was set.
; This function is latent and will wait for the quest to start up before returning (if it needed to be started)
bool Function SetCurrentStageID(int aiStageID) native

; Sets the specified objective to completed or not
Function SetObjectiveCompleted(int aiObjective, bool abCompleted = true) native

; Sets the specified objective to displayed or hidden - if abForce is true, will display the objective even if it has already been displayed
Function SetObjectiveDisplayed(int aiObjective, bool abDisplayed = true, bool abForce = false) native

; Sets the specified objective to failed or not
Function SetObjectiveFailed(int aiObjective, bool abFailed = true) native

; Alias of SetCurrentStage - Set the quest to the requested stage
; This function is latent and will wait for the quest to start up before returning (if it needed to be started)
bool Function SetStage(int aiStage)
  return SetCurrentStageID(aiStage)
EndFunction

; Starts the quest - returns whether the quest was able to be started or not
; This function is latent and will wait for the quest to start up before returning
bool Function Start() native

; Stops the quest
Function Stop() native

; Updates current instance's value for the given global
bool Function UpdateCurrentInstanceGlobal( GlobalVariable aUpdateGlobal ) native

; Story manager events - fired in parallel with the quest startup stage

Event OnStoryAddToPlayer(ObjectReference akOwner, ObjectReference akContainer, \
	Location akLocation, Form akItemBase, int aiAcquireType)
EndEvent

Event OnStoryArrest(ObjectReference akArrestingGuard, ObjectReference akCriminal, \
	Location akLocation, int aiCrime)
EndEvent

Event OnStoryAssaultActor(ObjectReference akVictim, ObjectReference akAttacker, \
	Location akLocation, int aiCrime)
EndEvent

Event OnStoryBribeNPC(ObjectReference akActor)
EndEvent

Event OnStoryCastMagic(ObjectReference akCastingActor, ObjectReference akSpellTarget, \
	Location akLocation, Form akSpell)
EndEvent

Event OnStoryChangeLocation(ObjectReference akActor, Location akOldLocation, \
	Location akNewLocation)
EndEvent

Event OnStoryCrimeGold(ObjectReference akVictim, ObjectReference akCriminal, \
	Form akFaction, int aiGoldAmount, int aiCrime)
EndEvent

Event OnStoryCure(Form akInfection)
EndEvent

Event OnStoryDialogue(Location akLocation, ObjectReference akActor1, ObjectReference akActor2)
EndEvent

Event OnStoryDiscoverDeadBody(ObjectReference akActor, ObjectReference akDeadActor, \
	Location akLocation)
EndEvent

Event OnStoryEscapeJail(Location akLocation, Form akCrimeGroup)
EndEvent

Event OnStoryActivateActor(Location akLocation, ObjectReference akActor)
EndEvent

Event OnStoryFlatterNPC(ObjectReference akActor)
EndEvent

Event OnStoryHello(Location akLocation, ObjectReference akActor1, ObjectReference akActor2)
EndEvent

Event OnStoryIncreaseLevel(int aiNewLevel)
EndEvent

Event OnStoryIncreaseSkill(string asSkill)
EndEvent

Event OnStoryInfection(ObjectReference akTransmittingActor, Form akInfection)
EndEvent

Event OnStoryIntimidateNPC(ObjectReference akActor)
EndEvent

Event OnStoryJail(ObjectReference akGuard, Form akCrimeGroup, Location akLocation, \
	int aiCrimeGold)
EndEvent

Event OnStoryKillActor(ObjectReference akVictim, ObjectReference akKiller, \
	Location akLocation, int aiCrimeStatus, int aiRelationshipRank)
EndEvent

Event OnStoryCraftItem(ObjectReference akBench, Location akLocation, Form akCreatedItem)
EndEvent

Event OnStoryNewVoicePower(ObjectReference akActor, Form akVoicePower)
EndEvent

Event OnStoryPickLock(ObjectReference akActor, ObjectReference akLock)
EndEvent

Event OnStoryPayFine(ObjectReference akCriminal, ObjectReference akGuard, \
	Form akCrimeGroup, int aiCrimeGold)
EndEvent

Event OnStoryPlayerGetsFavor(ObjectReference akActor)
EndEvent

Event OnStoryRelationshipChange(ObjectReference akActor1, ObjectReference akActor2, \
	int aiOldRelationship, int aiNewRelationship)
EndEvent

Event OnStoryRemoveFromPlayer(ObjectReference akOwner, ObjectReference akItem, \
	Location akLocation, Form akItemBase, int aiRemoveType)
EndEvent

Event OnStoryScript(Keyword akKeyword, Location akLocation, ObjectReference akRef1, \
	ObjectReference akRef2, int aiValue1, int aiValue2)
EndEvent

Event OnStoryServedTime(Location akLocation, Form akCrimeGroup, int aiCrimeGold, \
	int aiDaysJail)
EndEvent

Event OnStoryTrespass(ObjectReference akVictim, ObjectReference akTrespasser, \
	Location akLocation, int aiCrime)
EndEvent