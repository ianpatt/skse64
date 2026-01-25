#pragma once

/**** hierarchy ****************************************************************
 *	
 *	BSFaceGenMorphData
 *		BSFaceGenMorphDataHead
 *		BSFaceGenMorphDataHair
 *
 *	BSTempEffect
 *		BSTempEffectDebris
 *		BSTempEffectGeometryDecal
 *		BSTempEffectParticle
 *		BSTempEffectSimpleDecal
 *		BSTempEffectSPG
 *		ReferenceEffect
 *			ModelReferenceEffect
 *			ShaderReferenceEffect
 *			SummonPlacementEffect
 *
 *	NiObject
 *		NiObjectNET
 *			NiAVObject
 *				NiNode
 *					BGSDecalNode
 *					BSFaceGenNiNode
 *					NiSwitchNode
 *					BSFlattenedBoneTree
 *					NiBillboardNode
 *					NiBSPNode
 *					BSTempNodeManager
 *					BSTempNode
 *					BSPortalSharedNode
 *					BSParticleSystemManager
 *					BSSceneGraph
 *						SceneGraph
 *					BSMasterParticleSystem
 *					BSNiNode
 *						BSValueNode
 *						BSRangeNode
 *							BSBlastNode
 *							BSDebrisNode
 *							BSDamageStage
 *						BSMultiBoundNode
 *						BSOrderedNode
 *					BSFadeNode
 *					BSLeafAnimNode
 *					BSTreeNode
 *					ShadowSceneNode
 *					BSClearZNode
 *				NiLight
 *					NiPointLight
 *						NiSpotLight
 *					NiDirectionalLight
 *					NiAmbientLight
 *				NiCamera
 *					BSCubeMapCamera
 *				BSGeometry
 *					BSTriShape
 *						BSDynamicTriShape
 *						BSSubIndexTriShape
 *							BSSubIndexLandTriShape
 *						BSInstanceTriShape
 *							BSMultiStreamInstanceTriShape
 *						BSMultiIndexTriShape
 *							BSLODMultiIndexTriShape
 *						BSMeshLODTriShape
 *					NiParticles
 *						NiParticleMeshes
 *						NiParticleSystem
 *							NiMeshParticleSystem
 *							BSStripParticleSystem
 *					BSLines
 *						BSDynamicLines
 *				NiGeometry
 *					NiTriBasedGeom
 *						NiTriShape
 *							BSLODTriShape
 *							BSSegmentedTriShape
 *						NiTriStrips
 *			NiProperty
 *				NiAlphaProperty
 *				NiShadeProperty
 *					BSShaderProperty
 *						BSLightingShaderProperty
 *						BSGrassShaderProperty
 *						BSEffectShaderProperty
 *						BSWaterShaderProperty
 *						BSBloodSplatterShaderProperty
 *						BSParticleShaderProperty
 *						BSSkyShaderProperty
 *						BSDistantTreeShaderProperty
 *				NiFogProperty
 *					BSFogProperty
 *			NiSequenceStreamHelper
 *		BSDismemberSkinInstance
 *		NiTimeController
 *			REFRSyncController
 *			BSDoorHavokController
 *			BSPlayerDistanceCheckController
 *			BSSimpleScaleController
 *			NiControllerManager
 *			NiInterpController
 *				NiMultiTargetTransformController
 *				BSMultiTargetTreadTransfController
 *				NiSingleInterpController
 *					NiTransformController
 *					NiExtraDataController
 *						NiFloatExtraDataController
 *						NiColorExtraDataController
 *						NiFloatsExtraDataController
 *						NiFloatsExtraDataPoint3Controller
 *					NiPoint3InterpController
 *						NiLightColorController
 *						BSLightingShaderPropertyColorController
 *						BSEffectShaderPropertyColorController
 *					NiFloatInterpController
 *						NiLightDimmerController
 *						BSFrustumFOVController
 *						BSLightingShaderPropertyFloatController
 *						BSLightingShaderPropertyUShortController
 *						BSEffectShaderPropertyFloatController
 *						BSNiAlphaPropertyTestRefController
 *					NiBoolInterpController
 *						NiVisController
 *					NiPSysModifierCtlr
 *						NiPSysEmitterCtlr
 *							BSPSysMultiTargetEmitterCtlr
 *						NiPSysModifierFloatCtlr
 *							NiPSysAirFieldAirFrictionCtlr
 *							NiPSysAirFieldInheritVelocityCtlr
 *							NiPSysAirFieldSpreadCtlr
 *							NiPSysEmitterDeclinationCtlr
 *							NiPSysEmitterDeclinationVarCtlr
 *							NiPSysEmitterInitialRadiusCtlr
 *							NiPSysEmitterLifeSpanCtlr
 *							NiPSysEmitterPlanarAngleCtlr
 *							NiPSysEmitterPlanarAngleVarCtlr
 *							NiPSysEmitterSpeedCtlr
 *							NiPSysFieldAttenuationCtlr
 *							NiPSysFieldMagnitudeCtlr
 *							NiPSysFieldMaxDistanceCtlr
 *							NiPSysGravityStrengthCtlr
 *							NiPSysInitialRotAngleCtlr
 *							NiPSysInitialRotAngleVarCtlr
 *							NiPSysInitialRotSpeedCtlr
 *							NiPSysInitialRotSpeedVarCtlr
 *						NiPSysModifierBoolCtlr
 *							NiPSysModifierActiveCtlr
 *				BSProceduralLightningController
 *			NiKeyframeManager
 *			NiLookAtController
 *			NiPathController
 *			NiFloatController
 *				NiRollController
 *			NiPSysUpdateCtlr
 *			NiPSysResetOnLoopCtlr
 *			NiBSBoneLODController
 *			BSLagBoneController
 *			bhkBlendController
 *		NiExtraData
 *			BSSplatterExtraData
 *			BGSAddonNodeSoundHandleExtra
 *			BSFaceGenAnimationData
 *			BSFaceGenModelExtraData
 *			BSFaceGenBaseMorphExtraData
 *			NiStringsExtraData
 *			NiStringExtraData
 *			NiVertWeightsExtraData
 *			NiBinaryExtraData
 *			NiBooleanExtraData
 *				BSDistantObjectLargeRefExtraData
 *			NiColorExtraData
 *			NiFloatExtraData
 *			NiFloatsExtraData
 *			NiIntegerExtraData
 *				BSXFlags
 *				BSDecalPlacementVectorExtraData
 *			NiIntegersExtraData
 *				BSWArray
 *			NiSwitchStringExtraData
 *			NiVectorExtraData
 *			NiTextKeyExtraData
 *			PArrayPoint
 *			BSBodyMorphOffsetsExtraData
 *			BSBehaviorGraphExtraData
 *			BSFurnitureMarkerNode
 *			BSBound
 *			BSBoneMap
 *			BSAnimInteractionMarker
 *			BSInvMarker
 *			BSBoneLODExtraData
 *			BSNonUniformScaleExtraData
 *			bhkRagdollTemplate
 *			bhkExtraData
 *			DebugTextExtraData
 *		NiGeometryData
 *			NiTriBasedGeomData
 *				NiTriShapeData
 *				NiTriStripsData
 *		NiTexture
 *			NiSourceTexture
 *		NiSkinPartition
 *		NiSkinInstance
 *		NiAVObjectPalette
 *			NiDefaultAVObjectPalette
 *		NiSkinData
 *		NiParticlesData
 *			NiParticleMeshesData
 *			NiPSysData
 *				NiMeshPSysData
 *				BSStripPSysData
 *		NiAdditionalGeometryData
 *		NiAccumulator
 *			NiBackToFrontAccumulator
 *				NiAlphaAccumulator
 *					BSShaderAccumulator
 *		NiControllerSequence
 *			BSAnimGroupSequence
 *		NiFloatData
 *		NiColorData
 *		NiInterpolator
 *			NiBlendInterpolator
 *				NiBlendTransformInterpolator
 *				NiBlendFloatInterpolator
 *				NiBlendAccumTransformInterpolator
 *				BSBlendTreadTransfInterpolator
 *				NiBlendBoolInterpolator
 *				NiBlendColorInterpolator
 *				NiBlendPoint3Interpolator
 *				NiBlendQuaternionInterpolator
 *			NiKeyBasedInterpolator
 *				NiFloatInterpolator
 *				NiColorInterpolator
 *				NiTransformInterpolator
 *					BSRotAccumTransfInterpolator
 *				NiPathInterpolator
 *				NiBoolInterpolator
 *					NiBoolTimelineInterpolator
 *				NiPoint3Interpolator
 *				NiQuaternionInterpolator
 *				BSTreadTransfInterpolator
 *			NiLookAtInterpolator
 *			NiBSplineInterpolator
 *				NiBSplineColorInterpolator
 *					NiBSplineCompColorInterpolator
 *				NiBSplineFloatInterpolator
 *					NiBSplineCompFloatInterpolator
 *				NiBSplinePoint3Interpolator
 *					NiBSplineCompPoint3Interpolator
 *				NiBSplineTransformInterpolator
 *					NiBSplineCompTransformInterpolator
 *		NiTransformData
 *		NiPosData
 *		NiBoolData
 *		NiBSplineBasisData
 *		NiBSplineData
 *		NiMorphData
 *		NiRotData
 *		NiSequence
 *		NiStringPalette
 *		NiUVData
 *		BSAnimNote
 *			BSGrabIKNote
 *			BSLookIKNote
 *		BSAnimNotes
 *		NiPSysModifier
 *			NiPSysGravityModifier
 *			NiPSysEmitter
 *				NiPSysMeshEmitter
 *				NiPSysVolumeEmitter
 *					NiPSysCylinderEmitter
 *					NiPSysBoxEmitter
 *					NiPSysSphereEmitter
 *					BSPSysArrayEmitter
 *			NiPSysMeshUpdateModifier
 *				BSPSysHavokUpdateModifier
 *			NiPSysAgeDeathModifier
 *			NiPSysBombModifier
 *			NiPSysBoundUpdateModifier
 *			NiPSysColliderManager
 *			NiPSysColorModifier
 *			NiPSysDragModifier
 *			NiPSysGrowFadeModifier
 *			NiPSysPositionModifier
 *			NiPSysRotationModifier
 *			NiPSysSpawnModifier
 *			BSPSysRecycleBoundModifier
 *			BSPSysInheritVelocityModifier
 *			NiPSysFieldModifier
 *				NiPSysAirFieldModifier
 *				NiPSysDragFieldModifier
 *				NiPSysGravityFieldModifier
 *				NiPSysRadialFieldModifier
 *				NiPSysTurbulenceFieldModifier
 *				NiPSysVortexFieldModifier
 *			BSWindModifier
 *			BSParentVelocityModifier
 *			BSPSysStripUpdateModifier
 *			BSPSysSubTexModifier
 *			BSPSysScaleModifier
 *			BSPSysSimpleColorModifier
 *			BSPSysLODModifier
 *		NiPSysEmitterCtlrData
 *		NiPSysCollider
 *			NiPSysPlanarCollider
 *			NiPSysSphericalCollider
 *		BSMultiBound
 *		BSMultiBoundRoom
 *		BSOcclusionShape
 *		BSMultiBoundShape
 *			BSMultiBoundAABB
 *				BSMultiBoundOBB
 *			BSMultiBoundSphere
 *			BSMultiBoundCapsule
 *		BSOcclusionBox
 *		BSOcclusionPlane
 *			BSPortal
 *		BSReference
 *		BSNodeReferences
 *		bhkRefObject
 *			bhkWorld
 *				bhkWorldM
 *			bhkSerializable
 *				bhkWorldObject
 *					bhkEntity
 *						bhkRigidBody
 *							bhkRigidBodyT
 *					bhkPhantom
 *						bhkAabbPhantom
 *							bhkAutoWater
 *							bhkAvoidBox
 *						bhkShapePhantom
 *							bhkSimpleShapePhantom
 *								bhkCollisionBox
 *							bhkCachingShapePhantom
 *				bhkShape
 *					bhkSphereRepShape
 *						bhkConvexShape
 *							bhkCapsuleShape
 *							bhkBoxShape
 *							bhkSphereShape
 *							bhkConvexSweepShape
 *							bhkConvexTransformShape
 *							bhkConvexTranslateShape
 *							bhkConvexVerticesShape
 *								bhkCharControllerShape
 *							bhkCylinderShape
 *							bhkTriangleShape
 *						bhkMultiSphereShape
 *					bhkBvTreeShape
 *						bhkTriSampledHeightFieldBvTreeShape
 *						bhkMoppBvTreeShape
 *					bhkTransformShape
 *					bhkShapeCollection
 *						bhkNiTriStripsShape
 *						bhkPackedNiTriStripsShape
 *						bhkCompressedMeshShape
 *						bhkListShape
 *					bhkHeightFieldShape
 *						bhkPlaneShape
 *				bhkConstraint
 *					bhkLimitedHingeConstraint
 *					bhkMalleableConstraint
 *					bhkPrismaticConstraint
 *					bhkHingeConstraint
 *					bhkBallAndSocketConstraint
 *					bhkBallSocketConstraintChain
 *					bhkGroupConstraint
 *					bhkHingeLimitsConstraint
 *					bhkRagdollConstraint
 *					bhkRagdollLimitsConstraint
 *					bhkStiffSpringConstraint
 *					bhkWheelConstraint
 *					bhkBreakableConstraint
 *					bhkGenericConstraint
 *						bhkFixedConstraint
 *					bhkConstraintChain
 *					bhkPointToPathConstraint
 *				bhkAction
 *					bhkUnaryAction
 *						bhkTiltPreventAction
 *						bhkWheelAction
 *						bhkMouseSpringAction
 *						bhkLiquidAction
 *						bhkMotorAction
 *						bhkOrientHingedBodyAction
 *					bhkBinaryAction
 *						bhkAngularDashpotAction
 *						bhkDashpotAction
 *						bhkSpringAction
 *				bhkCharacterProxy
 *				bhkCharacterRigidBody
 *		hkPackedNiTriStripsData
 *		bhkRagdollTemplateData
 *		bhkCompressedMeshShapeData
 *		bhkPoseArray
 *		BSTextureSet
 *			BSShaderTextureSet
 *		NiCollisionObject
 *			NiCollisionData
 *			bhkNiCollisionObject
 *				bhkCollisionObject
 *					bhkCartTether
 *					bhkBlendCollisionObject
 *						bhkAttachmentCollisionObject
 *						WeaponObject
 *				bhkPCollisionObject
 *					bhkSPCollisionObject
 *
 *	NiCullingProcess
 *		BSCullingProcess
 *			BSGeometryListCullingProcess
 *			BSParabolicCullingProcess
 *		BSFadeNodeCuller
 *
 *	bhkPositionConstraintMotor
 *
 *	bhkVelocityConstraintMotor
 *
 *	bhkSpringDamperConstraintMotor
 *	
 ****/

class NiObject;

// 08
class NiRTTI
{
public:
	const char	* name;
	NiRTTI		* parent;
};

NiObject * DoNiRTTICast(NiObject * src, const NiRTTI * typeInfo);
bool IsType(NiRTTI * rtti, const NiRTTI * typeInfo);

#define ni_cast(obj, type)	(type *)DoNiRTTICast(obj, NiRTTI_##type)
#define ni_is_type(obj, type) IsType(obj, NiRTTI_##type)

extern const NiRTTI *	NiRTTI_BGSDecalNode;
extern const NiRTTI *	NiRTTI_BSAnimGroupSequence;
extern const NiRTTI *	NiRTTI_BSSplatterExtraData;
extern const NiRTTI *	NiRTTI_BGSAddonNodeSoundHandleExtra;
extern const NiRTTI *	NiRTTI_REFRSyncController;
extern const NiRTTI *	NiRTTI_bhkCartTether;
extern const NiRTTI *	NiRTTI_bhkTiltPreventAction;
extern const NiRTTI *	NiRTTI_bhkWheelAction;
extern const NiRTTI *	NiRTTI_BSFaceGenAnimationData;
extern const NiRTTI *	NiRTTI_BSFaceGenModelExtraData;
extern const NiRTTI *	NiRTTI_BSFaceGenBaseMorphExtraData;
extern const NiRTTI *	NiRTTI_BSFaceGenMorphData;
extern const NiRTTI *	NiRTTI_BSFaceGenMorphDataHead;
extern const NiRTTI *	NiRTTI_BSFaceGenMorphDataHair;
extern const NiRTTI *	NiRTTI_BSFaceGenNiNode;
extern const NiRTTI *	NiRTTI_BSTempEffect;
extern const NiRTTI *	NiRTTI_BSTempEffectDebris;
extern const NiRTTI *	NiRTTI_BSTempEffectGeometryDecal;
extern const NiRTTI *	NiRTTI_BSTempEffectParticle;
extern const NiRTTI *	NiRTTI_BSTempEffectSimpleDecal;
extern const NiRTTI *	NiRTTI_BSTempEffectSPG;
extern const NiRTTI *	NiRTTI_bhkAutoWater;
extern const NiRTTI *	NiRTTI_ModelReferenceEffect;
extern const NiRTTI *	NiRTTI_ReferenceEffect;
extern const NiRTTI *	NiRTTI_ShaderReferenceEffect;
extern const NiRTTI *	NiRTTI_SummonPlacementEffect;
extern const NiRTTI *	NiRTTI_SceneGraph;
extern const NiRTTI *	NiRTTI_BSDoorHavokController;
extern const NiRTTI *	NiRTTI_BSPlayerDistanceCheckController;
extern const NiRTTI *	NiRTTI_BSSimpleScaleController;
extern const NiRTTI *	NiRTTI_NiObject;
extern const NiRTTI *	NiRTTI_NiAVObject;
extern const NiRTTI *	NiRTTI_NiNode;
extern const NiRTTI *	NiRTTI_NiObjectNET;
extern const NiRTTI *	NiRTTI_NiLight;
extern const NiRTTI *	NiRTTI_NiSwitchNode;
extern const NiRTTI *	NiRTTI_NiStringsExtraData;
extern const NiRTTI *	NiRTTI_NiCamera;
extern const NiRTTI *	NiRTTI_BSTriShape;
extern const NiRTTI *	NiRTTI_NiProperty;
extern const NiRTTI *	NiRTTI_NiAlphaProperty;
extern const NiRTTI *	NiRTTI_NiSourceTexture;
extern const NiRTTI *	NiRTTI_BSFlattenedBoneTree;
extern const NiRTTI *	NiRTTI_BSDismemberSkinInstance;
extern const NiRTTI *	NiRTTI_NiStringExtraData;
extern const NiRTTI *	NiRTTI_NiTimeController;
extern const NiRTTI *	NiRTTI_NiExtraData;
extern const NiRTTI *	NiRTTI_NiGeometryData;
extern const NiRTTI *	NiRTTI_BSGeometry;
extern const NiRTTI *	NiRTTI_BSDynamicTriShape;
extern const NiRTTI *	NiRTTI_NiPointLight;
extern const NiRTTI *	NiRTTI_NiDefaultAVObjectPalette;
extern const NiRTTI *	NiRTTI_NiBillboardNode;
extern const NiRTTI *	NiRTTI_NiDirectionalLight;
extern const NiRTTI *	NiRTTI_NiCullingProcess;
extern const NiRTTI *	NiRTTI_NiParticles;
extern const NiRTTI *	NiRTTI_NiTexture;
extern const NiRTTI *	NiRTTI_NiSkinPartition;
extern const NiRTTI *	NiRTTI_NiVertWeightsExtraData;
extern const NiRTTI *	NiRTTI_NiSkinInstance;
extern const NiRTTI *	NiRTTI_NiAVObjectPalette;
extern const NiRTTI *	NiRTTI_NiGeometry;
extern const NiRTTI *	NiRTTI_NiSkinData;
extern const NiRTTI *	NiRTTI_NiShadeProperty;
extern const NiRTTI *	NiRTTI_NiAlphaAccumulator;
extern const NiRTTI *	NiRTTI_NiAmbientLight;
extern const NiRTTI *	NiRTTI_NiBinaryExtraData;
extern const NiRTTI *	NiRTTI_NiBooleanExtraData;
extern const NiRTTI *	NiRTTI_NiBSPNode;
extern const NiRTTI *	NiRTTI_NiColorExtraData;
extern const NiRTTI *	NiRTTI_NiFloatExtraData;
extern const NiRTTI *	NiRTTI_NiFloatsExtraData;
extern const NiRTTI *	NiRTTI_NiFogProperty;
extern const NiRTTI *	NiRTTI_NiIntegerExtraData;
extern const NiRTTI *	NiRTTI_NiIntegersExtraData;
extern const NiRTTI *	NiRTTI_NiParticlesData;
extern const NiRTTI *	NiRTTI_NiParticleMeshesData;
extern const NiRTTI *	NiRTTI_NiParticleMeshes;
extern const NiRTTI *	NiRTTI_NiSpotLight;
extern const NiRTTI *	NiRTTI_NiSwitchStringExtraData;
extern const NiRTTI *	NiRTTI_NiTriShapeData;
extern const NiRTTI *	NiRTTI_NiTriShape;
extern const NiRTTI *	NiRTTI_NiTriStripsData;
extern const NiRTTI *	NiRTTI_NiTriStrips;
extern const NiRTTI *	NiRTTI_NiVectorExtraData;
extern const NiRTTI *	NiRTTI_BSLODTriShape;
extern const NiRTTI *	NiRTTI_NiAdditionalGeometryData;
extern const NiRTTI *	NiRTTI_BSSegmentedTriShape;
extern const NiRTTI *	NiRTTI_NiAccumulator;
extern const NiRTTI *	NiRTTI_NiBackToFrontAccumulator;
extern const NiRTTI *	NiRTTI_NiTriBasedGeomData;
extern const NiRTTI *	NiRTTI_NiTriBasedGeom;
extern const NiRTTI *	NiRTTI_NiCollisionData;
extern const NiRTTI *	NiRTTI_NiControllerManager;
extern const NiRTTI *	NiRTTI_NiControllerSequence;
extern const NiRTTI *	NiRTTI_NiBlendInterpolator;
extern const NiRTTI *	NiRTTI_NiMultiTargetTransformController;
extern const NiRTTI *	NiRTTI_BSMultiTargetTreadTransfController;
extern const NiRTTI *	NiRTTI_NiInterpController;
extern const NiRTTI *	NiRTTI_NiFloatData;
extern const NiRTTI *	NiRTTI_NiFloatInterpolator;
extern const NiRTTI *	NiRTTI_NiColorData;
extern const NiRTTI *	NiRTTI_NiColorInterpolator;
extern const NiRTTI *	NiRTTI_NiSingleInterpController;
extern const NiRTTI *	NiRTTI_NiTransformInterpolator;
extern const NiRTTI *	NiRTTI_NiPathInterpolator;
extern const NiRTTI *	NiRTTI_NiBlendTransformInterpolator;
extern const NiRTTI *	NiRTTI_NiBlendFloatInterpolator;
extern const NiRTTI *	NiRTTI_NiFloatExtraDataController;
extern const NiRTTI *	NiRTTI_NiTransformController;
extern const NiRTTI *	NiRTTI_NiBlendAccumTransformInterpolator;
extern const NiRTTI *	NiRTTI_NiInterpolator;
extern const NiRTTI *	NiRTTI_BSBlendTreadTransfInterpolator;
extern const NiRTTI *	NiRTTI_NiKeyBasedInterpolator;
extern const NiRTTI *	NiRTTI_NiTransformData;
extern const NiRTTI *	NiRTTI_NiPosData;
extern const NiRTTI *	NiRTTI_NiBlendBoolInterpolator;
extern const NiRTTI *	NiRTTI_NiBlendColorInterpolator;
extern const NiRTTI *	NiRTTI_NiBlendPoint3Interpolator;
extern const NiRTTI *	NiRTTI_NiBlendQuaternionInterpolator;
extern const NiRTTI *	NiRTTI_NiBoolData;
extern const NiRTTI *	NiRTTI_NiBoolInterpolator;
extern const NiRTTI *	NiRTTI_NiBoolTimelineInterpolator;
extern const NiRTTI *	NiRTTI_NiBSplineBasisData;
extern const NiRTTI *	NiRTTI_NiBSplineData;
extern const NiRTTI *	NiRTTI_NiBSplineColorInterpolator;
extern const NiRTTI *	NiRTTI_NiBSplineCompColorInterpolator;
extern const NiRTTI *	NiRTTI_NiBSplineCompFloatInterpolator;
extern const NiRTTI *	NiRTTI_NiBSplineCompPoint3Interpolator;
extern const NiRTTI *	NiRTTI_NiBSplineCompTransformInterpolator;
extern const NiRTTI *	NiRTTI_NiBSplineFloatInterpolator;
extern const NiRTTI *	NiRTTI_NiBSplinePoint3Interpolator;
extern const NiRTTI *	NiRTTI_NiBSplineTransformInterpolator;
extern const NiRTTI *	NiRTTI_NiColorExtraDataController;
extern const NiRTTI *	NiRTTI_NiFloatsExtraDataController;
extern const NiRTTI *	NiRTTI_NiFloatsExtraDataPoint3Controller;
extern const NiRTTI *	NiRTTI_NiKeyframeManager;
extern const NiRTTI *	NiRTTI_NiLightColorController;
extern const NiRTTI *	NiRTTI_NiLightDimmerController;
extern const NiRTTI *	NiRTTI_NiLookAtController;
extern const NiRTTI *	NiRTTI_NiLookAtInterpolator;
extern const NiRTTI *	NiRTTI_NiMorphData;
extern const NiRTTI *	NiRTTI_NiPathController;
extern const NiRTTI *	NiRTTI_NiPoint3Interpolator;
extern const NiRTTI *	NiRTTI_NiQuaternionInterpolator;
extern const NiRTTI *	NiRTTI_NiRollController;
extern const NiRTTI *	NiRTTI_NiRotData;
extern const NiRTTI *	NiRTTI_NiSequence;
extern const NiRTTI *	NiRTTI_NiSequenceStreamHelper;
extern const NiRTTI *	NiRTTI_NiStringPalette;
extern const NiRTTI *	NiRTTI_NiTextKeyExtraData;
extern const NiRTTI *	NiRTTI_NiUVData;
extern const NiRTTI *	NiRTTI_NiVisController;
extern const NiRTTI *	NiRTTI_BSAnimNote;
extern const NiRTTI *	NiRTTI_BSAnimNotes;
extern const NiRTTI *	NiRTTI_BSGrabIKNote;
extern const NiRTTI *	NiRTTI_BSLookIKNote;
extern const NiRTTI *	NiRTTI_BSRotAccumTransfInterpolator;
extern const NiRTTI *	NiRTTI_BSTreadTransfInterpolator;
extern const NiRTTI *	NiRTTI_BSFrustumFOVController;
extern const NiRTTI *	NiRTTI_NiExtraDataController;
extern const NiRTTI *	NiRTTI_NiBSplineInterpolator;
extern const NiRTTI *	NiRTTI_NiPoint3InterpController;
extern const NiRTTI *	NiRTTI_NiFloatInterpController;
extern const NiRTTI *	NiRTTI_NiFloatController;
extern const NiRTTI *	NiRTTI_NiBoolInterpController;
extern const NiRTTI *	NiRTTI_NiParticleSystem;
extern const NiRTTI *	NiRTTI_NiPSysEmitterCtlr;
extern const NiRTTI *	NiRTTI_NiPSysGravityModifier;
extern const NiRTTI *	NiRTTI_BSPSysHavokUpdateModifier;
extern const NiRTTI *	NiRTTI_NiMeshParticleSystem;
extern const NiRTTI *	NiRTTI_NiPSysCylinderEmitter;
extern const NiRTTI *	NiRTTI_BSStripParticleSystem;
extern const NiRTTI *	NiRTTI_NiPSysEmitter;
extern const NiRTTI *	NiRTTI_NiPSysModifierCtlr;
extern const NiRTTI *	NiRTTI_NiPSysModifier;
extern const NiRTTI *	NiRTTI_NiPSysMeshUpdateModifier;
extern const NiRTTI *	NiRTTI_NiPSysUpdateCtlr;
extern const NiRTTI *	NiRTTI_NiMeshPSysData;
extern const NiRTTI *	NiRTTI_NiPSysAirFieldAirFrictionCtlr;
extern const NiRTTI *	NiRTTI_NiPSysAirFieldInheritVelocityCtlr;
extern const NiRTTI *	NiRTTI_NiPSysAirFieldModifier;
extern const NiRTTI *	NiRTTI_NiPSysAirFieldSpreadCtlr;
extern const NiRTTI *	NiRTTI_NiPSysAgeDeathModifier;
extern const NiRTTI *	NiRTTI_NiPSysBombModifier;
extern const NiRTTI *	NiRTTI_NiPSysBoundUpdateModifier;
extern const NiRTTI *	NiRTTI_NiPSysBoxEmitter;
extern const NiRTTI *	NiRTTI_NiPSysColliderManager;
extern const NiRTTI *	NiRTTI_NiPSysColorModifier;
extern const NiRTTI *	NiRTTI_NiPSysData;
extern const NiRTTI *	NiRTTI_NiPSysDragFieldModifier;
extern const NiRTTI *	NiRTTI_NiPSysDragModifier;
extern const NiRTTI *	NiRTTI_NiPSysEmitterCtlrData;
extern const NiRTTI *	NiRTTI_NiPSysEmitterDeclinationCtlr;
extern const NiRTTI *	NiRTTI_NiPSysEmitterDeclinationVarCtlr;
extern const NiRTTI *	NiRTTI_NiPSysEmitterInitialRadiusCtlr;
extern const NiRTTI *	NiRTTI_NiPSysEmitterLifeSpanCtlr;
extern const NiRTTI *	NiRTTI_NiPSysEmitterPlanarAngleCtlr;
extern const NiRTTI *	NiRTTI_NiPSysEmitterPlanarAngleVarCtlr;
extern const NiRTTI *	NiRTTI_NiPSysEmitterSpeedCtlr;
extern const NiRTTI *	NiRTTI_NiPSysFieldAttenuationCtlr;
extern const NiRTTI *	NiRTTI_NiPSysFieldMagnitudeCtlr;
extern const NiRTTI *	NiRTTI_NiPSysFieldMaxDistanceCtlr;
extern const NiRTTI *	NiRTTI_NiPSysGravityFieldModifier;
extern const NiRTTI *	NiRTTI_NiPSysGravityStrengthCtlr;
extern const NiRTTI *	NiRTTI_NiPSysGrowFadeModifier;
extern const NiRTTI *	NiRTTI_NiPSysInitialRotAngleCtlr;
extern const NiRTTI *	NiRTTI_NiPSysInitialRotAngleVarCtlr;
extern const NiRTTI *	NiRTTI_NiPSysInitialRotSpeedCtlr;
extern const NiRTTI *	NiRTTI_NiPSysInitialRotSpeedVarCtlr;
extern const NiRTTI *	NiRTTI_NiPSysMeshEmitter;
extern const NiRTTI *	NiRTTI_NiPSysModifierActiveCtlr;
extern const NiRTTI *	NiRTTI_NiPSysPlanarCollider;
extern const NiRTTI *	NiRTTI_NiPSysPositionModifier;
extern const NiRTTI *	NiRTTI_NiPSysRadialFieldModifier;
extern const NiRTTI *	NiRTTI_NiPSysResetOnLoopCtlr;
extern const NiRTTI *	NiRTTI_NiPSysRotationModifier;
extern const NiRTTI *	NiRTTI_NiPSysSpawnModifier;
extern const NiRTTI *	NiRTTI_NiPSysSphereEmitter;
extern const NiRTTI *	NiRTTI_NiPSysSphericalCollider;
extern const NiRTTI *	NiRTTI_NiPSysTurbulenceFieldModifier;
extern const NiRTTI *	NiRTTI_NiPSysVortexFieldModifier;
extern const NiRTTI *	NiRTTI_BSStripPSysData;
extern const NiRTTI *	NiRTTI_BSPSysRecycleBoundModifier;
extern const NiRTTI *	NiRTTI_BSPSysInheritVelocityModifier;
extern const NiRTTI *	NiRTTI_NiPSysVolumeEmitter;
extern const NiRTTI *	NiRTTI_NiPSysModifierFloatCtlr;
extern const NiRTTI *	NiRTTI_NiPSysFieldModifier;
extern const NiRTTI *	NiRTTI_NiPSysModifierBoolCtlr;
extern const NiRTTI *	NiRTTI_NiPSysCollider;
extern const NiRTTI *	NiRTTI_BSMultiBound;
extern const NiRTTI *	NiRTTI_BSMultiBoundRoom;
extern const NiRTTI *	NiRTTI_BSMultiBoundAABB;
extern const NiRTTI *	NiRTTI_BSMultiBoundOBB;
extern const NiRTTI *	NiRTTI_BSXFlags;
extern const NiRTTI *	NiRTTI_BSValueNode;
extern const NiRTTI *	NiRTTI_BSWindModifier;
extern const NiRTTI *	NiRTTI_BSTempNodeManager;
extern const NiRTTI *	NiRTTI_BSTempNode;
extern const NiRTTI *	NiRTTI_BSOcclusionShape;
extern const NiRTTI *	NiRTTI_BSRangeNode;
extern const NiRTTI *	NiRTTI_BSBlastNode;
extern const NiRTTI *	NiRTTI_BSDebrisNode;
extern const NiRTTI *	NiRTTI_BSDamageStage;
extern const NiRTTI *	NiRTTI_BSPSysArrayEmitter;
extern const NiRTTI *	NiRTTI_PArrayPoint;
extern const NiRTTI *	NiRTTI_BSMultiStreamInstanceTriShape;
extern const NiRTTI *	NiRTTI_BSMultiBoundShape;
extern const NiRTTI *	NiRTTI_BSMultiBoundSphere;
extern const NiRTTI *	NiRTTI_BSOcclusionBox;
extern const NiRTTI *	NiRTTI_BSOcclusionPlane;
extern const NiRTTI *	NiRTTI_BSPortal;
extern const NiRTTI *	NiRTTI_BSPortalSharedNode;
extern const NiRTTI *	NiRTTI_BSBodyMorphOffsetsExtraData;
extern const NiRTTI *	NiRTTI_BSBehaviorGraphExtraData;
extern const NiRTTI *	NiRTTI_NiBSBoneLODController;
extern const NiRTTI *	NiRTTI_BSCullingProcess;
extern const NiRTTI *	NiRTTI_BSParticleSystemManager;
extern const NiRTTI *	NiRTTI_BSFurnitureMarkerNode;
extern const NiRTTI *	NiRTTI_BSBound;
extern const NiRTTI *	NiRTTI_BSMultiBoundNode;
extern const NiRTTI *	NiRTTI_BSBoneMap;
extern const NiRTTI *	NiRTTI_BSAnimInteractionMarker;
extern const NiRTTI *	NiRTTI_BSSceneGraph;
extern const NiRTTI *	NiRTTI_BSPSysMultiTargetEmitterCtlr;
extern const NiRTTI *	NiRTTI_BSGeometryListCullingProcess;
extern const NiRTTI *	NiRTTI_BSSubIndexTriShape;
extern const NiRTTI *	NiRTTI_BSDistantObjectLargeRefExtraData;
extern const NiRTTI *	NiRTTI_BSMasterParticleSystem;
extern const NiRTTI *	NiRTTI_BSProceduralLightningController;
extern const NiRTTI *	NiRTTI_BSInvMarker;
extern const NiRTTI *	NiRTTI_BSBoneLODExtraData;
extern const NiRTTI *	NiRTTI_BSReference;
extern const NiRTTI *	NiRTTI_BSNodeReferences;
extern const NiRTTI *	NiRTTI_BSDecalPlacementVectorExtraData;
extern const NiRTTI *	NiRTTI_BSParentVelocityModifier;
extern const NiRTTI *	NiRTTI_BSWArray;
extern const NiRTTI *	NiRTTI_BSMultiBoundCapsule;
extern const NiRTTI *	NiRTTI_BSPSysStripUpdateModifier;
extern const NiRTTI *	NiRTTI_BSPSysSubTexModifier;
extern const NiRTTI *	NiRTTI_BSPSysScaleModifier;
extern const NiRTTI *	NiRTTI_BSLagBoneController;
extern const NiRTTI *	NiRTTI_BSNonUniformScaleExtraData;
extern const NiRTTI *	NiRTTI_BSNiNode;
extern const NiRTTI *	NiRTTI_BSInstanceTriShape;
extern const NiRTTI *	NiRTTI_bhkWorldObject;
extern const NiRTTI *	NiRTTI_bhkWorld;
extern const NiRTTI *	NiRTTI_bhkRigidBody;
extern const NiRTTI *	NiRTTI_bhkCollisionObject;
extern const NiRTTI *	NiRTTI_bhkNiCollisionObject;
extern const NiRTTI *	NiRTTI_bhkAttachmentCollisionObject;
extern const NiRTTI *	NiRTTI_WeaponObject;
extern const NiRTTI *	NiRTTI_bhkRigidBodyT;
extern const NiRTTI *	NiRTTI_bhkWorldM;
extern const NiRTTI *	NiRTTI_bhkRefObject;
extern const NiRTTI *	NiRTTI_bhkSerializable;
extern const NiRTTI *	NiRTTI_bhkShape;
extern const NiRTTI *	NiRTTI_bhkEntity;
extern const NiRTTI *	NiRTTI_bhkPhantom;
extern const NiRTTI *	NiRTTI_bhkAabbPhantom;
extern const NiRTTI *	NiRTTI_bhkSphereRepShape;
extern const NiRTTI *	NiRTTI_bhkConvexShape;
extern const NiRTTI *	NiRTTI_bhkPCollisionObject;
extern const NiRTTI *	NiRTTI_hkPackedNiTriStripsData;
extern const NiRTTI *	NiRTTI_bhkShapePhantom;
extern const NiRTTI *	NiRTTI_bhkSimpleShapePhantom;
extern const NiRTTI *	NiRTTI_bhkCapsuleShape;
extern const NiRTTI *	NiRTTI_bhkBoxShape;
extern const NiRTTI *	NiRTTI_bhkSphereShape;
extern const NiRTTI *	NiRTTI_bhkBvTreeShape;
extern const NiRTTI *	NiRTTI_bhkNiTriStripsShape;
extern const NiRTTI *	NiRTTI_bhkPackedNiTriStripsShape;
extern const NiRTTI *	NiRTTI_bhkBlendCollisionObject;
extern const NiRTTI *	NiRTTI_bhkAvoidBox;
extern const NiRTTI *	NiRTTI_bhkLimitedHingeConstraint;
extern const NiRTTI *	NiRTTI_bhkMalleableConstraint;
extern const NiRTTI *	NiRTTI_bhkConstraint;
extern const NiRTTI *	NiRTTI_bhkPrismaticConstraint;
extern const NiRTTI *	NiRTTI_bhkAction;
extern const NiRTTI *	NiRTTI_bhkUnaryAction;
extern const NiRTTI *	NiRTTI_bhkTriSampledHeightFieldBvTreeShape;
extern const NiRTTI *	NiRTTI_bhkCachingShapePhantom;
extern const NiRTTI *	NiRTTI_bhkRagdollTemplateData;
extern const NiRTTI *	NiRTTI_bhkRagdollTemplate;
extern const NiRTTI *	NiRTTI_bhkSPCollisionObject;
extern const NiRTTI *	NiRTTI_bhkMouseSpringAction;
extern const NiRTTI *	NiRTTI_bhkHingeConstraint;
extern const NiRTTI *	NiRTTI_bhkCompressedMeshShape;
extern const NiRTTI *	NiRTTI_bhkCompressedMeshShapeData;
extern const NiRTTI *	NiRTTI_bhkConvexSweepShape;
extern const NiRTTI *	NiRTTI_bhkConvexTransformShape;
extern const NiRTTI *	NiRTTI_bhkConvexTranslateShape;
extern const NiRTTI *	NiRTTI_bhkConvexVerticesShape;
extern const NiRTTI *	NiRTTI_bhkCylinderShape;
extern const NiRTTI *	NiRTTI_bhkMultiSphereShape;
extern const NiRTTI *	NiRTTI_bhkPlaneShape;
extern const NiRTTI *	NiRTTI_bhkTriangleShape;
extern const NiRTTI *	NiRTTI_bhkMoppBvTreeShape;
extern const NiRTTI *	NiRTTI_bhkTransformShape;
extern const NiRTTI *	NiRTTI_bhkListShape;
extern const NiRTTI *	NiRTTI_bhkBallAndSocketConstraint;
extern const NiRTTI *	NiRTTI_bhkBallSocketConstraintChain;
extern const NiRTTI *	NiRTTI_bhkGroupConstraint;
extern const NiRTTI *	NiRTTI_bhkHingeLimitsConstraint;
extern const NiRTTI *	NiRTTI_bhkFixedConstraint;
extern const NiRTTI *	NiRTTI_bhkRagdollConstraint;
extern const NiRTTI *	NiRTTI_bhkRagdollLimitsConstraint;
extern const NiRTTI *	NiRTTI_bhkStiffSpringConstraint;
extern const NiRTTI *	NiRTTI_bhkWheelConstraint;
extern const NiRTTI *	NiRTTI_bhkBreakableConstraint;
extern const NiRTTI *	NiRTTI_bhkAngularDashpotAction;
extern const NiRTTI *	NiRTTI_bhkDashpotAction;
extern const NiRTTI *	NiRTTI_bhkLiquidAction;
extern const NiRTTI *	NiRTTI_bhkMotorAction;
extern const NiRTTI *	NiRTTI_bhkOrientHingedBodyAction;
extern const NiRTTI *	NiRTTI_bhkSpringAction;
extern const NiRTTI *	NiRTTI_bhkBlendController;
extern const NiRTTI *	NiRTTI_bhkExtraData;
extern const NiRTTI *	NiRTTI_bhkPoseArray;
extern const NiRTTI *	NiRTTI_bhkGenericConstraint;
extern const NiRTTI *	NiRTTI_bhkCharControllerShape;
extern const NiRTTI *	NiRTTI_bhkCollisionBox;
extern const NiRTTI *	NiRTTI_bhkShapeCollection;
extern const NiRTTI *	NiRTTI_bhkPositionConstraintMotor;
extern const NiRTTI *	NiRTTI_bhkVelocityConstraintMotor;
extern const NiRTTI *	NiRTTI_bhkSpringDamperConstraintMotor;
extern const NiRTTI *	NiRTTI_bhkCharacterProxy;
extern const NiRTTI *	NiRTTI_bhkCharacterRigidBody;
extern const NiRTTI *	NiRTTI_bhkHeightFieldShape;
extern const NiRTTI *	NiRTTI_bhkConstraintChain;
extern const NiRTTI *	NiRTTI_bhkBinaryAction;
extern const NiRTTI *	NiRTTI_bhkPointToPathConstraint;
extern const NiRTTI *	NiRTTI_DebugTextExtraData;
extern const NiRTTI *	NiRTTI_BSFadeNode;
extern const NiRTTI *	NiRTTI_BSShaderProperty;
extern const NiRTTI *	NiRTTI_BSLeafAnimNode;
extern const NiRTTI *	NiRTTI_BSTreeNode;
extern const NiRTTI *	NiRTTI_ShadowSceneNode;
extern const NiRTTI *	NiRTTI_BSLightingShaderProperty;
extern const NiRTTI *	NiRTTI_BSGrassShaderProperty;
extern const NiRTTI *	NiRTTI_BSShaderAccumulator;
extern const NiRTTI *	NiRTTI_BSEffectShaderProperty;
extern const NiRTTI *	NiRTTI_BSWaterShaderProperty;
extern const NiRTTI *	NiRTTI_BSBloodSplatterShaderProperty;
extern const NiRTTI *	NiRTTI_BSParticleShaderProperty;
extern const NiRTTI *	NiRTTI_BSTextureSet;
extern const NiRTTI *	NiRTTI_BSShaderTextureSet;
extern const NiRTTI *	NiRTTI_BSSkyShaderProperty;
extern const NiRTTI *	NiRTTI_BSFadeNodeCuller;
extern const NiRTTI *	NiRTTI_BSDistantTreeShaderProperty;
extern const NiRTTI *	NiRTTI_BSCubeMapCamera;
extern const NiRTTI *	NiRTTI_BSFogProperty;
extern const NiRTTI *	NiRTTI_BSClearZNode;
extern const NiRTTI *	NiRTTI_NiCollisionObject;
extern const NiRTTI *	NiRTTI_BSOrderedNode;
extern const NiRTTI *	NiRTTI_BSLines;
extern const NiRTTI *	NiRTTI_BSDynamicLines;
extern const NiRTTI *	NiRTTI_BSMultiIndexTriShape;
extern const NiRTTI *	NiRTTI_BSLightingShaderPropertyFloatController;
extern const NiRTTI *	NiRTTI_BSLightingShaderPropertyUShortController;
extern const NiRTTI *	NiRTTI_BSLightingShaderPropertyColorController;
extern const NiRTTI *	NiRTTI_BSEffectShaderPropertyFloatController;
extern const NiRTTI *	NiRTTI_BSEffectShaderPropertyColorController;
extern const NiRTTI *	NiRTTI_BSNiAlphaPropertyTestRefController;
extern const NiRTTI *	NiRTTI_BSPSysSimpleColorModifier;
extern const NiRTTI *	NiRTTI_BSPSysLODModifier;
extern const NiRTTI *	NiRTTI_BSParabolicCullingProcess;
extern const NiRTTI *	NiRTTI_BSMeshLODTriShape;
extern const NiRTTI *	NiRTTI_BSLODMultiIndexTriShape;
extern const NiRTTI *	NiRTTI_BSSubIndexLandTriShape;
