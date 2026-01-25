#include "GameExtraData.h"
#include "GameBSExtraData.h"
#include "GameRTTI.h"
#include "GameAPI.h"
#include "GameData.h"
#include "HashUtil.h"

// ??_7ExtraHealth@@6B@
const RelocPtr<uintptr_t> s_ExtraHealthVtbl(0x0152C060);
// ??_7ExtraCharge@@6B@
const RelocPtr<uintptr_t> s_ExtraChargeVtbl(0x0152C0C0);
// ??_7ExtraCount@@6B@
const RelocPtr<uintptr_t> s_ExtraCountVtbl(0x0152BFE0);
// ??_7ExtraTextDisplayData@@6B@
const RelocPtr<uintptr_t> s_ExtraTextDisplayVtbl(0x0152CAE0);
// ??_7ExtraSoul@@6B@
const RelocPtr<uintptr_t> s_ExtraSoulVtbl(0x0152F2E0);
// ??_7ExtraOwnership@@6B@
const RelocPtr<uintptr_t> s_ExtraOwnershipVtbl(0x0152BF80);
// ??_7ExtraAliasInstanceArray@@6B@
const RelocPtr<uintptr_t> s_ExtraAliasInstanceArrayVtbl(0x0152C9A0);
// ??_7ExtraCannotWear@@6B@
const RelocPtr<uintptr_t> s_ExtraCannotWearVtbl(0x0152C040);
// ??_7ExtraHotkey@@6B@
const RelocPtr<uintptr_t> s_ExtraHotkeyVtbl(0x0152C120);
// ??_7ExtraForcedTarget@@6B@
const RelocPtr<uintptr_t> s_ExtraForcedTargetVtbl(0x0152CB60);
// ??_7ExtraReferenceHandle@@6B@
const RelocPtr<uintptr_t> s_ExtraReferenceHandleVtbl(0x0152C160);
// ??_7ExtraEnchantment@@6B@
const RelocPtr<uintptr_t> s_ExtraEnchantmentVtbl(0x0152C480);
// ??_7ExtraRank@@6B@
const RelocPtr<uintptr_t> s_ExtraRankVtbl(0x0152BFC0);
// ??_7ExtraUniqueID@@6B@
const RelocPtr<uintptr_t> s_ExtraUniqueIdVtbl(0x0152CBA0);

ExtraHealth* ExtraHealth::Create() 
{
	ExtraHealth* xHealth = (ExtraHealth*)BSExtraData::Create(sizeof(ExtraHealth), s_ExtraHealthVtbl.GetUIntPtr());
	xHealth->health = 1;
	return xHealth;
}

ExtraCharge* ExtraCharge::Create()
{
	ExtraCharge* xCharge = (ExtraCharge*)BSExtraData::Create(sizeof(ExtraCharge), s_ExtraChargeVtbl.GetUIntPtr());
	xCharge->charge = 0;
	return xCharge;
}

ExtraEnchantment* ExtraEnchantment::Create()
{
	ExtraEnchantment* xEnchantment = (ExtraEnchantment*)BSExtraData::Create(sizeof(ExtraEnchantment), s_ExtraEnchantmentVtbl.GetUIntPtr());
	xEnchantment->enchant = NULL;
	xEnchantment->maxCharge = 0;
	xEnchantment->unk0E = 0;
	return xEnchantment;
}

ExtraCannotWear* ExtraCannotWear::Create() 
{
	ExtraCannotWear* xCannotWear = (ExtraCannotWear*)BSExtraData::Create(sizeof(ExtraCannotWear), s_ExtraCannotWearVtbl.GetUIntPtr());
	return xCannotWear;
}

ExtraHotkey* ExtraHotkey::Create()
{
	ExtraHotkey* xHotkey = (ExtraHotkey*)BSExtraData::Create(sizeof(ExtraHotkey), s_ExtraHotkeyVtbl.GetUIntPtr());
	xHotkey->hotkey = -1;
	return xHotkey;
}

ExtraCount* ExtraCount::Create()
{
	ExtraCount* xCount = (ExtraCount*)BSExtraData::Create(sizeof(ExtraCount), s_ExtraCountVtbl.GetUIntPtr());
	return xCount;
}

ExtraSoul* ExtraSoul::Create()
{
	ExtraSoul* xSoul = (ExtraSoul*)BSExtraData::Create(sizeof(ExtraSoul), s_ExtraSoulVtbl.GetUIntPtr());
	return xSoul;
}

ExtraForcedTarget* ExtraForcedTarget::Create()
{
	ExtraForcedTarget* xForcedTarget = (ExtraForcedTarget*)BSExtraData::Create(sizeof(ExtraForcedTarget), s_ExtraForcedTargetVtbl.GetUIntPtr());
	xForcedTarget->handle = (*g_invalidRefHandle);
	return xForcedTarget;
}

ExtraTextDisplayData* ExtraTextDisplayData::Create()
{
	ExtraTextDisplayData* xText = (ExtraTextDisplayData*)BSExtraData::Create(sizeof(ExtraTextDisplayData), s_ExtraTextDisplayVtbl.GetUIntPtr());
	xText->unk14 = -1;
	xText->extraHealthValue = 1.0;
	return xText;
}

ExtraReferenceHandle* ExtraReferenceHandle::Create()
{
	ExtraReferenceHandle* xReference = (ExtraReferenceHandle*)BSExtraData::Create(sizeof(ExtraReferenceHandle), s_ExtraReferenceHandleVtbl.GetUIntPtr());
	return xReference;
}

ExtraRank* ExtraRank::Create() 
{
	ExtraRank* xRank = (ExtraRank*)BSExtraData::Create(sizeof(ExtraRank), s_ExtraRankVtbl.GetUIntPtr());
	xRank->rank = 0;
	return xRank;
}

ExtraUniqueID* ExtraUniqueID::Create() 
{
	ExtraUniqueID* xUniqueId = (ExtraUniqueID*)BSExtraData::Create(sizeof(ExtraUniqueID), s_ExtraUniqueIdVtbl.GetUIntPtr());
	xUniqueId->ownerFormId = 0;
	xUniqueId->uniqueId = 0;
	return xUniqueId;
}

NiPointer<TESObjectREFR> ExtraReferenceHandle::GetReference()
{
	NiPointer<TESObjectREFR> reference;
	if(handle == (*g_invalidRefHandle) || handle == 0)
		return NULL;

	LookupREFRByHandle(handle, reference);
	return reference;
}

NiPointer<TESObjectREFR> ExtraEnableStateParent::GetReference()
{
	NiPointer<TESObjectREFR> reference;
	if(handle == (*g_invalidRefHandle) || handle == 0)
		return NULL;

	LookupREFRByHandle(handle, reference);
	return reference;
}

NiPointer<TESObjectREFR> ExtraForcedTarget::GetReference()
{
	NiPointer<TESObjectREFR> reference;
	if(handle == (*g_invalidRefHandle) || handle == 0)
		return NULL;

	LookupREFRByHandle(handle, reference);
	return reference;
}

const char* ExtraTextDisplayData::GenerateName(TESForm * form, float extraHealthValue)
{
	return CALL_MEMBER_FN(this, GenerateName_Internal)(form, extraHealthValue);
}

struct GetMatchingEquipped
{
	FormMatcher&	m_matcher;
	EquipData		m_found;
	bool			m_isWorn;
	bool			m_isWornLeft;

	GetMatchingEquipped(FormMatcher& matcher, bool isWorn = true, bool isWornLeft = true) : m_matcher(matcher), m_isWorn(isWorn), m_isWornLeft(isWornLeft)
	{
		m_found.pForm = NULL;
		m_found.pExtraData = NULL;
	}

	bool Accept(InventoryEntryData* pEntryData)
	{
		if (pEntryData)
		{
			// quick check - needs an extendData or can't be equipped
			ExtendDataList* pExtendList = pEntryData->extendDataList;
			if (pExtendList && m_matcher.Matches(pEntryData->type))
			{
				for (ExtendDataList::Iterator it = pExtendList->Begin(); !it.End(); ++it)
				{
					BaseExtraList * pExtraDataList = it.Get();

					if (pExtraDataList)
					{
						if ((m_isWorn && pExtraDataList->HasType(kExtraData_Worn)) || (m_isWornLeft && pExtraDataList->HasType(kExtraData_WornLeft)))
						{
							m_found.pForm = pEntryData->type;
							m_found.pExtraData = pExtraDataList;
							return false;
						}
					}
				}
			}
		}
		return true;
	}

	EquipData Found()
	{
		return m_found;
	}
};

EquipData ExtraContainerChanges::FindEquipped(FormMatcher& matcher, bool isWorn, bool isWornLeft) const
{
	FoundEquipData equipData;
	if (data && data->objList) {
		GetMatchingEquipped getEquipped(matcher, isWorn, isWornLeft);
		data->objList->Visit(getEquipped);
		equipData = getEquipped.Found();
	}
	return equipData;
};

class HotkeyDataFinder
{
private:

	// Match by either of those, depending on which ctor was used
	SInt32		m_matchHotkey;
	TESForm *	m_matchForm;

	HotkeyData	m_found;

public:

	HotkeyDataFinder(SInt32 hotkey) : m_matchHotkey(hotkey)
	{
		m_matchForm = NULL;

		m_found.pForm = NULL;
		m_found.pHotkey = NULL;
	}

	HotkeyDataFinder(TESForm * form) : m_matchForm(form)
	{
		m_matchHotkey = -1;

		m_found.pForm = NULL;
		m_found.pHotkey = NULL;
	}

	bool Accept(InventoryEntryData* pEntryData)
	{
		if (!pEntryData)
			return true;

		// If matching by form, skip if this is not the one we're looking for
		if (m_matchForm && m_matchForm != pEntryData->type)
			return true;

		ExtendDataList* pExtendList = pEntryData->extendDataList;
		if (!pExtendList)
			return true;

		SInt32 n = 0;
		BaseExtraList* pExtraDataList = pExtendList->GetNthItem(n);
		while (pExtraDataList)
		{
			if (ExtraHotkey * extraHotkey = static_cast<ExtraHotkey*>(pExtraDataList->GetByType(kExtraData_Hotkey)))
			{
				// Matching by form - found ExtraHotkey?
				if (m_matchForm)
				{
					m_found.pForm = pEntryData->type;
					m_found.pHotkey = extraHotkey;
					return false;
				}
				// Matching by hotkey - compare hotkeys
				else
				{
					if (extraHotkey->hotkey == m_matchHotkey)
					{
						m_found.pForm = pEntryData->type;
						m_found.pHotkey = extraHotkey;
						return false;
					}
				}
			}

			n++;
			pExtraDataList = pExtendList->GetNthItem(n);
		}

		return true;
	}

	HotkeyData& Found()
	{
		return m_found;
	}
};

HotkeyData ExtraContainerChanges::FindHotkey(SInt32 hotkey) const
{
	FoundHotkeyData hotkeyData;
	if (data && data->objList) {
		HotkeyDataFinder getHotkey(hotkey);
		data->objList->Visit(getHotkey);
		hotkeyData = getHotkey.Found();
	}
	return hotkeyData;
}

HotkeyData ExtraContainerChanges::FindHotkey(TESForm * form) const
{
	FoundHotkeyData hotkeyData;
	if (data && data->objList) {
		HotkeyDataFinder getHotkey(form);
		data->objList->Visit(getHotkey);
		hotkeyData = getHotkey.Found();
	}
	return hotkeyData;
}

InventoryEntryData::InventoryEntryData(TESForm * item, UInt32 count)
{
	type = item;
	countDelta = count;
	extendDataList = NULL;
}

InventoryEntryData * InventoryEntryData::Create(TESForm * item, UInt32 count)
{
	InventoryEntryData * p = (InventoryEntryData *)Heap_Allocate(sizeof(InventoryEntryData));
	ASSERT(p);

	new (p) InventoryEntryData(item, count);
	p->extendDataList = ExtendDataList::Create();

	return p;
}

void InventoryEntryData::Delete(void)
{
	if (extendDataList)
	{
		extendDataList->Delete();
		extendDataList = NULL;
	}
	Heap_Free(this);
}

void InventoryEntryData::GetExtraWornBaseLists(BaseExtraList ** pWornBaseListOut, BaseExtraList ** pWornLeftBaseListOut) const
{
	bool checkWorn = pWornBaseListOut != NULL;
	bool checkWornLeft = pWornLeftBaseListOut != NULL;

	if (!extendDataList)
		return;

	if (!checkWorn && !checkWornLeft)
		return;

	for (ExtendDataList::Iterator it = extendDataList->Begin(); !it.End(); ++it)
	{
		BaseExtraList * xList = it.Get();
		if (!xList)
			continue;

		if (checkWorn && xList->HasType(kExtraData_Worn))
		{
			checkWorn = false;
			*pWornBaseListOut = xList;

			if (!checkWornLeft)
				break;
		}

		if (checkWornLeft && xList->HasType(kExtraData_WornLeft))
		{
			checkWornLeft = false;
			*pWornLeftBaseListOut = xList;

			if (!checkWorn)
				break;
		}
	}
}

InventoryEntryData::EquipData::EquipData() :
	itemCount(0),
	itemExtraList(NULL),
	wornExtraList(NULL),
	wornLeftExtraList(NULL),
	isItemWorn(false),
	isItemWornLeft(false),
	isTypeWorn(false),
	isTypeWornLeft(false)
{
}

void InventoryEntryData::GetEquipItemData(EquipData& stateOut, SInt32 itemId, SInt32 baseCount) const
{
	bool checkDisplayName = itemId != 0;

	// When searching for a specific itemId, start at 0 and count up for every match.
	// When searching for the base item, start at baseCount+delta and subtract 1 for every named item.
	stateOut.itemCount = checkDisplayName ? 0 : (baseCount + countDelta);

	// Search for match based on textDisplayData
	for (ExtendDataList::Iterator it = extendDataList->Begin(); !it.End(); ++it)
	{
		BaseExtraList * xList = it.Get();
		if (!xList)
			continue;

		SInt32 count = 1;

		ExtraCount* xCount = static_cast<ExtraCount*>(xList->GetByType(kExtraData_Count));
		if (xCount)
			count = xCount->count;

		const char * displayName = xList->GetDisplayName(type);

		bool isWorn = xList->HasType(kExtraData_Worn);
		bool isWornLeft = xList->HasType(kExtraData_WornLeft);

		if (isWorn)
		{
			stateOut.isTypeWorn = true;
			stateOut.wornExtraList = xList;
		}

		if (isWornLeft)
		{
			stateOut.isTypeWornLeft = true;
			stateOut.wornLeftExtraList = xList;
		}

		if (checkDisplayName)
		{
			if (displayName)
			{
				SInt32 xItemId = (SInt32) HashUtil::CRC32(displayName, type->formID & 0x00FFFFFF);
				if (itemId == xItemId)
				{
					if (isWorn)
						stateOut.isItemWorn = true;
					else if (isWornLeft)
						stateOut.isItemWornLeft = true;
					else
						stateOut.itemExtraList = xList;

					stateOut.itemCount += count;
				}
			}
		}
		else
		{
			if (!displayName)
			{
				if (isWorn)
					stateOut.isItemWorn = true;
				else if (isWornLeft)
					stateOut.isItemWornLeft = true;
				else
					stateOut.itemExtraList = xList;
			}
			else
			{
				stateOut.itemCount -= count;
			}
		}
	}
}

InventoryEntryData * ExtraContainerChanges::Data::FindItemEntry(TESForm * item) const
{
	typedef EntryDataList::Iterator	EntryDataIterator;

	if (!objList)
		return NULL;

	for (EntryDataIterator it = objList->Begin(); !it.End(); ++it)
	{
		InventoryEntryData * e = it.Get();
		if (e && e->type == item)
			return e;
	}

	return NULL;
}

InventoryEntryData * ExtraContainerChanges::Data::CreateEquipEntryData(TESForm * item)
{
	InventoryEntryData * newEntryData = NULL;

	// Get count from baseForm container
	UInt32 baseCount = 0;
	if (owner && owner->baseForm)
	{
		TESContainer * container = DYNAMIC_CAST(owner->baseForm, TESForm, TESContainer);
		if (container)
			baseCount = container->CountItem(item);
	}

	// Find existing entryData for this item
	InventoryEntryData * curEntryData = FindItemEntry(item);

	if (curEntryData)
	{
		newEntryData = InventoryEntryData::Create(item, baseCount + curEntryData->countDelta);
		
		ExtendDataList * curExtendDataList = curEntryData->extendDataList;
		ExtendDataList * newExtendDataList = newEntryData->extendDataList;

		if (curExtendDataList)
		{
			BaseExtraList * head =  curExtendDataList->GetNthItem(0);
			if (head && (! head->CheckContainerExtraData(true)))
				newExtendDataList->Insert(head);
			else
				newExtendDataList->Append(curExtendDataList->Begin());
		}
		else
		{
			// Native func does this, though the entryData is deleted later anyway...
			newExtendDataList->Delete();
			newEntryData->extendDataList = NULL;
		}
	}
	else
	{
		if (baseCount > 0)
		{
			newEntryData = InventoryEntryData::Create(item, baseCount);
		}
	}

	return newEntryData;
}

void ExtraContainerChanges::Data::GetEquipItemData(InventoryEntryData::EquipData& stateOut, TESForm * item, SInt32 itemId) const
{
	// Get count from baseForm container
	UInt32 baseCount = 0;
	if (owner && owner->baseForm)
	{
		TESContainer * container = DYNAMIC_CAST(owner->baseForm, TESForm, TESContainer);
		if (container)
			baseCount = container->CountItem(item);
	}

	bool matchedBaseForm = false;

	// Test base form name for itemId
	TESFullName* pFullName = DYNAMIC_CAST(item, TESForm, TESFullName);
	if (pFullName)
	{
		const char * name = pFullName->name.data;
		SInt32 baseItemId = (SInt32)HashUtil::CRC32(name, item->formID & 0x00FFFFFF);

		if (baseItemId == itemId)
			matchedBaseForm = true;
	}

	// Find existing entryData for this item
	InventoryEntryData * curEntryData = FindItemEntry(item);

	// Found entryData
	if (curEntryData)
	{
		curEntryData->GetEquipItemData(stateOut, (matchedBaseForm ? 0 : itemId), baseCount);
	}
	else if (matchedBaseForm)
	{
		stateOut.itemCount = baseCount;
	}
}
