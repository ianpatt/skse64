#pragma once

#include "GameTypes.h"
#include "GameEvents.h"
#include "InputMap.h"

class PlayerControls;
class BGSEncounterZone;
class MenuEventHandler;

// Note: These are different from those in Hooks_DI
enum
{
	kDeviceType_Keyboard = 0,
	kDeviceType_Mouse,
	kDeviceType_Gamepad
};

class BSInputDevice
{
public:

	// SE: destructor is now at top
	virtual			~BSInputDevice();
	virtual UInt32	Unk_01(void);		// pure
	virtual	void	Unk_02(float unk1);	// pure
	virtual	void	Unk_03(void);		// pure

	// These 3 ones added in SE. The three of them do some call to CRC functions but I didn't do further research
	virtual	bool	Unk_04(UInt32 unk0, void *unk1);
	virtual bool	Unk_05(void *unk0);
	virtual bool	Unk_06(UInt32 unk0, UInt32 *unk1);

	virtual bool	IsEnabled(void);	// Always 1 for non-gamepad?	
	virtual void	Unk_08(void* unk1);		// pure 	
};

// 18
class InputEvent
{
public:
	enum
	{
		kEventType_Button = 0,
		kEventType_MouseMove,
		kEventType_Char,
		kEventType_Thumbstick,
		kEventType_DeviceConnect,
		kEventType_Kinect
	};

	virtual					~InputEvent();
	virtual bool			IsIDEvent();
	virtual BSFixedString *	GetControlID();

//	void			** _vtbl;	// 00
	UInt32			deviceType;	// 08
	UInt32			eventType;	// 0C
	InputEvent		* next;		// 10
};

class IDEvent
{
public:
	BSFixedString	controlID;	// 00
};

// 30
class ButtonEvent : public IDEvent, public InputEvent
{
public:
	virtual					~ButtonEvent();
	virtual bool			IsIDEvent();
	virtual BSFixedString *	GetControlID();

	// 18 -> controlID from IDEvent
	UInt32			keyMask;	// 20 (00000038 when ALT is pressed, 0000001D when STRG is pressed)
	UInt32			pad24;		// 24
	UInt32			flags;		// 28 - isn't this a float?
	float			timer;		// 2C (hold duration)
};

class MouseMoveEvent : public IDEvent, public InputEvent
{
};

// 20
class CharEvent : public InputEvent
{
public:
	UInt32			keyCode;		// 18 (ascii code)
};

class ThumbstickEvent : public IDEvent, public InputEvent
{
public:
	virtual					~ThumbstickEvent();
	virtual bool			IsIDEvent();
	virtual BSFixedString *	GetControlID();

	// 18 -> controlID from IDEvent
	UInt32	keyMask;	// 20 - b for left stick, c for right stick
	float	x;			// 24
	float	y;			// 28
};

class DeviceConnectEvent : public InputEvent
{
};

class KinectEvent : public IDEvent, public InputEvent
{
};

// E8 
class InputEventDispatcher : public EventDispatcher<InputEvent,InputEvent*>
{
public:
	UInt32			unk058;			// 058
	UInt32			pad05C;			// 05C
	BSInputDevice	* keyboard;		// 060 
	BSInputDevice	* mouse;		// 068
	BSInputDevice	* gamepad;		// 070
	BSInputDevice	* vkeyboard;	// 078	- New in SE  .?AVBSWin32VirtualKeyboardDevice@@
	UInt8			unk080;			// 080
	UInt8			unk081;			// 081
	UInt8			pad082[6];		// 082
	BSTEventSource<void *>	unk088;	// 088	- TODO: template type
	UInt8			unk0E0;			// 0E0
	UInt8			pad0E1[7];		// 0E1

	bool	IsGamepadEnabled(void);

	static InputEventDispatcher* GetSingleton();
};
STATIC_ASSERT(offsetof(InputEventDispatcher, gamepad) == 0x70);
STATIC_ASSERT(sizeof(InputEventDispatcher) == 0xE8);

template <>
class BSTEventSink <InputEvent>
{
public:
	virtual ~BSTEventSink() {}; // todo?
	virtual	EventResult ReceiveEvent(InputEvent ** evn, InputEventDispatcher * dispatcher) = 0;
};

// 128 
class InputManager
{
public:
	enum
	{
		kContext_Gameplay = 0,
		kContext_MenuMode,
		kContext_Console,
		kContext_ItemMenu,
		kContext_Inventory,
		kContext_DebugText,
		kContext_Favorites,
		kContext_Map,
		kContext_Stats,
		kContext_Cursor,
		kContext_Book,
		kContext_DebugOverlay,
		kContext_Journal,
		kContext_TFCMode,
		kContext_MapDebug,
		kContext_Lockpicking,
		kContext_Favor,
		kContextCount = 17
	};

	struct InputContext
	{
		// 18
		struct Mapping
		{
			BSFixedString	name;		// 00
			UInt32			buttonID;	// 08
			UInt32			sortIndex;	// 0C
			UInt32			unk10;		// 10
			UInt32			pad14;		// 14
		};

		tArray<Mapping>	keyboardMap;
		tArray<Mapping>	mouseMap;
		tArray<Mapping>	gamepadMap;
	};


	void*			unkPtr000;					// 000
	BSTEventSource<void *>	unk008;				// 008 - TODO: template type
	InputContext	* context[kContextCount];	// 060
	tArray<void*>	unk0E8;						// 0E8
	tArray<void*>	unk100;						// 100
	UInt32			unk118;						// 118 - init'd to 0xFFFFFFFF
	UInt32			unk11C;						// 11C - init'd to 0x80000000
	UInt8			allowTextInput;				// 120
	UInt8			unk121;						// 121
	UInt8			unk122;						// 122
	UInt8			pad[5];						// 123

	static InputManager *	GetSingleton(void);

	UInt8			AllowTextInput(bool allow);

	UInt32			GetMappedKey(BSFixedString name, UInt32 deviceType, UInt32 contextIdx);

	BSFixedString	GetMappedControl(UInt32 buttonID, UInt32 deviceType, UInt32 contextIdx);
};
STATIC_ASSERT(sizeof(InputManager) == 0x128);

// 10
class PlayerInputHandler
{
public:
	PlayerInputHandler();
	virtual ~PlayerInputHandler();

	virtual void Unk_01();
	virtual void Unk_02();
	virtual void Unk_03();
	virtual void Unk_04();

	UInt32	unk08;				// 08
	UInt32	pad0C;				// 0C
};

// 1D8 or 1E0
class PlayerControls
{
public:
	virtual			~PlayerControls();
	virtual UInt32	Unk_01();

//	void			** _vtbl;		// 000
	BSTEventSink<void*> menuOpenCloseEvent;	// 008
	BSTEventSink<void*> menuModeChangeEvent;	// 010
	BSTEventSink<void*> furnitureEvent;	// 018
	UInt32			unk020;			// 020
	float			unk024;			// 024
	float			unk028;			// 028
	float			unk02C;			// 02C
	float			unk030;			// 030
	float			unk034;			// 034
	float			unk038;			// 038
	float			unk03C;			// 03C
	float			unk040;			// 040
	UInt32			unk044;			// 044
	UInt8			autoRun;		// 048
	UInt8			runMode;		// 049
	UInt8			unk04A;			// 04A
	UInt8			unk04B;			// 04B
	UInt16			unk04C;			// 04C
	UInt8			unk04E;			// 04E
	UInt8			unk04F;			// 04F
	bool			remapMode;		// 050 - might be named differently
	UInt8			pad51[7];		// 051
	tArray<void*>	unk058;			// 058
	tArray<void*>	unk070;			// 070
	tArray<void *>	unk088;			// 088
	UInt8			unkA0[8];		// 0A0
	UInt64			unkA8;			// 0A8
	float			unkB0[8];		// B0
	UInt32			unkD0[10];		// D0
	UInt8			unkF8[8];		// F8
	float			unk100[20];		// 100
	tArray<void*>	unk150;			// 150
	UInt64			unk168;			// 168

	PlayerInputHandler*	movementHandler;	// 170
	PlayerInputHandler*	lookHandler;		// 178
	PlayerInputHandler*	sprintHandler;		// 180
	PlayerInputHandler*	readyWeaponHandler; // 188
	PlayerInputHandler*	autoMoveHandler;	// 190
	PlayerInputHandler*	toggleRunHandler;	// 198
	PlayerInputHandler*	activateHandler;	// 1A0
	PlayerInputHandler*	jumpHandler;		// 1A8
	PlayerInputHandler*	shoutHandler;		// 1B0
	PlayerInputHandler*	attackBlockHandler; // 1B8
	PlayerInputHandler*	runHandler;			// 1C0
	PlayerInputHandler*	sneakHandler;		// 1C8
	PlayerInputHandler*	togglePOVHandler;	// 1D0

	static PlayerControls *	GetSingleton(void);

	// used by Hooks_Event
	PlayerControls * ctor_Hook(void);
	MEMBER_FN_PREFIX(PlayerControls);
	DEFINE_MEMBER_FN(ctor, PlayerControls *, 0x0079A9B0);
};
STATIC_ASSERT(offsetof(PlayerControls, runMode) == 0x049);
STATIC_ASSERT(offsetof(PlayerControls, remapMode) == 0x050);

// 90
class MenuControls
{
public:
	virtual			~MenuControls();
	virtual UInt32	Unk_01();

//	void			** _vtbl;		// 00
	BSTEventSink<MenuModeChangeEvent> menuModeEventSink; // 08
	UInt64			unk10;			// 10
	tArray<void*>	arr18;			// 18
	UInt64			unk30[3];		// 30

	MenuEventHandler* clickHandler;	// 48
	MenuEventHandler* directionHandler;	// 50
	MenuEventHandler* consoleOpenHandler;	// 58
	MenuEventHandler* quickSaveLoadHandler;	// 60
	MenuEventHandler* menuOpenHandler;	// 68
	MenuEventHandler* favoritesHandler;	// 70
	MenuEventHandler* screenshotHandler;	// 78

	UInt8			unk80;			// 80
	UInt8			unk81;			// 81
	bool			remapMode;		// 82
	UInt8			unk83;			// 83
	UInt8			pad84[0x90 - 0x84];	// 84

	static MenuControls *	GetSingleton(void);
};
STATIC_ASSERT(offsetof(MenuControls, remapMode) == 0x082);

// 340
class InputStringHolder
{
public:
	void			* unk00;			// 000
	BSFixedString	forward;			// 008 "Forward"
	BSFixedString	back;				// 010 "Back"
	BSFixedString	strafeLeft;			// 018 "Strafe Left"
	BSFixedString	strafeRight;		// 020 "Strafe Right"
	BSFixedString	move;				// 028 "Move"
	BSFixedString	look;				// 030 "Look"
	BSFixedString	activate;			// 038 "Activate"
	BSFixedString	leftAttack;			// 040 "Left Attack/Block"
	BSFixedString	rightAttack;		// 048 "Right Attack/Block"
	BSFixedString	dualAttack;			// 050 "Dual Attack"
	BSFixedString	forceRelease;		// 058 "ForceRelease"
	BSFixedString	pause;				// 060 "Pause"
	BSFixedString	readyWeapon;		// 068 "Ready Weapon"
	BSFixedString	togglePOV;			// 070 "Toggle POV"
	BSFixedString	jump;				// 078 "Jump"
	BSFixedString	journal;			// 080 "Journal"
	BSFixedString	sprint;				// 088 "Sprint"
	BSFixedString	sneak;				// 090 "Sneak"
	BSFixedString	shout;				// 098 "Shout"
	BSFixedString	kinectShout;		// 0A0 "KinectShout"
	BSFixedString	grab;				// 0A8 "Grab"
	BSFixedString	run;				// 0B0 "Run"
	BSFixedString	toggleRun;			// 0B8 "Toggle Always Run"
	BSFixedString	autoMove;			// 0C0 "Auto-Move"
	BSFixedString	quicksave;			// 0C8 "Quicksave"
	BSFixedString	quickload;			// 0D0 "Quickload"
	BSFixedString	newSave;			// 0D8 "NewSave"
	BSFixedString	inventory;			// 0E0 "Inventory"
	BSFixedString	stats;				// 0E8 "Stats"
	BSFixedString	map;				// 0F0 "Map"
	BSFixedString	screenshot;			// 0F8 "Screenshot"
	BSFixedString	multiScreenshot;	// 100 "Multi-Screenshot"
	BSFixedString	console;			// 108 "Console"
	BSFixedString	cameraPath;			// 110 "CameraPath"
	BSFixedString	tweenMenu;			// 118 "Tween Menu"
	BSFixedString	takeAll;			// 120 "Take All"
	BSFixedString	accept;				// 128 "Accept"
	BSFixedString	cancel;				// 130 "Cancel"
	BSFixedString	up;					// 138 "Up"
	BSFixedString	down;				// 140 "Down"
	BSFixedString	left;				// 148 "Left"
	BSFixedString	right;				// 150 "Right"
	BSFixedString	pageUp;				// 158 "PageUp"
	BSFixedString	pageDown;			// 160 "PageDown"
	BSFixedString	pick;				// 168 "Pick"
	BSFixedString	pickNext;			// 170 "PickNext"
	BSFixedString	pickPrevious;		// 178 "PickPrevious"
	BSFixedString	cursor;				// 180 "Cursor"
	BSFixedString	kinect;				// 188 "Kinect"
	BSFixedString	sprintStart;		// 190 "SprintStart"
	BSFixedString	sprintStop;			// 198 "SprintStop"
	BSFixedString	sneakStart;			// 1A0 "sneakStart"
	BSFixedString	sneakStop;			// 1A8 "sneakStop"
	BSFixedString	blockStart;			// 1B0 "blockStart"
	BSFixedString	blockStop;			// 1B8 "blockStop"
	BSFixedString	blockBash;			// 1C0 "blockBash"
	BSFixedString	attackStart;		// 1C8 "attackStart"
	BSFixedString	attackPowerStart;	// 1D0 "attackPowerStart"
	BSFixedString	reverseDirection;	// 1D8 "reverseDirection"
	BSFixedString	unequip;			// 1E0 "Unequip"
	BSFixedString	zoomIn;				// 1E8 "Zoom In"
	BSFixedString	zoomOut;			// 1F0 "Zoom Out"
	BSFixedString	rotateItem;			// 1F8 "RotateItem"
	BSFixedString	leftStick;			// 200 "Left Stick"
	BSFixedString	prevPage;			// 208 "PrevPage"
	BSFixedString	nextPage;			// 210 "NextPage"
	BSFixedString	prevSubPage;		// 218 "PrevSubPage"
	BSFixedString	nextSubPage;		// 220 "NextSubPage"
	BSFixedString	leftEquip;			// 228 "LeftEquip"
	BSFixedString	rightEquip;			// 230 "RightEquip"
	BSFixedString	toggleFavorite;		// 238 "ToggleFavorite"
	BSFixedString	favorites;			// 240 "Favorites"
	BSFixedString	hotkey1;			// 248 "Hotkey1"
	BSFixedString	hotkey2;			// 250 "Hotkey2"
	BSFixedString	hotkey3;			// 258 "Hotkey3"
	BSFixedString	hotkey4;			// 260 "Hotkey4"
	BSFixedString	hotkey5;			// 268 "Hotkey5"
	BSFixedString	hotkey6;			// 270 "Hotkey6"
	BSFixedString	hotkey7;			// 278 "Hotkey7"
	BSFixedString	hotkey8;			// 280 "Hotkey8"
	BSFixedString	quickInventory;		// 288 "Quick Inventory"
	BSFixedString	quickMagic;			// 290 "Quick Magic"
	BSFixedString	quickStats;			// 298 "Quick Stats"
	BSFixedString	quickMap;			// 2A0 "Quick Map"
	BSFixedString	toggleCursor;		// 2A8 "ToggleCursor"
	BSFixedString	wait;				// 2B0 "Wait"
	BSFixedString	click;				// 2B8 "Click"
	BSFixedString	mapLookMode;		// 2C0 "MapLookMode"
	BSFixedString	equip;				// 2C8 "Equip"
	BSFixedString	dropItem;			// 2D0 "DropItem"
	BSFixedString	rotate;				// 2D8 "Rotate"
	BSFixedString	nextFocus;			// 2E0 "NextFocus"
	BSFixedString	prevFocus;			// 2E8 "PreviousFocus"
	BSFixedString	setActiveQuest;		// 2F0 "SetActiveQuest"
	BSFixedString	placePlayerMarker;	// 2F8 "PlacePlayerMarker"
	BSFixedString	xButton;			// 300 "XButton"
	BSFixedString	yButton;			// 308 "YButton"
	BSFixedString	chargeItem;			// 310 "ChargeItem"
	UInt64			unk318;				// 318
	BSFixedString	playerPosition;		// 320 "PlayerPosition"
	BSFixedString	localMap;			// 328 "LocalMap"
	BSFixedString	localMapMoveMode;	// 330 "LocalMapMoveMode"
	BSFixedString	itemZoom;			// 338 "Item Zoom"

	static InputStringHolder *	GetSingleton(void)
	{
		// 8BDB97B9BE3B9EBCCFB4F3BA9237EDC8341B354C+B
		static RelocPtr<InputStringHolder*> g_inputStringHolder(0x0314F930);
		return *g_inputStringHolder;
	}
};
STATIC_ASSERT(sizeof(InputStringHolder) == 0x340);
