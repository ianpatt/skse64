#pragma once

#include "skse64_common/Utilities.h"
#include "GameAPI.h"

class Actor;
class BGSHeadPart;
class NiAVObject;
class BGSTextureSet;
class NiAVObject;
class TESObjectREFR;

class TaskDelegate
{
public:
	virtual void Run() = 0;
	virtual void Dispose() = 0;
};

class BSTaskPool
{
public:
	MEMBER_FN_PREFIX(BSTaskPool);
	DEFINE_MEMBER_FN(SetNiGeometryTexture, UInt32, 0x00658F70, NiAVObject * geometry, BGSTextureSet * textureSet);

	void ProcessTasks(void);

	DEFINE_MEMBER_FN(ProcessTaskQueue_HookTarget, void, 0x00656AB0);

	static BSTaskPool *	GetSingleton(void);
};
