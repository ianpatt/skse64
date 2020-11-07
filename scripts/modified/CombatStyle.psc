Scriptname CombatStyle extends Form Hidden

; functions related to the General Tab values
float Function GetOffensiveMult() native
float Function GetDefensiveMult() native
float Function GetGroupOffensiveMult() native
float Function GetAvoidThreatChance() native
float Function GetMeleeMult() native
float Function GetRangedMult() native
float Function GetMagicMult() native
float Function GetShoutMult() native
float Function GetStaffMult() native
float Function GetUnarmedMult() native

Function SetOffensiveMult(float mult) native
Function SetDefensiveMult(float mult) native
Function SetGroupOffensiveMult(float mult) native
Function SetAvoidThreatChance(float chance) native
Function SetMeleeMult(float mult) native
Function SetRangedMult(float mult) native
Function SetMagicMult(float mult) native
Function SetShoutMult(float mult) native
Function SetStaffMult(float mult) native
Function SetUnarmedMult(float mult) native

; functions related to the Melee tab values
float Function GetMeleeAttackStaggeredMult() native
float Function GetMeleePowerAttackStaggeredMult() native
float Function GetMeleePowerAttackBlockingMult() native
float Function GetMeleeBashMult() native
float Function GetMeleeBashRecoiledMult() native
float Function GetMeleeBashAttackMult() native
float Function GetMeleeBashPowerAttackMult() native
float Function GetMeleeSpecialAttackMult() native
bool Function GetAllowDualWielding() native

Function SetMeleeAttackStaggeredMult(float mult) native
Function SetMeleePowerAttackStaggeredMult(float mult) native
Function SetMeleePowerAttackBlockingMult(float mult) native
Function SetMeleeBashMult(float mult) native
Function SetMeleeBashRecoiledMult(float mult) native
Function SetMeleeBashAttackMult(float mult) native
Function SetMeleeBashPowerAttackMult(float mult) native
Function SetMeleeSpecialAttackMult(float mult) native
Function SetAllowDualWielding(bool allow) native

; functions related to the Close Range tab values
float Function GetCloseRangeDuelingCircleMult() native
float Function GetCloseRangeDuelingFallbackMult() native
float Function GetCloseRangeFlankingFlankDistance() native
float Function GetCloseRangeFlankingStalkTime() native

Function SetCloseRangeDuelingCircleMult(float mult) native
Function SetCloseRangeDuelingFallbackMult(float mult) native
Function SetCloseRangeFlankingFlankDistance(float mult) native
Function SetCloseRangeFlankingStalkTime(float mult) native

; functions related to the LongRange tab values
float Function GetLongRangeStrafeMult() native
Function SetLongRangeStrafeMult(float mult) native

; functions related to the Flight tab values
float Function GetFlightHoverChance() native
float Function GetFlightDiveBombChance() native
float Function GetFlightFlyingAttackChance() native

Function SetFlightHoverChance(float chance) native
Function SetFlightDiveBombChance(float chance) native
Function SetFlightFlyingAttackChance(float mult) native


