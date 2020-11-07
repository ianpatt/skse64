Scriptname ActorValueInfo extends Form Hidden

; Returns the AVI by name
ActorValueInfo Function GetActorValueInfoByName(string avName) global native
ActorValueInfo Function GetAVIByName(string avName) global
	return GetActorValueInfoByName(avName)
EndFunction

; Returns the AVI by id (0-164)
ActorValueInfo Function GetActorValueInfoByID(int id) global native
ActorValueInfo Function GetAVIByID(int id) global
	return GetActorValueInfoByID(id)
EndFunction

; Returns whether this AVI is a skill
bool Function IsSkill() native

; Skill Multiplier manipulation
float Function GetSkillUseMult() native
Function SetSkillUseMult(float value) native

float Function GetSkillOffsetMult() native
Function SetSkillOffsetMult(float value) native

float Function GetSkillImproveMult() native
Function SetSkillImproveMult(float value) native

float Function GetSkillImproveOffset() native
Function SetSkillImproveOffset(float value) native

; Returns the amount of experienced gained in this skill
float Function GetSkillExperience() native

; Does not trigger skill-up
Function SetSkillExperience(float exp) native

; Adds experience to this skill (Same as console AdvanceSkill, triggers skill-up)
Function AddSkillExperience(float exp) native

; Returns the experience required for skill-up
; (ImproveMult * currentLevel ^ fSkillUseCurve + ImproveOffset)
float Function GetExperienceForLevel(int currentLevel) native

; Returns the legendary level of this skill
int Function GetSkillLegendaryLevel() native

; Sets the legendary level of this skill
Function SetSkillLegendaryLevel(int level) native

; Returns perks from the skill into the FormList
; Actor filter applies to unowned and allRanks
; unowned will add perks that the actor does not own, or only perks the actor owns
; allRanks will add all ranks of each perk to the list, unowned/owned filter also applies
Function GetPerkTree(FormList list, Actor akActor = None, bool unowned = true, bool allRanks = false) native

; Same as GetPerkTree except returns into a new array
Perk[] Function GetPerks(Actor akActor = None, bool unowned = true, bool allRanks = false) native

; Same as Actor.GetActorValue (convenience function)
float Function GetCurrentValue(Actor akActor) native

; Same as Actor.GetBaseActorValue (convenience function)
float Function GetBaseValue(Actor akActor) native

; Acquires the Maximum value for the current ActorValue
float Function GetMaximumValue(Actor akActor) native