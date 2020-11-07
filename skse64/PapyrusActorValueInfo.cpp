#include "PapyrusActorValueInfo.h"
#include "GameObjects.h"
#include "GameData.h"
#include "GameSettings.h"

#include "PapyrusArgs.h"

class MatchSkillPerks : public BGSSkillPerkTreeNode::PerkVisitor
{
	Actor * m_actor;
	bool m_unowned; // Only perks the actor doesn't own, or only perks the actor does own
	bool m_allRanks; // Walk all ranks for all perks
public:
	MatchSkillPerks(Actor * actor, bool unowned, bool allRanks) : m_actor(actor), m_unowned(unowned), m_allRanks(allRanks) {}

	virtual bool Accept(BGSPerk * perk)
	{
		if(perk)
		{
			bool addPerk = true;
			if(m_actor) {
				if(!CALL_MEMBER_FN(m_actor, HasPerk)(perk))
					addPerk = m_unowned;
				else
					addPerk = !m_unowned;
			}

			if(addPerk) {
				AddPerk(perk);
			}

			if(m_allRanks) {
				BGSPerk * nextPerk = perk->nextPerk;
				while(nextPerk) {
					addPerk = true;
					if(m_actor) {
						if(!CALL_MEMBER_FN(m_actor, HasPerk)(nextPerk))
							addPerk = m_unowned;
						else
							addPerk = !m_unowned;
					}

					if(addPerk) {
						AddPerk(nextPerk);
					}

					nextPerk = nextPerk->nextPerk;
				}
			}
		}

		return false;
	}
	virtual void AddPerk(BGSPerk * perk) = 0;
};

class PerkFormListVisitor : public MatchSkillPerks
{
	BGSListForm * m_formList;
public:
	PerkFormListVisitor::PerkFormListVisitor(BGSListForm * list, Actor * actor, bool unowned, bool allRanks) : MatchSkillPerks(actor, unowned, allRanks)
	{
		m_formList = list;
	}

	virtual void AddPerk(BGSPerk * perk)
	{
		CALL_MEMBER_FN(m_formList, AddFormToList)(perk);
	}
};

class PerkArrayVisitor : public MatchSkillPerks
{
	VMResultArray<BGSPerk*> * m_perks;
public:
	PerkArrayVisitor::PerkArrayVisitor(VMResultArray<BGSPerk*> * perkArray, Actor * actor, bool unowned, bool allRanks) : MatchSkillPerks(actor, unowned, allRanks)
	{
		m_perks = perkArray;
	}
	virtual void AddPerk(BGSPerk * perk)
	{
		m_perks->push_back(perk);
	}
};

namespace papyrusActorValueInfo
{
	ActorValueInfo * GetActorValueInfoByName(StaticFunctionTag * base, BSFixedString avName)
	{
		UInt32 actorValue = LookupActorValueByName(avName.data);	
		ActorValueList * avList = ActorValueList::GetSingleton();
		if(!avList)
			return NULL;
		
		return avList->GetActorValue(actorValue);
	}

	ActorValueInfo * GetActorValueInfoByID(StaticFunctionTag * base, UInt32 actorValue)
	{
		ActorValueList * avList = ActorValueList::GetSingleton();
		if(!avList)
			return NULL;

		return avList->GetActorValue(actorValue);
	}

	bool IsSkill(ActorValueInfo * info)
	{
		return (info && info->skillUsages) ? true : false;
	}

	float GetSkillUseMult(ActorValueInfo * info)
	{
		return (info && info->skillUsages) ? info->skillUsages[ActorValueInfo::kSkillUseMult] : 0.0;
	}

	void SetSkillUseMult(ActorValueInfo * info, float value)
	{
		if(info && info->skillUsages)
			info->skillUsages[ActorValueInfo::kSkillUseMult] = value;
	}

	float GetSkillOffsetMult(ActorValueInfo * info)
	{
		return (info && info->skillUsages) ? info->skillUsages[ActorValueInfo::kSkillOffsetMult] : 0.0;
	}

	void SetSkillOffsetMult(ActorValueInfo * info, float value)
	{
		if(info && info->skillUsages)
			info->skillUsages[ActorValueInfo::kSkillOffsetMult] = value;
	}

	float GetSkillImproveMult(ActorValueInfo * info)
	{
		return (info && info->skillUsages) ? info->skillUsages[ActorValueInfo::kSkillImproveMult] : 0.0;
	}

	void SetSkillImproveMult(ActorValueInfo * info, float value)
	{
		if(info && info->skillUsages)
			info->skillUsages[ActorValueInfo::kSkillImproveMult] = value;
	}

	float GetSkillImproveOffset(ActorValueInfo * info)
	{
		return (info && info->skillUsages) ? info->skillUsages[ActorValueInfo::kSkillImproveOffset] : 0.0;
	}

	void SetSkillImproveOffset(ActorValueInfo * info, float value)
	{
		if(info && info->skillUsages)
			info->skillUsages[ActorValueInfo::kSkillImproveOffset] = value;
	}

	float GetSkillExperience(ActorValueInfo * info)
	{
		PlayerCharacter* pPC = (*g_thePlayer);
		return (info && pPC->skills) ? pPC->skills->GetSkillPoints(info->name) : 0.0;
	}

	void SetSkillExperience(ActorValueInfo * info, float points)
	{
		PlayerCharacter* pPC = (*g_thePlayer);
		if(info && pPC->skills) {
			pPC->skills->SetSkillPoints(info->name, points);
		}
	}

	void AddSkillExperience(ActorValueInfo * info, float points)
	{
		if(info) {
			UInt32 actorValue = LookupActorValueByName(info->name);
			if(actorValue < ActorValueList::kNumActorValues) {
				PlayerCharacter* pPC = (*g_thePlayer);
				pPC->AdvanceSkill(actorValue, points, 0, 0);
			}
		}
	}

	float GetExperienceForLevel(ActorValueInfo * info, UInt32 level)
	{
		if(!info || !info->skillUsages)
			return 0.0;

		double fSkillUseCurve = 0.0;
		SettingCollectionMap	* settings = *g_gameSettingCollection;
		if(!settings)
			return 0.0;

		Setting	* skillUseCurve = settings->Get("fSkillUseCurve");
		if(skillUseCurve)
			skillUseCurve->GetDouble(&fSkillUseCurve);

		return info->skillUsages[ActorValueInfo::kSkillImproveMult] * pow(level, fSkillUseCurve) + info->skillUsages[ActorValueInfo::kSkillImproveOffset];
	}

	SInt32 GetSkillLegendaryLevel(ActorValueInfo * info)
	{
		PlayerCharacter* pPC = (*g_thePlayer);
		if(pPC && info && pPC->skills) {
			return pPC->skills->GetSkillLegendaryLevel(info->name);
		}

		return -1;
	}

	void SetSkillLegendaryLevel(ActorValueInfo * info, UInt32 level)
	{
		PlayerCharacter* pPC = (*g_thePlayer);
		if(pPC && info && pPC->skills) {
			pPC->skills->SetSkillLegendaryLevel(info->name, level);
		}
	}

	void GetPerkTree(ActorValueInfo * info, BGSListForm * formList, Actor * actor, bool unowned, bool allRanks)
	{
		if(info && formList) {
			BGSSkillPerkTreeNode * root = info->perkTree;
			if(root) {
				PerkFormListVisitor matcher(formList, actor, unowned, allRanks);
				root->VisitPerks(matcher);
			}
		}
	}

	VMResultArray<BGSPerk*> GetPerks(ActorValueInfo * info, Actor * actor, bool unowned, bool allRanks)
	{
		VMResultArray<BGSPerk*> result;
		if(info) {
			BGSSkillPerkTreeNode * root = info->perkTree;
			if(root) {
				PerkArrayVisitor matcher(&result, actor, unowned, allRanks);
				root->VisitPerks(matcher);
			}
		}
		return result;
	}

	float GetCurrentValue(ActorValueInfo * info, Actor * actor)
	{
		if(info && actor) {
			UInt32 actorValue = LookupActorValueByName(info->name);
			if(actorValue < ActorValueList::kNumActorValues) {
				return actor->actorValueOwner.GetCurrent(actorValue);
			}
		}
		return 0.0;
	}

	float GetBaseValue(ActorValueInfo * info, Actor * actor)
	{
		if(info && actor) {
			UInt32 actorValue = LookupActorValueByName(info->name);
			if(actorValue < ActorValueList::kNumActorValues) {
				return actor->actorValueOwner.GetBase(actorValue);
			}
		}
		return 0.0;
	}

	float GetMaximumValue(ActorValueInfo * info, Actor * actor)
	{
		if(info && actor) {
			UInt32 actorValue = LookupActorValueByName(info->name);
			if(actorValue < ActorValueList::kNumActorValues) {
				return actor->actorValueOwner.GetMaximum(actorValue);
			}
		}
		return 0.0;
	}
}

#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

void papyrusActorValueInfo::RegisterFuncs(VMClassRegistry* registry)
{
	registry->RegisterForm(ActorValueInfo::kTypeID, "ActorValueInfo");

	registry->RegisterFunction(
		new NativeFunction1<StaticFunctionTag, ActorValueInfo*, BSFixedString>("GetActorValueInfoByName", "ActorValueInfo", papyrusActorValueInfo::GetActorValueInfoByName, registry));

	registry->RegisterFunction(
		new NativeFunction1<StaticFunctionTag, ActorValueInfo*, UInt32>("GetActorValueInfoByID", "ActorValueInfo", papyrusActorValueInfo::GetActorValueInfoByID, registry));

	registry->RegisterFunction(
		new NativeFunction0<ActorValueInfo, bool>("IsSkill", "ActorValueInfo", papyrusActorValueInfo::IsSkill, registry));

	// Skill Usage
	registry->RegisterFunction(
		new NativeFunction0<ActorValueInfo, float>("GetSkillUseMult", "ActorValueInfo", papyrusActorValueInfo::GetSkillUseMult, registry));

	registry->RegisterFunction(
		new NativeFunction1<ActorValueInfo, void, float>("SetSkillUseMult", "ActorValueInfo", papyrusActorValueInfo::SetSkillUseMult, registry));

	registry->RegisterFunction(
		new NativeFunction0<ActorValueInfo, float>("GetSkillOffsetMult", "ActorValueInfo", papyrusActorValueInfo::GetSkillOffsetMult, registry));

	registry->RegisterFunction(
		new NativeFunction1<ActorValueInfo, void, float>("SetSkillOffsetMult", "ActorValueInfo", papyrusActorValueInfo::SetSkillOffsetMult, registry));

	registry->RegisterFunction(
		new NativeFunction0<ActorValueInfo, float>("GetSkillImproveMult", "ActorValueInfo", papyrusActorValueInfo::GetSkillImproveMult, registry));

	registry->RegisterFunction(
		new NativeFunction1<ActorValueInfo, void, float>("SetSkillImproveMult", "ActorValueInfo", papyrusActorValueInfo::SetSkillImproveMult, registry));

	registry->RegisterFunction(
		new NativeFunction0<ActorValueInfo, float>("GetSkillImproveOffset", "ActorValueInfo", papyrusActorValueInfo::GetSkillImproveOffset, registry));

	registry->RegisterFunction(
		new NativeFunction1<ActorValueInfo, void, float>("SetSkillImproveOffset", "ActorValueInfo", papyrusActorValueInfo::SetSkillImproveOffset, registry));

	// Skills
	registry->RegisterFunction(
		new NativeFunction0<ActorValueInfo, float>("GetSkillExperience", "ActorValueInfo", papyrusActorValueInfo::GetSkillExperience, registry));

	registry->RegisterFunction(
		new NativeFunction1<ActorValueInfo, void, float>("SetSkillExperience", "ActorValueInfo", papyrusActorValueInfo::SetSkillExperience, registry));

	registry->RegisterFunction(
		new NativeFunction1<ActorValueInfo, void, float>("AddSkillExperience", "ActorValueInfo", papyrusActorValueInfo::AddSkillExperience, registry));

	registry->RegisterFunction(
		new NativeFunction1 <ActorValueInfo, float, UInt32>("GetExperienceForLevel", "ActorValueInfo", papyrusActorValueInfo::GetExperienceForLevel, registry));

	registry->RegisterFunction(
		new NativeFunction0 <ActorValueInfo, SInt32>("GetSkillLegendaryLevel", "ActorValueInfo", papyrusActorValueInfo::GetSkillLegendaryLevel, registry));

	registry->RegisterFunction(
		new NativeFunction1 <ActorValueInfo, void, UInt32>("SetSkillLegendaryLevel", "ActorValueInfo", papyrusActorValueInfo::SetSkillLegendaryLevel, registry));


	// Perk Tree
	registry->RegisterFunction(
		new NativeFunction4 <ActorValueInfo, void, BGSListForm*, Actor*, bool, bool>("GetPerkTree", "ActorValueInfo", papyrusActorValueInfo::GetPerkTree, registry));

	registry->RegisterFunction(
		new NativeFunction3 <ActorValueInfo, VMResultArray<BGSPerk*>, Actor*, bool, bool>("GetPerks", "ActorValueInfo", papyrusActorValueInfo::GetPerks, registry));



	// Value Info
	registry->RegisterFunction(
		new NativeFunction1 <ActorValueInfo, float, Actor*>("GetCurrentValue", "ActorValueInfo", papyrusActorValueInfo::GetCurrentValue, registry));

	registry->RegisterFunction(
		new NativeFunction1 <ActorValueInfo, float, Actor*>("GetBaseValue", "ActorValueInfo", papyrusActorValueInfo::GetBaseValue, registry));

	registry->RegisterFunction(
		new NativeFunction1 <ActorValueInfo, float, Actor*>("GetMaximumValue", "ActorValueInfo", papyrusActorValueInfo::GetMaximumValue, registry));


	registry->SetFunctionFlags("ActorValueInfo", "GetActorValueInfoByName", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ActorValueInfo", "GetActorValueInfoByID", VMClassRegistry::kFunctionFlag_NoWait);

	registry->SetFunctionFlags("ActorValueInfo", "GetSkillUseMult", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ActorValueInfo", "SetSkillUseMult", VMClassRegistry::kFunctionFlag_NoWait);

	registry->SetFunctionFlags("ActorValueInfo", "GetSkillOffsetMult", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ActorValueInfo", "SetSkillOffsetMult", VMClassRegistry::kFunctionFlag_NoWait);

	registry->SetFunctionFlags("ActorValueInfo", "GetSkillImproveMult", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ActorValueInfo", "SetSkillImproveMult", VMClassRegistry::kFunctionFlag_NoWait);

	registry->SetFunctionFlags("ActorValueInfo", "GetSkillImproveOffset", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ActorValueInfo", "SetSkillImproveOffset", VMClassRegistry::kFunctionFlag_NoWait);

	registry->SetFunctionFlags("ActorValueInfo", "GetSkillLegendaryLevel", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ActorValueInfo", "SetSkillLegendaryLevel", VMClassRegistry::kFunctionFlag_NoWait);

	registry->SetFunctionFlags("ActorValueInfo", "GetSkillExperience", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ActorValueInfo", "SetSkillExperience", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ActorValueInfo", "AddSkillExperience", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ActorValueInfo", "GetExperienceForLevel", VMClassRegistry::kFunctionFlag_NoWait);

	registry->SetFunctionFlags("ActorValueInfo", "GetPerkTree", VMClassRegistry::kFunctionFlag_NoWait);
}
