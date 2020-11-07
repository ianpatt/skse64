#include "PapyrusActor.h"
#include "PapyrusArgs.h"

#include "GameForms.h"
#include "GameData.h"
#include "GameObjects.h"
#include "GameReferences.h"
#include "GameExtraData.h"
#include "GameRTTI.h"
#include "GameThreads.h"
#include "HashUtil.h"

#include "NiExtraData.h"
#include "InternalTasks.h"

#include <set>

class MatchBySlot : public FormMatcher
{
	UInt32 m_mask;
public:
	MatchBySlot(UInt32 slot) : 
	  m_mask(slot) 
	{
		
	}

	bool Matches(TESForm* pForm) const {
		if (pForm) {
			BGSBipedObjectForm* pBip = DYNAMIC_CAST(pForm, TESForm, BGSBipedObjectForm);
			if (pBip) {
				return (pBip->data.parts & m_mask) != 0;
			}
		}
		return false;
	}
};

class MatchByForm : public FormMatcher
{
	TESForm * m_form;
public:
	MatchByForm(TESForm * form) : m_form(form) {}

	bool Matches(TESForm* pForm) const { return m_form == pForm; }
};

typedef std::set<TESFaction*> FactionRankSet;
class CollectUniqueFactions : public Actor::FactionVisitor
{
public:
	CollectUniqueFactions::CollectUniqueFactions(FactionRankSet * rankSet, SInt8 min, SInt8 max) : m_rankSet(rankSet), m_min(min), m_max(max) { }
	virtual bool Accept(TESFaction * faction, SInt8 rank)
	{
		if(rank >= m_min && rank <= m_max)
			m_rankSet->insert(faction);
		return false;
	}

private:
	SInt8 m_min;
	SInt8 m_max;
	FactionRankSet * m_rankSet;
};

bool CanEquipBothHands(Actor* actor, TESForm * item)
{
	BGSEquipType * equipType = DYNAMIC_CAST(item, TESForm, BGSEquipType);
	if (!equipType)
		return false;

	BGSEquipSlot * equipSlot = equipType->GetEquipSlot();
	if (!equipSlot)
		return false;

	// 2H
	if (equipSlot == GetEitherHandSlot())
	{
		return true;
	}
	// 1H
	else if (equipSlot == GetLeftHandSlot() || equipSlot == GetRightHandSlot())
	{
		return (actor->race->data.raceFlags & TESRace::kRace_CanDualWield) && item->IsWeapon();
	}

	return false;
}

BGSEquipSlot * GetEquipSlotById(SInt32 slotId)
{
	enum
	{
		kSlotId_Default = 0,
		kSlotId_Right = 1,
		kSlotId_Left = 2
	};

	if (slotId == kSlotId_Right)
		return GetRightHandSlot();
	else if (slotId == kSlotId_Left)
		return GetLeftHandSlot();
	else
		return NULL;
}

SInt32 CalcItemId(TESForm * form, BaseExtraList * extraList)
{
	if (!form || !extraList)
		return 0;

	const char * name = extraList->GetDisplayName(form);

	// No name in extra data? Use base form name
	if (!name)
	{
		TESFullName* pFullName = DYNAMIC_CAST(form, TESForm, TESFullName);
		if (pFullName)
			name = pFullName->name.data;
	}

	if (!name)
		return 0;

	return (SInt32)HashUtil::CRC32(name, form->formID & 0x00FFFFFF);
}

namespace papyrusActor
{

	TESForm* GetWornForm(Actor* thisActor, UInt32 mask)
	{
		MatchBySlot matcher(mask);	
		ExtraContainerChanges* pContainerChanges = static_cast<ExtraContainerChanges*>(thisActor->extraData.GetByType(kExtraData_ContainerChanges));
		if (pContainerChanges) {
			EquipData eqD = pContainerChanges->FindEquipped(matcher);
			return eqD.pForm;
		}
		return NULL;
	}

	SInt32 GetWornItemId(Actor* thisActor, UInt32 mask)
	{
		ExtraContainerChanges* containerChanges = static_cast<ExtraContainerChanges*>(thisActor->extraData.GetByType(kExtraData_ContainerChanges));
		if (!containerChanges)
			return 0;

		MatchBySlot matcher(mask);	
		EquipData equipData = containerChanges->FindEquipped(matcher);

		return CalcItemId(equipData.pForm, equipData.pExtraData);
	}

	TESForm * GetEquippedObject(Actor * thisActor, UInt32 slot)
	{
		if(!thisActor) return NULL;

		enum
		{
			kSlotID_Left = 0,
			kSlotID_Right,
			kSlotID_Voice,
		};

		if(slot == kSlotID_Voice)
			return thisActor->equippedShout;
		else
			return thisActor->GetEquippedObject(slot == kSlotID_Left);
	}

	SInt32 GetEquippedItemId(Actor * thisActor, UInt32 slot)
	{
		enum
		{
			kSlotID_Left = 0,
			kSlotID_Right
		};

		if (!thisActor)
			return NULL;

		TESForm * equippedForm = thisActor->GetEquippedObject(slot == kSlotID_Left);
		if (!equippedForm)
			return 0;

		ExtraContainerChanges* containerChanges = static_cast<ExtraContainerChanges*>(thisActor->extraData.GetByType(kExtraData_ContainerChanges));
		if (!containerChanges)
			return 0;

		MatchByForm matcher(equippedForm);
		EquipData equipData = containerChanges->FindEquipped(matcher, slot == kSlotID_Right, slot == kSlotID_Left);

		return CalcItemId(equipData.pForm, equipData.pExtraData);
	}

	UInt32 GetSpellCount(Actor* thisActor)
	{
		if(!thisActor) return NULL;

		return thisActor->addedSpells.Length();
	}

	SpellItem* GetNthSpell(Actor* thisActor, UInt32 n)
	{
		if(!thisActor) return NULL;

		return thisActor->addedSpells.Get(n);
	}

#ifdef _AEFFECTS
	UInt32 GetNumActiveEffects(Actor* thisActor)
	{
		if(thisActor)
		{
			tList<ActiveEffect> * effects = thisActor->magicTarget.GetActiveEffects();
			if(effects) {
				UInt32 count = effects->Count();
				_MESSAGE("Total Effects: %d", count);
				return count;
			}
		}
		return 0;
	}

	ActiveEffect* GetNthActiveEffect(Actor* thisActor, UInt32 n)
	{
		if(thisActor) {
			tList<ActiveEffect> * effects = thisActor->magicTarget.GetActiveEffects();
			if(effects) {
				UInt32 count = effects->Count();
				ActiveEffect * effect = effects->GetNthItem(n);
				_MESSAGE("Dumping n: %d Total: %d", n, count); // Test
				DumpClass(effect, 20);
				return (effects && n < count) ? effect : NULL;
			}
		}
		
		return NULL;
	}
#endif

	void EquipItemEx(Actor* thisActor, TESForm* item, SInt32 slotId, bool preventUnequip, bool equipSound)
	{
		if (!item)
			return;

		if (!item->Has3D())
			return;

		EquipManager* equipManager = EquipManager::GetSingleton();
		if (!equipManager)
			return;

		ExtraContainerChanges* containerChanges = static_cast<ExtraContainerChanges*>(thisActor->extraData.GetByType(kExtraData_ContainerChanges));
		ExtraContainerChanges::Data* containerData = containerChanges ? containerChanges->data : NULL;
		if (!containerData)
			return;

		// Copy/merge of extraData and container base. Free after use.
		InventoryEntryData* entryData = containerData->CreateEquipEntryData(item);
		if (!entryData)
			return;

		BGSEquipSlot * targetEquipSlot = GetEquipSlotById(slotId);

		SInt32 itemCount = entryData->countDelta;

		// For ammo, use count, otherwise always equip 1
		SInt32 equipCount = item->IsAmmo() ? itemCount : 1;

		bool isTargetSlotInUse = false;

		// Need at least 1 (maybe 2 for dual wield, checked later)
		bool hasItemMinCount = itemCount > 0;

		BaseExtraList * rightEquipList = NULL;
		BaseExtraList * leftEquipList = NULL;

		BaseExtraList * curEquipList = NULL;
		BaseExtraList * enchantList = NULL;

		if (hasItemMinCount)
		{
			entryData->GetExtraWornBaseLists(&rightEquipList, &leftEquipList);

			// Case 1: Already equipped in both hands.
			if (leftEquipList && rightEquipList)
			{
				isTargetSlotInUse = true;
				curEquipList = (targetEquipSlot == GetLeftHandSlot()) ? leftEquipList : rightEquipList;
				enchantList = NULL;
			}
			// Case 2: Already equipped in right hand.
			else if (rightEquipList)
			{
				isTargetSlotInUse = targetEquipSlot == GetRightHandSlot();
				curEquipList = rightEquipList;
				enchantList = NULL;
			}
			// Case 3: Already equipped in left hand.
			else if (leftEquipList)
			{
				isTargetSlotInUse = targetEquipSlot == GetLeftHandSlot();
				curEquipList = leftEquipList;
				enchantList = NULL;
			}
			// Case 4: Not equipped yet.
			else
			{
				isTargetSlotInUse = false;
				curEquipList = NULL;
				enchantList = entryData->extendDataList->GetNthItem(0);
			}
		}

		// Free temp equip entryData
		entryData->Delete();

		// Normally EquipManager would update CannotWear, if equip is skipped we do it here
		if (isTargetSlotInUse)
		{
			BSExtraData* xCannotWear = curEquipList->GetByType(kExtraData_CannotWear);
			if (xCannotWear && !preventUnequip)
				curEquipList->Remove(kExtraData_CannotWear, xCannotWear);
			else if (!xCannotWear && preventUnequip)
				curEquipList->Add(kExtraData_CannotWear, ExtraCannotWear::Create());

			// Slot in use, nothing left to do
			return;
		}

		// For dual wield, prevent that 1 item can be equipped in two hands if its already equipped
		bool isEquipped = (rightEquipList || leftEquipList);
		if (targetEquipSlot && isEquipped && CanEquipBothHands(thisActor, item))
			hasItemMinCount = itemCount > 1;

		if (!isTargetSlotInUse && hasItemMinCount)
			CALL_MEMBER_FN(equipManager, EquipItem)(thisActor, item, enchantList, equipCount, targetEquipSlot, equipSound, preventUnequip, false, NULL);
	}

	void EquipItemById(Actor* thisActor, TESForm* item, SInt32 itemId, SInt32 slotId, bool preventUnequip /*unused*/, bool equipSound)
	{
		if (!item || !item->Has3D() || itemId == 0)
			return;

		// Can't be improved or enchanted, no need for itemId
		if (item->IsAmmo())
		{
			EquipItemEx(thisActor, item, slotId, preventUnequip, equipSound);
			return;
		}

		EquipManager* equipManager = EquipManager::GetSingleton();
		if (!equipManager)
			return;

		ExtraContainerChanges* containerChanges = static_cast<ExtraContainerChanges*>(thisActor->extraData.GetByType(kExtraData_ContainerChanges));
		ExtraContainerChanges::Data* containerData = containerChanges ? containerChanges->data : NULL;
		if (!containerData)
			return;

		InventoryEntryData::EquipData itemData;
		containerData->GetEquipItemData(itemData, item, itemId);

		BGSEquipSlot * targetEquipSlot = GetEquipSlotById(slotId);
		bool isTargetSlotInUse = false;

		SInt32 itemCount = itemData.itemCount;

		// Need at least 1 (maybe 2 for dual wield, checked later)
		bool hasItemMinCount = itemCount > 0;
		bool canDualWield = false;

		BaseExtraList * newEquipList = itemData.itemExtraList;

		if (hasItemMinCount)
		{
			// Case 1: Type already equipped in both hands.
			if (itemData.isTypeWorn && itemData.isTypeWornLeft)
			{
				isTargetSlotInUse = true;
			}
			// Case 2: Type already equipped in right hand.
			else if (itemData.isTypeWorn)
			{
				isTargetSlotInUse = targetEquipSlot == GetRightHandSlot() || targetEquipSlot == NULL;
			}
			// Case 3: Type already equipped in left hand.
			else if (itemData.isTypeWornLeft)
			{
				isTargetSlotInUse = targetEquipSlot == GetLeftHandSlot();
			}
			// Case 4: Type not equipped yet.
			else
			{
				isTargetSlotInUse = false;
			}
		}

		// This also returns if the target slot is in use by another weapon of the same type.
		// Could handle this, but switching them here causes a bug (0 damage) for some reason.
		// So we just skip it. Can be handled on the Papyrus side.
		if (isTargetSlotInUse || !hasItemMinCount)
			return;

		bool isItemEquipped = itemData.isItemWorn || itemData.isItemWornLeft;

		// Does this item qualify for dual wield?
		if (item->IsWeapon() && targetEquipSlot && isItemEquipped && CanEquipBothHands(thisActor, item))
			canDualWield = true;

		// Not enough items to dual wield, weapon has to swap hands
		if (canDualWield && itemCount < 2)
		{
			BaseExtraList* unequipList = itemData.isItemWornLeft ? itemData.wornLeftExtraList : itemData.wornExtraList;

			// Unequip might destroy passed list (return value indicates that).
			newEquipList = CALL_MEMBER_FN(equipManager, UnequipItem)(thisActor, item, unequipList, 1, 0, false, false, true, false, NULL) ? NULL : unequipList;
		}

		CALL_MEMBER_FN(equipManager, EquipItem)(thisActor, item, newEquipList, 1, targetEquipSlot, equipSound, preventUnequip, false, NULL);
	}

	void UnequipItemEx(Actor* thisActor, TESForm* item, SInt32 slotId, bool preventEquip)
	{
		if (!item)
			return;

		if (!item->Has3D())
			return;

		EquipManager* equipManager = EquipManager::GetSingleton();
		if (!equipManager)
			return;

		ExtraContainerChanges* containerChanges = static_cast<ExtraContainerChanges*>(thisActor->extraData.GetByType(kExtraData_ContainerChanges));
		ExtraContainerChanges::Data* containerData = containerChanges ? containerChanges->data : NULL;
		if (!containerData)
			return;

		InventoryEntryData* entryData = containerData->FindItemEntry(item);
		if (!entryData)
			return;

		BGSEquipSlot * targetEquipSlot = GetEquipSlotById(slotId);

		SInt32 itemCount = entryData->countDelta;

		// For ammo, use count, otherwise always equip 1
		SInt32 equipCount = item->IsAmmo() ? itemCount : 1;

		BaseExtraList * rightEquipList = NULL;
		BaseExtraList * leftEquipList = NULL;

		entryData->GetExtraWornBaseLists(&rightEquipList, &leftEquipList);

		bool unequipRight = false;
		bool unequipLeft = false;

		if (targetEquipSlot == GetRightHandSlot())
			unequipRight = true;
		else if (targetEquipSlot == GetLeftHandSlot())
			unequipLeft = true;
		else
			unequipRight = unequipLeft = true;

		if (rightEquipList && unequipRight)
		{
			BSExtraData* xCannotWear = rightEquipList->GetByType(kExtraData_CannotWear);
			if (xCannotWear)
				rightEquipList->Remove(kExtraData_CannotWear, xCannotWear);

			CALL_MEMBER_FN(equipManager, UnequipItem)(thisActor, item, rightEquipList, equipCount, GetRightHandSlot(), true, preventEquip, true, false, NULL);
		}

		if (leftEquipList && unequipLeft)
		{
			BSExtraData* xCannotWear = leftEquipList->GetByType(kExtraData_CannotWear);
			if (xCannotWear)
				leftEquipList->Remove(kExtraData_CannotWear, xCannotWear);

			CALL_MEMBER_FN(equipManager, UnequipItem)(thisActor, item, leftEquipList, equipCount, GetLeftHandSlot(), true, preventEquip, true, false, NULL);
		}
	}

	void QueueNiNodeUpdate(Actor* thisActor)
	{
		Character * pChar = DYNAMIC_CAST(thisActor, Actor, Character);
		if(pChar) {
			CALL_MEMBER_FN(pChar, QueueNiNodeUpdate)(false); // False makes this allow weapons to not be auto holstered apparently
		}
	}

	void RegenerateHead(Actor * thisActor)
	{
		TaskInterface::RegenerateHead(thisActor);
	}

	void ChangeHeadPart(Actor * thisActor, BGSHeadPart * newPart)
	{
		if(!thisActor || !newPart)
			return;

		TESNPC* npc = DYNAMIC_CAST(thisActor->baseForm, TESForm, TESNPC);
		if(npc) {
			if(newPart->type != BGSHeadPart::kTypeMisc) {
				BGSHeadPart * oldPart = npc->GetCurrentHeadPartByType(newPart->type);

				// Alters the ActorBase's HeadPart list
				CALL_MEMBER_FN(npc, ChangeHeadPart)(newPart);

				// Alters the loaded mesh
				TaskInterface::ChangeHeadPart(thisActor, oldPart, newPart);
			}
		}
	}

	void ReplaceHeadPart(Actor * thisActor, BGSHeadPart * oldPart, BGSHeadPart * newPart)
	{
		if(!thisActor || !newPart)
			return;

		TESNPC* npc = DYNAMIC_CAST(thisActor->baseForm, TESForm, TESNPC);
		if(npc) {
			if(!oldPart) {
				oldPart = npc->GetCurrentHeadPartByType(newPart->type);
			}
			if(newPart->type != BGSHeadPart::kTypeMisc && oldPart && oldPart->type == newPart->type) {
				TaskInterface::ChangeHeadPart(thisActor, oldPart, newPart);
			}
		}
	}

	void UpdateWeight(Actor * thisActor, float neckDelta)
	{
		CALL_MEMBER_FN(thisActor, QueueNiNodeUpdate)(true);
		TaskInterface::UpdateWeight(thisActor, neckDelta,  0, true);
	}

	bool IsAIEnabled(Actor * thisActor)
	{
		if (!thisActor)
			return false;

		return (thisActor->flags1 & Actor::kFlags_AIEnabled) == Actor::kFlags_AIEnabled;
	}

	void ResetAI(Actor * thisActor)
	{
		if (!thisActor)
			return;

		UInt32 flags = thisActor->flags;

		if (!(flags & TESForm::kFlagUnk_0x800) &&
			!(flags & TESForm::kFlagIsDeleted))
			CALL_MEMBER_FN(thisActor,ResetAI)(0,1);
	}

	bool IsSwimming(Actor * thisActor)
	{
		if (!thisActor)
			return false;

		return (thisActor->actorState.flags04 & ActorState::kState_Swimming) == ActorState::kState_Swimming;
	}

	void SheatheWeapon(Actor * thisActor)
	{
		if (thisActor) {
			thisActor->DrawSheatheWeapon(false);
		}
	}

	TESObjectREFR * GetFurnitureReference(Actor * thisActor)
	{
		if(!thisActor)
			return NULL;
		ActorProcessManager * processManager = thisActor->processManager;
		if(!processManager)
			return NULL;
		MiddleProcess * middleProcess = processManager->middleProcess;
		if(!middleProcess)
			return NULL;

		NiPointer<TESObjectREFR> refr;
		UInt32 furnitureHandle = middleProcess->furnitureHandle;
		if(furnitureHandle == (*g_invalidRefHandle) || furnitureHandle == 0)
			return NULL;

		LookupREFRByHandle(furnitureHandle, refr);
		return refr;
	}

	void SetExpressionPhoneme(Actor * thisActor, UInt32 index, float value)
	{
		TaskInterface::UpdateExpression(thisActor, BSFaceGenAnimationData::kKeyframeType_Phoneme, index, value);
	}

	void SetExpressionModifier(Actor * thisActor, UInt32 index, float value)
	{
		TaskInterface::UpdateExpression(thisActor, BSFaceGenAnimationData::kKeyframeType_Modifier, index, value);
	}

	void ResetExpressionOverrides(Actor * thisActor)
	{
		TaskInterface::UpdateExpression(thisActor, BSFaceGenAnimationData::kKeyframeType_Reset, 0, 0);
	}

	VMResultArray<TESFaction*> GetFactions(Actor* thisActor, SInt32 gte, SInt32 lte)
	{
		VMResultArray<TESFaction*> factions;
		if(thisActor) {
			if(gte > SCHAR_MAX)
				gte = SCHAR_MAX;
			if(gte < SCHAR_MIN)
				gte = SCHAR_MIN;
			if(lte < SCHAR_MIN)
				lte = SCHAR_MIN;
			if(lte > SCHAR_MAX)
				lte = SCHAR_MAX;
			FactionRankSet rankSet;
			CollectUniqueFactions factionVisitor(&rankSet, gte, lte);
			thisActor->VisitFactions(factionVisitor);

			for(FactionRankSet::iterator it = rankSet.begin(); it != rankSet.end(); ++it)
				factions.push_back(*it);
		}

		return factions;
	}
}

#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

void papyrusActor::RegisterFuncs(VMClassRegistry* registry)
{
	registry->RegisterFunction(
		new NativeFunction1 <Actor, TESForm*, UInt32>("GetWornForm", "Actor", papyrusActor::GetWornForm, registry));

	registry->RegisterFunction(
		new NativeFunction1 <Actor, TESForm*, UInt32>("GetEquippedObject", "Actor", papyrusActor::GetEquippedObject, registry));

	registry->RegisterFunction(
		new NativeFunction0 <Actor, UInt32>("GetSpellCount", "Actor", papyrusActor::GetSpellCount, registry));

	registry->RegisterFunction(
		new NativeFunction1 <Actor, SpellItem*, UInt32>("GetNthSpell", "Actor", papyrusActor::GetNthSpell, registry));

#ifdef _AEFFECTS
	registry->RegisterFunction(
		new NativeFunction0 <Actor, UInt32>("GetNumActiveEffects", "Actor", papyrusActor::GetNumActiveEffects, registry));

	registry->RegisterFunction(
		new NativeFunction1 <Actor, ActiveEffect*, UInt32>("GetNthActiveEffect", "Actor", papyrusActor::GetNthActiveEffect, registry));
#endif

	registry->RegisterFunction(
		new NativeFunction4 <Actor, void, TESForm*, SInt32, bool, bool>("EquipItemEx", "Actor", papyrusActor::EquipItemEx, registry));

	registry->RegisterFunction(
		new NativeFunction3 <Actor, void, TESForm*, SInt32, bool>("UnequipItemEx", "Actor", papyrusActor::UnequipItemEx, registry));

	registry->RegisterFunction(
		new NativeFunction0 <Actor, void>("QueueNiNodeUpdate", "Actor", papyrusActor::QueueNiNodeUpdate, registry));

	registry->RegisterFunction(
		new NativeFunction1 <Actor, void, BGSHeadPart*>("ChangeHeadPart", "Actor", papyrusActor::ChangeHeadPart, registry));

	registry->RegisterFunction(
		new NativeFunction2 <Actor, void, BGSHeadPart*, BGSHeadPart*>("ReplaceHeadPart", "Actor", papyrusActor::ReplaceHeadPart, registry));

	registry->RegisterFunction(
		new NativeFunction0 <Actor, void>("RegenerateHead", "Actor", papyrusActor::RegenerateHead, registry));

	registry->RegisterFunction(
		new NativeFunction1 <Actor, void, float>("UpdateWeight", "Actor", papyrusActor::UpdateWeight, registry));

	registry->RegisterFunction(
		new NativeFunction0 <Actor, bool>("IsAIEnabled", "Actor", papyrusActor::IsAIEnabled, registry));

	registry->RegisterFunction(
		new NativeFunction0 <Actor, void>("ResetAI", "Actor", papyrusActor::ResetAI, registry));

	registry->RegisterFunction(
		new NativeFunction0 <Actor, bool>("IsSwimming", "Actor", papyrusActor::IsSwimming, registry));

	registry->RegisterFunction(
		new NativeFunction0 <Actor, void>("SheatheWeapon", "Actor", papyrusActor::SheatheWeapon, registry));

	registry->RegisterFunction(
		new NativeFunction5 <Actor, void, TESForm*, SInt32, SInt32, bool, bool>("EquipItemById", "Actor", papyrusActor::EquipItemById, registry));

	registry->RegisterFunction(
		new NativeFunction1 <Actor, SInt32, UInt32>("GetEquippedItemId", "Actor", papyrusActor::GetEquippedItemId, registry));

	registry->RegisterFunction(
		new NativeFunction1 <Actor, SInt32, UInt32>("GetWornItemId", "Actor", papyrusActor::GetWornItemId, registry));

	registry->RegisterFunction(
		new NativeFunction0 <Actor, TESObjectREFR*>("GetFurnitureReference", "Actor", papyrusActor::GetFurnitureReference, registry));

	registry->RegisterFunction(
		new NativeFunction2 <Actor, void, UInt32, float>("SetExpressionPhoneme", "Actor", papyrusActor::SetExpressionPhoneme, registry));

	registry->RegisterFunction(
		new NativeFunction2 <Actor, void, UInt32, float>("SetExpressionModifier", "Actor", papyrusActor::SetExpressionModifier, registry));

	registry->RegisterFunction(
		new NativeFunction0 <Actor, void>("ResetExpressionOverrides", "Actor", papyrusActor::ResetExpressionOverrides, registry));

	registry->RegisterFunction(
		new NativeFunction2 <Actor, VMResultArray<TESFaction*>, SInt32, SInt32>("GetFactions", "Actor", papyrusActor::GetFactions, registry));
}
