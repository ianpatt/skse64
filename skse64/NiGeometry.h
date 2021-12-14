#pragma once

#include "skse64/NiNodes.h"
#include "skse64/NiProperties.h"

// NiGeometry, NiGeometryData and children
MAKE_NI_POINTER(NiGeometryData);
MAKE_NI_POINTER(NiAdditionalGeometryData);
MAKE_NI_POINTER(NiSkinInstance);
MAKE_NI_POINTER(NiProperty);
MAKE_NI_POINTER(NiSkinData);
MAKE_NI_POINTER(NiSkinPartition);

class NiAdditionalGeometryData;
class NiTriShapeData;
class NiTriStripsData;

struct ID3D11Buffer;

// 138
class NiGeometry : public NiAVObject
{
public:
	virtual void Unk_33(void); // call controller vtbl+0xA0?
	virtual void Unk_34(void); // ret 0
	virtual void Unk_35(void); // same as Unk_33
	virtual void * Unk_36(void); // ret call m_spModelData vtbl+0x9C
	virtual void SetGeometryData(NiGeometryData * unk1); // set and AddRef geometry data
	virtual void * Unk_38(void); // ret call m_spModelData vtbl+0x94
	virtual UInt16 Unk_39(bool unk1); // ??

	NiPropertyPtr		m_spPropertyState;	// 110
	NiPropertyPtr		m_spEffectState;	// 118
	NiGeometryDataPtr	m_spModelData;		// 120
	NiSkinInstancePtr	m_spSkinInstance;	// 128
	UInt64				unk130;				// 130
};

class NiTriBasedGeom : public NiGeometry
{
public:
	// 9D070CBFB7F7B861F270D5D29E210A479261131B+C2
	DEFINE_MEMBER_FN_1(ctor, NiTriBasedGeom *, 0x00CBD740, NiTriShapeData * geometry);
};

class NiTriShape : public NiTriBasedGeom
{
public:
	static NiTriShape * Create(NiTriShapeData * geometry);	
};

class BSSegmentedTriShape : public NiTriShape
{
public:
};

class NiTriStrips : public NiTriBasedGeom
{
public:
	static NiTriStrips * Create(NiTriShapeData * geometry);
};

class BSGeometryData
{
public:
	ID3D11Buffer * buffer1;		// 00
	ID3D11Buffer * buffer2;		// 08
	UInt16		unk10;			// 10
	UInt16		unk12;			// 12
	UInt16		unk14;			// 14
	UInt16		unk16;			// 16
	volatile	UInt32	refCount;	// 18
	UInt16		unk1C;			// 1C
	UInt16		unk1E;			// 1E
	UInt8		* vertices;		// 20
	UInt8		* triangles;	// 28
};

// 158
class BSGeometry : public NiAVObject
{
public:
	virtual ~BSGeometry();

	enum ShapeType
	{
		kShapeType_TriShape = 3,
		kShapeType_Dynamic = 4,
		kShapeType_Particle = 11
	};

	UInt64				unk110;				// 110
	UInt64				unk118;				// 118
	NiPropertyPtr		m_spPropertyState;	// 120
	NiPropertyPtr		m_spEffectState;	// 128
	NiSkinInstancePtr	m_spSkinInstance;	// 130
	BSGeometryData		* geometryData;		// 138
	UInt64				unk140;				// 140
	UInt64				vertexDesc;			// 148
	UInt8				shapeType;			// 150 - type? 3, 4, 11
	UInt8				unk151;				// 151
	UInt16				unk152;				// 152
	UInt32				unk154;				// 154
};

// class 160
class BSTriShape : public BSGeometry
{
public:
	virtual ~BSTriShape();

	UInt16				unk158;				// 158
	UInt16				numVertices;		// 15A
	UInt16				unk15C;				// 15C
	UInt16				unk15D;				// 15D
};

typedef BSTriShape * (*_CreateBSTriShape)();
extern RelocAddr<_CreateBSTriShape> CreateBSTriShape;

// 180
class BSDynamicTriShape : public BSTriShape
{
public:
	void		* pDynamicData;	// 160
	SimpleLock	lock;			// 168
	UInt32		dataSize;		// 170
	UInt32		frameCount;		// 174
	UInt32		unk178;			// 178
	UInt32		unk17C;			// 17C

	// 
	DEFINE_MEMBER_FN_0(ctor, BSDynamicTriShape *, 0x00C9BAA0);
};
STATIC_ASSERT(sizeof(BSDynamicTriShape) == 0x180);

typedef BSDynamicTriShape * (*_CreateBSDynamicTriShape)();
extern RelocAddr<_CreateBSDynamicTriShape> CreateBSDynamicTriShape;

// 48+
class NiGeometryData : public NiObject
{
public:
	enum
	{
		kDataFlag_HasUVs =	1 << 0,
		kDataFlag_HasNBT =	1 << 12,
	};

	enum
	{
		kConsistency_Mutable	= 0,
		kConsistency_Static		= 0x4000,
		kConsistency_Volatile	= 0x8000,
		kConsistency_Mask		= 0xF000
	};

	enum
	{
		kKeep_XYZ		= 1 << 0,
		kKeep_Norm		= 1 << 1,
		kKeep_Color		= 1 << 2,
		kKeep_UV		= 1 << 3,
		kKeep_Indices	= 1 << 4,
		kKeep_BoneData	= 1 << 5,
		kKeep_All		= (kKeep_XYZ | kKeep_Norm | kKeep_Color | kKeep_UV | kKeep_Indices | kKeep_BoneData)
	};

	UInt16	m_usVertices;				// 08
	UInt16	m_usID;						// 0A
	UInt16	m_usDirtyFlags;				// 0C
	UInt16	m_usDataFlags;				// 0E
	NiBound	m_kBound;					// 10
	NiPoint3	* m_pkVertex;			// 20
	NiPoint3	* m_pkNormal;			// 24 - all normals, then all binormals etc
	NiColorA	* m_pkColor;			// 28 - yes really, floats (b g r a)
	NiPoint2	* m_pkTexture;			// 2C
	UInt32	unk30;						// 30
	UInt32	unk34;						// 34
	UInt32	unkInt2;					// 38
	NiAdditionalGeometryDataPtr	m_spAdditionalGeomData;	// 3C
	UInt32	unk40;						// 40
	UInt8	m_ucKeepFlags;				// 44
	UInt8	m_ucCompressFlags;			// 45
	UInt8	hasGeoData;					// 46

	void	AllocateVerts(UInt32 numVerts);
	void	AllocateNormals(UInt32 numVerts);
	void	AllocateNBT(UInt32 numVerts);
	void	AllocateColors(UInt32 numVerts);

	struct Data0
	{
		UInt32	unk00;
		UInt32	unk04;
		UInt32	unk08;
	};
};

// 70
class NiTriBasedGeomData : public NiGeometryData
{
public:
	UInt16	m_usTriangles;			// 68
	UInt16	m_usActiveTriangles;	// 6A
};

// 80
class NiTriShapeData : public NiTriBasedGeomData
{
public:
	UInt32	m_uiTriListLength;		// 70
	UInt16	* m_pusTriList;			// 78
};

// 5C
class BSSharedVertexesTriShapeData : public NiTriShapeData
{
public:
	NiTriShapeData	* m_refData;
	UInt32			unk58;
};

class NiTriStripsData : public NiTriBasedGeomData
{
public:
	UInt16	m_usStrips;
	UInt16	* m_pusStripLengths;
	UInt16	* m_pusStripLists;
};

// 58
class NiTriShapeDynamicData : public NiTriShapeData
{
public:
	struct SharedNormalArray
	{
		UInt16	m_usNumSharedNormals;
		UInt16	* m_pusSharedNormalIndexArray;
	};
	SharedNormalArray * m_pkSharedNormals;	// 54
	UInt16				m_usSharedNormalsArraySize;	// 56
};

enum VertexAttribute : UInt8
{
	VA_POSITION = 0x0,
	VA_TEXCOORD0 = 0x1,
	VA_TEXCOORD1 = 0x2,
	VA_NORMAL = 0x3,
	VA_BINORMAL = 0x4,
	VA_COLOR = 0x5,
	VA_SKINNING = 0x6,
	VA_LANDDATA = 0x7,
	VA_EYEDATA = 0x8,
	VA_COUNT = 9
};

enum VertexFlags : UInt16
{
	VF_VERTEX = 1 << VA_POSITION,
	VF_UV = 1 << VA_TEXCOORD0,
	VF_UV_2 = 1 << VA_TEXCOORD1,
	VF_NORMAL = 1 << VA_NORMAL,
	VF_TANGENT = 1 << VA_BINORMAL,
	VF_COLORS = 1 << VA_COLOR,
	VF_SKINNED = 1 << VA_SKINNING,
	VF_LANDDATA = 1 << VA_LANDDATA,
	VF_EYEDATA = 1 << VA_EYEDATA,
	VF_FULLPREC = 0x400
};

enum VertexMasks : UInt64
{
	DESC_MASK_VERT = 0xFFFFFFFFFFFFFFF0LL,
	DESC_MASK_UVS = 0xFFFFFFFFFFFFFF0FLL,
	DESC_MASK_NBT = 0xFFFFFFFFFFFFF0FFLL,
	DESC_MASK_SKCOL = 0xFFFFFFFFFFFF0FFFLL,
	DESC_MASK_DATA = 0xFFFFFFFFFFF0FFFFLL,
	DESC_MASK_OFFSET = 0xFFFFFF0000000000LL,
	DESC_MASK_FLAGS = ~(DESC_MASK_OFFSET)
};

// 28
class NiSkinPartition : public NiObject
{
public:
	static UInt32 GetVertexAttributeOffset(UInt64 vertexDesc, VertexAttribute attr) {
		return (vertexDesc >> (4 * (UInt8)attr + 2)) & 0x3C;
	}
	static VertexFlags GetVertexFlags(UInt64 vertexDesc) {
		return VertexFlags((vertexDesc & DESC_MASK_OFFSET) >> 44);
	}
	static UInt32 GetVertexSize(UInt64 vertexDesc)
	{
		UInt32 vertexSize = 0;
		VertexFlags flags = GetVertexFlags(vertexDesc);
		if (flags & VF_VERTEX)
		{
			vertexSize += sizeof(float) * 4;
		}
		if (flags & VF_UV)
		{
			vertexSize += sizeof(UInt16) * 2;
		}
		if (flags & VF_UV_2)
		{
			vertexSize += sizeof(UInt16) * 2;
		}
		if (flags & VF_NORMAL)
		{
			vertexSize += sizeof(UInt16) * 2;
			if (flags & VF_TANGENT)
			{
				vertexSize += sizeof(UInt16) * 2;
			}
		}
		if (flags & VF_COLORS)
		{
			vertexSize += sizeof(UInt8) * 4;
		}
		if (flags & VF_SKINNED)
		{
			vertexSize += sizeof(UInt16) * 4 + sizeof(UInt8) * 4;
		}
		if (flags & VF_EYEDATA)
		{
			vertexSize += sizeof(float);
		}
		return vertexSize;
	}

	// 30
	struct TriShape
	{
		struct ID3D11Buffer	* m_VertexBuffer;
		struct ID3D11Buffer	* m_IndexBuffer;
		uint64_t			m_VertexDesc;
		uint32_t			m_RefCount;
		UInt8				* m_RawVertexData;
		UInt16				* m_RawIndexData;
	};

	// 50
	struct Partition
	{
		UInt64		vertexDesc;				// 30
		UInt16		* m_pusBones;			// 00
		float		* m_pfWeights;			// 08
		UInt16		* m_pusVertexMap;		// 10
		UInt8		* m_pucBonePalette;		// 18
		UInt16		* m_pusTriList;			// 20
		UInt16		* m_pusStripLengths;	// 28
		UInt16		m_usVertices;			// 38
		UInt16		m_usTriangles;			// 3A
		UInt16		m_usBones;				// 3C
		UInt16		m_usStrips;				// 3E
		UInt16		m_usBonesPerVertex;		// 40
		float		unk44;					// 44
		TriShape	* shapeData;			// 48

		void	AllocateWeights(UInt32 numVerts);
	};

	UInt32		m_uiPartitions;		// 10
	UInt32		unk14;				// 14
	Partition	* m_pkPartitions;	// 18
	UInt32		vertexCount;		// 20
	UInt32		unk24;				// 24
};

// 48
class NiSkinData : public NiObject
{
public:
	// 08
	struct BoneVertData
	{
		UInt16	m_usVert;	// 00
		UInt8	pad02[2];	// 02
		float	m_fWeight;	// 04?
	};

	// 4C
	struct BoneData
	{
		NiTransform		m_kSkinToBone;		// 00
		NiBound			m_kBound;			// 34
		BoneVertData	* m_pkBoneVertData;	// 44
		UInt16			m_usVerts;			// 48
		UInt8			pad4A[2];			// 4A

		void	AllocateWeights(UInt32 numVerts);
	};

	NiSkinPartition	* m_spSkinPartition;	// 08
	NiTransform		m_kRootParentToSkin;	// 0C
	BoneData		* m_pkBoneData;			// 40
	UInt32			m_uiBones;				// 44
};

STATIC_ASSERT(sizeof(NiSkinData::BoneVertData) == 0x08);

// 88
class NiSkinInstance : public NiObject
{
public:
	NiSkinDataPtr		m_spSkinData;		// 10
	NiSkinPartitionPtr	m_spSkinPartition;	// 18
	NiAVObject			* m_pkRootParent;	// 20
	NiAVObject			** m_ppkBones;		// 28
	
	NiTransform			** m_worldTransforms;// 30
	SInt32				unk38;				// 38
	UInt32				m_uiBoneNodes;		// 3C
	UInt32				numFlags;			// 40
	UInt32				unk44;				// 44
	UInt32 				* flags;			// 48
	UInt32				* unk50;			// 50
	UInt64				unk58;				// 58
	CRITICAL_SECTION	lock;				// 60

	static NiSkinInstance * Create();

	NiSkinInstance * Clone();

	MEMBER_FN_PREFIX(NiSkinInstance);
	DEFINE_MEMBER_FN(Copy, NiSkinInstance*, 0x00C7AD90);
	DEFINE_MEMBER_FN(ctor, NiSkinInstance *, 0x00CA8AD0);
};
STATIC_ASSERT(offsetof(NiSkinInstance, lock) == 0x60);
STATIC_ASSERT(sizeof(NiSkinInstance) == 0x88);

// A0
class BSDismemberSkinInstance : public NiSkinInstance
{
public:
	UInt32	numPartitions;					// 88
	UInt32	unk8C;							// 8C
	UInt32	* partitionFlags;				// 90
	UInt8	unk98;							// 98
	UInt8	pad99[7];						// 99

	static BSDismemberSkinInstance * Create();

	// standalone constructor was inlined
};
STATIC_ASSERT(sizeof(BSDismemberSkinInstance) == 0xA0);
