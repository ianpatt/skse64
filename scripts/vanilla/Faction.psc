Scriptname Faction extends Form Hidden

; Checks to see if the player can pay the crime gold for this faction
bool Function CanPayCrimeGold() native

; Gets the amount of gold the player is to pay to this faction for crimes
int Function GetCrimeGold() native

; Gets the amount of gold the player is to pay to this faction for non-violent crimes
int Function GetCrimeGoldNonViolent() native

; Gets the amount of gold the player is to pay to this faction for violent crimes
int Function GetCrimeGoldViolent() native

; Get the player's "infamy" with this faction (accumulated crime gold)
int Function GetInfamy() native

; Get the player's "non-violent infamy" with this faction (accumulated non-violent crime gold)
int Function GetInfamyNonViolent() native

; Get the player's "violent infamy" with this faction (accumulated violent crime gold)
int Function GetInfamyViolent() native

; Gets this faction's reaction towards the other
int Function GetReaction(Faction akOther) native

; Obtains the value of all items stolen by the player from this faction that was witnessed
int Function GetStolenItemValueCrime() native

; Obtains the value of all items stolen by the player from this faction that was NOT witnessed
int Function GetStolenItemValueNoCrime() native

; Is the passed in faction in this faction's crime group
bool Function IsFactionInCrimeGroup(Faction akOther) native

; Is the player expelled from this faction?
bool Function IsPlayerExpelled() native

; Modifies the amount of crime gold for this faction - violent or non-violent
Function ModCrimeGold(int aiAmount, bool abViolent = false) native

; Modifies this faction's reaction towards the other faction
Function ModReaction(Faction akOther, int aiAmount) native

; Has the player pay the crime gold for this faction
Function PlayerPayCrimeGold(bool abRemoveStolenItems = true, bool abGoToJail = true) native

; Finds a nearby NPC in this faction and has them behave as if assaulted
Function SendAssaultAlarm() native

; Sends the player to this faction's jail - removing inventory if requested, and to a "real" jail or not
Function SendPlayerToJail(bool abRemoveInventory = true, bool abRealJail = true) native

; Sets this faction and the other as allies or friends - if the friend booleans are true - the specified one-way relationship
; is a friend instead of an ally
Function SetAlly(Faction akOther, bool abSelfIsFriendToOther = false, bool abOtherIsFriendToSelf = false) native

; Sets the non-violent crime gold on this faction
Function SetCrimeGold(int aiGold) native

; Sets the violent crime gold on this faction
Function SetCrimeGoldViolent(int aiGold) native

; Sets this faction and the other as enemies or neutral - if the friend booleans are true - the specified one-way relationship
; is a neutral instead of an enemy
Function SetEnemy(Faction akOther, bool abSelfIsNeutralToOther = false, bool abOtherIsNeutralToSelf = false) native

; Sets or clears the player as an enemy of this faction
Function SetPlayerEnemy(bool abIsEnemy = true) native

; Sets or clears the expelled flag for this faction on the player
Function SetPlayerExpelled(bool abIsExpelled = true) native

; Sets this faction's reaction towards the other
Function SetReaction(Faction akOther, int aiNewValue) native