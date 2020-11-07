#include "GameBSExtraData.h"
#include "GameExtraData.h"
#include "GameRTTI.h"

extern const void * RTTIForExtraType[0xB4] = 
{
	NULL,						// 0x0
	RTTI_ExtraHavok,			// 0x1,
	RTTI_ExtraCell3D,			// 0x2,
	RTTI_ExtraCellWaterType,	// 0x3,
	RTTI_ExtraRegionList,		// 0x4,
	RTTI_ExtraSeenData,			// 0x5,
	RTTI_ExtraEditorID,			// 0x6,
	RTTI_ExtraCellMusicType,	// 0x7,
	RTTI_ExtraCellSkyRegion,	// 0x8,
	RTTI_ExtraProcessMiddleLow,	// 0x9,
	RTTI_ExtraDetachTime,		// 0xA,
	RTTI_ExtraPersistentCell,	// 0xB,
	NULL,						// 0xC,
	RTTI_ExtraAction,			// 0xD,
	RTTI_ExtraStartingPosition,	// 0xE,
	NULL,						// 0xF,
	RTTI_ExtraAnimGraphManager,	// 0x10,
	NULL,						// 0x11,
	RTTI_ExtraUsedMarkers,		// 0x12,
	RTTI_ExtraDistantData,		// 0x13,
	RTTI_ExtraRagDollData,		// 0x14,
	RTTI_ExtraContainerChanges,	// 0x15,
	RTTI_ExtraWorn,				// 0x16,
	RTTI_ExtraWornLeft,			// 0x17,
	RTTI_ExtraPackageStartLocation,	// 0x18,
	RTTI_ExtraPackage,			// 0x19,
	RTTI_ExtraTresPassPackage,	// 0x1A,
	RTTI_ExtraRunOncePacks,		// 0x1B,
	RTTI_ExtraReferenceHandle,	// 0x1C,
	RTTI_ExtraFollower,			// 0x1D,
	RTTI_ExtraLevCreaModifier,	// 0x1E,
	RTTI_ExtraGhost,			// 0x1F,
	RTTI_ExtraOriginalReference,	// 0x20,
	RTTI_ExtraOwnership,		// 0x21,
	RTTI_ExtraGlobal,			// 0x22,
	RTTI_ExtraRank,				// 0x23,
	RTTI_ExtraCount,			// 0x24,
	RTTI_ExtraHealth,			// 0x25,
	NULL,						// 0x26,
	RTTI_ExtraTimeLeft,			// 0x27,
	RTTI_ExtraCharge,			// 0x28,
	RTTI_ExtraLight,			// 0x29,
	RTTI_ExtraLock,				// 0x2A,
	RTTI_ExtraTeleport,			// 0x2B,
	RTTI_ExtraMapMarker,		// 0x2C,
	RTTI_ExtraLeveledCreature,	// 0x2D,
	RTTI_ExtraLeveledItem,		// 0x2E,
	RTTI_ExtraScale,			// 0x2F,
	NULL,						// 0x30,	was ExtraSeed, removed in 1.7.7.0
	NULL,						// 0x31,
	NULL,						// 0x32,
	NULL,						// 0x33,
	RTTI_ExtraPlayerCrimeList,	// 0x34,
	NULL,						// 0x35
	RTTI_ExtraEnableStateParent,	// 0x36,
	RTTI_ExtraEnableStateChildren,	// 0x37,
	RTTI_ExtraItemDropper,		// 0x38,
	RTTI_ExtraDroppedItemList,	// 0x39,
	RTTI_ExtraRandomTeleportMarker, // 0x3A,
	NULL,						// 0x3B
	RTTI_ExtraSavedHavokData,	// 0x3C,
	RTTI_ExtraCannotWear,		// 0x3D,
	RTTI_ExtraPoison,			// 0x3E,
	NULL,						// 0x3F
	RTTI_ExtraLastFinishedSequence,	// 0x40,
	RTTI_ExtraSavedAnimation,	// 0x41,
	RTTI_ExtraNorthRotation,	// 0x42,
	RTTI_ExtraSpawnContainer,	// 0x43,
	RTTI_ExtraFriendHits,		// 0x44,
	RTTI_ExtraHeadingTarget,	// 0x45,
	NULL,						// 0x46
	RTTI_ExtraRefractionProperty,	// 0x47,
	RTTI_ExtraStartingWorldOrCell,	// 0x48,
	RTTI_ExtraHotkey,			// 0x49,
	NULL,						// 0x4A
	RTTI_ExtraEditorRefMoveData,	// 0x4B,
	RTTI_ExtraInfoGeneralTopic,	// 0x4C,
	RTTI_ExtraHasNoRumors,		// 0x4D,
	RTTI_ExtraSound,			// 0x4E,
	RTTI_ExtraTerminalState,	// 0x4F,
	RTTI_ExtraLinkedRef,		// 0x50,
	RTTI_ExtraLinkedRefChildren,	// 0x51,
	RTTI_ExtraActivateRef,		// 0x52,
	RTTI_ExtraActivateRefChildren,	// 0x53,
	RTTI_ExtraCanTalkToPlayer,	// 0x54,
	RTTI_ExtraObjectHealth,		// 0x55,
	RTTI_ExtraCellImageSpace,	// 0x56,
	RTTI_ExtraNavMeshPortal,	// 0x57,
	RTTI_ExtraModelSwap,		// 0x58,
	RTTI_ExtraRadius,			// 0x59,
	NULL,						// 0x5A,
	RTTI_ExtraFactionChanges,	// 0x5B,
	RTTI_ExtraDismemberedLimbs,	// 0x5C,
	RTTI_ExtraActorCause,		// 0x5D,
	RTTI_ExtraMultiBound,		// 0x5E,
	NULL,						// 0x5F,	was ExtraMultiBoundData, removed in 1.7.7.0
	RTTI_ExtraMultiBoundRef,	// 0x60,
	RTTI_ExtraReflectedRefs,	// 0x61,
	RTTI_ExtraReflectorRefs,	// 0x62,
	RTTI_ExtraEmittanceSource,	// 0x63,
	NULL,						// 0x64,	was ExtraRadioData, removed in 1.7.7.0
	RTTI_ExtraCombatStyle,		// 0x65,
	NULL,						// 0x66,
	RTTI_ExtraPrimitive,		// 0x67,
	RTTI_ExtraOpenCloseActivateRef,	// 0x68,
	RTTI_ExtraAnimNoteReceiver,	// 0x69,
	RTTI_ExtraAmmo,				// 0x6A,
	RTTI_ExtraPatrolRefData,	// 0x6B,
	RTTI_ExtraPackageData,		// 0x6C,
	RTTI_ExtraOcclusionShape,	// 0x6D,
	RTTI_ExtraCollisionData,	// 0x6E,
	RTTI_ExtraSayTopicInfoOnceADay,	// 0x6F,
	RTTI_ExtraEncounterZone,	// 0x70,
	RTTI_ExtraSayToTopicInfo,		// 0x71,
	RTTI_ExtraOcclusionPlaneRefData,	// 0x72,
	RTTI_ExtraPortalRefData,	// 0x73,
	RTTI_ExtraPortal,			// 0x74,
	RTTI_ExtraRoom,				// 0x75,
	RTTI_ExtraHealthPerc,		// 0x76,
	RTTI_ExtraRoomRefData,		// 0x77,
	RTTI_ExtraGuardedRefData,	// 0x78,
	RTTI_ExtraCreatureAwakeSound,	// 0x79,
	NULL,						// 0x7A,
	RTTI_ExtraHorse,			// 0x7B,
	RTTI_ExtraIgnoredBySandbox,	// 0x7C,
	RTTI_ExtraCellAcousticSpace,	// 0x7D,
	RTTI_ExtraReservedMarkers,	// 0x7E,
	RTTI_ExtraWeaponIdleSound,	// 0x7F,
	RTTI_ExtraWaterLightRefs,	// 0x80,
	RTTI_ExtraLitWaterRefs,		// 0x81,
	RTTI_ExtraWeaponAttackSound,	// 0x82,
	RTTI_ExtraActivateLoopSound,	// 0x83,
	RTTI_ExtraPatrolRefInUseData,	// 0x84,
	RTTI_ExtraAshPileRef,		// 0x85,
	NULL,						// 0x86
	RTTI_ExtraFollowerSwimBreadcrumbs,	// 0x87,
	RTTI_ExtraAliasInstanceArray,	// 0x88,
	RTTI_ExtraLocation,			// 0x89,
	NULL,						// 0x8A,
	RTTI_ExtraLocationRefType,	// 0x8B,
	RTTI_ExtraPromotedRef,		// 0x8C,
	NULL,						// 0x8D,
	RTTI_ExtraOutfitItem,		// 0x8E,
	NULL,						// 0x8F,
	RTTI_ExtraLeveledItemBase,	// 0x90,
	RTTI_ExtraLightData,		// 0x91,
	RTTI_ExtraSceneData,		// 0x92,
	RTTI_ExtraBadPosition,		// 0x93,
	RTTI_ExtraHeadTrackingWeight,	// 0x94,
	RTTI_ExtraFromAlias,		// 0x95,
	RTTI_ExtraShouldWear,		// 0x96,
	RTTI_ExtraFavorCost,		// 0x97,
	RTTI_ExtraAttachedArrows3D,	// 0x98,
	RTTI_ExtraTextDisplayData,	// 0x99,
	RTTI_ExtraAlphaCutoff,		// 0x9A,
	RTTI_ExtraEnchantment,		// 0x9B,
	RTTI_ExtraSoul,				// 0x9C,
	RTTI_ExtraForcedTarget,		// 0x9D,
	NULL,						// 0x9E,
	RTTI_ExtraUniqueID,			// 0x9F,
	RTTI_ExtraFlags,			// 0xA0,
	RTTI_ExtraRefrPath,			// 0xA1,
	RTTI_ExtraDecalGroup,		// 0xA2,
	RTTI_ExtraLockList,			// 0xA3,
	RTTI_ExtraForcedLandingMarker,	// 0xA4,
	RTTI_ExtraLargeRefOwnerCells,	// 0xA5,
	RTTI_ExtraCellWaterEnvMap,	// 0xA6,
	RTTI_ExtraCellGrassData,	// 0xA7,
	RTTI_ExtraTeleportName,		// 0xA8,
	RTTI_ExtraInteraction,		// 0xA9,
	RTTI_ExtraWaterData,		// 0xAA,
	RTTI_ExtraWaterCurrentZoneData,	// 0xAB,
	RTTI_ExtraAttachRef,		// 0xAC,
	RTTI_ExtraAttachRefChildren,	// 0xAD,
	RTTI_ExtraGroupConstraint,	// 0xAE,
	RTTI_ExtraScriptedAnimDependence, // 0xAF,
	RTTI_ExtraCachedScale,		// 0xB0,
	RTTI_ExtraRaceData,			// 0xB1,
	RTTI_ExtraGIDBuffer,		// 0xB2,
	RTTI_ExtraMissingRefIDs,	// 0xB3
};


BSExtraData* BaseExtraList::GetByType(UInt32 type)
{
	BSReadLocker locker(&m_lock);
	if (!HasType(type)) return NULL;

	const void* rttiType = RTTIForExtraType[type];

	for(BSExtraData * traverse = m_data; traverse; traverse = traverse->next) {
		if (Runtime_DynamicCast((void*)(traverse), RTTI_BSExtraData, rttiType) != NULL) {
			return traverse;
		}
		//if(traverse->type == type)
		//	return traverse;
	}
	return NULL;
}

bool BaseExtraList::Remove(UInt8 type, BSExtraData* toRemove)
{
	BSWriteLocker locker(&m_lock);
	if (!toRemove) return false;

	if (HasType(type)) {
		bool bRemoved = false;
		if (m_data == toRemove) {
			m_data = m_data->next;
			bRemoved = true;
		}

		for (BSExtraData* traverse = m_data; traverse; traverse = traverse->next) {
			if (traverse->next == toRemove) {
				traverse->next = toRemove->next;
				bRemoved = true;
				break;
			}
		}
		if (bRemoved) {
			MarkType(type, true);
		}
		return true;
	}

	return false;
}

bool BaseExtraList::Add(UInt8 type, BSExtraData* toAdd)
{
	if (!toAdd || HasType(type)) return false;

	BSWriteLocker locker(&m_lock);
	BSExtraData* next = m_data;
	m_data = toAdd;
	toAdd->next = next;
	MarkType(type, false);
	return true;
}

bool BaseExtraList::CheckContainerExtraData(bool isEquipped)
{
	return CALL_MEMBER_FN(this, CheckContainerExtraData_Internal)(isEquipped);
}

const char * BaseExtraList::GetDisplayName(TESForm * type)
{
	float healthValue = 1.0;

	ExtraHealth* xHealth = static_cast<ExtraHealth*>(GetByType(kExtraData_Health));
	if(xHealth)
		healthValue = xHealth->health;

	ExtraTextDisplayData * xText = CALL_MEMBER_FN(this, GetExtraTextDisplayData_Internal)();
	if (!xText && healthValue != 1.0)
	{
		xText = ExtraTextDisplayData::Create();
		Add(kExtraData_TextDisplayData, xText);
	}

	if (xText)
		return xText->GenerateName(type, healthValue);
	else
		return NULL;
}

BSExtraData* BSExtraData::Create(UInt32 size, uintptr_t vtbl)
{
	void* memory = Heap_Allocate(size);
	memset(memory, 0, size);
	((uintptr_t *)memory)[0] = vtbl;
	BSExtraData* xData = (BSExtraData*)memory;
	return xData;
}
