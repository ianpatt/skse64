#pragma once

// everything deriving from BaseFormComponent but not TESForm goes here

#include "skse64/GameTypes.h"

class TESObjectSTAT;
class BGSSoundDescriptorForm;
class BGSSoundOutput;
class BGSSoundCategory;
class BGSKeyword;
class TESForm;
class SpellItem;
class TESShout;
class IngredientItem;
class TESRace;
class EnchantmentItem;
class TESFaction;
class BGSVoiceType;
class BGSEquipSlot;
class Actor;
class TESObjectARMO;
class TESObjectARMA;
class TESIdleForm;
class BGSPerk;
class ActorValueInfo;
class TESGlobal;
class TESRegion;
class BGSMusicType;
class TESWeather;
class TESPackage;
class BGSMusicTrackFormWrapper;
class BGSImpactDataSet;
class BGSMaterialType;
class NiNode;
class NiAVObject;

//// root

// 04
class BaseFormComponent
{
public:
	BaseFormComponent();
	virtual ~BaseFormComponent();

	virtual void	Init(void);
	virtual void	ReleaseRefs(void);
	virtual void	CopyFromBase(BaseFormComponent * rhs);

	//	void	** _vtbl;	// 00
};

//// members of other components

// 10 
class TESTexture : public BaseFormComponent
{
public:
	virtual UInt32			Unk_04(void);
	virtual void			GetNormalMapName(BSString * out);	// might have const char * retn type
	virtual const char *	GetSearchDir(void);

	BSFixedString	str;	// 08
};

// 10 
class TESIcon : public TESTexture
{

};

// 10 
class TESTexture1024 : public TESTexture
{

};

//// leaf nodes

// 10 
class BGSAttackDataForm : public BaseFormComponent
{
public:
	BSTSmartPointer	<void>	unk08;	// 08
};

// 10 
class BGSBipedObjectForm : public BaseFormComponent
{
public:
	// applicable to DefaultRace
	enum
	{
		kPart_Head = 1 << 0,
		kPart_Hair = 1 << 1,
		kPart_Body = 1 << 2,
		kPart_Hands = 1 << 3,
		kPart_Forearms = 1 << 4,
		kPart_Amulet = 1 << 5,
		kPart_Ring = 1 << 6,
		kPart_Feet = 1 << 7,
		kPart_Calves = 1 << 8,
		kPart_Shield = 1 << 9,
		kPart_Unnamed10 = 1 << 10,
		kPart_LongHair = 1 << 11,
		kPart_Circlet = 1 << 12,
		kPart_Ears = 1 << 13,
		kPart_Unnamed14 = 1 << 14,
		kPart_Unnamed15 = 1 << 15,
		kPart_Unnamed16 = 1 << 16,
		kPart_Unnamed17 = 1 << 17,
		kPart_Unnamed18 = 1 << 18,
		kPart_Unnamed19 = 1 << 19,
		kPart_Unnamed20 = 1 << 20,
		kPart_Unnamed21 = 1 << 21,
		kPart_Unnamed22 = 1 << 22,
		kPart_Unnamed23 = 1 << 23,
		kPart_Unnamed24 = 1 << 24,
		kPart_Unnamed25 = 1 << 25,
		kPart_Unnamed26 = 1 << 26,
		kPart_Unnamed27 = 1 << 27,
		kPart_Unnamed28 = 1 << 28,
		kPart_Unnamed29 = 1 << 29,
		kPart_Unnamed30 = 1 << 30,
		kPart_FX01 = 1 << 31,
	};

	enum
	{
		kWeight_Light = 0,
		kWeight_Heavy,
		kWeight_None,
	};

	struct Data
	{
		UInt32	parts;			// 00 - init'd to 0
		UInt32	weightClass;	// 04 - init'd to 2 (none)
	};

	Data	data;	// 08

	UInt32 GetSlotMask() const { return data.parts; }
	void SetSlotMask(UInt32 mask) { data.parts = mask; }
	UInt32 AddSlotToMask(UInt32 mask);
	UInt32 RemoveSlotFromMask(UInt32 mask);
};

// 18 
class BGSBlockBashData : public BaseFormComponent
{
public:
	BGSImpactDataSet *	impact;	// 08
	BGSMaterialType *	material;	// 10
};

// 10 
class BGSDestructibleObjectForm : public BaseFormComponent
{
public:
	// 10 or 10+
	struct Data
	{
		UInt32	health;			// 00
		UInt8	destCount;		// 04
		UInt8	vatsTargetable;	// 05
		UInt8	pad06[2];	// 06
		void	*unk08;		// 08
		
		// ...?
	};

	Data	* data;	// 08
};

// 10 
class BGSEquipType : public BaseFormComponent
{
public:
	virtual	BGSEquipSlot *	GetEquipSlot(void);
	virtual void			SetEquipSlot(BGSEquipSlot * type);

	BGSEquipSlot * equipSlot;	// 08
};

// 20 
class BGSIdleCollection : public BaseFormComponent
{
public:
	UInt8	unk08;		// 08
	UInt8	unk09;		// 09 - unk10 length
	UInt8	unk0A[6];	// 0A
	void	** unk10;	// 10 - copyable struct
	UInt32	unk18;		// 18
	UInt32	pad1C;		// 1C
};

// 18 
class BGSKeywordForm : public BaseFormComponent
{
public:
	virtual bool	HasKeyword(BGSKeyword* keyword);
	virtual UInt32	GetDefaultKeyword(void);

	BGSKeyword	** keywords;	// 08
	UInt32		numKeywords;	// 10
	UInt32		pad14;
};

// 10 
class BGSMenuDisplayObject : public BaseFormComponent
{
public:
	virtual UInt32	GetMenuDisplayObject(void);

	TESObjectSTAT	* worldStatic;	// 08 - copyable
};

// 18 
class BGSMessageIcon : public BaseFormComponent
{
public:
	TESIcon	icon;	// 08
};

// 28 
class BGSOverridePackCollection : public BaseFormComponent
{
public:
	UInt64	unk08;	// 08
	UInt64	unk10;	// 10
	UInt64	unk18;	// 18
	UInt64	unk20;	// 20
};

// 18 
class BGSPerkRankArray : public BaseFormComponent
{
public:
	// 10
	struct Data
	{
		BGSPerk	* perk;		// 00
		UInt8	rank;		// 08
		UInt8	pad05[7];	// 09
	};

	Data	* perkRanks;	// 08
	UInt32	numPerkRanks;	// 10
	UInt32	pad14;	// 14
};

class BGSPerkRanks : public tArray<BGSPerkRankArray::Data*>
{
public:
	MEMBER_FN_PREFIX(BGSPerkRanks);
	DEFINE_MEMBER_FN(VisitPerks, void, 0x00000000, void * perkRankVisitor); // BGSPerk::PerkRankVisitor
};

// 18 
class BGSPickupPutdownSounds : public BaseFormComponent
{
public:
	BGSSoundDescriptorForm*	pickUp;	// 08
	BGSSoundDescriptorForm*	putDown;	// 10
};

// 08 
class BGSPreloadable : public BaseFormComponent
{
public:
	virtual void	Unk_04(void);	// pure virtual
};

// 10 
class BGSSkinForm : public BaseFormComponent
{
public:
	TESObjectARMO	* skin;	// 04
};

// 28 
class TESAIForm : public BaseFormComponent
{
public:
	// 10 - might be ExtraDataList
	struct Data
	{
		void	*unk0;	// 0
		Data	* next;	// 8
	};

	UInt32	flags;		// 08
	UInt32	unk0C;		// 0C
	UInt16	unk10;		// 10
	UInt8	pad12[6];	// 12

	Data	unk18;		// 18
};

// 58 
class TESActorBaseData : public BaseFormComponent
{
public:
	virtual void	Unk_04(UInt32 arg);
	virtual bool	GetFlag20000000(void);
	virtual bool	GetFlag80000000(void);
	virtual bool	Unk_07(void);
	virtual UInt32	GetUnk20(void);

	enum {
		kFlag_PCLevelMult = 0x80
	};

	UInt32	flags;		// 08 - init'd to 0
	UInt16	unk0C;		// 0C - init'd to 0
	UInt16	unk0E;		// 0E - init'd to 0
	UInt16	level;		// 10 - init'd to 1	(CK Multiplier * 1000 if PCLevelMult is true)
	UInt16	minLevel;	// 12 - init'd to 0
	UInt16	maxLevel;	// 14 - init'd to 0
	UInt16	unk16;		// 16 - init'd to 0x64 (100)
	UInt16	unk18;		// 18 - init'd to iBaseDisposition (35)
	UInt16	unk1A;		// 1A - init'd to 0
	UInt16	unk1C;		// 1C - init'd to 0
	UInt16	unk1E;		// 1E - init'd to 0
	UInt64	unk20;		// 20
	BGSVoiceType*	voiceType;	// 28
	UInt64	unk30;	// 30
	UInt32	unk38;	// 38 - init'd to 0
	UInt32	pad3C;	// 3C

	struct FactionInfo
	{
		TESFaction	* faction;	// 00
		SInt8		rank;		// 08
		UInt8		pad[7];		// 09
	};

	tArray<FactionInfo>	factions;	// 40
};

STATIC_ASSERT(sizeof(TESActorBaseData) == 0x58);

// 10 
class TESAttackDamageForm : public BaseFormComponent
{
public:
	virtual UInt16	GetAttackDamage(void);

	UInt16	attackDamage;	// 08
	UInt8	pad06[2];		// 0A
	UInt32	pad0C;			// 0C
};

class BGSTextureSet;

// 28 
class TESModel : public BaseFormComponent
{
public:
	virtual const char *	GetModelName(void);
	virtual void			SetModelName(const char * name);
	virtual UInt32			Unk_06(void);

	BSFixedString	name;	// 08

	UInt64 unk10; // 10
	UInt64 unk18; // 18
	UInt32 unk20; // 20
};

// 38   
class TESModelTextureSwap : public TESModel
{
public:
	// 18
	struct SwapInfo
	{
		BGSTextureSet	* textureSet;	// 00
		UInt32			unk04;			// 08 - index
		BSFixedString	name;			// 10
	};

	SwapInfo		* swaps;	// 28
	UInt32			count;		// 30
};

// 28 
class TESModelRDT : public TESModel
{
public:
};

// F0 
class TESBipedModelForm : public BaseFormComponent
{
public:
	TESModelTextureSwap	textureSwap[2];	// 08
	TESIcon				icon[2];		// 78
	BGSMessageIcon		messageIcon[2];	// 98
	TESModelRDT			modelRDT;		// C8
};


// 18 
class TESContainer : public BaseFormComponent
{
public:
	struct Entry
	{
		struct Data
		{
			TESForm*	owner; // 00
			TESGlobal*	global; // 08
			float	condition; // 10
		};

		UInt32	count; // 00
		UInt32  unk04; // 04 - likely just pad
		TESForm* form; // 08
		Data	* data; // 10 - extra data
	};

	Entry	** entries;	// 8
	UInt32	numEntries;	// 10

	template <class Op>
	UInt32 CountIf(Op& op) const
	{
		UInt32 count = 0;
		for (UInt32 n = 0; n < numEntries; n++) {
			Entry* pEntry = entries[n];
			if (pEntry && op.Accept(pEntry))
				count++;
		}
		return count;
	}

	template <class Op>
	Entry* Find(Op& op) const
	{
		bool bFound = false;
		UInt32 n = 0;
		Entry* pEntry = NULL;
		for (UInt32 n = 0; n < numEntries && !bFound; n++) {
			pEntry = entries[n];
			if (pEntry) {
				bFound = op.Accept(pEntry);
			}
		}
		return (bFound && pEntry) ? pEntry : NULL;
	}

	template <class Op>
	void Visit(Op& op) const {
		bool bContinue = true;
		for (UInt32 n = 0; n < numEntries && bContinue; n++) {
			Entry* pEntry = entries[n];
			if (pEntry) {
				bContinue = op.Accept(pEntry);
			}
		}
	}

	UInt32 CountItem(TESForm* item) const;
};

// 18 
class TESDescription : public BaseFormComponent
{
public:
	UInt32	unk08;	// 08
	UInt32	unk0C;	// 0C - init'd to FFFFFFFF

	MEMBER_FN_PREFIX(TESDescription);
	DEFINE_MEMBER_FN(Get, void, 0x00190480, BSString * out, TESForm * parent, UInt32 fieldType);
};

// 18 
class TESEnchantableForm : public BaseFormComponent
{
public:
	virtual UInt16	Unk_04(void);		// return unk10

	EnchantmentItem*	enchantment;		// 08 - init'd to 0
	UInt16				unk10;			// 10 - init'd to 3
	UInt16				maxCharge;		// 12
};

// 10 
class TESFullName : public BaseFormComponent
{
public:
	virtual UInt32	Unk_04(void);
	virtual const char *	GetName(void);

	BSFixedString	name;	// 08
};

// 10 
class TESImageSpaceModifiableForm : public BaseFormComponent
{
public:
	UInt64	unk08;	// 08
};

// 28 
class TESLeveledList : public BaseFormComponent
{
public:
	struct Entry
	{
		TESForm * form;	// 00 - init'd to 0
		UInt16	count;	// 08 - init'd to 1
		UInt16	level;	// 0A - init'd to 1
		UInt32	pad0C;	// 0C
		UInt64	unk10;	// 10 - init'd to 0
	};

	enum
	{
		kFlagCalculateFromAllLevelsLTPCLevel =	1 << 0,
		kFlagCalculateForEachItemInCount	 =	1 << 1,
		kFlagUseAll							 =	1 << 2,
		kFlagSpecialLoot					 =	1 << 3
	};

	virtual bool	Unk_04(void);
	virtual bool	GetFlag02(void);
	virtual UInt32	Unk_06(void);
	virtual void	Unk_07(void);	// pure

	Entry	* entries;	// 08
	UInt8	chanceValue;		// 10
	UInt8	flags;		// 11
	UInt8	length;		// 12
	UInt8	unk0B;		// 13
	UInt32	pad14;		// 14
	void	* unk18;	// 18
	TESGlobal * chanceGlobal;		// 20
};

// 28 
class BGSBehaviorGraphModel : public TESModel
{
public:
};

// 28 
class BGSTextureModel : public TESModel
{
public:
};

// 28 
class TESModelPSA : public TESModel
{
public:
};

// 28 
class TESModelTri : public TESModel
{
public:
};

// 20 
class TESProduceForm : public BaseFormComponent
{
public:
	BGSSoundDescriptorForm	* harvestSound;	// 08
	TESForm					* produce;		// 10
	UInt32	un18;	// 18
	UInt32	pad1C;	// 1C
};

// 10 
class TESQualityForm : public BaseFormComponent
{
public:
	UInt32	quality;	// 08
	UInt32	pad0C;		// 0C
};

// 10 
class TESRaceForm : public BaseFormComponent
{
public:
	TESRace	* race;	// 08
};

class TESFaction;

// 20 
class TESReactionForm : public BaseFormComponent
{
public:
	// 10
	struct Data
	{
		TESFaction *faction;	// 00
		UInt32	modifier;	// 08
		UInt32	reaction;	// 0C
	};

	// 10
	struct Entry
	{
		Data	* data;	// 00
		Entry	* unk4;	// 08
	};

	Entry	entry;		// 08
	UInt8	unk18;		// 18 - init'd to B
	UInt8	pad19[7];	// 19
};

// 10 
class TESSpellList : public BaseFormComponent
{
public:
	UInt32 GetSpellCount() const;
	SpellItem *	GetNthSpell(UInt32 n) const;

	UInt32 GetShoutCount() const;
	TESShout *	GetNthShout(UInt32 n) const;

	struct Data
	{
		SpellItem**	spells;	// 00 - SpellItem**  (null terminated array of spells)
		void	* unk4; // 08
		TESShout**	shouts; // 10
		UInt32		numSpells; // 18
		UInt32		numUnk4; // 1C
		UInt32		numShouts; // 20
	};

	Data	* data;	// 08
};

// 10 
class TESValueForm : public BaseFormComponent
{
public:
	UInt32	value;	// 08
	UInt32	pad0C;	// 0C
};

// 10 
class TESWeightForm : public BaseFormComponent
{
public:
	float	weight;	// 08
};

//// the following are not form components
//// they live here until there's a better header file

// 58 
class BSMaterialObject
{
public:
	// 34
	struct Data
	{
		// 0C
		struct Data10
		{
			UInt32	unk0;	// 0 - init'd to 0
			UInt32	unk4;	// 4 - init'd to 0
			float	unk8;	// 8 - init'd to -1
		};

		float	unk00;	// 00 - init'd to 0.4
		float	unk04;	// 04 - init'd to 0.4
		float	unk08;	// 08 - init'd to 512
		float	unk0C;	// 0C - init'd to 512
		Data10	unk10;	// 10
		float	unk1C;	// 1C - init'd to 1
		UInt32	unk20;	// 20
		UInt32	unk24;	// 24
		UInt32	unk28;	// 28
		UInt32	unk2C;	// 2C
		UInt32	unk30;	// 30		
	};

	BSMaterialObject();
	virtual ~BSMaterialObject();

	virtual void	Unk_01(void);

//	void	** _vtbl;	// 00
	
	Data		unk08;	// 08
	UnkArray	unk40;	// 40
};
STATIC_ASSERT(sizeof(BSMaterialObject) == 0x58);

// 08 
class TESChildCell
{
public:
	virtual ~TESChildCell();

	virtual	UInt32	Unk_01(void);

	//	void	** _vtbl;
};

// 08 
class ActorValueOwner
{
public:
	virtual ~ActorValueOwner();

	// Argument is the ActorValue ID
	virtual float	GetCurrent(UInt32 a_akValue);
	virtual float	GetMaximum(UInt32 a_akValue);
	virtual float	GetBase(UInt32 a_akValue);
	virtual void	SetBase(UInt32 a_akValue, float a_value);
	virtual void	ModBase(UInt32 a_akValue, float a_value);
	virtual void	ModCurrent(UInt32 a_arg1, UInt32 a_akValue, float a_value);
	virtual void	SetCurrent(UInt32 a_akValue, float a_value);
	virtual bool	UsePCAVMult() const;

	//	void	** _vtbl;	// 00
};
STATIC_ASSERT(sizeof(ActorValueOwner) == 0x8);

// 10 
class BSIMusicTrack
{
public:
	BSIMusicTrack();
	virtual ~BSIMusicTrack();

	virtual void	Unk_01(void);
	virtual void	Unk_02(void);
	virtual void	Unk_03(void);
	virtual void	Unk_04(void);
	virtual void	Unk_05(void);
	virtual void	Unk_06(void);
	virtual bool	Unk_07(void);
	virtual UInt32	Unk_08(void);	// return unk04
	virtual void	Unk_09(UInt32 arg);
	virtual void	Unk_0A(void);

	//	void	** _vtbl;	// 00
	UInt32	unk08;		// 08
};

// 50 
class BSIMusicType
{
public:
	virtual void	Unk_00(void);
	virtual void	Unk_01(void);
	virtual void	Unk_02(void);
	virtual void	Unk_03(void);
	virtual void	Unk_04(void);
	virtual void	Unk_05(void);
	virtual void	Unk_06(void);
	virtual void	Unk_07(void);

	enum
	{
		kFlagUnk1				= (1 << 0),
		kFlagUnk2				= (1 << 1),
		kFlagCycleTracks		= (1 << 2),
		kFlagDuclsCurrentTrack	= (1 << 5)
	};

//	void		** _vtbl;			// 00
	UInt32		flags;				// 08
	UInt8		priority;			// 0C - init'd to 0x32
	UInt8		unk09;				// 0D
	UInt16		ducking;			// 0E (db = ducking/100.0)
	float		fadeDuration;		// 10
	UInt32	unk14;		// 14
	UnkArray	unk18;	// 18
	tArray<BGSMusicTrackFormWrapper*>	tracks;		// 30
	UInt32	unk48;		// 48
};

// 08 
class BSIReverbType
{
public:
	virtual void	Unk_00(void);
	virtual void	Unk_01(void);
	virtual void	Unk_02(void);
	virtual void	Unk_03(void);
	virtual void	Unk_04(void);
	virtual void	Unk_05(void);
	virtual void	Unk_06(void);
	virtual void	Unk_07(void);
	virtual void	Unk_08(void);
	virtual void	Unk_09(void);
	virtual void	Unk_0A(void);

	//	void	** _vtbl;	// 00
};

// 08 
class BSISoundCategory
{
public:
	virtual ~BSISoundCategory();

	virtual void	Unk_01(void);
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

	//	void	** _vtbl;	// 00
};

// 08 
class BSISoundDescriptor
{
public:
	virtual ~BSISoundDescriptor();

	virtual void	Unk_01(void) = 0;
	virtual void	Unk_02(void) = 0;

//	void	** _vtbl;	// 00
};

// 08 
class BGSSoundDescriptor : public BSISoundDescriptor
{
public:
	virtual void	Unk_01(void);
	virtual void	Unk_02(void);

	//	void	** _vtbl;	// 00
};

// 08 
class BSISoundOutputModel
{
public:
	virtual ~BSISoundOutputModel();

	virtual void	Unk_01(void);
	virtual void	Unk_02(void);
	virtual void	Unk_03(void);
	virtual void	Unk_04(void);
	virtual void	Unk_05(void);
	virtual void	Unk_06(void);
	virtual void	Unk_07(void);
	virtual void	Unk_08(void);
	virtual void	Unk_09(void);

	//	void	** _vtbl;	// 00
};

class BSIPlaybackCharacteristics
{
public:
	virtual ~BSIPlaybackCharacteristics();

	virtual UInt8	GetFrequencyShift(void) = 0;
	virtual UInt8	GetFrequencyVariance(void) = 0;
	virtual UInt8	GetPriority(void) = 0;
	virtual UInt16	GetDBAttenuation(void) = 0;
	virtual UInt8	GetDBVariance(void) = 0;
};

// 28
class SoundPlaybackCharacteristics : public BSIPlaybackCharacteristics
{
public:
	virtual UInt8	GetFrequencyShift(void);
	virtual UInt8	GetFrequencyVariance(void);
	virtual UInt8	GetPriority(void);
	virtual UInt16	GetDBAttenuation(void);
	virtual UInt8	GetDBVariance(void);

	UInt8	frequencyShift;		// 08
	UInt8	frequencyVariance;	// 09
	UInt8	priority;	// 0A
	UInt8	dbVariance;	// 0B
	UInt16	dbAttenuation;	// 0C - CK Value * 100
	UInt16	pad0E;		// 0E
	UInt64	unk10;		// 10
	BGSSoundOutput	* soundOutput;	// 18
	UInt32	unk20;	// 20
	UInt32	unk24;	// 24
};

// 58
class BGSStandardSoundDef : public BGSSoundDescriptor
{
public:
	virtual ~BGSStandardSoundDef();

	BGSSoundCategory	* soundCategory; // 08
	UInt64				unk10;	// 10
	void*				unk18;	// 18
	UInt32				unk20;	// 20
	UInt32				unk24;	// 24
	UInt32				unk28;	// 28	
	UInt32				unk2C;	// 2C - probably pad
	SoundPlaybackCharacteristics soundCharacteristics; // 30
};
STATIC_ASSERT(sizeof(BGSStandardSoundDef) == 0x58);

//STATIC_ASSERT(sizeof(BSTEventSource<void*>) == 0x30);

// 08 
class BGSOpenCloseForm
{
public:
	virtual ~BGSOpenCloseForm();

	//	void	** _vtbl;	// 00
};

class ActiveEffect;
class Character;
// 18 
class MagicTarget
{
public:
	virtual ~MagicTarget();

	virtual UInt8					Unk_01(int);
	virtual Character				* Unk_02(void);
	virtual UInt8					Unk_03(void);
	virtual bool					Unk_04(void);
	virtual int						Unk_05(int);
	virtual bool					Unk_06(void); // pure   030
	virtual tList<ActiveEffect>		* GetActiveEffects(void); // pure     038
	virtual int						Unk_08(int);
	virtual void					Unk_09(int);
	virtual double					Unk_0A(int, int, int);
	virtual UInt8					Unk_0B(int, int, int);

//	void	** _vtbl;	// 00
	UInt64 unk04;		// 08
	UInt64 unk08;		// 10
};
STATIC_ASSERT(sizeof(MagicTarget) == 0x18);

// 38
class Condition
{
public:
	enum ComparisonFlags {
		kComparisonFlag_And = 0x00,
		kComparisonFlag_Or = 0x01,
		kComparisonFlag_Equal = 0x00,
		kComparisonFlag_UseAliases = 0x02,
		kComparisonFlag_Global = 0x04,
		kComparisonFlag_UsePackData = 0x08,
		kComparisonFlag_SwapTarget = 0x10,
		kComparisonFlag_NotEqual = 0x20,
		kComparisonFlag_Greater = 0x40,
		kComparisonFlag_GreaterEqual = 0x60,
		kComparisonFlag_Less = 0x80,
		kComparisonFlag_LessEqual = 0xA0
	};
	enum ReferenceTypes {
		kReferenceType_None = 0,
		kReferenceType_Target,
		kReferenceType_Reference,
		kReferenceType_CombatTarget,
		kReferenceType_LinkedRef,
		kReferenceType_Alias,
		kReferenceType_PackageData,
		kReferenceType_EventData
	};
	Condition	* next;					// 00
	UInt32		compareValue;			// 04-08
	UInt32		unk0C;					// 08-0C
	UInt32		unk10;					// 10 - New in SE?
	UInt32		unk14;					// 0C-14 - FFFFFFFF
	UInt16		functionId;				// 10-18
	UInt8		unk1A;					// 12-1A
	UInt8		unk1B;					// 13-1B
	UInt32		pad1C;					// 1C
	UInt64		param1;					// 14-20
	UInt64		param2;					// 18-28
	UInt8		comparisonType;			// 1C-30
	UInt8		referenceType;			// 1D-31
	UInt8		pad32[6];				// 32
};
STATIC_ASSERT(sizeof(Condition) == 0x38);

class TintMask
{
public:
	TintMask()
	{
		alpha = 0.0;
		tintType = 0;
		texture = NULL;
	};
	~TintMask() { };

#ifdef PAPYRUS_CUSTOM_CLASS
	enum { kTypeID = 300 };
#endif

	enum {
		kMaskType_Frekles = 0,
		kMaskType_Lips,
		kMaskType_Cheeks,
		kMaskType_Eyeliner,
		kMaskType_UpperEyeSocket,
		kMaskType_LowerEyeSocket,
		kMaskType_SkinTone,
		kMaskType_WarPaint,
		kMaskType_FrownLines,
		kMaskType_LowerCheeks,
		kMaskType_Nose,
		kMaskType_Chin,
		kMaskType_Neck,
		kMaskType_Forehead,
		kMaskType_Dirt
	};
	TESTexture	* texture;

	union {
		struct Color {
			UInt8   red, green, blue, alpha; // The alpha isn't actually used here so its usually zero
		} color;
		UInt32 abgr;
	};

	float		alpha;
	UInt32		tintType;

	UInt32 ToARGB();
};

// 2778
class Biped : public BSIntrusiveRefCounted
{
public:
	NiNode	* root;	// 08

	struct Data
	{
		TESForm*				armor;			// 00 - Can be ARMO or ARMA
		TESForm*				addon;			// 08 - Usually always ARMA
		TESModelTextureSwap*	model;			// 10
		BGSTextureSet*			textureSet;		// 18
		NiAVObject*				object;			// 20
		UInt64					unk28[(0x78 - 0x28) >> 3];
	};
	Data	unk10[42];		// 10
	Data	unk13C0[42];	// 13C0
	UInt32	handle;			// 2770
	UInt32	unk2774;		// 2774

	DEFINE_MEMBER_FN_0(UpdateWeightData, void, 0x001C61A0);
	DEFINE_MEMBER_FN_0(DeleteThis, void, 0x001C60A0);
};
STATIC_ASSERT(offsetof(Biped, unk10) == 0x10);
STATIC_ASSERT(offsetof(Biped, unk13C0) == 0x13C0);
STATIC_ASSERT(sizeof(Biped) == 0x2778);

// ??
class BipedModel
{
public:
	enum {
		kWeightModel_Small = 0,
		kWeightModel_Large = 1
	};
	Biped * bipedData;
};

class BSFixedStringCI;

template<class A>
class DoNothingUnhandledPolicy
{
public:
	virtual ~DoNothingUnhandledPolicy();
};

// 2C
#if 0
// Not done for SE, currently nothing references it
template<class A, class B, class C, class D>
class BSResponse : public BSIntrusiveRefCounted, public DoNothingUnhandledPolicy<C>
{
public:
	UInt32	unk08;	// 08
	UInt32	unk0C;	// 0C
	UInt32	unk10;	// 10 - NiNode?
	UInt32	unk14;	// 14 - NiNode?
	UInt32	unk18;	// 18 - NiNode?
	UInt32	unk1C;	// 1C
	UInt32	unk20;	// 20 - NiNode?
	UInt32	unk24;	// 24
	UInt32	unk28;	// 28
};
//STATIC_ASSERT(sizeof(BSResponse<BSFixedStringCI, Actor, BSFixedStringCI, DoNothingUnhandledPolicy<BSFixedStringCI>>) == 0x2C);
#endif

// 338 
// Ctor 0x673900 (SE 1.4), 0x72E600 (Skyrim32)
class MiddleProcess
{
public:
	// 10
	struct EffectListNode
	{
		ActiveEffect	* effect;	// 0
		EffectListNode	* next;		// 8
	};

	// 30
	// This is used also in ActorProcessManager
	struct Data58
	{
		UInt64		unk00;			// 00 - New in SE?
		TESPackage * package;		// 08
		UInt64		unk10;			// 10
		UInt32		unk18;			// 18
		UInt32		unk1C;			// 1C - init'd to FFFFFFFF
		float		unk20;			// 20 - init'd to -1
		UInt32		unk24;			// 24
		UInt16		unk28;			// 28
		UInt8		unk2A;			// 2A
		UInt8		unk2B;			// 2B
		UInt32		pad2C;			// 2C
	};	

	UnkArray	unk000;		// 000
	UnkArray	unk018;		// 018
	UnkArray	unk030;		// 030
	UInt64		unk048;		// 048
	UInt8		unk050;		// 050
	UInt8		unk051[7];	// 051	

	Data58		unk058;		// 058
	UnkArray	unk088;		// 088

	UInt64	unk0A0;			// 0A0
	UInt64	unk0A8;			// 0A8
	float	unk0B0;			// 0B0
	float	unk0B4;			// 0B4
	float	unk0B8;			// 0B8
	float	unk0BC;			// 0BC
	float	unk0C0;			// 0C0
	float	unk0C4;			// 0C4
	float	unk0C8;			// 0C8
	float	unk0CC;			// 0CC
	float	unk0D0;			// 0D0
	float	unk0D4;			// 0D4
	float	unk0D8;			// 0D8
	float	unk0DC;			// 0DC
	float	unk0E0;			// 0E0
	float	unk0E4;			// 0E4
	float	unk0E8;			// 0E8
	UInt32	unk0EC;			// 0EC
	UInt32	unk0F0;			// 0F0
	UInt32	pad0F4;			// 0F4	
	UInt64	unk0F8;			// 0F8
	UnkArray	unk100;		// 100
	UInt64	unk118;			// 118
	UInt64	unk120;			// 120
	UInt64	unk128;			// 128
	UInt64	unk130;			// 130
	UInt64	unk138;			// 138
	UInt64	unk140;			// 140
	UInt64	unk148;			// 148
	UInt64	unk150;			// 150
	void *	unk158;			// 158 - NiNode?
	void *	unk160;			// 160 - NiNode?
	UInt64	unk168;			// 168
	void *	unk170;			// 170 - BSFaceGenNiNode?
	void *	unk178;			// 178
	UInt64	unk180;			// 180
	UInt64	unk188;			// 188
	UInt64	unk190;			// 190
	UInt64	unk198;			// 198
	EffectListNode *	effectList;	// 1A0
	void * unk1A8;			// 1A8
	void * unk1B0;			// 1B0
	UnkArray	unk1B8;		// 1B8
	UnkArray	unk1D0;		// 1D0
	UInt64	unk1E8;			// 1E8
	UInt32	unk1F0;			// 1F0
	UInt32	unk1F4;			// 1F4
	UInt16	unk1F8;			// 1F8 - init'd to FFFF
	UInt16	unk1FA;			// 1FA - init'd to FFFF	
	UInt32	unk1FC;			// 1FC
	UInt32	unk200;			// 200
	UInt32	unk204;			// 204 - init'd to FF7FFFFF
	UInt32	furnitureHandle;	// 208
	UInt32	pad20C;			// 20C
	UInt64	unk210;			// 210
	UInt32	unk218;			// 218
	UInt32	pad21C;			// 21C
	UInt64	unk220;			// 220
	TESIdleForm	* currentIdle;	// 228
	UInt64	unk230;			// 230
	UInt64	unk238;			// 238
	UInt64	unk240;			// 240
	UInt64	unk248;			// 248
	void *	unk250;			// 250 - hkCharacterProxy?
	void *	unk258;			// 258
	UInt64	unk164;			// 260
	UInt64	unk268;			// 268
	UInt64	unk270;			// 270
	UInt64	unk278;			// 278
	UInt64	unk280;			// 280
	void *	unk288;			// 288
	UInt32	unk290;			// 290
	UInt32	unk294;			// 294
	float	unk298;			// 298 - init'd to -1
	UInt32	unk29C;			// 29C
	UInt32	unk2A0;			// 2A0
	UInt32	unk2A4;			// 2A4
	UInt32	unk2A8;			// 2A8
	UInt32	unk2AC;			// 2AC
	UInt32	unk2B0;			// 2B0
	UInt32	unk2B4;			// 2B4
	UInt32	unk2B8;			// 2B8
	float	unk2BC;			// 2BC - init'd to -1
	UInt32	unk2C0;			// 2C0
	UInt32	unk2C4;			// 2C4
	float	actorAlpha;		// 2C8 - init'd to 1
	
	UInt8	todo2CC[0x338 - 0x2CC]; // TODO
};
STATIC_ASSERT(offsetof(MiddleProcess, effectList) == 0x1A0);
STATIC_ASSERT(offsetof(MiddleProcess, currentIdle) == 0x228);
STATIC_ASSERT(offsetof(MiddleProcess, furnitureHandle) == 0x208);
STATIC_ASSERT(offsetof(MiddleProcess, actorAlpha) == 0x2C8);
STATIC_ASSERT(sizeof(MiddleProcess) == 0x338);

// 140 
class ActorProcessManager
{
public:
	enum {
		kEquippedHand_Left = 0,
		kEquippedHand_Right = 1
	};
	enum {
		kFlags_None = 0,
		kFlags_Unk01 = 1,
		kFlags_Unk02 = 2,
		kFlags_Unk03 = 4,
		kFlags_DrawHead = 8,
		kFlags_Mobile = 16,
		kFlags_Reset = 32
	};
	UInt64	unk00;						// 00
	MiddleProcess	* middleProcess;	// 08
	void	* unk10;					// 10

	
	MiddleProcess::Data58	unk18;		// 18
	float		unk48;					// 48 - init'd to -1
	UInt32		unk4C;					// 4C
	UInt64		unk50;					// 50
	UInt32		unk58;					// 58
	UInt32		pad5C;					// 5C
	UInt64		unk60;					// 60
	UInt64		unk68;					// 68
	UInt64		unk70;					// 70
	UInt64		unk78;					// 78
	UInt64		unk80;					// 80
	UInt64		unk88;					// 88
	UInt32		unk90;					// 90
	float		timeOfDeath;			// 94 - GetTimeDead = (GameDaysPassed*24) - timeOfDeath
	float		unk98;					// 98 - related with above
	UInt32		pad9C;					// 9C	
	UnkFormArray	forms;				// A0  seen containing [left hand, equip slot, right hand, equip slot, right hand, equip slot)
	UInt64		unkB8[7];				// B8 2nd slot points back to this adress
	TESForm	* equippedObject[2];		// F0
	UInt8	unk100[0x136 - 0x100];		// 100
	UInt8	unk136;						// 136
	SInt8	unk137;						// 137 - init'd to 3
	UInt8	unk138;						// 138
	UInt8	unk139;						// 139
	UInt8	unk13A;						// 13A
	UInt8	unk13B;						// 13B
	UInt32	pad13C;						// 13C

	MEMBER_FN_PREFIX(ActorProcessManager);
	DEFINE_MEMBER_FN(SetEquipFlag, void, 0x0067E3B0, UInt8 flags);
	DEFINE_MEMBER_FN(UpdateEquipment, void, 0x00650DF0, Actor * actor);
	// FBB0A4AE04B0C1C63470C26B004079D7D9B20D0B+8B
	DEFINE_MEMBER_FN(SetDataFlag, void, 0x0065C880, float flag); // Sets a number on the 0x10 object. SE: The parameter flag was actually a float (xmm1)

	void UpdateEquipment_Hooked(Actor * actor);
};

STATIC_ASSERT(offsetof(ActorProcessManager, equippedObject) == 0xF0);
STATIC_ASSERT(sizeof(ActorProcessManager) == 0x140);

// 68
class BGSSkillPerkTreeNode
{
public:
	virtual ~BGSSkillPerkTreeNode();

	UInt32	index;		// 08	- init'd to ctor argument
	UInt32	pad0C;		// 0C
	tArray<BGSSkillPerkTreeNode*> connections;	// 10
	tArray<BGSSkillPerkTreeNode*> connectees;	// 28
	BGSPerk	* perk;		// 40
	UInt32	unk48;		// 48
	UInt32	gridX;		// 4C
	UInt32	gridY;		// 50
	UInt32	pad54;		// 54
	ActorValueInfo	* skill;	// 58
	float	horizontalPos;		// 60
	float	verticalPos;		// 64

	class PerkVisitor
	{
	public:
		virtual bool Accept(BGSPerk * node) = 0;
	};

	bool VisitPerks(PerkVisitor & visitor)
	{
		if(visitor.Accept(perk))
			return true;

		for(UInt32 i = 0; i < connections.count; i++)
		{
			BGSSkillPerkTreeNode* connector = NULL;
			connections.GetNthItem(i, connector);

			if(connector->VisitPerks(visitor))
				return true;
		}

		return false;
	}
};

class PlayerSkills
{
public:

	enum {
		kAdvanceableSkillOffset = 6,
		kNumAdvanceableSkills = 18
	};

	struct StatData {
		struct LevelData {
			float level;		// 00
			float points;		// 04
			float pointsMax;	// 08
		};

		float		levelPoints;							// 00
		float		levelPointsMax;							// 04
		LevelData	levelData[kNumAdvanceableSkills];		// 08
		UInt32		legendaryLevel[kNumAdvanceableSkills];	// E0
	};

	StatData * data;

	static SInt32 ResolveAdvanceableSkillId(SInt32 actorValue);

	float GetSkillPoints(BSFixedString actorValue);
	void SetSkillPoints(BSFixedString actorValue, float points);

	SInt32 GetSkillLegendaryLevel(BSFixedString actorValue);
	void SetSkillLegendaryLevel(BSFixedString actorValue, UInt32 level);

	MEMBER_FN_PREFIX(PlayerSkills);
	DEFINE_MEMBER_FN(GetSkillData, UInt32, 0x006E6130, UInt32 actorValue, float * level, float * points, float * pointsMax, UInt32 * unk6);
	DEFINE_MEMBER_FN(IncrementLegendary, UInt32, 0x006E6620, UInt32 actorValue);
	DEFINE_MEMBER_FN(SetLevel, void, 0x006E65C0, UInt32 level);
};

// 10
class TESRegionData
{
public:
	virtual ~TESRegionData();

	virtual void Unk_01(void);
	virtual void Unk_02(void);
	virtual SInt32 GetType(void); // pure
	virtual void Unk_04(void); // pure - Init?
	virtual void Unk_05(void); // pure
	virtual void Unk_06(void); // pure
	virtual void Unk_07(void); // pure

	UInt8	unk08;	// 08
	UInt8	unk09;	// 09
	UInt8	unk0A;	// 0A
	UInt8	pad0B[5];	// 0B
};

class TESRegionObjectBase
{
public:
	virtual ~TESRegionObjectBase();

	virtual void Unk_01(void); // pure
	virtual void Unk_02(void); // pure
};

// 10
class TESRegionGrassObject : public TESRegionObjectBase
{
public:
	virtual ~TESRegionGrassObject();

	virtual void Unk_03(void);
	virtual void Unk_04(void);
	virtual void Unk_05(void);
	virtual void Unk_06(void);

	UInt32 unk08;		// 08
	UInt32 unk0C;		// 0C
};

// 20
class TESRegionGrassObjectList : public tList<TESRegionGrassObject*>
{
public:
	virtual ~TESRegionGrassObjectList();

	UInt32	unk18;	// 18
	UInt32	unk1C;	// 1C
};

// 20
class TESRegionList : public tList<TESRegion*>
{
public:
	virtual ~TESRegionList();

	UInt32	unk18;	// 18 - maybe a single byte and rest pad
	UInt32	pad1C;	// 1C
};

// 18
class TESRegionDataGrass : public TESRegionData
{
public:
	virtual ~TESRegionDataGrass();

	TESRegionGrassObjectList * grassObjectList;	// 10
};

// 30
class TESRegionDataSound : public TESRegionData
{
public:
	virtual ~TESRegionDataSound();

	BGSMusicType	* musicType;	// 10

	struct SoundData
	{
		BGSSoundDescriptorForm	* soundDescriptor;	// 00
		UInt32					flags;				// 08
		float					chance;				// 0C
	};

	tArray<SoundData*>	soundData;	// 18
};

// 18
class TESRegionDataLandscape : public TESRegionData
{
public:
	virtual ~TESRegionDataLandscape();

	UInt64	unk10;	// 10
};

// 18
class TESRegionDataMap : public TESRegionData
{
public:
	virtual ~TESRegionDataMap();

	UInt64	unk10;	// 10
};

// 20
class TESRegionDataWeather : public TESRegionData
{
public:
	virtual ~TESRegionDataWeather();

	struct WeatherData
	{
		TESWeather	* weather;		// 00
		UInt32		chance;			// 08
		TESGlobal	* globalChance;	// 10
	};

	tList<WeatherData*> weatherData;	// 10
};
