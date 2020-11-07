#include "PapyrusObjectReference.h"
#include "PapyrusWornObject.h"
#include "PapyrusForm.h"

#include "GameAPI.h"
#include "GameFormComponents.h"
#include "GameForms.h"
#include "GameRTTI.h"
#include "GameObjects.h"
#include "GameExtraData.h"
#include "GameData.h"
#include "GameThreads.h"

#include "InternalTasks.h"

//#include "NiNodes.h"

#include <vector>
#include <map>

typedef std::vector<InventoryEntryData*> ExtraDataVec;
typedef std::map<TESForm*, UInt32> ExtraContainerMap;

class ExtraContainerReceiver
{
public:
	virtual void AddFormToReceiver(TESForm * form) = 0;
};

class ExtraContainerFormList : public ExtraContainerReceiver
{
	BGSListForm * m_formList;
public:
	ExtraContainerFormList::ExtraContainerFormList(BGSListForm * list) : m_formList(list) { }

	virtual void AddFormToReceiver(TESForm * form)
	{
		CALL_MEMBER_FN(m_formList, AddFormToList)(form);
	}
};

class ExtraContainerArray : public ExtraContainerReceiver
{
	VMResultArray<TESForm*> * m_array;
public:
	ExtraContainerArray::ExtraContainerArray(VMResultArray<TESForm*> * list) : m_array(list) { }

	virtual void AddFormToReceiver(TESForm * form)
	{
		m_array->push_back(form);
	}
};

class ExtraContainerInfo
{
	ExtraDataVec	m_vec;
	ExtraContainerMap m_map;
public:
	ExtraContainerInfo(EntryDataList * entryList) : m_map(), m_vec()
	{
		m_vec.reserve(128);
		if (entryList) {
			entryList->Visit(*this);
		}
	}

	bool Accept(InventoryEntryData* data) 
	{
		if (data) {
			m_vec.push_back(data);
			m_map[data->type] = m_vec.size()-1;
		}
		return true;
	}

	bool IsValidEntry(TESContainer::Entry* pEntry, SInt32& numObjects)
	{
		if (pEntry) {
			numObjects = pEntry->count;
			TESForm* pForm = pEntry->form;

			if (DYNAMIC_CAST(pForm, TESForm, TESLevItem))
				return false;

			ExtraContainerMap::iterator it = m_map.find(pForm);
			ExtraContainerMap::iterator itEnd = m_map.end();
			if (it != itEnd) {
				UInt32 index = it->second;
				InventoryEntryData* pXData = m_vec[index];
				if (pXData) {
					numObjects += pXData->countDelta;
				}
				// clear the object from the vector so we don't bother to look for it
				// in the second step
				m_vec[index] = NULL;
			}

			if (numObjects > 0) {
				//if (IsConsoleMode()) {
				//	PrintItemType(pForm);
				//}
				return true;
			}
		}
		return false;
	}

	// returns the count of items left in the vector
	UInt32 CountItems() {
		UInt32 count = 0;
		ExtraDataVec::iterator itEnd = m_vec.end();
		ExtraDataVec::iterator it = m_vec.begin();
		while (it != itEnd) {
			InventoryEntryData* extraData = (*it);
			if (extraData && (extraData->countDelta > 0)) {
				count++;
				//if (IsConsoleMode()) {
				//	PrintItemType(extraData->type);
				//}
			}
			++it;
		}
		return count;
	}

	// returns the weight of items left in the vector
	float GetTotalWeight() {
		float weight = 0.0;
		ExtraDataVec::iterator itEnd = m_vec.end();
		ExtraDataVec::iterator it = m_vec.begin();
		while (it != itEnd) {
			InventoryEntryData* extraData = (*it);
			if (extraData && (extraData->countDelta > 0)) {
				weight += papyrusForm::GetWeight(extraData->type) * extraData->countDelta;
			}
			++it;
		}
		return weight;
	}

	void GetRemainingForms(ExtraContainerReceiver * receiver) {
		ExtraDataVec::iterator itEnd = m_vec.end();
		ExtraDataVec::iterator it = m_vec.begin();
		while (it != itEnd) {
			InventoryEntryData* extraData = (*it);
			if (extraData && (extraData->countDelta > 0)) {
				receiver->AddFormToReceiver(extraData->type);
			}
			++it;
		}
	}

	InventoryEntryData* GetNth(UInt32 n, UInt32 count) {
		ExtraDataVec::iterator itEnd = m_vec.end();
		ExtraDataVec::iterator it = m_vec.begin();
		while (it != itEnd) {
			InventoryEntryData* extraData = (*it);
			if (extraData && (extraData->countDelta > 0)) {
				if(count == n)
				{
					return extraData;
				}
				count++;
			}
			++it;
		}
		return NULL;
	}

};


class ContainerCountIf
{
	ExtraContainerInfo& m_info;
public:
	ContainerCountIf(ExtraContainerInfo& info) : m_info(info) { }

	bool Accept(TESContainer::Entry* pEntry) const
	{
		SInt32 numObjects = 0; // not needed in this count
		return m_info.IsValidEntry(pEntry, numObjects);
	}
};

class ContainerFindNth
{
	ExtraContainerInfo& m_info;
	UInt32 m_findIndex;
	UInt32 m_curIndex;
public:
	ContainerFindNth(ExtraContainerInfo& info, UInt32 findIndex) : m_info(info), m_findIndex(findIndex), m_curIndex(0) { }

	bool Accept(TESContainer::Entry* pEntry)
	{
		SInt32 numObjects = 0;
		if (m_info.IsValidEntry(pEntry, numObjects)) {
			if (m_curIndex == m_findIndex) {
				return true;
			}
			m_curIndex++;
		}
		return false;
	}

	UInt32 GetCurIdx() { return m_curIndex; }
};

class ContainerTotalWeight
{
	ExtraContainerInfo& m_info;
	float m_totalWeight;
public:
	ContainerTotalWeight(ExtraContainerInfo& info) : m_info(info), m_totalWeight(0.0) { }

	bool Accept(TESContainer::Entry* pEntry)
	{
		SInt32 numObjects = 0;
		if (m_info.IsValidEntry(pEntry, numObjects)) {
			m_totalWeight += papyrusForm::GetWeight(pEntry->form) * numObjects;
		}

		return true;
	}

	float GetTotalWeight() { return m_totalWeight; }
};

class ExtraContainerFiller
{
	ExtraContainerInfo& m_info;
	ExtraContainerReceiver * m_receiver;
public:
	ExtraContainerFiller(ExtraContainerInfo& info, ExtraContainerReceiver * receiver) : m_info(info), m_receiver(receiver) { }

	bool Accept(TESContainer::Entry* pEntry)
	{
		SInt32 numObjects = 0;
		if (m_info.IsValidEntry(pEntry, numObjects)) {
			m_receiver->AddFormToReceiver(pEntry->form);
		}

		return true;
	}
};

namespace papyrusObjectReference
{
	UInt32 GetNumItems(TESObjectREFR* pContainerRef)
	{
		if (!pContainerRef)
			return 0;

		TESContainer* pContainer = NULL;
		TESForm* pBaseForm = pContainerRef->baseForm;
		if (pBaseForm) {
			pContainer = DYNAMIC_CAST(pBaseForm, TESForm, TESContainer);
		}
		if (!pContainer)
			return 0;

		UInt32 count = 0;
			
		ExtraContainerChanges* pXContainerChanges = static_cast<ExtraContainerChanges*>(pContainerRef->extraData.GetByType(kExtraData_ContainerChanges));
		ExtraContainerInfo info(pXContainerChanges ? pXContainerChanges->data->objList : NULL);


		// first walk the base container
		if (pContainer) {
			ContainerCountIf counter(info);
			count = pContainer->CountIf(counter);
		}

		// now count the remaining items
		count += info.CountItems();

		return count;
	}
	

	TESForm* GetNthForm(TESObjectREFR* pContainerRef, UInt32 n)
	{
		if (!pContainerRef)
			return NULL;

		TESContainer* pContainer = NULL;
		TESForm* pBaseForm = pContainerRef->baseForm;
		if (pBaseForm) {
			pContainer = DYNAMIC_CAST(pBaseForm, TESForm, TESContainer);
		}
		if (!pContainer)
			return NULL;
		
		UInt32 count = 0;

		ExtraContainerChanges* pXContainerChanges = static_cast<ExtraContainerChanges*>(pContainerRef->extraData.GetByType(kExtraData_ContainerChanges));
		ExtraContainerInfo info(pXContainerChanges ? pXContainerChanges->data->objList : NULL);

		// first look in the base container
		if (pContainer) {
			ContainerFindNth finder(info, n);
			TESContainer::Entry* pFound = pContainer->Find(finder);
			if (pFound) {
				return pFound->form;
			} else {
				count = finder.GetCurIdx();
			}
		}

		// now walk the remaining items in the map
		InventoryEntryData* pEntryData = info.GetNth(n, count);
		if (pEntryData) {
			return pEntryData->type;
		}
		return NULL;
	}

	float GetTotalItemWeight(TESObjectREFR* pContainerRef)
	{
		if (!pContainerRef)
			return 0;
		ExtraContainerChanges* pXContainerChanges = static_cast<ExtraContainerChanges*>(pContainerRef->extraData.GetByType(kExtraData_ContainerChanges));
		if (!pXContainerChanges)
			return 0.0;
		
		// skyrim keeps track of the player's total item weight
		if (pContainerRef == *g_thePlayer)
			return pXContainerChanges->data->totalWeight;

		// Compute the weight for non-players
		float weight = 0.0;
		TESContainer* pContainer = NULL;
		TESForm* pBaseForm = pContainerRef->baseForm;
		if (pBaseForm) {
			pContainer = DYNAMIC_CAST(pBaseForm, TESForm, TESContainer);
		}

		if (!pContainer)
			return 0;

		ExtraContainerInfo info(pXContainerChanges ? pXContainerChanges->data->objList : NULL);

		// first walk the base container
		if (pContainer) {
			ContainerTotalWeight weightCounter(info);
			pContainer->Visit(weightCounter);
			weight = weightCounter.GetTotalWeight();
		}

		// now sum the remaining weight
		weight += info.GetTotalWeight();
		return weight;
	}

	float GetTotalArmorWeight(TESObjectREFR* pContainerRef)
	{
		if (!pContainerRef)
			return 0;
		ExtraContainerChanges* pXContainerChanges = static_cast<ExtraContainerChanges*>(pContainerRef->extraData.GetByType(kExtraData_ContainerChanges));
		return (pXContainerChanges) ? pXContainerChanges->data->armorWeight : 0.0;
	}

	bool IsHarvested(TESObjectREFR* pProduceRef)
	{
		if(!pProduceRef)
			return false;

		UInt8 formType = pProduceRef->baseForm->formType;
		if (formType == kFormType_Tree || formType == kFormType_Flora) {
			return (pProduceRef->flags & TESObjectREFR::kFlag_Harvested) != 0;
		}
		return false;
	}

	void SetHarvested(TESObjectREFR * pProduceRef, bool isHarvested)
	{
		if(pProduceRef) {
			UInt8 formType = pProduceRef->baseForm->formType;
			if (formType == kFormType_Tree || formType == kFormType_Flora) {
				if(isHarvested)
					pProduceRef->flags |= TESObjectREFR::kFlag_Harvested;
				else
					pProduceRef->flags &= ~TESObjectREFR::kFlag_Harvested;
			}

			TaskInterface::UpdateHarvestModel(pProduceRef);
		}
	}

	void SetItemHealthPercent(TESObjectREFR* object, float value)
	{
		// Object must be a weapon, or armor
		if(object) {
			referenceUtils::SetItemHealthPercent(object->baseForm, &object->extraData, value);
		}
	}

	float GetItemMaxCharge(TESObjectREFR* object)
	{
		if (!object)
			return 0.0;
		
		return referenceUtils::GetItemMaxCharge(object->baseForm, &object->extraData);
	}

	void SetItemMaxCharge(TESObjectREFR* object, float maxCharge)
	{
		if (object) {
			referenceUtils::SetItemMaxCharge(object->baseForm, &object->extraData, maxCharge);
		}
	}

	float GetItemCharge(TESObjectREFR* object)
	{
		if (!object)
			return 0.0;

		return referenceUtils::GetItemCharge(object->baseForm, &object->extraData);
	}

	void SetItemCharge(TESObjectREFR* object, float value)
	{
		// Object must be an enchanted weapon
		if(object) {
			referenceUtils::SetItemCharge(object->baseForm, &object->extraData, value);
		}
	}

	EnchantmentItem * GetEnchantment(TESObjectREFR* object)
	{
		if (!object)
			return NULL;

		return referenceUtils::GetEnchantment(&object->extraData);
	}

	void CreateEnchantment(TESObjectREFR* object, float maxCharge, VMArray<EffectSetting*> effects, VMArray<float> magnitudes, VMArray<UInt32> areas, VMArray<UInt32> durations)
	{
		if(object) {
			referenceUtils::CreateEnchantment(object->baseForm, &object->extraData, maxCharge, effects, magnitudes, areas, durations);
		}
	}

	void SetEnchantment(TESObjectREFR* object, EnchantmentItem * enchantment, float maxCharge)
	{
		if(object) {
			referenceUtils::SetEnchantment(object->baseForm, &object->extraData, enchantment, maxCharge);
		}
	}

	AlchemyItem * GetPoison(TESObjectREFR* object)
	{
		if (!object)
			return NULL;

		return referenceUtils::GetPoison(&object->extraData);
	}


	void ResetInventory(TESObjectREFR * obj)
	{
		obj->ResetInventory(false);
	}

	bool IsOffLimits(TESObjectREFR * obj)
	{
		return CALL_MEMBER_FN(obj, IsOffLimits)();
	}

	BSFixedString GetDisplayName(TESObjectREFR* object)
	{
		return (object) ? CALL_MEMBER_FN(object, GetReferenceName)() : "";
	}

	bool SetDisplayName(TESObjectREFR* object, BSFixedString value, bool force)
	{
		return referenceUtils::SetDisplayName(&object->extraData, value, force);
	}

	TESObjectREFR * GetEnableParent(TESObjectREFR* object)
	{
		if(!object)
			return NULL;

		ExtraEnableStateParent* xEnableParent = static_cast<ExtraEnableStateParent*>(object->extraData.GetByType(kExtraData_EnableStateParent));
		if(!xEnableParent)
			return NULL;

		return xEnableParent->GetReference();
	}

	UInt32 GetNumReferenceAliases(TESObjectREFR* object)
	{
		if(!object)
			return 0;

		return referenceUtils::GetNumReferenceAliases(&object->extraData);
	}

	BGSRefAlias * GetNthReferenceAlias(TESObjectREFR* object, UInt32 n)
	{
		if(!object)
			return 0;

		return referenceUtils::GetNthReferenceAlias(&object->extraData, n);
	}

	VMResultArray<BGSRefAlias*> GetReferenceAliases(TESObjectREFR* object)
	{
		VMResultArray<BGSRefAlias*> result;
		if(!object)
			return result;

		return referenceUtils::GetReferenceAliases(&object->extraData);
	}

	void GetAllForms(TESObjectREFR* pContainerRef, BGSListForm * list)
	{
		if(pContainerRef && list) {
			ExtraContainerChanges* pXContainerChanges = static_cast<ExtraContainerChanges*>(pContainerRef->extraData.GetByType(kExtraData_ContainerChanges));
			if (pXContainerChanges) {
				TESContainer* pContainer = NULL;
				TESForm* pBaseForm = pContainerRef->baseForm;
				if (pBaseForm)
					pContainer = DYNAMIC_CAST(pBaseForm, TESForm, TESContainer);

				ExtraContainerFormList formContainer(list);
				ExtraContainerInfo info(pXContainerChanges ? pXContainerChanges->data->objList : NULL);
				// first walk the base container
				if (pContainer) {
					ExtraContainerFiller formFiller(info, &formContainer);
					pContainer->Visit(formFiller);
				}

				info.GetRemainingForms(&formContainer);
			}
		}
	}

	VMResultArray<TESForm*> GetContainerForms(TESObjectREFR* pContainerRef)
	{
		VMResultArray<TESForm*> result;
		if(pContainerRef) {
			ExtraContainerChanges* pXContainerChanges = static_cast<ExtraContainerChanges*>(pContainerRef->extraData.GetByType(kExtraData_ContainerChanges));
			if (pXContainerChanges) {
				TESContainer* pContainer = NULL;
				TESForm* pBaseForm = pContainerRef->baseForm;
				if (pBaseForm)
					pContainer = DYNAMIC_CAST(pBaseForm, TESForm, TESContainer);

				// Declare the container to receive the forms
				ExtraContainerArray formContainer(&result);
				ExtraContainerInfo info(pXContainerChanges ? pXContainerChanges->data->objList : NULL);
				// first walk the base container
				if (pContainer) {
					// Fill the container
					ExtraContainerFiller formFiller(info, &formContainer);
					pContainer->Visit(formFiller);
				}

				// Fill the container with remaining forms
				info.GetRemainingForms(&formContainer);
			}
		}

		return result;
	}
};

#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

void papyrusObjectReference::RegisterFuncs(VMClassRegistry* registry)
{
	registry->RegisterFunction(
		new NativeFunction0<TESObjectREFR, UInt32>("GetNumItems", "ObjectReference", papyrusObjectReference::GetNumItems, registry));

	registry->RegisterFunction(
		new NativeFunction1<TESObjectREFR, TESForm*, UInt32>("GetNthForm", "ObjectReference", papyrusObjectReference::GetNthForm, registry));

	registry->RegisterFunction(
		new NativeFunction0<TESObjectREFR, float>("GetTotalItemWeight", "ObjectReference", papyrusObjectReference::GetTotalItemWeight, registry));

	registry->RegisterFunction(
		new NativeFunction0<TESObjectREFR, float>("GetTotalArmorWeight", "ObjectReference", papyrusObjectReference::GetTotalArmorWeight, registry));

	registry->RegisterFunction(
		new NativeFunction0<TESObjectREFR, bool>("IsHarvested", "ObjectReference", papyrusObjectReference::IsHarvested, registry));

	registry->RegisterFunction(
		new NativeFunction1<TESObjectREFR, void, bool>("SetHarvested", "ObjectReference", papyrusObjectReference::SetHarvested, registry));

	// Item modifications, Tempering/Charges
	registry->RegisterFunction(
		new NativeFunction1<TESObjectREFR, void, float>("SetItemHealthPercent", "ObjectReference", papyrusObjectReference::SetItemHealthPercent, registry));

	registry->RegisterFunction(
		new NativeFunction0<TESObjectREFR, float>("GetItemMaxCharge", "ObjectReference", papyrusObjectReference::GetItemMaxCharge, registry));

	registry->RegisterFunction(
		new NativeFunction1<TESObjectREFR, void, float>("SetItemMaxCharge", "ObjectReference", papyrusObjectReference::SetItemMaxCharge, registry));

	registry->RegisterFunction(
		new NativeFunction0<TESObjectREFR, float>("GetItemCharge", "ObjectReference", papyrusObjectReference::GetItemCharge, registry));

	registry->RegisterFunction(
		new NativeFunction1<TESObjectREFR, void, float>("SetItemCharge", "ObjectReference", papyrusObjectReference::SetItemCharge, registry));

	registry->RegisterFunction(
		new NativeFunction0<TESObjectREFR, EnchantmentItem*>("GetEnchantment", "ObjectReference", papyrusObjectReference::GetEnchantment, registry));

	registry->RegisterFunction(
		new NativeFunction2<TESObjectREFR, void, EnchantmentItem*, float>("SetEnchantment", "ObjectReference", papyrusObjectReference::SetEnchantment, registry));
	
	registry->RegisterFunction(
		new NativeFunction5<TESObjectREFR, void, float, VMArray<EffectSetting*>, VMArray<float>, VMArray<UInt32>, VMArray<UInt32>>("CreateEnchantment", "ObjectReference", papyrusObjectReference::CreateEnchantment, registry));

	registry->RegisterFunction(
		new NativeFunction0<TESObjectREFR, AlchemyItem*>("GetPoison", "ObjectReference", papyrusObjectReference::GetPoison, registry));


	registry->RegisterFunction(
		new NativeFunction0<TESObjectREFR, BSFixedString>("GetDisplayName", "ObjectReference", papyrusObjectReference::GetDisplayName, registry));

	registry->RegisterFunction(
		new NativeFunction2<TESObjectREFR, bool, BSFixedString, bool>("SetDisplayName", "ObjectReference", papyrusObjectReference::SetDisplayName, registry));

	registry->RegisterFunction(
		new NativeFunction0<TESObjectREFR, void>("ResetInventory", "ObjectReference", papyrusObjectReference::ResetInventory, registry));

	registry->RegisterFunction(
		new NativeFunction0<TESObjectREFR, bool>("IsOffLimits", "ObjectReference", papyrusObjectReference::IsOffLimits, registry));

	registry->RegisterFunction(
		new NativeFunction0<TESObjectREFR, TESObjectREFR*>("GetEnableParent", "ObjectReference", papyrusObjectReference::GetEnableParent, registry));

	registry->RegisterFunction(
		new NativeFunction0<TESObjectREFR, UInt32>("GetNumReferenceAliases", "ObjectReference", papyrusObjectReference::GetNumReferenceAliases, registry));

	registry->RegisterFunction(
		new NativeFunction1<TESObjectREFR, BGSRefAlias*, UInt32>("GetNthReferenceAlias", "ObjectReference", papyrusObjectReference::GetNthReferenceAlias, registry));

	registry->RegisterFunction(
		new NativeFunction0<TESObjectREFR, VMResultArray<BGSRefAlias*>>("GetReferenceAliases", "ObjectReference", papyrusObjectReference::GetReferenceAliases, registry));

	registry->RegisterFunction(
		new NativeFunction1<TESObjectREFR, void, BGSListForm*>("GetAllForms", "ObjectReference", papyrusObjectReference::GetAllForms, registry));

	registry->RegisterFunction(
		new NativeFunction0<TESObjectREFR, VMResultArray<TESForm*>>("GetContainerForms", "ObjectReference", papyrusObjectReference::GetContainerForms, registry));
}
