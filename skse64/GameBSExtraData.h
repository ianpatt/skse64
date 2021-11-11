#pragma once

#include "GameTypes.h"
#include "skse64_common/Utilities.h"

class ExtraTextDisplayData;

enum ExtraDataType
{
	kExtraData_Havok			= 0x1,
	kExtraData_Cell3D			= 0x2,
	kExtraData_CellWaterType	= 0x3,
	kExtraData_RegionList		= 0x4,
	kExtraData_SeenData			= 0x5,
	kExtraData_EditorID			= 0x6,
	kExtraData_CellMusicType	= 0x7,
	kExtraData_CellSkyRegion	= 0x8,
 	kExtraData_ProcessMiddleLow	= 0x9,
 	kExtraData_DetachTime		= 0xA,
 	kExtraData_PersistentCell	= 0xB,
	// ??						= 0xC,
 	kExtraData_Action			= 0xD,
 	kExtraData_StartingPosition	= 0xE,
	// ??						= 0xF,
 	kExtraData_AnimGraphManager	= 0x10,
	// ??						= 0x11,
 	kExtraData_UsedMarkers		= 0x12,
 	kExtraData_DistantData		= 0x13,
 	kExtraData_RagDollData		= 0x14,
 	kExtraData_ContainerChanges	= 0x15,
 	kExtraData_Worn				= 0x16,
 	kExtraData_WornLeft			= 0x17,
 	kExtraData_PackageStartLocation	= 0x18,
 	kExtraData_Package			= 0x19,
 	kExtraData_TresPassPackage	= 0x1A,
 	kExtraData_RunOncePacks		= 0x1B,
 	kExtraData_ReferenceHandle	= 0x1C,
 	kExtraData_Follower			= 0x1D,
 	kExtraData_LevCreaModifier	= 0x1E,
 	kExtraData_Ghost			= 0x1F,
 	kExtraData_OriginalReference	= 0x20,
 	kExtraData_Ownership		= 0x21,
 	kExtraData_Global			= 0x22,
 	kExtraData_Rank				= 0x23,
 	kExtraData_Count			= 0x24,
 	kExtraData_Health			= 0x25,
	// ??						= 0x26,
 	kExtraData_TimeLeft			= 0x27,
 	kExtraData_Charge			= 0x28,
 	kExtraData_Light			= 0x29,
 	kExtraData_Lock				= 0x2A,
 	kExtraData_Teleport			= 0x2B,
 	kExtraData_MapMarker		= 0x2C,
 	kExtraData_LeveledCreature	= 0x2D,
 	kExtraData_LeveledItem		= 0x2E,
 	kExtraData_Scale			= 0x2F,
 	kExtraData_Seed				= 0x30,
 	kExtraData_MagicCaster		= 0x31, // ExtraMagicLight??
 	// ??						= 0x32, 
 	// ??						= 0x33, 
 	kExtraData_PlayerCrimeList	= 0x34,
	// ??						= 0x35,
 	kExtraData_EnableStateParent	= 0x36,
 	kExtraData_EnableStateChildren	= 0x37,
 	kExtraData_ItemDropper		= 0x38,
 	kExtraData_DroppedItemList	= 0x39,
 	kExtraData_RandomTeleportMarker = 0x3A,
	//??						= 0x3B
 	kExtraData_SavedHavokData	= 0x3C,
 	kExtraData_CannotWear		= 0x3D,
 	kExtraData_Poison			= 0x3E,
	//??						= 0x3F
 	kExtraData_LastFinishedSequence	= 0x40,
 	kExtraData_SavedAnimation	= 0x41,
 	kExtraData_NorthRotation	= 0x42,
 	kExtraData_SpawnContainer	= 0x43,
 	kExtraData_FriendHits		= 0x44,
 	kExtraData_HeadingTarget	= 0x45,
	//??						= 0x46
 	kExtraData_RefractionProperty	= 0x47,
 	kExtraData_StartingWorldOrCell	= 0x48,
 	kExtraData_Hotkey			= 0x49,
	//??						0x4A
 	kExtraData_EditiorRefMoveData	= 0x4B,
 	kExtraData_InfoGeneralTopic	= 0x4C,
 	kExtraData_HasNoRumors		= 0x4D,
 	kExtraData_Sound			= 0x4E,
 	kExtraData_TerminalState	= 0x4F,
 	kExtraData_LinkedRef		= 0x50,
 	kExtraData_LinkedRefChildren	= 0x51,
 	kExtraData_ActivateRef		= 0x52,
 	kExtraData_ActivateRefChildren	=0x53,
 	kExtraData_CanTalkToPlayer	= 0x54,
 	kExtraData_ObjectHealth		= 0x55,
 	kExtraData_CellImageSpace	= 0x56,
 	kExtraData_NavMeshPortal	= 0x57,
 	kExtraData_ModelSwap		= 0x58,
 	kExtraData_Radius			= 0x59,
	//??						= 0x5A,
 	kExtraData_FactionChanges	= 0x5B,
 	kExtraData_DismemberedLimbs	= 0x5C,
 	kExtraData_ActorCause		= 0x5D,
 	kExtraData_MultiBound		= 0x5E,
 	kExtraData_MultiBoundData	= 0x5F,
 	kExtraData_MultiBoundRef	= 0x60,
 	kExtraData_ReflectedRefs	= 0x61,
 	kExtraData_ReflectorRefs	= 0x62,
 	kExtraData_EmittanceSource	= 0x63,
 	kExtraData_RadioData		= 0x64,
 	kExtraData_CombatStyle		= 0x65,
	//??						= 0x66,
 	kExtraData_Primitive		= 0x67,
 	kExtraData_OpenCloseActivateRef	= 0x68,
 	kExtraData_AnimNoteReceiver	= 0x69,
 	kExtraData_Ammo				= 0x6A,
 	kExtraData_PatrolRefData	= 0x6B,
 	kExtraData_PackageData		= 0x6C,
 	kExtraData_OcclusionShape	= 0x6D,
 	kExtraData_CollisionData	= 0x6E,
 	kExtraData_SayTopicInfoOnceADay	= 0x6F,
 	kExtraData_EncounterZone	= 0x70,
 	kExtraData_SayTopicInfo		= 0x71,
 	kExtraData_OcclusionPlaneRefData	= 0x72,
 	kExtraData_PortalRefData	= 0x73,
 	kExtraData_Portal			= 0x74,
 	kExtraData_Room				= 0x75,
 	kExtraData_HealthPerc		= 0x76,
 	kExtraData_RoomRefData		= 0x77,
 	kExtraData_GuardedRefData	= 0x78,
 	kExtraData_CreatureAwakeSound	= 0x79,
	//??						= 0x7A,
 	kExtraData_Horse			= 0x7B,
 	kExtraData_IgnoredBySandbox	= 0x7C,
 	kExtraData_CellAcousticSpace	= 0x7D,
 	kExtraData_ReservedMarkers	= 0x7E,
 	kExtraData_WeaponIdleSound	= 0x7F,
 	kExtraData_WaterLightRefs	= 0x80,
 	kExtraData_LitWaterRefs		= 0x81,
 	kExtraData_WeaponAttackSound	= 0x82,
 	kExtraData_ActivateLoopSound	= 0x83,
 	kExtraData_PatrolRefInUseData	= 0x84,
 	kExtraData_AshPileRef		= 0x85,
	//??						= 0x86
 	kExtraData_FollowerSwimBreadcrumbs	= 0x87,
 	kExtraData_AliasInstanceArray	= 0x88,
 	kExtraData_Location			= 0x89,
	//??						= 0x8A,
 	kExtraData_LocationRefType	= 0x8B,
 	kExtraData_PromotedRef		= 0x8C,
	//??						= 0x8D,
 	kExtraData_OutfitItem		= 0x8E,
	//??						= 0x8F,
 	kExtraData_LeveledItemBase	= 0x90,
 	kExtraData_LightData		= 0x91,
 	kExtraData_SceneData		= 0x92,
 	kExtraData_BadPosition		= 0x93,
 	kExtraData_HeadTrackingWeight	= 0x94,
 	kExtraData_FromAlias		= 0x95,
 	kExtraData_ShouldWear		= 0x96,
 	kExtraData_FavorCost		= 0x97,
 	kExtraData_AttachedArrows3D	= 0x98,
 	kExtraData_TextDisplayData	= 0x99,
 	kExtraData_AlphaCutoff		= 0x9A,
 	kExtraData_Enchantment		= 0x9B,
 	kExtraData_Soul				= 0x9C,
 	kExtraData_ForcedTarget		= 0x9D,
	//??						= 0x9E,
 	kExtraData_UniqueID			= 0x9F,
 	kExtraData_Flags			= 0xA0,
 	kExtraData_RefrPath			= 0xA1,
 	kExtraData_DecalGroup		= 0xA2,
 	kExtraData_LockList			= 0xA3,
 	kExtraData_ForcedLandingMarker	= 0xA4,
 	kExtraData_LargeRefOwnerCells	= 0xA5,
 	kExtraData_CellWaterEnvMap	= 0xA6,
 	kExtraData_CellGrassData	= 0xA7,
 	kExtraData_TeleportName		= 0xA8,
 	kExtraData_Interaction		= 0xA9,
 	kExtraData_WaterData		= 0xAA,
 	kExtraData_WaterCurrentZoneData	= 0xAB,
 	kExtraData_AttachRef		= 0xAC,
 	kExtraData_AttachRefChildren	= 0xAD,
 	kExtraData_GroupConstraint	= 0xAE,
 	kExtraData_ScriptedAnimDependence = 0xAF,
 	kExtraData_CachedScale		= 0xB0,
 	kExtraData_RaceData			= 0xB1,
 	kExtraData_GIDBuffer		= 0xB2,
 	kExtraData_MissingRefIDs	= 0xB3
};

// 10
class BSExtraData
{
public:
	BSExtraData();
	virtual ~BSExtraData();
	virtual UInt32 GetType(void);

	BSExtraData* next;		// 08
	//UInt8 type;				//	10
	//UInt8 padding[3];		
	static BSExtraData* Create(UInt32 size, uintptr_t vtbl);
};
//STATIC_ASSERT(sizeof(BSExtraData) == 0x10);

// 18
class BaseExtraList
{
public:
	BaseExtraList();
	~BaseExtraList();
	
	struct PresenceBitfield
	{
		bool HasType(UInt32 type) const
		{
			UInt32 index = (type >> 3);
			UInt8 bitMask = 1 << (type % 8);
			return (bits[index] & bitMask) != 0;
		}

		UInt8	bits[0x18];
	};

	bool HasType(UInt32 type)
	{
		BSReadLocker locker(&m_lock);
		return (m_presence) ? m_presence->HasType(type) : false;
	}

	void MarkType(UInt32 type, bool bCleared)
	{
		UInt32 index = (type >> 3);
		UInt8 bitMask = 1 << (type % 8);
		UInt8& flag = m_presence->bits[index];
		if (bCleared) {
			flag &= ~bitMask;
		} else {
			flag |= bitMask;
		}
	}

	bool Remove(UInt8 type, BSExtraData* toRemove);
	bool Add(UInt8 type, BSExtraData* toAdd);

	bool CheckContainerExtraData(bool isEquipped);

	const char * GetDisplayName(TESForm * type);

	BSExtraData* GetByType(UInt32 type);
	BSExtraData			* m_data;		// 00
	PresenceBitfield	* m_presence;	// 08

	// These added in SE
	BSReadWriteLock		m_lock;

private:
	MEMBER_FN_PREFIX(BaseExtraList);
	// 
	DEFINE_MEMBER_FN(CheckContainerExtraData_Internal, bool, 0x001195B0, bool isEquipped);
	// This also does some internal ReferenceHandle lookup
	// 
	DEFINE_MEMBER_FN(GetExtraTextDisplayData_Internal, ExtraTextDisplayData*, 0x0011D390);
};

typedef tList<BaseExtraList> ExtendDataList;

STATIC_ASSERT(sizeof(BaseExtraList) == 0x18);
