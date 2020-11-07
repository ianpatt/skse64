#pragma once

#include "skse64_common/Utilities.h"

class TESObjectREFR;
class Script;
class ScriptLocals;
class ScriptLineBuffer;
class ScriptBuffer;

// 0C
struct ObScriptParam
{
	enum Type
	{
		kType_String = 0x0,
		kType_Integer = 0x1,
		kType_Float = 0x2,
		kType_ObjectReference = 0x4,
		kType_ActorValue = 0x5,
		kType_Actor = 0x6,
		kType_SpellItem = 0x7,
		kType_Axis = 0x8,
		kType_Cell = 0x9,
		kType_AnimationGroup = 0x0A,
		kType_MagicItem = 0x0B,
		kType_Sound = 0x0C,
		kType_Topic = 0x0D,
		kType_Quest = 0x0E,
		kType_Race = 0x0F,
		kType_Class = 0x10,
		kType_Faction = 0x11,
		kType_Sex = 0x12,
		kType_Object = 0x15,
		kType_VariableName = 0x16,
		kType_QuestStage = 0x17,
		kType_MapMarker = 0x18,
		kType_ActorBase = 0x19,
		kType_Coontainer = 0x1A,
		kType_WorldSpace = 0x1B,
		kType_CrimeType = 0x1C,
		kType_Package = 0x1D,
		kType_MagicEffect = 0x1F,
		kType_Weather = 0x21,
		kType_Owner = 0x23,
		kType_EffectShader = 0x24,
		kType_FormList = 0x25,
		kType_Perk = 0x27,
		kType_Note = 0x28,
		kType_MiscellaneousStat = 0x29,
		kType_ImagespaceModifier = 0x2A,
		kType_EventFunction = 0x2E,
		kType_EventMember = 0x2F,
		kType_EventData = 0x30,
		kType_Voice = 0x31,
		kType_EncounterZone = 0x32,
		kType_Message = 0x34,
		kType_ObjectID = 0x35,
		kType_Alignment = 0x36,
		kType_EquipType = 0x37,
		kType_Music = 0x39,
		kType_Keyword = 0x3B,
		kType_RefType = 0x3C,
		kType_Location = 0x3D,
		kType_Form = 0x3E,
		kType_QuestAlias = 0x3F,
		kType_Shout = 0x40,
		kType_WordOfPower = 0x41,
		kType_Scene = 0x43,
		kType_Hand = 0x44,
		kType_AssociationType = 0x45,
		kType_WardState = 0x46,
		kType_PackageData_PossiblyNull = 0x47,
		kType_PackageData_Numeric = 0x48,
		kType_FurnitureAnimType = 0x49,
		kType_FurnitureEntryType = 0x4A,
		kType_VMVariableName = 0x4C,
		kType_PackageData = 0x4E,
		kType_KnowableForm = 0x51,
		kType_Region = 0x52
	};


	const char	* typeStr;	// 00
	UInt32		typeID;		// 04
	UInt32		isOptional;	// 08
};

struct ScriptData
{
	// members
	UInt16	opcode;		// 00
	UInt16	chunkSize;	// 02
	UInt16	numParams;	// 04
};

typedef bool (* ObScript_Execute)(const ObScriptParam * paramInfo, ScriptData * scriptData, TESObjectREFR * thisObj, TESObjectREFR* containingObj, Script* scriptObj, ScriptLocals* locals, double& result, UInt32& opcodeOffsetPtr);
typedef bool (* ObScript_Parse)(UInt32 numParams, const ObScriptParam* paramInfo, ScriptLineBuffer* lineBuf, ScriptBuffer* scriptBuf);
typedef bool (* ObScript_Eval)(TESObjectREFR* thisObj, void* arg1, void* arg2, double& result);

typedef bool(* _ObScript_ExtractArgs)(const ObScriptParam * paramInfo, ScriptData * scriptData, UInt32& opcodeOffsetPtr, TESObjectREFR * thisObj, TESObjectREFR* containingObj, Script* scriptObj, ScriptLocals* locals, void* args1, void* args2);
extern RelocAddr<_ObScript_ExtractArgs> ObjScript_ExtractArgs;

// 02E8400

// 50
struct ObScriptCommand
{
	const char			* longName;		// 00
	const char			* shortName;	// 08
	UInt32				opcode;			// 10
	UInt32				pad14;			// 14
	const char			* helpText;		// 18
	UInt8				needsParent;	// 20
	UInt8				pad21;			// 21
	UInt16				numParams;		// 22
	UInt32				pad24;			// 24
	ObScriptParam		* params;		// 28

	// handlers
	ObScript_Execute	execute;		// 30
	ObScript_Parse		parse;			// 38
	ObScript_Eval		eval;			// 40

	UInt32				flags;			// 48
	UInt32				pad4C;			// 4C
};

enum
{
	kObScript_NumObScriptCommands =	0x02E0,
	kObScript_NumConsoleCommands =	0x01B4,

	kObScript_ScriptOpBase =		0x1000,
	kObScript_ConsoleOpBase =		0x0100,
};

extern RelocPtr <ObScriptCommand>	g_firstObScriptCommand;
extern RelocPtr <ObScriptCommand>	g_firstConsoleCommand;
