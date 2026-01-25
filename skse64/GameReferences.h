#pragma once

#include "skse64/GameFormComponents.h"
#include "skse64/GameForms.h"
#include "skse64/GameEvents.h"
#include "skse64/NiObjects.h"
#include "skse64/GameObjects.h"
#include "GameBSExtraData.h"

class BSAnimationGraphEvent;
class NiNode;
class NiPoint3;
class TESObjectREFR;
class BSFaceGenNiNode;
class BSFaceGenAnimationData;
class PlayerCharacter;
class ImageSpaceModifierInstanceDOF;
class ObjectListItem;
class BSFadeNode;
class BGSLightingShaderProperty;
class BSAnimationGraphManager;
class QueuedFile;
class InventoryEntryData;

class VMClassRegistry;

// TESObjectREFR and child classes

// 10 
class BSHandleRefObject : public NiRefObject
{
public:
	enum
	{
		kMask_RefCount = 0x3FF
	};

	UInt32 GetRefCount() const
	{
		return m_uiRefCount & kMask_RefCount;
	}

	void DecRef()
	{
		if((InterlockedDecrement(&m_uiRefCount) & kMask_RefCount) == 0)
			DeleteThis();
	}
};

// The refHandle is stored in BSHandleRefObject.m_uiRefCount
// Bits:
//	01 - 10	ref count
//	11		isHandleSet
//	12 - ??	handle (seems to be divided in two parts, 0xFFFFF and 0x3F00000)

// Adds entry to lookup table if not yet there
typedef void (* _CreateRefHandleByREFR)(UInt32 & refHandleOut, TESObjectREFR * refr);
extern RelocAddr<_CreateRefHandleByREFR> CreateRefHandleByREFR;

// Note: May set refHandle to 0
typedef bool (* _LookupREFRByHandle)(UInt32 & refHandle, NiPointer<TESObjectREFR> & refrOut);
extern RelocAddr<_LookupREFRByHandle> LookupREFRByHandle;

typedef bool (* _LookupREFRObjectByHandle)(UInt32 & refHandle, NiPointer<BSHandleRefObject> & refrOut);
extern RelocAddr<_LookupREFRObjectByHandle> LookupREFRObjectByHandle;

extern RelocPtr<UInt32> g_invalidRefHandle;

// Try to retrive/create handle. Return invalid handle otherwise.
UInt32 GetOrCreateRefrHandle(TESObjectREFR* refr);

// Place/move reference
typedef void (* _MoveRefrToPosition)(TESObjectREFR* source, UInt32* pTargetHandle, void* parentCell, void* worldSpace, NiPoint3* postion, NiPoint3* rotation);
extern RelocAddr<_MoveRefrToPosition> MoveRefrToPosition;

typedef TESObjectREFR* (* _PlaceAtMe_Native)(VMClassRegistry* registry, UInt32 stackId, TESObjectREFR* target, TESForm* form, SInt32 count, bool bForcePersist, bool bInitiallyDisabled);
extern RelocAddr<_PlaceAtMe_Native> PlaceAtMe_Native;

typedef void (* _AddItem_Native)(VMClassRegistry* registry, UInt32 stackId, TESObjectREFR* target, TESForm* form, SInt32 count, bool bSilent);
extern RelocAddr<_AddItem_Native> AddItem_Native;

// 8 
class IAnimationGraphManagerHolder
{
public:
	virtual ~IAnimationGraphManagerHolder();

	virtual UInt32			Unk_01(void);
	virtual UInt32			Unk_02(void);
	virtual UInt32			Unk_03(void);
	virtual UInt32			Unk_04(void);
	virtual UInt32			Unk_05(void);
	virtual UInt32			Unk_06(void);
	virtual UInt32			Unk_07(void);
	virtual UInt32			Unk_08(void);
	virtual UInt32			Unk_09(void);
	virtual UInt32			Unk_0A(void);
	virtual UInt32			Unk_0B(void);
	virtual UInt32			Unk_0C(void);
	virtual UInt32			Unk_0D(void);
	virtual UInt32			Unk_0E(void);
	virtual UInt32			Unk_0F(void);
	virtual UInt32			Unk_10(void);
	virtual UInt32			Unk_11(void);
	virtual UInt32			Unk_12(void);

//	void	** _vtbl;
};

// 8 
class IPostAnimationChannelUpdateFunctor
{
public:
	virtual ~IPostAnimationChannelUpdateFunctor();
//	void	** _vtbl;
};

// 98 
class TESObjectREFR : public TESForm
{
public:
	enum { kTypeID = kFormType_Reference };

	enum
	{
		kFlag_Harvested = 0x2000,

		kChange_IsEmpty = 0x00200000,
	};

	// currently none of these have been analyzed
	virtual void	Unk_39(void);
	virtual void	Unk_3A(void);
	virtual void	Unk_3B(void);
	virtual void	Unk_3C(void);
	virtual void	Unk_3D(void);
	virtual void	Unk_3E(void);
	virtual void	Unk_3F(void);
	virtual void	Unk_40(void);
	virtual void	Unk_41(void);
	virtual void	Unk_42(void);
	virtual void	Unk_43(void);
	virtual void	Unk_44(void);
	virtual void	Unk_45(void);
	virtual void	Unk_46(void);
	virtual void	Unk_47(void);
	virtual void	Unk_48(void);
	virtual void	Unk_49(void);
	virtual void	Unk_4A(void);
	virtual void	Unk_4B(void);
	virtual void	Unk_4C(void);
	virtual void	Unk_4D(void);
	virtual void	Unk_4E(void);
	virtual void	Unk_4F(void);
	virtual void	Unk_50(void);
	virtual void	GetStartingPos(NiPoint3 * pos);
	virtual void	Unk_52(void);
	virtual void	Unk_53(void);
	virtual void	Unk_54(void);
	virtual void	Unk_55(void);
	virtual void	Unk_56(void);
	virtual void	Unk_57(void);
	virtual void	Unk_58(void);
	virtual void	Unk_59(void);
	virtual void	Unk_5A(void);
	virtual void	GetMarkerPosition(NiPoint3 * pos);
	virtual void	Unk_5C(void);
	virtual void	Unk_5D(void);
	virtual void	Unk_5E(void);
	virtual void	Unk_5F(void);
	virtual void	Unk_60(void);
	virtual BSFaceGenNiNode *	GetFaceGenNiNode(void);
	virtual void	Unk_62(void);
	virtual BSFaceGenAnimationData *	GetFaceGenAnimationData(void);
	virtual void	Unk_64(void);
	virtual void	Unk_65(void);
	virtual void	Unk_66(void);
	virtual void	Unk_67(void);
	virtual void	Unk_68(void);
	virtual void	Unk_69(void);
	virtual void	Unk_6A(void);
	virtual void 	Unk_6B(void);
	virtual void	Unk_6C(void);
	virtual void	SetNiNode(NiNode * root, UInt32 unk1); // NULL, 1?
	virtual void	Unk_6E(void);
	virtual NiNode	* GetNiRootNode(UInt32 firstPerson);
	virtual NiNode	* GetNiNode(void);	// Root of the skeleton (Get3D)
	virtual void	Unk_71(void);
	virtual void	Unk_72(void);
	virtual void	Unk_73(void);
	virtual void	Unk_74(void);
	virtual void	Unk_75(void);
	virtual void	Unk_76(void);
	virtual void	Unk_77(void);
	virtual void	Unk_78(void);
	virtual void	Unk_79(void);
	virtual void	Unk_7A(void);
	virtual void	Unk_7B(void);
	virtual void	Unk_7C(void);
	virtual void	Unk_7D(void);
	virtual BipedModel* GetBiped(UInt32 weightModel); // 0 Small 1 Large
	virtual BipedModel*	GetBipedSmall(void);
	virtual void	Unk_80(void);
	virtual void	Unk_81(void);
	virtual void	Unk_82(void);
	virtual void	Unk_83(void);
	virtual void	Unk_84(void);
	virtual void	Unk_85(void);
	virtual void	Unk_86(void);
	virtual void	Unk_87(void);
	virtual void	Unk_88(void);
	virtual void	Unk_89(void);
	virtual void	ResetInventory(bool unk);
	virtual void	Unk_8B(void);
	virtual void	Unk_8C(void);
	virtual void	Unk_8D(void);
	virtual void	Unk_8E(void);
	virtual void	Unk_8F(void);
	virtual void	Unk_90(void);
	virtual void	Unk_91(void);
	virtual void	Unk_92(void);
	virtual void	Unk_93(void);
	virtual void	Unk_94(void);
	virtual void	Unk_95(void);
	virtual void	Unk_96(void);
	virtual void	Unk_97(void);
	virtual void	Unk_98(void);
	virtual bool	IsDead(UInt8 unk1); // unk1 = 1 for Actors
	virtual void	Unk_9A(void);
	virtual void	Unk_9B(void);

	struct LoadedState
	{
		UInt8	todo[0x68];
		NiNode	* node;	// 68
		// ... probably more
	};

	// parents
	BSHandleRefObject	handleRefObject;	// 20
	BSTEventSink <BSAnimationGraphEvent>	animGraphEventSink;	// 30
	IAnimationGraphManagerHolder			animGraphHolder;	// 38

	// members
	TESForm* baseForm;	// 40
	NiPoint3	rot;	// 48
	NiPoint3	pos;	// 54

	TESObjectCELL	* parentCell;	// 60
	LoadedState		* loadedState;	// 68	
	BaseExtraList	extraData;		// 70
	UInt64			unk88; // 88 - New in SE
	UInt16			unk90;	// 90 - init'd to 100
	UInt8			unk92;	// 92
	UInt8			unk93;	// 93
	UInt32			pad94;	// 94

	UInt32 CreateRefHandle(void);

	void IncRef();
	void DecRef();

	MEMBER_FN_PREFIX(TESObjectREFR);
	DEFINE_MEMBER_FN(GetBaseScale, float, 0x0028CC60);
	DEFINE_MEMBER_FN(IsOffLimits, bool, 0x0029A330);
	// E2B825B476DC5CFBC69E194AA76CB0C049AC78E3+137
	DEFINE_MEMBER_FN(GetWeight, float, 0x002A6FF0);
	DEFINE_MEMBER_FN(GetReferenceName, const char *, 0x002961F0);
	DEFINE_MEMBER_FN(GetWorldspace, TESWorldSpace*, 0x00299750);
};

STATIC_ASSERT(sizeof(TESObjectREFR) == 0x98);
STATIC_ASSERT(offsetof(TESObjectREFR, extraData) == 0x70);
STATIC_ASSERT(offsetof(TESObjectREFR, loadedState) == 0x68);
STATIC_ASSERT(offsetof(TESObjectREFR::LoadedState, node) == 0x68);

// 08 
class IMovementInterface
{
public:
	virtual ~IMovementInterface();
//	void	** _vtbl;	// 00
};

// 08 
class IMovementState : public IMovementInterface
{
public:
	virtual ~IMovementState();
};

// 10
class ActorState : public IMovementState
{
public:
	virtual ~ActorState();

//	void	** _vtbl;	// 00

	// older versions of this class stored flags in a UInt64
	// this forced the addition of 4 useless padding bytes
	// current and future versions store flags as two UInt32s

	enum {
		kState_Running = 0x40,
		kState_Walking = 0x80,
		kState_Sprinting = 0x100,
		kState_Sneaking = 0x200,
		kState_Swimming = 0x400
	};

	// SE: not changing names of these, they are referenced somewhere else
	UInt32	flags04;	// 08
	UInt32	flags08;	// 0C

	bool IsWeaponDrawn()
	{
		return (flags08 >> 5 & 7) >= 3;
	}
};
STATIC_ASSERT(sizeof(ActorState) == 0x10);

// 2B0 
class Actor : public TESObjectREFR
{
public:
	enum { kTypeID = kFormType_Character };

	virtual ~Actor();
	virtual void Unk_9C(void);
	virtual void Unk_9D(void);
	virtual void Unk_9E(void);
	virtual void Unk_9F(void);
	virtual void Unk_A0(void);
	virtual void Unk_A1(void);
	virtual void Unk_A2(void);
	virtual void Unk_A3(void);
	virtual void Unk_A4(void);
	virtual void Unk_A5(void);
	virtual void DrawSheatheWeapon(bool draw);
	virtual void Unk_A7(void);
	virtual void Unk_A8(void);
	virtual void Unk_A9(void);
	virtual void Unk_AA(void);
	virtual void Unk_AB(void);
	virtual void Unk_AC(void);
	virtual void Unk_AD(void);
	virtual void Unk_AE(void);
	virtual void Unk_AF(void);
	virtual void Unk_B0(void);
	virtual void Unk_B1(void);
	virtual void Unk_B2(void);
	virtual void Unk_B3(void);
	virtual void Unk_B4(void);
	virtual void Unk_B5(void);
	virtual void Unk_B6(void);
	virtual void Unk_B7(void);
	virtual void Unk_B8(void);
	virtual void Unk_B9(void);
	virtual void Unk_BA(void);
	virtual void Unk_BB(void);
	virtual void Unk_BC(void);
	virtual void Unk_BD(void);
	virtual void Unk_BE(void);
	virtual void Unk_BF(void);
	virtual void Unk_C0(void);
	virtual void Unk_C1(void);
	virtual void Unk_C2(void);
	virtual void Unk_C3(void);
	virtual void Unk_C4(void);
	virtual void Unk_C5(void);
	virtual void Unk_C6(void);
	virtual void Unk_C7(void);
	virtual void Unk_C8(void);
	virtual void Unk_C9(void);
	virtual void Unk_CA(void);
	virtual void Unk_CB(void);
	virtual void Unk_CC(void);
	virtual void Unk_CD(void);
	virtual void Unk_CE(void);
	virtual void Unk_CF(void);
	virtual void Unk_D0(void);
	virtual void Unk_D1(void);
	virtual void Unk_D2(void);
	virtual void Unk_D3(void);
	virtual void Unk_D4(void);
	virtual void Unk_D5(void);
	virtual void Unk_D6(void);
	virtual void Unk_D7(void);
	virtual void Unk_D8(void);
	virtual void Unk_D9(void);
	virtual void Unk_DA(void);
	virtual void Unk_DB(void);
	virtual void Unk_DC(void);
	virtual void Unk_DD(void);
	virtual void Unk_DE(void);
	virtual void Unk_DF(void);
	virtual void Unk_E0(void);
	virtual void Unk_E1(void);
	virtual void Unk_E2(void);
	virtual bool IsInCombat(void);
	virtual void Unk_E4(void);
	virtual void Unk_E5(void);
	virtual void Unk_E6(void);
	virtual void Unk_E7(void);
	virtual void Unk_E8(void);
	virtual void Unk_E9(void);
	virtual void Unk_EA(void);
	virtual void Unk_EB(void);
	virtual void Unk_EC(void);
	virtual void Unk_ED(void);
	virtual void Unk_EE(void);
	virtual void Unk_EF(void);
	virtual void Unk_F0(void);
	virtual void Unk_F1(void);
	virtual void Unk_F2(void);
	virtual void Unk_F3(void);
	virtual void Unk_F4(void);
	virtual void Unk_F5(void);
	virtual void Unk_F6(void);
	virtual void AdvanceSkill(UInt32 skillId, float points, UInt32 unk1, UInt32 unk2);
	virtual void Unk_F8(void);
	virtual void Unk_F9(void);
	virtual void VisitPerks(void); // BGSPerk::FindPerkInRanksVisitor
	virtual void AddPerk(BGSPerk * perk, UInt32 unk1);
	virtual void RemovePerk(BGSPerk * perk);

	// 0C
	class SpellArray
	{
	public:
		SpellItem * Get(UInt32 idx)
		{
			if(idx >= spellCount) return NULL;

			if(allocatedCount & kLocalAlloc)
				return (idx == 0) ? singleSpell : NULL;
			else
				return spells ? spells[idx] : NULL;
		}

		UInt32	Length(void)	{ return spellCount; }

	private:
		enum
	{
			kLocalAlloc = 0x80000000,
		};

		UInt32		allocatedCount;		// 00 - upper bit is set when single-element optimization is used
		union {
			SpellItem	** spells;		// 08
			SpellItem	* singleSpell;	// 08 - used when kLocalAlloc is set
		};
		UInt32		spellCount;		// 10
	};

	enum Flags1 {
		kFlags_AIEnabled = 0x02,
		kFlags_IsPlayerTeammate = 0x4000000
	};
	enum Flags2 {
		kFlags_CanDoFavor = 0x80
	};

	MagicTarget		magicTarget;					// 098
	ActorValueOwner	actorValueOwner;				// 0B0
	ActorState		actorState;						// 0B8
	BSTEventSink<void*>	transformDeltaEvent;		// 0C8 .?AV?$BSTEventSink@VBSTransformDeltaEvent@@@@
	BSTEventSink<void*>	characterMoveFinishEvent;	// 0D0 .?AV?$BSTEventSink@VbhkCharacterMoveFinishEvent@@@@
	IPostAnimationChannelUpdateFunctor	unk0D8;		// 0D8 IPostAnimationChannelUpdateFunctor
	UInt32	flags1;									// 0E0
	float	unk0E4;									// 0E4
	UInt32	unk0E8;									// 0E8
	UInt32	pad0EC;									// 0EC
	ActorProcessManager	* processManager;			// 0F0 
	UInt32	unk0F8;									// 0F8
	UInt32	unk0FC;									// 0FC
	UInt32	unk100;									// 100
	UInt32	unk104;									// 104
	float	unk108;									// 108	- init'd to -1
	UInt32	unk10C;									// 10C
	UInt32	unk110;									// 110
	UInt32	unk114;									// 114
	UInt32	unk118;									// 118
	UInt32	unk11C;									// 11C
	float	unk120;									// 120
	float	unk124;									// 124
	float	unk128;									// 128
	UInt32	unk12C;									// 12C
	UInt64	unk130;									// 130
	UInt64	unk138;									// 138
	void*	unk140;									// 140 ActorMover*
	void*	unk148;									// 148 MovementControllerNPC*
	UInt64	unk150;									// 150
	UInt64	unk158;									// 158
	UInt64	unk160;									// 160
	float	unk168;									// 168
	UInt32	unk16C;									// 16C
	UInt32	unk170;									// 170
	UInt32	unk174;									// 174 - init'd to 50
	UInt32	unk178;									// 178
	UInt32	unk17C;									// 17C - init'd to 7FFFFFFF
	UInt64	unk180;									// 180
	SpellArray	addedSpells;						// 188
	void*	unk1A0;									// 1A0	ActorMagicCaster*
	void*	unk1A8;									// 1A8	ActorMagicCaster*
	UInt64	unk1B0;									// 1B0
	void*	unk1B8;									// 1B8	ActorMagicCaster*
	SpellItem* leftHandSpell;						// 1C0
	SpellItem* rightHandSpell;						// 1C8
	UInt64	unk1D0;									// 1D0
	UInt64	unk1D8;									// 1D8
	TESForm	* equippedShout;						// 1E0
	UInt32	unk1E8;									// 1E8
	UInt32	pad1EC;									// 1EC
	TESRace*	race;								// 1F0
	float	unk1F8;									// 1F8 - init'd to -1
	UInt32	flags2;									// 1FC
	
	// These two are part of an object
	BSString	unk200;								// 200
	BSString	unk210;								// 210
	
	UInt64	unk220;									// 220

	// C
	struct Data228
	{
		UInt32	unk0;
		UInt32	unk4;
		UInt32	unk8;
	};

	Data228	unk228;
	Data228	unk234;
	Data228	unk240;
	Data228 unk24C;
	float	unk258;									// 258 - init'd to -1
	UInt32	unk25C;									// 25C
	UInt64	unk260;									// 260
	float	unk268;									// 268
	float	unk26C;									// 26C
	UInt32	unk270;									// 270
	UInt32	unk274;									// 274	
	UInt64	unk278;									// 278
	UInt64	unk280;									// 280
	UInt64	unk288;									// 288
	UInt64	unk290;									// 290
	UInt64	unk298;									// 298
	UInt64	unk2A0;									// 2A0
	UInt64	unk2A8;									// 2A8

	MEMBER_FN_PREFIX(Actor);
	DEFINE_MEMBER_FN(QueueNiNodeUpdate, void, 0x00693110, bool updateWeight);
	DEFINE_MEMBER_FN(HasPerk, bool, 0x005F9E30, BGSPerk * perk);
	DEFINE_MEMBER_FN(GetLevel, UInt16, 0x005D62E0);
	DEFINE_MEMBER_FN(SetRace, void, 0x00607680, TESRace*, bool isPlayer);
	DEFINE_MEMBER_FN(UpdateWeaponAbility, void, 0x006313D0, TESForm*, BaseExtraList * extraData, bool bLeftHand);
	DEFINE_MEMBER_FN(UpdateArmorAbility, void, 0x00631360, TESForm*, BaseExtraList * extraData);
	DEFINE_MEMBER_FN(IsHostileToActor, bool, 0x005E7E40, Actor * actor);
	DEFINE_MEMBER_FN(ResetAI, void, 0x005DB310, UInt32 unk1, UInt32 unk2);

	TESForm * GetEquippedObject(bool abLeftHand);
	void UpdateSkinColor();
	void UpdateHairColor();

	class FactionVisitor
	{
	public:
		virtual bool Accept(TESFaction * faction, SInt8 rank) = 0;
	};

	// Can contain duplicate entries with different rankings
	bool VisitFactions(FactionVisitor & visitor);
};

STATIC_ASSERT(offsetof(Actor, magicTarget) == 0x98);
STATIC_ASSERT(offsetof(Actor, actorValueOwner) == 0xB0);
STATIC_ASSERT(offsetof(Actor, actorState) == 0xB8);
STATIC_ASSERT(offsetof(Actor, unk0D8) == 0xD8);
STATIC_ASSERT(offsetof(Actor, addedSpells) == 0x188);
STATIC_ASSERT(sizeof(Actor) == 0x2B0);

// 2B0 
class Character : public Actor
{
public:
	enum { kTypeID = kFormType_Character };
};

STATIC_ASSERT(sizeof(Character) == 0x2B0);

// BE0 
class PlayerCharacter : public Character
{
public:
	virtual ~PlayerCharacter();

	// parents
	BSTEventSink <void *>	menuOpenCloseEvent;			// 2B0 .?AV?$BSTEventSink@VMenuOpenCloseEvent@@@@
	BSTEventSink <void *>	menuModeChangeEvent;		// 2B8 .?AV?$BSTEventSink@VMenuModeChangeEvent@@@@
	BSTEventSink <void *>	userEventEnabledEvent;		// 2C0 .?AV?$BSTEventSink@VUserEventEnabledEvent@@@@
	BSTEventSink <void *>	trackedStatsEvent;			// 2C8  BSTEventSink@UTESTrackedStatsEvent@@@@@

	BSTEventSource <void *>	actorCellEventSource;		// 2D0 .?AV?$BSTEventSource@UBGSActorCellEvent@@@@  
														// tArray<void*>: 4 PlayerRegionState, BGSPlayerMusicChanger, CellAcousticSpaceListener, PlayerParentCellListener
	BSTEventSource <void *>	actorDeathEventSource;		// 328 .?AV?$BSTEventSource@UBGSActorDeathEvent@@@@ 
														// tArray<void*>: 1 BGSPlayerMusicChanger
	BSTEventSource <void *>	positionPlayerEventSource;	// 380 .?AV?$BSTEventSource@UPositionPlayerEvent@@@@
														// tArray<void*>: 9 MovementAvoidBoxEventAdapter, GarbaseCollector, Main, MenuTopicManager, TES (85E27728),
														// PathManagerPositionPlayerAdapter, CharacterCollisionMessagePlayerAdapter, PlayerSleepWaitMovementControllerAdapter, SkyrimVM

	UInt32	unk3D8;									// 3D8
	UInt32	unk3DC;									// 3DC
	UInt32	unk3E0;									// 3E0
	UInt32	unk3E4;									// 3E4
	UInt32	unk3E8;									// 3E8
	UInt32	unk3EC;									// 3EC
	UInt32	unk3F0;									// 3F0
	UInt32	unk3F4;									// 3F4
	void*	unk3F8;									// 3F8
	UInt32	unk400;									// 400
	UInt32	unk404;									// 404
	void*	unk408;									// 408
	UInt32	unk410;									// 410
	UInt32	unk414;									// 414
	UInt32	unk418;									// 418
	UInt32	unk41C;									// 41C
	UInt32	unk420;									// 420
	UInt32	unk424;									// 424
	void*	unk428;									// 428
	UInt32	unk430;									// 430
	UInt32	unk434;									// 434
	void*	unk438;									// 438
	UInt32	unk440;									// 440
	UInt32	unk444;									// 444
	UInt32	unk448;									// 448
	UInt32	unk44C;									// 44C
	UInt32	unk450;									// 450
	UInt32	unk454;									// 454
	UInt32	unk458;									// 458
	UInt32	unk45C;									// 45C
	void*	unk460;									// 460
	UInt64	unk468;									// 468	
	ObjectListItem*	unk470;							// 470
	float	unk478;									// 478
	float	unk47C;									// 47C
	float	unk480;									// 480
	float	unk484;									// 484
	float	unk488;									// 488
	float	unk48C;									// 48C
	float	unk490;									// 490
	float	unk494;									// 494
	float	unk498;									// 498
	float	unk49C;									// 49C
	UInt64	unk4A0;									// 4A0
	UInt64	unk4A8;									// 4A8	
	BGSPerkRanks addedPerks;						// 4B0
	tArray<BGSPerk*> perks;							// 4C8
	tArray<BGSPerk*> standingStonePerks;			// 4E0
	tArray<UInt32>	unk4F8;							// 4F8
	struct DataUnk510 { UInt32 a; UInt32 b; UInt32 c; UInt32 pad; };
	tArray<DataUnk510>	unk510;						// 510
	UnkArray		unk528;							// 528
	tArray<void*>	unk540;							// 540
	tArray<void*>	unk558;							// 558
	void *			unk570;							// 570
	void*			unk578;							// 578
	tArray<void*>	unk580;							// 580
	UInt32			unk598;							// 598
	UInt32			unk59C;							// 59C
	UInt32			unk5A0;							// 5A0
	UInt32			unk5A4;							// 5A4
	UInt32			unk5A8;							// 5A8
	UInt32			unk5AC;							// 5AC
	void*			unk5B0;							// 5B0
	UInt64			unk5B8;							// 5B8
	void*			unk5C0;							// 5C0
	UInt32			unk5C8;							// 5C8
	UInt32			unk5CC;							// 5CC
	UInt32			unk5D0;							// 5D0
	UInt32			unk5D4;							// 5D4
	UInt32			unk5D8;							// 5D8
	UInt32			unk5DC;							// 5DC
	void*			unk5E0;							// 5E0
	UInt8			unk5E8;							// 5E8
	UInt8			pad5E9[7];						// 5E9
	void*			unk5F0;							// 5F0
	UInt32			unk5F8;							// 5F8
	UInt32			pad5FC;							// 5FC
	UInt64			unk600;							// 600
	NiTMap<UInt64,UInt64>			unk608;			// 608
	TESWorldSpace*	currentWorldSpace;				// 628
	float			unk630;							// 630
	float			unk634;							// 634
	float			unk638;							// 638
	UInt32			unk63C;							// 63C
	UInt64			unk640;							// 640
	UInt64			unk648;							// 648
	float			unk650;							// 650
	float			unk654;							// 654
	float			unk658;							// 658
	float			unk65C;							// 65C
	float			unk660;							// 660
	float			unk664;							// 664
	UInt64			unk668;							// 668
	UInt64			unk670;							// 670
	UInt32			unk678;							// 678
	UInt32			unk67C;							// 67C
	UInt8			unk680;							// 680
	UInt8			unk681;							// 681
	UInt8			unk682;							// 682
	UInt8			pad683;							// 683
	UInt32			unk684;							// 684
	UInt32			unk688;							// 688 - init'd to FFFFFFFF
	UInt8			unk68C;							// 68C
	UInt8			pad68D[3];						// 68D
	UInt32			unk690;							// 690
	UInt32			unk694;							// 694
	UInt8			unk698;							// 698
	UInt8			pad699[3];						// 699
	UInt32			unk69C;							// 69C
	UInt32			unk6A0;							// 6A0
	UInt8			unk6A4;							// 6A4
	UInt8			pad6A5[7];						// 6A5
	UInt64			unk6B0;							// 6B0
	UInt32			unk6B8;							// 6B8
	UInt32			unk6BC;							// 6BC
	UInt64			unk6C0;							// 6C0
	UInt32			unk6C8;							// 6C8
	float			unk6CC;							// 6CC - init'd to -1
	UInt32			unk6D0;							// 6D0
	float			unk6D4;							// 6D4 - init'd to -1
	float			unk6D8;							// 6D8 - FF7FFFFF
	UInt32			unk6DC;							// 6DC - probably pad
	ImageSpaceModifierInstanceDOF* unk6E0;			// 6E0
	ImageSpaceModifierInstanceDOF* unk6E8;			// 6E8
	ImageSpaceModifierInstanceDOF* unk6F0;			// 6F0
	UInt32			unk6F8;							// 6F8 - looks like a pointer along with next in debugger, but it isn't
	UInt32			unk6FC;							// 6FC
	UInt64			unk700[3];						// 700
	UInt64			unk718;							// 718
	UInt32			unk720;							// 720
	UInt32			pad724;							// 724
	UInt64			unk728;							// 728
	UInt8			unk730[0xA0];					// 730 - memset to 0 in ctor
	UInt32			unk7D0;							// 7D0
	UInt32			unk7D4;							// 7D4
	UInt32			unk7D8;							// 7D8

	// C
	struct Data7DC
	{
		float	unk0; // init'd to -1
		UInt32	unk4;
		UInt32	unk8; // init'd to 16
	};

	Data7DC			unk7DC[15];						// 7DC
	UInt32			unk890;							// 890 - init'd to 15
	UInt32			unk894;							// 894
	UInt32			unk898;							// 898
	UInt32			unk89C;							// 89C
	UInt64			unk8A0[5];						// 8A0
	UInt32			unk8C8;							// 8C8
	UInt32			unk8CC;							// 8CC
	UInt32			unk8D0;							// 8D0
	float			unk8D4;							// 8D4 - init'd to -5
	UInt64			unk8D8;							// 8D8
	UInt32			unk8E0;							// 8E0
	UInt32			unk8E4;							// 8E4
	UInt64			unk8E8;							// 8E8
	BSFadeNode*		firstPersonSkeleton;			// 8F0
	float			unk8F8;							// 8F8
	UInt32			pad8FC;							// 8FC
	float			unk900;							// 900
	UInt32			pad904;							// 904
	UInt64			unk908;							// 908
	UInt32			unk910;							// 910
	UInt32			lastRiddenHorseHandle;			// 914
	UInt32			unk918;							// 918
	UInt32			unk91C;							// 91C
	UInt64			unk920;							// 920
	UInt64			unk928;							// 928
	UInt32			unk930;							// 930
	UInt32			unk934;							// 934
	UInt64			unk938;							// 938
	UInt64			unk940;							// 940
	UInt32			unk948;							// 948
	UInt32			unk94C;							// 94C
	UInt32			unk950;							// 950
	UInt32			unk954;							// 954
	UInt32			unk958;							// 958
	UInt32			unk95C;							// 95C
	UInt32			unk960;							// 960
	UInt32			unk964;							// 964
	UInt64			unk968;							// 968
	UInt64			unk970;							// 970 - init'd to GetTickCount() in ctor
	UInt64			unk978;							// 978
	UInt32			unk980;							// 980 - init'd to _time64(NULL) in ctor
	UInt32			unk984;							// 984
	TESWorldSpace*	sameWorldSpace;					// 988
	UInt32			unk990;							// 990
	UInt32			unk994;							// 994
	UInt64			unk998;							// 998
	UInt64			unk9A0;							// 9A0
	UInt64			unk9A8;							// 9A8
	PlayerSkills *	skills;							// 9B0
	UInt32			targetHandle;					// 9B8 
	UInt32			unk9BC;							// 9BC
	UInt64			unk9C0;							// 9C0
	BSFadeNode*		fadeNode9C8;					// 9C8
	void*			unk9D0;							// 9D0
	tArray<UInt32>	hostileHandles; 				// 9D8
	UInt64			unk9F0;							// 9F0
	UInt64			unk9F8;							// 9F8
	TESForm*		tempPoison;						// A00
	UInt32			numTeammates;					// A08
	UInt32			unkA0C;							// A0C
	UInt64			unkA10;							// A10
	float			unkA18;							// A18
	UInt32			unkA1C;							// A1C
	UInt8			unkA20[0xA0];					// A20 - memset to 0 in ctor
	UInt32			unkAC0;							// AC0
	UInt32			unkAC4;							// AC4
	BGSLocation*	locationAC8;					// AC8
	float			unkAD0;							// AD0
	UInt32			unkAD4;							// AD4
	UInt32			unkAD8;							// AD8
	UInt32			unkADC;							// ADC
	UInt64			unkAE0;							// AE0
	UInt32			unkAE8;							// AE8
	UInt32			unkAEC;							// AEC
	UInt32			unkAF0;							// AF0
	UInt32			unkAF4;							// AF4
	UInt32			unkAF8;							// AF8
	UInt32			unkAFC;							// AFC
	UInt8			unkB00;							// B00
	UInt8			numPerkPoints;					// B01
	UInt8			unkB02;							// B02
	UInt8			unkB03;							// B03
	UInt32			unkB04;							// B04
	void*			unkB08;							// B08
	tArray<TintMask *>	tintMasks;					// B10
	tArray <TintMask *>	* overlayTintMasks;			// B28
	BGSTextureSet*	texSetB30;						// B30
	TESRace*		race;							// B38
	TESRace*		raceAgain;						// B40 - transformed race maybe for vamps and werewolves?
	UInt32			unkB48;							// B48
	UInt32			unkB4C;							// B4C
	UnkArray		unkB50;							// B50
	UInt64			unkB68[5];						// B68
	UInt64			unkB90;							// B90
	UInt64			unkB98;							// B98
	UInt32			unkBA0;							// BA0
	UInt32			unkBA4;							// BA4
	UInt64			unkBA8;							// BA8
	UInt64			unkBB0[3];						// BB0
	UInt32			unkBC8;							// BC8
	UInt32			unkBCC;							// BCC
	UInt64			unkBD0;							// BD0
	UInt8			unkBD8;							// BD8
	UInt8			unkBD9;							// BD9
	UInt8			unkBDA;							// BDA
	UInt8			unkBDB;							// BDB
	UInt8			unkBDC;							// BDC
	UInt8			unkBDD;							// BDD
	UInt16			padBDE;							// BDE
	// Object allocation size is 0xBE0. Anything >= this address wouldn't be part of this object!!!!

	// Overlayed tints should be the same as original tints
	// occasionally they can have no type so index matching
	// is required to set anything on the tint
	TintMask * GetOverlayTintMask(TintMask * original);

	// Confirmed - Same as ExtraContainerChanges::EntryData
	// This type is used by scaleform to extend data
	// It can be used to extend more of the "ExtraData"
	/*struct ObjDesc
	{
		TESForm					* form;
		tList<BaseExtraList>	* extraData;
		SInt32					countDelta;

		MEMBER_FN_PREFIX(ObjDesc);
		//DEFINE_MEMBER_FN(GenerateName, const char *, 0x00475AA0);
	};*/

	// I've replaced the old GetTintList defined through DEFINE_MEMBER_FN by a regular method.
	// GetTintList doesn't longer exist in Skyrim64. There is a function that fullfills same role at 0x680630 (1.4),
	// but it cannot be used like it is because it assumes it is called with object at offsset 0xB10.
	tArray <TintMask *> * GetTintList()
	{
		if (overlayTintMasks)
			return overlayTintMasks;

		return &tintMasks;
	}

	MEMBER_FN_PREFIX(PlayerCharacter);
	DEFINE_MEMBER_FN(GetNumTints, UInt32, 0x006B5450, UInt32 tintType);
	DEFINE_MEMBER_FN(GetTintMask, TintMask *, 0x006B52A0, UInt32 tintType, UInt32 index);
	DEFINE_MEMBER_FN(GetDamage, float, 0x00693050, InventoryEntryData * pForm);
	DEFINE_MEMBER_FN(GetArmorValue, float, 0x00692CD0, InventoryEntryData * pForm);
};

STATIC_ASSERT(offsetof(PlayerCharacter, userEventEnabledEvent) == 0x2C0);
STATIC_ASSERT(offsetof(PlayerCharacter, numPerkPoints) == 0xB01);
STATIC_ASSERT(offsetof(PlayerCharacter, tintMasks) == 0xB10);
STATIC_ASSERT(offsetof(PlayerCharacter, overlayTintMasks) == 0xB28);
STATIC_ASSERT(offsetof(PlayerCharacter, unk3D8) == 0x3D8);
STATIC_ASSERT(offsetof(PlayerCharacter, lastRiddenHorseHandle) == 0x914);
STATIC_ASSERT(offsetof(PlayerCharacter, skills) == 0x9B0);
STATIC_ASSERT(offsetof(PlayerCharacter, tempPoison) == 0xA00);
STATIC_ASSERT(offsetof(PlayerCharacter, hostileHandles) == 0x9D8);
STATIC_ASSERT(offsetof(PlayerCharacter, currentWorldSpace) == 0x628);
STATIC_ASSERT(offsetof(PlayerCharacter, addedPerks) == 0x4B0);
STATIC_ASSERT(offsetof(PlayerCharacter, sameWorldSpace) == 0x988);
STATIC_ASSERT(offsetof(PlayerCharacter, unk890) == 0x890);
STATIC_ASSERT(sizeof(PlayerCharacter) == 0xBE0);


// 140 
class Explosion : public TESObjectREFR
{
	UInt8	unk098[0x140 - 0x98]; // 098 TODO
};

// 1A0 
class ChainExplosion : public Explosion
{
	UInt8	unk1A0[0x1A0 - 0x140]; // 140 TODO
};

// D8 
class Hazard : public TESObjectREFR
{
	enum { kTypeID = kFormType_Hazard };

	UInt64	unk98[8];	// 98 TODO
};

// 1D8
class Projectile : public TESObjectREFR
{
	// A8
	class LaunchData
	{
	public:
		virtual ~LaunchData();

		UInt8	unk08[0xA0]; // 08 TODO                                                             
	};

	UInt8	unk98[0x1D8 - 0x98];	// 98 TODO
};

// 1F8 
class BarrierProjectile : public Projectile
{
	enum { kTypeID = kFormType_BarrierProj };

	UInt32		unk1D8;	// 1D8
	UInt32		pad1DC;	// 1DC	
	UnkArray	unk1E0;	// 1E0
};
STATIC_ASSERT(sizeof(BarrierProjectile) == 0x1F8);

// 240 
class BeamProjectile : public Projectile
{
	enum { kTypeID = kFormType_BeamProj };

	UInt8	unk1D8[0x240 - 0x1D8]; // 1D8 TODO
};

// 218 
class ConeProjectile : public Projectile
{
	enum { kTypeID = kFormType_ConeProj };

	UInt8	unk1D8[0x218 - 0x1D8]; // 1D8 TODO
};

// 1E0 
class FlameProjectile : public Projectile
{
	enum { kTypeID = kFormType_FlameProj };

	UInt64	unk1D8; // 1D8
};

// 1E8 
class GrenadeProjectile : public Projectile
{
	enum { kTypeID = kFormType_Grenade };

	void*	unk1D8;		// 1D8 - Allocated in ctor
	UInt8	unk1E0;		// 1E0
	UInt8	unk1E1[7];	// 1E1 - probably padding
};

// 1E0 
class MissileProjectile  : public Projectile
{
	enum { kTypeID = kFormType_Missile };

	UInt32	unk1D8;		// 1D8
	UInt8	unk1DC;		// 1DC
	UInt8	pad1DD[3];	// 1DD
};

// 1F0 
class ArrowProjectile : public MissileProjectile
{
	enum { kTypeID = kFormType_Arrow };

	UInt64	unk1E0;
	UInt64	unk1E8;
};

// This does alot more, but no idea what it is yet
// ??
class CrosshairRefHandleHolder
{
	UInt32	unk00;					// 00
	UInt32	crosshairRefHandle;		// 04
	// ...

public:

	static CrosshairRefHandleHolder * GetSingleton(void);

	UInt32 CrosshairRefHandle(void) const		{ return crosshairRefHandle; }
};
