#pragma once

#include "skse64/GameTypes.h"
#include "skse64/GameEvents.h"
#include "skse64/GameCamera.h"
#include "skse64/GameReferences.h"

#include "skse64/ScaleformCallbacks.h"
#include "skse64/ScaleformMovie.h"

#include "skse64_common/Utilities.h"
#include "skse64/GameUtilities.h"
//#include "skse64/NiNodes.h"

class TESObjectREFR;
class TESFullName;

class InventoryEntryData;

class UIDelegate;
class UIDelegate_v1;

class UIMessage;

//// menu implementations

// 30
class IMenu : public FxDelegateHandler
{
	struct BSUIScaleformData
	{
		virtual ~BSUIScaleformData() {}

		//	void	** _vtbl;		// 00
		UInt32				unk04; // 04
		void*				unk08; // 08
	};

	struct UnkData1
	{
		BSFixedString		name;	// 00
		UInt32				unk04;	// 04
		BSUIScaleformData*	data;	// 08 - BSUIScaleformData
	};

public:
	IMenu();
	virtual ~IMenu() { CALL_MEMBER_FN(this, dtor)(); } // TODO

	enum
	{
		kFlag_None = 0,
		kFlag_PausesGame = 1 << 0,
		kFlag_AlwaysOpen = 1 << 1,
		kFlag_UsesCursor = 1 << 2,
		kFlag_UsesMenuContext = 1 << 3,
		kFlag_Modal = 1 << 4,  // prevents lower movies with this flag from advancing
		kFlag_FreezeFrameBackground = 1 << 5,
		kFlag_OnStack = 1 << 6,
		kFlag_DisablePauseMenu = 1 << 7,
		kFlag_RequiresUpdate = 1 << 8,
		kFlag_TopmostRenderedMenu = 1 << 9,
		kFlag_UpdateUsesCursor = 1 << 10,
		kFlag_AllowSaving = 1 << 11,
		kFlag_RendersOffscreenTargets = 1 << 12,
		kFlag_InventoryItemMenu = 1 << 13,
		kFlag_DontHideCursorWhenTopmost = 1 << 14,
		kFlag_CustomRendering = 1 << 15,
		kFlag_AssignCursorToRenderer = 1 << 16,
		kFlag_ApplicationMenu = 1 << 17,
		kFlag_HasButtonBar = 1 << 18,
		kFlag_IsTopButtonBar = 1 << 19,
		kFlag_AdvancesUnderPauseMenu = 1 << 20,
		kFlag_RendersUnderPauseMenu = 1 << 21,
		kFlag_UsesBlurredBackground = 1 << 22,
		kFlag_CompanionAppAllowed = 1 << 23,
		kFlag_FreezeFramePause = 1 << 24,
		kFlag_SkipRenderDuringFreezeFrameScreenshot = 1 << 25,
		kFlag_LargeScaleformRenderCacheMode = 1 << 26,
		kFlag_UsesMovementToDirection = 1 << 27
	};

	virtual void	Accept(CallbackProcessor * processor) {}
	virtual void	Unk_02(void) {}
	virtual void	Unk_03(void) {}
	virtual UInt32	ProcessMessage(UIMessage* message) { return CALL_MEMBER_FN(this, ProcessMessage_internal)(message); }
	virtual void	NextFrame(UInt32 arg0, UInt32 arg1) { CALL_MEMBER_FN(this, NextFrame_internal)(arg0, arg1); }
	virtual void	Render(void);
	virtual void	Unk_07(void) {}
	virtual void	InitMovie(void) { CALL_MEMBER_FN(this, InitMovie_internal)(view); }

	GFxMovieView	* view;		// 10 - init'd to 0, a class, virtual fn 0x228 called in dtor
	UInt8			unk0C;		// 18 - init'd to 3
	UInt8			pad19[3];	// 19
	UInt32			flags;		// 1C - init'd to 0
	UInt32			unk14;		// 20 - init'd to 0x12
	UInt32			pad24;		// 24 
	GRefCountBase	* unk18;	// 28 - holds a reference

	MEMBER_FN_PREFIX(IMenu);
	DEFINE_MEMBER_FN(InitMovie_internal, void, 0x00ECD970, GFxMovieView* view);
	DEFINE_MEMBER_FN(NextFrame_internal, void, 0x00ECD8A0, UInt32 arg0, UInt32 arg1);
	DEFINE_MEMBER_FN(ProcessMessage_internal, UInt32, 0x00ECD850, UIMessage* message);
	DEFINE_MEMBER_FN(dtor, void, 0x00ECD7F0);
};
STATIC_ASSERT(offsetof(IMenu, view) == 0x10);

// 58 
class Console : public IMenu
{
public:
	// unk0C - 0x0C
	// Flags - 0x807
	// unk14 - 2

	void	* opcodeInfo;	// 30 - constructor checks that opcodes match
	UnkArray	unk38;		// 38 - probably history linked list?
	UInt32		unk50;		// 50 - init'd to 0
	UInt8		unk54;		// 54 - init'd to 0
	UInt8		pad55[3];	// 55
};

// A8
class BarterMenu : public IMenu
{
	// unk0C - 0
	// Flags - 0xA489
	// unk14 - 3
	GFxValue	* root;		// 30
	UInt64		unk38;		// 38
	UInt64		unk40;		// 40
	UInt64		unk48;		// 48
	UInt32		unk50;		// 50
	UInt32		unk54;		// 54
	UInt32		unk58;		// 58
	UInt32		pad5C;		// 5C
	UInt64		unk60;		// 60
	UInt64		unk68;		// 68
	UInt64		unk70;		// 70
	UInt8		unk78;		// 78
	UnkArray	unk80;		// 80
	UInt32		unk98;		// 98
	UInt32		unk9C;		// 9C
	UInt32		unkA0;		// A0
	bool		bPcControlsReady;	// A4
	UInt8		padA5[3];	// A5
};

class BGSHeadPart;
class TESRace;

// 138
class RaceMenuSlider
{
public:
	RaceMenuSlider::RaceMenuSlider() {};
	RaceMenuSlider::RaceMenuSlider(UInt32 filterFlag, const char * sliderName, const char * callbackName, UInt32 sliderId, UInt32 index, UInt32 type, UInt8 unk8, float min, float max, float value, float interval, UInt32 unk13);

	enum {
		kTypeHeadPart = 0,
		kTypeUnk1,
		kTypeDoubleMorph,
		kTypePreset,
		kTypeTintingMask,
		kTypeHairColorPreset,
		kTypeUnk6,
		kTypeUnused7,
		kTypeUnk8,
		kTypeUnk9,
		kTypeUnk10
	};

	float	min;	// 00
	float	max;	// 04
	float	value;	// 08
	float	interval;	// 0C
	UInt32	filterFlag;	// 10
	UInt32	type;	// 14
	const char	* name;	// 18
	char	callback[MAX_PATH];	// 20
	UInt32	index;	// 124
	UInt32	id;	// 128
	UInt32	unk12C;	// 12C
	UInt32	unk130;	// 130 - 0x7F7FFFFF
	UInt8	unk134;	// 134
	UInt8	pad135[3]; // 135
};
STATIC_ASSERT(sizeof(RaceMenuSlider) == 0x138);

// 1A8
class RaceSexMenu : public IMenu
{
public:
	// unk0C - 3
	// Flags - 0x709
	// unk14 - 3
	void					* menuHandler;	// 30
	UInt32					unk38;			// 38
	UInt8					unk3C;			// 3C
	UInt8					unk3D;			// 3D
	UInt16					pad3E;			// 3E

	enum {
		kHeadPartsHairLine = 0,
		kHeadPartsHead,
		kHeadPartsEyes,
		kHeadPartsHair,
		kHeadPartsBeard,
		kHeadPartsScars,
		kHeadPartsBrows,
		kNumHeadPartLists
	};
	tArray<BGSHeadPart*>	headParts[kNumHeadPartLists];	// 40
	/*tArray<BGSHeadPart*>	hairline;		// 28
	tArray<BGSHeadPart*>	head;			// 34
	tArray<BGSHeadPart*>	eyes;			// 40
	tArray<BGSHeadPart*>	hair;			// 4C
	tArray<BGSHeadPart*>	beard;			// 58
	tArray<BGSHeadPart*>	scars;			// 64
	tArray<BGSHeadPart*>	brows;			// 70*/
	RaceSexCamera			camera;			// E8

	float					unk120[0x07];	// 120
	UInt32					pad13C;			// 13C

	struct RaceComponent
	{
		TESRace				* race;			// 00
		tArray<RaceMenuSlider>	sliders;	// 08
		UInt32				unk20;			// 20
		UInt32				pad24;			// 24
	};

	tArray<RaceComponent>	sliderData[2];	// 140
	UnkArray				unk170;			// 170
	UInt32					raceIndex;		// 188
	UInt32					unk18C;			// 18C
	UInt32					unk190;			// 190
	UInt32					unk194;			// 194
	UInt32					unk198;			// 198
	UInt8					unk19C;			// 19C
	UInt8					unk19D;			// 19D
	UInt8					unk19E;			// 19E - init'd to 1
	UInt8					unk19F;			// 19F
	UInt8					unk1A0;			// 1A0
	UInt8					unk1A1;			// 1A1
	UInt8					unk1A2;			// 1A2
	UInt8					unk1A3;			// 1A3
	UInt8					unk1A4;			// 1A4
	UInt8					unk1A5;			// 1A5
	UInt16					pad1A6;			// 1A6


	MEMBER_FN_PREFIX(RaceSexMenu);
	// FCFD4B12540A068252C0A71C5480E518DDF6EF60+58
	DEFINE_MEMBER_FN(LoadSliders, void *, 0x008B5E20, UInt64 unk1, UInt8 unk2);
};
STATIC_ASSERT(offsetof(RaceSexMenu, sliderData) == 0x140);
STATIC_ASSERT(offsetof(RaceSexMenu, raceIndex) == 0x188);

// 30598
class MapMenu : public IMenu
{
public:
	// unk0C - 3
	// Flags - 0x9005
	// unk14 - 7
	enum
	{
		kMarkerType_Location = 0
	};

	// 38
	struct MarkerData
	{
		// mostly wrong
		TESFullName * name;			// 00
		UInt32		refHandle;		// 08
		UInt32		pad0C;			// 0C
		void		* unk10;		// 10
		UInt32		unk18;			// 18
		UInt32		unk1C;			// 1C
		UInt32		unk20;			// 20
		UInt32		unk24;			// 24
		void		* unk28;		// 28
		UInt32		unk30;			// 30
		UInt32		unk34;			// 34
	};

	// 30400
	struct LocalMap
	{
		UnkArray				unk000;							// 000
		GFxValue				markerData;						// 018
		float					unk030;							// 030
		float					unk034;							// 034
		float					unk038;							// 038
		float					unk03C;							// 03C

		// SE: this structure growed by a lot! 
		// FIXME: Replace when LocalMapCullingProcess has a size of 0x30360
		//LocalMapCullingProcess	cullingProcess;				// 40 /// ---------------- commented out but needs to be sorted out by size
		UInt8					cullingProcess[0x30360];		// 40

		// TODO: these 4 should go in their own structure
		NiRenderedTexture		* renderedLocalMapTexture;		// 303A0
		UInt32					unk303A8;						// 303A8
		UInt32					pad303AC;						// 303AC
		UInt64					unk303B0;						// 303B0

		GFxValue				localMapRoot;					// 303B8
		GFxValue				mapRoot;						// 303D0
		GFxMovieView			* view;							// 303E8
		void					* localMapInputHandler;			// 303F0
		UInt32					unk303F8;						// 303F8 - init'd to FFFFFFFF
		UInt8					unk303FC[4];					// 303FC
	};

	void				* eventSinkMenuOpenCloseEvent; 			// 030
	void				* mapCameraCallback;					// 038
	UInt64				mapMoveHandler; 						// 040
	UInt64				mapLookHandler; 						// 048
	UInt64				mapZoomHandler; 						// 050
	UInt32				unk058; 								// 058
	UInt32				unk05C; 								// 05C
	LocalMap			localMap;								// 060
	UInt32				unk30460;								// 30460
	float				unk30464;								// 30464
	float				unk30468;								// 30468
	float				unk3046C;								// 3046C
	tArray<MarkerData>	markers;								// 30470	
	UInt8				todo[0x30598 - 0x30488];				// 30488
};
STATIC_ASSERT(offsetof(MapMenu, localMap) == 0x60);
STATIC_ASSERT(offsetof(MapMenu::LocalMap, cullingProcess) == 0x40);
STATIC_ASSERT(sizeof(MapMenu::LocalMap::cullingProcess) == 0x30360);
STATIC_ASSERT(offsetof(MapMenu::LocalMap, renderedLocalMapTexture) == 0x303A0);
STATIC_ASSERT(offsetof(MapMenu, markers) == 0x30470);
STATIC_ASSERT(sizeof(MapMenu) == 0x030598);

// 28
class HUDObject
{
public:
	HUDObject::HUDObject(GFxMovieView* movie)
	{
		if (movie)
			InterlockedIncrement(&movie->refCount);
		view = movie;
	}
	virtual ~HUDObject(void)
	{
		object.CleanManaged();

		GFxMovieView * thisView = view;
		if (thisView)
			thisView->ForceCollectGarbage();
	}

	virtual void Update(void) = 0;	// Called per-frame
	virtual UInt8 Unk_02(void * unk1) { return 0; };
	virtual void * Unk_03(void * unk1) { return Impl_Fn03(unk1); };
	virtual void Unk_04(void) { }; // No implementation?

	GFxMovieView	* view;		// 08
	GFxValue		object;		// 10

	MEMBER_FN_PREFIX(HUDObject);
	DEFINE_MEMBER_FN_0(dtor, void, 0x00885C40);
	DEFINE_MEMBER_FN_1(Impl_Fn03, void *, 0x00880140, void * unk1);

	DEFINE_STATIC_HEAP(Heap_Allocate, Heap_Free);
};
STATIC_ASSERT(sizeof(HUDObject) == 0x28);

// 40
class Compass : public HUDObject
{
public:
	UInt64	unk28;	// 28
	UInt32	unk30;	// 30
	UInt32	unk34;	// 34
	UInt64	unk38;	// 38
};
STATIC_ASSERT(sizeof(Compass) == 0x40);

// B8
class FloatingQuestMarker : public HUDObject
{
public:

	UInt8	todo[0xB8 - 0x28];	// 28

};

// 60
class Notification
{
public:
	Notification() : type(0), quest(nullptr), word(nullptr), time(0) { }
	Notification(const Notification& other);
	~Notification() { dtor(); }
	
	BSString		text; 	// 00 - size 10
	BSString		status;	// 10 - size 10
	BSFixedString	sound;	// 20 - size 08
	tArray<BSFixedString> objectives; // 28 - size 18
	UInt64			type;	// 40
	TESQuest*		quest;	// 48
	TESWordOfPower*	word;	// 50
	UInt32			time;	// 58 - g_gameTime + iObjectivesWaitTime

	DEFINE_MEMBER_FN_0(dtor, void, 0x00885400);
};

// 78
class HUDNotifications : public HUDObject
{
public:
	tArray<Notification> queue; // 28 - size 18
	GFxValue		object;	// 40 - size 18 - .Invoke("CanShowNotification")
	GFxValue		array;	// 58 - size 18 - .GetType() == 7
	UInt32		time;	// 70 - last meter update time
	float		points;	// 74 - cached meter progress
};

// 88
class EnemyHealth : public HUDObject
{
public:
	UInt32			handle;			// 28
	UInt32			unk2C;			// 2C
	UInt32			unk30;			// 30
	UInt32			unk34;			// 34
	GFxValue		unk38;			// 38
	GFxValue		unk50;			// 50
	GFxValue		text;			// 68
	UInt32			unk5C;			// 5C
	UInt8			unk80;			// 80
	UInt8			unk81[7];		// 81

	NiPointer<TESObjectREFR> GetTarget() const;
};
STATIC_ASSERT(offsetof(EnemyHealth, handle) == 0x28);

// 98
class StealthMeter : public HUDObject
{
public:

	UInt8	todo[0x98 - 0x28];	// 28
};

// 30
class HUDChargeMeter : public HUDObject
{
public:

	float	unk28;	// 28 - init'd to 100
	float	unk2C;	// 2C - init'd to 100
};

// 50
class HUDMeter : public HUDObject
{
public:
	virtual double GetMaxValue(void);

	char	* setMeterPercent;	// 28
	char	* startBlinking;	// 30
	char	* fadeOut;			// 38
	float	unk40;				// 40 - init'd to 100
	UInt32	unk44;				// 44
	UInt64	unk48;				// 48 - TODO: may be 32 
};
STATIC_ASSERT(sizeof(HUDMeter) == 0x50);

// 50
class ActorValueMeter : public HUDMeter
{
public:

};

// 50
class ShoutMeter : public HUDMeter
{
public:

};

// 98
class HUDMenu : public IMenu
{
public:
	BSTEventSink<void>	unk30;			// 30 - UserEventEnabledEvent
	void				*unk38;			// 38 - New in SE
	tArray<HUDObject*>	hudComponents;	// 40
	ActorValueMeter		*unk58;			// 58
	ActorValueMeter		*unk60;			// 60
	ActorValueMeter		*unk68;			// 68
	ShoutMeter			*unk70;			// 70
	UInt64				unk78;			// 78
	UInt32				unk80;			// 80
	UInt32				unk84;			// 84
	UInt64				unk88;			// 88
	UInt8				unk90;			// 90
	UInt8				unk91[7];		// 91
};
STATIC_ASSERT(sizeof(HUDMenu) == 0x98);

// 38
class CraftingMenu : public IMenu
{
public:

	UInt64	unk30;	// 30
};

// ???
class CraftingSubMenu : public FxDelegateHandler
{
public:
	virtual ~CraftingSubMenu();

	UInt64			unk10;		// 010
	GFxMovieView*	view;		// 018
	// ...
};

STATIC_ASSERT(offsetof(CraftingSubMenu, view) == 0x018);

// 220
class EnchantConstructMenu : public CraftingSubMenu
{
public:
	enum
	{
		kFilterFlag_EnchantWeapon = 0x1,
		kFilterFlag_DisenchantWeapon = 0x2,
		kFilterFlag_EnchanteArmor = 0x4,
		kFilterFlag_DisenchantArmor = 0x8,
		kFilterFlag_EffectWeapon = 0x10,
		kFilterFlag_EffectArmor = 0x20,
		kFilterFlag_SoulGem = 0x40
	};

	// 18
	class CategoryListEntry
	{
	public:
		virtual ~CategoryListEntry();

		virtual void Unk1();
		virtual void Unk2();
		virtual void Unk3(); // pure
		virtual void SetData(GFxValue* dataContainer);

		//	void		** _vtbl;	// 00
		UInt32		unk08;		// 08
		UInt32		filterFlag;	// 0C
		UInt8		bEquipped;	// 10
		UInt8		bEnabled;	// 11
		UInt8		pad12[6];	// 12

		MEMBER_FN_PREFIX(CategoryListEntry);
		DEFINE_MEMBER_FN(SetData, void, 0x0086F130, GFxValue* target);

		void SetData_Extended(EnchantConstructMenu*	subMenu, GFxValue* target);
	};

	// 020
	class ItemChangeEntry : public CategoryListEntry
	{
	public:
		InventoryEntryData*	data;	// 18		
	};

	// 028
	class EnchantmentEntry : public CategoryListEntry
	{
	public:
		EnchantmentItem*	data;	// 18
		float				unk20;	// 20 - init'd to 1
		float				unk24;	// 24 - init'd to 1
	};

	// ...
};

// 168
class SmithingMenu : public CraftingSubMenu
{
public:


	// ...
};

// 160
class ConstructibleObjectMenu : public CraftingSubMenu
{
public:
	// 08
	struct EntryData
	{
		BGSConstructibleObject*	object;			// 00
		UInt32					filterFlag;		// 08
	};

	// ...
};

// Declared outside of AlchemyMenu for forward decls
// 08
struct AlchemyEffectCategory
{
	UInt32 formId;
	UInt32 unk1;
};

// 1A0
class AlchemyMenu : public CraftingSubMenu
{
public:
	// 10
	struct EntryData
	{
		InventoryEntryData*	data;		// 00
		UInt32				filterFlag;	// 08
		UInt8				bEquipped;	// 0C
		UInt8				bEnabled;	// 0D
		UInt16				pad0E;		// 0E		
	};

	// ...
};



// HUDMenu
// unk0C - 2
// Flags - 0x18902
// unk14 - 0x12

// DialogueMenu
// unk0C - 3
// Flags - 0x4400
// unk14 - 1

// MainMenu
// unk0C - 9
// Flags - 0x581
// unk14 - 1

// MagicMenu
// unk0C - 0
// Flags - 0xA489
// unk14 - 3

// InventoryMenu
// unk0C - 0
// Flags - 0x4400
// unk14 - 0x12

//// menu management

// 10
class IUIMessageData
{
public:
	virtual ~IUIMessageData();

	//	void	** _vtbl;	// 00
	UInt8	unk08;		// 08
	UInt8	unk09;		// 09
	UInt8	pad0A[6];	// 0A
};

// 28
class BSUIMessageData : public IUIMessageData
{
public:
	BSString			* unk08;	// 10
	StringCache::Ref	unk18;		// 18
	UInt32				unk20;		// 20
	UInt32				pad24;		// 24
};

// 18
class RefHandleUIData : public IUIMessageData
{
public:
	UInt32	refHandle;	// 10
	UInt32	pad14;		// 14
};

class HUDData : public IUIMessageData
{
public:
	enum MessageType
	{
		kType_ShowMessage = 1,
		kType_SetCrosshairTarget = 3,
		kType_SetLoadDoorInfo = 4,
		kType_ShowSubtitle = 5,
		kType_HideSubtitle = 6,
		kType_ShowArrowCount = 7,
		kType_HideArrowCount = 8,
		kType_SetSubtitlesEnabled = 9,
		kType_QuestStarted = 14,
		kType_QuestCompleted = 15,
		kType_QuestUpdated = 16,
		kType_NewObjective = 17,
		kType_ObjectiveCompleted = 18,
		kType_ObjectiveFailed = 19,
		kType_SkillIncreased = 20,
		kType_WorldOfPowerLearned = 21,
		kType_DragonSoulAbsorbed = 22,
		kType_ShowElements = 23,
		kType_LocationDiscoverd = 26,
		kType_Favor = 27,
		kType_ValidateCrosshair = 28,
		kType_SetLocationName = 29,
		kType_ShowTutrialHintText = 30,
		kType_SetCrosshairEnabled = 31,
		kType_Unk32 = 32,
		kType_RefreshActivateButtonArt = 33,
		kType_Unk34 = 34	

	};

	UInt32			type;	// 10
	UInt8			pad14[4]; // or UInt64 with 'type'?
	BSString			text;	// 18 - size 10
	void	*			unk14;	// 28 - size 8
	TESQuest*		quest;	// 30 - size 8
	TESWordOfPower*	word;	// 38 - size 8
	bool				unk40;	// 40 - some flag
	UInt8			pad41[3];
	UInt32			unk44;	// 44 - location type

};

typedef void * (*_CreateUIMessageData)(BSFixedString * name);
extern RelocAddr<_CreateUIMessageData> CreateUIMessageData;

// 20
// ### pool added in 1.3 (or maybe 1.2)
class UIMessage
{
public:
	enum
	{
		kMessage_Refresh = 0,	// used after ShowAllMapMarkers
		kMessage_Open,
		kMessage_PreviouslyKnownAsClose,
		kMessage_Close
	};

	StringCache::Ref	strData;	// 00
	UInt32				message;	// 08
	UInt32				pad0C;		// 0C
	IUIMessageData		* objData;	// 10 - something with a virtual destructor
	UInt8				isPooled;	// 18
	UInt8				pad19[7];	// 19
};

// 08
template <typename T>
class BSTMessageQueue
{
public:
	BSTMessageQueue();
	virtual ~BSTMessageQueue();

	virtual bool	Push(T * obj);		// pure, add (loop until lock taken, call Fn05)
	virtual bool	TryPush(T * obj);	// pure, try add (try to take lock, return false if already taken, else call Fn05)
	virtual bool	Pop(T * obj);		// pure, remove (loop until lock taken, call Fn06)
	virtual bool	TryPop(T * obj);	// pure, try remove (try to take lock, return false if already taken, else call Fn06)

//	void	** _vtbl;	// 00
};

// 10
template <typename T>
class BSTCommonMessageQueue : public BSTMessageQueue <T>
{
protected:
	virtual bool	PushInternal(T * obj);	// pure
	virtual bool	PopInternal(T * obj);	// pure

public:
	volatile UInt32	lock;	// 08
	UInt32	pad0C;	// 0C
};

template <typename T>
class BSTCommonScrapHeapMessageQueue : public BSTCommonMessageQueue<T>
{
public:
	BSTCommonScrapHeapMessageQueue();
	virtual ~BSTCommonScrapHeapMessageQueue();

	UInt64 unk10;	// 10
	UInt64 unk18;	// 18
	UInt64 unk20;	// 20
};

// 10 + sizeof(T) * T_len + 10, aligned to 8
template <typename T, UInt32 T_len>
class BSTCommonStaticMessageQueue : public BSTCommonMessageQueue <T>
{
public:
	T		data[T_len];	// 010
	UInt32	numEntries;		// 330 - offsets are for <UIMessage *, 100>
	UInt32	writePtr;		// 334
	UInt32	readPtr;		// 338
};

// B80
class UIManager
{
public:
	enum
	{
		kPoolSize = 0x40,
	};

	typedef BSTCommonStaticMessageQueue <UIMessage *, 100>	MessageQueue;

	UInt64			unk000;		// 000
	MessageQueue	messages;	// 008
	UInt8			pad348[0x378 - 0x348];	// 348
	UInt32			poolUsed;	// 378
	UInt32			pad37C;		// 37C
	UIMessage		messagePool[kPoolSize];	// 380

	MEMBER_FN_PREFIX(UIManager);
	// this takes ownership of the message ptr
//	DEFINE_MEMBER_FN(AddMessage, void, 0x004503E0, UIMessage * msg);	// old 1.1 implementation
	// 1.3 uses a little non-thread-safe pool of UIMessages to wrap around the nicely thread-safe BSTMessageQueue it gets added to
	DEFINE_MEMBER_FN(AddMessage, void, 0x001652D0, StringCache::Ref * strData, UInt32 msgID, void * objData);
	DEFINE_MEMBER_FN(CreateUIMessageData, IUIMessageData *, 0x00EC31D0, const BSFixedString &type);

	static UIManager *	GetSingleton(void)
	{
		// CB598A8812CFF3959F94DC8F8371BA876F68AE34+67
		static RelocPtr<UIManager*> g_UIManager(0x01EC0A70);
		return *g_UIManager;
	}

	// Used by Hooks_UI
	void ProcessCommands(void);
	void QueueCommand(UIDelegate* cmd);
	void QueueCommand(UIDelegate_v1* cmd);

	DEFINE_MEMBER_FN(ProcessEventQueue_HookTarget, void, 0x00EC30F0);
};
STATIC_ASSERT(offsetof(UIManager, pad348) == 0x348);
STATIC_ASSERT(sizeof(UIManager) == 0xB80);

// 258
class UIStringHolder
{
public:
	void			* unk00;					// 000
	BSFixedString	faderData;					// 008 "FaderData"
	BSFixedString	hudData;					// 010 "HUDData"
	BSFixedString	hudCamData;					// 018 "HUDCamData"
	BSFixedString	floatingQuestMarkers;		// 020 "FloatingQuestMarkers"
	BSFixedString	consoleData;				// 028 "ConsoleData"
	BSFixedString	quantityData;				// 030 "QuantityData"
	BSFixedString	messageBoxData;				// 038 "MessageBoxData"
	BSFixedString	bsUIScaleformData;			// 040 "BSUIScaleformData"
	BSFixedString	bsUIMessageData;			// 048 "BSUIMessageData"
	BSFixedString	bsUIAnalogData;				// 050 "BSUIAnalogData"
	BSFixedString	inventoryUpdateData;		// 058 "InventoryUpdateData"
	BSFixedString	refHandleUIData;			// 060 "RefHandleUIData"
	BSFixedString	tesFormUIData;				// 068 "TESFormUIData"
	BSFixedString	loadingMenuData;			// 070 "LoadingMenuData"
	BSFixedString	kinectStateData;			// 078 "KinectStateChangeData"
	BSFixedString	kinectUserEventData;		// 080 "KinectUserEventData"
	BSFixedString	inventoryMenu;				// 088 "InventoryMenu"
	BSFixedString	console;					// 090 "Console"
	BSFixedString	dialogueMenu;				// 098 "Dialogue Menu"
	BSFixedString	hudMenu;					// 0A0 "HUD Menu"
	BSFixedString	mainMenu;					// 0A8 "Main Menu"
	BSFixedString	messageBoxMenu;				// 0B0 "MessageBoxMenu"
	BSFixedString	cursorMenu;					// 0B8 "Cursor Menu"
	BSFixedString	faderMenu;					// 0C0 "Fader Menu"
	BSFixedString	magicMenu;					// 0C8 "MagicMenu"
	BSFixedString	topMenu;					// 0D0 "Top Menu"
	BSFixedString	overlayMenu;				// 0D8 "Overlay Menu"
	BSFixedString	overlayInteractionMenu;		// 0E0 "Overlay Interaction Menu"
	BSFixedString	loadingMenu;				// 0E8 "Loading Menu"
	BSFixedString	tweenMenu;					// 0F0 "TweenMenu"
	BSFixedString	barterMenu;					// 0F8 "BarterMenu"
	BSFixedString	giftMenu;					// 100 "GiftMenu"
	BSFixedString	debugTextMenu;				// 108 "Debug Text Menu"
	BSFixedString	mapMenu;					// 110 "MapMenu"
	BSFixedString	lockpickingMenu;			// 118 "Lockpicking Menu"
	BSFixedString	quantityMenu;				// 120 "Quantity Menu"
	BSFixedString	statsMenu;					// 128 "StatsMenu"
	BSFixedString	containerMenu;				// 130 "ContainerMenu"
	BSFixedString	sleepWaitMenu;				// 138 "Sleep/Wait Menu"
	BSFixedString	levelUpMenu;				// 140 "LevelUp Menu"
	BSFixedString	journalMenu;				// 148 "Journal Menu"
	BSFixedString	bookMenu;					// 150 "Book Menu"
	BSFixedString	favoritesMenu;				// 158 "FavoritesMenu"
	BSFixedString	raceSexMenu;				// 160 "RaceSex Menu"
	BSFixedString	craftingMenu;				// 168 "Crafting Menu"
	BSFixedString	trainingMenu;				// 170 "Training Menu"
	BSFixedString	mistMenu;					// 178 "Mist Menu"
	BSFixedString	tutorialMenu;				// 180 "Tutorial Menu"
	BSFixedString	creditsMenu;				// 188 "Credits Menu"
	BSFixedString	modManagerMenu;				// 190 "Mod Manager Menu"		- NEW IN SE
	BSFixedString	titleSequenceMenu;			// 198 "TitleSequence Menu"
	BSFixedString	consoleNativeUIMenu;		// 1A0 "Console Native UI Menu"
	BSFixedString	kinectMenu;					// 1A8 "Kinect Menu"
	BSFixedString	loadWaitSpinner;			// 1B0 "LoadWaitSpinner"		- NEW IN SE
	BSFixedString	streamingInstallMenu;		// 1B8	"StreamingInstallMenu"  - NEW IN SE
	BSFixedString	textWidget;					// 1C0 "TextWidget"
	BSFixedString	buttonBarWidget;			// 1C8 "ButtonBarWidget"
	BSFixedString	graphWidget;				// 1D0 "GraphWidget"
	BSFixedString	textureWidget;				// 1D8 "TextWidget"
	BSFixedString	uiMenuOK;					// 1E0 "UIMenuOK"
	BSFixedString	uiMenuCancel;				// 1E8 "UIMenuCancel"
	BSFixedString	showText;					// 1F0 "Show Text"
	BSFixedString	hideText;					// 1F8 "Hide Text"
	BSFixedString	showList;					// 200 "Show List"
	BSFixedString	voiceReady;					// 208 "Voice Ready"
	BSFixedString	dmfoStr;					// 210 "DMFOStr"
	BSFixedString	showJournal;				// 218 "Show Journal"
	BSFixedString	journalSettingsSaved;		// 220 "Journal Settings Saved"
	BSFixedString	closeMenu;					// 228 "CloseMenu"
	BSFixedString	closingAllMenus;			// 230 "Closing All Menus"
	BSFixedString	refreshMenu;				// 238 "RefreshMenu"
	BSFixedString	cancelLoading;				// 240 "CancelLoading"		- NEW IN SE
	BSFixedString	menuTextureDegradeEvent;	// 248 "Menu Texture Degrade Event"
	BSFixedString	diamondMarker;				// 250 "<img src='DiamondMarker' width='10' height='15' align='baseline' vspace='5'>"

	static UIStringHolder *	GetSingleton(void)
	{
		// 81B349AB8ABC9944E48046819F0345AB0526CDB5+9
		static RelocPtr<UIStringHolder *> g_UIStringHolder(0x01EC0A78);
		return *g_UIStringHolder;
	}
};

// 160
class Inventory3DManager
{
public:
	virtual ~Inventory3DManager();

	static Inventory3DManager * GetSingleton(void)
	{
		// 6BC34CC398831C8B8BE5BE20EC213B5BE7C47A7A+2E
		static RelocPtr<Inventory3DManager*> g_inventory3DManager(0x02F27180);
		return *g_inventory3DManager;
	}

	//	void			** _vtbl;		// 00
	UInt32			unk08;			// 08
	UInt8			unk0C;			// 0C - This appears to be 1 when a menu is open
	UInt8			unk0D;			// 0D
	UInt16			pad0E;			// 0E
	UInt32			unk10;			// 10
	float			unk14[8];		// 14
	UInt32			pad34;			// 34
	TESObjectREFR	* object;		// 38
	BaseExtraList	baseExtraList;	// 40 - Only valid when NewInventoryMenuItemLoadTask is pending
	UInt32			unk58;			// 58
	UInt32			pad5C;			// 5C

	// 20
	struct ItemData
	{
		TESForm	* form1;
		TESForm	* form2;
		NiNode	* node;
		UInt32	  unk18;
		float	  unk1C;
	};

	ItemData		itemData[7];	// 60
	UInt32			meshCount;	// 140 - Number of ItemData where there is a valid BSFadeNode
	UInt32			pad144;		// 144
	UInt32			unk148;		// 148
	UInt32			unk14C;		// 14C
	void*			unk150;		// 150 - Pointer to NewInventoryMenuItemLoadTask when loading
	UInt8			unk158;
	UInt8			unk159; // Somekind of mode (0 for MagicMenu)
	UInt8			unk15A;
	UInt8			pad15B[5];

	MEMBER_FN_PREFIX(Inventory3DManager);
	DEFINE_MEMBER_FN(UpdateItem3D, void, 0x00887970, InventoryEntryData * objDesc);
	DEFINE_MEMBER_FN(UpdateMagic3D, void, 0x008879A0, TESForm * form, UInt32 unk1);
	DEFINE_MEMBER_FN(Clear3D, void, 0x00887D10);
	DEFINE_MEMBER_FN(Render, UInt32, 0x00887750);

	//DEFINE_MEMBER_FN(Unk1, void, 0x008667E0, UInt32 unk1);
	//DEFINE_MEMBER_FN(Unk2, void, 0x00867110);
	//DEFINE_MEMBER_FN(Unk3, bool, 0x008664C0);
	//DEFINE_MEMBER_FN(Unk4, double, 0x008663E0);
	//DEFINE_MEMBER_FN(Unk5, bool, 0x008418D0);
	//DEFINE_MEMBER_FN(Unk6, int, 0x00867730);
};

STATIC_ASSERT(offsetof(Inventory3DManager, unk14) == 0x14);
STATIC_ASSERT(offsetof(Inventory3DManager, object) == 0x38);
STATIC_ASSERT(offsetof(Inventory3DManager, unk158) == 0x158);

// 018
class MenuTableItem
{
public:
	BSFixedString	name;				// 000
	IMenu			* menuInstance;		// 008	0 if the menu is not currently open
	void			* menuConstructor;	// 010

	bool operator==(const MenuTableItem & rhs) const { return name == rhs.name; }
	bool operator==(const BSFixedString a_name) const { return name == a_name; }
	operator UInt64() const { return (UInt64)name.data; }

	static inline UInt32 GetHash(BSFixedString * key)
	{
		UInt32 hash;
		CalculateCRC32_64(&hash, (UInt64)key->data);
		return hash;
	}

	void Dump(void)
	{
		_MESSAGE("\t\tname: %s", name);
		_MESSAGE("\t\tinstance: %08X", menuInstance);
	}
};

// 1C8
class MenuManager
{
	typedef tHashSet<MenuTableItem, BSFixedString> MenuTable;

	// 030-040
	struct Unknown3
	{
		UInt64		unk00;		// 000 - New in SE. Not init'd?

		UInt64		frequency;	// 008 

		UInt64		unk_010;	// 010 (= 0)
		UInt32		unk_018;	// 018 (= 0)
		UInt32		unk_01C;	// 01C (= 0)
		UInt32		unk_020;	// 020 (= 0)
		UInt32		unk_024;	// 024 (= 0)
		float		unk_028;	// 028 (= frequency related)

		UInt32		unk_02C;	// 02C
		UInt32		unk_030;	// 030

		UInt32		unk_034;	// 034 (= 0)
		UInt16		unk_038;	// 038
		UInt8		unk_03A;	// 03A (= 0)
		UInt8		pad[5];		// 03B
	};
	STATIC_ASSERT(sizeof(Unknown3) == 0x40);

private:
	UInt64					unk_000;	// 000

	EventDispatcher<MenuOpenCloseEvent>		menuOpenCloseEventDispatcher;	// 008
	EventDispatcher<MenuModeChangeEvent>	menuModeChangeEventDispatcher;	// 060
	EventDispatcher<void*>					unk_064;						// 0B8 - New in 1.6.87.0 - Kinect related?

	UnkArray			menuStack;					// 110
	MenuTable			menuTable;					// 128   (Entries ptr at 150)
	SimpleLock			menuTableLock;				// 158
	UInt32				numPauseGame;				// 160 (= 0) += 1 if (imenu->flags & 0x0001)
	UInt32				numItemMenu;				// 164 (= 0) += 1 if (imenu->flags & 0x2000)
	UInt32				numPreventGameLoad;			// 168 (= 0) += 1 if (imenu->flags & 0x0080)
	UInt32				numDoNotPreventSaveGame;	// 16C (= 0) += 1 if (imenu->flags & 0x0800)
	UInt32				numStopCrosshairUpdate;		// 170 (= 0) += 1 if (imenu->flags & 0x4000)
	UInt32				numFlag8000;				// 174 (= 0) += 1 if (imenu->flags & 0x8000)
	UInt32				numFlag20000;				// 178 (= 0)  = 1 if (imenu->flags & 0x20000)
	UInt8				numModal;					// 17C (= 0)  = 1 if (imenu->flags & 0x10)
	UInt8				pad_17D[3];					// 17D
	Unknown3			unk_180;					// 180
	bool				showMenus;					// 1C0 (= 0)
	bool				unk_1C1;					// 1C1 (= 0)
	char				pad[6];						// 1C2

public:
	typedef IMenu*	(*CreatorFunc)(void);

private:
	MEMBER_FN_PREFIX(MenuManager);
	DEFINE_MEMBER_FN(IsMenuOpen, bool, 0x00EBE150, BSFixedString * menuName);
	DEFINE_MEMBER_FN(Register_internal, void, 0x00EBF9C0, const char * name, CreatorFunc creator);

public:

	static MenuManager * GetSingleton(void)
	{
		// 502FDB8FEA80C3705F9E228F79D4EA7A399CC7FD+32
		static RelocPtr<MenuManager *> g_menuManager(0x01EBEB20);
		return *g_menuManager;
	}

	EventDispatcher<MenuOpenCloseEvent> * MenuOpenCloseEventDispatcher()
	{
		return &menuOpenCloseEventDispatcher;
	}

	bool				IsMenuOpen(BSFixedString * menuName);
	IMenu *				GetMenu(BSFixedString * menuName);
	GFxMovieView *		GetMovieView(BSFixedString * menuName);
	void				ShowMenus(bool show) { showMenus = show; }
	bool				IsShowingMenus() const { return showMenus; }

	typedef IMenu* (*CreatorFunc)(void);

	void Register(const char* name, CreatorFunc creator)
	{
		CALL_MEMBER_FN(this, Register_internal)(name, creator);
	}
};
STATIC_ASSERT(sizeof(MenuManager) == 0x1C8);
