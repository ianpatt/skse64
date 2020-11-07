#include "skse64/GameFormComponents.h"
#include "skse64/GameData.h"

UInt32 BGSBipedObjectForm::AddSlotToMask(UInt32 slot)
{
	data.parts |= slot;
	return data.parts;
}

UInt32 BGSBipedObjectForm::RemoveSlotFromMask(UInt32 slot)
{
	data.parts &= ~slot;
	return data.parts;
}

UInt32 TESSpellList::GetSpellCount() const
{
	return data ? data->numSpells : 0;
}

SpellItem* TESSpellList::GetNthSpell(UInt32 n) const
{
	if (data && n < data->numSpells) {
		return data->spells[n];
	}
	else return NULL;
}

UInt32 TESSpellList::GetShoutCount() const
{
	return data ? data->numShouts : 0;
}

TESShout* TESSpellList::GetNthShout(UInt32 n) const
{
	if (data && n < data->numShouts) {
		return data->shouts[n];
	}
	else return NULL;
}

class ItemCounter
{
	UInt32	m_count;
	TESForm	* m_item;

public:
	ItemCounter(TESForm * item) : m_count(0), m_item(item) {}

	bool Accept(TESContainer::Entry * entry)
	{
		if (entry->form == m_item)
			m_count += entry->count;
		return true;
	}

	UInt32 Count() const	{ return m_count; }
};

UInt32 TESContainer::CountItem(TESForm * item) const
{
	ItemCounter v(item);
	Visit(v);
	return v.Count();
}

UInt32 TintMask::ToARGB()
{
	return MAKE_COLOR((UInt32)(alpha * 255), color.red, color.green, color.blue);
}

SInt32 PlayerSkills::ResolveAdvanceableSkillId(SInt32 actorValue)
{
	if(actorValue - kAdvanceableSkillOffset >= 0 && actorValue < kNumAdvanceableSkills + kAdvanceableSkillOffset)
		return actorValue - kAdvanceableSkillOffset;

	return -1;
}

float PlayerSkills::GetSkillPoints(BSFixedString actorValue)
{
	if(data) {
		UInt32 avId = LookupActorValueByName(actorValue.data);
		SInt32 skillId = ResolveAdvanceableSkillId(avId);
		if(skillId != -1)
			return data->levelData[skillId].points;
	}

	return 0.0;
}

void PlayerSkills::SetSkillPoints(BSFixedString actorValue, float points)
{
	if(data) {
		UInt32 avId = LookupActorValueByName(actorValue.data);
		SInt32 skillId = ResolveAdvanceableSkillId(avId);
		if(skillId != -1)
			data->levelData[skillId].points = points;
	}
}

SInt32 PlayerSkills::GetSkillLegendaryLevel(BSFixedString actorValue)
{
	if(data) {
		UInt32 avId = LookupActorValueByName(actorValue.data);
		SInt32 skillId = ResolveAdvanceableSkillId(avId);
		if(skillId != -1)
			return data->legendaryLevel[skillId];
	}

	return -1;
}

void PlayerSkills::SetSkillLegendaryLevel(BSFixedString actorValue, UInt32 level)
{
	if(data) {
		UInt32 avId = LookupActorValueByName(actorValue.data);
		SInt32 skillId = ResolveAdvanceableSkillId(avId);
		if(skillId != -1)
			data->legendaryLevel[skillId] = level;
	}
}
