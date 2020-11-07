#pragma once

class TESForm;
class TESObjectWEAP;
class Actor;
class SpellItem;
class ActiveEffect;
class VMClassRegistry;
class BGSHeadPart;
class TESObjectREFR;

namespace papyrusActor
{
	void RegisterFuncs(VMClassRegistry* registry);

	TESForm* GetWornForm(Actor* thisActor, UInt32 slot);
	void QueueNiNodeUpdate(Actor* thisActor);
	UInt32 GetSpellCount(Actor* thisActor);
	SpellItem* GetNthSpell(Actor* thisActor, UInt32 n);

#ifdef _AEFFECTS
	UInt32 GetNumActiveEffects(Actor* thisActor);
	ActiveEffect* GetNthActiveEffect(Actor* thisActor, UInt32 n);
#endif

	void EquipItemEx(Actor* thisActor, TESForm* item, SInt32 slotId, bool preventUnequip, bool equipSound);
	void UnequipItemEx(Actor* thisActor, TESForm* item, SInt32 slotId, bool preventEquip);

	//void ReplaceHeadPart(Actor * thisActor, BGSHeadPart * oldPart, BGSHeadPart* newPart);
	//void ChangeHeadPart(Actor * thisActor, BGSHeadPart * newPart);
	//void RegenerateHead(Actor * thisActor);
	//void UpdateWeight(Actor * thisActor, float neckDelta);

	bool IsAIEnabled(Actor * thisActor);
	bool IsSwimming(Actor * thisActor);
	void SheatheWeapon(Actor * thisActor);

	void EquipItemById(Actor* thisActor, TESForm* item, SInt32 itemId, SInt32 slotId, bool preventUnequip, bool equipSound);
	TESObjectREFR * GetFurnitureReference(Actor * thisActor);
}
