#pragma once

#include "skse64/NiObjects.h"
#include "skse64/NiTypes.h"

/**** hierarchy ****************************************************************
 *	
 *	NiInterpolator
 *		NiBlendInterpolator
 *			NiBlendTransformInterpolator
 *			NiBlendFloatInterpolator
 *			NiBlendAccumTransformInterpolator
 *			BSBlendTreadTransfInterpolator
 *			NiBlendQuaternionInterpolator
 *			NiBlendPoint3Interpolator
 *			NiBlendColorInterpolator
 *			NiBlendBoolInterpolator
 *		NiKeyBasedInterpolator
 *			NiFloatInterpolator
 *			NiColorInterpolator
 *			NiPathInterpolator
 *			NiTransformInterpolator
 *				BSRotAccumTransfInterpolator
 *			BSTreadTransfInterpolator
 *			NiQuaternionInterpolator
 *			NiPoint3Interpolator
 *			NiBoolInterpolator
 *				NiBoolTimelineInterpolator
 *		NiLookAtInterpolator
 *		NiBSplineInterpolator
 *			NiBSplineTransformInterpolator
 *				NiBSplineCompTransformInterpolator
 *			NiBSplinePoint3Interpolator
 *				NiBSplineCompPoint3Interpolator
 *			NiBSplineFloatInterpolator
 *				NiBSplineCompFloatInterpolator
 *			NiBSplineColorInterpolator
 *				NiBSplineCompColorInterpolator
 *	
 ****/

// NiInterpolator and children

class NiObjectNET;
class NiQuaternion;
class NiPoint3;
class NiColorA;
class NiQuatTransform;
class NiAVObjectPalette;
class NiAnimationKey;
class NiFloatData;
class NiColorData;
class NiPosData;

// 0C
class NiInterpolator : public NiObject
{
public:
	// work functions

	virtual bool	Update(float delta, NiObjectNET * target, NiQuatTransform * data);
	virtual bool	Update(float delta, NiObjectNET * target, NiColorA * data);
	virtual bool	Update(float delta, NiObjectNET * target, NiPoint3 * data);
	virtual bool	Update(float delta, NiObjectNET * target, NiQuaternion * data);
	virtual bool	Update(float delta, NiObjectNET * target, float * data);
	virtual bool	Update(float delta, NiObjectNET * target, bool * data);

	// query functions
	virtual bool	IsBoolValueSupported(void) const;
	virtual bool	IsFloatValueSupported(void) const;
	virtual bool	IsQuaternionValueSupported(void) const;
	virtual bool	IsPoint3ValueSupported(void) const;
	virtual bool	IsColorAValueSupported(void) const;
	virtual bool	IsTransformValueSupported(void) const;
	
	virtual void	Collapse();	// ??
	virtual void	GetActiveTimeRange(float * start, float * end) const;
	virtual void	GuaranteeTimeRange(float start, float end);
	virtual NiInterpolator *	GetSequenceInterpolator(float start, float end);
	virtual bool	ResolveDependencies(NiAVObjectPalette * palette);
	virtual bool	SetUpDependencies(void);
	virtual bool	AlwaysUpdate(void) const;
	virtual UInt32	Unk_34(void);

	float	m_fLastTime;	// 08
};

// 30
class NiBlendInterpolator : public NiInterpolator
{
public:
	virtual UInt8	AddInterpInfo(NiInterpolator * interpolator, float weight, SInt8 priority, float easeSpinner);
	virtual NiPointer <NiInterpolator>	RemoveInterpInfo(UInt8 idx);
	virtual bool	InterpolatorIsCorrectType(NiInterpolator * interpolator);	// pure virtual
	virtual bool	ReallocateArrays(void);

	// 18
	struct InterpArrayItem
	{
		NiPointer <NiInterpolator>	m_spInterpolator;	// 00
		float	m_fWeight;								// 04
		float	m_fNormalizedWeight;					// 08
		UInt8	m_cPriority;							// 0C
		float	m_fEaseSpinner;							// 10
		float	m_fUpdateTime;							// 14
	};

	UInt8	m_uFlags;				// 0C
	UInt8	m_ucArraySize;			// 0D
	UInt8	m_ucInterpCount;		// 0E
	UInt8	m_ucSingleIdx;			// 0F
	SInt8	m_cHighPriority;		// 10
	SInt8	m_cNextHighPriority;	// 11
	UInt8	pad12[2];				// 12

	InterpArrayItem	* m_pkInterpArray;	// 14
	NiInterpolator	* m_pkSingleInterpolator;	// 18

	float	m_fWeightThreshold;			// 1C
	float	m_fSingleTime;				// 20
	float	m_fHighSumOfWeights;		// 24
	float	m_fNextHighSumOfWeights;	// 28
	float	m_fHighEaseSpinner;			// 2C
};

// 30
class NiBlendTransformInterpolator : public NiBlendInterpolator
{
public:
};

// 34
class NiBlendFloatInterpolator : public NiBlendInterpolator
{
public:
	float	m_fFloatValue;	// 30
};

// 58
class NiBlendAccumTransformInterpolator : public NiBlendInterpolator
{
public:
	UInt8	unk30[0x58 - 0x30];	// 30
};

// 34
class BSBlendTreadTransfInterpolator : public NiBlendInterpolator
{
public:
	float	unk30;	// 30
};

// 40
class NiBlendQuaternionInterpolator : public NiBlendInterpolator
{
public:
	NiQuaternion	m_kQuaternionValue;	// 30
};

// 40
class NiBlendPoint3Interpolator : public NiBlendInterpolator
{
public:
	NiPoint3	m_kPoint3Value;	// 30
	bool		m_bValidValue;	// 3C
};

// 44
class NiBlendColorInterpolator : public NiBlendInterpolator
{
public:
	NiColorA	m_kColorValue;	// 30
	bool		m_bValidValue;	// 40
};

// 34
class NiBlendBoolInterpolator : public NiBlendInterpolator
{
public:
	bool	m_bBoolValue;	// 30
};

// 0C
class NiKeyBasedInterpolator : public NiInterpolator
{
public:
	virtual UInt16				GetKeyChannelCount(void) const;
	virtual UInt32				GetKeyCount(UInt16 channel) const;
	virtual UInt32				GetKeyType(UInt16 channel) const;
	virtual UInt32				GetKeyContent(UInt16 channel) const;
	virtual NiAnimationKey *	GetKeyArray(UInt16 channel) const;
	virtual UInt8				GetKeyStride(UInt16 channel) const;

	virtual bool	GetChannelPosed(UInt16 channel) const;
};

// 18
class NiFloatInterpolator : public NiKeyBasedInterpolator
{
public:
	float					m_fFloatValue;	// 0C
	NiPointer <NiFloatData>	m_spFloatData;	// 10
	UInt32					m_uiLastIdx;	// 14
};

// 24
class NiColorInterpolator : public NiKeyBasedInterpolator
{
public:
	NiColorA				m_kColorValue;	// 0C
	NiPointer <NiColorData>	m_spColorData;	// 1C
	UInt32					m_uiLastIdx;	// 20
};

// 5C
class NiPathInterpolator : public NiKeyBasedInterpolator
{
public:
	UInt16					m_uFlags;				// 0C
	UInt8					pad0E[2];				// 0E
	UInt32					m_uiLastPathIdx;		// 10
	UInt32					m_uiLastPctIdx;			// 14
	NiPointer <NiPosData>	m_spPathData;			// 18
	NiPointer <NiFloatData>	m_spPctData;			// 1C
	mutable float			* m_pfPartialLength;	// 20
	mutable float			m_fTotalLength;			// 24
	float					m_fBankAngle;			// 28
	float					m_fSmoothing;			// 2C
	UInt16					m_sFollowAxis;			// 30
	UInt8					pad32[2];				// 32
	float					m_fMaxCurvature;		// 34
	UInt32					m_eBankDir;				// 38
	NiQuaternion			m_kRotation;			// 3C
	NiPoint3				m_kPosition;			// 4C
	float					m_fLastTime;			// 58
};

//STATIC_ASSERT(sizeof(NiPathInterpolator) == 0x5C);

// ### remainder are placeholder

// 38
class NiTransformInterpolator : public NiKeyBasedInterpolator
{
public:
};

// 48
class BSRotAccumTransfInterpolator : public NiTransformInterpolator
{
public:
};

// 24
class BSTreadTransfInterpolator : public NiKeyBasedInterpolator
{
public:
};

// 
class NiQuaternionInterpolator : public NiKeyBasedInterpolator
{
public:
};

// 
class NiPoint3Interpolator : public NiKeyBasedInterpolator
{
public:
};

// 
class NiBoolInterpolator : public NiKeyBasedInterpolator
{
public:
};

// 
class NiBoolTimelineInterpolator : public NiBoolInterpolator
{
public:
};

// 
class NiLookAtInterpolator : public NiInterpolator
{
public:
};

// 
class NiBSplineInterpolator : public NiInterpolator
{
public:
};

// 
class NiBSplineTransformInterpolator : public NiBSplineInterpolator
{
public:
};

//
class NiBSplineCompTransformInterpolator : public NiBSplineTransformInterpolator
{
public:
};

// 
class NiBSplinePoint3Interpolator : public NiBSplineInterpolator
{
public:
};

//
class NiBSplineCompPoint3Interpolator : public NiBSplinePoint3Interpolator
{
public:
};

// 
class NiBSplineFloatInterpolator : public NiBSplineInterpolator
{
public:
};

//
class NiBSplineCompFloatInterpolator : public NiBSplineFloatInterpolator
{
public:
};

// 
class NiBSplineColorInterpolator : public NiBSplineInterpolator
{
public:
};

//
class NiBSplineCompColorInterpolator : public NiBSplineColorInterpolator
{
public:
};
