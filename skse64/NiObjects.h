#pragma once

#include "skse64/NiRTTI.h"
#include "skse64/NiTypes.h"
#include "skse64_common/Utilities.h"
#include "skse64/GameTypes.h"

// NiRefObject/NiObject and important children
// generally other children should go in other files
// especially if they can be grouped

// SE notice: all definitions updated except BSRenderTargetGroup

class NiCloningProcess;
class NiStream;
class NiObjectGroup;
class NiExtraData;
class NiTimeController;
class NiNode;
class NiGeometry;
class BSGeometry;
class NiRenderedTexture;
class NiSwitchNode;
class NiTriBasedGeom;
class NiTriShape;
class NiTriStrips;
class BSSegmentedTriShape;
class NiRenderTargetGroup;
class NiProperty;
class NiSourceTexture;
class BSTriShape;
class TESObjectCELL;
class TESModelTri;
class BSFaceGenMorphData;
class TESObjectREFR;

extern RelocPtr<float> g_worldToCamMatrix;
extern RelocPtr<NiRect<float>> g_viewPort;

typedef bool (* _WorldPtToScreenPt3_Internal)(float * worldToCamMatrix, NiRect<float> * port, NiPoint3 * p_in, float * x_out, float * y_out, float * z_out, float zeroTolerance);
extern RelocAddr<_WorldPtToScreenPt3_Internal> WorldPtToScreenPt3_Internal;

typedef void * (*_NiAllocate)(size_t size);
extern RelocAddr<_NiAllocate> NiAllocate;

typedef void(*_NiFree)(void * ptr);
extern RelocAddr<_NiFree> NiFree;

// 10
class NiRefObject
{
public:
	NiRefObject();
	virtual ~NiRefObject();

	virtual void	DeleteThis(void);	// calls virtual dtor

	void	IncRef(void);
	void	DecRef(void);
	bool	Release(void);

//	void	** _vtbl;		// 00
	volatile SInt32	m_uiRefCount;	// 08
	UInt32	pad0C;	// 0C
};

// ### not all of this is verified, I'm just assuming that little has changed from other
// ### games using previous versions of NetImmerse that have released .pdb files for reference

// 10
class NiObject : public NiRefObject
{
public:
	// standard NetImmerse
	virtual NiRTTI*		GetRTTI(void);

	// then a bunch of attempts to avoid dynamic_cast?
	// unverified, do not use
	virtual NiNode			* GetAsNiNode(void);
	virtual NiSwitchNode	* GetAsNiSwitchNode(void);
	virtual void			* Unk_05(void);
	virtual UInt32			Unk_06(void);
	virtual BSGeometry		* GetAsBSGeometry(void);
	virtual void			* Unk_08(void);
	virtual BSTriShape		* GetAsBSTriShape(void);
	virtual void			* Unk_0A(void);
	virtual void			* Unk_0B(void);
	virtual void			* Unk_0C(void);
	virtual NiGeometry		* GetAsNiGeometry(void);
	virtual NiTriBasedGeom	* GetAsNiTriBasedGeom(void);
	virtual NiTriShape		* GetAsNiTriShape(void);
	virtual void			* Unk_10(void);
	virtual void			* Unk_11(void);
	virtual void			* Unk_12(void);

	// SE: these 4 were added 
	virtual	UInt32			Unk_13(void);
	virtual UInt32			Unk_14(void);
	virtual UInt32			Unk_15(void);
	virtual UInt32			Unk_16(void);

	// then back to NetImmerse
	virtual NiObject *		CreateClone(NiCloningProcess cloner);

	virtual void			LoadBinary(NiStream * stream);
	virtual void			LinkObject(NiStream * stream);
	virtual bool			RegisterStreamables(NiStream * stream);
	virtual void			SaveBinary(NiStream * stream);
	virtual bool			IsEqual(NiObject * object);

	// viewer appears to be disabled sadly
	// why did you do that? it was really useful for figuring out class data
	// and it's not like it totally bloated up the executa... er never mind

	virtual void			ProcessClone(NiCloningProcess * cloner);
	virtual void			PostLinkObject(NiStream * stream);
	virtual bool			StreamCanSkip(void);
	virtual const /*NiRTTI*/void *	GetStreamableRTTI(void) const;
	virtual UInt32			GetBlockAllocationSize(void) const;
	virtual NiObjectGroup *	GetGroup(void) const;
	virtual void			SetGroup(NiObjectGroup * group);

	// begin bethesda extensions? possibly just stuff we can't match up
	virtual UInt32			Unk_20(void);	// SE: function 24

	
	MEMBER_FN_PREFIX(NiObject);
	DEFINE_MEMBER_FN(DeepCopy, NiStream *, 0x00C529A0, NiObject ** result);
};
STATIC_ASSERT(sizeof(NiObject) == 0x10);

// 30
class NiObjectNET : public NiObject
{
public:
	const char	* m_name;	// 10

	NiTimeController	* m_controller;	// 18 next pointer at +0x30

	NiExtraData	** m_extraData;			// 20 extra data
	UInt16		m_extraDataLen;			// 28 max valid entry
	UInt16		m_extraDataCapacity;	// 2A array len
	UInt32		pad2C;

	// UNTESTED
	void AddExtraData(NiExtraData * extraData);
	bool RemoveExtraData(NiExtraData * extraData);
	SInt32 GetIndexOf(NiExtraData * extraData);
	NiExtraData * GetExtraData(BSFixedString name);
};
STATIC_ASSERT(sizeof(NiObjectNET) == 0x30);

// 110
class NiAVObject : public NiObjectNET
{
public:
	enum
	{
		kFlag_SelectiveUpdate =				0x00000002,
		kFlag_UpdatePropertyControllers =	0x00000004,

		kFlag_SelectiveUpdateRigid =		0x00000010,

		kFlag_OverrideSelectiveTransforms =	0x00000080,
	};

	struct ControllerUpdateContext
	{
		enum
		{
			kDirty =	1 << 0,
		};

		float	delta;
		UInt32	flags;
	};

	virtual void	UpdateControllers(ControllerUpdateContext * ctx);	// calls controller vtbl+0x8C
	virtual void	UpdateNodeBound(ControllerUpdateContext * ctx);

	// SE: one of these 4 functions was deleted. Until figuring out which one, assuming it was Unk_26
	virtual void	ApplyTransform(NiMatrix33 * mtx, NiPoint3 * translate, bool postTransform);
	virtual void	SetPropertyState(NiProperty * prop);
	virtual void	Unk_25(UInt32 arg0); // SE: function 29
	//virtual void	Unk_26(UInt32 arg0);


	virtual NiAVObject *	GetObjectByName(const char ** name);	// BSFixedString? alternatively BSFixedString is a typedef of a netimmerse type
	virtual void	SetSelectiveUpdateFlags(bool * selectiveUpdate, bool selectiveUpdateTransforms, bool * rigid);
	virtual void	UpdateDownwardPass(ControllerUpdateContext * ctx, void *unk1); // SE: changed unk to void*
	virtual void	UpdateSelectedDownwardPass(ControllerUpdateContext * ctx, void *unk1); // SE: changed unk to void*
	virtual void	UpdateRigidDownwardPass(ControllerUpdateContext * ctx, void *unk1); // SE: changed unk to void*
	virtual void	UpdateWorldBound(void);
	virtual void	UpdateWorldData(ControllerUpdateContext * ctx);	// SE: function 30 (vtable+0x180)
	virtual void	UpdateNoControllers(ControllerUpdateContext * ctx);
	virtual void	UpdateDownwardPassTempParent(NiNode * parent, ControllerUpdateContext * ctx);
	virtual void	Unk_30(void);	// calls virtual function on parent - SE: function 33

	// SE: one of these functions was deleted. Doesn't matter which, they are unks, assuming Unk_32.
	virtual void	Unk_31(UInt32 arg0); // SE: function 34
	//virtual void	Unk_32(UInt32 arg0);

	NiNode		* m_parent;				// 30
	UInt32		unk038;					// 38 - New in SE, init'd to FFFFFFFF
	UInt32		pad03C;					// 3C
	NiAVObject	* unk040;				// 40
	NiTransform	m_localTransform;		// 48
	NiTransform	m_worldTransform;		// 7C
	NiTransform	m_oldWorldTransform;	// B0 - SE: this one is new
	float		unkE4;					// E4
	float		unkE8;					// E8
	float		unkEC;					// EC
	float		unkF0;					// F0
	UInt32		m_flags;				// F4 - bitfield
	TESObjectREFR*	m_owner;			// F8
	float		unk100;					// 100 - New in SE? init's to 1.0
	UInt32		unk104;					// 104 - New in SE? init'd to 0
	UInt8		unk108;					// 108
	UInt8		unk109;					// 109 - bitfield
	UInt8		pad10A[6];				// 10A

	MEMBER_FN_PREFIX(NiAVObject);
	// 3239A102C6E8818F0FBFEF58A1B6EA724A237258+26
	DEFINE_MEMBER_FN(UpdateNode, void, 0x00C56B50, ControllerUpdateContext * ctx);
};
STATIC_ASSERT(offsetof(NiAVObject, m_localTransform) == 0x48);
STATIC_ASSERT(offsetof(NiAVObject, m_worldTransform) == 0x7C);
STATIC_ASSERT(sizeof(NiAVObject) == 0x110);


MAKE_NI_POINTER(NiAVObject);

// SE: TODO this class needs update, if it still exists
// Bethesda class, unknown name
class BSRenderTargetGroup : public NiObject
{
public:
	virtual ~BSRenderTargetGroup();

	NiRenderTargetGroup * unk08[6];			// 08
	UInt32	unk20;							// 20
	UInt32	unk24;							// 24
	UInt32	unk28;							// 28
	UInt32	unk2C;							// 2C inited to FFFFFFFF
	NiRenderedTexture * renderedTexture[4];	// 30

	static BSRenderTargetGroup *	GetPlayerFaceMask(void)
	{
		return *((BSRenderTargetGroup **)0x00000000);
	}
};

// 20
class BSFaceGenModel : public NiRefObject
{
public:
	struct Data10
	{
		UInt32				unk00;		// 00
		UInt32				pad04;		// 04
		NiAVObject			* unk08;	// 08
		NiAVObject			* unk10;	// 10
		void				* unk18;	// 18
		BSFaceGenMorphData	* unk20;	// 20
	};

	Data10	* unk10;	// 10
	UInt32	unk18;		// 18
	UInt32	pad1C;		// 1C

	MEMBER_FN_PREFIX(BSFaceGenModel);
	DEFINE_MEMBER_FN(ctor, void, 0x003D4070);
	DEFINE_MEMBER_FN(CopyFrom, void, 0x003D4150, BSFaceGenModel * other);
	DEFINE_MEMBER_FN(SetModelData, bool, 0x003D47C0, const char * meshPath, void * unk1, UInt8 unk2);
	DEFINE_MEMBER_FN(ApplyMorph, UInt8, 0x003D4630, BSFixedString * morphName, TESModelTri * triModel, NiAVObject ** headNode, float relative, UInt8 unk1);
};

// 18
class BSFaceGenMorphData : public NiRefObject
{
public:
	void	* unk10;	// 10
	

	MEMBER_FN_PREFIX(BSFaceGenMorphData);
	DEFINE_MEMBER_FN(ApplyMorph, UInt8, 0x003D7860, const char ** morphName, NiAVObject * faceTrishape, float relative, UInt8 unk2);
};

// 20
class BSFaceGenModelMap : public NiRefObject
{
public:
	struct Entry
	{
		BSFaceGenModel	* unk00;	// 00
		UInt64			unk08;		// 08
	};

	Entry	unk10;	// 10
};

// A0
class LoadedAreaBound : public NiRefObject
{
public:
	virtual ~LoadedAreaBound();

	UInt64	unk10[6];	// 10
	UInt64 unk40;		// 40
	TESObjectCELL * cell; // 48
	UInt32 unk50;
	UInt32 unk54;
	UInt32 unk58;
	UInt32 unk5C;
	void* unk60; // 60 - inited 0xDEADBEEF
	UInt64 unk68;
	UInt64 unk70;
	NiPoint3 boundsMax; // 78
	NiPoint3 boundsMin;	// 84
	float unk90;	// 90 - init'd from constructor arg (xmm1)
	float unk94;	// 94 - init'd from constructor arg (xmm2)
	float unk98;	// 98 - init'd from constructor arg (xmm3)
	float unk9C;	// 9C - init'd from constructor last arg (stack)
};
STATIC_ASSERT(sizeof(LoadedAreaBound) == 0xA0);
STATIC_ASSERT(offsetof(LoadedAreaBound, boundsMax) == 0x78);

// 68
class BSFaceGenMorphDataHead : public BSFaceGenMorphData
{
public:

	UInt8	unk18[0x50 - 0x18];	// 18 - 18-4A -> init'd to FF

	UInt64	unk50;
	UInt64	unk58;
	UInt32	unk60;
};
STATIC_ASSERT(sizeof(BSFaceGenMorphDataHead) == 0x68);

// 20
class BSFaceGenMorphDataHair : public BSFaceGenMorphData
{
public:
	UInt32	unk18;
	UInt32	pad;
};
STATIC_ASSERT(sizeof(BSFaceGenMorphDataHair) == 0x20);

// 188
class NiCamera : public NiAVObject
{
public:
	float			m_aafWorldToCam[4][4];	// 110
	NiFrustum		m_frustum;				// 150
	float			m_fMinNearPlaneDist;	// 16C
	float			m_fMaxFarNearRatio;		// 170
	NiRect<float>	m_kPort;				// 174
	float			m_fLODAdjust;			// 184

	bool WorldPtToScreenPt3(NiPoint3 * p_in, float * x_out, float * y_out, float * z_out, float zeroTolerance = 1e-5);
};
STATIC_ASSERT(offsetof(NiCamera, m_frustum) == 0x150);
STATIC_ASSERT(offsetof(NiCamera, m_fMinNearPlaneDist) == 0x16C);
STATIC_ASSERT(offsetof(NiCamera, m_fLODAdjust) == 0x184);
