#include "skse64/GameReferences.h"
#include "skse64/GameObjects.h"
#include "skse64/GameForms.h"
#include "skse64/GameExtraData.h"
#include "skse64/GameRTTI.h"
#include "skse64/GameData.h"
#include "skse64/NiNodes.h"

// 123ED1FD37DCAF9CBB5808F1DFB2B9B4E6D678DC+67
RelocAddr<_CreateRefHandleByREFR> CreateRefHandleByREFR(0x001320F0);
// D479D10F77326C177B3D5AE2C5738B0D39A1546A+5F
RelocAddr<_LookupREFRByHandle> LookupREFRByHandle(0x001328A0);
// B995A21984B048C2B4F61777E615FFFB2806F9B7+8C
RelocAddr<_LookupREFRObjectByHandle> LookupREFRObjectByHandle(0x001329D0);

// 123ED1FD37DCAF9CBB5808F1DFB2B9B4E6D678DC+4D
RelocPtr<UInt32> g_invalidRefHandle(0x01EBEABC);

RelocAddr<_MoveRefrToPosition> MoveRefrToPosition(0x009AE5C0);
RelocAddr<_PlaceAtMe_Native> PlaceAtMe_Native(0x009951F0);
RelocAddr<_AddItem_Native> AddItem_Native(0x00993100);

UInt32 GetOrCreateRefrHandle(TESObjectREFR* ref)
{
	UInt32 invalidRefHandleVal = *(g_invalidRefHandle.GetPtr());
	if (ref == NULL)
		return invalidRefHandleVal;

	ExtraReferenceHandle* xRefHandle = static_cast<ExtraReferenceHandle*>(ref->extraData.GetByType(kExtraData_ReferenceHandle));

	UInt32 result = xRefHandle != NULL ?
		xRefHandle->handle :
		invalidRefHandleVal;

	if (result == invalidRefHandleVal)
		result = ref->CreateRefHandle();

	return result;
}

UInt32 TESObjectREFR::CreateRefHandle(void)
{
	if (handleRefObject.GetRefCount() > 0)
	{
		UInt32 refHandle = 0;
		CreateRefHandleByREFR(refHandle, this);
		return refHandle;
	}
	else
	{
		return *g_invalidRefHandle;
	}
}

void TESObjectREFR::IncRef()
{
	handleRefObject.IncRef();
}

void TESObjectREFR::DecRef()
{
	handleRefObject.DecRef();
}

TESForm * Actor::GetEquippedObject(bool abLeftHand)
{
	if(!processManager) 
		return NULL;

	if(abLeftHand)
		return processManager->equippedObject[ActorProcessManager::kEquippedHand_Left];
	else
		return processManager->equippedObject[ActorProcessManager::kEquippedHand_Right];

	return NULL;
}

TintMask * PlayerCharacter::GetOverlayTintMask(TintMask * original)
{
	SInt32 curIndex = -1;
	if(!overlayTintMasks)
		return NULL;

	TintMask * foundMask;
	for(UInt32 i = 0; i < tintMasks.count; i++)
	{
		tintMasks.GetNthItem(i, foundMask);
		if(foundMask == original) {
			curIndex = i;
			break;
		}
	}

	overlayTintMasks->GetNthItem(curIndex, foundMask);
	if(foundMask)
		return foundMask;

	return NULL;
}

void Actor::UpdateHairColor()
{
	TESNPC* npc = DYNAMIC_CAST(baseForm, TESForm, TESNPC);
	if(npc && npc->headData) {
		BGSColorForm * hairColor = npc->headData->hairColor; // Non-player actors won't have this
		if(hairColor) {
			NiColorA val;
			val.r = hairColor->color.red / 128.0;
			val.g = hairColor->color.green / 128.0;
			val.b = hairColor->color.blue / 128.0;
			NiColorA * color = &val;

			NiNode * model = GetNiRootNode(0);
			if(model) {
				UpdateModelHair(model, &color);
			}
		}
	}
}

void Actor::UpdateSkinColor()
{
	TESNPC* npc = DYNAMIC_CAST(baseForm, TESForm, TESNPC);
	if(npc) {
		NiColorA val;
		val.r = npc->color.red / 255.0;
		val.g = npc->color.green / 255.0;
		val.b = npc->color.blue / 255.0;
		NiColorA * color = &val;

		NiNode * thirdPerson = GetNiRootNode(0);
		NiNode * firstPerson = GetNiRootNode(1);
		if(thirdPerson) {
			UpdateModelSkin(thirdPerson, &color); // Update for 3rd Person
		}
		if(firstPerson) {
			UpdateModelSkin(firstPerson, &color); // Update for 1st Person
		}
	}
}

bool Actor::VisitFactions(FactionVisitor & visitor)
{
	TESNPC* npc = DYNAMIC_CAST(baseForm, TESForm, TESNPC);
	if(npc) {
		for(UInt32 i = 0; i < npc->actorData.factions.count; i++)
		{
			TESActorBaseData::FactionInfo info;
			npc->actorData.factions.GetNthItem(i, info);
			if(visitor.Accept(info.faction, info.rank))
				return true;
		}

		ExtraFactionChanges* pFactionChanges = static_cast<ExtraFactionChanges*>(extraData.GetByType(kExtraData_FactionChanges));
		if (pFactionChanges) {
			for(UInt32 i = 0; i < pFactionChanges->factions.count; i++)
			{
				ExtraFactionChanges::FactionInfo info;
				pFactionChanges->factions.GetNthItem(i, info);
				if(visitor.Accept(info.faction, info.rank))
					return true;
			}
		}
	}

	return false;
}

RelocPtr<CrosshairRefHandleHolder*> g_crosshairRefHandleHolder(0x02F011D0);

CrosshairRefHandleHolder * CrosshairRefHandleHolder::GetSingleton(void)
{
	return *(g_crosshairRefHandleHolder.GetPtr());
}
