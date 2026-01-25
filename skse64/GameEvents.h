#pragma once

#include "skse64_common/Utilities.h"
#include "GameTypes.h"
#include "GameForms.h"

class TESObjectREFR;
class Actor;
class TESObjectBOOK;
class TESFullName;
class TESQuest;
class TESObjectWEAP;
class TESWordOfPower;
class Character;
class TESShout;
class TESFaction;
class BGSLocation;

template <typename T> class BSTEventSink;

enum EventResult
{
	kEvent_Continue = 0,
	kEvent_Abort
};

// 058 
template <typename EventT, typename EventArgT = EventT>
class EventDispatcher
{
	typedef BSTEventSink<EventT> SinkT;

	tArray<SinkT*>		eventSinks;			// 000
	tArray<SinkT*>		addBuffer;			// 018 - schedule for add
	tArray<SinkT*>		removeBuffer;		// 030 - schedule for remove
	SimpleLock			lock;				// 048
	bool				stateFlag;			// 050 - some internal state changed while sending
	char				pad[7];				// 051

	// Note: in SE there are multiple identical copies of all these functions 
	MEMBER_FN_PREFIX(EventDispatcher);
	// 66B1C7AC473D5EA48E4FD620BBFE0A06392C5885+66
	DEFINE_MEMBER_FN(AddEventSink_Internal, void, 0x0056B600, SinkT * eventSink);
	// ??_7BGSProcedureShoutExecState@@6B@ dtor | +43
	DEFINE_MEMBER_FN(RemoveEventSink_Internal, void, 0x00423B70, SinkT * eventSink);
	// D6BA7CEC95B2C2B9C593A9AEE7F0ADFFB2C10E11+456
	DEFINE_MEMBER_FN(SendEvent_Internal, void, 0x00177DC0, EventArgT * evn);

public:

	EventDispatcher() : stateFlag(false) {}

	void AddEventSink(SinkT * eventSink)		{ CALL_MEMBER_FN(this,AddEventSink_Internal)(eventSink); }
	void RemoveEventSink(SinkT * eventSink)		{ CALL_MEMBER_FN(this,RemoveEventSink_Internal)(eventSink); }
	void SendEvent(EventArgT * evn)				{ CALL_MEMBER_FN(this,SendEvent_Internal)(evn); }
};
STATIC_ASSERT(sizeof(EventDispatcher<void*>) == 0x58);

// 08 
template <typename T>
class BSTEventSink
{
public:
	virtual ~BSTEventSink() { };
	virtual	EventResult	ReceiveEvent(T * evn, EventDispatcher<T> * dispatcher) { return kEvent_Continue; }; // pure
//	void	** _vtbl;	// 00
};

// 58 
template <typename T>
class BSTEventSource
{
public:
	
	tArray<void*> unk00; // 08
	UnkArray	unk18;	// 18
	UnkArray	unk30;	// 30
	UInt32		unk48;	// 48
	UInt32		unk4C;	// 4C
	UInt8		unk50;	// 50
	UInt8		pad51[7];	// 51
};
STATIC_ASSERT(sizeof(BSTEventSource<void*>) == 0x58);

// 08 
struct TESSleepStartEvent
{
	float startTime;	// 00
	float endTime;		// 04
};

// 10 
struct MenuOpenCloseEvent
{
	BSFixedString	menuName;	// 00
	bool			opening;	// 08
	char			pad[7];
};

// Todo
struct MenuModeChangeEvent
{
};

class TESObjectREFR;
class TESForm;
class ActiveEffect;

struct TESActiveEffectApplyRemoveEvent
{
	TESObjectREFR	* caster;	// 0
	TESObjectREFR	* target;	// 8
	UInt16			unk10;		// 10
	UInt8			unk12;		// 12	
};

struct TESQuestStageEvent
{
	void			* finishedCallback;	// 0
	UInt32			formId;	// 8
	UInt16			stage;	// C
	UInt8			unk0E;	// E
	UInt8			pad;	// F
};

// This isn't necessarily correct, just there to receive events
struct TESHarvestEvent
{
	struct ItemHarvested
	{
		TESForm*	 object;
		TESForm*	 player;
	};
};

struct LevelIncrease
{
	struct Event
	{
		TESForm		* character;
		UInt32		level;
	};
};

struct SkillIncrease
{
	struct Event
	{
		TESForm*		character;
		UInt32		avId;
	};
};

struct WordLearned
{
	struct Event
	{
		bool	 first;
	};
};

struct WordUnlocked
{
	struct Event
	{
		bool	 third;
	};
};

struct Inventory
{
	struct Event
	{
		TESForm*	 character;
	};
};

struct Bounty
{
	struct Event
	{
		TESFaction*	faction;
		UInt32		before;
		UInt32		after;
	};
};

struct QuestStatus
{
	struct Event
	{
		TESQuest*	quest;
		UInt32		status;
	};
};

struct ObjectiveState
{
	struct Event
	{
		TESQuest::Objective* objective;
		UInt32	oldState;
		UInt32	newState;
	};
};

struct Trespass
{
	struct Event
	{
		// Unknown
	};
};

struct FinePaid
{
	struct Event
	{
		// Unknown
	};
};

struct HoursPassed
{
	struct Event
	{
		// Unknown
	};
};

struct DaysPassed
{
	struct Event
	{
		// Unknown
	};
};

struct DaysJailed
{
	struct Event
	{
		UInt32	days;
		BGSLocation*	 location;
		TESFaction*	 faction;
		UInt32	bounty;
	};
};

struct CriticalHitEvent
{
	struct Event
	{
		// Unknown
	};
};

struct DisarmedEvent
{
	struct Event
	{
		// Unknown
	};
};

struct ItemsPickpocketed
{
	struct Event
	{
		// Unknown
	};
};

struct ItemSteal
{
	struct Event
	{
		// Unknown
	};
};

struct ItemCrafted
{
	struct Event
	{
		// Unknown
	};
};

struct LocationDiscovery
{
	struct Event
	{
		TESFullName	* name;
		const char	* world;
	};
};

struct Jailing
{
	struct Event
	{
		// Unknown
	};
};

struct ChestsLooted
{
	struct Event
	{
		// Unknown
	};
};

struct TimesTrained
{
	struct Event
	{
		// Unknown
	};
};

struct TimesBartered
{
	struct Event
	{
		// Unknown
	};
};

struct ContractedDisease
{
	struct Event
	{
		// Unknown
	};
};

struct SpellsLearned
{
	struct Event
	{
		// Unknown
	};
};

struct DragonSoulGained
{
	struct Event
	{
		float	souls;
	};
};

struct SoulGemsUsed
{
	struct Event
	{
		// Unknown
	};
};

struct SoulsTrapped
{
	struct Event
	{
		TESObjectREFR*	player;
		TESObjectREFR*	target;
	};
};

struct PoisonedWeapon
{
	struct Event
	{
		// Unknown
	};
};

struct ShoutAttack
{
	struct Event
	{
		TESShout*	shout;
	};
};

struct JailEscape
{
	struct Event
	{
		// Unknown
	};
};

struct GrandTheftHorse
{
	struct Event
	{
		// Unknown
	};
};

struct AssaultCrime
{
	struct Event
	{
		UInt8	pad;
		TESObjectREFR*	target;
	};
};

struct MurderCrime
{
	struct Event
	{
		UInt8	pad;
		TESObjectREFR*	source;
		TESObjectREFR*	target;
	};
};

struct LocksPicked
{
	struct Event
	{
		// Unknown
	};
};

struct ShoutMastered
{
	struct Event
	{
		// Unknown
	};
};

struct TESCombatEvent 
{
	TESObjectREFR	* source;	// 00
	TESObjectREFR	* target;	// 08
	UInt32			state;		// 10
};

struct TESDeathEvent
{
	TESObjectREFR	* source;	// 00
	TESObjectREFR	* killer;	// 08
	UInt8			state;		// 10 -  0 - dying; 1 - death
};

struct TESHitEvent
{
	TESObjectREFR	* target;			// 00
	TESObjectREFR	* caster;			// 08
	UInt32			sourceFormID;		// 10
	UInt32			projectileFormID;	// 14

	enum
	{
		kFlag_PowerAttack = (1 << 0),
		kFlag_SneakAttack = (1 << 1),
		kFlag_Bash		  = (1 << 2),
		kFlag_Blocked	  = (1 << 3)
	};

	UInt32			flags;				// 18 ??	
};

struct TESUniqueIDChangeEvent
{
	UInt32	oldOwnerFormId;	// 0
	UInt32	newOwnerFormId;	// 4
	UInt32	formId;			// 8
	UInt16	oldUniqueId;	// C
	UInt16	newUniqueId;	// E
};

struct TESContainerChangedEvent
{
	UInt32	fromFormId;		// 0
	UInt32	toFormId;		// 4
	UInt32	itemFormId;		// 8
	UInt32	count;			// C
	UInt32	toReference;	// 10
	UInt16	unk14;			// 14
};

struct TESObjectLoadedEvent
{
	UInt32	formId;	// 00
	UInt8	loaded; // 04	- 01 - loaded, 00 - unloaded
};

struct TESCellAttachDetachEvent
{
	TESObjectREFR * reference;	// 00
	UInt8			attached;	// 08	- 01 - attached, 00 - detached
};

struct TESCellFullyLoadedEvent
{
	TESObjectCELL* cell;
};

struct TESMoveAttachDetachEvent
{
	TESObjectREFR * reference;	// 00
	UInt8			attached;	// 08	- 01 - attached, 00 - detached
};

struct TESInitScriptEvent
{
	TESObjectREFR * reference;
};

struct BGSFootstepEvent
{
	UInt32	actorHandle;
};

struct TESFurnitureEvent
{
	TESObjectREFR*	character;
	TESObjectREFR*	furniture;
	UInt32			state;
};

struct TESLoadGameEvent
{
	// empty
};

struct TESLockChangedEvent
{
	TESObjectREFR*	lock;
	TESObjectREFR*	unlocker;
};

struct TESQuestInitEvent
{
	UInt32	formId;
};

struct TESQuestStartStopEvent
{
	UInt32	formId;
};

struct TESTrackedStatsEvent
{
	BSFixedString	statName;
	UInt32			newValue;
};

struct TESWaitStartEvent
{
	float current;
	float desired;
};

struct TESWaitStopEvent
{
	//empty?
};

struct TESSwitchRaceCompleteEvent
{
	Actor* actor;
};

struct LocationCleared
{
	struct Event
	{
		//empty?
	};
};

struct BookRead
{
	struct Event
	{
		TESObjectBOOK* book;
	};
};

struct HourPassed
{
	struct Event
	{
		UInt32 sleep;
	};
};

struct ActorKill
{
	struct Event
	{
		TESObjectREFR*	killer;
		TESObjectREFR*	victim;
	};
};

struct CriticalHit
{
	struct Event
	{
		TESForm*		character;
		TESObjectWEAP*	weapon;
	};
};

struct Disarmed
{
	struct Event
	{
		TESObjectREFR*	source;
		TESObjectREFR*	target;
	};
};

struct WeaponAttack
{
	struct Event
	{
		TESObjectWEAP*	weapon;
		Character*		wielder; //unk
	};
};

struct DefaultObjectsReadyEvent
{
	struct Event
	{
		//g_defaultObjectManager ?
	};
};

class EventDispatcherList
{
public:
	EventDispatcher<void>								unk00;					//	00
	EventDispatcher<void>								unk58;					//  58  - sink offset 010
	EventDispatcher<TESActiveEffectApplyRemoveEvent>	unkB0;					//  B0  - sink offset 018
	EventDispatcher<void>								unk108;					//  108 - sink offset 020
	EventDispatcher<void>								unk160;					//  160 - sink offset 028
	EventDispatcher<TESCellAttachDetachEvent>			unk1B8;					//  1B8 - sink offset 030
	EventDispatcher<TESCellFullyLoadedEvent>			unk210;					//  210 - sink offset 038
	EventDispatcher<void>								unk2C0;					//  2C0 - sink offset 040
	EventDispatcher<TESCombatEvent>						combatDispatcher;		//  318 - sink offset 048
	EventDispatcher<TESContainerChangedEvent>			unk370;					//  370 - sink offset 050
	EventDispatcher<TESDeathEvent>						deathDispatcher;		//  3C8 - sink offset 058
	EventDispatcher<void>								unk420;					//  420 - sink offset 068
	EventDispatcher<void>								unk478;					//  478 - sink offset 070
	EventDispatcher<void>								unk4D0;					//  4D0 - sink offset 078
	EventDispatcher<void>								unk528;					//  528 - sink offset 080
	EventDispatcher<void>								unk580;					//  580 - sink offset 088
	EventDispatcher<void>								unk5D8;					//  5D8 - sink offset 090
	EventDispatcher<void>								unk630;					//  630 - sink offset 098
	EventDispatcher<TESInitScriptEvent>					initScriptDispatcher;	//  688 - sink offset 0A0
	EventDispatcher<TESLoadGameEvent>					loadGameEventDispatcher;//  6E0 - sink offset 0A8
	EventDispatcher<void>								unk738;					//  738 - sink offset 0B0
	EventDispatcher<void>								unk790;					//  790 - sink offset 0B8
	EventDispatcher<void>								unk7E8;					//  7E8 - sink offset 0C0
	EventDispatcher<void>								unk840;					//  840 - sink offset 0C8
	EventDispatcher<TESObjectLoadedEvent>				objectLoadedDispatcher;	//  898 - sink offset 0D0
	EventDispatcher<void>								unk8F0;					//  8F0 - sink offset 0D8
	EventDispatcher<void>								unk948;					//  948 - sink offset 0E0
	EventDispatcher<void>								unk9A0;					//  9A0 - sink offset 0E8
	EventDispatcher<void>								unk9F8;					//  9F8 - sink offset 0F0
	EventDispatcher<void>								unkA50;					//  A50 - sink offset 0F8
	EventDispatcher<void>								unkAA8;					//  AA8 - sink offset 100
	EventDispatcher<void>								unkB00;					//  B00 - sink offset 108
	EventDispatcher<void>								unkB58;					//  B58 - sink offset 110
	EventDispatcher<void>								unkBB0;					//  BB0 - sink offset 118
	EventDispatcher<void>								unkC08;					//  C08 - sink offset 120
	EventDispatcher<void>								unkC60;					//  C60 - sink offset 128
	EventDispatcher<void>								unkCB8;					//  CB8 - sink offset 130
	EventDispatcher<void>								unkD10;					//  D10 - sink offset 138
	EventDispatcher<void>								unkD68;					//  D68 - sink offset 140
	EventDispatcher<void>								unkDC0;					//  DC0 - sink offset 148
	EventDispatcher<void>								unkE18;					//  E18 - sink offset 150
	EventDispatcher<void>								unkE70;					//  E70 - sink offset 158
	EventDispatcher<void>								unkEC8;					//  EC8 - sink offset 160
	EventDispatcher<void>								unkF20;					//  F20 - sink offset 168
	EventDispatcher<void>								unkF78;					//  F78 - sink offset 170
	EventDispatcher<void>								unkFD0;					//  FD0 - sink offset 178
	EventDispatcher<void>								unk1028;				// 1028 - sink offset 180
	EventDispatcher<void>								unk1080;				// 1080 - sink offset 188
	EventDispatcher<void>								unk10D8;				// 10D8 - sink offset 190
	EventDispatcher<TESUniqueIDChangeEvent>				uniqueIdChangeDispatcher;				// 1130 - sink offset 198
	EventDispatcher<void>								unk1188;				// 1188 - sink offset 200
	EventDispatcher<void>								unk11E0;				// 11E0 - sink offset 208
	EventDispatcher<void>								unk1238;				// 1238 - sink offset 210
	EventDispatcher<void>								unk1290;				// 1290 - sink offset 218
};
STATIC_ASSERT(offsetof(EventDispatcherList, uniqueIdChangeDispatcher) == 0x10D8);

typedef EventDispatcherList*(*_GetEventDispatcherList)();
extern RelocAddr<_GetEventDispatcherList> GetEventDispatcherList;

// For testing
//extern EventDispatcher<TESSleepStartEvent> * g_sleepStartEventDispatcher;
//extern EventDispatcher<TESCombatEvent> * g_combatEventDispatcher;
//extern EventDispatcher<TESDeathEvent> * g_deathEventDispatcher;
//extern EventDispatcher<BGSFootstepEvent> * g_footstepEventDispatcher;
//extern EventDispatcher<TESQuestStageEvent> * g_questStageEventDispatcher;
//extern EventDispatcher<TESHarvestEvent::ItemHarvested> * g_harvestEventDispatcher;
//extern EventDispatcher<LevelIncrease::Event> * g_levelIncreaseEventDispatcher;
//extern EventDispatcher<TESHitEvent> * g_hitEventDispatcher;
//extern EventDispatcher<TESContainerChangedEvent> * g_containerChangedEventDispatcher;
//extern EventDispatcher<TESUniqueIDChangeEvent> * g_changeUniqueIDEventDispatcher;
//extern EventDispatcher<TESObjectLoadedEvent> * g_objectLoadedEventDispatcher;
//extern EventDispatcher<TESInitScriptEvent> * g_initScriptEventDispatcher;
//extern EventDispatcher<TESCellAttachDetachEvent> * g_cellAttachDetachEventDispatcher;
//extern EventDispatcher<TESMoveAttachDetachEvent> * g_moveAttachDetachEventDispatcher;
