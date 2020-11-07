#include "PapyrusReferenceAlias.h"

#include "GameForms.h"
#include "GameExtraData.h"
#include "GameFormComponents.h"
#include "GameRTTI.h"

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

namespace papyrusReferenceAlias
{
	bool ForceRefToWornItem(BGSRefAlias * alias, TESObjectREFR * reference, UInt32 mask)
	{
		if(!reference || !alias)
			return false;

		MatchBySlot matcher(mask);
		ExtraContainerChanges* pContainerChanges = static_cast<ExtraContainerChanges*>(reference->extraData.GetByType(kExtraData_ContainerChanges));
		if (pContainerChanges) {
			EquipData eqD = pContainerChanges->FindEquipped(matcher);
			NiPointer<TESObjectREFR> foundReference;
			ExtraReferenceHandle* xReference = static_cast<ExtraReferenceHandle*>(eqD.pExtraData->GetByType(kExtraData_ReferenceHandle));
			if(xReference) { // Persistent Ref exists, acquire it
				UInt32 handleIn = xReference->handle;
				LookupREFRByHandle(handleIn, foundReference);
				_MESSAGE("Found Reference %08X", foundReference->formID);
			} else { // No Persistent Ref, Create One
				/*IFormFactory * factory = IFormFactory::GetFactoryForType(TESObjectREFR::kTypeID);
				if(factory) {
					foundReference = (TESObjectREFR*)factory->Create();
					if(foundReference) {
						foundReference->Init();
						foundReference->baseForm = eqD.pForm;
					}
				}
				if(foundReference) {
					ExtraReferenceHandle* newHandle = ExtraReferenceHandle::Create();
					UInt32 handleOut = 0;
					CreateRefHandleByREFR(&handleOut, foundReference);
					newHandle->handle = handleOut;
					eqD.pExtraData->Add(kExtraData_ReferenceHandle, newHandle);
					_MESSAGE("Created Reference %08X", foundReference->formID);
				}*/
			}

			if(foundReference)
			{
				_MESSAGE("Forced Reference to %08X", foundReference->formID);
				CALL_MEMBER_FN(alias->owner, ForceRefTo)(alias->aliasId, foundReference);
				return true;
			}
			else
			{
				_MESSAGE("Forced Reference to <not found>");
			}
		}

		return false;
	}
}


#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

void papyrusReferenceAlias::RegisterFuncs(VMClassRegistry* registry)
{
	registry->RegisterFunction(
		new NativeFunction2<BGSRefAlias, bool, TESObjectREFR*, UInt32> ("ForceRefToWornItem", "ReferenceAlias", papyrusReferenceAlias::ForceRefToWornItem, registry));
}
