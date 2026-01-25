#pragma once

#include "skse64/GameForms.h"
#include "skse64/NiObjects.h"
#include "skse64/GameEvents.h"

class Character;

// TESObject and children

// 20
class TESObject : public TESForm
{
public:
	virtual UInt32	Unk_39(void);
	virtual bool	Unk_3A(void);
	virtual UInt32	Unk_3B(void);
	virtual bool	Unk_3C(void);
	virtual void	Unk_3D(UInt32 arg);
	virtual UInt32	Unk_3E(UInt32 arg0, UInt32 arg1);
	virtual void	Unk_3F(void * arg);
	virtual bool	Unk_40(void);
	virtual bool	Unk_41(void);
	virtual void	Unk_42(void);
	virtual void	Unk_43(UInt32 arg0, UInt32 arg1);
	virtual UInt32	Unk_44(void);
	virtual UInt32	Unk_45(void);
	virtual void *	Unk_46(void);
};

// 30 
class TESBoundObject : public TESObject
{
public:
	struct Bound
	{
		UInt16	x;
		UInt16	y;
		UInt16	z;
	};

	virtual void	Unk_47(UInt32 arg);
	virtual UInt32	Unk_48(void);
	virtual UInt32	Unk_49(UInt32 arg);	// return Unk_3E(arg, 0);
										// model-related?
	virtual bool	Unk_4A(void);
	virtual bool	Unk_4B(void * arg0, BSString * dst);	// steal/take string
	virtual bool	Unk_4C(void * arg0, UInt8 arg1, UInt32 arg2, float arg3);
	virtual void	Unk_4D(UInt32 arg);
	virtual void	Unk_4E(UInt32 arg);
	virtual void	Unk_4F(void);
	virtual void	Unk_50(void);
	virtual void	Unk_51(void);

	Bound	bounds;		// 20
	Bound	bounds2;	// 26  (not 28!)
};

STATIC_ASSERT(offsetof(TESBoundObject, bounds2) == 0x26);
STATIC_ASSERT(sizeof(TESBoundObject) == 0x30);

// 48 
class BGSAcousticSpace : public TESBoundObject
{
public:
	enum { kTypeID = kFormType_AcousticSpace };

	TESForm	* loopingSound;	// 30
	TESForm	* soundRegion;	// 38
	TESForm	* reverbType;	// 40
};

// 80 
class BGSAddonNode : public TESBoundObject
{
public:
	enum { kTypeID = kFormType_AddonNode };

	// parents
	TESModelTextureSwap	texSwap;	// 30

	// members
	UInt32	unk68;		// 68
	UInt64	unk70;		// 70
	UInt8	unk78[4];	// 78
	UInt32	unk7C;		// 7C
};

// 70 
class BGSArtObject : public TESBoundObject
{
public:
	enum { kTypeID = kFormType_Art };

	// parents
	TESModelTextureSwap	texSwap;	// 30

	// members
	UInt32	unk68;	// 68
};

// This class was already removed in Skyrim32 and doesn't apply to SE, I'll leave it commented
/*
// 34
class BGSCloudClusterForm : public TESBoundObject
{
public:
	// parents
	TESModel	model;	// 20
};
*/

// 60 
class BGSDualCastData : public TESBoundObject
{
public:
	enum { kTypeID = kFormType_DualCastData };

	// 30
	struct Data
	{
		UInt64	unk00;	// 00
		UInt64	unk08;	// 08
		UInt64	unk10;	// 10
		UInt64	unk18;	// 18
		UInt64	unk20;	// 20
		UInt32	unk28;	// 28
		UInt32	pad2C;	// 2C
	};

	Data	unk30;	// 30
};

// E8 
class BGSExplosion : public TESBoundObject
{
public:
	enum { kTypeID = kFormType_Explosion };

	// parents
	TESFullName			fullName;		// 30
	TESModel			model;			// 40
	TESEnchantableForm	enchantment;	// 68
	BGSPreloadable		preloadable;	// 80
	TESImageSpaceModifiableForm	imageSpaceModifier;	// 88

	// members

	// 50
	struct Data
	{
		UInt64	unk00;	// 00
		UInt64	unk08;	// 08
		UInt64	unk10;	// 10
		UInt64	unk18;	// 18
		UInt64	unk20;	// 20 
		UInt64	unk28;	// 28
		UInt32	unk30;	// 30
		UInt32	unk34;	// 34
		UInt32	unk38;	// 38
		UInt32	unk3C;	// 3C
		UInt32	unk40;	// 40
		UInt32	unk44;	// 44
		UInt32	unk48;	// 48
		UInt32	unk4C;	// 4C
	};

	Data	data;	// 98
};

// B8 
class BGSHazard : public TESBoundObject
{
public:
	enum { kTypeID = kFormType_Hazard };

	// parents
	TESFullName		fullName;		// 30
	TESModel		model;			// 40
	BGSPreloadable	preloadable;	// 68
	TESImageSpaceModifiableForm	imageSpaceModifier;	// 70

	// members

	// 38
	struct Data
	{
		UInt32	unk00;	// 00
		UInt32	unk04;	// 04
		UInt32	unk08;	// 08
		UInt32	unk0C;	// 0C
		float	unk10;	// 10
		UInt32	unk14;	// 14
		UInt64	unk18;	// 18
		UInt64	unk20;	// 20
		UInt64	unk28;	// 28
		UInt64	unk30;	// 30
	};

	Data	unk80;	// 80
};

// 78 
class BGSIdleMarker : public TESBoundObject
{
public:
	enum { kTypeID = kFormType_IdleMarker };

	// parents
	TESModel			model;	// 30
	BGSIdleCollection	idle;	// 58
};

// B8 
class BGSNote : public TESBoundObject
{
public:
	enum { kTypeID = kFormType_Note };

	// parents
	TESModel		model;		// 30
	TESFullName		fullName;	// 58
	TESIcon			icon;		// 68
	BGSPickupPutdownSounds	sounds;	// 78

	// members
	struct Entry
	{
		void	* data;
		Entry	* next;
	};

	UInt64	unk90;		// 90
	UInt64	unk98;		// 98
	Entry	unkA0;		// A0
	UInt8	unkB0;		// B0
	UInt8	unkB1;		// B1
	UInt8	padB2[6];	// B2
};

// 138 
class BGSProjectile : public TESBoundObject
{
public:
	enum { kTypeID = kFormType_Projectile };

	// parents
	TESFullName		fullName;		// 030
	TESModel		model;			// 040
	BGSPreloadable	preloadable;	// 068
	BGSDestructibleObjectForm	destructible;	// 070

	// members

	// 88
	struct Data
	{
		UInt32	unk00;				// 00 - init'd to 65536
		UInt32	unk04;				// 04
		float	speed;				// 08 - init'd to 1000
		float	range;				// 0C - init'd to 10000
		void*	unk10;				// 10
		UInt32	unk18;				// 18
		UInt32	unk1C;				// 1C
		UInt64	unk20;				// 20
		UInt64	unk28;				// 28
		UInt64	unk30;				// 30
		BGSSoundDescriptorForm*	sound;	// 38
		UInt32	unk40;				// 40 
		float	fadeDuration;		// 44 - init'd to 0.5  
		float	impactForce;		// 48
		UInt32	pad4C;				// 4C
		UInt64	unk50;				// 50
		UInt64	unk58;				// 58
		UInt64	unk60;				// 60
		float	coneSpread;			// 68
		float	collisionRadius;	// 6C - init'd to 10
		UInt32	unk70;				// 70 - probably "lifetime" (float)
		float	relaunchInterval;	// 74
		UInt64	unk78;				// 78
		UInt64	unk80;				// 80
	};

	Data		data;		// 080
	TESModel	model108;	// 108
	UInt32		unk130;		// 130 - init'd to 1
};

// 68 
class BGSStaticCollection : public TESBoundObject
{
public:
	enum { kTypeID = kFormType_StaticCollection };

	// parents
	TESModelTextureSwap	texSwap;	// 30
};

class BSShaderTextureSet;

// 10
// TODO: virtual functions need check
class BSTextureSet : public NiObject
{
public:
	virtual const char * GetTexturePath(UInt32 index);
	virtual void	SetTexture(UInt32 index, /*NiSourceTexture*/void * texture);
	virtual void	SetTexturePath(UInt32 index, const char * path);

	enum
	{
		kTextureDiffuse = 0,
		kTextureNormal,
		kTextureEnvironmentMask,
		kTextureSubsurfaceTint = kTextureEnvironmentMask,
		kTextureGlowMap,
		kTextureDetailMap = kTextureGlowMap,
		kTextureHeight,
		kTextureEnvironment,
		kTextureMultilayer,
		kTextureBacklightMask,
		kTextureSpecular = kTextureBacklightMask,
		kTextureUnused08,
		kNumTextures = 9
	};
};

// 58 
class BSShaderTextureSet : public BSTextureSet
{
public:
	
	// According to constructor, they are all in a fixed array of 9
	const char *	textures[9];

	static BSShaderTextureSet * Create();

	MEMBER_FN_PREFIX(BSShaderTextureSet);
	DEFINE_MEMBER_FN(ctor, BSShaderTextureSet *, 0x012C9320);
};

STATIC_ASSERT(sizeof(BSShaderTextureSet) == 0x58);

// 130 
class BGSTextureSet : public TESBoundObject
{
public:
	enum { kTypeID = kFormType_TextureSet };

	// parents
	BSTextureSet	textureSet;	// 30

	// members

	// 0C
	struct Data
	{
		UInt32	unk00;	// 00
		UInt32	unk04;	// 04
		UInt32	unk08;	// 08
	};

	enum
	{
		kTextureDiffuse = 0,
		kTextureNormal,
		kTextureEnvironmentMask,
		kTextureSubsurfaceTint = kTextureEnvironmentMask,
		kTextureGlowMap,
		kTextureDetailMap = kTextureGlowMap,
		kTextureHeight,
		kTextureEnvironment,
		kTextureMultilayer,
		kTextureBacklightMask,
		kTextureSpecular = kTextureBacklightMask,
		//kTextureUnused08,	  // commented this one, it made the below array to be 9, not 8 as needed! 
		kNumTextures
	};

	TESTexture		texturePaths[kNumTextures];	// 40
	UInt64			unkC0;						// C0
	UInt16			unkC8;						// C8
	UInt8			padCA[6];					// CA
	Data			unkD0[kNumTextures];		// D0
};

STATIC_ASSERT(sizeof(BGSTextureSet) == 0x130);

// 90 
class MagicItem : public TESBoundObject
{
public:
	virtual UInt32	Unk_52(void);	// pure
	virtual void	Unk_53(UInt32 arg);
	virtual UInt32	Unk_54(void);	// pure
	virtual void	Unk_55(UInt32 arg);
	virtual UInt32	Unk_56(void);	// pure
	virtual bool	Unk_57(UInt32 arg);
	virtual float	Unk_58(void);
	virtual float	Unk_59(void);
	virtual bool	Unk_5A(void);
	virtual bool	Unk_5B(void);
	virtual bool	Unk_5C(void);
	virtual bool	Unk_5D(void);
	virtual bool	Unk_5E(void);
	virtual bool	Unk_5F(UInt32 arg);
	virtual bool	Unk_60(void);
	virtual bool	Unk_61(void);
	virtual void	Unk_62(UInt32 arg0, UInt32 arg1);
	virtual float	Unk_63(void);
	virtual UInt32	Unk_64(void);
	virtual UInt32	Unk_65(void);
	virtual bool	Unk_66(void);
	virtual UInt32	Unk_67(void);	// pure, returns char code
	virtual void	Unk_68(MagicItem * src);	// pure, copy data if possible?
	virtual void	Unk_69(UInt32 arg0, UInt32 arg1);	// loading-related
	virtual void	Unk_6A(UInt32 arg);
	virtual void *	Unk_6B(void);	// pure, returns data
	virtual void *	Unk_6C(void);	// pure, returns data
	virtual UInt32	Unk_6D(void);	// pure, return size of data
	virtual void	Unk_6E(void);	// pure, byteswap?

	// parents
	TESFullName		fullName;	// 30
	BGSKeywordForm	keyword;	// 40

	// members
	struct EffectItem
	{
		float	magnitude;		// 00
		UInt32	area;			// 04
		UInt32	duration;		// 08
		UInt32	pad0C;			// 0C
		EffectSetting* mgef;	// 10
		float	cost;			// 18 - ?
		UInt32	unk1C;			// 1C - probably pad
		void	*unk20;			// 20 - looks like the condition

		EffectItem()
		{
			magnitude = 0;
			area = 0;
			duration = 0;
			mgef = NULL;
			cost = 0.0;
			unk1C = 0;
			unk20 = NULL;
		}

		DEFINE_STATIC_HEAP(Heap_Allocate, Heap_Free);
	};

	tArray<EffectItem*> effectItemList;	// 58
	UInt32				hostile;	// 70
	EffectSetting*		effectTemplate;	// 78
	UInt32				unk80;	// 80
	UInt64				unk88;	// 88

	MEMBER_FN_PREFIX(MagicItem);
	DEFINE_MEMBER_FN(GetCostliestEffectItem, EffectItem *, 0x00101CC0, int arg1, bool arg2);
	DEFINE_MEMBER_FN(GetEffectiveMagickaCost, float, 0x00101A30, Character* caster);
};

STATIC_ASSERT(sizeof(MagicItem) == 0x90);

// 168 
class AlchemyItem : public MagicItem
{
public:
	enum { kTypeID = kFormType_Potion };

	enum
	{
		kFlag_ManualCalc =	0x00000001,
		kFlag_Food =		0x00000002,
		kFlag_Medicine =	0x00010000,
		kFlag_Poison =		0x00020000,
	};

	// parents
	TESModelTextureSwap	texSwap;		// 090
	TESIcon				icon;			// 0C8
	BGSMessageIcon		messageIcon;	// 0D8
	TESWeightForm		weight;			// 0F0
	BGSEquipType		equipType;		// 100
	BGSDestructibleObjectForm	destructibleObj;	// 110
	BGSPickupPutdownSounds		sounds;	// 120

	// members

	// 20
	struct Data
	{
		UInt32	value;	// 00 - init'd to FFFFFFFF
		UInt32	flags;	// 04
		UInt64	unk08;	// 08 addiction (legacy?)
		UInt32	unk10;	// 10 addiction chance (legacy?)
		UInt32	pad14;	// 14
		BGSSoundDescriptorForm *	useSound;	// 18
	};

	Data	itemData;	// 138
	TESIcon	unkB8;		// 158

	bool IsFood() { return (itemData.flags & kFlag_Food) != 0; }
	bool IsPoison() { return (itemData.flags & kFlag_Poison) != 0; }
};

// C0 
class EnchantmentItem : public MagicItem
{
public:
	enum { kTypeID = kFormType_Enchantment };

	enum
	{
		kFlag_ManualCalc =	0x00000001,
	};

	// 30
	struct Data
	{
		struct Calculations
		{
			UInt32	cost;	// 00 - init'd to FFFFFFFF
			UInt32	flags;	// 04
		};

		Calculations	calculations;	// 00
		UInt32	unk08;					// 08
		UInt32	amount;					// 0C - init'd to FFFFFFFF
		UInt32	deliveryType;			// 10 - init'd to 5
		UInt32	castingType;			// 14 - init'd to 6
		float	castTime;					// 18
		UInt32	pad1C;						// 1C
		EnchantmentItem	* baseEnchantment;	// 20
		BGSListForm		* restrictions;		// 28
	};

	Data	data;	// 90
};

// 140 
class IngredientItem : public MagicItem
{
public:
	enum { kTypeID = kFormType_Ingredient };

	// parents
	TESModelTextureSwap	texSwap;	// 090
	TESIcon			icon;		// 0C8
	TESWeightForm	weight;		// 0D8
	BGSEquipType	equipType;	// 0E8
	BGSDestructibleObjectForm	destructible;	// 0F8
	BGSPickupPutdownSounds		sounds;			// 108
	TESValueForm	value;		// 120

	// members

	// SE: I'm taking members out of the several structures, they break alignment 
	
	UInt32 unk130; // 00 - init'd to FFFFFFFF
	UInt32 unk134; // 04	

	// ahzaab 8-25-13
	enum   // type - these are flags
	{
		kType_NoEffect =     0,
		kType_FirstEffect =  1 << 0,
		kType_SecondEffect = 1 << 1,
		kType_ThirdEffect =  1 << 2,
		kType_FourthEffect = 1 << 3
	};
	
	UInt8  knownEffects;  // 138       //The lower nibble contains the known effects, the upper nibble is unknown

	UInt8	unk139;	// 139
	UInt16	unk13A; // 13A	
	UInt32	pad13C; // 13C
};

class Character;

// E8 
class SpellItem : public MagicItem
{
public:
	enum { kTypeID = kFormType_Spell };

	// parents
	BGSEquipType			equipType;		// 90
	BGSMenuDisplayObject	dispObj;		// A0
	TESDescription			description;	// B0

	// members

	enum
	{
		kTypeSpell = 0,
		kTypeDisease,
		kTypePower,
		kTypeLesserPower,
		kTypeAbility,
		kTypePoison,
		kTypeAddition,
		kTypeVoice
	};

	// 24-28
	struct Data
	{
		struct Data0
		{
			UInt32	cost;	// 00 - init'd to FFFFFFFF
			UInt32	flags;	// 04
		};

		Data0	unk00;	// 00
		UInt32	type;	// 08
		float	castTime;	// 0C
		UInt32	castType;	// 10 - init'd to 3
		UInt32	unk14;	// 14 - init'd to 5
		UInt32	unk18;	// 18
		UInt32	unk1C;	// 1C
		BGSPerk*	spellPerk;	// 20
	};

	Data	data;	// C0

	UInt32	GetMagickaCost() { return data.unk00.cost; }
};

// 168 
class ScrollItem : public SpellItem
{
public:
	enum { kTypeID = kFormType_ScrollItem };

	// parents
	TESModelTextureSwap			texSwap;		// E8
	BGSDestructibleObjectForm	destructible;	// 120
	BGSPickupPutdownSounds		sounds;			// 130
	TESWeightForm				weight;			// 148
	TESValueForm				value;			// 158
};

// 128
class TESAmmo : public TESBoundObject
{
public:
	enum { kTypeID = kFormType_Ammo };

	// parents
	TESFullName					fullName;		// 030
	TESModelTextureSwap			texSwap;		// 040
	TESIcon						icon;			// 078
	BGSMessageIcon				messageIcon;	// 088
	TESValueForm				value;			// 0A0
	TESWeightForm				weight;			// 0B0
	BGSDestructibleObjectForm	destructible;	// 0C0
	BGSPickupPutdownSounds		sounds;			// 0D0
	TESDescription				description;	// 0E8
	BGSKeywordForm				keyword;		// 0F8

	// members

	// 10
	struct AmmoSettings
	{
		BGSProjectile	* projectile;	// 0
		UInt32			flags;			// 8
		float			damage;			// C
	};

	enum {
		kIgnoreNormalResist	= (1 << 0),
		kNotPlayable		= (1 << 1),
		kNotBolt			= (1 << 2)
	};

	bool isBolt() { return (settings.flags & kNotBolt) != kNotBolt; }
	bool isPlayable() { return (settings.flags & kNotPlayable) != kNotPlayable; }

	AmmoSettings	settings;	// 110
	BSFixedString	unk120;		// 120
};
STATIC_ASSERT(sizeof(TESAmmo) == 0x128);

// 30 
class TESBoundAnimObject : public TESBoundObject
{
public:
};

// 150 
class TESActorBase : public TESBoundAnimObject
{
public:
	// parents
	TESActorBaseData	actorData;	// 030
	TESContainer		container;	// 088
	TESSpellList		spellList;	// 0A0
	TESAIForm			aiForm;		// 0B0
	TESFullName			fullName;	// 0D8
	ActorValueOwner				actorValueOwner;	// 0E8
	BGSDestructibleObjectForm	destructible;		// 0F0
	BGSSkinForm			skinForm;	// 100
	BGSKeywordForm		keyword;	// 110
	BGSAttackDataForm	attackData;	// 128
	BGSPerkRankArray	perkRanks;	// 138 

	virtual bool			Unk_52(void);
	virtual UInt32			Unk_53(void);
	virtual void			Unk_54(UInt32 arg);
	virtual TESActorBase *	Unk_55(void);
};
STATIC_ASSERT(offsetof(TESActorBase, keyword) == 0x110);
STATIC_ASSERT(sizeof(TESActorBase) == 0x150);

class BSFaceGenNiNode;

// 268 
class TESNPC : public TESActorBase
{
public:
	enum { kTypeID = kFormType_NPC };

	struct FaceMorphs
	{
		enum
		{
			kNumOptions =	19,
			kNumPresets =	4,
		};
		
		float	option[kNumOptions];
		UInt32	presets[kNumPresets];
	};

	struct HeadData {
		BGSColorForm * hairColor;
		BGSTextureSet * headTexture;	// Only seems to apply to the player
	};


	// parents
	TESRaceForm		race;	// 150
	BGSOverridePackCollection	overridePacks;	// 160
	/*BSTEventSink <void *>*/void*	menuOpenCloseEvent;	// 188 - MenuOpenCloseEvent

	// members @190
	UInt8		unk190[0x12];	// 190 - init'd to 5
	UInt8		unk1A2[0x12];	// 1A2 - init'd to 0

	UInt8		pad1B4[0x1BA - 0x1B4];	// 1B4

	UInt16		unk1BA;				// 1BA
	UInt32		pad1BC;				// 1BC
	TESClass*	npcClass;			// 1C0

	HeadData	* headData;			// 1C8
	UInt64		unk1D0;				// 1D0
	TESCombatStyle*	combatStyle;	// 1D8
	UInt32		unk1E0;				// 1E0
	UInt32		pad1E4;				// 1E4
	TESRace		* overlayRace;		// 1E8
	TESNPC		* nextTemplate;		// 1F0
	float		height;				// 1F8 - init'd to 1
	float		weight;				// 1FC - init'd to 50

	UInt64		unk200;				// 200
	
	BSFixedString	shortName;		// 208
	TESObjectARMO*	skinFar;		// 210
	BGSOutfit*		defaultOutfit;	// 218
	BGSOutfit*		sleepOutfit;	// 220
	UInt64			unk228;			// 228
	TESFaction*		faction;		// 230
	BGSHeadPart		** headparts;	// 238
	UInt8			numHeadParts;	// 240
	UInt8			unk241;			// 241 - init'd to 1
	UInt8			unk242;			// 242 - New in SE?
	UInt8			unk243;			// 243
	UInt8			unk244;			// 244 - New in SE?
	UInt8			unk245;			// 245 - init'd to 1
	struct Color { // 797979 Transparent
		UInt8   red, green, blue; // 246 - 248 - Skin Color
	} color;
	UInt8			pad249[7];		// 249
	UInt64			unk250;			// 250 // Relationships?

	FaceMorphs		* faceMorph;	// 258
	UInt64			unk260;			// 260

	MEMBER_FN_PREFIX(TESNPC);
	DEFINE_MEMBER_FN(GetSex, char, 0x003512A0);
	DEFINE_MEMBER_FN(HasOverlays, bool, 0x00368A20);

	// Swaps a headPart of the same type as target with target
	DEFINE_MEMBER_FN(ChangeHeadPart, void, 0x00365BF0, BGSHeadPart * target);

	struct MorphAction {
		BSFaceGenNiNode * faceNode;
		TESNPC * npc;
		const char ** morphName;
		float	value;
	};
	
	// Updates the neck seam when weight changed
	DEFINE_MEMBER_FN(UpdateNeck, void, 0x003606D0, BSFaceGenNiNode * faceNode);

	// Computes RGB SkinTone from RGBA TintMask
	DEFINE_MEMBER_FN(SetSkinFromTint, void, 0x00360480, NiColorA * result, TintMask * tintMask, bool fromTint);

	void SetFaceTexture(BGSTextureSet * textureSet);
	void SetHairColor(BGSColorForm * hairColor);

	BGSHeadPart * GetHeadPartByType(UInt32 type);
	BGSHeadPart * GetHeadPartOverlayByType(UInt32 type);
	BGSHeadPart * GetCurrentHeadPartByType(UInt32 type);
	TESNPC * GetRootTemplate();
};
STATIC_ASSERT(offsetof(TESNPC, nextTemplate) == 0x1F0);
STATIC_ASSERT(sizeof(TESNPC) == 0x268);

// 90 
class TESLevCharacter : public TESBoundAnimObject
{
public:
	enum { kTypeID = kFormType_LeveledCharacter };

	TESLeveledList		leveledList;	// 30
	TESModelTextureSwap	texSwap;		// 58
};

// C8 
class TESObjectACTI : public TESBoundAnimObject
{
public:
	enum { kTypeID = kFormType_Activator };

	// parents
	TESFullName					fullName;		// 20-30
	TESModelTextureSwap			texSwap;		// 28-40
	BGSDestructibleObjectForm	destructible;	// 44-78
	BGSOpenCloseForm			openClose;		// 4C-88
	BGSKeywordForm				keyword;		// 50-90

	// members
	UInt64	unkA8;		// A8
	UInt64	unkB0;		// B0
	UInt64	unkB8;		// B8
	UInt16	unkC0;		// C0
	UInt8	padC2[6];	// C2
};

// D8 
class BGSTalkingActivator : public TESObjectACTI
{
public:
	enum { kTypeID = kFormType_TalkingActivator };

	void			* unkC8;	// C8
	BGSVoiceType	* unkD0;	// D0
};

// E8 
class TESFlora : public TESObjectACTI
{
public:
	enum { kTypeID = kFormType_Flora };

	// parents
	TESProduceForm	produce;	// C8
};

// F0 
class TESFurniture : public TESObjectACTI
{
public:
	enum { kTypeID = kFormType_Furniture };

	// 4
	struct DataE0
	{
		UInt8	unk0;		// 0
		UInt8	unk1;		// 1 - init'd to FF
		UInt8	pad2[2];	// 2
	};

	UnkArray	unkC8;	// C8
	DataE0	unkE0;	// E0
	UInt32	unkE4;	// E4
	UInt64	unkE8;	// E8
};

// D0 
class TESObjectCONT : public TESBoundAnimObject
{
public:
	enum { kTypeID = kFormType_Container };

	// parents
	TESContainer				container;		// 30
	TESFullName					fullName;		// 48
	TESModelTextureSwap			texSwap;		// 58
	TESWeightForm				weight;			// 90
	BGSDestructibleObjectForm	destructible;	// A0
	BGSOpenCloseForm			openClose;		// B0

	// members
	UInt8	unkB8;	// B8
	UInt8	unkB9;	// B9
	UInt8	padBA[6];	// BA
	UInt64	padC0;	// C0
	UInt64	padC8;	// C8
};

// D0 
class TESObjectDOOR : public TESBoundAnimObject
{
public:
	enum { kTypeID = kFormType_Door };

	// parents
	TESFullName					fullName;		// 30
	TESModelTextureSwap			texSwap;		// 40
	BGSDestructibleObjectForm	destructible;	// 78
	BGSOpenCloseForm			openClose;		// 88

	// members
	UInt64	unk90;		// 90
	UInt64	unk98;		// 98
	UInt64	unkA0;		// A0
	UInt64	unkA8;		// A8
	UInt8	unkB0;		// B0
	UInt8	padB1[7];	// B1
	UnkArray	unkB8;	// 64-B8
};

// B4-130
class TESObjectLIGH : public TESBoundAnimObject
{
public:
	enum { kTypeID = kFormType_Light };

	// parents
	TESFullName					fullName;		// 20-030
	TESModelTextureSwap			texSwap;		// 28-040
	TESIcon						icon;			// 44-078
	BGSMessageIcon				messageIcon;	// 4C-088
	TESWeightForm				weight;			// 58-0A0
	TESValueForm				value;			// 60-0B0
	BGSDestructibleObjectForm	destructible;	// 68-0C0
	BGSEquipType				equipType;		// 70-0D0

	// members (78)

	// 28
	struct DataE0
	{
		SInt32	time;	// 00 time ?
		UInt32	radius;	// 04
		struct Color { 
			UInt8   red, green, blue;
		}		color;	// 08
		UInt32	unk0C;	// 0C
		float	unk10;	// 10
		float	FOV;	// 14 - 90
		float	unk18;	// 18 - .001
		float	unk1C;	// 1C
		UInt32	unk20;	// 20
		UInt32	unk24;	// 24
	};

	// 18
	struct Data118
	{
		UInt64	unk0;	// 00
		UInt32	unk4;	// 08
		UInt32	padC;	// 0C
		UInt64	unk8;	// 10
	};

	DataE0	unkE0;	// 78-0E0
	float	fade;	// A0-108 - init'd to 
	UInt32	pad10C;	// 10C
	UInt64	unkA4;	// A4-110
	Data118	unk118;	// 118
};

STATIC_ASSERT(sizeof(TESObjectLIGH) == 0x130);

// 38 
class TESSound : public TESBoundAnimObject
{
public:
	enum { kTypeID = kFormType_Sound };

	BGSSoundDescriptorForm	* descriptor;	// 30
};

// 78 
class TESGrass : public TESBoundObject
{
public:
	enum { kTypeID = kFormType_Grass };

	virtual UInt8	GetUnk34(void);				// return unk34;
	virtual bool	SetUnk34(UInt8 data);		// if(data > 100) return false;
												// unk34 = data; return true;
	virtual UInt8	GetUnk35(void);				// return unk35;
	virtual bool	SetUnk35(UInt8 data);		// if(data > 90) return false;
												// if(data > unk36) return false;
												// unk35 = data; return true;
	virtual UInt8	GetUnk36(void);				// return unk36;
	virtual bool	SetUnk36(UInt8 data);		// if(data > 90) return false;
												// if(data < unk35) return false;
												// unk36 = data; return true;
	virtual float	GetUnk35_Float(void);		// return unk35 * (M_PI / 180);
	virtual float	GetUnk36_Float(void);		// return unk36 * (M_PI / 180);
	virtual UInt16	GetUnk38(void);				// return unk38;
	virtual void	SetUnk38(UInt16 data);		// unk38 = data;
	virtual UInt32	GetUnk3C(void);				// return unk3C;
	virtual void	SetUnk3C(UInt32 data);		// unk3C = data;
	virtual float	GetUnk40(void);				// return unk40;
	virtual bool	SetUnk40(float data);		// if((data < 0) || (data > 512)) return false;
												// unk40 = data; return true;
	virtual float	GetUnk44(void);				// return unk44;
	virtual bool	SetUnk44(float data);		// if((data < 0) || (data > 1)) return false;
												// unk44 = data; return true;
	virtual float	GetUnk48(void);				// return unk48;
	virtual bool	GetUnk48(float data);		// if((data < 0) || (data > 1)) return false;
												// unk48 = data; return true;
	virtual float	GetUnk4C(void);				// return unk4C;
	virtual bool	SetUnk4C(float data);		// if(data <= 0) return false;
												// unk4C = data; return true;
	virtual bool	GetUnk50_01(void);			// return (unk50 & 1) != 0;
	virtual void	SetUnk50_01(UInt8 data);	// if(data) unk50 |= 1; else unk50 &= ~1;
	virtual bool	GetUnk50_02(void);			// return (unk50 & 2) != 0;
	virtual void	SetUnk50_02(UInt8 data);	// if(data) unk50 |= 2; else unk50 &= ~2;
	virtual bool	GetUnk50_04(void);			// return (unk50 & 4) != 0;
	virtual void	SetUnk50_04(UInt8 data);	// if(data) unk50 |= 4; else unk50 &= ~4;

	// parents
	TESModel	model;		// 30

	// members
	UInt8		unk58;		// 58 - init'd to 30
	UInt8		unk59;		// 59 - degrees
	UInt8		unk5A;		// 5A - init'd to 90 - degrees
	UInt8		pad5B;		// 5B
	UInt16		unk5C;		// 5C
	UInt8		pad5E[2];	// 5E
	UInt32		unk60;		// 60
	float		unk64;		// 64 - init'd to 32
	float		unk68;		// 68 - init'd to 0.2
	float		unk6C;		// 6C - init'd to 0.5
	float		unk70;		// 70 - init'd to 10
	UInt8		unk74;		// 74
	UInt8		pad75[3];	// 75
};

// 58 
class TESLevItem : public TESBoundObject
{
public:
	enum { kTypeID = kFormType_LeveledItem };

	// parents
	TESLeveledList	leveledList;	// 30
};

// 58 
class TESLevSpell : public TESBoundObject
{
public:
	enum { kTypeID = kFormType_LeveledSpell };

	// parents
	TESLeveledList	leveledList;	// 30
};

class TESObjectARMA;

// 228 
class TESObjectARMO : public TESBoundObject
{
public:
	enum { kTypeID = kFormType_Armor };

	// parents
	TESFullName					fullName;		// 030
	TESRaceForm					race;			// 040
	TESEnchantableForm			enchantable;	// 050
	TESValueForm				value;			// 068
	TESWeightForm				weight;			// 078
	BGSDestructibleObjectForm	destructible;	// 088
	BGSPickupPutdownSounds		pickupSounds;	// 098
	TESBipedModelForm			bipedModel;		// 0B0
	BGSEquipType				equipType;		// 1A0
	BGSBipedObjectForm			bipedObject;	// 1B0
	BGSBlockBashData			blockBash;		// 1C0
	BGSKeywordForm				keyword;		// 1D8
	TESDescription				description;	// 1F0

	// members
	UInt32						armorValTimes100;	// 200
	UInt32						pad204;				// 204
	tArray<TESObjectARMA *>		armorAddons;		// 208
	TESObjectARMO				* templateArmor;	// 220 - enchantment related?
};

STATIC_ASSERT(offsetof(TESObjectARMO, armorValTimes100) == 0x200);
STATIC_ASSERT(sizeof(TESObjectARMO) == 0x228);

// 138 
class TESObjectBOOK : public TESBoundObject
{
public:
	enum { kTypeID = kFormType_Book };

	// parents
	TESFullName					fullName;		// 30
	TESModelTextureSwap			texSwap;		// 40
	TESIcon						icon;			// 78
	TESValueForm				value;			// 88
	TESWeightForm				weight;			// 98
	TESDescription				description;	// A8
	BGSDestructibleObjectForm	destructible;	// B8
	BGSMessageIcon				messageIcon;	// C8
	BGSPickupPutdownSounds		pickupSounds;	// E0
	BGSKeywordForm				keyword;		// F8

	// members
	struct Data
	{
		enum	// type - these are technically flags
		{
			kType_None	=	0,
			kType_Skill =	1 << 0,
			kType_CantBeTaken	=	1 << 1,
			kType_Spell =	1 << 2,	// takes priority over skill
			kType_Read	=	1 << 3, // set once the book is equipped by the player, along with the CHANGE_BOOK_READ (0x40) change flag
		};

		UInt8	flags;		// 0
		UInt8	type;		// 1
		UInt16	unk02;		// 2, probably padding too
		UInt32	unk04;		// 4, probably padding (SE)
		
		union
		{
			UInt32		skill;
			SpellItem	* spell;
		} teaches;			// 8		

		// make some sense of the flags field so we know what's in the union
		UInt32	GetSanitizedType(void);
	};

	Data			data;			// 110
	TESObjectSTAT	* bookStat;		// 120
	TESDescription	description2;	// 128
};

STATIC_ASSERT(sizeof(TESObjectBOOK) == 0x138);

// 100 
class TESObjectMISC : public TESBoundObject
{
public:
	enum { kTypeID = kFormType_Misc };

	// parents
	TESFullName					fullName;		// 30
	TESModelTextureSwap			texSwap;		// 40
	TESIcon						icon;			// 78
	TESValueForm				value;			// 88
	TESWeightForm				weight;			// 98
	BGSDestructibleObjectForm	destructible;	// A8
	BGSMessageIcon				messageIcon;	// B8
	BGSPickupPutdownSounds		pickupSounds;	// D0
	BGSKeywordForm				keyword;		// E8

	virtual void	Unk_52(void);
	virtual void	Unk_53(UInt32 arg0, UInt32 arg1);
	virtual void	Unk_54(void);
};

// 120 
class BGSApparatus : public TESObjectMISC
{
public:
	enum { kTypeID = kFormType_Apparatus };

	// parents
	TESQualityForm	quality;		// 100
	TESDescription	description;	// 110
};

// 100 
class TESKey : public TESObjectMISC
{
public:
	enum { kTypeID = kFormType_Key };
};

// 110 
class TESSoulGem : public TESObjectMISC
{
public:
	enum { kTypeID = kFormType_SoulGem };

	UInt64	unk100;		// 100
	UInt8	soulSize;	// 108
	UInt8	gemSize;	// 109
	UInt8	pad10A[6];	// 10A
};


// 80 
class TESObjectSTAT : public TESBoundObject
{
public:
	enum { kTypeID = kFormType_Static };

	// parents
	TESModelTextureSwap		texSwap;	// 30
	
	// members
	float unk68;	// 68 - init'd to 90
	UInt64	unk70;	// 70
	
	// New members in SE:
	UInt8	unk78;	// 78
	UInt8	unk79;	// 79
	UInt8	unk7A;	// 7A
	UInt8	unk7B;	// 7B

	UInt32	pad7C;	// 7C
};

// B0 
// they probably wanted this to derive from TESObjectSTAT first
// doesn't really hurt anything except performance (shims)
class BGSMovableStatic : public TESFullName
{
public:
	enum { kTypeID = kFormType_MovableStatic };

	// parents
	BGSDestructibleObjectForm	destructible;	// 10
	TESObjectSTAT				staticObj;		// 20

	// members
	void*	unkA0;		// A0 - New in SE?
	UInt8	unkA8;		// A8
	UInt8	pad59[7];	// A9
};

// C8 
class TESObjectTREE : public TESBoundObject
{
public:
	enum { kTypeID = kFormType_Tree };

	// parents
	TESModel		model;		// 30
	TESFullName		fullName;	// 58
	TESProduceForm	produce;	// 68
	
	// members
	// 40
	struct Data88
	{
		float	unk00;	// 00 - init'd to 1
		float	unk04;	// 04 - init'd to 1
		float	unk08;	// 08 - init'd to .03
		float	unk0C;	// 0C - init'd to .6
		float	unk10;	// 10 - init'd to .2
		float	unk14;	// 14 - init'd to .4
		float	unk18;	// 18 - init'd to .075
		float	unk1C;	// 1C - init'd to .025
		float	unk20;	// 20 - init'd to .035
		float	unk24;	// 24 - init'd to 1
		float	unk28;	// 28 - init'd to 1
		float	unk2C;	// 2C - init'd to 1
		UInt64	unk30;	// 30
		UInt32	unk38;	// 38 - init'd to 2
		UInt32	pad3C;	// 3C
	};

	Data88	unk88;	// 88
};

// 220 
class TESObjectWEAP : public TESBoundObject
{
public:
	enum { kTypeID = kFormType_Weapon };

	// parents
	TESFullName					fullName;		// 030
	TESModelTextureSwap			texSwap;		// 040
	TESIcon						icon;			// 078
	TESEnchantableForm			enchantable;	// 088
	TESValueForm				value;			// 0A0
	TESWeightForm				weight;			// 0B0
	TESAttackDamageForm			damage;			// 0C0
	BGSDestructibleObjectForm	destructible;	// 0D0
	BGSEquipType				equipType;		// 0E0
	BGSPreloadable				preloadable;	// 0F0
	BGSMessageIcon				messageIcon;	// 0F8
	BGSPickupPutdownSounds		pickupSounds;	// 110
	BGSBlockBashData			blockBash;		// 128
	BGSKeywordForm				keyword;		// 140
	TESDescription				description;	// 158

	// members

	// 38
	struct GameData
	{
		enum	// type
		{
			kType_HandToHandMelee = 0,
			kType_OneHandSword,
			kType_OneHandDagger,
			kType_OneHandAxe,
			kType_OneHandMace,
			kType_TwoHandSword,
			kType_TwoHandAxe,
			kType_Bow,
			kType_Staff,
			kType_CrossBow,
			kType_H2H,
			kType_1HS,
			kType_1HD,
			kType_1HA,
			kType_1HM,
			kType_2HS,
			kType_2HA,
			kType_Bow2,
			kType_Staff2,
			kType_CBow
		};

		enum Flags1 {
			kFlags_PlayerOnly = 0x01,
			kFlags_NPCUseAmmo = 0x02,
			kFlags_NoJamAfterReload = 0x04,
			kFlags_MinorCrime = 0x10,
			kFlags_NotUsedInNormalCombat = 0x40,
			kFlags_Unknown1 = 0x100,
			kFlags_LongBursts = 0x800,
			kFlags_NonHostile = 0x1000,
			kFlags_BoundWeapon = 0x2000,
		};

		enum Flags2 {
			kFlags_Hidebackpack = 0x01,
			kFlags_Automatic = 0x02,
			kFlags_CantDrop = 0x08,
			kFlags_EmbeddedWeapon = 0x20,
			kFlags_Unknown2 = 0x40,
			kFlags_NotPlayable = 0x80,
		};

		void	*unk00;	// 00
		float	speed;	// 08
		float	reach;	// 0C 
		float	minRange;	// 10 - init'd to 500
		float	maxRange;	// 14 - init'd to 2000
		float	animationMult;	// 18
		float	unk18;	// 1C
		float	stagger;// 20 - init'd to 1
		UInt32	unk24;	// 24
		UInt32	skill;	// 28 - init'd to FFFFFFFF
		UInt32	resist;	// 2C - init'd to FFFFFFFF
		UInt16	flags1;	// 30
		UInt8	vatsChance;	// 32 - init'd to 5
		UInt8	unk33;	// 33 - init'd to FF
		UInt8	unk34;	// 34
		UInt8	type;	// 35
		UInt8	flags2;	// 36
		UInt8	unk37;	// 37
	};

	// 18
	struct CritData
	{
		float	critMult;			// 00 - init'd to 1
		UInt32	pad04;				// 04
		SpellItem	* critEffect;	// 08
		UInt16	critDamage;			// 10
		UInt8	effectOnDeath;		// 12 - init'd to 1
		UInt8	pad13[5];			// 13
	};

	GameData	gameData;	// 168
	CritData	critData;	// 1A0
	TESForm	* scopeEffect;		// 1B8
	BGSSoundDescriptorForm	* attackSound;		// 1C0
	BGSSoundDescriptorForm	* attackSound2D;	// 1C8
	BGSSoundDescriptorForm	* attackLoopSound;	// 1D0
	BGSSoundDescriptorForm	* attackFailSound;	// 1D8
	BGSSoundDescriptorForm	* idleSound;		// 1E0
	BGSSoundDescriptorForm	* equipSound;		// 1E8
	BGSSoundDescriptorForm	* unequipSound;		// 1F0
	BGSImpactDataSet	* impactDataSet;	// 1F8
	TESObjectSTAT	* model;				// 200
	TESObjectWEAP	* templateForm;			// 208 - Non-weapon templates don't make sense here and would probably crash anyway so assume it
	BSFixedString	embeddedNode;			// 210
	UInt32	unk218;							// 218
	UInt32	pad21C;							// 21C

	float speed() { return gameData.speed; }
	float reach() { return gameData.reach; }
	float stagger() { return gameData.stagger; }
	float minRange() { return gameData.minRange; }
	float maxRange() { return gameData.maxRange; }
	UInt8 type() { return gameData.type; }
	UInt16 critDamage() { return critData.critDamage; }

	void GetNodeName(char * dstBuff);
};
STATIC_ASSERT(offsetof(TESObjectWEAP, messageIcon) == 0xF8);
STATIC_ASSERT(sizeof(TESObjectWEAP) == 0x220);

// 178 
class TESObjectARMA : public TESObject
{
public:
	enum { kTypeID = kFormType_ARMA };

	// parents
	TESRaceForm			race;	// 20
	BGSBipedObjectForm	biped;	// 30

	// members

	// 10
	struct Data40
	{
		UInt8	priority[2];	// 00
		UInt8	unk02;			// 02
		UInt8	unk03;			// 03
		UInt8	unk04;			// 04
		UInt8	unk05;			// 05
		UInt8	unk06;			// 06
		UInt8	pad07;			// 07
		UInt32	unk08;			// 08
		UInt32	pad0C;			// 0C
	};

	Data40						data;					// 40
	TESModelTextureSwap			models[2][2];			// 50
	UInt64						unk130;					// 130
	UInt64						unk138;					// 138
	UInt64						unk140;					// 140
	UInt64						unk148;					// 148
	tArray<TESRace*>			additionalRaces;		// 150
	BGSFootstepSet				* footstepSet;			// 168
	UInt64						unk170;					// 170

	bool isValidRace(TESRace * race) const;
	void GetNodeName(char * dstBuff, TESObjectREFR * refr, TESObjectARMO * armor, float weightOverride);
};

STATIC_ASSERT(sizeof(TESObjectARMA) == 0x178);

class ActiveEffect;

// 10 
class ActiveEffectReferenceEffectController
{
public:
	virtual ~ActiveEffectReferenceEffectController();

//	void			** _vtbl;	// 00
	ActiveEffect	* effect;	// 08
	// possibly more
};

class ActiveEffect
{
public:
	enum { kTypeID = kFormType_ActiveMagicEffect };

	enum
	{
		kFlag_Inactive = 0x8000
	};

	virtual ~ActiveEffect();

//	void					** _vtbl;		// 00
	ActiveEffectReferenceEffectController	controller;	// 08
	UInt32					unk18;			// 18
	float					unk1C;			// 1C
	float					unk20;			// 20
	float					unk24;			// 24
	UInt32					unk28;			// 28 - init'd to FFFFFFFF
	UInt8					unk2C;			// 2C
	UInt8					pad2D[3];		// 2D
	UInt32					unk30;			// 30
	UInt32					unk34;			// 34
	void					* niNode;		// 38
	MagicItem				* item;			// 40 - init'd to ctor argument
	MagicItem::EffectItem	* effect;		// 48 - init'd to ctor argumet
	TESObjectREFR			* reference;	// 50
	TESForm					* sourceItem;	// 58
	UInt64					unk60;			// 60
	UInt64					un68;			// 68
	float					elapsed;		// 70
	float					duration;		// 74
	float					magnitude;		// 78
	UInt32					flags;			// 7C
	UInt32					unk80;			// 80 - init'd to 1
	UInt32					effectNum;		// 84 - Somekind of counter used to determine whether the ActiveMagicEffect handle is valid
	UInt32					unk88;			// 88 - init'd to 4
	UInt32					pad8C;			// 8C
	UInt32					actorValue;		// 90 - Only seems to appear on value modifiers
	UInt32					unk94;			// 94
	UInt64					unk98;			// 98
};

class ScriptEffect : public ActiveEffect
{
public:
	virtual ~ScriptEffect();

	// ??
};

class ScriptedRefEffect : public ScriptEffect
{
public:
	virtual ~ScriptedRefEffect();

	// ??
};

class SlowTimeEffect : public ScriptEffect
{
public:
	virtual ~SlowTimeEffect();

	// ??
};

class ValueModifierEffect : public ActiveEffect
{
public:
	virtual ~ValueModifierEffect();

	// ??
};

class BoundItemEffect : public ActiveEffect
{
public:
	virtual ~BoundItemEffect();

	// ??
};

class CloakEffect : public ActiveEffect
{
public:
	virtual ~CloakEffect();

	// ??
};

class CommandEffect : public ActiveEffect
{
public:
	virtual ~CommandEffect();

	// ??
};

class ReanimateEffect : public CommandEffect
{
public:
	virtual ~ReanimateEffect();

	// ??
};

class CommandSummonedEffect : public ActiveEffect
{
public:
	virtual ~CommandSummonedEffect();

	// ??
};

class SummonCreatureEffect : public ActiveEffect
{
public:
	virtual ~SummonCreatureEffect();

	// ??
};

class CureEffect : public ActiveEffect
{
public:
	virtual ~CureEffect();

	// ??
};

class DetectLifeEffect : public ActiveEffect
{
public:
	virtual ~DetectLifeEffect();

	// ??
};

class StaggerEffect : public ActiveEffect
{
public:
	virtual ~StaggerEffect();

	// ??
};

class DisarmEffect : public StaggerEffect
{
public:
	virtual ~DisarmEffect();

	// ??
};

class DisguiseEffect : public ActiveEffect
{
public:
	virtual ~DisguiseEffect();

	// ??
};

class DispelEffect : public ActiveEffect
{
public:
	virtual ~DispelEffect();

	// ??
};

class EtherealizationEffect : public ActiveEffect
{
public:
	virtual ~EtherealizationEffect();

	// ??
};

class GuideEffect : public ActiveEffect
{
public:
	virtual ~GuideEffect();

	// ??
};

class LightEffect : public ActiveEffect
{
public:
	virtual ~LightEffect();

	// ??
};

class LockEffect : public ActiveEffect
{
public:
	virtual ~LockEffect();

	// ??
};

class OpenEffect : public ActiveEffect
{
public:
	virtual ~OpenEffect();

	// ??
};

class SoulTrapEffect : public ActiveEffect
{
public:
	virtual ~SoulTrapEffect();

	// ??
};

class TelekinesisEffect : public ActiveEffect
{
public:
	virtual ~TelekinesisEffect();

	// ??
};

class VampireLordEffect : public ActiveEffect
{
public:
	virtual ~VampireLordEffect();

	// ??
};

class WerewolfEffect : public ActiveEffect
{
public:
	virtual ~WerewolfEffect();

	// ??
};

class WerewolfFeedEffect : public ActiveEffect
{
public:
	virtual ~WerewolfFeedEffect();

	// ??
};

class SpawnHazardEffect : public ActiveEffect
{
public:
	virtual ~SpawnHazardEffect();

	// ??
};

class PeakValueModifierEffect : public ValueModifierEffect
{
public:
	virtual ~PeakValueModifierEffect();

	// ??
};

class DualValueModifierEffect : public ValueModifierEffect
{
public:
	virtual ~DualValueModifierEffect();

	// ??
};

class EnhanceWeaponEffect : public DualValueModifierEffect
{
public:
	virtual ~EnhanceWeaponEffect();

	// ??
};

class AccumulatingValueModifierEffect : public ValueModifierEffect
{
public:
	virtual ~AccumulatingValueModifierEffect();

	// ??
};

class InvisibilityEffect : public ValueModifierEffect
{
public:
	virtual ~InvisibilityEffect();

	// ??
};

class NightEyeEffect : public ValueModifierEffect
{
public:
	virtual ~NightEyeEffect();

	// ??
};

class ParalysisEffect : public ValueModifierEffect
{
public:
	virtual ~ParalysisEffect();

	// ??
};

class DarknessEffect : public ValueModifierEffect
{
public:
	virtual ~DarknessEffect();

	// ??
};

class AbsorbEffect : public ValueModifierEffect
{
public:
	virtual ~AbsorbEffect();

	// ??
};

class ValueAndConditionsEffect : public ValueModifierEffect
{
public:
	virtual ~ValueAndConditionsEffect();

	// ??
};

class TargetValueModifierEffect : public ValueModifierEffect
{
public:
	virtual ~TargetValueModifierEffect();

	// ??
};

class RallyEffect : public TargetValueModifierEffect
{
public:
	virtual ~RallyEffect();

	// ??
};

class DemoralizeEffect : public TargetValueModifierEffect
{
public:
	virtual ~DemoralizeEffect();

	// ??
};

class CalmEffect : public TargetValueModifierEffect
{
public:
	virtual ~CalmEffect();

	// ??
};

class FrenzyEffect : public TargetValueModifierEffect
{
public:
	virtual ~FrenzyEffect();

	// ??
};

class TurnUndeadEffect : public DemoralizeEffect
{
public:
	virtual ~TurnUndeadEffect();

	// ??
};

class BanishEffect : public DemoralizeEffect
{
public:
	virtual ~BanishEffect();

	// ??
};

class BGSEntryPointFunctionData
{
public:
	virtual ~BGSEntryPointFunctionData();
};


class BGSEntryPointFunctionDataTwoValue : public BGSEntryPointFunctionData
{
public:
	virtual ~BGSEntryPointFunctionDataTwoValue();

	enum {
		kNumValues = 2
	};

	float value[kNumValues];
};

class BGSEntryPointFunctionDataOneValue : public BGSEntryPointFunctionData
{
public:
	virtual ~BGSEntryPointFunctionDataOneValue();

	float value;
};

class BGSEntryPointFunctionDataText : public BGSEntryPointFunctionData
{
public:
	virtual ~BGSEntryPointFunctionDataText();

	StringCache::Ref text;
};

class BGSEntryPointFunctionDataSpellItem : public BGSEntryPointFunctionData
{
public:
	virtual ~BGSEntryPointFunctionDataSpellItem();

	SpellItem	* spellItem;
};

class BGSEntryPointFunctionDataLeveledList : public BGSEntryPointFunctionData
{
public:
	virtual ~BGSEntryPointFunctionDataLeveledList();

	TESLevItem	* leveledList;
};

class BGSEntryPointFunctionDataActivateChoice : public BGSEntryPointFunctionData
{
public:
	virtual ~BGSEntryPointFunctionDataActivateChoice();

	StringCache::Ref	label;
	BGSPerk				* perk;
	SpellItem			* appliedSpell;
	UInt32				flags;
	UInt32				unk14;
};

class BGSEntryPointFunctionDataBooleanGraphVariable : public BGSEntryPointFunctionData
{
public:
	virtual ~BGSEntryPointFunctionDataBooleanGraphVariable();

	StringCache::Ref	variable;
};

// 10
class BGSPerkEntry
{
public:
	virtual ~BGSPerkEntry();

	UInt8	rank;	//8
	UInt8	priority;	// 9
	UInt16	unk0A;
	UInt32	pad0C;
};
STATIC_ASSERT(sizeof(BGSPerkEntry) == 0x10);

class BGSQuestPerkEntry : public BGSPerkEntry
{
public:
	virtual ~BGSQuestPerkEntry();

	TESQuest	* quest;	// 10
	UInt8		stage;		// 18
	UInt8		pad[7];		// 19
};

class BGSAbilityPerkEntry : public BGSPerkEntry
{
public:
	virtual ~BGSAbilityPerkEntry();

	SpellItem	* spellItem;	// 10
};

class BGSEntryPointPerkEntry : public BGSPerkEntry
{
public:
	virtual ~BGSEntryPointPerkEntry(); // Has a number of virtuals, don't know what they are yet

	UInt32						unk10;
	UInt32						pad14;
	BGSEntryPointFunctionData	* data; // 18
	void						* unk20;
	BGSPerk						* perk;	// 28
};

class BGSPrimitive
{
public:
	BGSPrimitive();
	virtual ~BGSPrimitive();

	enum {
		kPrimitive_None = 0,
		kPrimitive_Box = 1,
		kPrimitive_Sphere = 2
	};

	UInt32	type;
	float	bounds_x; // Div 2 from CK
	float	bounds_y; // Div 2 from CK
	float	bounds_z; // Div 2 from CK
};

class BGSPrimitiveBox : public BGSPrimitive
{
public:
	BGSPrimitiveBox();
	virtual ~BGSPrimitiveBox();
};

class BGSPrimitiveLine : public BGSPrimitiveBox
{
public:
	BGSPrimitiveLine();
	virtual ~BGSPrimitiveLine();
};

class BGSPrimitivePlane : public BGSPrimitive
{
public:
	BGSPrimitivePlane();
	virtual ~BGSPrimitivePlane();
};

class BGSPrimitiveSphere : public BGSPrimitive
{
public:
	BGSPrimitiveSphere();
	virtual ~BGSPrimitiveSphere();
};
