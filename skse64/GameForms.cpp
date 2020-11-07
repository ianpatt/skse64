#include "GameForms.h"
#include "GameObjects.h"

RelocAddr <_LookupFormByID> LookupFormByID(0x00194230);

BGSDefaultObjectManager *BGSDefaultObjectManager::GetSingleton(void)
{
	// 81542B44FD6902A56B6B1464C37C41C529E9FD2A+31CB
	static RelocPtr<BGSDefaultObjectManager> g_BGSDefaultObjectManager(0x01EBD710);
	return g_BGSDefaultObjectManager;
}

void TESForm::CopyFromEx(TESForm * rhsForm)
{
	if(!rhsForm || rhsForm->formType != formType)
		return;

	switch(formType)
	{
		case kFormType_CombatStyle:
		{
			TESCombatStyle	* lhs = (TESCombatStyle *)this;
			TESCombatStyle	* rhs = (TESCombatStyle *)rhsForm;

			lhs->general =				rhs->general;
			lhs->melee =				rhs->melee;
			lhs->closeRange =			rhs->closeRange;
			lhs->longRange =			rhs->longRange;
			lhs->flight =				rhs->flight;
			lhs->flags =				rhs->flags;
		}
		break;

		// Untested, probably shouldn't use this
		case kFormType_Armor:
		{
			TESObjectARMO	* lhs = (TESObjectARMO*)this;
			TESObjectARMO	* rhs = (TESObjectARMO*)rhsForm;

			lhs->fullName.CopyFromBase(&rhs->fullName);
			lhs->race.CopyFromBase(&rhs->race);
			lhs->enchantable.CopyFromBase(&rhs->enchantable);
			lhs->value.CopyFromBase(&rhs->value);
			lhs->weight.CopyFromBase(&rhs->weight);
			lhs->destructible.CopyFromBase(&rhs->destructible);
			lhs->pickupSounds.CopyFromBase(&rhs->pickupSounds);
			lhs->bipedModel.CopyFromBase(&rhs->bipedModel);
			lhs->equipType.CopyFromBase(&rhs->equipType);
			lhs->bipedObject.CopyFromBase(&rhs->bipedObject);
			lhs->blockBash.CopyFromBase(&rhs->blockBash);
			lhs->keyword.CopyFromBase(&rhs->keyword);
			lhs->description.CopyFromBase(&rhs->description);

			lhs->armorValTimes100	= rhs->armorValTimes100;
			lhs->armorAddons.CopyFrom(&rhs->armorAddons);
			lhs->templateArmor = rhs->templateArmor;
		}
		break;

		default:
			// unsupported
			break;
	}
}

float GetFormWeight(TESForm *form)
{
	// 27090A4B7BDF5406F9A6871190673EC666A22195+38
	static RelocAddr<uintptr_t> GetFormWeight_Address(0x001A1730);

	return ((_GetFormWeight)(GetFormWeight_Address.GetUIntPtr())) (form);
}

UInt32 BGSListForm::GetSize()
{
	UInt32 totalSize = forms.count;
	if(addedForms) {
		totalSize += addedForms->count;
	}

	return totalSize;
}

bool BGSListForm::Visit(BGSListForm::Visitor & visitor)
{
	// Base Added Forms
	for(UInt32 i = 0; i < forms.count; i++)
	{
		TESForm* childForm = NULL;
		if(forms.GetNthItem(i, childForm))
		{
			if(visitor.Accept(childForm))
				return true;
		}
	}

	// Script Added Forms
	if(addedForms) {
		for(int i = 0; i < addedForms->count; i++) {
			UInt32 formid = 0;
			addedForms->GetNthItem(i, formid);
			TESForm* childForm = LookupFormByID(formid);
			if(visitor.Accept(childForm))
				return true;
		}
	}

	return false;
}

bool TESPackage::IsExtraType()
{
	switch(type)
	{
	case kPackageType_Activate:
	case kPackageType_Alarm:
	case kPackageType_Flee:
	case kPackageType_Trespass:
	case kPackageType_Spectator:
	case kPackageType_ReactToDead:
	case kPackageType_DoNothing:
	case kPackageType_InGameDialogue:
	case kPackageType_Surface:
	case kPackageType_AvoidPlayer:
	case kPackageType_ReactToDestroyedObject:
	case kPackageType_ReactToGrenadeOrMine:
	case kPackageType_StealWarning:
	case kPackageType_PickPocketWarning:
	case kPackageType_MovementBlocked:
	case kPackageType_Unk37:
	case kPackageType_Unk38:
		return true;
		break;
	}

	return false;
}
