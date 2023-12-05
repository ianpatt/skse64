#pragma once

#include "skse64/NiTypes.h"
#include "skse64/NiObjects.h"

#include "GameCamera.h"

class BSFaceGenAnimationData;

// 128
class NiNode : public NiAVObject
{
public:
	virtual void	AttachChild(NiAVObject * obj, bool firstAvail);
	virtual void	DetachChild(UInt32 unk1, NiAVObject * obj);
	virtual void	Unk_35(void);
	virtual void	RemoveChild(NiAVObject * obj);
	virtual void	Unk_37(void);
	virtual void	RemoveAt(UInt32 i);
	virtual void	Unk_39(void);
	virtual void	Unk_3A(void);
	virtual void	UpdateUpwardPass(void);

	NiTArray <NiAVObject *>	m_children;	// 110

	static NiNode * Create(UInt16 arrBufLen = 0);

	DEFINE_MEMBER_FN_1(ctor, NiNode*, 0x00D18700, UInt16 arrBufLen);
};
STATIC_ASSERT(sizeof(NiNode) == 0x128);

// 158
class BSFadeNode : public NiNode
{
public:
	UInt32	unkB8[(0x158 - 0x128) >> 2];

	static BSFadeNode * Create();

	// 905B699D46B52EE8B3BD44B9DAFBC2F728A310B5+81
	DEFINE_MEMBER_FN_0(ctor, BSFadeNode *, 0x01471320);
};
STATIC_ASSERT(sizeof(BSFadeNode) == 0x158);

// EC
class BSFaceGenNiNode : public NiNode
{
public:
	UInt32	unkB8;
	UInt32	unkBC;
	UInt32	unkC0;
	UInt32	unkC4;
	float	unkC8;
	UInt32	unkCC;
	UInt32	unkD0;
	UInt32	unkD4;
	float	unkD8;
	BSFaceGenAnimationData	* animData;
	float	unkE0;
	UInt32	handle;	// handle
	UInt32	unkE8;

	enum {
		kAdjustType_Unk0 = 0,
		kAdjustType_Unk1 = 1,
		kAdjustType_Unk2 = 2,
		kAdjustType_Neck = 3,
	};

	MEMBER_FN_PREFIX(BSFaceGenNiNode);
	DEFINE_MEMBER_FN(AdjustHeadMorph, void, 0x00432C60, UInt32 unk04, UInt32 unk08, float delta);
};

//STATIC_ASSERT(sizeof(BSFaceGenNiNode) == 0xEC);
//STATIC_ASSERT(offsetof(BSFaceGenNiNode, animData) == 0xDC);
//STATIC_ASSERT(offsetof(BSFaceGenNiNode, handle) == 0xE4);


class NiSwitchNode : public NiNode
{
public:
	// Nothing yet
};

// 110 ?
class NiCullingProcess
{
public:
	virtual ~NiCullingProcess();
	virtual void	Unk_01(void); // 01 - 10 unused
	virtual void	Unk_02(void);
	virtual void	Unk_03(void);
	virtual void	Unk_04(void);
	virtual void	Unk_05(void);
	virtual void	Unk_06(void);
	virtual void	Unk_07(void);
	virtual void	Unk_08(void);
	virtual void	Unk_09(void);
	virtual void	Unk_0A(void);
	virtual void	Unk_0B(void);
	virtual void	Unk_0C(void);
	virtual void	Unk_0D(void);
	virtual void	Unk_0E(void);
	virtual void	Unk_0F(void);
	virtual void	Unk_10(void);
	virtual void	Unk_11(void);
	virtual void	Unk_12(void);
	virtual void	Unk_13(void);

	UInt8	unk04;									// 04 Perhaps refcount?
	UInt8	pad05[3];
	UInt32	visibleArray; 							// 08 NiVisibleArray *
	UInt32	camera;   								// 0C NiCamera *
	UInt32	fustrum[(0x30 - 0x10) >> 2];			// 10 NiFustrum
	UInt32	fustrumPlanes[(0x90 - 0x30) >> 2];		// 2C NiFrustumPlanes
	UInt32	activePlanes;							// 90
	UInt32	unk94;									// 94
	UInt32	unk98;									// 98
	UInt32	fustrumPlanes2[(0x100 - 0x9C) >> 2];	// 9C NiFrustumPlanes
	UInt32	activePlanes2;							// 100
	UInt32	unk104;									// 104
	UInt32	unk108;									// 108
	UInt32	unk10C;									// 10C
};

//STATIC_ASSERT(sizeof(NiCullingProcess) == 0x110);

// 170
class BSCullingProcess : public NiCullingProcess
{
public:
	virtual ~BSCullingProcess();
	virtual void	Unk_14(void);
	virtual void	Unk_15(void);
	virtual void	Unk_16(void);
	virtual void	Unk_17(void);
	virtual void	Unk_18(void);

	UInt32	unk110[(0x170 - 0x110) >> 2];
};

//STATIC_ASSERT(sizeof(BSCullingProcess) == 0x170);

// 238
class LocalMapCullingProcess : public BSCullingProcess
{
public:
	LocalMapCamera		localMapCamera;					// 170
	void				* shaderAccumulator;			// 1BC
	BSRenderTargetGroup	* localMapTexture;				// 1C0
	UInt32				unk1C4[(0x230 - 0x1C4) >> 2];	// 1C4
	UInt32				width;							// 230
	UInt32				height;							// 234
	NiNode				* niNode;						// 238

	MEMBER_FN_PREFIX(LocalMapCullingProcess);
};

//STATIC_ASSERT(offsetof(LocalMapCullingProcess, localMapCamera) == 0x170);
//STATIC_ASSERT(offsetof(LocalMapCullingProcess, niNode) == 0x238);

class NiBoneNames
{
public:
	static NiBoneNames * GetSingleton(void);

	BSFixedString	root;	// 00
	BSFixedString	npc;	// 04
	BSFixedString	head;	// 08
	BSFixedString	pelvis;	// 0C
	BSFixedString	spine0;	// 10
	BSFixedString	spine1;	// 14
	BSFixedString	spine2;	// 18
	BSFixedString	lFoot;	// 1C
	BSFixedString	rFoot;	// 20
	BSFixedString	lCalf;	// 24
	BSFixedString	rCalf;	// 28
	BSFixedString	specialIdleCast;		// 2C
	BSFixedString	specialIdleAreaEffect;	// 30
	BSFixedString	attachSound;			// 34
	BSFixedString	soundMarker;			// 38
	BSFixedString	skinnedDecalNode;		// 3C
	BSFixedString	decalNode;				// 40
	BSFixedString	modelSwapNode;			// 44
	BSFixedString	open;	// 48
	BSFixedString	close;	// 4C
	BSFixedString	dvpg;	// 50
	BSFixedString	prn;	// 54
	BSFixedString	weapon;	// 58
	BSFixedString	weaponSword;	// 5C
	BSFixedString	weaponDagger;	// 60
	BSFixedString	weaponAxe;		// 64
	BSFixedString	weaponMace;		// 68
	BSFixedString	shield;			// 6C
	BSFixedString	weaponBack;		// 70
	BSFixedString	weaponBow;		// 74
	BSFixedString	quiver;			// 78
	BSFixedString	editorMarker;	// 7C	
	BSFixedString	editorMarker0;	// 80
	BSFixedString	editorMarker1;	// 84
	BSFixedString	editorMarker2;	// 88
	BSFixedString	arrowQuiver;	// 8C
	BSFixedString	markerSource;	// 90
	BSFixedString	markerTarget;	// 94
	BSFixedString	attachLight;	// 98
	BSFixedString	skin;			// 9C
	BSFixedString	faceGenEars;	// A0
	BSFixedString	unequip;		// A4
	BSFixedString	laserSight;		// A8
	BSFixedString	aimSight;		// AC
	BSFixedString	decal;			// B0
	BSFixedString	permanentDecal;	// B4
	BSFixedString	grabLeft;		// B8
	BSFixedString	grabRight;		// BC
	BSFixedString	arrow0;			// C0
	BSFixedString	arrowBone;		// C4
	BSFixedString	faceGenNiNodeSkinned;	// C8
	BSFixedString	entryPoint;		// CC
	BSFixedString	lUpperArm;		// D0
	BSFixedString	lForearm;		// D4
	BSFixedString	rUpperArm;		// D8
	BSFixedString	lookNode;		// DC
	BSFixedString	tail1;			// E0
	BSFixedString	tailHub;		// E4
	BSFixedString	npcPelvis;		// E8
	BSFixedString	talking;		// EC
	BSFixedString	camera1st;		// F0
	BSFixedString	camera3rd;		// F4
	BSFixedString	headMeshForExport;	// F8
	BSFixedString	pinnedLimb;		// FC
	BSFixedString	backpack;		// 100
	BSFixedString	projectileNode;	// 104
	BSFixedString	blastRadiusNode;	// 108
	BSFixedString	torchFire;		// 10C
	BSFixedString	lightOn;		// 110
	BSFixedString	npcCom;			// 114
	BSFixedString	skinAttachment;	// 118
	BSFixedString	npcNeck;		// 11C
	BSFixedString	nifRound;		// 120
	BSFixedString	scb;			// 124
	BSFixedString	upperBody;		// 128
	BSFixedString	lightOff;		// 12C
	BSFixedString	headMagicNode;	// 130
	BSFixedString	lMagicNode;		// 134
	BSFixedString	rMagicNode;		// 138
	BSFixedString	magicLeft;		// 13C
	BSFixedString	magicRight;		// 140
	BSFixedString	magicOther;		// 144
	BSFixedString	cameraControl;	// 148
	BSFixedString	npcRoot;		// 14C
	BSFixedString	saddleBone;		// 150
	BSFixedString	perchFireNode;	// 154
	BSFixedString	animationGraphVariables[101];
};

// this is just part of NiBoneNames starting at offset 0xB0
class NiWeaponNodes
{
public:
	enum
	{
		kTypeWeapon1 = 0,
		kTypeWeaponSword,
		kTypeWeaponDagger,
		kTypeWeaponAxe,
		kTypeWeaponMace,
		kTypeWeaponBack1,
		kTypeWeaponBack2,
		kTypeWeaponBow1,
		kTypeWeapon2,
		kTypeWeaponBow2,
		kTypeUnknown,
		kNumTypes
	};

	BSFixedString	nodes[kNumTypes];
};
