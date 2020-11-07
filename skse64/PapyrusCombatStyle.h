#pragma once

class TESCombatStyle;
class VMClassRegistry;

namespace papyrusCombatStyle
{
	void RegisterFuncs(VMClassRegistry* registry);

	float GetOffensiveMult(TESCombatStyle* thisCombatStyle);
	void SetOffensiveMult(TESCombatStyle* thisCombatStyle, float nuVal);
	float GetDefensiveMult(TESCombatStyle* thisCombatStyle);
	void SetDefensiveMult(TESCombatStyle* thisCombatStyle, float nuVal);
	float GetGroupOffensiveMult(TESCombatStyle* thisCombatStyle);
	void SetGroupOffensiveMult(TESCombatStyle* thisCombatStyle, float nuVal);
	float GetMeleeMult(TESCombatStyle* thisCombatStyle);
	void SetMeleeMult(TESCombatStyle* thisCombatStyle, float nuVal);
	float GetRangedMult(TESCombatStyle* thisCombatStyle);
	void SetRangedMult(TESCombatStyle* thisCombatStyle, float nuVal);
	float GetMagicMult(TESCombatStyle* thisCombatStyle);
	void SetMagicMult(TESCombatStyle* thisCombatStyle, float nuVal);
	float GetShoutMult(TESCombatStyle* thisCombatStyle);
	void SetShoutMult(TESCombatStyle* thisCombatStyle, float nuVal);
	float GetStaffMult(TESCombatStyle* thisCombatStyle);
	void SetStaffMult(TESCombatStyle* thisCombatStyle, float nuVal);
	float GetUnarmedMult(TESCombatStyle* thisCombatStyle);
	void SetUnarmedMult(TESCombatStyle* thisCombatStyle, float nuVal);
	float GetAvoidThreatChance(TESCombatStyle* thisCombatStyle);
	void SetAvoidThreatChance(TESCombatStyle* thisCombatStyle, float nuVal);

	float GetMeleeAttackStaggeredMult(TESCombatStyle* thisCombatStyle);
	void SetMeleeAttackStaggeredMult(TESCombatStyle* thisCombatStyle, float nuVal);
	float GetMeleePowerAttackStaggeredMult(TESCombatStyle* thisCombatStyle);
	void SetMeleePowerAttackStaggeredMult(TESCombatStyle* thisCombatStyle, float nuVal);
	float GetMeleePowerAttackBlockingMult(TESCombatStyle* thisCombatStyle);
	void SetMeleePowerAttackBlockingMult(TESCombatStyle* thisCombatStyle, float nuVal);
	float GetMeleeBashMult(TESCombatStyle* thisCombatStyle);
	void SetMeleeBashMult(TESCombatStyle* thisCombatStyle, float nuVal);
	float GetMeleeBashRecoiledMult(TESCombatStyle* thisCombatStyle);
	void SetMeleeBashRecoiledMult(TESCombatStyle* thisCombatStyle, float nuVal);
	float GetMeleeBashAttackMult(TESCombatStyle* thisCombatStyle);
	void SetMeleeBashAttackMult(TESCombatStyle* thisCombatStyle, float nuVal);
	float GetMeleeBashPowerAttackMult(TESCombatStyle* thisCombatStyle);
	void SetMeleeBashPowerAttackMult(TESCombatStyle* thisCombatStyle, float nuVal);
	float GetMeleeSpecialAttackMult(TESCombatStyle* thisCombatStyle);
	void SetMeleeSpecialAttackMult(TESCombatStyle* thisCombatStyle, float nuVal);

	float GetCloseRangeDuelingCircleMult(TESCombatStyle* thisCombatStyle);
	void SetCloseRangeDuelingCircleMult(TESCombatStyle* thisCombatStyle, float nuVal);
	float GetCloseRangeDuelingFallbackMult(TESCombatStyle* thisCombatStyle);
	void SetCloseRangeDuelingFallbackMult(TESCombatStyle* thisCombatStyle, float nuVal);
	float GetCloseRangeFlankingFlankDistance(TESCombatStyle* thisCombatStyle);
	void SetCloseRangeFlankingFlankDistance(TESCombatStyle* thisCombatStyle, float nuVal);
	float GetCloseRangeFlankingStalkTime(TESCombatStyle* thisCombatStyle);
	void SetCloseRangeFlankingStalkTime(TESCombatStyle* thisCombatStyle, float nuVal);

	float GetLongRangeStrafeMult(TESCombatStyle* thisCombatStyle);
	void SetLongRangeStrafeMult(TESCombatStyle* thisCombatStyle, float nuVal);

	float GetFlightHoverChance(TESCombatStyle* thisCombatStyle);
	void SetFlightHoverChance(TESCombatStyle* thisCombatStyle, float nuVal);
	float GetFlightDiveBombChance(TESCombatStyle* thisCombatStyle);
	void SetFlightDiveBombChance(TESCombatStyle* thisCombatStyle, float nuVal);
	// undecoded
	float GetFlightFlyingAttackChance(TESCombatStyle* thisCombatStyle);
	void SetFlightFlyingAttackChance(TESCombatStyle* thisCombatStyle, float nuVal);
	bool GetAllowDuelWielding(TESCombatStyle* thisCombatStyle);
	void SetAllowDuelWielding(TESCombatStyle* thisCombatStyle, float nuVal);

};
