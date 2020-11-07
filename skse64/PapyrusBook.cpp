#include "PapyrusBook.h"

#include "GameObjects.h"

namespace papyrusBook
{
	UInt32 GetSkill(TESObjectBOOK * thisBook)
	{
		if(thisBook && thisBook->data.GetSanitizedType() == TESObjectBOOK::Data::kType_Skill)
			return thisBook->data.teaches.skill;

		return 0;
	}

	SpellItem * GetSpell(TESObjectBOOK * thisBook)
	{
		if(thisBook && thisBook->data.GetSanitizedType() == TESObjectBOOK::Data::kType_Spell)
			return thisBook->data.teaches.spell;

		return NULL;
	}

	bool IsRead(TESObjectBOOK * thisBook)
	{
		return (thisBook) ? ((thisBook->data.flags & TESObjectBOOK::Data::kType_Read) == TESObjectBOOK::Data::kType_Read) : false;
	}

	bool IsTakeable(TESObjectBOOK * thisBook)
	{
		return (thisBook) ? ((thisBook->data.flags & TESObjectBOOK::Data::kType_CantBeTaken) == 0) : false;
	}
}

#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

void papyrusBook::RegisterFuncs(VMClassRegistry* registry)
{
	registry->RegisterFunction(
		new NativeFunction0<TESObjectBOOK, UInt32>("GetSkill", "Book", papyrusBook::GetSkill, registry));

	registry->RegisterFunction(
		new NativeFunction0<TESObjectBOOK, SpellItem *>("GetSpell", "Book", papyrusBook::GetSpell, registry));

	registry->RegisterFunction(
		new NativeFunction0<TESObjectBOOK, bool>("IsRead", "Book", papyrusBook::IsRead, registry));

	registry->RegisterFunction(
		new NativeFunction0<TESObjectBOOK, bool>("IsTakeable", "Book", papyrusBook::IsTakeable, registry));
}