#pragma once

#include "GameTypes.h"
#include "GameInput.h"

#include "GameSettings.h"

#include "skse64/NiObjects.h"
#include "skse64/NiTypes.h"

class TESCamera;
class NiNode;

// 20
class TESCameraState
{
public:
	TESCameraState();
	virtual ~TESCameraState();

	virtual void OnStateStart();	// pure
	virtual void OnStateEnd();	// pure
	virtual void OnUpdate(void * unk1);
	virtual void Unk_04();
	virtual void Unk_05();
	virtual void Unk_06();	// pure
	virtual void Unk_07();	// pure
	virtual void Unk_08();	// pure

	BSIntrusiveRefCounted	refCount;		// 08
	TESCamera				* camera;		// 10
	UInt32					stateId;		// 18
	UInt32					pad1C;			// 1C
};

// 90
class FirstPersonState : public TESCameraState
{
public:
	FirstPersonState();
	virtual ~FirstPersonState();

	PlayerInputHandler		inputHandler;	// 20
	float					unk30[6];		// 30 
	UInt64					unk48;			// 48
	UInt64					unk50;			// 50
	void*					unk58;			// 58
	void*					unk60;			// 60
	float					unk68[3];		// 70
	UInt32					unk74;			// 74
	UInt32					unk78;			// 78
	float					unk7C;			// 7C
	UInt32					unk80;			// 80
	UInt8					unk84[4];		// 84
	UInt64					unk88;			// 88
};
STATIC_ASSERT(sizeof(FirstPersonState) == 0x90);

// E8
class ThirdPersonState : public TESCameraState
{
public:
	ThirdPersonState();
	virtual ~ThirdPersonState();
	virtual void Unk_09(void);
	virtual void Unk_0A(void);
	virtual void UpdateMode(bool weaponDrawn);

	PlayerInputHandler		inputHandler;				// 20
	NiNode					* cameraNode;				// 30
	NiNode					* controllerNode;			// 38
	float					unk40[4];					// 40
	UInt32					unk50[3];					// 50
	float					fOverShoulderPosX;			// 5C
	float					fOverShoulderCombatAddY;	// 60
	float					fOverShoulderPosZ;			// 64
	float					unk68[3];					// 68
	UInt32					unk74[6];					// 74
	float					unk8C;						// 8C - init'd 7F7FFFFF
	UInt32					unk90[3];					// 90
	float					unk9C;						// 9C - init'd 7F7FFFFF
	UInt64					unkA0;						// A0
	UInt64					unkA8;						// A8
	float					unkB0;						// B0
	UInt32					unkB4[3];					// B4
	float					unkC0[6];					// C0
	UInt32					unkD8;						// D8
	UInt8					unkDC[7];					// DC 
	UInt8					padE3[5];					// E3
};
STATIC_ASSERT(sizeof(ThirdPersonState) == 0xE8);

// 110
class DragonCameraState : public ThirdPersonState
{
public:
	DragonCameraState();
	virtual ~DragonCameraState();

	UInt32	unkE8;		// 0E8
	UInt32	unkEC;		// 0EC
	UInt8	unkF0;		// 0F0
	UInt8	padF1[3];	// 0F1
	float	unkF4;		// 0F4 - init'd to 1
	UInt8	unkF8;		// 0F8
	UInt8	padF9[3];	// 0F9
	float	unkFC[4];	// 0FC
	UInt32	unk10C;		// 10C
};

// F8
class HorseCameraState : public ThirdPersonState
{
public:
	HorseCameraState();
	virtual ~HorseCameraState();

	UInt32	unkE8;		// E8
	UInt32	unkEC;		// EC
	UInt8	unkF0;		// F0
	UInt8	padF1[7];	// F1
};

// 50
class TweenMenuCameraState : public TESCameraState
{
public:
	TweenMenuCameraState();
	virtual ~TweenMenuCameraState();

	float	unk20[4];	// 20
	UInt32	unk30;		// 30
	float	unk34[4];	// 34
	UInt32	unk44;		// 44
	UInt32	unk48;		// 48
	UInt8	unk4C;		// 4C
	UInt8	pad4D[3];	// 4D
};

// 38
class VATSCameraState : public TESCameraState
{
public:
	VATSCameraState();
	virtual ~VATSCameraState();

	float	unk20[6];	// 20
};

// 50
class FreeCameraState : public TESCameraState
{
public:
	FreeCameraState();
	virtual ~FreeCameraState();

	PlayerInputHandler		inputHandler;	// 20
	float					unk30[7];		// 30
	UInt32					unk4C;			// 4C
};

// 28
class AutoVanityState : public TESCameraState
{
public:
	AutoVanityState();
	virtual ~AutoVanityState();

	UInt32	unk20;	// 20
	UInt32	pad24;	// 24
};

// 40
class FurnitureCameraState : public TESCameraState
{
public:
	FurnitureCameraState();
	virtual ~FurnitureCameraState();

	UInt32	unk20;	// 20
	float	unk24;	// 24
	float	unk28;	// 28
	float	unk2C;	// 2C
	UInt32	unk30;	// 30
	UInt32	unk34;	// 34
	UInt32	unk38;	// 38
	UInt8	unk3C;	// 3C
	UInt8	unk3D;	// 3D
	UInt8	unk3E;	// 3E
	UInt8	pad3F;	// 3F
};

// 28
class IronSightsState : public TESCameraState
{
public:
	IronSightsState();
	virtual ~IronSightsState();

	UInt64	unk20;	// 20
};

// 138
class BleedoutCameraState : public ThirdPersonState
{
public:
	BleedoutCameraState();
	virtual ~BleedoutCameraState();

	float	unkE8[8];	// E8
	UInt32	unk108;		// 108
	float	unk10C;		// 10C - init'd to 200
	float	unk110;		// 110 - init'd to 0.523599 (PI/6)
	UInt32	unk114;		// 114
	float	unk118;		// 118
	UInt32	pad11C;		// 11C
	UInt64	unk120;		// 120
	UInt32	unk128;		// 128 - init'd to FFFFFFFF
	UInt8	unk12C;		// 12C
	UInt8	pad12D[3];	// 12D
	UInt32	unk130;		// 130
	UInt8	unk134;		// 134
	UInt8	pad135[3];	// 135
};

// 40
class PlayerCameraTransitionState : public TESCameraState
{
public:
	PlayerCameraTransitionState();
	virtual ~PlayerCameraTransitionState();

	UInt32	unk20;			// 20
	UInt32	unk24;			// 24
	UInt64	unk28;			// 28
	UInt64	unk30;			// 30
	UInt32	unk38;			// 38
	UInt8	unk3C;			// 3C
	UInt8	unk3D;			// 3D
	UInt16	pad3E;
};

// 38
class TESCamera
{
public:
	TESCamera();
	virtual ~TESCamera();

	virtual void SetNode(NiNode * node);
	virtual void Update();

	float		rotZ;	// 08
	float		rotX;	// 0C
	NiPoint3	pos;	// 10
	float		zoom;	// 1C
	NiNode		* cameraNode;	// 20 - First child is usually NiCamera
	TESCameraState	* cameraState;	// 28
	UInt8		unk30;			// 30
	UInt8		pad31[7];		// 31

	MEMBER_FN_PREFIX(TESCamera);
	DEFINE_MEMBER_FN(SetCameraState, UInt32, 0x00551070, TESCameraState * cameraState);
};
STATIC_ASSERT(offsetof(TESCamera, cameraNode) == 0x20);
STATIC_ASSERT(sizeof(TESCamera) == 0x38);

// 68
class LocalMapCamera : public TESCamera
{
public:
	LocalMapCamera();
	virtual ~LocalMapCamera();

	// 48
	class DefaultState : public TESCameraState
	{
	public:
		NiPoint3	someBoundMax;		// 20
		NiPoint3	someBoundMin;		// 2C
		float		zoomPercent;		// 38
		float		minFrustumWidth;	// 3C
		float		minFrustumHeight;	// 40
		UInt32		pad44;				// 44
	};

	NiPoint3	areaBoundsMax;			// 38
	NiPoint3	areaBoundsMin;			// 44
	DefaultState	* defaultState;		// 50
	NiObject	* niCamera;				// 58
	float		northRotation;			// 60
	UInt32		pad64;					// 64

	void SetDefaultStateMinFrustumDimensions(float width, float height);
	void SetAreaBounds(NiPoint3 * maxBound, NiPoint3 * minBound);
	void SetDefaultStateMaxBound(NiPoint3 * maxBound);
	void SetDefaultStateBounds(float x, float y, float z);

	MEMBER_FN_PREFIX(LocalMapCamera);
	DEFINE_MEMBER_FN(ctor, void, 0x00242460);
	DEFINE_MEMBER_FN(SetNorthRotation, void, 0x002428B0, float northRotation);
};

STATIC_ASSERT(offsetof(LocalMapCamera, northRotation) == 0x60);

// 90
class MapCamera : public TESCamera
{
public:
	MapCamera();
	virtual ~MapCamera();

	float	unk38[5];	// 38
	UInt32	unk4C[3];	// 4C
	UInt64	unk58;		// 58
	UInt32	unk60;		// 60
	UInt32	unk64;		// 64
	UInt64	unk68[2];	// 68
	UInt64	unk78;		// 78
	UInt64	unk80;		// 80
	UInt8	unk88;		// 88
	UInt8	pad89[7];	// 89
};

// 38
class RaceSexCamera : public TESCamera
{
public:
	RaceSexCamera();
	virtual ~RaceSexCamera();
};

// 168
class PlayerCamera : public TESCamera
{
public:
	PlayerCamera();
	virtual ~PlayerCamera();

	static PlayerCamera *	GetSingleton(void)
	{
		// 8737E3612AB5A303F82C06809C0B0B41B2015C66+1E
		static RelocPtr<PlayerCamera*> g_playerCamera(0x030F04F8);
		return *g_playerCamera;
	}

	enum
	{
		kCameraState_FirstPerson = 0,
		kCameraState_AutoVanity,
		kCameraState_VATS,
		kCameraState_Free,
		kCameraState_IronSights,
		kCameraState_Furniture,
		kCameraState_Transition,
		kCameraState_TweenMenu,
		kCameraState_ThirdPerson1,
		kCameraState_ThirdPerson2,
		kCameraState_Horse,
		kCameraState_Bleedout,
		kCameraState_Dragon,
		kNumCameraStates
	};

	UInt8	unk38[0xB8-0x38];							// 028
	TESCameraState * cameraStates[kNumCameraStates];	// 0B8
	UInt64	unk120;										// 120
	UInt64	unk128;										// 128
	UInt64	unk130;										// 130
	UInt32	unk138;										// 138
	float	worldFOV;									// 13C
	float	firstPersonFOV;								// 140
	UInt8	unk144[0x160-0x144];						// 144
	UInt8	unk160;										// 160
	UInt8	unk161;										// 161
	UInt8	unk162;										// 162 - init'd to 1
	UInt8	unk163;										// 163
	UInt8	unk164;										// 164
	UInt8	unk165;										// 165
	UInt8	pad166[2];									// 166

	MEMBER_FN_PREFIX(PlayerCamera);
	DEFINE_MEMBER_FN(UpdateThirdPerson, void, 0x008E5B50, bool weaponDrawn);
};

STATIC_ASSERT(offsetof(PlayerCamera, cameraStates) == 0xB8);
STATIC_ASSERT(offsetof(PlayerCamera, pad166) == 0x166);
