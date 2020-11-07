#include "skse64/InternalTasks.h"
#include "common/IMemPool.h"

#include "common/ICriticalSection.h"
#include <queue>

#include "skse64/GameAPI.h"
#include "skse64/GameReferences.h"
#include "skse64/GameData.h"
#include "skse64/GameForms.h"
#include "skse64/GameRTTI.h"

#include "skse64/NiNodes.h"
#include "skse64/NiExtraData.h"

#include "Hooks_Threads.h"

IThreadSafeBasicMemPool<SKSETaskUpdateTintMasks, 10>		s_updateTintMasksDelegatePool;
IThreadSafeBasicMemPool<SKSETaskUpdateHairColor, 10>		s_updateHairColorDelegatePool;
IThreadSafeBasicMemPool<SKSETaskUpdateWeight, 10>		s_updateWeightDelegatePool;
IThreadSafeBasicMemPool<SKSETaskRegenHead, 10>			s_regenHeadDelegatePool;
IThreadSafeBasicMemPool<SKSETaskChangeHeadPart, 10>		s_changeHeadPartDelegatePool;
IThreadSafeBasicMemPool<SKSETaskUpdateWorldData, 10>		s_updateWorldDataDelegatePool;
IThreadSafeBasicMemPool<SKSETaskUpdateExpression, 10>	s_updateExpressionDelegatePool;
IThreadSafeBasicMemPool<SKSETaskUpdateHarvestModel, 10>	s_updateHarvestModelDelegatePool;

void TaskInterface::UpdateTintMasks()
{
	SKSETaskUpdateTintMasks * cmd = s_updateTintMasksDelegatePool.Allocate();
	if (cmd) {
		TaskInterface::AddTask(cmd);
	}
}

void TaskInterface::UpdateHairColor()
{
	SKSETaskUpdateHairColor * cmd = s_updateHairColorDelegatePool.Allocate();
	if (cmd) {
		TaskInterface::AddTask(cmd);
	}
}

void TaskInterface::RegenerateHead(Actor * actor)
{
	SKSETaskRegenHead * cmd = SKSETaskRegenHead::Create(actor);
	if (cmd) {
		TaskInterface::AddTask(cmd);
	}
}

void TaskInterface::ChangeHeadPart(Actor * actor, BGSHeadPart * oldPart, BGSHeadPart * newPart)
{
	SKSETaskChangeHeadPart * cmd = SKSETaskChangeHeadPart::Create(actor, oldPart, newPart);
	if (cmd) {
		TaskInterface::AddTask(cmd);
	}
}

void TaskInterface::UpdateWeight(Actor * actor, float delta, UInt32 updateFlags, bool redrawWeapon)
{
	SKSETaskUpdateWeight * cmd = SKSETaskUpdateWeight::Create(actor, delta, updateFlags, redrawWeapon);
	if (cmd) {
		TaskInterface::AddTask(cmd);
	}
}

void TaskInterface::UpdateWorldData(NiAVObject * object)
{
	SKSETaskUpdateWorldData * cmd = SKSETaskUpdateWorldData::Create(object);
	if (cmd) {
		TaskInterface::AddTask(cmd);
	}
}

void TaskInterface::UpdateExpression(Actor * actor, UInt8 type, UInt16 index, float value)
{
	SKSETaskUpdateExpression * cmd = SKSETaskUpdateExpression::Create(actor, type, index, value);
	if (cmd) {
		TaskInterface::AddTask(cmd);
	}
}

void TaskInterface::UpdateHarvestModel(TESObjectREFR * refr)
{
	SKSETaskUpdateHarvestModel * cmd = SKSETaskUpdateHarvestModel::Create(refr);
	if (cmd) {
		TaskInterface::AddTask(cmd);
	}
}


void SKSETaskUpdateTintMasks::Dispose(void)
{
	s_updateTintMasksDelegatePool.Free(this);
}

void SKSETaskUpdateTintMasks::Run()
{
	(*g_thePlayer)->UpdateSkinColor();
	UpdatePlayerTints();
}

void SKSETaskUpdateHairColor::Run()
{
	(*g_thePlayer)->UpdateHairColor();
}

void SKSETaskUpdateHairColor::Dispose(void)
{
	s_updateHairColorDelegatePool.Free(this);
}

SKSETaskRegenHead * SKSETaskRegenHead::Create(Actor * actor)
{
	SKSETaskRegenHead * cmd = s_regenHeadDelegatePool.Allocate();
	if (cmd)
	{
		cmd->m_actor = actor;
	}
	return cmd;
}

void SKSETaskRegenHead::Dispose(void)
{
	s_regenHeadDelegatePool.Free(this);
}

void SKSETaskRegenHead::Run()
{
	TESNPC * npc = DYNAMIC_CAST(m_actor->baseForm, TESForm, TESNPC);
	BSFaceGenNiNode * faceNode = m_actor->GetFaceGenNiNode();
	BGSHeadPart * facePart = npc->GetCurrentHeadPartByType(BGSHeadPart::kTypeFace);
	if (npc && faceNode && facePart) {
		CALL_MEMBER_FN(FaceGen::GetSingleton(), RegenerateHead)(faceNode, facePart, npc);
	}
}


SKSETaskChangeHeadPart * SKSETaskChangeHeadPart::Create(Actor * actor, BGSHeadPart* oldPart, BGSHeadPart* newPart)
{
	SKSETaskChangeHeadPart * cmd = s_changeHeadPartDelegatePool.Allocate();
	if (cmd)
	{
		cmd->m_actor = actor;
		cmd->m_newPart = newPart;
		cmd->m_oldPart = oldPart;
	}
	return cmd;
}

void SKSETaskChangeHeadPart::Dispose(void)
{
	s_changeHeadPartDelegatePool.Free(this);
}

void SKSETaskChangeHeadPart::Run()
{
	if (m_actor) {
		ChangeActorHeadPart(m_actor, m_oldPart, m_newPart);
	}
}

SKSETaskUpdateWeight * SKSETaskUpdateWeight::Create(Actor * actor, float delta, UInt32 updateFlags, bool redrawWeapon)
{
	SKSETaskUpdateWeight * cmd = s_updateWeightDelegatePool.Allocate();
	if (cmd)
	{
		cmd->m_actor = actor;
		cmd->m_delta = delta;
		cmd->m_updateFlags = updateFlags;
		cmd->m_redraw = redrawWeapon;
	}
	return cmd;
}

void SKSETaskUpdateWeight::Dispose(void)
{
	s_updateWeightDelegatePool.Free(this);
}

void SKSETaskUpdateWeight::Run()
{
	if (m_actor) {
		TESNPC * npc = DYNAMIC_CAST(m_actor->baseForm, TESForm, TESNPC);
		if (npc) {
			BSFaceGenNiNode * faceNode = m_actor->GetFaceGenNiNode();
			if (faceNode) {
				CALL_MEMBER_FN(faceNode, AdjustHeadMorph)(BSFaceGenNiNode::kAdjustType_Neck, 0, m_delta);
				UpdateModelFace(faceNode);
			}

			/*ActorWeightModel * lowModel = m_actor->GetWeightModel(ActorWeightModel::kWeightModel_Small);
			if(lowModel && lowModel->weightData)
			CALL_MEMBER_FN(lowModel->weightData, UpdateWeightData)();

			ActorWeightModel * highModel = m_actor->GetWeightModel(ActorWeightModel::kWeightModel_Large);
			if(highModel && highModel->weightData)
			CALL_MEMBER_FN(highModel->weightData, UpdateWeightData)();

			//UInt32 updateFlags = ActorEquipData::kFlags_Unk01 | ActorEquipData::kFlags_Unk02 | ActorEquipData::kFlags_Mobile;
			// Resets ActorState
			//updateFlags |= ActorEquipData::kFlags_DrawHead | ActorEquipData::kFlags_Reset;
			UInt32 updateFlags = m_updateFlags;

			CALL_MEMBER_FN(m_actor->processManager, SetEquipFlag)(updateFlags);
			CALL_MEMBER_FN(m_actor->processManager, UpdateEquipment)(m_actor);*/

			// Force redraw weapon, weight model update causes weapon position to be reset
			// Looking at DrawSheatheWeapon there is a lot of stuff going on, hard to find
			// out how to just manually move the weapon to its intended position
			if (m_redraw && m_actor->actorState.IsWeaponDrawn()) {
				m_actor->DrawSheatheWeapon(false);
				m_actor->DrawSheatheWeapon(true);
			}
		}
	}
}

SKSETaskUpdateWorldData * SKSETaskUpdateWorldData::Create(NiAVObject * object)
{
	SKSETaskUpdateWorldData * cmd = s_updateWorldDataDelegatePool.Allocate();
	if (cmd)
	{
		if (object)
			object->IncRef();
		cmd->m_object = object;
	}
	return cmd;
	return NULL;
}

void SKSETaskUpdateWorldData::Dispose(void)
{
	if (m_object)
		m_object->DecRef();
	s_updateWorldDataDelegatePool.Free(this);
}

void SKSETaskUpdateWorldData::Run()
{
	NiAVObject::ControllerUpdateContext ctx;
	m_object->UpdateWorldData(&ctx);
}

SKSETaskUpdateHarvestModel * SKSETaskUpdateHarvestModel::Create(TESObjectREFR * reference)
{
	SKSETaskUpdateHarvestModel * cmd = s_updateHarvestModelDelegatePool.Allocate();
	if (cmd)
	{
		cmd->m_reference = reference;
	}
	return cmd;
}

void SKSETaskUpdateHarvestModel::Dispose(void)
{
	s_updateHarvestModelDelegatePool.Free(this);
}

void SKSETaskUpdateHarvestModel::Run()
{
	if (m_reference) {
		NiNode * rootModel = m_reference->GetNiNode();
		if (rootModel)
			UpdateHarvestModel(m_reference, rootModel);
	}
}

SKSETaskUpdateExpression * SKSETaskUpdateExpression::Create(Actor * actor, UInt8 type, UInt16 index, float value)
{
	SKSETaskUpdateExpression * cmd = s_updateExpressionDelegatePool.Allocate();
	if (cmd)
	{
		cmd->m_actor = actor;
		cmd->m_type = type;
		cmd->m_index = index;
		if (value < 0.0f)
			value = 0.0f;
		if (value > 1.0f)
			value = 1.0f;
		cmd->m_value = value;
	}
	return cmd;
}

void SKSETaskUpdateExpression::Dispose(void)
{
	s_updateExpressionDelegatePool.Free(this);
}

void SKSETaskUpdateExpression::Run()
{
	BSFaceGenAnimationData * animationData = m_actor->GetFaceGenAnimationData();
	if (animationData) {
		if (m_type != BSFaceGenAnimationData::kKeyframeType_Reset) {
			FaceGen::GetSingleton()->isReset = 0;
			BSFaceGenKeyframeMultiple * keyframe = &animationData->keyFrames[m_type];
			if (m_index >= keyframe->count)
				return;
			keyframe->values[m_index] = m_value;
			keyframe->isUpdated = 0;
		}
		else {
			animationData->overrideFlag = 0;
			CALL_MEMBER_FN(animationData, Reset)(1.0, 1, 1, 0, 0);
			FaceGen::GetSingleton()->isReset = 1;
		}
		NiNode * face = m_actor->GetFaceGenNiNode();
		if (face)
			UpdateModelFace(face);
	}
}
