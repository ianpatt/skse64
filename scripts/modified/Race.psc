; returns the number of spells for the race
int Function GetSpellCount() native

; returns the specified spell from the race
Spell Function GetNthSpell(int n) native

; returns whether the specified race flag is set
bool Function IsRaceFlagSet(int n) native

; sets the specified race flag
Function SetRaceFlag(int n) native

; clears the specified race flag
Function ClearRaceFlag(int n) native

; Returns the races default voice type
VoiceType Function GetDefaultVoiceType(bool female) native

; Sets the races default voice type
Function SetDefaultVoiceType(bool female, VoiceType voice) native

; Gets/sets the skin of the race
Armor Function GetSkin() native
Function SetSkin(Armor skin) native

; Returns the number of playable races
int Function GetNumPlayableRaces() native global

; Returns the nth playable race
Race Function GetNthPlayableRace(int n) native global

; Returns a race by it's editorId name
Race Function GetRace(string editorId) native global

; race flags for previous functions
int property kRace_Playable						= 0x00000001 AutoReadOnly
int property kRace_FaceGenHead					= 0x00000002 AutoReadOnly
int property kRace_Child						= 0x00000004 AutoReadOnly
int property kRace_TiltFrontBack				= 0x00000008 AutoReadOnly
int property kRace_TiltLeftRight				= 0x00000010 AutoReadOnly
int property kRace_NoShadow						= 0x00000020 AutoReadOnly
int property kRace_Swims						= 0x00000040 AutoReadOnly
int property kRace_Flies						= 0x00000080 AutoReadOnly
int property kRace_Walks						= 0x00000100 AutoReadOnly
int property kRace_Immobile						= 0x00000200 AutoReadOnly
int property kRace_NotPushable					= 0x00000400 AutoReadOnly
int property kRace_NoCombatInWater				= 0x00000800 AutoReadOnly
int property kRace_NoRotatingToHeadTrack		= 0x00001000 AutoReadOnly
int property kRace_UseHeadTrackAnim				= 0x00008000 AutoReadOnly
int property kRace_SpellsAlignWithMagicNode		= 0x00010000 AutoReadOnly
int property kRace_UseWorldRaycasts				= 0x00020000 AutoReadOnly
int property kRace_AllowRagdollCollision		= 0x00040000 AutoReadOnly
int property kRace_CantOpenDoors				= 0x00100000 AutoReadOnly
int property kRace_AllowPCDialogue				= 0x00200000 AutoReadOnly
int property kRace_NoKnockdowns					= 0x00400000 AutoReadOnly
int property kRace_AllowPickpocket				= 0x00800000 AutoReadOnly
int property kRace_AlwaysUseProxyController		= 0x01000000 AutoReadOnly
int property kRace_AllowMultipleMembraneShaders	= 0x20000000 AutoReadOnly
int property kRace_AvoidsRoads					= 0x80000000 AutoReadOnly

bool Function IsPlayable()
	return IsRaceFlagSet(self.kRace_Playable)
endFunction

Function MakePlayable()
	SetRaceFlag(self.kRace_Playable)
endFunction

Function MakeUnplayable()
	ClearRaceFlag(self.kRace_Playable)
endFunction

bool Function IsChildRace()
	return IsRaceFlagSet(self.kRace_Child)
endFunction

Function MakeChildRace()
	SetRaceFlag(self.kRace_Child)
endFunction

Function MakeNonChildRace()
	ClearRaceFlag(self.kRace_Child)
endFunction

bool Function CanFly()
	return IsRaceFlagSet(self.kRace_Flies)
endFunction

Function MakeCanFly()
	SetRaceFlag(self.kRace_Flies)
endFunction

Function MakeNonFlying()
	ClearRaceFlag(self.kRace_Flies)
endFunction

bool Function CanSwim()
	return IsRaceFlagSet(self.kRace_Swims)
endFunction

Function MakeCanSwim()
	SetRaceFlag(self.kRace_Swims)
endFunction

Function MakeNonSwimming()
	ClearRaceFlag(self.kRace_Swims)
endFunction

bool Function CanWalk()
	return IsRaceFlagSet(self.kRace_Walks)
endFunction

Function MakeCanWalk()
	SetRaceFlag(self.kRace_Walks)
endFunction

Function MakeNonWalking()
	ClearRaceFlag(self.kRace_Walks)
endFunction

bool Function IsImmobile()
	return IsRaceFlagSet(self.kRace_Immobile)
endFunction

Function MakeImmobile()
	SetRaceFlag(self.kRace_Immobile)
endFunction

Function MakeMobile()
	ClearRaceFlag(self.kRace_Immobile)
endFunction

bool Function IsNotPushable()
	return IsRaceFlagSet(self.kRace_NotPushable)
endFunction

Function MakeNotPushable()
	SetRaceFlag(self.kRace_NotPushable)
endFunction

Function MakePushable()
	ClearRaceFlag(self.kRace_NotPushable)
endFunction

bool Function NoKnockdowns()
	return IsRaceFlagSet(self.kRace_AllowPickpocket)
endFunction

Function MakeNoKnockdowns()
	SetRaceFlag(self.kRace_AllowPickpocket)
endFunction

Function ClearNoKNockdowns()
	ClearRaceFlag(self.kRace_AllowPickpocket)
endFunction

bool Function NoCombatInWater()
	return IsRaceFlagSet(self.kRace_NoCombatInWater)
endFunction

Function SetNoCombatInWater()
	SetRaceFlag(self.kRace_NoCombatInWater)
endFunction

Function ClearNoCombatInWater()
	ClearRaceFlag(self.kRace_NoCombatInWater)
endFunction

bool Function AvoidsRoads()
	return IsRaceFlagSet(self.kRace_AvoidsRoads)
endFunction

Function SetAvoidsRoads()
	SetRaceFlag(self.kRace_AvoidsRoads)
endFunction

Function ClearAvoidsRoads()
	ClearRaceFlag(self.kRace_AvoidsRoads)
endFunction

bool Function AllowPickpocket()
	return IsRaceFlagSet(self.kRace_AllowPickpocket)
endFunction

Function SetAllowPickpocket()
	SetRaceFlag(self.kRace_AllowPickpocket)
endFunction

Function ClearAllowPickpocket()
	ClearRaceFlag(self.kRace_AllowPickpocket)
endFunction

bool Function AllowPCDialogue()
	return IsRaceFlagSet(self.kRace_AllowPCDialogue)
endFunction

Function SetAllowPCDialogue()
	SetRaceFlag(self.kRace_AllowPCDialogue)
endFunction

Function ClearAllowPCDialogue()
	ClearRaceFlag(self.kRace_AllowPCDialogue)
endFunction

bool Function CantOpenDoors()
	return IsRaceFlagSet(self.kRace_CantOpenDoors)
endFunction

Function SetCantOpenDoors()
	SetRaceFlag(self.kRace_CantOpenDoors)
endFunction

Function ClearCantOpenDoors()
	ClearRaceFlag(self.kRace_CantOpenDoors)
endFunction

bool Function NoShadow()
	return IsRaceFlagSet(self.kRace_NoShadow)
endFunction

Function SetNoShadow()
	SetRaceFlag(self.kRace_NoShadow)
endFunction

Function ClearNoShadow()
	ClearRaceFlag(self.kRace_NoShadow)
endFunction

