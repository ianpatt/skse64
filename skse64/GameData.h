#pragma once

#include "skse64_common/Utilities.h"
#include "skse64/GameTypes.h"
#include "skse64/GameForms.h"
#include "skse64/GameObjects.h"
#include "skse64/GameReferences.h"

class TESObjectWEAP;
class TESNPC;
class AlchemyItem;
class SpellItem;
class ScrollItem;
class IngredientItem;
class EffectSetting;
class TESObjectARMO;
class TESObjectARMA;
class TESObjectCONT;
class TESObjectCELL;
class TESRegionList;
class BGSAddonNode;
class BGSKeyword;
class BGSAction;
class BGSHeadPart;
class TESRace;
class EnchantmentItem;
class TESObjectACTI;
class TESFurniture;
class BGSEncounterZone;
class BGSLocation;
class BGSColorForm;
class BSFile;
class BSFaceGenModelMap;
class BSFaceGenModel;
class NiTexture;
class Setting;

class NiAVObject;
class NiColorA;

extern RelocAddr <UInt32*> g_gameTime;

struct FormRecordData
{
	UInt8		typeID;		// corresponds to kFormType_XXX
	UInt32		typeCode;	// i.e. 'GMST', 'FACT'
	UInt32		unk08;		// only seen zero
};

struct ChunkHeader
{
	UInt32	type : 4;	// i.e. 'XGRD', 'DATA'
	UInt16	size : 2;
};

// 4C8
struct ModInfo		// referred to by game as TESFile
{
	ModInfo();
	~ModInfo();

	// 18 info about currently loading form
	struct FormInfo
	{
		UInt32		recordType;			// 00 i.e. 'FACT', 'GMST'
		UInt32		unk04;				// 04 looks like size of entire record
		UInt32		formFlags;			// 08 copied to TESForm->flags
		UInt32		formID;				// 0C 
		UInt32		unk10;				// 10
		UInt16		unk14;				// 14 always initialized to 0F on SaveForm. 
		UInt16		unk16;
	};

	enum FileFlags
	{
		kFileFlags_Light = (1 << 9)
	};

	UInt32								unk000;				// 000
	UInt8								unk004[0xC];		// 004
	UInt32 /*NiTPointerMap<TESFile*>*/	* pointerMap;		// 010   CHECK
	UInt64								unk018[2];			// 018
	UInt8								unk028;				// 028
	UInt8								unk029;				// 029
	UInt8								pad02A[6];			// 029
	UInt64								unk030;				// 030
	BSFile*								unkFile;			// 038
	UInt64								unk040[3];			// 040
	char								name[0x104];		// 058
	char								filepath[0x104];	// 15C
	void*								unk260;				// 260 
	UInt32								unk268;				// 268 - init'd to ctor arg(0x800) or 0x10000 if arg is 0
	UInt32								unk26C;				// 26C
	UInt32								unk270;				// 270
	UInt32								unk274;				// 274 - pad?
	UInt64								unk278;				// 278
	UInt32								unk280;				// 280
	FormInfo							formInfo;			// 284 
	ChunkHeader							subRecord;			// 29C
	UInt32								unk2A4;				// 2A4
	UInt32								fileOffset;			// 2A8
	UInt32								dataOffset;			// 2AC index into dataBuf
	UInt32								subrecordBytesRead;	// 2B0 generates error on Read if != expected length
	UInt8								unk2B4[0x2D8 - 0x2B4];	// 2B4
	UInt64								unk2D8;				// 2D8
	UInt64								unk2E0;				// 2E0
	UInt8								unk2E8;				// 2E8
	UInt8								bIsBigEndian;		// 2E9
	UInt8								unk2EA;				// 2EA
	UInt8								pad2EB;				// 2EB
	WIN32_FIND_DATA						fileData;			// 2EC
	float								unk42C;				// 42C init'd to 0.94
	UInt32								unk430;				// 430
	UInt32								flags;				// 434 init'd to 0x00000800. 4000 and 40000 do stuff
	UInt32								fileFlags;			// 438
	UInt32								unk43C;				// 43C
	void*								unk440;				// 440
	void*								unk448;				// 448
	void*								unk450;				// 450
	void*								unk458;				// 458
	UInt32								numRefMods;			// 460 related to modindex; see 4472D0
															// formIDs in mod are as saved in GECK, must fix up at runtime
	UInt32								pad464;
	ModInfo								** refModInfo;		// 468 used to look up modInfo based on fixed mod index, double-check 
	UInt32								unk470;				// 470
	UInt32								unk474;				// 474
	UInt8								modIndex;			// 478 init to 0xFF
	UInt8								pad479;				// 479
	UInt16								lightIndex;			// 47A
	UInt8								pad47C[4];
	BSString							author;				// 480
	BSString							description;		// 490
	void								* dataBuf;			// 4A0 
	UInt32								unk4A8;				// 4A8 looks like size of entire record
	UInt32								pad4AC;				// 4AC
	void								* unk4B0;			// 4B0
	UInt32								unk4B8;				// 4B8
	UInt32								pad4BC;				// 4BC
	void								* unk4C0;			// 4C0

	// Checks if a particular formID is part of the mod
	bool IsFormInMod(UInt32 formID) const
	{
		if (!IsLight() && (formID >> 24) == modIndex)
			return true;
		if (IsLight() && (formID >> 24) == 0xFE && ((formID & 0x00FFF000) >> 12) == lightIndex)
			return true;
		return false;
	}

	// Returns either a modIndex or a modIndex|lightIndex pair
	UInt32 GetPartialIndex() const
	{
		return !IsLight() ? modIndex : (0xFE000 | lightIndex);
	}

	// Converts the lower bits of a FormID to a full FormID depending on plugin type
	UInt32 GetFormID(UInt32 formLower) const
	{
		return !IsLight() ? UInt32(modIndex) << 24 | (formLower & 0xFFFFFF) : 0xFE000000 | (UInt32(lightIndex) << 12) | (formLower & 0xFFF);
	}

	bool IsActive() const { return modIndex != 0xFF; }
	bool IsLight() const { return (fileFlags & kFileFlags_Light) == kFileFlags_Light; }
};

STATIC_ASSERT(offsetof(ModInfo, formInfo) == 0x284);
STATIC_ASSERT(offsetof(ModInfo, subRecord) == 0x29C);
STATIC_ASSERT(offsetof(ModInfo, fileOffset) == 0x2A8);
STATIC_ASSERT(offsetof(ModInfo, fileData) == 0x2EC);
STATIC_ASSERT(offsetof(ModInfo, numRefMods) == 0x460);
STATIC_ASSERT(offsetof(ModInfo, author) == 0x480);
STATIC_ASSERT(sizeof(ModInfo) == 0x4C8);

// 40 - updated in 1.5.3
struct ModList
{
	tList<ModInfo>		modInfoList;		// 00 - DataHandler D60
	tArray<ModInfo *>	loadedMods;			// 10 - DataHandler D70
	tArray<ModInfo *>	loadedCCMods;		// 28 - DataHandler D88 - just assuming this is for CC
};
STATIC_ASSERT(sizeof(ModList) == 0x40);

// DC0
class DataHandler
{
public:
	UInt64								unk00;
	UInt64								unk08;
	UnkFormArray						arrNONE;	// Form Type 0
	UnkFormArray						arrTES4;	// Form Type 1
	UnkFormArray						arrGRUP;	// Form Type 2
	UnkFormArray						arrGMST;	// Form Type 3
	tArray<BGSKeyword*>					keywords;	// Form Type 4
	UnkFormArray						arrLCRT;	// Form Type 5
	UnkFormArray						arrAACT;	// Form Type 6
	UnkFormArray						arrTXST;	// Form Type 7
	UnkFormArray						arrMICN;	// Form Type 8
	UnkFormArray						arrGLOB;	// Form Type 9
	UnkFormArray						arrCLAS;	// Form Type 10
	UnkFormArray						arrFACT;	// Form Type 11
	tArray<BGSHeadPart*>				headParts;	// Form Type 12
	UnkFormArray						arrEYES;	// Form Type 13
	tArray<TESRace*>					races;	// Form Type 14
	UnkFormArray						arrSOUN;	// Form Type 15
	UnkFormArray						arrASPC;	// Form Type 16
	UnkFormArray						arrSKIL;	// Form Type 17
	tArray<EffectSetting*>				arrEffectSettings;	// Form Type 18
	UnkFormArray						arrSCPT;	// Form Type 19
	UnkFormArray						arrLTEX;	// Form Type 20
	tArray<EnchantmentItem*>			enchantments;	// Form Type 21
	tArray<SpellItem*>					spells;	// Form Type 22
	tArray<ScrollItem*>					scrolls;	// Form Type 23
	tArray<TESObjectACTI*>				arrACTI;	// Form Type 24
	UnkFormArray						arrTACT;	// Form Type 25
	tArray<TESObjectARMO*>				armors;	// Form Type 26
	tArray<TESObjectBOOK*>				books;	// Form Type 27
	tArray<TESObjectCONT*>				arrCONT;	// Form Type 28
	UnkFormArray						arrDOOR;	// Form Type 29
	tArray<IngredientItem*>				ingredients;	// Form Type 30
	UnkFormArray						arrLIGH;	// Form Type 31
	tArray<TESObjectMISC*>				miscObjects;	// Form Type 32
	UnkFormArray						arrSTAT;	// Form Type 33
	UnkFormArray						arrAPPA;	// Form Type 34
	UnkFormArray						arrSCOL;	// Form Type 35
	UnkFormArray						arrMSTT;	// Form Type 36
	UnkFormArray						arrGRAS;	// Form Type 37
	UnkFormArray						arrTREE;	// Form Type 38
	UnkFormArray						arrFLOR;	// Form Type 39
	tArray<TESFurniture*>				arrFURN;	// Form Type 40
	tArray<TESObjectWEAP*>				weapons;	// Form Type 41
	tArray<TESAmmo*>					ammo;	// Form Type 42
	tArray<TESNPC*>						npcs;	// Form Type 43
	UnkFormArray						arrLVLN;	// Form Type 44
	tArray<TESKey*>						keys;	// Form Type 45
	tArray<AlchemyItem*>				potions;	// Form Type 46
	UnkFormArray						arrIDLM;	// Form Type 47
	UnkFormArray						arrNOTE;	// Form Type 48
	UnkFormArray						arrCOBJ;	// Form Type 49
	UnkFormArray						arrPROJ;	// Form Type 50
	UnkFormArray						arrHAZD;	// Form Type 51
	UnkFormArray						arrSLGM;	// Form Type 52
	UnkFormArray						arrLVLI;	// Form Type 53
	UnkFormArray						arrWTHR;	// Form Type 54
	UnkFormArray						arrCLMT;	// Form Type 55
	UnkFormArray						arrSPGD;	// Form Type 56
	UnkFormArray						arrRFCT;	// Form Type 57
	UnkFormArray						arrREGN;	// Form Type 58
	UnkFormArray						arrNAVI;	// Form Type 59
	UnkFormArray						arrCELL;	// Form Type 60
	UnkFormArray						arrREFR;	// Form Type 61
	UnkFormArray						arrACHR;	// Form Type 62
	UnkFormArray						arrPMIS;	// Form Type 63
	UnkFormArray						arrPARW;	// Form Type 64
	UnkFormArray						arrPGRE;	// Form Type 65
	UnkFormArray						arrPBEA;	// Form Type 66
	UnkFormArray						arrPFLA;	// Form Type 67
	UnkFormArray						arrPCOM;	// Form Type 68
	UnkFormArray						arrPBAR;	// Form Type 69
	UnkFormArray						arrPHZD;	// Form Type 70
	UnkFormArray						arrWRLD;	// Form Type 71
	UnkFormArray						arrLAND;	// Form Type 72
	UnkFormArray						arrNAVM;	// Form Type 73
	UnkFormArray						arrTLOD;	// Form Type 74
	UnkFormArray						arrDIAL;	// Form Type 75
	UnkFormArray						arrINFO;	// Form Type 76
	tArray<TESQuest*>					quests;	// Form Type 77
	UnkFormArray						arrIDLE;	// Form Type 78
	UnkFormArray						arrPACK;	// Form Type 79
	UnkFormArray						arrCSTY;	// Form Type 80
	UnkFormArray						arrLSCR;	// Form Type 81
	UnkFormArray						arrLVSP;	// Form Type 82
	UnkFormArray						arrANIO;	// Form Type 83
	UnkFormArray						arrWATR;	// Form Type 84
	UnkFormArray						arrEFSH;	// Form Type 85
	UnkFormArray						arrTOFT;	// Form Type 86
	UnkFormArray						arrEXPL;	// Form Type 87
	UnkFormArray						arrDEBR;	// Form Type 88
	UnkFormArray						arrIMGS;	// Form Type 89
	UnkFormArray						arrIMAD;	// Form Type 90
	UnkFormArray						arrFLST;	// Form Type 91
	UnkFormArray						arrPERK;	// Form Type 92
	UnkFormArray						arrBPTD;	// Form Type 93
	UnkFormArray						arrADDN;	// Form Type 94
	UnkFormArray						arrAVIF;	// Form Type 95
	UnkFormArray						arrCAMS;	// Form Type 96
	UnkFormArray						arrCPTH;	// Form Type 97
	UnkFormArray						arrVTYP;	// Form Type 98
	UnkFormArray						arrMATT;	// Form Type 99
	UnkFormArray						arrIPCT;	// Form Type 100
	UnkFormArray						arrIPDS;	// Form Type 101
	tArray<TESObjectARMA*>				arrARMA;	// Form Type 102
	tArray<BGSEncounterZone*>			arrECZN;	// Form Type 103
	tArray<BGSLocation*>				arrLCTN;	// Form Type 104
	UnkFormArray						arrMESG;	// Form Type 105
	UnkFormArray						arrRGDL;	// Form Type 106
	UnkFormArray						arrDOBJ;	// Form Type 107
	UnkFormArray						arrLGTM;	// Form Type 108
	UnkFormArray						arrMUSC;	// Form Type 109
	UnkFormArray						arrFSTP;	// Form Type 110
	UnkFormArray						arrFSTS;	// Form Type 111
	UnkFormArray						arrSMBN;	// Form Type 112
	UnkFormArray						arrSMQN;	// Form Type 113
	UnkFormArray						arrSMEN;	// Form Type 114
	UnkFormArray						arrDLBR;	// Form Type 115
	UnkFormArray						arrMUST;	// Form Type 116
	UnkFormArray						arrDLVW;	// Form Type 117
	tArray<TESWordOfPower*>				arrWOOP;	// Form Type 118
	tArray<TESShout*>					arrSHOU;	// Form Type 119
	UnkFormArray						arrEQUP;	// Form Type 120
	UnkFormArray						arrRELA;	// Form Type 121
	UnkFormArray						arrSCEN;	// Form Type 122
	UnkFormArray						arrASTP;	// Form Type 123
	UnkFormArray						arrOTFT;	// Form Type 124
	UnkFormArray						arrARTO;	// Form Type 125
	UnkFormArray						arrMATO;	// Form Type 126
	UnkFormArray						arrMOVT;	// Form Type 127
	UnkFormArray						arrSNDR;	// Form Type 128
	UnkFormArray						arrDUAL;	// Form Type 129
	UnkFormArray						arrSNCT;	// Form Type 130
	UnkFormArray						arrSOPM;	// Form Type 131
	UnkFormArray						arrCOLL;	// Form Type 132
	tArray<BGSColorForm*>				arrCLFM;	// Form Type 133
	UnkFormArray						arrREVB;	// Form Type 134
	UnkFormArray						arrLENS;	// Form Type 135
	UnkFormArray						arrLSPR;	// Form Type 136
	UnkFormArray						arrVOLI;	// Form Type 137

	TESRegionList						* regionList;	// D00
//	NiTArray<TESObjectCELL*>			cellList;		// D08
	UInt64								cellList[0x3];	// D08														
//	NiTArray<BGSAddonNode*>				addonNodes;		// D20
	UInt64								addonNodes[0x3]; // D20

	UInt64								unkD38[3];		// D38
	UInt32								unkD50;			// D50 - init'd to 0x800
	UInt32								padD54;			// D54
	UInt64								unkD58;			// D58
	ModList								modList;		// D60

	UInt8								unkDA0;		// DA0
	UInt8								unkDA1;		// DA1
	UInt8								unkDA2;		// DA2
	UInt8								unkDA3;		// DA3
	UInt8								unkDA4;		// DA4
	UInt8								unkDA5;		// DA5
	UInt8								unkDA6;		// DA6
	UInt8								unkDA7;		// DA7
	UInt8								unkDA8;		// DA8
	UInt8								unkDA9;		// DA9
	UInt8								unkDAA;		// DAA
	UInt8								padDAB[5];	// DAB

	void								* regionDataManager;	// DB0 - TESRegionDataManager*, allocated in ctor
	UInt64								unkDB8;		// DB8

	const ModInfo* LookupModByName(const char* modName);

	static DataHandler * GetSingleton();

	UInt32 LoadScripts_Hook();

	MEMBER_FN_PREFIX(DataHandler);
	DEFINE_MEMBER_FN(LoadScripts, UInt32, 0x001713D0);
};

STATIC_ASSERT(offsetof(DataHandler, regionList) == 0xD00);
STATIC_ASSERT(offsetof(DataHandler, addonNodes) == 0xD20);
STATIC_ASSERT(offsetof(DataHandler, modList) == 0xD60);
STATIC_ASSERT(sizeof(DataHandler) == 0xDC0);

extern RelocPtr <DataHandler*> g_dataHandler;
extern RelocPtr <bool> g_isGameDataReady;

class MiscStatManager
{
public:
	static MiscStatManager *	GetSingleton(void);

	// 20
	struct MiscStat
	{
		const char	* name;		// 00
		const char	* unk04;	// 08
		UInt32		value;		// 10
		UInt32		unk14;		// 14
		UInt8		unk18;		// 18
		UInt8		pad19[7];	// 19
	};

	class Visitor
	{
	public:
		virtual void	Visit(MiscStat * stat, void * stat_unk04, UInt32 stat_unk0C, UInt32 value, UInt32 stat_unk10) = 0;
	};

	MEMBER_FN_PREFIX(MiscStatManager);
	// SE: It looks like another inline victim. It wasn't used by anything else in SKSE.
	//DEFINE_MEMBER_FN(Visit, void, 0x00000000, Visitor ** visitor);

	tArray<MiscStat> m_stats;	// 00

	MiscStat *	Get(const char * name);
};
STATIC_ASSERT(sizeof(MiscStatManager::MiscStat) == 0x20);

class EquipManager
{
public:
	virtual ~EquipManager();

	static EquipManager *   GetSingleton(void);

	MEMBER_FN_PREFIX(EquipManager);
	DEFINE_MEMBER_FN(EquipItem, void, 0x00637A80, Actor * actor, TESForm * item, BaseExtraList * extraData, SInt32 count, BGSEquipSlot * equipSlot, bool withEquipSound, bool preventUnequip, bool showMsg, void * unk);
	DEFINE_MEMBER_FN(UnequipItem, bool, 0x00638190, Actor * actor, TESForm * item, BaseExtraList * extraData, SInt32 count, BGSEquipSlot * equipSlot, bool unkFlag1, bool preventEquip, bool unkFlag2, bool unkFlag3, void * unk);
};


typedef BGSEquipSlot * (*_GetEitherHandSlot)();
extern RelocAddr<_GetEitherHandSlot> GetEitherHandSlot;

typedef BGSEquipSlot * (*_GetRightHandSlot)();
extern RelocAddr<_GetRightHandSlot> GetRightHandSlot;

typedef BGSEquipSlot * (*_GetLeftHandSlot)();
extern RelocAddr<_GetLeftHandSlot> GetLeftHandSlot;

typedef UInt32(*_LookupActorValueByName)(const char * name);
extern RelocAddr<_LookupActorValueByName> LookupActorValueByName;

typedef bool(*_HasLOS)(Actor* source, TESObjectREFR* target, UInt8 * unk1);
extern RelocAddr<_HasLOS> HasLOS;

typedef UInt32(*_GetRelationshipIndex)(TESForm * form1, TESForm * form2);
extern RelocAddr<_GetRelationshipIndex> GetRelationshipIndex;

class RelationshipRanks
{
public:
	static RelationshipRanks * GetSingleton(void);

	enum
	{
		kRelationshipLover = 0,
		kRelationshipAlly,
		kRelationshipConfidant,
		kRelationshipFriend,
		kRelationshipAcquaintance,
		kRelationshipRival,
		kRelationshipFoe,
		kRelationshipEnemy,
		kRelationshipArchnemesis,
		kNumRelationships
	};
	SInt32 value[kNumRelationships];

	static SInt32 GetRelationshipRank(TESForm * form1, TESForm * form2);
};

class ActorValueList
{
public:
	enum {
		kNumActorValues = 164
	};

	static ActorValueList * GetSingleton(void);
	ActorValueInfo * GetActorValue(UInt32 id);

	static UInt32 ResolveActorValueByName(const char * name);

private:
	UInt32	unk00;	// 00
	UInt32	pad04;	// 04
	ActorValueInfo * actorValues[kNumActorValues]; // 08
};

// 68 or 70
class FaceGen
{
public:
	static FaceGen *	GetSingleton(void);

	struct Action {
		BSFixedString name;
		UInt32	unk04;
		float	delta;
	};

	// 40
	class MorphDatabase
	{
	public:
		MEMBER_FN_PREFIX(MorphDatabase);
		DEFINE_MEMBER_FN(GetFaceGenModelMapEntry, bool, 0x003D4F90, const char * meshPath, BSFaceGenModelMap ** entry);
		DEFINE_MEMBER_FN(SetFaceGenModelMapEntry, void, 0x003D4D80, const char * meshPath, BSFaceGenModel * model);

		UInt64	unk00;	// 00
		UInt32	unk08;	// 08
		UInt32	unk0C;	// 0C
		UInt32	unk10;	// 10
		UInt32	unk14;	// 14
		void	*unk18;	// 18
		UInt64	unk20;	// 20
		UInt64	unk28;	// 28

		struct Data30
		{
			UInt32	unk00;
			UInt32	unk04;
		};

		Data30	unk30;		// 30
		UInt8	unk38;		// 38
		UInt8	pad39[7];	// 39
	};

	UInt32			unk00;						// 00
	UInt32			unk04;						// 04
	UInt8			unk08;						// 08
	UInt8			pad09[7];					// 09
	UInt64			unk10;						// 10
	MorphDatabase	morphDatabase;				// 18
	UInt8			isReset;					// 58
	UInt8			pad59[3];					// 59
	UInt32			unk5C;						// 5C
	UInt8			unk60;						// 60
	UInt8			pad61[7];					// 61

	MEMBER_FN_PREFIX(FaceGen);
	DEFINE_MEMBER_FN(RegenerateHead, void, 0x003D2A60, BSFaceGenNiNode * headNode, BGSHeadPart * head, TESNPC * npc);
	DEFINE_MEMBER_FN(ApplyMorph, void, 0x003D2380, BSFaceGenNiNode * faceGenNode, BGSHeadPart * headPart, BSFixedString * morphName, float relative);
};
STATIC_ASSERT(offsetof(FaceGen, isReset) == 0x58);

// Changes one HeadPart to another
typedef void(*_ChangeActorHeadPart)(Actor*, BGSHeadPart* oldPart, BGSHeadPart* newPart);
extern RelocAddr<_ChangeActorHeadPart> ChangeActorHeadPart;


// Regenerates dynamic tints
typedef UInt32(*_UpdatePlayerTints)();
extern RelocAddr<_UpdatePlayerTints> UpdatePlayerTints;

typedef BGSHeadPart ** (*_GetActorBaseOverlays)(TESNPC * npc);
extern RelocAddr<_GetActorBaseOverlays> GetActorBaseOverlays;

typedef UInt32(*_GetNumActorBaseOverlays)(TESNPC * npc);
extern RelocAddr<_GetNumActorBaseOverlays> GetNumActorBaseOverlays;

typedef bool(*_ApplyMasksToRenderTarget)(tArray<TintMask*> * tintMask, NiTexture * renderTarget);
extern RelocAddr<_ApplyMasksToRenderTarget> ApplyMasksToRenderTarget;

typedef UInt32(*_UpdateModelSkin)(NiAVObject*, NiColorA**);
extern RelocAddr<_UpdateModelSkin> UpdateModelSkin;

typedef UInt32(*_UpdateModelHair)(NiAVObject*, NiColorA**);
extern RelocAddr<_UpdateModelHair> UpdateModelHair;

typedef UInt32(*_UpdateModelFace)(NiAVObject*);
extern RelocAddr<_UpdateModelFace> UpdateModelFace;

typedef void(*_UpdateHarvestModel)(TESObjectREFR*, NiAVObject*);
extern RelocAddr<_UpdateHarvestModel> UpdateHarvestModel;

// 40
class MagicFavorites
{
	//	void			** _vtbl;	// 00
	UInt64			unk008;		// 08
	UnkFormArray	spells;		// 10
	UnkFormArray	hotkeys;	// 28

public:
	virtual	~MagicFavorites();

	void		SetHotkey(TESForm * form, SInt8 idx);
	void		ClearHotkey(SInt8 idx);
	TESForm	*	GetSpell(SInt8 idx);
	bool		IsFavorited(TESForm * form);

	static MagicFavorites * GetSingleton(void);
};

// D0
class PersistentFormManager
{
public:
	
	// 10
	struct EnchantData
	{
		EnchantmentItem *	enchantment;	// 00
		volatile SInt32		refCount;		// 08
		UInt32				pad;			// 0C
	};

	UInt64	unk00;	// 00
	tArray<EnchantData>	weaponEnchants;	// 08
	tArray<EnchantData>	armorEnchants;	// 20
	UInt64	unk38;	// 38
	UInt32	unk40;	// 40
	UInt32	unk44;	// 44
	UInt32	unk48;	// 48
	UInt32	unk4C;	// 4C

	// 30
	struct Data
	{
		void*	unk00;	// 00
		UInt64	unk08;	// 08
		void*	unk10;	// 10
		UInt64	unk18;	// 18
		UInt32	unk20;	// 20
		UInt32	unk24;	// 24
		UInt32	unk28;	// 28
		UInt32	unk2C;	// 2C
	};

	Data	unk50;	// 50
	Data	unk80;	// 80
	void*	unkB0;	// B0
	UInt64	unkB8;	// B8
	UInt64	unkC0;	// C0
	UInt64	unkC8;	// C8

	static PersistentFormManager * GetSingleton(void);

	void IncRefEnchantment(EnchantmentItem * enchantment)
	{
		if (enchantment && enchantment->formID >= 0xFF000000) {
			for (UInt32 i = 0; i < weaponEnchants.count; i++) {
				EnchantData foundData;
				weaponEnchants.GetNthItem(i, foundData);
				if (foundData.enchantment == enchantment) {
					InterlockedIncrement(&weaponEnchants[i].refCount);
					break;
				}
			}
			for (UInt32 i = 0; i < armorEnchants.count; i++) {
				EnchantData foundData;
				armorEnchants.GetNthItem(i, foundData);
				if (foundData.enchantment == enchantment) {
					InterlockedIncrement(&armorEnchants[i].refCount);
					break;
				}
			}
		}
	}

	// The game doesn't bother to dec ref or even delete custom enchants
	// when they are no longer used, maybe we can fix this?
	void DecRefEnchantment(EnchantmentItem * enchantment)
	{
		if (enchantment && enchantment->formID >= 0xFF000000) {
			for (UInt32 i = 0; i < weaponEnchants.count; i++) {
				EnchantData foundData;
				weaponEnchants.GetNthItem(i, foundData);
				if (foundData.enchantment == enchantment) {
					if (!InterlockedDecrement(&weaponEnchants[i].refCount))
						CALL_MEMBER_FN(this, ScheduleForDeletion)(enchantment);
					break;
				}
			}
			for (UInt32 i = 0; i < armorEnchants.count; i++) {
				EnchantData foundData;
				armorEnchants.GetNthItem(i, foundData);
				if (foundData.enchantment == enchantment) {
					if (!InterlockedDecrement(&armorEnchants[i].refCount))
						CALL_MEMBER_FN(this, ScheduleForDeletion)(enchantment);
					break;
				}
			}
		}
	}

	MEMBER_FN_PREFIX(PersistentFormManager);
	DEFINE_MEMBER_FN(CreateOffensiveEnchantment, EnchantmentItem *, 0x0059F0F0, tArray<MagicItem::EffectItem> * effectArray);
	DEFINE_MEMBER_FN(CreateDefensiveEnchantment, EnchantmentItem *, 0x0059F190, tArray<MagicItem::EffectItem> * effectArray);
	DEFINE_MEMBER_FN(CreatePoison, void, 0x0059F2E0, tArray<MagicItem::EffectItem> * effectArray, AlchemyItem ** poison);
	DEFINE_MEMBER_FN(CreatePotion, void, 0x0059F230, AlchemyItem ** potion, tArray<MagicItem::EffectItem> * effectArray);
	//DEFINE_MEMBER_FN(AddPersistentForm, void, 0x0068A0F0, TESForm *);
	DEFINE_MEMBER_FN(ScheduleForDeletion, void, 0x0059F6E0, TESForm *);
};
STATIC_ASSERT(sizeof(PersistentFormManager) == 0xD0);

// D8 or E0
class MenuTopicManager
{
public:
	virtual ~MenuTopicManager();
	virtual void Unk_01(void);

	NiPointer<TESObjectREFR> GetDialogueTarget();

	static MenuTopicManager * GetSingleton(void);

	BSTEventSink<void>	playerPositionEvent;	// 08
	UInt64				unk10[6];				// 10
	CRITICAL_SECTION	critSection;			// 40
	UInt32	talkingHandle;						// 68	 - init'd to g_InvalidRefHandler
	UInt32	handle2;							// 6C	 - init'd to g_InvalidRefHandler
	void*	unk70;								// 70
	UInt64	unk78;								// 78
	tArray<void *>	unk80;						// 80
	tArray<void *>	unk98;						// 98
	UInt8	unkB0;
	UInt8	unkB1;
	UInt8	unkB2;
	UInt8	unkB3;
	UInt8	unkB4;
	UInt8	unkB5;
	UInt8	unkB6;
	UInt8	unkB7;
	UInt8	unkB8;
	UInt8	unkB9;
	UInt8	unkBA;
	UInt8	unkBB;
	UInt16	padBC;
	tArray<void *>	unkC0;
};
STATIC_ASSERT(offsetof(MenuTopicManager, talkingHandle) == 0x68);

// 3D0
// Note: this class heavily changed in SE!
class BGSSaveLoadManager
{
public:
	enum
	{
		kEvent_Autosave = 1 << 0,
		kEvent_Save = 1 << 1,
		kEvent_Unk02 = 1 << 2,
		kEvent_Unk03 = 1 << 3,
		kEvent_Unk04 = 1 << 4,

		kEvent_Unk07 = 1 << 7,
	};

	static BGSSaveLoadManager *	GetSingleton(void);

	void	Save(const char * name);
	void	Load(const char * name);

	// used by Hooks_SaveLoad
	void	SaveGame_Hook(UInt64 *unk0);
	bool	LoadGame_Hook(UInt64 *unk0, UInt32 unk1, UInt32 unk2, void *unk3);
	void	ProcessEvents_Hook(void);
	void	DeleteSavegame_Hook(const char * saveName, UInt32 unk1);

	// use these when calling from a papyrus thread
	void	RequestSave(const char * name);
	void	RequestLoad(const char * name);

	MEMBER_FN_PREFIX(BGSSaveLoadManager);

	BSTEventSink<void*>	unk000;				// 000 - .?AV?$BSTEventSink@VBSSaveDataEvent@@@@
	BSTEventSink<void*>	unk008;				// 008 - .?AV?$BSTEventSink@UNameChangedEvent@RaceSexMenuEvent@@@@
	void*				unk010;				// 010

	// 58
	struct Data018
	{
		UnkArray	unk00;		// 00
		UnkArray	unk18;		// 18
		UnkArray	unk30;		// 30
		UInt64		unk48;		// 48
		UInt8		unk50;		// 50
		UInt8		pad51[7];	// 51
	};

	Data018				unk018;				// 018
	UInt32				unk070;				// 070
	UInt32				unk074;				// 074
	UInt32				unk078;				// 078
	UInt32				unk07C;				// 07C
	UInt32				unk080;				// 080
	UInt32				unk084;				// 084
	void*				unk088;				// 088
	UInt32				unk090;				// 090
	UInt32				unk094;				// 094
	void*				unk098;				// 098
	UInt32				unk0A0;				// 0A0
	UInt32				unk0A4;				// 0A4
	UInt32				unk0A8;				// 0A8
	UInt32				unk0AC;				// 0AC
	UInt32				unk0B0;				// 0B0
	UInt32				unk0B4;				// 0B4
	void*				unk0B8;				// 0B8
	UInt32				unk0C0;				// 0C0
	UInt32				unk0C4;				// 0C4
	UInt32				unk0C8;				// 0C8
	UInt32				unk0CC;				// 0CC
	UInt32				unk0D0;				// 0D0
	UInt32				unk0D4;				// 0D4
	UInt32				unk0D8;				// 0D8 - init'd to 0xFFFFFFFF
	UInt32				unk0DC;				// 0DC
	UInt32				unk0E0;				// 0E0
	UInt32				unk0E4;				// 0E4
	UnkArray			unk0E8;				// 0E8 - TODO: it is really a tArray of a structure of 0x48 bytes
	UnkArray			unk100;				// 100
	UInt8				unk118;				// 118
	UInt8				pad119[3];			// 119
	UInt32				unk11C;				// 11C
	UInt16				unk120;				// 120
	UInt16				pad122;				// 122
	UInt32				unk124;				// 124
	UInt64				startTickCount;		// 128 - GetTickCount when constructed, casted to 64 bits
	UInt8				unk130;				// 130 - init'd to 1
	UInt8				pad131[7];			// 131

	// 118
	struct Data138
	{
		char		latestSaveFileName[0x104];	// 000
		UInt32		pad104;						// 104
		Data138*	unk108;						// 108 - Init'd as self ptr. Might has been intended as a ptr to the char[] instead
		UInt8		unk110;						// 110 - init'd to 0
		UInt8		unk111;						// 111 - init'd to 0
		UInt8		unk112;						// 112 - init'd to 4
		UInt8		unk113;						// 113 - init'd to 1
		UInt32		pad114;						// 114

	};

	Data138			unk138;					// 138
	UInt32			unk250;					// 250 - init'd to FFFFFFFF   (unk28 in Skyrim32)
	bool			unk254;					// 254	(unk2C in Skyrim32)
	UInt8			pad255[3];				// 255
	void*			unk258;					// 258  (unk30 in Skyrim32)
	UInt8			unk260;					// 260 - init'd to 1  (unk34 in Skyrim32)
	UInt8			unk261;					// 261  (unk35 in Skyrim32)
	UInt8			pad262[2];				// 262
	UInt32			unk264;					// 264
	UInt32			unk268;					// 268
	UInt32			unk26C;					// 26C
	UInt8			unk270[0x2B0 - 0x270];	// 270

	// C0 .?AVThread@BGSSaveLoadManager@@
	class Thread
	{
	public:

		virtual ~Thread();
		virtual	UInt32	Unk01();
		virtual	void SetEvent();	// Just calls Windows SetEvent on hEvent

		CRITICAL_SECTION	criticalSection;	// 08
		HANDLE				hThread;			// 30
		HANDLE				currentThread;		// 38
		DWORD				threadId;			// 40
		DWORD				currentThreadId;	// 44
		UInt8				unk48;				// 48
		UInt8				pad49[3];			// 49
		UInt8				unk50;				// 50 - init'd to 1
		UInt8				unk51;				// 51
		UInt16				pad52;				// 52
		UInt32				unk54;				// 54 - init'd to 1
		HANDLE				hEvent;				// 58
		UInt8				unk60[0x60];		// 60 - TODO:  .?AV?$BSTCommonStaticMessageQueue@V?$BSTSmartPointer@VRequest@saveload@bgs@@UBSTSmartPointerIntrusiveRefCount@@@@$07@@
	};

	Thread				thread;				// 2B0
	UInt8				unk370[0x60];		// 370 - TODO:  .?AV?$BSTCommonStaticMessageQueue@V?$BSTSmartPointer@VRequest@saveload@bgs@@UBSTSmartPointerIntrusiveRefCount@@@@$07@@
	
	private:
	// SE: Save_Internal signature changed! Normal save: unk1=2, unk2=0
	DEFINE_MEMBER_FN(Save_Internal, bool, 0x00586DE0, int unk1, UInt32 unk2, const char * name);
	DEFINE_MEMBER_FN(Load_Internal, bool, 0x00587350, const char * name, int unk1, UInt32 unk2, UInt32 unk3);

	DEFINE_MEMBER_FN(SaveGame_HookTarget, void, 0x0057CCC0, UInt64 *unk0);
	DEFINE_MEMBER_FN(LoadGame_HookTarget, bool, 0x0057D1A0, UInt64 *unk0, UInt32 unk1, UInt32 unk2, void *unk3);

	DEFINE_MEMBER_FN(ProcessEvents_Internal, void, 0x00589A60);
	DEFINE_MEMBER_FN(DeleteSavegame, void, 0x00586D70, const char * saveName, UInt32 unk1);
};
STATIC_ASSERT(offsetof(BGSSaveLoadManager, thread) == 0x2B0);
STATIC_ASSERT(offsetof(BGSSaveLoadManager::Thread, hThread) == 0x30);
STATIC_ASSERT(sizeof(BGSSaveLoadManager::Thread) == 0xC0);
STATIC_ASSERT(sizeof(BGSSaveLoadManager) == 0x3D0);

class DefaultObjectList
{
public:
	enum {
		kNumDefaultObjects = 0x15B		// SE: there is one more than in Skyrim32 ("Mods Help FormList")
	};

	// 18 - Verified
	struct DefaultObject
	{
		const char	* description;	// 00
		UInt32		unk08;			// 08
		UInt32		key;			// 0C
		UInt32		unk10;			// 10
		UInt32		pad;			// 14
	};

	static DefaultObjectList * GetSingleton(void);

	DefaultObject	objects[kNumDefaultObjects];
};
STATIC_ASSERT(sizeof(DefaultObjectList::DefaultObject) == 0x18);

class FacePresetList
{
public:
	enum {
		kNumPresets = 4
	};
	enum {
		kPreset_NoseType,
		kPreset_BrowType,
		kPreset_EyesType,
		kPreset_LipType
	};

	struct Preset
	{
		const char * presetName;
		Setting * gameSetting;
	};

	Preset presets[kNumPresets];

	static FacePresetList *	GetSingleton(void);
};

class FaceMorphList
{
public:
	enum {
		kNumMorphs = 19
	};

	enum {
		kMorph_NoseShortLong = 0,
		kMorph_NoseDownUp,
		kMorph_JawUpDown,
		kMorph_JawNarrowWide,
		kMorph_JawBackForward,
		kMorph_CheeksDownUp,
		kMorph_CheeksInOut,
		kMorph_EyesMoveDownUp,
		kMorph_EyesMoveInOut,
		kMorph_BrowDownUp,
		kMorph_BrowInOut,
		kMorph_BrowBackForward,
		kMorph_LipMoveDownUp,
		kMorph_LipMoveInOut,
		kMorph_ChinThinWide,
		kMorph_ChinMoveUpDown,
		kMorph_OverbiteUnderbite,
		kMorph_EyesBackForward,
		kMorph_Vampire
	};

	struct Morph
	{
		UInt32 type;
		const char * lowerName;
		const char * upperName;
	};

	Morph morphs[kNumMorphs];
};
