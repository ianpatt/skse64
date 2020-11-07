#pragma once

class VMClassRegistry;
class ActorValueInfo;
struct StaticFunctionTag;
class BGSListForm;
class Actor;

#include "GameTypes.h"

namespace papyrusActorValueInfo
{
	void RegisterFuncs(VMClassRegistry* registry);

	ActorValueInfo * GetActorValueInfoByName(StaticFunctionTag * base, BSFixedString avName);
	ActorValueInfo * GetActorValueInfoByID(StaticFunctionTag * base, UInt32 actorValue);

	bool IsSkill(ActorValueInfo * info);

	float GetSkillUseMult(ActorValueInfo * info);
	void SetSkillUseMult(ActorValueInfo * info, float value);

	float GetSkillOffsetMult(ActorValueInfo * info);
	void SetSkillOffsetMult(ActorValueInfo * info, float value);

	float GetSkillImproveMult(ActorValueInfo * info);
	void SetSkillImproveMult(ActorValueInfo * info, float value);

	float GetSkillImproveOffset(ActorValueInfo * info);
	void SetSkillImproveOffset(ActorValueInfo * info, float value);

	float GetSkillExperience(ActorValueInfo * info);
	void SetSkillExperience(ActorValueInfo * info, float points);
	void AddSkillExperience(ActorValueInfo * info, float points);
	float GetExperienceForLevel(ActorValueInfo * info, UInt32 level);

	SInt32 GetSkillLegendaryLevel(ActorValueInfo * info);
	void SetSkillLegendaryLevel(ActorValueInfo * info, UInt32 level);
	


	void GetPerkTree(ActorValueInfo * info, BGSListForm * formList, Actor * actor, bool unowned, bool allRanks);
};
