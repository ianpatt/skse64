Scriptname Game Hidden

; Adds the specified achievement to the player's profile
Function AddAchievement(int aiAchievementID) native global

; Add the specified number of perk points to the player
Function AddPerkPoints(int aiPerkPoints) native global

; Advance the given skill on the player by the provided amount of skill usage
Function AdvanceSkill(string asSkillName, float afMagnitude) native global

; Adds a ball-and-socket constraint between two rigid bodies, identified by their ref and node names
bool Function AddHavokBallAndSocketConstraint( ObjectReference arRefA, string arRefANode, ObjectReference arRefB, string arRefBNode, float afRefALocalOffsetX = 0.0, float afRefALocalOffsetY = 0.0, float afRefALocalOffsetZ = 0.0, float afRefBLocalOffsetX = 0.0, float afRefBLocalOffsetY = 0.0, float afRefBLocalOffsetZ = 0.0) native global

; Removes any constraint between two rigid bodies
bool Function RemoveHavokConstraints(ObjectReference arFirstRef, string arFirstRefNodeName, ObjectReference arSecondRef, string arSecondRefNodeName) native global

; Calculates how much a x point favor would cost the player
int Function CalculateFavorCost(int aiFavorPrice) native global

; Clears the prison variables on the player 
Function ClearPrison() native global

; Clears temp effects from game
Function ClearTempEffects() native global

; Disables the user's controls
Function DisablePlayerControls(bool abMovement = true, bool abFighting = true, bool abCamSwitch = false, bool abLooking = false, \
  bool abSneaking = false, bool abMenu = true, bool abActivate = true, bool abJournalTabs = false, int aiDisablePOVType = 0) native global
  
; Enables the user's controls
Function EnablePlayerControls(bool abMovement = true, bool abFighting = true, bool abCamSwitch = true, bool abLooking = true, \
  bool abSneaking = true, bool abMenu = true, bool abActivate = true, bool abJournalTabs = true, int aiDisablePOVType = 0) native global
  
; Enables or disables the ability to fast travel
Function EnableFastTravel(bool abEnable = true) native global

; Fades out the game to black, or vice versa
Function FadeOutGame(bool abFadingOut, bool abBlackFade, float afSecsBeforeFade, float afFadeDuration) native global
  
; Fast-travels the player to the specified object's location
Function FastTravel(ObjectReference akDestination) native global

; Finds the closest reference of a given base object within a given radius of a location
ObjectReference Function FindClosestReferenceOfType(Form arBaseObject, float afX, float afY, float afZ, float afRadius) native global

; Finds a random reference of a given base object within a given radius of a location
ObjectReference Function FindRandomReferenceOfType(Form arBaseObject, float afX, float afY, float afZ, float afRadius) native global

; Finds the closest reference of any base object in the list within a given radius of a location
ObjectReference Function FindClosestReferenceOfAnyTypeInList(FormList arBaseObjects, float afX, float afY, float afZ, float afRadius) native global

; Finds a random reference of a any base object in the list within a given radius of a location
ObjectReference Function FindRandomReferenceOfAnyTypeInList(FormList arBaseObjects, float afX, float afY, float afZ, float afRadius) native global

; Finds the closest reference of a given base object within a given radius of a reference
ObjectReference Function FindClosestReferenceOfTypeFromRef(Form arBaseObject, ObjectReference arCenter, float afRadius) global
	return FindClosestReferenceOfType(arBaseObject, arCenter.X, arCenter.Y, arCenter.Z, afRadius)
endFunction

; Finds a random reference of a given base object within a given radius of a reference
ObjectReference Function FindRandomReferenceOfTypeFromRef(Form arBaseObject, ObjectReference arCenter, float afRadius) global
	return FindRandomReferenceOfType(arBaseObject, arCenter.X, arCenter.Y, arCenter.Z, afRadius)
endFunction

; Finds the closest reference of a given base object within a given radius of a reference
ObjectReference Function FindClosestReferenceOfAnyTypeInListFromRef(FormList arBaseObjects, ObjectReference arCenter, float afRadius) global
	return FindClosestReferenceOfAnyTypeInList(arBaseObjects, arCenter.X, arCenter.Y, arCenter.Z, afRadius)
endFunction

; Finds a random reference of a given base object within a given radius of a reference
ObjectReference Function FindRandomReferenceOfAnyTypeInListFromRef(FormList arBaseObjects, ObjectReference arCenter, float afRadius) global
	return FindRandomReferenceOfAnyTypeInList(arBaseObjects, arCenter.X, arCenter.Y, arCenter.Z, afRadius)
endFunction

; Finds the closest actor within a given radius of a location
Actor Function FindClosestActor(float afX, float afY, float afZ, float afRadius) native global

; Finds a random actor within a given radius of a location
Actor Function FindRandomActor(float afX, float afY, float afZ, float afRadius) native global

; Finds the closest actor within a given radius of a reference
Actor Function FindClosestActorFromRef(ObjectReference arCenter, float afRadius) global
	return FindClosestActor(arCenter.X, arCenter.Y, arCenter.Z, afRadius)
endFunction

; Finds a random actor within a given radius of a reference
Actor Function FindRandomActorFromRef(ObjectReference arCenter, float afRadius) global
	return FindRandomActor(arCenter.X, arCenter.Y, arCenter.Z, afRadius)
endFunction

; Make the player got to 3rd person camera mode
Function ForceThirdPerson() native global

; Make the player got to 1st person camera mode
Function ForceFirstPerson() native global

; Show the players first person geometry.
Function ShowFirstPersonGeometry( bool abShow = true ) native global

; Returns the form specified by the ID
Form Function GetForm(int aiFormID) native global

; Returns the form specified by the ID originating in the given file
Form Function GetFormFromFile(int aiFormID, string asFilename) native global

; Obtains the value of a game setting - one for each type of game setting
float Function GetGameSettingFloat(string asGameSetting) native global
int Function GetGameSettingInt(string asGameSetting) native global
string Function GetGameSettingString(string asGameSetting) native global

; Returns the player actor
Actor Function GetPlayer() native global

; Returns the reference the player is currently grabbing
ObjectReference Function GetPlayerGrabbedRef() native global

; Returns the horse last ridden by the player
Actor Function GetPlayersLastRiddenHorse() native global

; Returns the X position of the Sun.
float Function GetSunPositionX() native global

; Returns the Y position of the Sun.
float Function GetSunPositionY() native global

; Returns the Z position of the Sun.
float Function GetSunPositionZ() native global

; Returns the number of days spent in play
float Function GetRealHoursPassed() native global

; Increment the given skill on the player by the one point
Function IncrementSkill(string asSkillName) native global

; Increment the given skill on the player by the given number of points
Function IncrementSkillBy(string asSkillName, int aiCount) native global

; Modifies the specified MiscStat by the given amount.
Function IncrementStat(string asStatName, int aiModAmount = 1) native global

; Are the activation controls enabled?
bool Function IsActivateControlsEnabled() native global

; Are the camera switch controls enabled?
bool Function IsCamSwitchControlsEnabled() native global

; Is fast travel controls enabled? Returns false if EnableFastTravel(false) has been called
bool Function IsFastTravelControlsEnabled() native global

; Is fast travel enabled?
bool Function IsFastTravelEnabled() native global

; Are the fighting controls enabled?
bool Function IsFightingControlsEnabled() native global

; Are the journal menu controls enabled?
bool Function IsJournalControlsEnabled() native global

; Are the looking controls enabled?
bool Function IsLookingControlsEnabled() native global

; Are the menu controls enabled?
bool Function IsMenuControlsEnabled() native global

; Are the movement controls enabled?
bool Function IsMovementControlsEnabled() native global

; Is the player looking at the sun?
bool Function IsPlayerSungazing() native global

; Are the sneaking controls enabled?
bool Function IsSneakingControlsEnabled() native global

; Is the specified Word of Power Unlocked?
bool Function IsWordUnlocked(WordOfPower akWord) native global

; Plays a bink video - does not return until bink has finished, use with care!
Function PlayBink(string asFileName, bool abInterruptible = false, bool abMuteAudio = true, bool abMuteMusic = true, \
  bool abLetterbox = true ) native global

; Precaches character gen data.
Function PrecacheCharGen() native global

; Clears Precached character gen data.
Function PrecacheCharGenClear() native global

; Queries the given stat and returns its value
int Function QueryStat(string asStat) native global

; Forces the game back to the main menu
Function QuitToMainMenu() native global

; Request that an auto-save be made
Function RequestAutoSave() native global

; Requests the specified model
Function RequestModel(string asModelName) native global

; Request that a normal save be made
Function RequestSave() native global

; Has the player serve their prison time
Function ServeTime() native global

; Finds an actor in high who can detect the player to call werewolf crime on the player
Function SendWereWolfTransformation() native global

; Called as we enter/exit beast form
Function SetBeastForm(bool abEntering) native global

; Sets the camera target actor
Function SetCameraTarget(Actor arTarget) native global

; Sets or clears "cart mode" for the HUD
Function SetHudCartMode(bool abSetCartMode = true) native global

; Informs the game whether we are in CharGen or not
Function SetInChargen(bool abDisableSaving, bool abDisableWaiting, bool abShowControlsDisabledMessage) native global

; Enables or disables the AI driven flag on Player
Function SetPlayerAIDriven(bool abAIDriven = true) native global

; Enables or disables  crime reporting on Player
Function SetPlayerReportCrime(bool abReportCrime = true) native global

; Set the players sitting camera rotation - in degrees, offset from the standard angle.
Function SetSittingRotation(float afValue) native global

; Shakes the object from the location of the passed-in object. If none, it will shake the camera from the player's location.
; Strength is clamped from 0 to 1
; Duration in seconds. By default (0.0) use the game setting.
Function ShakeCamera(ObjectReference akSource = None, float afStrength = 0.5, float afDuration = 0.0) native global

; Shakes the controller for the specified length of time (in seconds). The strength values are clamped from 0 to 1
Function ShakeController(float afSmallMotorStrength, float afBigMotorStreangth, float afDuration) native global

; Displays the race/sex menu
Function ShowRaceMenu() native global
Function ShowLimitedRaceMenu() native global

; Title Sequence menu functions
Function ShowTitleSequenceMenu() native global
Function HideTitleSequenceMenu() native global
Function StartTitleSequence(string asSequenceName) native global

; Allow or disallow player requests to have a flying mount land.
Function SetAllowFlyingMountLandingRequests(bool abAllow) native global

; Sets the Image Space Modifier that is triggered when the player gazes at the sun.
Function SetSunGazeImageSpaceModifier(ImageSpaceModifier apImod = NONE ) native global

; Displays the training menu based on passed in trainer actor
Function ShowTrainingMenu(Actor aTrainer) native global

; Teaches the specified word of power to the player
Function TeachWord(WordOfPower akWord) native global

; Trigger screen blood with the given count
Function TriggerScreenBlood(int aiValue) native global

; Unlocks the specified word of power so the player can use it
Function UnlockWord(WordOfPower akWord) native global

; Returns true if we're using a gamepad
bool Function UsingGamepad() native global
