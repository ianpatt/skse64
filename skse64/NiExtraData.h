#pragma once

#include "skse64/NiTypes.h"
#include "skse64/NiObjects.h"
#include "skse64/GameTypes.h"

class NiGeometryData;

// 8
class BSFaceGenKeyframe
{
public:
	virtual ~BSFaceGenKeyframe();

	virtual void Unk_01(void); // pure
	virtual void Unk_02(void); // pure
	virtual void Unk_03(void); // pure
	virtual void Unk_04(UInt32 unk1); // pure
	virtual void Unk_05(void); // pure
	virtual void Unk_06(void); // pure
	virtual void Unk_07(void); // pure
	virtual void Unk_08(void); // pure
	virtual void Unk_09(void); // pure
	virtual void Unk_0A(void); // pure
	virtual void Unk_0B(void); // pure
	virtual void Unk_0C(void);
	virtual void Unk_0D(void);
};

// 20
class BSFaceGenKeyframeMultiple : public BSFaceGenKeyframe
{
public:
	UInt32	type;	// 08
	UInt32	unk08;	// 0C

	float	* values;	// 10
	UInt32	count;		// 18
	UInt8	isUpdated;	// 1C - Set to 0 for update request
	UInt8	pad1D[3];
};
STATIC_ASSERT(sizeof(BSFaceGenKeyframeMultiple) == 0x20);

// 18
class NiExtraData : public NiObject
{
public:
	NiExtraData();
	~NiExtraData();

	char *	m_pcName;	// 10

	static NiExtraData* Create(UInt32 size, uintptr_t vtbl);
};
STATIC_ASSERT(sizeof(NiExtraData) == 0x18);

// 20
class NiBooleanExtraData : public NiExtraData
{
public:
	NiBooleanExtraData();
	~NiBooleanExtraData();

	bool	m_data;	// 18

	static NiBooleanExtraData * Create(BSFixedString name, bool value);
};

// 20
class NiStringExtraData : public NiExtraData
{
public:
	NiStringExtraData();
	~NiStringExtraData();

	char	* m_pString;	// 18
};

// 20
class NiIntegerExtraData : public NiExtraData
{
public:
	NiIntegerExtraData();
	~NiIntegerExtraData();

	SInt32 m_data;	// 18
};

// 20
class NiFloatExtraData : public NiExtraData
{
public:
	NiFloatExtraData();
	~NiFloatExtraData();

	float m_data;	// 18
};

// 28
class NiBinaryExtraData : public NiExtraData
{
public:
	NiBinaryExtraData();
	~NiBinaryExtraData();

	char		* m_data;	// 18
	UInt32		m_size;		// 20

	static NiBinaryExtraData * Create(BSFixedString name, char * data, UInt32 size);
	void SetData(char * data, UInt32 size);
};

// 28
class NiFloatsExtraData : public NiExtraData
{
public:
	NiFloatsExtraData();
	~NiFloatsExtraData();

	UInt32	m_size;		// 18
	UInt32	pad;		// 1C
	float	* m_data;	// 20
};

// 28
class NiIntegersExtraData : public NiExtraData
{
public:
	NiIntegersExtraData();
	~NiIntegersExtraData();

	UInt32	m_size;		// 18
	UInt32	pad;		// 1C
	SInt32	* m_data;	// 20
};

// 28
class NiStringsExtraData : public NiExtraData
{
public:
	NiStringsExtraData();
	~NiStringsExtraData();

	UInt32	m_size;		// 18
	UInt32	pad;		// 1C
	char	** m_data;	// 20

	static NiStringsExtraData * Create(BSFixedString name, BSFixedString * data, UInt32 size);
	void SetData(BSFixedString * data, UInt32 size);
};

// 28
class NiVectorExtraData : public NiExtraData
{
public:
	NiVectorExtraData();
	~NiVectorExtraData();

	float m_vector[4];	// 18
};

// 1B4-230
class BSFaceGenAnimationData : public NiExtraData
{
public:
	enum {
		kKeyframeType_Expression = 0,
		kKeyframeType_Unk1,
		kKeyframeType_Modifier,
		kKeyframeType_Phoneme,
		kKeyframeType_Custom,
		kKeyframeType_Reset = 255,
		kNumKeyframes = 12
	};
	UInt64	unk018;							// 18
	BSFaceGenKeyframeMultiple	keyFrames[kNumKeyframes];	// 20
	float	unk1A0;							// 1A0
	float	unk1A4;							// 1A4
	float	unk1A8;							// 1A8
	UInt32	unk1AC;							// 1AC
	UInt32	unk1B0;							// 1B0
	UInt32	unk1B4;							// 1B4
	UInt32	unk1B8;							// 1B8
	float	unk1BC;							// 1BC
	float	unk1C0;							// 1C0
	UInt32	pad1C4;							// 1C4
	UInt64	unk1C8;							// 1C8
	UInt8	unk1D0;							// 1D0
	UInt8	unk1D1;							// 1D1
	UInt8	unk1D2;							// 1D2
	UInt8	unk1D3;							// 1D3
	UInt32	unk1D4;							// 1D4
	UInt32	unk1D8;							// 1D8
	UInt32	unk1DC;							// 1DC
	UInt32	unk1E0;							// 1E0
	UInt32	unk1E4;							// 1E4
	UInt32	unk1E8;							// 1E8
	UInt32	unk1EC;							// 1EC
	UInt32	unk1F0;							// 1F0
	UInt32	unk1F4;							// 1F4
	UInt32	unk1F8;							// 1F8
	UInt32	unk1FC;							// 1FC
	UInt32	unk200;							// 200
	float	unk204;							// 204
	float	unk208;							// 208
	float	unk20C;							// 20C
	float	unk210;							// 210
	UInt8	unk214;							// 214
	UInt8	unk215;							// 215 - init'd to 1
	UInt8	unk216;							// 216
	UInt8	unk217;							// 217
	UInt8	unk218;							// 218
	UInt8	unk219;							// 219
	UInt8	unk21A;							// 21A
	UInt8	unk21B;							// 21B
	UInt8	unk21C;							// 21C
	UInt8	unk21D;							// 21D
	UInt8	overrideFlag;					// 21E
	UInt8	unk21F;							// 21F
	UInt64	unk220[2];						// 220

	MEMBER_FN_PREFIX(BSFaceGenAnimationData);
	DEFINE_MEMBER_FN(SetExpression, void, 0x003DD950, UInt32 type, float value);
	DEFINE_MEMBER_FN(Reset, void, 0x003DD250, float value, UInt8 unk1, UInt8 unk2, UInt8 unk3, UInt8 unk4);

	// Alternatives to inlined functions
	void SetPhonome(UInt32 type, float value)
	{
		if (type < 16)
		{
			keyFrames[8].values[type] = value;
			keyFrames[8].isUpdated = true;
		}
	}

	void SetModifier(UInt32 type, float value)
	{
		if (type < 17)
		{
			keyFrames[6].values[type] = value;
			keyFrames[6].isUpdated = true;
		}
	}

	void SetCustom(UInt32 type, float value)
	{
		if (type < 1)
		{
			keyFrames[10].values[type] = value;
			keyFrames[10].isUpdated = true;
		}
	}
};

STATIC_ASSERT(offsetof(BSFaceGenAnimationData, keyFrames) == 0x20);
STATIC_ASSERT(offsetof(BSFaceGenAnimationData, overrideFlag) == 0x21E);
STATIC_ASSERT(sizeof(BSFaceGenAnimationData) == 0x230);


// 60 (FMD)
class BSFaceGenModelExtraData : public NiExtraData
{
public:
	BSFaceGenModel* m_model;	// 18
	BSFixedString bones[8];		// 20
	
};
STATIC_ASSERT(sizeof(BSFaceGenModelExtraData) == 0x60);

// 28 (FOD)
class BSFaceGenBaseMorphExtraData : public NiExtraData
{
public:
	NiPoint3 * vertexData;		// 18
	UInt32	modelVertexCount;	// 20 - Same as 14 if the tri model is correct
	UInt32	vertexCount;		// 24

	static BSFaceGenBaseMorphExtraData* Create(NiGeometryData * data, bool copy = false);
};
