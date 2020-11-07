#pragma once

#include "skse64/NiNodes.h"

/**** controllers
 *	NiTimeController
 *		BSDoorHavokController
 *		BSPlayerDistanceCheckController
 *		NiD3DController
 *		NiControllerManager
 *		NiInterpController
 *			NiSingleInterpController
 *				NiTransformController
 *				NiPSysModifierCtlr
 *					NiPSysEmitterCtlr
 *					NiPSysModifierBoolCtlr
 *						NiPSysModifierActiveCtlr
 *					NiPSysModifierFloatCtlr
 *						NiPSysInitialRotSpeedVarCtlr
 *						NiPSysInitialRotSpeedCtlr
 *						NiPSysInitialRotAngleVarCtlr
 *						NiPSysInitialRotAngleCtlr
 *						NiPSysGravityStrengthCtlr
 *						NiPSysFieldMaxDistanceCtlr
 *						NiPSysFieldMagnitudeCtlr
 *						NiPSysFieldAttenuationCtlr
 *						NiPSysEmitterSpeedCtlr
 *						NiPSysEmitterPlanarAngleVarCtlr
 *						NiPSysEmitterPlanarAngleCtlr
 *						NiPSysEmitterLifeSpanCtlr
 *						NiPSysEmitterInitialRadiusCtlr
 *						NiPSysEmitterDeclinationVarCtlr
 *						NiPSysEmitterDeclinationCtlr
 *						NiPSysAirFieldSpreadCtlr
 *						NiPSysAirFieldInheritVelocityCtlr
 *						NiPSysAirFieldAirFrictionCtlr
 *				NiFloatInterpController
 *					NiFlipController
 *					NiAlphaController
 *					NiTextureTransformController
 *					NiLightDimmerController
 *				NiBoolInterpController
 *					NiVisController
 *				NiPoint3InterpController
 *					NiMaterialColorController
 *					NiLightColorController
 *				NiExtraDataController
 *					NiFloatsExtraDataPoint3Controller
 *					NiFloatsExtraDataController
 *					NiFloatExtraDataController
 *					NiColorExtraDataController
 *			NiMultiTargetTransformController
 *			NiGeomMorpherController
 *		bhkBlendController
 *		bhkForceController
 *		NiBSBoneLODController
 *		NiUVController
 *		NiPathController
 *		NiLookAtController
 *		NiKeyframeManager
 *		NiBoneLODController
 *		NiPSysUpdateCtlr
 *		NiPSysResetOnLoopCtlr
 *		NiFloatController
 *			NiRollController
 */

class NiInterpolator;

// 03C
class NiTimeController : public NiObject
{
public:
	NiTimeController();
	~NiTimeController();

	virtual void		Start(float time);
	virtual void		Stop(void);
	virtual void		Update(float time) = 0;
	virtual void		SetTarget(NiObjectNET * node);
	virtual bool		Unk_17(void);
	virtual bool		Unk_18(void);
	virtual void		Unk_19(void);
	virtual bool		Unk_1A(void);
	virtual bool		Unk_1B(void);
	virtual bool		Unk_1C(void) = 0;

	enum
	{
		kAnimType_AppTime =		0 << 0,
		kAnimType_AppInit =		1 << 0,

		kCycleType_Loop =		0 << 1,
		kCycleType_Reverse =	1 << 1,
		kCycleType_Clamp =		2 << 1,

		kActive =				1 << 3,
		
		kPlayBackwards =		1 << 4,
	};

	UInt16				flags;			// 008
	UInt8				pad00A[2];		// 00A
	float				m_fFrequency;	// 00C
	float				m_fPhase;		// 010
	float				m_fLoKeyTime;	// 014
	float				m_fHiKeyTime;	// 018
	float				m_fStartTime;	// 01C
	float				m_fLastTime;	// 020
	float				unk024;			// 024 - updated in Unk_19
	float				unk028;			// 028
	UInt8				unk02C;			// 02C
	UInt8				pad02D[3];		// 02D
	NiObjectNET			* next;			// 030 - singly linked list of controllers
	NiObject			* m_pTarget;	// 034 - type is a (wrong) guess; NET has a back controller pointer, but many children assume larger objects
	UInt8				unk038;			// 038
	UInt8				unk039[3];		// 039
};

// 03C
class NiInterpController : public NiTimeController
{
public:
	NiInterpController();
	~NiInterpController();

	virtual UInt16	GetNumInterpolators(void) = 0;	// 1D
	virtual char *	GetInterpolatorName(UInt16 idx) = 0;
	virtual UInt16	GetInterpolatorIdx(char * name) = 0;
	virtual NiInterpolator *	GetInterpolator(UInt16 idx) = 0;
	virtual void	SetInterpolator(NiInterpolator * _interpolator, UInt16 idx) = 0;
	virtual void	Unk_22(void);
	virtual char *	GetName(void);
	virtual void	Unk_24(UInt16 idx) = 0;	// create interpolators
	virtual void	Unk_25(void) = 0;
	virtual void	Unk_26(UInt16 idx) = 0;	// create blend interpolators
	virtual void	Unk_27(void) = 0;
	virtual void	Unk_28(void * arg, UInt16 idx) = 0;

	enum
	{
		kIsManagerControlled =	1 << 5,
	};
};

// 040
class NiSingleInterpController : public NiInterpController
{
public:
	NiSingleInterpController();
	~NiSingleInterpController();

	virtual bool	Unk_29(void * arg) = 0;

	NiInterpolator	* interpolator;	// 03C
};

// 040
class NiFloatInterpController : public NiSingleInterpController
{
public:
	NiFloatInterpController();
	~NiFloatInterpController();

	virtual void	Unk_2A(float * out) = 0;
};
