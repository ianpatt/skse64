#pragma once

#include "skse64/GameTypes.h"
#include "skse64/GameFormComponents.h"
//#include "skse/GamePathing.h"
//#include "skse/NiInterpolators.h"
#include "skse64/NiObjects.h"
#include "skse64/NiTypes.h"

class TESForm;
class TESObjectSTAT;
class TESFile;
class TESObjectLIGH;
class TESEffectShader;
class BGSProjectile;
class BGSArtObject;
class BGSImpactDataSet;
class TESSound;
class TESObjectREFR;
class BGSListForm;
class TESQuest;
class BGSExplosion;
class BGSDualCastData;
class TESImageSpaceModifier;
class TESWordOfPower;
class BGSHazard;
class TESNPC;
class TESWorldSpace;
class BSExtraData;
struct ModInfo;

typedef TESForm* (*_LookupFormByID)(UInt32 id);
extern RelocAddr <_LookupFormByID> LookupFormByID;

class FormMatcher
{
public:
	virtual bool Matches(TESForm* pForm) const = 0;
};


// TESObject and derivatives are in GameObjects.h
// TESObjectREFR and derivatives are in GameReferences.h
// everything else deriving from TESForm goes here

/**** form types ***************************************************************
 *	
 *	TESHair and BGSCloudClusterForm were removed in 1.2
 *	
 *	1.1	1.2	se1.4.2
 *	id	id	id		code	name
 *	
 *	00	00	??		NONE	
 *	01	01	??		TES4	
 *	02	02	??		GRUP	
 *	03	03	??		GMST	<nonstandard loader?>
 *	04	04	04		KYWD	BGSKeyword
 *	05	05	05		LCRT	BGSLocationRefType
 *	06	06	06		AACT	BGSAction
 *	07	07	07		TXST	BGSTextureSet
 *	08	08	08		MICN	BGSMenuIcon
 *	09	09	09		GLOB	TESGlobal
 *	0A	0A	0A		CLAS	TESClass
 *	0B	0B	0B		FACT	TESFaction
 *	0C	0C	0C		HDPT	BGSHeadPart
 *	0D	xx	xx		HAIR	TESHair
 *	0E	0D	0D		EYES	TESEyes
 *	0F	0E	0E		RACE	TESRace
 *	10	0F	0F		SOUN	TESSound
 *	11	10	10		ASPC	BGSAcousticSpace
 *	12	11	??		SKIL	<missing>
 *	13	12	12		MGEF	EffectSetting
 *	14	13	13		SCPT	Script
 *	15	14	14		LTEX	TESLandTexture
 *	16	15	15		ENCH	EnchantmentItem
 *	17	16	16		SPEL	SpellItem
 *	18	17	17		SCRL	ScrollItem
 *	19	18	18		ACTI	TESObjectACTI
 *	1A	19	19		TACT	BGSTalkingActivator
 *	1B	1A	1A		ARMO	TESObjectARMO
 *	1C	1B	1B		BOOK	TESObjectBOOK
 *	1D	1C	1C		CONT	TESObjectCONT
 *	1E	1D	1D		DOOR	TESObjectDOOR
 *	1F	1E	1E		INGR	IngredientItem
 *	20	1F	1F		LIGH	TESObjectLIGH
 *	21	20	20		MISC	TESObjectMISC
 *	22	21	21		APPA	BGSApparatus
 *	23	22	22		STAT	TESObjectSTAT
 *	24	23	23		SCOL	BGSStaticCollection
 *	25	24	24		MSTT	BGSMovableStatic
 *	26	25	25		GRAS	TESGrass
 *	27	26	26		TREE	TESObjectTREE
 *	28	xx	xx		CLDC	BGSCloudClusterForm
 *	29	27	27		FLOR	TESFlora
 *	2A	28	28		FURN	TESFurniture
 *	2B	29	29		WEAP	TESObjectWEAP
 *	2C	2A	2A		AMMO	TESAmmo
 *	2D	2B	2B		NPC_	TESNPC
 *	2E	2C	2C		LVLN	TESLevCharacter
 *	2F	2D	2D		KEYM	TESKey
 *	30	2E	2E		ALCH	AlchemyItem
 *	31	2F	2F		IDLM	BGSIdleMarker / BGSDefaultObjectManager? strange
 *	32	30	30		NOTE	BGSNote
 *	33	31	31		COBJ	BGSConstructibleObject
 *	34	32	32		PROJ	BGSProjectile
 *	35	33	33		HAZD	BGSHazard
 *	36	34	34		SLGM	TESSoulGem
 *	37	35	35		LVLI	TESLevItem
 *	38	36	36		WTHR	TESWeather
 *	39	37	37		CLMT	TESClimate
 *	3A	38	38		SPGD	BGSShaderParticleGeometryData
 *	3B	39	39		RFCT	BGSReferenceEffect
 *	3C	3A	3A		REGN	TESRegion
 *	3D	3B	??		NAVI	NavMeshInfoMap
 *	3E	3C	3C		CELL	TESObjectCELL
 *	3F	3D	3D		REFR	TESObjectREFR / Actor
 *	40	3E	3E		ACHR	Character / PlayerCharacter
 *	41	3F	3F		PMIS	MissileProjectile
 *	42	40	40		PARW	ArrowProjectile
 *	43	41	41		PGRE	GrenadeProjectile
 *	44	42	42		PBEA	BeamProjectile
 *	45	43	43		PFLA	FlameProjectile
 *	46	44	44		PCON	ConeProjectile
 *	47	45	45		PBAR	BarrierProjectile
 *	48	46	46		PHZD	Hazard
 *	49	47	47		WRLD	TESWorldSpace
 *	4A	48	48		LAND	TESObjectLAND
 *	4B	49	49		NAVM	NavMesh
 *	4C	4A	4A		TLOD	?
 *	4D	4B	4B		DIAL	TESTopic
 *	4E	4C	4C		INFO	TESTopicInfo
 *	4F	4D	4D		QUST	TESQuest
 *	50	4E	4E		IDLE	TESIdleForm
 *	51	4F	4F		PACK	TESPackage
 *	52	50	50		CSTY	TESCombatStyle
 *	53	51	51		LSCR	TESLoadScreen
 *	54	52	52		LVSP	TESLevSpell
 *	55	53	53		ANIO	TESObjectANIO
 *	56	54	54		WATR	TESWaterForm
 *	57	55	55		EFSH	TESEffectShader
 *	58	56	56		TOFT	?
 *	59	57	57		EXPL	BGSExplosion
 *	5A	58	58		DEBR	BGSDebris
 *	5B	59	59		IMGS	TESImageSpace
 *	5C	5A	5A		IMAD	TESImageSpaceModifier
 *	5D	5B	5B		FLST	BGSListForm
 *	5E	5C	5C		PERK	BGSPerk
 *	5F	5D	5D		BPTD	BGSBodyPartData
 *	60	5E	5E		ADDN	BGSAddonNode
 *	61	5F	5F		AVIF	ActorValueInfo
 *	62	60	60		CAMS	BGSCameraShot
 *	63	61	61		CPTH	BGSCameraPath
 *	64	62	62		VTYP	BGSVoiceType
 *	65	63	63		MATT	BGSMaterialType
 *	66	64	64		IPCT	BGSImpactData
 *	67	65	65		IPDS	BGSImpactDataSet
 *	68	66	66		ARMA	TESObjectARMA
 *	69	67	67		ECZN	BGSEncounterZone
 *	6A	68	68		LCTN	BGSLocation
 *	6B	69	69		MESH	BGSMessage
 *	6C	6A	6A		RGDL	BGSRagdoll
 *	6D	6B	6B		DOBJ	?
 *	6E	6C	6C		LGTM	BGSLightingTemplate
 *	6F	6D	6D		MUSC	BGSMusicType
 *	70	6E	6E		FSTP	BGSFootstep
 *	71	6F	6F		FSTS	BGSFootstepSet
 *	72	70	70		SMBN	BGSStoryManagerBranchNode
 *	73	71	71		SMQN	BGSStoryManagerQuestNode
 *	74	72	72		SMEN	BGSStoryManagerEventNode
 *	75	73	73		DLBR	BGSDialogueBranch
 *	76	74	74		MUST	BGSMusicTrackFormWrapper
 *	77	75	??		DLVW	?
 *	78	76	76		WOOP	TESWordOfPower
 *	79	77	77		SHOU	TESShout
 *	7A	78	78		EQUP	BGSEquipSlot
 *	7B	79	79		RELA	BGSRelationship
 *	7C	7A	7A		SCEN	BGSScene
 *	7D	7B	7B		ASTP	BGSAssociationType
 *	7E	7C	7C		OTFT	BGSOutfit
 *	7F	7D	7D		ARTO	BGSArtObject
 *	80	7E	7E		MATO	BGSMaterialObject
 *	81	7F	7F		MOVT	BGSMovementType
 *	82	80	80		SNDR	BGSSoundDescriptorForm
 *	83	81	81		DUAL	BGSDualCastData
 *	84	82	82		SNCT	BGSSoundCategory
 *	85	83	83		SOPM	BGSSoundOutput
 *	86	84	84		COLL	BGSCollisionLayer
 *	87	85	85		CLFM	BGSColorForm
 *	88	86	86		REVB	BGSReverbParameters
 *		87	87		LENS	Unknown87 / BGSLensFlare
 *		88	??		LSPR	Unknown88
 *		89	89		VOLI	Unknown89 / BGSVolumetricLighting
 *		8A			Unknown8A
 *		8B			Alias
 *		8C			ReferenceAlias
 *		8D			LocAlias
 *		8E			ActiveMagicEffect
 */

enum FormType {
	kFormType_None = 0,				//	NONE
	kFormType_TES4,					//	TES4
	kFormType_Group,				//	GRUP	
	kFormType_GMST,					//	GMST
	kFormType_Keyword,				//	KYWD	BGSKeyword
	kFormType_LocationRef,			//	LCRT	BGSLocationRefType
	kFormType_Action,				//	AACT	BGSAction
	kFormType_TextureSet,			//	TXST	BGSTextureSet
	kFormType_MenuIcon,				//	MICN	BGSMenuIcon
	kFormType_Global,				//	GLOB	TESGlobal
	kFormType_Class,				//	CLAS	TESClass
	kFormType_Faction,				//	FACT	TESFaction
	kFormType_HeadPart,				//	HDPT	BGSHeadPart
	kFormType_Eyes,					//	EYES	TESEyes
	kFormType_Race,					//	RACE	TESRace
	kFormType_Sound,				//	SOUN	TESSound
	kFormType_AcousticSpace,		//	ASPC	BGSAcousticSpace
	kFormType_Skill,				//	SKIL	<missing>
	kFormType_EffectSetting,		//	MGEF	EffectSetting
	kFormType_Script,				//	SCPT	Script
	kFormType_LandTexture,			//	LTEX	TESLandTexture
	kFormType_Enchantment,			//	ENCH	EnchantmentItem
	kFormType_Spell,				//	SPEL	SpellItem
	kFormType_ScrollItem,			//	SCRL	ScrollItem
	kFormType_Activator,			//	ACTI	TESObjectACTI
	kFormType_TalkingActivator,		//	TACT	BGSTalkingActivator
	kFormType_Armor,				//	ARMO	TESObjectARMO
	kFormType_Book,					//	BOOK	TESObjectBOOK
	kFormType_Container,			//	CONT	TESObjectCONT
	kFormType_Door,					//	DOOR	TESObjectDOOR
	kFormType_Ingredient,			//	INGR	IngredientItem
	kFormType_Light,				//	LIGH	TESObjectLIGH
	kFormType_Misc,					//	MISC	TESObjectMISC
	kFormType_Apparatus,			//	APPA	BGSApparatus
	kFormType_Static,				//	STAT	TESObjectSTAT
	kFormType_StaticCollection,		//	SCOL	BGSStaticCollection
	kFormType_MovableStatic,		//	MSTT	BGSMovableStatic
	kFormType_Grass,				//	GRAS	TESGrass
	kFormType_Tree,					//	TREE	TESObjectTREE
	kFormType_Flora,				//	FLOR	TESFlora
	kFormType_Furniture,			//	FURN	TESFurniture
	kFormType_Weapon,				//	WEAP	TESObjectWEAP
	kFormType_Ammo,					//	AMMO	TESAmmo
	kFormType_NPC,					//	NPC_	TESNPC
	kFormType_LeveledCharacter,		//	LVLN	TESLevCharacter
	kFormType_Key,					//	KEYM	TESKey
	kFormType_Potion,				//	ALCH	AlchemyItem
	kFormType_IdleMarker,			//	IDLM	BGSIdleMarker / BGSDefaultObjectManager? strange
	kFormType_Note,					//	NOTE	BGSNote
	kFormType_ConstructibleObject,	//	COBJ	BGSConstructibleObject
	kFormType_Projectile,			//	PROJ	BGSProjectile
	kFormType_Hazard,				//	HAZD	BGSHazard
	kFormType_SoulGem,				//	SLGM	TESSoulGem
	kFormType_LeveledItem,			//	LVLI	TESLevItem
	kFormType_Weather,				//	WTHR	TESWeather
	kFormType_Climate,				//	CLMT	TESClimate
	kFormType_SPGD,					//	SPGD	BGSShaderParticleGeometryData
	kFormType_ReferenceEffect,		//	RFCT	BGSReferenceEffect
	kFormType_Region,				//	REGN	TESRegion
	kFormType_NAVI,					//	NAVI	NavMeshInfoMap
	kFormType_Cell,					//	CELL	TESObjectCELL
	kFormType_Reference,			//	REFR	TESObjectREFR / Actor
	kFormType_Character,			//	ACHR	Character / PlayerCharacter
	kFormType_Missile,				//	PMIS	MissileProjectile
	kFormType_Arrow,				//	PARW	ArrowProjectile
	kFormType_Grenade,				//	PGRE	GrenadeProjectile
	kFormType_BeamProj,				//	PBEA	BeamProjectile
	kFormType_FlameProj,			//	PFLA	FlameProjectile
	kFormType_ConeProj,				//	PCON	ConeProjectile
	kFormType_BarrierProj,			//	PBAR	BarrierProjectile
	kFormType_PHZD,					//	PHZD	Hazard
	kFormType_WorldSpace,			//	WRLD	TESWorldSpace
	kFormType_Land,					//	LAND	TESObjectLAND
	kFormType_NAVM,					//	NAVM	NavMesh
	kFormType_TLOD,					//	TLOD	?
	kFormType_Topic,				//	DIAL	TESTopic
	kFormType_TopicInfo,			//	INFO	TESTopicInfo
	kFormType_Quest,				//	QUST	TESQuest
	kFormType_Idle,					//	IDLE	TESIdleForm
	kFormType_Package,				//	PACK	TESPackage
	kFormType_CombatStyle,			//	CSTY	TESCombatStyle
	kFormType_LoadScreen,			//	LSCR	TESLoadScreen
	kFormType_LeveledSpell,			//	LVSP	TESLevSpell
	kFormType_ANIO,					//	ANIO	TESObjectANIO
	kFormType_Water,				//	WATR	TESWaterForm
	kFormType_EffectShader,			//	EFSH	TESEffectShader
	kFormType_TOFT,					//	TOFT	?
	kFormType_Explosion,			//	EXPL	BGSExplosion
	kFormType_Debris,				//	DEBR	BGSDebris
	kFormType_ImageSpace,			//	IMGS	TESImageSpace
	kFormType_ImageSpaceMod,		//	IMAD	TESImageSpaceModifier
	kFormType_List,					//	FLST	BGSListForm
	kFormType_Perk,					//	PERK	BGSPerk
	kFormType_BodyPartData,			//	BPTD	BGSBodyPartData
	kFormType_AddonNode,			//	ADDN	BGSAddonNode
	kFormType_ActorValueInfo,		//	AVIF	ActorValueInfo
	kFormType_CameraShot,			//	CAMS	BGSCameraShot
	kFormType_CameraPath,			//	CPTH	BGSCameraPath
	kFormType_VoiceType,			//	VTYP	BGSVoiceType
	kFormType_MaterialType,			//	MATT	BGSMaterialType
	kFormType_ImpactData,			//	IPCT	BGSImpactData
	kFormType_ImpactDataSet,		//	IPDS	BGSImpactDataSet
	kFormType_ARMA,					//	ARMA	TESObjectARMA
	kFormType_EncounterZone,		//	ECZN	BGSEncounterZone
	kFormType_Location,				//	LCTN	BGSLocation
	kFormType_Message,				//	MESH	BGSMessage
	kFormType_Ragdoll,				//	RGDL	BGSRagdoll
	kFormType_DOBJ,					//	DOBJ	? (used for default objects, custom loader)
	kFormType_LightingTemplate,		//	LGTM	BGSLightingTemplate
	kFormType_MusicType,			//	MUSC	BGSMusicType
	kFormType_Footstep,				//	FSTP	BGSFootstep
	kFormType_FootstepSet,			//	FSTS	BGSFootstepSet
	kFormType_StoryBranchNode,		//	SMBN	BGSStoryManagerBranchNode
	kFormType_StoryQuestNode,		//	SMQN	BGSStoryManagerQuestNode
	kFormType_StoryEventNode,		//	SMEN	BGSStoryManagerEventNode
	kFormType_DialogueBranch,		//	DLBR	BGSDialogueBranch
	kFormType_MusicTrack,			//	MUST	BGSMusicTrackFormWrapper
	kFormType_DLVW,					//	DLVW	?
	kFormType_WordOfPower,			//	WOOP	TESWordOfPower
	kFormType_Shout,				//	SHOU	TESShout
	kFormType_EquipSlot,			//	EQUP	BGSEquipSlot
	kFormType_Relationship,			//	RELA	BGSRelationship
	kFormType_Scene,				//	SCEN	BGSScene
	kFormType_AssociationType,		//	ASTP	BGSAssociationType
	kFormType_Outfit,				//	OTFT	BGSOutfit
	kFormType_Art,					//	ARTO	BGSArtObject
	kFormType_Material,				//	MATO	BGSMaterialObject
	kFormType_MovementType,			//	MOVT	BGSMovementType
	kFormType_SoundDescriptor,		//	SNDR	BGSSoundDescriptorForm
	kFormType_DualCastData,			//	DUAL	BGSDualCastData
	kFormType_SoundCategory,		//	SNCT	BGSSoundCategory
	kFormType_SoundOutput,			//	SOPM	BGSSoundOutput
	kFormType_CollisionLayer,		//	COLL	BGSCollisionLayer
	kFormType_ColorForm,			//	CLFM	BGSColorForm
	kFormType_ReverbParam,			//	REVB	BGSReverbParameters
	kFormType_LensFlare,			//	
	kFormType_Unk88,
	kFormType_VolumetricLighting,	//	
	kFormType_Unk8A,
	kFormType_Alias,				//			BGSBaseAlias
	kFormType_ReferenceAlias,		//			BGSRefAlias
	kFormType_LocationAlias,		//			BGSLocAlias
	kFormType_ActiveMagicEffect,	//			ActiveMagicEffect

	kFormType_Max = kFormType_VolumetricLighting	// max of standard types
};

STATIC_ASSERT(kFormType_Weapon == 0x29);
STATIC_ASSERT(kFormType_WordOfPower == 0x76);

// 20 
class TESForm : public BaseFormComponent
{
public:
	enum { kTypeID = 0 };	// special-case

	enum {
		kFlagIsDeleted = 0x20,
		kFlagPlayerKnows = 0x40,
		kFlagUnk_0x800 = 0x800
	};

	virtual void			Unk_04(void);		// reset/init? would leak pointers if called on a live object
	virtual void			Unk_05(void);		// release pointers?
	virtual bool			LoadForm(UInt32 arg);
	virtual bool			Unk_07(UInt32 arg);
	virtual bool			Unk_08(UInt32 arg);	// calls LoadForm
	virtual TESForm *		Unk_09(UInt32 arg1, void * arg2);
	virtual bool			MarkChanged(UInt32 changed);	// mark as changed?
	virtual void			Unk_0B(UInt32 arg);
	virtual bool			Unk_0C(UInt32 arg);
	virtual void			Unk_0D(UInt32 arg);
	virtual void			Unk_0E(UInt32 arg);
	virtual void			Unk_0F(void * arg);
	virtual void			Unk_10(UInt32 arg);
	virtual void			Unk_11(UInt32 arg);
	virtual void			Unk_12(UInt32 arg);
	virtual void			InitItem(void);		// called once all dependent forms are loaded
	virtual ModInfo*		GetFinalSourceFile();
	virtual UInt32			GetFormType(void);
	virtual void			GetFormDesc(char * buf, UInt32 bufLen);
	virtual bool			GetFlag00000040(void);
	virtual bool			GetFlag00010000(void);
	virtual bool			IsPlayable(void);
	virtual bool			GetFlag00080000(void);
	virtual bool			GetFlag02000000(void);
	virtual bool			Unk_1B(void);
	virtual bool			GetFlag00000200(void);
	virtual bool			GetFlag00000100(void);
	virtual void			SetFlag00000200(bool set);
	virtual bool			Unk_20(void);
	virtual void			SetFlag00000020(bool set);
	virtual void			SetFlag00000002(bool set);
	virtual void			Unk_23(void);
	virtual void			Unk_24(UInt32 arg);
	virtual bool			Unk_25(void);
	virtual bool			Unk_26(void);
	virtual bool			Has3D(void);
	virtual bool			Unk_28(void);
	virtual UInt32			Unk_29(void);
	virtual UInt32			Unk_2A(void);
	virtual UInt32			Unk_2B(void);
	virtual const char *	GetFullName(void);
	virtual void			CopyFrom(TESForm * srcForm);
	virtual bool			Compare(TESForm * rhs);
	virtual void			Unk_2F(void * dst, UInt32 unk);
	virtual const char *	GetName(void);
	virtual bool			SetName(const char * str);
	virtual bool			Unk_32(void);
	virtual bool			Unk_33(void);
	virtual bool			Unk_34(UInt32 arg);
	virtual bool			Unk_35(UInt32 arg0, UInt32 arg1, UInt32 arg2, UInt32 arg3, UInt32 arg4);
	virtual void			SetFormID(UInt32 id, UInt32 generateID);
	virtual const char *	Unk_37(void);
	virtual bool			Unk_38(void);

	bool IsWeapon() { return formType == kFormType_Weapon; }
	bool IsAmmo() { return formType == kFormType_Ammo; }
	bool IsArmor() { return formType == kFormType_Armor; }

	// bethesda removed most of the functionality from their code, this reimplements it for simple classes
	void CopyFromEx(TESForm * rhs);

	void	* unk08;	// 08
	UInt32	flags;		// 10
	UInt32	formID;		// 14
	UInt16	unk18;		// 18
	UInt8	formType;	// 1A
	UInt8	pad1B;		// 1B
	UInt32	pad;		// 1C
};

typedef float (* _GetFormWeight)(TESForm* form);
float GetFormWeight(TESForm *form);

// 128 
class ActorValueInfo : public TESForm
{
public:
	enum { kTypeID = kFormType_ActorValueInfo };

	// parents
	TESFullName			fullName;		// 020
	TESDescription		description;	// 030
	TESIcon				icon;			// 040

	enum {
		kSkillUseMult = 0,
		kSkillOffsetMult,
		kSkillImproveMult,
		kSkillImproveOffset
	};

	// members
	const char *		name;			// 050 - second ctor argument
	StringCache::Ref	unk58;			// 058
	UInt32				unk60;			// 060
	UInt32				unk64;			// 064
	UInt64				unk68;			// 068
	UInt32				unk70;			// 070
	UInt32				unk74[0x0F];	// 074 - init'd to 0xA4
	UInt32				numSubTypes;	// 0B0
	const char *		subTypes[0x0A];	// 0B8 - init'd to 0
	float *				skillUsages;	// 108
	UInt32				unk110;			// 110 - init to FFFFFFFF
	UInt32				unk118;			// 114 - init to FFFFFFFF
	BGSSkillPerkTreeNode *	perkTree;	// 118 - BGSSkillPerkTreeNode

	UInt8	pad120[8];	// 120 - ? not initialized
};

// 48 
class BGSAssociationType : public TESForm
{
public:
	enum { kTypeID = kFormType_AssociationType };

	BSFixedString	unk20[4];	// 20
	UInt32			unk40;		// 40
};

// 88 
class BGSBodyPartData : public TESForm
{
public:
	enum { kTypeID = kFormType_BodyPartData };

	// parents
	TESModel		model;			// 20
	BGSPreloadable	preloadable;	// 48

	// members
	struct Data
	{
		BSFixedString	unk00;		// 00
		BSFixedString	unk08;		// 08
		BSFixedString	unk10;		// 10
		BSFixedString	unk18;		// 18
		BSFixedString	unk20;		// 20
		TESModel		model;		// 28
		TESModelRDT		modelRDT;	// 50
	};

	Data	* part[5];		// 50 - init'd to 0
	UInt64	unk78;			// 78 - init'd to 0
	UInt64  unk80;			// 80 - init'd to 0
};

// 58 
class BGSCameraPath : public TESForm
{
public:
	enum { kTypeID = kFormType_CameraPath };

	void	*unk20;		// 20
	UInt64	unk28;		// 28
	UInt64	unk30;		// 30
	UInt8	unk38;		// 38
	UInt8	pad39[7];	// 39
	UInt64	unk40;		// 40
	UInt64	unk48;		// 48
	UInt64	unk50;		// 50
};

// C0 
class BGSCameraShot : public TESForm
{
public:
	enum { kTypeID = kFormType_CameraShot };

	// parents
	TESModel					model;			// 20
	TESImageSpaceModifiableForm	imageSpaceMod;	// 48

	// members
	UInt32	unk58;		// 58 - init to 2
	UInt32	unk5C;		// 5C - init to 2
	UInt32	unk60;		// 60 
	UInt32	unk64;		// 64 - init to 3
	UInt32	unk68;		// 68
	UInt32	unk6C;		// 6C
	float	globalTimeMultiplier;	// 70 - init to 1.0f
	UInt32	unk74;		// 74
	UInt32	unk78;		// 78
	UInt32	unk7C;		// 7C
	UInt32  unk80;		// 80
	UInt64	unk88;		// 88
	UInt64	unk90;		// 90
	UInt64	unk98;		// 98
	UInt64	unkA0;		// A0
	UInt64	unkA8;		// A8
	UInt16	unkB0;		// B0 - init to FF
	UInt8	padB2[6];	// B2
	UInt64	unkB8;		// B8
};

// 60 
class BGSCollisionLayer : public TESForm
{
public:
	enum { kTypeID = kFormType_CollisionLayer };

	enum {
		kCollisionLayer_Unidentified = 0x00,
		kCollisionLayer_Static,
		kCollisionLayer_AnimStatic,
		kCollisionLayer_Transparent,
		kCollisionLayer_Clutter,
		kCollisionLayer_Weapon,
		kCollisionLayer_Projectile,
		kCollisionLayer_Spell,
		kCollisionLayer_Biped,
		kCollisionLayer_Trees,
		kCollisionLayer_Props,
		kCollisionLayer_Water,
		kCollisionLayer_Trigger,
		kCollisionLayer_Terrain,
		kCollisionLayer_Trap,
		kCollisionLayer_NonCollidable,
		kCollisionLayer_CloudTrap,
		kCollisionLayer_Ground,
		kCollisionLayer_Portal,
		kCollisionLayer_DebrisSmall,
		kCollisionLayer_DebrisLarge,
		kCollisionLayer_AcousticSpace,
		kCollisionLayer_ActorZone,
		kCollisionLayer_ProjectileZone,
		kCollisionLayer_GasTrap,
		kCollisionLayer_Shellcasing,
		kCollisionLayer_TransparentSmall,
		kCollisionLayer_InvisibleWall,
		kCollisionLayer_TransparentSmallAnim,
		kCollisionLayer_Ward,
		kCollisionLayer_CharController,
		kCollisionLayer_StairHelper,
		kCollisionLayer_DeadBip,
		kCollisionLayer_BipedNoCC,
		kCollisionLayer_AvoidBox,
		kCollisionLayer_CollisionBox,
		kCollisionLayer_CameraSphere,
		kCollisionLayer_DoorDetection,
		kCollisionLayer_ConeProjectile,
		kCollisionLayer_Camera,
		kCollisionLayer_ItemPicker,
		kCollisionLayer_Los,
		kCollisionLayer_PathingPick,
		kCollisionLayer_CustomPick1,
		kCollisionLayer_CustomPick2,
		kCollisionLayer_SpellExplosion,
		kCollisionLayer_DroppingPick,
		kCollisionLayer_DeadActorZone,
		kCollisionLayer_TriggerFallingTrap,
		kCollisionLayer_Navcut,
		kCollisionLayer_Critter,
		kCollisionLayer_SpellTrigger,
		kCollisionLayer_LivingAndDeadActors,
		kCollisionLayer_Detection,
		kCollisionLayer_TrapTrigger
	};

	// parents
	TESDescription				description;	// 20

	// members
	UInt32						layerId;	// 30 - init to FFFFFFFF
	UInt32						flags;	// 34
	UInt32						unk38;	// 38
	BSFixedString			name;	// 40
	tArray<BGSCollisionLayer*>	interactables;	// 48 - really?
};

// 38 
class BGSColorForm : public TESForm
{
public:
	enum { kTypeID = kFormType_ColorForm };

	// parents
	TESFullName     fullName;       // 20

	union {
		struct Color {
			UInt8   red, green, blue, alpha; // The alpha isn't actually used here so its usually zero
		} color;
		UInt32 abgr;    // 30
	};
	UInt32  unk34;  // 34
};

// 58 
class BGSConstructibleObject : public TESForm
{
public:
    enum { kTypeID = kFormType_ConstructibleObject };

    // members
	TESContainer	container;		// 20 - not inherited
	void*			unk20;			// 38 - linked list
    TESForm*		createdObject;	// 40
    BGSKeyword*		wbKeyword;		// 48
    UInt16			quantity;		// 50
    UInt8			pad52[6];		// 52
};

// 38 
class BGSDebris : public TESForm
{
public:
	enum { kTypeID = kFormType_Debris };

	struct Data
	{
		UInt64	unk0;
		UInt64	unk8;
	};

	// parents
	BGSPreloadable	preloadable;	// 20
	
	// members
	Data	unk28;	// 28 - linked list
};

// ?? - singleton, ID 00000031 
class BGSDefaultObjectManager : public TESForm
{
public:
	enum { kTypeID = kFormType_DOBJ };

	// cleared to 0 in ctor
	TESForm	* objects[0x15B];			// 20
	UInt8	pad[0x15B];					// just pad out the rest of the space

	static BGSDefaultObjectManager *	GetSingleton(void);
};

// 40 
class BGSDialogueBranch : public TESForm
{
public:
	enum { kTypeID = kFormType_DialogueBranch };

	UInt32	unk20;		// 20 - init'd to 1
	TESForm	* unk28;	// 28 - init'd to 0, TESQuest
	TESForm	* unk30;	// 30 - init'd to 0, starting topic (type 4D)
	UInt32	unk38;		// 38 - init'd to 0
};

class BGSLocation;

// 48 
class BGSEncounterZone : public TESForm
{
public:
	enum { kTypeID = kFormType_EncounterZone };

	TESForm *owner;	// 20
	BGSLocation *location; // 28

	UInt8 rank; // 30
	UInt8 minLevel; // 31
	UInt8 encounterFlags; // 32
	UInt8 maxLevel; // 33

	UInt64 unk38; // 38
	UInt64 unk40; // 40	
};

// 40 
class BGSEquipSlot : public TESForm
{
public:
	enum { kTypeID = kFormType_EquipSlot };

	tArray<BGSEquipSlot*>		parentSlots; // 20
	UInt32						unk38; // 38
};

// 30 
class BGSFootstep : public TESForm
{
public:
	enum { kTypeID = kFormType_Footstep };

	BSFixedString	unk20;	// 20
	UInt64			unk28;	// 28
};

// 98 
class BGSFootstepSet : public TESForm
{
public:
	enum { kTypeID = kFormType_FootstepSet };

	UnkArray	unk14[5];	// 20
};

// 120 
class BGSHeadPart : public TESForm
{
public:
	enum { kTypeID = kFormType_HeadPart };


	// parents
	TESFullName				fullName;       // 020
	TESModelTextureSwap		model;          // 030

	// members
	UInt8					partFlags;          // 068 // Flag Inconsistencies (Is Extra Part?) (Use Solid Tint?)
	UInt8					pad69[3];       //069
	enum {
		kTypeMisc = 0,
		kTypeFace,
		kTypeEyes,
		kTypeHair,
		kTypeFacialHair,
		kTypeScar,
		kTypeBrows,
		kNumTypes
	};

	enum 
	{
		kFlagPlayable	= 1 << 0,
		kFlagMale		= 1 << 1,
		kFlagFemale		= 1 << 2,
		kFlagExtraPart	= 1 << 3,
		kFlagSolidTint	= 1 << 4
	};

	UInt32					type;           // 06C
	tArray <BGSHeadPart *>	extraParts;		// 070
	BGSTextureSet *			textureSet;		// 088
	TESModelTri				raceMorph;		// 090 - This and next two are part of a fixed array of 3, according to ctor
	TESModelTri				morph;			// 0B8
	TESModelTri				chargenMorph;	// 0E0
	UInt32					unk128;         // 108
	BGSListForm *			validRaces;     // 110
	BSFixedString		partName;		// 118

	bool IsExtraPart() { return (partFlags & kFlagExtraPart) == kFlagExtraPart; }
};

// B0 
class BGSImpactData : public TESForm
{
public:
	enum { kTypeID = kFormType_ImpactData };

	// 24
	struct DecalData
	{
		float	minw;				// 00 - init'd to 8
		float	maxw;				// 04 - init'd to 32
		float	minh;				// 08 - init'd to 8
		float	maxh;				// 0C - init'd to 32
		float	depth;				// 10 - init'd to 32
		float	shininess;			// 14 - init'd to 4
		float	parallaxScale;		// 18 - init'd to 1
		UInt8	parallaxPasses;		// 1C - init'd to 4

		enum
		{
			kDecalFlag_Parallax			= (1 << 0),
			kDecalFlag_AlphaBlending	= (1 << 1),
			kDecalFlag_AlphaTesting		= (1 << 2),
			kDecalFlag_NoSubtextures	= (1 << 3)
		};

		UInt8	flags;		// 1D - init'd to 0
		UInt8	pad1E[2];	// 1E

		UInt32 unk20; // 20 - New in SE . init'd to FFFFFFFF
	};

	// parents
	TESModel	model;		// 20

	// members
	float	duration;			// 48 - init'd to .25

	enum
	{
		kOrientation_SurfaceNormal = 0,
		kOrientation_ProjectileNormal = 1,
		kOrientation_ProjtectileReflection = 2
	};
	UInt32	orientation;		// 4C - init'd to 2
	float	angleThreshold;		// 50 - init'd to 15
	float	placementRadius;	// 54 - init'd to 16
	UInt32	soundLevel;			// 58 - init'd to 1

	enum
	{
		kFlags_NoDecalData = 1
	};

	UInt8	flags;				// 5C
	UInt8	impactResult;		// 5D
	UInt8	pad5E[2];			// 5E

	BGSTextureSet			* textures[2];	// 60 - texture set
	BGSSoundDescriptorForm	* sounds[2];	// 70 - sound
	BGSHazard				* hazard;		// 80 - hazard

	DecalData	unk88;			// 88
	
	UInt32	padAC;			// AC
};

// 58 
class BGSImpactDataSet : public TESForm
{
public:
	enum { kTypeID = kFormType_ImpactDataSet };

	BGSPreloadable	preloadable;	// 20

	UInt32	unk28;	// 28
	UInt32	unk30;	// 30
	UInt32	unk34;	// 34
	UInt32	unk38;	// 38
	UInt32	unk3C;	// 3C
	void*	unk40;	// 40
	UInt32	unk48;	// 48
	void*	unk50;	// 50
};
STATIC_ASSERT(sizeof(BGSImpactDataSet) == 0x58);

// 28 
class BGSKeyword : public TESForm
{
public:
	enum { kTypeID = kFormType_Keyword };

	BSFixedString	keyword;	// 20
};

// 30 
class BGSAction : public BGSKeyword
{
public:
	enum { kTypeID = kFormType_Action };

	UInt32	idx;	// 28 - ctor sets to a unique value
};

// 28 
class BGSLocationRefType : public BGSKeyword
{
public:
	enum { kTypeID = kFormType_LocationRef };
};

// A0 
class BGSLightingTemplate : public TESForm
{
public:
	enum { kTypeID = kFormType_LightingTemplate };

	// 5C
	struct Data
	{
		// 20 - used elsewhere
		struct Color
		{
			UInt32	x[2];				// 00 - init'd to 00FFFFFF
			UInt32	y[2];
			UInt32	z[2];
			UInt32	specular;			// 18 - init'd to 0
			float	fresnelPower;		// 1C - init'd to 1
		};

		UInt32	unk00;			// 00 - init'd to 0
		UInt32	unk04;			// 04 - init'd to 0
		UInt32	unk08;			// 08 - init'd to 0
		UInt32	unk0C;			// 0C - init'd to 0
		UInt32	unk10;			// 10 - init'd to 0
		UInt32	unk14;			// 14 - init'd to 0
		UInt32	unk18;			// 18 - init'd to 0
		float	unk1C;			// 1C - init'd to 1
		UInt32	unk20;			// 20 - init'd to 0
		float	unk24;			// 24 - init'd to 1
		Color	unk28;			// 28
		UInt32	unk48;			// 48 - init'd to 0
		float	unk4C;			// 4C - init'd to 1
		float	lodStartFade;	// 50 - fLightLODStartFade
		float	lodStopFade;	// 54 - fLightLODStartFade + fLightLODRange
		UInt32	unk58;			// 58 - init'd to 0000079F
	};

	Data			unk20;	// 20
	UInt32			pad7C;	// 7C
	Data::Color		unk80;	// 80
};
STATIC_ASSERT(sizeof(BGSLightingTemplate) == 0xA0);

// 48 
class BGSListForm : public TESForm
{
public:
	enum { kTypeID = kFormType_List };

	tArray<TESForm*>	forms;	// 20
	tArray<UInt32> *	addedForms;	// 38
	UInt32	unk40;	// 40

	MEMBER_FN_PREFIX(BGSListForm);
	DEFINE_MEMBER_FN(AddFormToList, void, 0x002C53D0, TESForm * form);
	DEFINE_MEMBER_FN(RemoveFormFromList, void, 0x002C5570, TESForm * form);
	DEFINE_MEMBER_FN(RevertList, void, 0x002C5300);

	class Visitor
	{
	public:
		virtual bool Accept(TESForm * form) = 0;
	};

	bool Visit(BGSListForm::Visitor & visitor);
	UInt32 GetSize();
};

// F0 
class BGSLocation : public TESForm
{
public:
	enum { kTypeID = kFormType_Location };

	struct UnloadedRefData
	{
	public:
		union CellKey
		{
			struct XY
			{
				UInt16 x;
				UInt16 y;
			} xy;
			UInt32	raw;
		};
		STATIC_ASSERT(sizeof(CellKey) == 0x4);


		// members
		UInt32	refID;			// 0
		UInt32	parentSpaceID;	// 4
		CellKey cellKey;		// 8
	};
	STATIC_ASSERT(sizeof(UnloadedRefData) == 0xC);

	struct SpecialRefData  // LCSR
	{
	public:
		// members
		BGSLocationRefType* type;	  // 00
		UnloadedRefData		refData;  // 08
		UInt32				pad14;	  // 14
	};
	STATIC_ASSERT(sizeof(SpecialRefData) == 0x18);

	struct UniqueNPCData  // LCUN
	{
	public:
		// members
		Actor*			actor;	  // 00
		UInt32			refID;	  // 08
		UInt32			pad0C;	  // 0C
		BGSLocation*	editorLoc;  // 10
	};
	STATIC_ASSERT(sizeof(UniqueNPCData) == 0x18);

	struct OverrideData
	{
	public:
		// members
		tArray<UnloadedRefData> addedData;		// 00 - ACPR
		tHashSet<UInt32>		removedData;	// 18 - RCPR
	};
	STATIC_ASSERT(sizeof(OverrideData) == 0x48);

	struct ChangeFlags
	{
		enum ChangeFlag : UInt32
		{
			kKeywordData = 1 << 30,
			kCleared = (UInt32)1 << 31
		};
	};

	struct RecordFlags
	{
		enum RecordFlag : UInt32
		{
			kDeleted = 1 << 5,
			kIgnored = 1 << 12,
			kCleared = (UInt32)1 << 31
		};
	};


	struct KEYWORD_DATA
	{
	public:
		// members
		BGSKeyword*		keyword;	// 00
		float			data;		// 08
		UInt32			pad0C;		// 0C
	};
	STATIC_ASSERT(sizeof(KEYWORD_DATA) == 0x10);

	// parents
	TESFullName		fullName;	// 20
	BGSKeywordForm	keyword;	// 30

	BGSLocation*								parentLoc;				// 48 - PNAM
	TESFaction*									unreportedCrimeFaction;	// 50 - FNAM
	BGSMusicType*								musicType;				// 58 - NAM1
	UInt32										worldLocMarker;			// 60 - MNAM
	float										worldLocRadius;			// 64 - RNAM
	UInt32										horseLocMarker;			// 68 - NAM0
	UInt32										pad6C;					// 6C
	tArray<SpecialRefData>						specialRefs;			// 70 - LCSR
	tArray<UniqueNPCData>						uniqueNPCs;				// 88 - LCUN
	OverrideData*								overrideData;			// A0
	NiPointer<NiObject>							promoteRefsTask;		// A8
	tArray<UInt32>								promotedRefs;			// B0
	SInt32										loadedCount;			// C8
	UInt32										fileOffset;				// CC
	tArray<KEYWORD_DATA>						keywordData;			// D0
	UInt32										lastChecked;			// E8
	bool										cleared;				// EC
	bool										everCleared;			// ED
	UInt16										padEE;					// EE
};
STATIC_ASSERT(sizeof(BGSLocation) == 0xF0);

// B8 
class BGSMaterialObject : public TESForm
{
public:
	enum { kTypeID = kFormType_Material };

	// parents
	TESModel			model;			// 20
	BSMaterialObject	materialObject;	// 48

	// members
	UnkArray	unkA0;	// A0
};

// 50 
class BGSMaterialType : public TESForm
{
public:
	enum { kTypeID = kFormType_MaterialType };

	// C
	struct Data20
	{
		float	unk0;
		float	unk4;
		float	unk8;
	};

	TESForm				* parentType;		// 20 - init'd to 0
	BSFixedString		unk28;				// 28 - init'd to 0
	UInt32				unk30;				// 30 - init'd to 0
	Data20				unk34;				// 34
	float				unk40;				// 40 - init'd to 1
	UInt32				unk44;				// 44 - init'd to 0
	TESForm				* impactDataSet;	// 48 - init'd to 0
};

// 30 
class BGSMenuIcon : public TESForm
{
public:
	enum { kTypeID = kFormType_MenuIcon };

	// parents
	TESIcon	icon;	// 20
};

// 68 
class BGSMessage : public TESForm
{
public:
	enum { kTypeID = kFormType_Message };

	// parents
	TESFullName		fullName;		// 20
	TESDescription	description;	// 30

	// members
	TESForm	* icon;		// 40 - init'd to 0
	TESQuest				* quest;	// 48 - init'd to 0
	tList<BSFixedString>	buttons;	// 50
	UInt32	unk60;		// 60 - init'd to 1
	UInt32	unk64;		// 64 - init'd to 2

	std::string GetDescription()
	{
		BSString str;
		CALL_MEMBER_FN(&description, Get)(&str, this, 'DESC');
		return str.Get();
	};
};

// 60 
class BGSMovementType : public TESForm
{
public:
	enum { kTypeID = kFormType_MovementType };

	enum {
		kType_Walk = 0,
		kType_Run,
		kType_Max
	};

	enum
	{
		kDefaultData_Left = 0,
		kDefaultData_Right,
		kDefaultData_Forward,
		kDefaultData_Back,
		kDefaultData_RotateInPlace,
		kDefaultData_Max
	};

	enum
	{
		kAnimChangeThresholds_Direction = 0,
		kAnimChangeThresholds_MovementSpeed,
		kAnimChangeThresholds_RotationSpeed,
		kAnimChangeThresholds_Max
	};

	BSFixedString	typeId;									// 20
	float				data[kDefaultData_Max][kType_Max];		// 28
	float				rotateWhileMoving;						// 50
	float				thresholds[kAnimChangeThresholds_Max];	// 54 - read from INAM chunk - init'd to 7F7FFFFF, max float
};

// 38 
class BGSMusicTrackFormWrapper : public TESForm
{
public:
	enum { kTypeID = kFormType_MusicTrack };

	// parents
	BSIMusicTrack	track;	// 20

	// members
	UInt64	unk30;			// 30
};

// 78 
class BGSMusicType : public TESForm
{
public:
	enum { kTypeID = kFormType_MusicType };

	// parents
	BSIMusicType		type;	// 20

	// members
	BSFixedString	unk70;	// 70
};

// 38 
class BGSOutfit : public TESForm
{
public:
	enum { kTypeID = kFormType_Outfit };

	tArray<TESForm*>	armorOrLeveledItemArray;	// 20
};

class BGSPerkEntry;

// 80 
class BGSPerk : public TESForm
{
public:
	enum { kTypeID = kFormType_Perk };

	// parents
	TESFullName		fullName;		// 20
	TESDescription	description;	// 30
	TESIcon			icon;			// 40

	class PerkRankVisitor
	{
	public:
		virtual ~PerkRankVisitor();
	};

	// members
	UInt8	unk30;		// 50
	UInt8	unk31;		// 51
	UInt8	unk32;		// 52 - init'd to 1
	UInt8	unk33;		// 53 - init'd to 1
	UInt8	unk34;		// 54
	UInt8	pad55[3];	// 55
	Condition						* conditions;	// 58
	tArray<BGSPerkEntry*>			perkEntries;	// 60
	BGSPerk *						nextPerk;		// 78
};

// 1B8 
class BGSRagdoll : public TESForm
{
public:
	enum { kTypeID = kFormType_Ragdoll };

	// parents
	TESModel	model;	// 20

	// members

	// 58
	struct Data58
	{
		// 3C
		struct Data
		{
			float	unk00;	// 00 - init'd to .9
			float	unk04;	// 04 - init'd to .8
			float	unk08;	// 08 - init'd to .4
			float	unk0C;	// 0C - init'd to .2
			float	unk10;	// 10 - init'd to .1
			float	unk14;	// 14 - init'd to .3
			float	unk18;	// 18 - init'd to 0
			float	unk1C;	// 1C - init'd to 50
			float	unk20;	// 20 - init'd to 50
			float	unk24;	// 24 - init'd to 25
			float	unk28;	// 28 - init'd to 25
			float	unk2C;	// 2C - init'd to 50
			float	unk30;	// 30 - init'd to 50
			UInt32	unk34;	// 34 - init'd to 10000
			UInt32	unk38;	// 38 - init'd to 30000
		};

		UInt8						unk00;		// 00
		UInt8						pad01[3];	// 01
		Data						unk04;		// 04
		UnkArray	unk40;		// 40
	};

	// 50
	struct DataB0
	{
		// 1C
		struct Data
		{
			UInt32	unk00;	// 00
			UInt32	unk04;	// 04
			UInt32	unk08;	// 08
			UInt32	unk0C;	// 0C
			UInt32	unk10;	// 10
			UInt32	unk14;	// 14
			UInt32	unk18;	// 18
		};

		UInt8						unk00;		// 00
		UInt8						pad01[3];	// 01
		Data						unk04;		// 04
		UnkArray	unk20;		// 20
		UnkArray	unk38;		// 38
	};

	// 38
	struct Data100
	{
		UInt8						unk00;		// 00
		UInt8						pad01[7];	// 01
		UnkArray	unk08;		// 08
		UnkArray	unk20;		// 20
	};

	// 30
	struct Data170
	{
		// 18
		struct Data
		{
			UInt16	unk00;	// 00 - init'd to FFFF
			UInt16	unk02;	// 02 - init'd to FFFF
			UInt16	unk04;	// 04 - init'd to FFFF
			UInt8	unk06;	// 06 - init'd to 0
			UInt8	pad07;	// 07
			UInt32	unk08;	// 08 - init'd to 0
			UInt32	unk0C;	// 0C - init'd to 0
			float	unk10;	// 10 - init'd to 0.1
			UInt32	unk14;	// 14 - init'd to 0
		};

		UInt8		unk00;		// 00
		UInt8		pad01[7];	// 01
		BSString	unk08;		// 08
		Data		unk18;		// 18
	};

	UInt8	pad48[0x10];	// 48

	Data58	unk58;			// 58
	DataB0	unkB0;			// B0
	Data100	unk100;			// 100
	Data100	unk138;			// 138
	Data170	unk170;			// 170
	UInt8	unk1A0[8];		// 1A0
	UInt64	unk1A8;			// 1A8
	UInt64	unk1B0;			// 1B0
};
STATIC_ASSERT(sizeof(BGSRagdoll) == 0x1B8);

// 38 
class BGSReferenceEffect : public TESForm
{
public:
	enum { kTypeID = kFormType_ReferenceEffect };

	// C
	struct Data
	{
		UInt64	unk00;
		UInt64	unk08;
		UInt32	unk10;
	};

	Data	unk20; // 20
};

// 40 
class BGSRelationship : public TESForm
{
public:
	enum { kTypeID = kFormType_Relationship };

	UInt64	unk20;	// 20
	UInt64	unk28;	// 28
	UInt64	unk30;	// 30
	UInt32	unk38;	// 38 - init'd to 3
};

// 38 
class BGSReverbParameters : public TESForm
{
public:
	enum { kTypeID = kFormType_ReverbParam };

	BSIReverbType	reverbType;	// 20

	UInt16	unk28;	// 28 - init'd to 1250
	UInt16	unk2A;	// 2A - init'd to 800
	UInt8	unk2C;	// 2C - init'd to 100
	UInt8	unk2D;	// 2D - init'd to 100
	UInt8	unk2E;	// 2E - init'd to 0
	UInt8	unk2F;	// 2F - init'd to 0
	UInt8	unk30;	// 30 - init'd to 100
	UInt8	unk31;	// 31 - init'd to 0
	UInt8	unk32;	// 32 - init'd to 0
	UInt8	unk33;	// 33 - init'd to 100
	UInt32	unk34;	// 34 - ???
};

// C8 
class BGSScene : public TESForm
{
public:
	enum { kTypeID = kFormType_Scene };

	UnkArray	unk20;	// 20
	UnkArray	unk38;	// 38
	UnkArray	unk50;	// 50
	UnkArray	unk68;	// 68
	UnkArray	unk80;	// 80
	TESForm	* quest;	// 98
	UInt32	unkA0;		// A0
	void	* unkA8;	// A8 - linked list
	UInt8	unkB0;		// B0
	UInt8	unkB1;		// B1
	UInt8	unkB2;		// B2
	UInt8	unkB3;		// B3
	UInt8	unkB4;		// B4
	UInt8	unkB5;		// B5
	UInt8	unkB6;		// B6
	UInt8	unkB7;		// B7
	UInt8	unkB8;		// B8
	UInt8	padB9[3];	// B9
	UInt32	unkBC;		// BC - init'd to FFFFFFFF
	float	unkC0;		// C0 - init'd to 7F7FFFFF, max float
};

// 48 
class BGSShaderParticleGeometryData : public TESForm
{
public:
	enum { kTypeID = kFormType_SPGD };

	UnkArray	unk20;		// 20
	TESTexture	texture;	// 38
};

// 58 
class BGSSoundCategory : public TESForm
{
public:
	enum { kTypeID = kFormType_SoundCategory };

	// parents
	TESFullName			fullName;		// 20
	BSISoundCategory	soundCategory;	// 30

	// members
	UInt32	unk38;	// 38 - init'd to 0
	UInt64	unk40;	// 40 - init'd to 0 
	UInt16	unk48;	// 48 - init'd to 0
	UInt16	unk4A;	// 4A - init'd to 0
	UInt16	unk4C;	// 4C - init'd to FFFF
	UInt16	unk4E;	// 4E - init'd to FFFF
	float	unk50;	// 50 - init'd to 1
	float	unk54;	// 54 - init'd to 1
};

// 30 
class BGSSoundDescriptorForm : public TESForm
{
public:
	enum { kTypeID = kFormType_SoundDescriptor };

	virtual void	Unk_39(void);	// calls something on unk18

	// parents
	BGSSoundDescriptor	soundDescriptor;	// 20

	// members
	BGSStandardSoundDef		* standardSoundDef;	// 28
};

// 40 
class BGSSoundOutput : public TESForm
{
public:
	enum { kTypeID = kFormType_SoundOutput };

	// parents
	BSISoundOutputModel	soundOutputModel;	// 20

	// members
	UInt32	unk28;		// 28
	UInt32	unk2C;		// 2C
	UInt64	unk30;		// 30
	void	* unk38;	// 38
};

// 28 
class BGSStoryManagerTreeForm : public TESForm
{
public:
	virtual UInt32	Unk_39(void);
	virtual UInt32	Unk_3A(UInt32 arg);
	virtual void *	Unk_3B(void);	// pure
	virtual void	Unk_3C(void);	// pure

	UInt32	unk20;	// 20
};

class BGSStoryManagerBranchNode;

// 48 
class BGSStoryManagerNodeBase : public BGSStoryManagerTreeForm
{
public:
	BGSStoryManagerBranchNode	* unk28;	// 28
	BGSStoryManagerNodeBase		* unk30;	// 30
	UInt32	unk38;							// 38
	UInt32	unk3C;							// 3C
	void	* unk40;						// 40 - linked list
};

// 60 
class BGSStoryManagerBranchNode : public BGSStoryManagerNodeBase
{
public:
	enum { kTypeID = kFormType_StoryBranchNode };

	UnkArray	unk48;	// 48
};

// 68 
class BGSStoryManagerEventNode : public BGSStoryManagerBranchNode
{
public:
	enum { kTypeID = kFormType_StoryEventNode };

	UInt64	unk60;	// 60
};

// E0 
class BGSStoryManagerQuestNode : public BGSStoryManagerNodeBase
{
public:
	enum { kTypeID = kFormType_StoryQuestNode };


	UnkArray	unk48;			// 48
	UInt8		unk60[0x60];	// 60
	UInt32		unkC0;			// C0
	UnkArray	unkC8;			// C8
};
STATIC_ASSERT(sizeof(BGSStoryManagerQuestNode) == 0xE0);

// 28 
class BGSBaseAlias // Not actually a form, but its used like one in Papyrus
{
public:
	virtual ~BGSBaseAlias();
	virtual void		Unk01(void);
	virtual void		Unk02(void);
	virtual const char	* Unk03(void);

	enum { kTypeID = kFormType_Alias };

	BSFixedString name;	// 08
	TESQuest * owner;		// 10
	UInt32 aliasId;			// 18 - init'd to FFFFFFFF
	UInt32 flags;			// 1C - init'd to 0
	UInt16 unk20;			// 20 - init'd to 0
};

// 48 
class BGSRefAlias : public BGSBaseAlias
{
public:
	enum { kTypeID = kFormType_ReferenceAlias };	

	// One of these is the filltype/filltype filter
	UInt64 unk28[3]; // 28
	void *unk40; // 40
};

// 60 
class BGSLocAlias : public BGSBaseAlias
{
public:
	enum { kTypeID = kFormType_LocationAlias };

	UInt64	unk28; // 28
	UInt32	unk30; // 30
	UInt64	unk38; // 38
	UInt32	unk40; // 40
	UInt32	unk44; // 44
	UInt64	unk48; // 48
	UInt32	unk50; // 50
	void*	unk58; // 58
};

// 268 
class TESQuest : public BGSStoryManagerTreeForm
{
public:
	enum { kTypeID = kFormType_Quest };

	// parents
	TESFullName	fullName;	// 028

	// members

	// 30
	struct Data070
	{
		UInt8	data[0x30];	// ### todo
	};

	// 30
	struct Data0A0
	{
		UInt8	data[0x30];	// ### todo
	};

	// 8
	struct Data0D8
	{
		UInt32	unk0; 
		UInt16	flags; // - init'd to 0x100
		UInt8	priority;
		UInt8	type;
	};

	struct Objective
	{
		StringCache::Ref	displayText; // 0
		TESQuest			* owner; // 8
		void*				unk10; // 10
		UInt32				unk18; //18
		UInt16				index; // 1C
		UInt16				unk1E; // 1E
		UInt64				unk20; // 20
	};

	// 10
	struct Data0E8
	{
		UInt64	unk0;
		UInt64	unk4;
	};

	// 30
	struct Data118
	{
		UInt8	data[0x30];	// ### todo
	};

	UnkArray	unk038;		// 038
	UInt32		unk050;		// 050
	tArray<BGSBaseAlias*>	aliases;		// 058
	Data070		unk070;		// 070
	Data0A0		unk0A0;		// 0A0
	UInt64		unk0D0;		// 0D0
	Data0D8		unk0D8;		// 0D8
	UInt32		unk0E0;		// 0E0 - init'd to FFFFFFFF
	UInt32		pad0E4;		// 0E4
	Data0E8		unk0E8;		// 0E8
	tList<Objective>		objectives; // 0F8
	void		* unk108;	// 108 - linked list
	void		* unk110;	// 110 - linked list
	Data118		unk118[2];		// 118
	UnkArray	unk178[6];	// 178
	UnkArray	unk208;		// 208
	void		* unk220;	// 220 - linked list
	UInt16		unk228;		// 228
	UInt8		unk22A;		// 22A
	UInt8		pad22B[5];	// 22B
	BSString	questID;	// 230
	UInt64		unk144;		// 240
	UInt64		unk148;		// 248
	UnkArray	unk250;		// 250

	MEMBER_FN_PREFIX(TESQuest);
	DEFINE_MEMBER_FN(ForceRefTo, UInt32, 0x00375050, UInt32 aliasId, TESObjectREFR * reference);
	DEFINE_MEMBER_FN(NewGame_Internal, UInt8, 0x00370910, UInt8 * unk1, UInt8 unk2);

	UInt8 NewGame_Hook(UInt8 * unk1, UInt8 unk2);
};
STATIC_ASSERT(sizeof(TESQuest) == 0x268);

// 38 
class BGSVoiceType : public TESForm
{
public:
	enum { kTypeID = kFormType_VoiceType };

	UInt8		unk14;		// 20
	UInt8		pad15[7];	// 21
	BSString	editorId;		// 28
};

// 198 
class EffectSetting : public TESForm
{
public:
	enum { kTypeID = kFormType_EffectSetting };

	// parents
	TESFullName				fullName;			// 20
	BGSMenuDisplayObject	menuDisplayObject;	// 30
	BGSKeywordForm			keywordForm;		// 40

												// members

	// F0
	struct Properties
	{
		enum {
			kEffectType_Hostile = 0x00000001,
			kEffectType_Recover = 0x00000002,
			kEffectType_Detrimental = 0x00000004,
			kEffectType_NoHitEvent = 0x00000010,
			kEffectType_DispelKeywords = 0x00000100,
			kEffectType_NoDuration = 0x00000200,
			kEffectType_NoMagnitude = 0x00000400,
			kEffectType_NoArea = 0x00000800,
			kEffectType_FXPersist = 0x00001000,
			kEffectType_GloryVisuals = 0x00004000,
			kEffectType_HideInUI = 0x00008000,
			kEffectType_NoRecast = 0x00020000,
			kEffectType_Magnitude = 0x00200000,
			kEffectType_Duration = 0x00400000,
			kEffectType_Painless = 0x04000000,
			kEffectType_NoHitEffect = 0x08000000,
			kEffectType_NoDeathDispel = 0x10000000
		};

		enum {
			kArchetype_ValueMod = 0,
			kArchetype_Script,
			kArchetype_Dispel,
			kArchetype_CureDisease,
			kArchetype_Absorb,
			kArchetype_DualValueMod,
			kArchetype_Calm,
			kArchetype_Demoralize,
			kArchetype_Frenzy,
			kArchetype_Disarm,
			kArchetype_CommandSummoned,
			kArchetype_Invisibility,
			kArchetype_Light,
			kArchetype_Lock = 15,
			kArchetype_Open,
			kArchetype_BoundWeapon,
			kArchetype_SummonCreature,
			kArchetype_DetectLife,
			kArchetype_Telekinesis,
			kArchetype_Paralysis,
			kArchetype_Reanimate,
			kArchetype_SoulTrap,
			kArchetype_TurnUndead,
			kArchetype_Guide,
			kArchetype_WerewolfFeed,
			kArchetype_CureParalysis,
			kArchetype_CureAddiction,
			kArchetype_CurePoison,
			kArchetype_Concussion,
			kArchetype_ValueAndParts,
			kArchetype_AccumulateMagnitude,
			kArchetype_Stagger,
			kArchetype_PeakValueMod,
			kArchetype_Cloak,
			kArchetype_Werewolf,
			kArchetype_SlowTime,
			kArchetype_Rally,
			kArchetype_EnhanceWeapon,
			kArchetype_SpawnHazard,
			kArchetype_Etherealize,
			kArchetype_Banish,
			kArchetype_Disguise = 44,
			kArchetype_GrabActor,
			kArchetype_VampireLord
		};

		enum {
			kCastingType_ConstantEffect = 0,
			kCastingType_FireAndForget,
			kCastingType_Concentration
		};

		enum {
			kDeliveryType_Self = 0,
			kDeliveryType_Contact,
			kDeliveryType_Aimed,
			kDeliveryType_TargetActor,
			kDeliveryType_TargetLocation
		};

		enum {
			kVolume_Loud = 0,
			kVolume_Normal,
			kVolume_Silent,
			kVolume_VeryLoud
		};

		UInt32					flags;				// 00 - init'd to 0
		float					baseCost;			// 04 - init'd to 0
		TESForm*				primaryObject;		// 08 - init'd to 0
		UInt32					school;				// 10 - init'd to FFFFFFFF
		UInt32					resistance;			// 14 - init'd to FFFFFFFF
		UInt16					unk18;				// 18 - init'd to 0 
		UInt8					pad1A[6];			// 1A
		TESObjectLIGH*			light;				// 20 - init'd to 0
		float					taperWeight;		// 28 - init'd to 0
		TESEffectShader*		hitShader;			// 30 - init'd to 0
		TESEffectShader*		enchantShader;		// 38 - init'd to 0
		UInt32					level;				// 40 - init'd to 0
		UInt32					area;				// 44 - init'd to 0
		float					castingTime;		// 48 - init'd to 0
		float					taperCurve;			// 4C - init'd to 0
		float					taperDuration;		// 50 - init'd to 0
		float					secondAVWeight;		// 54 - init'd to 0 float - Always 0
		UInt32					archetype;			// 58 - init'd to 0
		UInt32					primaryValue;		// 5C - init'd to FFFFFFFF
		BGSProjectile*			projectile;			// 60 - init'd to 0
		BGSExplosion*			explosion;			// 68 - init'd to 0
		UInt32					castType;			// 70 - init'd to 3 cast type?
		UInt32					deliveryType;		// 74 - init'd to 5 cast type?
		UInt32					secondaryValue;		// 78 - init'd to FFFFFFFF
		BGSArtObject*			castingArt;			// 80 - init'd to 0
		BGSArtObject*			hitEffectArt;		// 88 - init'd to 0
		BGSImpactDataSet*		impactDataSet;		// 90 - init'd to 0
		float					skillUsageMult;		// 98 - init'd to 0
		BGSDualCastData*		dualCastData;		// A0 - init'd to 0
		float					dualCastingScale;	// A8 - init'd to 1
		BGSArtObject*			enchantArt;			// B0 - init'd to 0
		UInt64					unkB8;				// B8 - init'd to 0
		UInt64					unkC0;				// C0 - init'd to 0
		SpellItem*				equipAbility;		// C8 - init'd to 0
		TESImageSpaceModifier*	imageSpaceMod;		// D0 - init'd to 0
		BGSPerk*				perk;				// D8 - init'd to 0
		UInt32					soundVolume;		// E0 - init'd to 1
		float					aiScore;			// E4 - init'd to 0
		float					delayTime;			// E8 - init'd to 0
	};

	struct SoundInfo
	{
		enum
		{
			kSound_DrawSheathe = 0,
			kSound_Charge,
			kSound_Ready,
			kSound_Release,
			kSound_CastLoop,
			kSound_OnHit,
			kNumSounds
		};
		
		UInt32		index; 
		TESSound*	sound;
	};

	UInt64						unk058;		// 58 
	UInt64						unk060;		// 60 
	Properties					properties;		// 68
	tList<EffectSetting>		counterEffects; // 158
	tArray<SoundInfo>			sounds;		// 168
	StringCache::Ref			description; // 180
	UInt32						unk188;		// 188 
	UInt32						unk18C;		// 18C 
	void						* unk190;	// 190 - linked list

	UInt32 school() { return properties.school; }
	UInt32 level() { return properties.level; }
};

#if 0
// SE CONVERSION NOT DONE YET
// B4
class NavMesh : public TESForm
{
public:
	enum { kTypeID = kFormType_NAVM };

	// parents
	TESChildCell	childCell;	// 14
	BSNavmesh		navmesh;	// 18
};

//STATIC_ASSERT(sizeof(NavMesh) == 0xB4);

// SE CONVERSION NOT DONE YET
// 98
class NavMeshInfoMap : public TESForm
{
public:
	enum { kTypeID = kFormType_NAVI };

	// parents
	BSNavmeshInfoMap				infoMap;	// 14
	PrecomputedNavmeshInfoPathMap	pathMap;	// 1C

	// members

	// 20
	struct Data4C
	{
		UInt32	unk00;		// 00
		UInt32	unk04;		// 04
		UInt32	unk08;		// 08 - 0
		UInt32	unk0C;		// 0C - 0
		void	* unk10;	// 10 - ptr to 0xDEADBEEF
		UInt32	unk14;		// 14 - 0
		UInt32	unk18;		// 18
		UInt32	unk1C;		// 1C - 0
	};

	// 20
	struct Data6C
	{
		UInt32	unk00;		// 00
		UInt32	unk04;		// 04
		UInt32	unk08;		// 08 - 0
		UInt32	unk0C;		// 0C - 0
		UInt32	unk10;		// 10 - 0
		void	* unk14;	// 14 - ptr to 0xDEADBEEF
		UInt32	unk18;		// 18
		UInt32	unk1C;		// 1C
	};

	// 8
	struct Data8C
	{
		UInt32	unk0;
		UInt32	unk4;
	};

	UInt8	unk48;		// 48
	UInt8	pad49[3];	// 49
	Data4C	unk4C;		// 4C
	Data6C	unk6C;		// 6C
	Data8C	unk8C;		// 8C
	UInt8	unk94;		// 94
	UInt8	pad95[3];	// 95
};

//STATIC_ASSERT(sizeof(NavMeshInfoMap) == 0x98);
#endif

// 80 
class Script : public TESForm
{
public:
	enum { kTypeID = kFormType_Script };

	// 14
	struct Data
	{
		UInt32	unk00;	// 00
		UInt32	unk04;	// 04 - numRefs?
		UInt32	unk08;	// 08 - dataLength?
		UInt32	unk0C;	// 0C - varCount?
		UInt32	unk10;	// 10 - type?
	};

	// 10
	struct RefListEntry
	{
		void			* data;
		RefListEntry	* next;
	};

	// 10
	struct VarInfoEntry
	{
		void			* data;
		VarInfoEntry	* next;
	};

	Data			unk20;				// 20
	UInt32			pad34;				// 34
	void			* unk38;			// 38
	void			* unk40;			// 40
	UInt32			unk30;				// 48
	float			questDelayTimer;	// 4C
	float			secondsPassed;		// 50
	TESForm			* parentQuest;		// 58
	RefListEntry	refList;			// 60
	VarInfoEntry	varList;			// 70
};

// 78 
class TESClass : public TESForm
{
public:
	enum { kTypeID = kFormType_Class };

	// parents
	TESFullName		fullName;		// 14-20
	TESDescription	description;	// 1C-30
	TESTexture		texture;		// 28-40

	// members

	// 24
	struct Data50
	{
		UInt8	unk00[0x4];		// 00
		UInt8	trainSkill;		// 04 (AV-6)
		UInt8	maxTrainingLevel;	// 05
		UInt8	oneHanded;		// 06
		UInt8	twoHanded;		// 07
		UInt8	archery;
		UInt8	block;
		UInt8	smithing;
		UInt8	heavyArmor;
		UInt8	lightArmor;
		UInt8	pickPocket;
		UInt8	lockpicking;
		UInt8	sneak;
		UInt8	alchemy;
		UInt8	speech;
		UInt8	alteration;
		UInt8	conjuration;
		UInt8	destruction;
		UInt8	illusion;
		UInt8	restoration;
		UInt8	enchanting;
		float	bleedout;		// 18
		UInt32	voicePoints;	// 1C
		UInt8	health;			// 20
		UInt8	magicka;		// 21
		UInt8	stamina;		// 22
		UInt8	unk23;			// 23
	};

	Data50	unk50;				// 50
	UInt32	pad74;				// 74
};
STATIC_ASSERT(sizeof(TESClass) == 0x78);

// 80 
class TESClimate : public TESForm
{
public:
	enum { kTypeID = kFormType_Climate };

	// 10
	struct Data48
	{
		UInt64	unk0;
		UInt64	unk4;
	};

	TESModel	model;			// 20
	Data48		unk48;			// 48
	TESTexture	textures[2];	// 58
	UInt8		unk78[6];		// 78
	UInt8		pad7E[2];		// 7E
};

// A0 
class TESCombatStyle : public TESForm
{
public:
	enum { kTypeID = kFormType_CombatStyle };

	// 28
	struct General	// CSGD
	{
		float	offensiveMult;	// 00 - init'd to 0.5
		float	defensiveMult;	// 04 - init'd to 0.5
		float	groupOffensiveMult;	// 08 - init'd to 1
		float	meleeMult;	// 0C - init'd to 1
		float	magicMult;			// 10 - init'd to 1
		float	rangedMult;			// 14 - init'd to 1
		float	shoutMult;	// 18 - init'd to 1
		float	unarmedMult;			// 1C - init'd to 1
		float	staffMult;		// 20 - init'd to 1
		float	avoidThreatChance;	// 24 - init'd to 0.2
	};

	// 20
	struct Melee	// CSME
	{
		float	attackStaggeredMult;	// 00 - init'd to 1
		float	powerAttackStaggeredMult;	// 04 - init'd to 1
		float	powerAttackBlockingMult;	// 08 - init'd to 1
		float	bashMult;	// 0C - init'd to 1
		float	bashRecoiledMult;	// 10 - init'd to 1
		float	bashAttackMult;	// 14 - init'd to 1
		float	bashPowerAttackMult;	// 18 - init'd to 1
		float	specialAttackMult;	// 1C - init'd to 0.1
	};

	// 10
	struct CloseRange	// CSCR
	{
		float	duelingCircleMult;	// 00 - init'd to 0.2
		float	duelingFallbackMult;	// 04 - init'd to 0.2
		float	flankingFlankDistance;	// 08 - init'd to 0.2
		float	flankingStalkTime;	// 0C - init'd to 0.2
	};

	// 4
	struct LongRange	// CSLR
	{
		float	strafeMult;	// 00 - init'd to 0.2
	};

	// 20
	struct Flight	// CSFL
	{
		float	hoverChance;	// 00 - init'd to 0.5
		float	diveBombChance;	// 04 - init'd to 1
		float	groundAttackChance;	// 08 - init'd to 0.5
		float	hoverTime;			// 0C - init'd to 0.5
		float	unk10;	// 10 - init'd to 0.5
		float	perchAttackChance;	// 14 - init'd to 0.5
		float	unk18;	// 18 - init'd to 0.5
		float	flyingAttackChance;	// 1C - init'd to 0.75
	};

	enum {
		kFlag_Dueling = 1,
		kFlag_Flanking = 2,
		kFlag_AllowDualWielding = 4
	};

	General	general;		// 20 - CSGD
	Melee	melee;		// 48 - CSME
	CloseRange	closeRange;		// 68 - CSCR
	LongRange	longRange;		// 78 - CSLR
	Flight	flight;		// 7C - CSFL
	UInt8	flags;				// 9C - DATA
	UInt8	pad9D[3];	// 9D
};

// 220 
class TESEffectShader : public TESForm
{
public:
	enum { kTypeID = kFormType_EffectShader };

	// 1A0
	struct Data
	{
		UInt8	pad[0x1A0];	// ###
	};

	Data		unk020;	// 020
	TESTexture	unk1C0; // 1C0
	TESTexture	unk1D0; // 1D0
	TESTexture	unk1E0; // 1E0
	TESTexture	unk1F0; // 1F0
	TESTexture	unk200; // 200
	UInt64		unk210;	// 210 BSIntrusiveRefCounted *
	UInt64		unk218;	// 218 BSIntrusiveRefCounted *
};

// 48 
class TESEyes : public TESForm
{
public:
	enum { kTypeID = kFormType_Eyes };

	// parents
	TESFullName	fullName;	// 20
	TESTexture	texture;	// 30

	// members
	UInt8		unk40;		// 40
	UInt8		pad41[7];	// 41
};

// 100 
class TESFaction : public TESForm
{
public:
	enum { kTypeID = kFormType_Faction };

	// parents
	TESFullName		fullName;	// 20
	TESReactionForm	reaction;	// 30

	// members

	// 48
	struct CrimeValues
	{
		UInt64	unk00;				// 00
		UInt64	unk08;				// 08
		UInt64	unk10;				// 10
		UInt64	unk18;				// 18
		UInt64	unk20;				// 20
		UInt64	unk28;				// 28
		bool	arrest;				// 30
		bool	attackOnSight;		// 31
		UInt16	murder;				// 32
		UInt16	assault;			// 34
		UInt16	trespass;			// 36
		UInt16	pickpocket;			// 38
		float	stealMult;			// 3C
		UInt16	escape;				// 40
		UInt16	werewolf;			// 42
		UInt32  pad44;				// 44
	};

	// 38
	struct VendorData
	{
		UInt16			startHour;				// 00
		UInt16			endHour;				// 02
		UInt32			radius;					// 04
		UInt8			onlyBuysStolenItems;	// 08
		UInt8			notSellBuy;				// 09
		UInt8			pad0A[6];				// 0A
		void			* packageLocation;		// 10
		UInt32			unk18;					// 18
		BGSListForm		* buySellList;			// 20
		TESObjectREFR	* merchantContainer;	// 28
		UInt32			unk30;					// 30 - init'd to FFFFFFFF
		UInt32			pad34;					// 34
	};

	// 10
	struct DataE0
	{
		UInt64	unk0;	// 0
		UInt64	unk8;	// 8
	};

	UInt64	unk50;	// 50

	enum
	{
		kFactionFlag_HiddenFromNPC		= (1 << 0),
		kFactionFlag_SpecialCombat		= (1 << 1),
		kFactionFlag_TrackCrime			= (1 << 4),
		kFactionFlag_IgnoreMurder		= (1 << 5),
		kFactionFlag_IgnoreAssult		= (1 << 6),
		kFactionFlag_IngoreStealing		= (1 << 7),
		kFactionFlag_IgnoreTrespass		= (1 << 8),
		kFactionFlag_NoReportCrime		= (1 << 9),
		kFactionFlag_CrimeGoldDefaults	= (1 << 10),
		kFactionFlag_IgnorePickpocket	= (1 << 11),
		kFactionFlag_Vendor				= (1 << 12),
		kFactionFlag_CanBeOwner			= (1 << 13),
		kFactionFlag_IgnoreWerewolf		= (1 << 14)
	};

	UInt32		factionFlags;	// 58
	UInt32		pad5C;			// 5C
	CrimeValues	crimeValues;	// 60
	VendorData	vendorData;		// A8
	DataE0	unkE0;	// E0
	UInt32	unkF0;	// F0
	UInt32	unkF4;	// F4
	float	unkF8;	// F8
	UInt32	unkFC;	// FC
};
STATIC_ASSERT(sizeof(TESFaction) == 0x100);
STATIC_ASSERT(offsetof(TESFaction, vendorData) == 0xA8);

// 38 
class TESGlobal : public TESForm
{
public:
	enum { kTypeID = kFormType_Global };

	BSString	unk20;	// 20
	UInt8		unk1C;	// 30 - type? - init'd to 115
	UInt8		pad[3];	// 31
	UInt32		unk34;	// 34
};

// This was removed at some point in Skyrim32, it doesn't exist in SE either, commenting this
/*
// 50
class TESHair : public TESForm
{
public:
	// parents
	TESFullName	fullName;	// 14
	TESModel	model;		// 1C
	TESTexture	texture;	// 30

	// members
	UInt8		unk38;		// 38
	UInt8		pad39[3];	// 39
	TESModel	unk40;		// 3C
};*/

// 68 
class TESIdleForm : public TESForm
{
public:
	enum { kTypeID = kFormType_Idle };

	void				* unk20;	// 20 - linked list
	UInt8				loopMin;			// 28
	UInt8				loopMax;			// 29
	UInt8				flags;				// 2A
	UInt8				group;				// 2B
	UInt16				unk1C;		// 2C
	UInt8				pad1E[2];	// 2E
	UInt64				unk30;				// 30 - NiFormArray
	TESIdleForm			* relatedIdle1;		// 38
	TESIdleForm			* relatedIdle2;		// 40
	StringCache::Ref	fileName;			// 48
	StringCache::Ref	animationEvent;		// 50
	BSString			editorId;			// 58
};

// 70 
class TESImageSpace : public TESForm
{
public:
	enum { kTypeID = kFormType_ImageSpace };

	// 50
	struct Data20
	{
		float	unk00;	// 00 - init'd to 3
		float	unk04;	// 04 - init'd to 7
		float	unk08;	// 08 - init'd to .6
		float	unk0C;	// 0C - init'd to .5
		float	unk10;	// 10 - init'd to .15
		float	unk14;	// 14 - init'd to .15
		float	unk18;	// 18 - init'd to 1.8
		float	unk1C;	// 1C - init'd to 1.5
		float	unk20;	// 20 - init'd to 3.5, 3
		float	unk24;	// 24 - init'd to 1.8, .8
		float	unk28;	// 28 - init'd to 1.5
		float	unk2C;	// 2C - init'd to 1.1
		float	unk30;	// 30 - init'd to 0
		float	unk34;	// 34 - init'd to 0
		float	unk38;	// 38 - init'd to 0
		float	unk3C;	// 3C - init'd to 0
		float	unk40;	// 40 - init'd to 0
		UInt32	unk44;	// 44 - init'd to 0
		UInt32	unk48;	// 48 - init'd to 0
		float	unk4C;	// 4C - init'd to 2
	};

	Data20	unk20;	// 20
};

// TODO: replace with NiFloatInterpolator when definition available
// 2E0
class TESImageSpaceModifier : public TESForm
{
public:
	enum { kTypeID = kFormType_ImageSpaceMod };

	// F4
	struct Data020
	{
		// 8
		struct Data0CC
		{
			float	unk0;
			float	unk4;
		};

		UInt8	unk000;		// 000
		UInt8	pad001[3];	// 001
		float	unk004;		// 004
							// 008

		UInt8	todo008[0xC8 - 0x08];	// 008

		UInt8	unk0C8;		// 0C8
		UInt8	unk0C9[3];	// 0C9
		Data0CC	unk0CC;		// 0CC
							// 0D4

		UInt8	todo0D4[0xE0 - 0xD4];	// 0D4

		UInt8	unk0E0;		// 0E0
		UInt8	unk0E1;		// 0E1
		UInt8	pad0E2[2];	// 0E2
							// 0E4

		UInt8	todo0E4[0xF4 - 0xE4];	// 0E4
	};

	Data020	unk020;				// 020
	UInt32	pad114;				// 114
	void*	unk118[0x15 * 2];	// 118
	UInt64	unk268[14];			// 268
	UInt16	unk2D8;				// 2D8
	UInt16	unk2DA;				// 2DA
	UInt32	pad2DC;				// 2DC
};
STATIC_ASSERT(sizeof(TESImageSpaceModifier::Data020) == 0xF4);
STATIC_ASSERT(offsetof(TESImageSpaceModifier, pad114) == 0x114);
STATIC_ASSERT(sizeof(TESImageSpaceModifier) == 0x2E0);


// 50 
class TESLandTexture : public TESForm
{
public:
	enum { kTypeID = kFormType_LandTexture };

	// 10
	struct Data40
	{
		UInt64	unk0;	// 0
		UInt64	unk4;	// 8
	};

	UInt64	unk20;		// 20
	UInt8	unk28;		// 28 - init'd to 30
	UInt8	unk29;		// 29 - init'd to 30
	UInt8	pad2A[6];	// 2A
	UInt64	unk30;		// 30
	UInt8	unk38;		// 38 - init'd to 30
	UInt8	pad39[3];	// 39
	UInt32	unk3C;		// 3C
	Data40	unk40;		// 40
};

// 38 
class TESLoadScreen : public TESForm
{
public:
	enum { kTypeID = kFormType_LoadScreen };

	void				* unk20;	// 20 - linked list
	TESObjectSTAT		* object;	// 28
	BSFixedString	unk30;		// 30
};

// 68 
class TESObjectANIO : public TESForm
{
public:
	enum { kTypeID = kFormType_ANIO };

	TESModelTextureSwap	textureSwap;	// 20
	BSFixedString	unk58;			// 58
	BSFixedString	unk60;			// 60
};

// 140 
class TESObjectCELL : public TESForm
{
public:
	enum { kTypeID = kFormType_Cell };

	// parents
	TESFullName					fullName;	// 020

	// members

	// 8
	struct Data
	{
		UInt32	unk0;
		UInt32	unk4;
	};

	// 18
	struct Data048
	{
		BSExtraData	* extraData;;	// 00
		void*	unk00;	// 08
		UInt32	unk10;	// 10
		UInt32	unk14;	// 14
	};

	struct TVDT
	{
		struct TVDT1
		{
			UInt32	* unk00;	// array?
			UInt32	* unk08;	// array?
			UInt32	unk10;		// size?
		};

		struct TVDT0
		{
			TVDT1	* unk00;
			UInt64	unk08;
			UInt32	unk10;	// skip TVDT1 if > 1
		};

		UInt32	unk00;
		UInt32	unk04;
		UInt32	unk08;
		TVDT0	* unk10;
		// ...
	};

	// 28
	struct ReferenceData
	{
		// 10
		struct Reference
		{
			TESObjectREFR	*ref;	// 0 - only valid if unk08 is NOT NULL, ignore value otherwise
			void			*unk08;	// 8 
		};
		
		UInt32	unk00;			// 0
		UInt32	maxSize;		// 4
		UInt32	freeEntries;	// 8 - maxSize - freeEntries = num valid entries (where Reference.unk08 is not NULL)
		UInt32	unk0C;			// 0C
		void	*unk10;			// 10 - Reference.unk08 is usually inititalized to this, but it is not always this
		void	*unk18;			// 18
		Reference*	refArray;	// 20
	};


	Data						unk030;		// 030
	Data						unk038;		// 038
	UInt16						unk040;		// 040  1 - no 060
	UInt16						unk042;		// 042
	UInt8						unk044;		// 044
	UInt8						unk045;		// 045
	UInt8						unk046;		// 046
	UInt8						pad047;		// 047
	
	// ExtraEditorID
	// ExtraCellImageSpace
	// ExtraCellMusicType
	// ExtraLocation
	// ExtraEncounterZone
	// ExtraCellAcousticSpace
	// ExtraSeenData
	// ExtraHavok
	Data048						unk048;				// 048
	TVDT *						* unk060;			// 060 
	void						* unk068;			// 068
	float						waterLevel;			// 070 - init'd to 7F7FFFFFh, max float
	void						* unk078;			// 078

	void						* unk080;			// 080
	ReferenceData				refData;			// 088 - New in SE
	TESForm						* unk0B0;			// 0B0 - REFR owner of cell?

	tArray<TESObjectREFR*> objectList;				// 0B8 - SE: not always valid
	UnkArray					unk0D0;				// 0D0
	UnkArray					unk0F8;				// 0F8
	UnkArray					unk100;				// 100
	Data						unk118;				// 118
	TESWorldSpace				* worldSpace;		// 120
	UInt64						unk128;				// 128
	BGSLightingTemplate			* lightingTemplate;	// 130
	UInt64						unk138;				// 138

	MEMBER_FN_PREFIX(TESObjectCELL);
	DEFINE_MEMBER_FN(GetNorthRotation, double, 0x0026D510);
};
STATIC_ASSERT(offsetof(TESObjectCELL, refData) == 0x88);
STATIC_ASSERT(offsetof(TESObjectCELL, objectList) == 0xB8);
STATIC_ASSERT(sizeof(TESObjectCELL) == 0x140);

// 48 
class TESObjectLAND : public TESForm
{
public:
	enum { kTypeID = kFormType_Land };

	// parents
	TESChildCell	childCell;	// 20

	// members
	UInt32		unk28;	// 28
	UInt64		unk30;	// 30
	UInt64		unk38;	// 38
	UInt64		unk40;	// 40
};

// E0 
class TESPackage : public TESForm
{
public:
	enum { kTypeID = kFormType_Package };

	virtual bool	Unk_39(UInt32 arg0, UInt32 arg1, UInt32 arg2, UInt32 arg3);
	virtual bool	Unk_3A(UInt32 arg0, UInt32 arg1, UInt32 arg2, UInt32 arg3, UInt32 arg4);
	virtual bool	Unk_3B(UInt32 arg0, UInt32 arg1);
	virtual bool	Unk_3C(UInt32 arg0, UInt32 arg1);
	virtual bool	Unk_3D(UInt32 arg0);

	enum
	{
		kPackageType_Find = 0,
		kPackageType_Follow,
		kPackageType_Escort,
		kPackageType_Eat,
		kPackageType_Sleep,
		kPackageType_Wander,
		kPackageType_Travel,
		kPackageType_Accompany,
		kPackageType_UseItemAt,
		kPackageType_Ambush,
		kPackageType_FleeNotCombat,
		kPackageType_CastMagic,
		kPackageType_Sandbox,
		kPackageType_Patrol,
		kPackageType_Guard,
		kPackageType_Dialogue,
		kPackageType_UseWeapon,
		kPackageType_Find2,
		kPackageType_Package,
		kPackageType_PackageTemplate,
		kPackageType_Activate,
		kPackageType_Alarm,
		kPackageType_Flee,
		kPackageType_Trespass,
		kPackageType_Spectator,
		kPackageType_ReactToDead,
		kPackageType_GetUpFromChair,
		kPackageType_DoNothing,
		kPackageType_InGameDialogue,
		kPackageType_Surface,
		kPackageType_SearchForAttacker,
		kPackageType_AvoidPlayer,
		kPackageType_ReactToDestroyedObject,
		kPackageType_ReactToGrenadeOrMine,
		kPackageType_StealWarning,
		kPackageType_PickPocketWarning,
		kPackageType_MovementBlocked,
		kPackageType_Unk37,
		kPackageType_Unk38
	};

	// C
	struct Data50
	{
		UInt8	unk00;		// 0 - init'd to FF
		UInt8	unk01;		// 1 - init'd to FF
		UInt8	unk02;		// 2 - init'd to FF
		UInt8	unk03;		// 3 - init'd to FF
		UInt8	unk04;		// 4 - init'd to FF
		UInt8	pad05[3];	// 5
		UInt32	unk08;		// 8 - init'd to 0
	};

	// 20
	struct Data
	{
		UInt64	unk00;	// 00
		UInt32	unk08;	// 08
		UInt32	pad0C; // 0C
		UInt32	unk10;	// 10
		UInt64	unk18;	// 18
	};

	UInt32	packageFlags;	// 20
	UInt8	type;			// 24 - see kPackageType_ enum
	UInt8	unk25;			// 25
	UInt8	unk26;			// 26
	UInt8	pad27;			// 27
	UInt16	unk28;			// 28
	UInt16	unk2A;			// 2A
	UInt64	unk30;			// 30
	UInt64	unk38;			// 38
	UInt64	unk40;			// 40
	UInt64	unk48;			// 48
	Data50	unk50;			// 50
	UInt32	pad5C;			// 5C
	void	* unk60;		// 60 - linked list
	UInt64	unk68;			// 68
	UInt64	unk70;			// 70
	Data	unk78;			// 78
	Data	unk98;			// 98
	Data	unkB8;			// B8
	UInt32	unkD8;			// D8
	UInt32	unkDC;			// DC - incremented in dtor

	bool IsExtraType();
};

// F8 
class AlarmPackage : public TESPackage
{
public:
	
	UInt64	unkE0;		// E0
	UInt64	unkE8;		// E8
	UInt8	unkF0;		// F0
	UInt8	padF1[7];	// F1
};

// 100 
class DialoguePackage : public TESPackage
{
public:
	UInt32	unkE0;		// E0
	UInt32	unkE4;		// E4
	UInt32	unkE8;		// E8
	UInt32	unkEC;		// EC
	UInt8	unkF0;		// F0
	UInt8	unkF1;		// F1
	UInt8	padF2[2];	// F2
	UInt32	unkF4;		// F4
	UInt32	unkF8;		// F8
	UInt8	unkFC;		// FC
	UInt8	padFD[3];	// FD
};

// 118 
class FleePackage : public TESPackage
{
public:
	struct DataF8
	{
		UInt32	unk0;	// 0
		UInt32	unk4;	// 4
		UInt32	unk8;	// 8
	};

	UnkArray	unk0E0;	// 0E0
	DataF8	unk0F8;		// 0F8
	float	unk104;		// 104 - init'd to -1
	UInt32	unk108;		// 108
	UInt32	unk10C;		// 10C
	UInt8	unk110;		// 110
	UInt8	unk111;		// 111 - init'd to 1
	UInt8	unk112;		// 112
	UInt8	unk113;		// 113
	UInt8	unk114;		// 114
	UInt8	pad115[3];	// 115
};

// 120 
class SpectatorPackage : public TESPackage
{
public:
	struct Data0F4
	{
		UInt32	unk0;	// 0
		UInt32	unk4;	// 4
		UInt32	unk8;	// 8
	};

	UInt32	unk0E0;		// 0E0
	UInt32	pad0E4;		// 0E4 - not init'd
	UInt32	unk0E8;		// 0E8
	UInt32	unk0EC;		// 0EC
	UInt8	unk0F0;		// 0F0
	UInt8	pad0F1[3];	// 0F1
	Data0F4	unk94;		// 0F4
	UnkArray	unk100;	// 100
	UInt64	pad118;		// 118 - not init'd
};

// 100 
class TrespassPackage : public TESPackage
{
public:
	// Warning: either unkE0 or unkE8 is a UInt64, but not both, but it hasn't been confirmed yet which one.
	UInt32	unkE0;	// E0
	UInt32	unkE4;	// E4
	UInt64	unkE8;	// E8
	UInt32	unkF0;	// F0 - init'd to -1
	UInt32	unkF4;	// F4 - init'd to 3
	UInt32	unkF8;	// F8
};

// 4B8 
class TESRace : public TESForm
{
public:
	enum { kTypeID = kFormType_Race };

	enum {
		kRace_Playable = 0x00000001,
		kRace_FaceGenHead = 0x00000002,
		kRace_Child = 0x00000004,
		kRace_TiltFrontBack = 0x00000008,
		kRace_TiltLeftRight = 0x00000010,
		kRace_NoShadow = 0x00000020,
		kRace_Swims = 0x00000040,
		kRace_Flies = 0x00000080,
		kRace_Walks = 0x00000100,
		kRace_Immobile = 0x00000200,
		kRace_NotPushable = 0x00000400,
		kRace_NoCombatInWater = 0x00000800,
		kRace_NoRotatingToHeadTrack = 0x00001000,
		kRace_UseHeadTrackAnim = 0x00008000,
		kRace_SpellsAlignWithMagicNode = 0x00010000,
		kRace_UseWorldRaycasts = 0x00020000,
		kRace_AllowRagdollCollision = 0x00040000,
		kRace_CantOpenDoors = 0x00100000,
		kRace_AllowPCDialogue = 0x00200000,
		kRace_NoKnockdowns = 0x00400000,
		kRace_AllowPickpocket = 0x00800000,
		kRace_AlwaysUseProxyController = 0x01000000,
		kRace_OverlayHeadParts = 0x04000000,
		kRace_OverrideHeadParts = 0x08000000,
		kRace_AllowMultipleMembraneShaders = 0x20000000,
		kRace_CanDualWield = 0x40000000,
		kRace_AvoidsRoads = 0x80000000,
	};

	// parents
	TESFullName			fullName;		// 020
	TESDescription		description;	// 030
	TESSpellList		spellList;		// 040
	BGSSkinForm			skin;			// 050
	BGSBipedObjectForm	biped;			// 060
	BGSKeywordForm		keyword;		// 070
	BGSAttackDataForm	attackData;		// 088

	// members

	enum {
		kRace_NumSkillBonuses = 7
	};

	// AC
	struct Data
	{
		struct SkillBonus
		{
			UInt8 skill;
			UInt8 bonus;
		};
		SkillBonus skillBonus[kRace_NumSkillBonuses];
		UInt8	pad0E[2];			// 0E
		float	height[2];			// 10
		float	weight[2];			// 18
		UInt32	raceFlags;			// 20 - not init'd
		float	health;				// 24
		float	magicka;			// 28
		float	stamina;			// 2C
		float	carryweight;		// 30
		float	mass;				// 34
		float	accelRate;			// 38
		float	deaccelRate;		// 3C
		UInt32	unk40;				// 40 - init'd to 1 - Flags?
		UInt32	unk44;				// 44 - init'd to FFFFFFFF
		UInt32	unk48;				// 48 - init'd to FFFFFFFF - Flags?
		UInt32	unk4C;				// 4C - not init'd
		float	injuredHealthPct;	// 50 - init'd to FFFFFFFF
		float	healthRegen;		// 54
		float	manaRegen;			// 58
		float	staminaRegen;		// 5C
		float	unarmedDamage;		// 60
		float	handReach;			// 64
		UInt32	unk68;				// 68 - init'd to FFFFFFFF - Flags?
		float	aimAngleTolerance;	// 6C
		UInt32	unk70;				// 70 - init'd to 0
		float	angleAcceleration;	// 74
		float	angleTolerance;		// 78
		UInt32	unk7C;			// 7C
		UInt32	unk80;			// 80
		UInt32	unk84;			// 84
		float	unk88;			// 88
		float	unk8C;			// 8C
		float	unk90;			// 90
		float	unk94;			// 94
		float	unk98;			// 98
		float	unk9C;			// 9C
		float	unkA0;			// A0
		float	unkA4;			// A4
		float	unkA8;			// A8
	};

	TESModel					models[2];			// 098
	Data						data;				// 0E8
	BGSTextureModel				textureModel[2];	// 198
	BGSBehaviorGraphModel		behaviorGraph[2];	// 1E8
	StringCache::Ref			behaviorPath[2];	// 238
	StringCache::Ref			behaviorName[2];	// 248
	BGSVoiceType				* voiceTypes[2];	// 258
	BGSBodyPartData				* bodyPartData;		// 268
	TESForm						* decapitateArmor[2];	// 270
	UnkArray					unk280[2]; // 280
	void						* unk2B0[4]; // 2B0
	void						* unk2D0[2]; // 2D0 AttackAnimationArrayMap
	StringCache::Ref			editorId; // 2E0
	BGSMaterialType				* impactMaterial; // 2E8
	BGSImpactDataSet			* meleeImpact; // 2F0
	BGSArtObject				* decapitateBloodArt; // 2F8
	BGSSoundDescriptorForm		* openCorpseSound; // 300
	BGSSoundDescriptorForm		* closeCorpseSound; // 308
	StringCache::Ref			bipedObjectNames[0x20]; // 310
	tArray<BGSEquipSlot*>		slotRestrictions; // 410
	UInt32						unk428; // 428
	BGSEquipSlot				* unarmedEquipSlot; // 430
	TESRace						* morphRace; // 438
	TESRace						* armorRace; // 440
	UnkArray					unk448; // 448
	UnkArray					unk460; // 460
	UInt8						unk478[0x30]; // 478

	 // C8
	struct CharGenData
	{
		struct TintOption
		{
			UInt32					unk00;			// 00 
			TESTexture				texture;		// 08
			BGSColorForm			* defaultColor;	// 18
			tArray<BGSColorForm*>	colors;			// 20
			tArray<float>			alpha;			// 38
			tArray<UInt32>			unk50;			// 50
		};
		UInt32	presetFlags[4][8];					// 00
		UInt32	totalPresets[4];					// 80
		tArray<TintOption*>		* tintData;			// 90
		tArray<BGSTextureSet*>	* textureSet;		// 98
		BGSTextureSet			* defaultTexture;	// A0
		tArray<TESNPC*>			* presets;			// A8
		tArray<BGSColorForm*>	* colors;			// B0
		BGSColorForm			* defaultColor;		// B8
		tArray<BGSHeadPart*>	* headParts;		// C0
	};

	CharGenData					* chargenData[2]; // 4A8
};

// 50 
class TESRegion : public TESForm
{
public:
	enum { kTypeID = kFormType_Region };

	virtual bool	Unk_39(void);

	// 18
	struct Data20
	{
		UInt64	unk00;		// 0
		UInt64	unk08;		// 8
		UInt8	unk10;		// 10
		UInt8	pad11[7];	// 11
	};

	// 10
	struct Data28
	{
		UInt64	unk0;
		UInt64	unk8;
	};

	// C
	struct Data40
	{
		UInt32	unk0;
		UInt32	unk4;
		UInt32	unk8;
	};

	Data20	* unk20;	// 20
	Data28	* unk28;	// 28
	TESWorldSpace	* worldSpace;		// 30
	UInt64	unk38;		// 38
	Data40	unk40;		// 40
};

// A8 
class TESShout : public TESForm
{
public:
	enum { kTypeID = kFormType_Shout };

	// parents
	TESFullName				fullName;		// 20
	BGSMenuDisplayObject	menuDispObject;	// 30
	BGSEquipType			equipType;		// 40
	TESDescription			description;	// 50

	// members

	// 18
	struct Words
	{
		enum {
			kNumWords = 3
		};
		TESWordOfPower *	word; // 0
		SpellItem *			spell; // 8
		float				recoverytime;	// 10
		UInt32				pad14; // 14		
	};

	Words	words[Words::kNumWords];	// 60
};

// 60 
class TESTopic : public TESForm
{
public:
	enum { kTypeID = kFormType_Topic };

	// parents
	TESFullName			fullName;	// 20 

	// members
	UInt8				unk1C[4];	// 30 
	UInt32				unk20;		// 34 - init'd to 0x32000000
	void				*unk38;		// 38
	UInt64				unk40;		// 40
	void				** unk2C;	// 48 - buf[unk50]
	UInt32				unk50;		// 50 - len
	BSFixedString	editorId;		// 58 
};

// 48 
class TESTopicInfo : public TESForm
{
public:
	enum { kTypeID = kFormType_TopicInfo };

	enum
	{
		kDialogFlag_Goodbye =					1 << 0,
		kDialogFlag_Random =					1 << 1,
		kDialogFlag_SayOnce =					1 << 2,
		kDialogFlag_RandomEnd =					1 << 5,
		kDialogFlag_InvisibleContinue =			1 << 6,
		kDialogFlag_ForceSubtitle =				1 << 8,
		kDialogFlag_CanMoveWhileGreeting =		1 << 10,
		kDialogFlag_HasNoLipFile =				1 << 11,
		kDialogFlag_RequiresPostProcessing =	1 << 12,
		kDialogFlag_SpendsFavorPoints =			1 << 14,
	};

	UInt64		unk14;				// 20
	UInt64		unk18;				// 28 - show response data from info?
	Condition	* conditions;		// 30 - linked list
	UInt16		unk20;				// 38 - init'd to FFFF
	UInt8	unk22;					// 3A
	UInt8		favorLevel;			// 3B
	UInt16		dialogFlags;		// 3C
	UInt16		hoursUntilReset;	// 3E
	UInt32		unk40;				// 40
	UInt32		pad44;				// 44
};
STATIC_ASSERT(sizeof(TESTopicInfo) == 0x48);

// 260 
class TESWaterForm : public TESForm
{
public:
	enum { kTypeID = kFormType_Water };

	// parents
	TESFullName			fullName;		// 20
	TESAttackDamageForm	attackDamage;	// 30

	// members

	// E4-E8
	struct Data0D0
	{
		float	unk00;		// 00 - init'd to .1
		float	unk04;		// 04 - init'd to 90
		float	unk08;		// 08 - init'd to .5
		float	unk0C;		// 0C - init'd to 1
		float	unk10;		// 10 - init'd to 50
		float	unk14;		// 14 - init'd to .5
		float	unk18;		// 18 - init'd to .025
		UInt32	unk1C;		// 1C - init'd to 0
		UInt32	unk20;		// 20 - init'd to 0
		UInt32	unk24;		// 24 - init'd to 0
		UInt32	unk28;		// 28 - init'd to 00808000
		UInt32	unk2C;		// 2C - init'd to 00190000
		UInt32	unk30;		// 30 - init'd to 00FFFFFF
		UInt8	unk34;		// 34 - init'd to 0
		UInt8	pad35[3];	// 35
		float	unk38;		// 38 - init'd to 0.1
		float	unk3C;		// 3C - init'd to 0.6
		float	unk40;		// 40 - init'd to 0.985
		float	unk44;		// 44 - init'd to 2
		float	unk48;		// 48 - init'd to .01
		float	unk4C;		// 4C - init'd to .4
		float	unk50;		// 50 - init'd to .6
		float	unk54;		// 54 - init'd to .985
		float	unk58;		// 58 - init'd to 10
		float	unk5C;		// 5C - init'd to .05
		float	unk60;		// 60 - init'd to 300
		UInt32	unk64;		// 64 - init'd to 0
		UInt32	unk68;		// 68 - init'd to 0
		UInt32	unk6C;		// 6C - init'd to 0
		UInt32	unk70;		// 70 - init'd to 0
		UInt32	unk74;		// 74 - init'd to 0
		UInt32	unk78;		// 78 - init'd to 0
		float	unk7C;		// 7C - init'd to 300
		float	unk80;		// 80 - init'd to 300
		float	unk84;		// 84 - init'd to 1
		UInt32	unk88;		// 88 - not init'd
		float	unk8C;		// 8C - init'd to 1
		UInt32	unk90;		// 90 - init'd to 0
		float	unk94;		// 94 - init'd to 1000
		float	unk98;		// 98 - init'd to 250
		float	unk9C;		// 9C - init'd to 100
		float	unkA0;		// A0 - init'd to 1
		float	unkA4;		// A4 - init'd to 10000
		float	unkA8;		// A8 - init'd to 1
		float	unkAC;		// AC - init'd to 100
		float	unkB0;		// B0 - init'd to 100
		float	unkB4;		// B4 - init'd to 100
		UInt32	unkB8;		// B8 - init'd to 0
		UInt32	unkBC;		// BC - init'd to 0
		UInt32	unkC0;		// C0 - init'd to 0
		float	unkC4;		// C4 - init'd to 1
		float	unkC8;		// C8 - init'd to 1
		float	unkCC;		// CC - init'd to 1
		float	unkD0;		// D0 - init'd to 1
		float	unkD4;		// D4 - init'd to 1
		float	unkD8;		// D8 - init'd to 1
		float	unkDC;		// DC - init'd to 1
		float	unkE0;		// E0 - init'd to 1
		float	unkE4;		// E4 - init'd to 1 - New in SE
	};

	UInt8		unk040;		// 040 - init'd to 1
	UInt8		pad041[3];	// 041
	UInt32		unk044;		// 044

	// SE: I had to remove the Data2C structures and extract the variables out because there was no way for the alignment to work
	UInt32		unk048;		// 048
	UInt32		unk04C;		// 04C
	UInt32		unk050;		// 050
	UInt32		unk054;		// 054
	UInt32		unk058;		// 058
	UInt32		unk05C;		// 05C
	UInt32		unk060;		// 060
	UInt32		unk064;		// 064
	UInt32		unk068;		// 068
	UInt32		unk06C;		// 06C
	UInt32		unk070;		// 070
	UInt32		pad074;		// 074	

	TESTexture	unk078;		// 078
	TESTexture	unk088;		// 088
	TESTexture	unk098;		// 098	
	TESTexture	unk0A8;		// 0A8 - New in SE?

	UInt8		unk0B8;		// 0B8 - init'd to 75
	UInt8		unk0B9;		// 0B9
	UInt8		pad0BA[6];	// 0BA
	UInt64		unk0C0;		// 0C0
	UInt64		unk0C8;		// 0C8
	Data0D0		unk0D0;		// 0D0
	UInt64		unk1B8[3];	// 1B8
	UInt32		unk1D0;		// 1D0
	UInt32		unk1D4;		// 1D4
	UInt32		unk1D8;		// 1D8 - init'd to 9
	UInt32		unk1DC;		// 1DC - init'd to 9
	UInt32		unk1E0;		// 1E0 - init'd to 4
	float		unk1E4;		// 1E4 - init'd to 0.5
	float		unk1E8;		// 1E8 - init'd to 2.0
	float		unk1EC;		// 1EC - init'd to 0.5
	float		unk1F0;		// 1F0 - init'd to 0.5
	UInt32		pad1F4;		// 1F4
	UInt64		unk1F8;		// 1F8
	UInt64		unk200;		// 200
	UInt64		unk208;		// 201
	UInt64		unk210;		// 210
	UInt64		unk218;		// 218
	UInt64		unk220;		// 220
	UInt64		unk228;		// 228 - it may be new in SE
	void		* unk230;	// 230 - refcounted ptr
	UInt32		unk238;		// 238
	UInt8		unk23C;		// 23C
	UInt8		pad23D[3];	// 23D
	UInt32		unk240;		// 240

	// SE: extracting data out from old Data1C8 structure, or alignment won't work
	UInt32		unk244;		// 244
	UInt32		unk248;		// 248
	UInt32		unk24C;		// 24C
	UInt32		unk250;		// 250
	UInt32		unk254;		// 254
	UInt32		unk258;		// 258
	UInt32		unk25C;		// 25C
};
STATIC_ASSERT(sizeof(TESWaterForm) == 0x260);

// 8D8 
class TESWeather : public TESForm
{
public:
	enum { kTypeID = kFormType_Weather };

	enum { kNumColorTypes = 17 };
	enum { kNumTimeOfDay = 4 };

	enum ColorTypes {
		kColorType_SkyUpper = 0,
		kColorType_FogNear = 1,
		kColorType_Unk = 2,
		kColorType_Ambient = 3,
		kColorType_Sunlight = 4,
		kColorType_Sun = 5,
		kColorType_Stars = 6,
		kColorType_SkyLower = 7,
		kColorType_Horizon = 8,
		kColorType_EffectLighting = 9,
		kColorType_CloudLODDiffuse = 10,
		kColorType_CloudLODAmbient = 11,
		kColorType_FogFar = 12,
		kColorType_SkyStatics = 13,
		kColorType_WaterMultiplier = 14,
		kColorType_SunGlare = 15,
		kColorType_MoonGlare = 16
	};

	enum TimeOfDay {
		kTime_Sunrise = 0,
		kTime_Day = 1,
		kTime_Sunset = 2,
		kTime_Night = 3
	};

	// 10
	struct ColorType
	{
		UInt32	time[kNumTimeOfDay];
	};

	// 14
	struct General
	{
		//UInt8	unk00[0x13];			// 00
		UInt8	windSpeed;
		UInt8	unk01[2];
		UInt8	transDelta;				// Div 1000
		UInt8	sunGlare;				// Div 256
		UInt8	sunDamage;				// Div 256
		UInt16	unk06;					// precipitation fadein/out
		UInt32	unk08;					// thunder ligting fadein/out/frequency/flags
		UInt32	lightingColor; 
		UInt8	unk10;
		UInt8	windDirection;			// Div (256/360)
		UInt8	windDirRange;			// Div (256/180)
		UInt8	pad13;
	};

	// 20
	struct FogDistance
	{
		float	nearDay;
		float	farDay;				// 10
		float	nearNight;			// 14
		float	farNight;			// 18
		float	powerDay;			// 1C
		float	powerNight;
		float	maxDay;
		float	maxNight;
	};

	// 10
	struct CloudAlpha
	{
		float	time[kNumTimeOfDay];
	};

	TESTexture1024	texture[0x20];		// 020
	UInt8			unk220[0x20];		// 220 - cleared to 0x7F
	UInt8			unk240[0x20];		// 240 - cleared to 0x7F
	UInt8			unk260[0x200];		// 260
	CloudAlpha		cloudAlpha[0x20];	// 460
	UInt32			unk660;				// 660
	General			general;			// 664
	FogDistance		fogDistance;		// 678
	ColorType		colorTypes[kNumColorTypes];		// 698
	TESAIForm::Data	unk7A8;				// 7A8
	UnkArray		unk7B8;				// 7B8
	UInt32			unk7D0;				// 7D0 - not init'd - LNAM unknown
	TESImageSpace	* imageSpaces[kNumTimeOfDay]; // 7D8
	BGSLightingTemplate::Data::Color	directionalAmbient[kNumTimeOfDay];	// 7F8
	TESModel		unk878;	// 878
	TESForm		*lensFlare; // 8A0 - TODO: replace with actual form pointer when a definition is available
	TESForm		*volumetricLightings[kNumTimeOfDay]; // 8A8 - TODO: replace with actual form pointer when a definition is available
	BGSShaderParticleGeometryData	* particleShader;	// 8C8
	BGSReferenceEffect				* referenceEffect;	// 8D0
};
STATIC_ASSERT(sizeof(TESWeather) == 0x8D8);

// 38 
class TESWordOfPower : public TESForm
{
public:
	enum { kTypeID = kFormType_WordOfPower };

	// parents
	TESFullName			fullName;	// 20

	// members
	BSFixedString	word;		// 30
};

// 358
class TESWorldSpace : public TESForm
{
public:
	enum { kTypeID = kFormType_WorldSpace };

	// parents
	TESFullName		fullName;	// 14-20
	
	// members

	// F0
	struct Data250
	{
		// this struct keeps showing up everywhere
		// 30
		struct Entry
		{
			UInt64	unk00;	// 00 - not init'd
			UInt32	unk08;	// 08 - not init'd
			UInt32	unk0C;	// 0C
			UInt32	unk10;	// 10
			UInt32	unk14;	// 14
			void*	unk18;	// 18
			UInt64	unk20;	// 20 - not init'd
			UInt64	unk28;
		};

		Entry	unk00;
		Entry	unk30;
		Entry	unk60;
		Entry	unk90;
		Entry	unkC0;
	};

	struct LandData	// DNAM
	{
		float	defaultLandHeight;	// 0
		float	defaultWaterHeight;	// 4
	};
	STATIC_ASSERT(sizeof(LandData) == 0x8);


	TESModel	unk030;			// 030	

	// TODO: this block may all be part of same thing
	UInt8		unk058[0x18];	// 058
	void*		unk070;			// 070 	
	UInt8		unk078[0x10];	// 078	
	TESObjectCELL*	unk088;		// 088
	void*		unk090;			// 090 
	UInt64		unk098;			// 098

	UInt8		unk0A0;	// 0A0
	UInt8		pad0A1;	// 0A1
	UInt16		unk0A2;	// 0A2
	UInt16		unkA04;	// 0A4
	UInt16		unk0A6; // 0A6
	UInt64		unk0A8; // 0A8
	UInt32		unk0B0;	// 0B0
	UInt32		unk0B4;	// 0B4
	UInt32		unk0B8; // 0B8
	UInt32		unk0BC;	// 0BC

	void*		unk0C0;	// 0C0
	UInt64		unk0C8;	// 0C8
	void*		unk0D0;	// 0D0
	tArray<TESObjectREFR *> unk0D8; // 0D8
	void*		unk0F0;		// 0F0
	TESObjectCELL*	unk0F8;	// 0F8

	UInt64		unk100;	// 100
	UInt32		unk108;	// 108
	UInt32		unk10C;	// 10C
	UInt32		unk110; // 110
	UInt32		unk114;	// 114

	void*		unk118;	// 118
	UInt64		unk120;	// 120
	UInt64		unk128;	// 128
	UInt64		unk130;	// 130
	UInt64		unk138;	// 138
	UInt64		unk140;	// 140
	UInt64		unk148;	// 148
	UInt64		unk150;	// 150
	void*		unk158;	// 158

	BGSLightingTemplate*	interiorLighting;	// 160
	TESWaterForm*			water; // 168
	TESWaterForm*			lodWater;	// 170

	UInt8		unk178[0x20];	// 178

	float		unk198;		// init'd to 50000
	float		unk19C;		// init'd to 80000
	float		unk1A0;		// init'd to 50
	float		unk1A4;		// world map scale?
	float		unk1A8;		// cell X offset?
	float		unk1AC;		// cell Y offset?
	float		unk1B0;		// cell Z offset?
	UInt32		unk1B4;		

	BGSMusicType*	musicType;	// 1B8

	float		unk1C0; // 1C0 
	float		unk1C4;	// 1C4 
	float		unk1C8;	// 1C8
	float		unk1CC;	// 1CC

	UInt64		unk1D0;	// 1D0
	UInt32		unk1D8;	// 1D8
	UInt32		unk1DC;	// 1DC
	UInt32		unk1E0;	// 1E0
	UInt32		unk1E4;	// 1E4

	void*		unk1E8;	// 1E8
	UInt64		unk1F0;	// 1F0
	void*		unk1F8;	// 1F8
	BSString	editorId;	// 200
	LandData	landData;	// 210
	float		unk218;	// 218
	UInt32		pad21C;	// 21C

	BGSEncounterZone*	encounterZone;	// 220
	BGSLocation*		location;		// 228

	TESTexture	unk230;	// 230
	TESTexture	unk240;	// 240

	Data250		unk250;	// 250

	UInt64		unk340;	// 340
	UInt64		unk348;	// 348
	void*		unk350;	// 350
};
STATIC_ASSERT(sizeof(TESWorldSpace) == 0x358);

// 8
class IFormFactory
{
public:
	IFormFactory();
	virtual ~IFormFactory();

	virtual TESForm *		Create(void) = 0;
	virtual const char *	GetName(void) = 0;
	virtual UInt32			GetTypeID(void) = 0;
	virtual const char *	Unk_04(void);	// return NULL
	virtual UInt32			Unk_05(void);	// return 0x67
	virtual UInt32			Unk_06(void);	// return 9

//	void		** _vtbl;	// 00

	static IFormFactory *	GetFactoryForType(UInt32 type)
	{
		if(type > kFormType_Max)
			return NULL;

		// B8146167FE300AED18E0DCE1DA9C86E4C4D08513+11
		static RelocPtr<IFormFactory *> kFactoryList(0x01EC3CE0);

		return kFactoryList[type];
	}
};

// 10
class ConcreteFormFactory : public IFormFactory
{
public:
	const char	* name;		// 08
};
