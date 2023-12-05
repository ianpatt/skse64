#pragma once

#include "GameBSExtraData.h"
#include "GameReferences.h"
#include "GameAPI.h"
#include "GameObjects.h"

//	ExtraHavok
//	ExtraCell3D
//	ExtraCellWaterType
//	ExtraRegionList
//	ExtraSeenData
//	ExtraEditorID
//	ExtraCellMusicType
//	ExtraCellSkyRegion
//	ExtraProcessMiddleLow
//	ExtraDetachTime

// 18
class ExtraPersistentCell : public BSExtraData
{
public:
	ExtraPersistentCell();
	virtual ~ExtraPersistentCell();

	TESObjectCELL* cell;	// 10	
};
//	ExtraAction
//	ExtraStartingPosition
//	ExtraAnimGraphManager
//	ExtraUsedMarkers
//	ExtraDistantData
//	ExtraRagDollData
class ExtraHotkey;

class InventoryEntryData
{
public:
	struct EquipData
	{
		SInt32			itemCount;

		BaseExtraList*	itemExtraList;

		BaseExtraList*	wornExtraList;
		BaseExtraList*	wornLeftExtraList;

		bool			isItemWorn;
		bool			isItemWornLeft;
		bool			isTypeWorn;
		bool			isTypeWornLeft;

		EquipData();
	};

	InventoryEntryData(TESForm * item, UInt32 count);

	TESForm* type;
	ExtendDataList* extendDataList;
	SInt32 countDelta;

	// Heap allocated
	static InventoryEntryData * Create(TESForm * item, UInt32 count);
	void Delete(void);

	void GetExtraWornBaseLists(BaseExtraList ** pWornBaseListOut, BaseExtraList ** pWornLeftBaseListOut) const;
	void GetEquipItemData(EquipData& stateOut, SInt32 itemId, SInt32 baseCount) const;

	MEMBER_FN_PREFIX(InventoryEntryData);
	DEFINE_MEMBER_FN(GenerateName, const char *, 0x00223C90);
	DEFINE_MEMBER_FN(GetValue, SInt32, 0x00222FE0);
	DEFINE_MEMBER_FN(IsOwnedBy, bool, 0x00223DD0, TESForm * actor, bool unk1);
	DEFINE_MEMBER_FN(GetSoulLevel, UInt32, 0x00223160);
};

typedef tList<InventoryEntryData> EntryDataList;

class ExtraContainerChanges : public BSExtraData
{
public:
	ExtraContainerChanges();
	virtual ~ExtraContainerChanges();	

	class Data
	{
	public:
		EntryDataList*	objList;
		TESObjectREFR*	owner;
		float			totalWeight;
		float			armorWeight;

		InventoryEntryData * FindItemEntry(TESForm * item) const;

		// Allocate new entry data as a merge between base container data and extra data
		// Uses BaseExtraList*'s from original extra data and combined count
		InventoryEntryData * CreateEquipEntryData(TESForm * item);

		void GetEquipItemData(InventoryEntryData::EquipData& stateOut, TESForm * item, SInt32 itemId) const;

		MEMBER_FN_PREFIX(Data);
		DEFINE_MEMBER_FN(SetUniqueID, void, 0x00239A80, BaseExtraList* itemList, TESForm * oldForm, TESForm * newForm);
		DEFINE_MEMBER_FN(TransferItemUID, void, 0x00239D30, BaseExtraList * extraList, TESForm * oldForm, TESForm * newForm, UInt32 unk1);
	};

	Data * data;

	struct FoundEquipData {
		TESForm* pForm;
		BaseExtraList* pExtraData;
	};

	struct FoundHotkeyData {
		TESForm* pForm;
		ExtraHotkey * pHotkey;
	};

	FoundEquipData	FindEquipped(FormMatcher& matcher, bool isWorn = true, bool isWornLeft = true) const;
	FoundHotkeyData	FindHotkey(SInt32 hotkey) const;
	FoundHotkeyData	FindHotkey(TESForm * form) const;
};
typedef ExtraContainerChanges::FoundEquipData EquipData;
typedef ExtraContainerChanges::FoundHotkeyData HotkeyData;
 
// 10
 class ExtraWorn : public BSExtraData
{
public:
	ExtraWorn();
	virtual ~ExtraWorn();
};

 // 10
class ExtraWornLeft : public BSExtraData
{
public:
	ExtraWornLeft();
	virtual ~ExtraWornLeft();
};
 //	ExtraPackageStartLocation

// 28
class ExtraPackage : public BSExtraData
{
public:
	ExtraPackage();
	virtual ~ExtraPackage();

	TESPackage	* currentPackage;	// 10
	UInt32	unk18;					// 18
	UInt32	unk1C;					// 1C - init'd to 0000FFFFF
	UInt8	unk20;					// 20 - init'd to F8
	UInt8	pad21[7];				// 21
};
 //	ExtraTresPassPackage
 //	ExtraRunOncePacks

// 18
class ExtraReferenceHandle : public BSExtraData
{
public:
	ExtraReferenceHandle();
	virtual ~ExtraReferenceHandle();

	UInt32	handle;		// 10
	UInt32	pad;		// 14

	NiPointer<TESObjectREFR> GetReference();
	static ExtraReferenceHandle* Create();
};

// 28
class ExtraFollower : public BSExtraData
{
public:
	ExtraFollower();
	virtual ~ExtraFollower();

	tArray<void *> unk10;	// 10
};
 //	ExtraLevCreaModifier
 //	ExtraGhost
 //	ExtraOriginalReference

// 18
class ExtraOwnership : public BSExtraData
{
public:
	ExtraOwnership();
	virtual ~ExtraOwnership();

	TESForm* owner;
};
 //	ExtraGlobal

// 18
class ExtraRank : public BSExtraData
{
public:
	ExtraRank();
	virtual ~ExtraRank();

	SInt32	rank;	// 10
	UInt32	pad;	// 14

	static ExtraRank* Create();
};

// 18
class ExtraCount : public BSExtraData
{
public:
	ExtraCount();
	virtual ~ExtraCount();

	UInt32 count;	// 10 - Should be 16...
	UInt32 pad;		// 14

	static ExtraCount* Create();
};

// 18
class ExtraHealth : public BSExtraData // Tempered
{
public:
	ExtraHealth();
	virtual ~ExtraHealth();

	enum { // Multiplier
		kHealthBase = 1
	};

	float health;
	UInt32	pad;

	static ExtraHealth* Create();
};

// 18
class ExtraTimeLeft : public BSExtraData
{
public:
	ExtraTimeLeft();
	virtual ~ExtraTimeLeft();

	float time;
	UInt32	pad;
};

// 18
class ExtraCharge : public BSExtraData
{
public:
	ExtraCharge();
	virtual ~ExtraCharge();

	float charge;
	UInt32	pad;

	static ExtraCharge* Create();
};
 //	ExtraLight
 //	ExtraLock
 //	ExtraTeleport
 //	ExtraMapMarker
 //	ExtraLeveledCreature
 //	ExtraLeveledItem

// 18
class ExtraScale : public BSExtraData
{
public:
	ExtraScale();
	virtual ~ExtraScale();

	float scale;
	UInt32	pad;
};
 //	ExtraSeed
 //	ExtraMagicCaster
 //	ExtraPlayerCrimeList

// 18
class ExtraEnableStateParent : public BSExtraData
{
public:
	ExtraEnableStateParent();
	virtual ~ExtraEnableStateParent();

	UInt8	unk10;		// 10
	UInt8	pad11[3];	// 11
	UInt32	handle;		// 14 handle?

	NiPointer<TESObjectREFR> GetReference();
};
 //	ExtraEnableStateChildren
 //	ExtraItemDropper
 //	ExtraDroppedItemList
 //	ExtraRandomTeleportMarker
 //	ExtraSavedHavokData

// 10
class ExtraCannotWear : public BSExtraData
{
public:
	ExtraCannotWear();
	virtual ~ExtraCannotWear();	

	static ExtraCannotWear* Create();
};

// 20
class ExtraPoison : public BSExtraData
{
public:
	ExtraPoison();
	virtual ~ExtraPoison();

	// SE: they swapped order of poison and unk
	AlchemyItem*	poison;		// 10
	UInt32			unk18;		// 18
	UInt32			pad;		// 1C
};

 //	ExtraLastFinishedSequence
 //	ExtraSavedAnimation
 //	ExtraNorthRotation
 //	ExtraSpawnContainer
 //	ExtraFriendHits
 //	ExtraHeadingTarget
 //	ExtraRefractionProperty
 //	ExtraStartingWorldOrCell

// 18
class ExtraHotkey : public BSExtraData
{
public:
	ExtraHotkey();
	virtual ~ExtraHotkey();

	SInt8	hotkey;	// 10 (1 to 8, -1 unbound)
	UInt8	pad[7];	// 11

	static ExtraHotkey* Create();
};
 //	ExtraEditiorRefMoveData
 //	ExtraInfoGeneralTopic
 //	ExtraHasNoRumors
 //	ExtraSound
 //	ExtraTerminalState
 //	ExtraLinkedRef
 //	ExtraLinkedRefChildren
 //	ExtraActivateRef
 //	ExtraActivateRefChildren
 //	ExtraCanTalkToPlayer
 //	ExtraObjectHealth
 //	ExtraCellImageSpace
 //	ExtraNavMeshPortal
 //	ExtraModelSwap
 //	ExtraRadius

// 38
struct ExtraFactionChanges : public BSExtraData
{
public:
	ExtraFactionChanges();
	virtual ~ExtraFactionChanges();

	struct FactionInfo
	{
		TESFaction	* faction;	// 00
		SInt8		rank;		// 04
		UInt8		pad[3];		// 05
	};

	tArray<FactionInfo> factions;	// 10
	UInt64	unk28;		// 28
	UInt8	unk30;		// 30 - SE: this used to be at beginning in Skyrim32
	UInt8	unk31[7];	// 31
};
 //	ExtraDismemberedLimbs
 //	ExtraActorCause
 //	ExtraMultiBound
 //	ExtraMultiBoundData
 //	ExtraMultiBoundRef
 //	ExtraReflectedRefs
 //	ExtraReflectorRefs
 //	ExtraEmittanceSource
 //	ExtraRadioData
 //	ExtraCombatStyle

// 18
struct ExtraPrimitive : public BSExtraData
{
public:
	ExtraPrimitive();
	virtual ~ExtraPrimitive();

	BGSPrimitive	* primitive;
};
 //	ExtraOpenCloseActivateRef
 //	ExtraAnimNoteReceiver
 //	ExtraAmmo
 //	ExtraPatrolRefData
 //	ExtraPackageData
 //	ExtraOcclusionShape

// 18
struct ExtraCollisionData : public BSExtraData
{
public:
	ExtraCollisionData();
	virtual ~ExtraCollisionData();

	struct Data
	{
		UInt32	collisionLayer; // LayerId
	};

	Data	* data;
};
 //	ExtraSayTopicInfoOnceADay

 // 18
struct ExtraEncounterZone : public BSExtraData
{
public:
	ExtraEncounterZone();
	virtual ~ExtraEncounterZone();

	BGSEncounterZone* encounterZone;
};
 //	ExtraSayTopicInfo
 //	ExtraOcclusionPlaneRefData
 //	ExtraPortalRefData
 //	ExtraPortal
 //	ExtraRoom
 //	ExtraHealthPerc
 //	ExtraRoomRefData
 //	ExtraGuardedRefData
 //	ExtraCreatureAwakeSound
 //	ExtraHorse
 //	ExtraIgnoredBySandbox
 //	ExtraCellAcousticSpace
 //	ExtraReservedMarkers
 //	ExtraWeaponIdleSound
 //	ExtraWaterLightRefs
 //	ExtraWeaponAttackSound
 //	ExtraActivateLoopSound
 //	ExtraPatrolRefInUseData
 //	ExtraAshPileRef

// 38
class ExtraFollowerSwimBreadcrumbs : public BSExtraData
{
public:
	ExtraFollowerSwimBreadcrumbs();
	virtual ~ExtraFollowerSwimBreadcrumbs();

	UInt32	unk10;
	float	unk14; // Looks like a position?
	float	unk18;
	float	unk1C;
	UInt32	unk20;
	UInt32	pad24;
	UInt64	unk28;
	UInt64	unk30;
};

// 30
class ExtraAliasInstanceArray : public BSExtraData
{
public:
	ExtraAliasInstanceArray();
	virtual ~ExtraAliasInstanceArray();

	// 18
	struct AliasInfo
	{
		TESQuest		* quest;
		BGSBaseAlias	* alias;
		tArray<TESPackage*>	* packages;
	};

	tArray<AliasInfo*> aliases;	// 10
	UInt32	unk28;
	UInt32	unk2C;
};

// 18
class ExtraLocation : public BSExtraData
{
public:
	ExtraLocation(); // Related to protected/essential
	~ExtraLocation();

	BGSLocation* location;
};
 //	ExtraLitWaterRefs
 //	ExtraLocationRefType

// 28
class ExtraPromotedRef : public BSExtraData
{
public:
	ExtraPromotedRef();
	virtual ~ExtraPromotedRef();

	tArray<TESForm*> unk08;
};
 //	ExtraOutfitItem
 //	ExtraLeveledItemBase

// 28
class ExtraLightData : public BSExtraData
{
public:
	ExtraLightData();
	virtual ~ExtraLightData();

	float	unk10;		// 10
	UInt32	unk14;		// 14
	UInt32	unk18;		// 18
	float	unk1C;		// 1C
	UInt8	unk20;		// 20
	UInt8	pad21[7];	// 21
};
 //	ExtraSceneData
 //	ExtraBadPosition
 //	ExtraHeadTrackingWeight
 //	ExtraFromAlias
 //	ExtraShouldWear
 //	ExtraFavorCost
 //	ExtraAttachedArrows3D

// 38
class ExtraTextDisplayData : public BSExtraData
{
public:
	ExtraTextDisplayData();
	virtual ~ExtraTextDisplayData();

	BSFixedString	name;				// 10
	BGSMessage		* message;			// 18
	TESQuest		* owner;			// 20
	SInt32			unk14;				// 28 -1 default -2 explicit name?
	float			extraHealthValue;	// 2C
	UInt16			unk30;				// 30
	UInt8			pad32[6];			// 32

	const char* GenerateName(TESForm * form, float extraHealthValue);

	static ExtraTextDisplayData* Create();

	MEMBER_FN_PREFIX(ExtraTextDisplayData);
	DEFINE_MEMBER_FN(SetName_Internal, void, 0x00184140, const char * name);
	DEFINE_MEMBER_FN(GenerateName_Internal, const char*, 0x00184010, TESForm * form, float extraHealthValue);
};
 //	ExtraAlphaCutoff

// 20
class ExtraEnchantment : public BSExtraData
{
public:
	ExtraEnchantment();
	virtual ~ExtraEnchantment();

	EnchantmentItem*	enchant;		// 10
	UInt16				maxCharge;		// 18
	UInt8				unk0E;			// 1A - usually 0
	UInt8				pad1B[5];		// 1B

	static ExtraEnchantment* Create();
};

// 18
class ExtraSoul : public BSExtraData
{
public:
	ExtraSoul();
	virtual ~ExtraSoul();

	UInt32 count;	// 10 - Should be 8 bits
	UInt32 pad;		// 14

	static ExtraSoul* Create();
};

// 18
class ExtraForcedTarget : public BSExtraData
{
public:
	UInt32	handle;	// 10
	UInt32	pad;	// 14

	static ExtraForcedTarget* Create();
	NiPointer<TESObjectREFR> GetReference();
};
STATIC_ASSERT(sizeof(ExtraForcedTarget) == 0x18);

// 18
class ExtraUniqueID : public BSExtraData
{
public:
	UInt32	ownerFormId;	// 10
	UInt16	uniqueId;		// 14
	UInt16	pad;			// 16

	static ExtraUniqueID* Create();
};
 //	ExtraUniqueID
 //	ExtraFlags

// 18
class ExtraFlags : public BSExtraData
{
	ExtraFlags();
	virtual ~ExtraFlags();

	UInt32 flags;	// 10
	UInt32 pad;		// 14
};
 //	ExtraRefrPath
 //	ExtraDecalGroup
 //	ExtraLockList
 //	ExtraForcedLandingMarker
 //	ExtraLargeRefOwnerCells
 //	ExtraCelLWaterEnvMap
 //	ExtraCellGrassData
 //	ExtraTeleportName
 //	ExtraInteraction
 //	ExtraWaterData
 //	ExtraWaterCurrentZoneData
 //	ExtraAttachRef
 //	ExtraAttachRefChildren
 //	ExtraGroupConstraint
 //	ExtraScriptedAnimDependence
 //	ExtraCachedScale
 //	ExtraRaceData
 //	ExtraGIDBuffer
 //	ExtraMissingRefIDs


extern const RelocPtr<uintptr_t> s_ExtraHealthVtbl;
extern const RelocPtr<uintptr_t> s_ExtraChargeVtbl;
extern const RelocPtr<uintptr_t> s_ExtraCountVtbl;
extern const RelocPtr<uintptr_t> s_ExtraTextDisplayVtbl;
extern const RelocPtr<uintptr_t> s_ExtraSoulVtbl;
extern const RelocPtr<uintptr_t> s_ExtraOwnershipVtbl;
extern const RelocPtr<uintptr_t> s_ExtraAliasInstanceArrayVtbl;
extern const RelocPtr<uintptr_t> s_ExtraCannotWearVtbl;
extern const RelocPtr<uintptr_t> s_ExtraHotkeyVtbl;
extern const RelocPtr<uintptr_t> s_ExtraForcedTargetVtbl;
extern const RelocPtr<uintptr_t> s_ExtraReferenceHandleVtbl;
extern const RelocPtr<uintptr_t> s_ExtraEnchantmentVtbl;
extern const RelocPtr<uintptr_t> s_ExtraRankVtbl;
extern const RelocPtr<uintptr_t> s_ExtraUniqueIdVtbl;
