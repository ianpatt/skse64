#pragma once

#include "skse64_common/Utilities.h"
#include "skse64/GameTypes.h"
#include "skse64/GameUtilities.h"

#include "skse64/PapyrusValue.h"
#include "skse64/PapyrusInterfaces.h"

class IFunction;
class VMIdentifier;
class VMValue;
class VMClassRegistry;
class IFunctionArguments;
class DelayFunctor;


class VMClassFunctionVisitor
{
public:
	virtual bool Accept(IFunction * func) = 0;
};

// ??
class VMClassInfo
{
public:
	SInt32				refCount;	// 00
	UInt32				unk04;		// 04
	StringCache::Ref	name;		// 08
	VMClassInfo			* parent;	// 10
	StringCache::Ref	* unk0C;	// 18
	// ... More

	//UInt32				unk10;		// 10
	//UInt32				unk14;		// 14
	//UInt32				numFuncs;	// 18
	//UInt32				funcPtr;	// 1C

	/*IFunction** GetFunctions() const
	{
		return (IFunction**)(funcPtr + 
			4 * (
					((unk14 >> 10) & 0x3FF) * 10 +
					((unk10 >> 8) & 0x3FF) * 2 +
					((unk10 >> 2) & 0x3F) +
					(unk14 & 0x3FF) * 3
				)
			);
	}

	UInt32 GetNumMemberFunctions() const
	{
		return numFuncs & 0x1FF;
	}

	UInt32 GetNumGlobalFunctions() const
	{
		return (unk14 >> 20) & 0x1FF;
	}

	void Visit(VMClassFunctionVisitor & visitor)
	{
		UInt32 globalFns = GetNumGlobalFunctions();
		UInt32 memberFns = GetNumMemberFunctions();

		IFunction ** funcPtr = GetFunctions();
		for(UInt32 i = 0; i < globalFns + memberFns; i++) {
			IFunction * func = funcPtr[i];
			if(func) {
				if(visitor.Accept(func))
					break;
			}	
		}
	}*/

	void	AddRef(void);
	void	Release(void);

	MEMBER_FN_PREFIX(VMClassInfo);
	DEFINE_MEMBER_FN(Destroy, void, 0x0135FEB0);
	DEFINE_MEMBER_FN(GetVariable, SInt32, 0x01361140, BSFixedString * name);
	// 
	DEFINE_MEMBER_FN(GetFunction, IFunction*, 0x01364880, const char * fnName);
};

// This type is not fully decoded or correctly sized, just enough to use the functor
class VMScriptInstance
{
public:
	UInt32				unk00;
	VMClassInfo			* classInfo;
	void				* unk08;
	UInt32				unk0C;
	UInt32				formId;
	UInt32				unk14;
	UInt32				unk18;
};

// 04
class IForEachScriptObjectFunctor
{
public:
	IForEachScriptObjectFunctor(){};
	virtual ~IForEachScriptObjectFunctor() {};

	// return true to continue
	virtual bool	Visit(VMScriptInstance * arg, void * arg2) { return false; };

//	void	** _vtbl;	// 00
};

// 48+
class VMUnlinkedClassList
{
	virtual ~VMUnlinkedClassList()	{ }
	virtual VMUnlinkedClassList *	Create(void);
	virtual void					SetUnk0C(void * arg);
	virtual bool					Link(StringCache::Ref * className);

//	void	** _vtbl;	// 00
	VMClassRegistry	* unk04;	// 04
	void	* unk08;	// 08
	void	* unk0C;	// 0C - loader
	UInt8	unk10;		// 10
	UInt8	pad11[3];	// 11
	UInt32	unk14;		// 14
	UInt32	unk18;		// 18
	UInt32	unk1C;		// 1C
	void	* unk20;	// 20
	UInt32	unk24;		// 24
	UInt32	unk28;		// 28
	UInt32	unk2C;		// 2C
	void	* unk30;	// 30
	UInt32	unk34;		// 34
	UInt32	unk38;		// 38
	UInt32	unk3C;		// 3C
	void	* unk40;	// 40
	UInt32	unk44;		// 44
};

// 044
class VMStackInfo
{
public:
	virtual ~VMStackInfo();
	
	// more virtual functions ...

	//void**	_vtbl;			// 000
	SInt32		refCount;		// 004
	const char*	rootCallName;	// 008 - not sure if BSFixedString
	const char*	scriptName;		// 00C - not sure if BSFixedString
	// ...

};

// ??
class UnkVMStackData1
{
public:
	UInt32			unk000;		// 000
	void*			unk004;		// 004
	UInt32			unk008;		// 008

	VMStackInfo*	stackInfo;	// 00C
	// ...
};

// ??
class UnkVMStackData2
{
public:
	UInt32		unk000;		// 000 - refCount?
	void*		unk004;		// 004
	UInt32		unk008;		// 008
	UInt32		unk00C;		// 00C

	UnkVMStackData1*	unkData;		// 010
	// ...
};

// 00C
class VMStackTableItem
{
public:
	UInt32				stackId;
	UnkVMStackData2*	data;

	operator UInt32() const	{ return stackId; }

	static inline UInt32 GetHash(UInt32* pStackId)
	{
		UInt32 hash;
		CalculateCRC32_32(&hash, *pStackId);
		return hash;
	}
};

// 4B04
// this does more than hold on to class registrations, but for now that's all we care about
class VMClassRegistry
{
public:
	enum
	{
		kFunctionFlag_NoWait = 0x01	// set this only if your function is thread-safe
	};

	enum
	{
		kLogLevel_Info = 0,
		kLogLevel_Warning,
		kLogLevel_Error,
		kLogLevel_Fatal
	};

	typedef tHashSet<VMStackTableItem,UInt32> StackTableT;

	VMClassRegistry();
	virtual ~VMClassRegistry();

	// ### indices are from 1.5.26

	virtual void	Unk_01(void);
	virtual void	PrintToDebugLog(const char* text, UInt32 stackId, UInt32 logLevel);
	virtual void	Unk_03(void);
	virtual void	Unk_04(void);
	virtual void	Unk_05(void);
	virtual void	Unk_06(void);
	virtual void	Unk_07(void);
	virtual void	RegisterForm(UInt32 typeID, const char * papyrusClassName);
	virtual void	Unk_09(void);
	virtual bool	GetFormTypeClass(UInt32 formType, VMClassInfo ** outClass);
	virtual void	Unk_0B(void);
	virtual void	Unk_0C(void);
	virtual bool	Unk_0D(StringCache::Ref * className, UInt32 * unk);
	virtual void	Unk_0E(void);
	virtual void	Unk_0F(void);
	virtual void	Unk_10(void);
	virtual void	Unk_11(void);
	virtual void	Unk_12(void);
	virtual void	Unk_13(void);
	virtual void	Unk_14(void);
	virtual bool	Unk_15(StringCache::Ref * className, VMIdentifier ** identifier);
	virtual bool	CreateArray(VMValue * value, UInt32 size, VMValue::ArrayData ** unk1);
	virtual void	Unk_17(void);
	virtual void	RegisterFunction(IFunction * fn);
	virtual void	SetFunctionFlagsEx(const char * className, UInt32 unk0, const char * fnName, UInt32 flags);
	virtual void	SetFunctionFlags(const char * className, const char * fnName, UInt32 flags);
	virtual void	VisitScripts(UInt64 handle, IForEachScriptObjectFunctor * functor);
	virtual bool	Unk_1C(UInt64 handle, const char * className, VMIdentifier ** identifier);
	virtual void	Unk_1D(void);
	virtual void	Unk_1E(void);
	virtual void	Unk_1F(void);
	virtual void	Unk_20(void);
	virtual void	Unk_21(void);
	virtual void	Unk_22(void);
	virtual bool	ExtractValue(UInt64 handle, StringCache::Ref * className, SInt32 variableIndex, VMValue * out);	// unk probably script context object?
	virtual void	QueueEvent(UInt64 handle, const StringCache::Ref * eventName, IFunctionArguments * args);
	virtual void	Unk_25(void);
	virtual void	Unk_26(void);
	virtual void	Unk_27(void);
	virtual void	Unk_28(void);
	virtual void	Unk_29(void);
	virtual void	Unk_2A(void);
	virtual void	ResumeStack(UInt32 stackId, VMValue* result);
	virtual void	Unk_2C(void);
	virtual IObjectHandlePolicy *	GetHandlePolicy(void);
	virtual void	Unk_2E(void);
	virtual ObjectBindPolicy *		GetObjectBindPolicy(void);
	virtual void	Unk_30(void);
	virtual void	Unk_31(void);
	virtual void	Unk_32(void);
	virtual void	Unk_33(void);
	virtual void	Unk_34(void);
	virtual void	Unk_35(void);

//	void		** _vtbl;		// 0000
	UInt64		unk0008;		// 0008 - refcount?
	void		** vtbl0008;	// 0010
	void		** vtbl000C;	// 0018
	void		** vtbl0010;	// 0020

	UInt64		unk9310[(0x9310 - 0x20) >> 3];	// 9310

	SimpleLock	stackLock;		// 9318
	StackTableT	allStacks;		// 9320
	StackTableT	waitingStacks;	// 9350

	// More...

	VMStackInfo* GetStackInfo(UInt32 stackId);

	void LogError(const char* message, UInt32 stackId)   { PrintToDebugLog(message, stackId, kLogLevel_Error); }
	void LogWarning(const char* message, UInt32 stackId) { PrintToDebugLog(message, stackId, kLogLevel_Warning); }
};
STATIC_ASSERT(offsetof(VMClassRegistry, stackLock) == 0x9318);
STATIC_ASSERT(offsetof(VMClassRegistry, allStacks) == 0x9320);
STATIC_ASSERT(offsetof(VMClassRegistry, waitingStacks) == 0x9350);

class IStackCallbackSaveInterface
{
public:
	virtual ~IStackCallbackSaveInterface();

	virtual void	Unk_01(void);
	virtual void	Unk_02(void);
};

// 45D0
class SkyrimVM
{
public:
	SkyrimVM();
	virtual ~SkyrimVM();

	virtual void	Unk_01(void);

	enum
	{
		kEventSink_Activate = 0,
		kEventSink_ActiveEffectApplyRemove,
		kEventSink_ActorLocationChange,
		kEventSink_BookRead,
		kEventSink_CellAttachDetach,
		kEventSink_CellFullyLoaded,
		kEventSink_Combat,
		kEventSink_ContainerChanged,
		kEventSink_Death,
		kEventSink_DestructionStateChanged,
		kEventSink_EnterBleedout,
		kEventSink_Equip,
		kEventSink_FormDelete,
		kEventSink_Furniture,
		kEventSink_GrabRelease,
		kEventSink_Hit,
		kEventSink_InitScript,
		kEventSink_LoadGame,
		kEventSink_LockChanged,
		kEventSink_MagicEffectApply,
		kEventSink_MagicWardHit,
		kEventSink_MoveAttachDetach,
		kEventSink_ObjectLoaded,
		kEventSink_ObjectREFRTranslation,
		kEventSink_OpenClose,
		kEventSink_Package,
		kEventSink_PerkEntryRun,
		kEventSink_QuestInit,
		kEventSink_QuestStage,
		kEventSink_Reset,
		kEventSink_ResolveNPCTemplates,
		kEventSink_Scene,
		kEventSink_SceneAction,
		kEventSink_ScenePhase,
		kEventSink_Sell,
		kEventSink_SleepStart,
		kEventSink_SleepStop,
		kEventSink_SpellCast,
		kEventSink_TopicInfo,
		kEventSink_TrackedStats,
		kEventSink_TrapHit,
		kEventSink_Trigger,
		kEventSink_TriggerEnter,
		kEventSink_TriggerLeave,
		kEventSink_UniqueIDChange,
		kEventSink_SwitchRaceComplete,
		kEventSink_PlayerBowShot,
		kEventSink_FastTravelEnd,	// added in 1.5.3
		kEventSink_PositionPlayer,
		kEventSink_Stats,
		kEventSink_NumEvents
	};

//	void						** _vtbl;								// 0000
	IStackCallbackSaveInterface	m_callbackSaveInterface;				// 0008
	void						* eventSinks[kEventSink_NumEvents];		// 0010
	UInt64						unk1A0[(0x200 - 0x1A0) >> 3];			// 01A0
	VMClassRegistry				* m_classRegistry;						// 0200
	UInt8						pad200[0x714 - 0x208];					// 0208
	SimpleLock					m_updateLock;							// 0714
	UInt8						pad71C[0x720 - 0x71C];					// 071C
	UpdateRegistrationHolder	m_updateRegHolder;						// 0720
	UpdateRegistrationHolder	m_updateGameTimeRegHolder;				// 0738

	VMClassRegistry	*	GetClassRegistry(void)	{ return m_classRegistry; }

	UInt32				ClearInvalidRegistrations(void);

	MEMBER_FN_PREFIX(SkyrimVM);

	// Used by Hooks_Papyrus
	// 
	DEFINE_MEMBER_FN(UnregisterFromSleep_Internal, void, 0x00963EA0, UInt64 handle);	// AE: using one function earlier due to inlining
	DEFINE_MEMBER_FN(RevertGlobalData_Internal, bool, 0x0096DB90);
	// 4DF4B0288419C72FBF262CE2DBBAAFE7D9713C51+390
	DEFINE_MEMBER_FN(SaveRegSleepEventHandles_Internal, bool, 0x0096EF50, void * handleReaderWriter, void * saveStorageWrapper);
	// 4F9ECB23ABD5935D4160213D8A06DB7A94E6B988+2FB
	DEFINE_MEMBER_FN(LoadRegSleepEventHandles_Internal, bool, 0x009701F0, void * handleReaderWriter, void * loadStorageWrapper);	// AE: using one function earlier due to inlining

	// 96B4034421813AABF0DBF8B2B00A84CC25FB9CA0+112
	DEFINE_MEMBER_FN(QueueDelayFunctor_Internal, bool, 0x00964D10, void** pFunctor);

	bool QueueDelayFunctor(void** pFunctor)
	{
		return CALL_MEMBER_FN(this, QueueDelayFunctor_Internal)(pFunctor);
	}

	void OnFormDelete_Hook(UInt64 handle);
	void RevertGlobalData_Hook(void);
	bool SaveGlobalData_Hook(void * handleReaderWriter, void * saveStorageWrapper);
	bool LoadGlobalData_Hook(void * handleReaderWriter, void * loadStorageWrapper);
};
STATIC_ASSERT(offsetof(SkyrimVM, m_classRegistry) == 0x200);
STATIC_ASSERT(offsetof(SkyrimVM, m_updateLock) == 0x714);
STATIC_ASSERT(offsetof(SkyrimVM, m_updateRegHolder) == 0x720);
STATIC_ASSERT(offsetof(SkyrimVM, m_updateGameTimeRegHolder) == 0x738);

extern RelocPtr<SkyrimVM*> g_skyrimVM;

// ??
class IFunctionArguments
{
public:
	virtual ~IFunctionArguments()	{ }

	// ??
	struct Output
	{
		UInt32	unk00;		// 00
		UInt32	unk04;		// 04
		VMValue	* m_data;	// 08 - confirmed offset
		UInt64	unk08;		// 10
		UInt32	m_size;		// 18 - confirmed offset

		void	Resize(UInt32 len)	{ CALL_MEMBER_FN(this, Resize)(len); }
		VMValue	* Get(UInt32 idx)	{ return (idx < m_size) ? &m_data[idx] : NULL; }

		MEMBER_FN_PREFIX(Output);
		DEFINE_MEMBER_FN(Resize, bool, 0x0095F5B0, UInt32 len);
	};

	virtual bool	Copy(Output * dst) = 0;
};
