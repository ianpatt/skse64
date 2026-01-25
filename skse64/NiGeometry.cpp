#include "skse64/NiGeometry.h"
#include "skse64/GameAPI.h"

RelocAddr<_CreateBSTriShape> CreateBSTriShape(0x00C67490);
RelocAddr<_CreateBSDynamicTriShape> CreateBSDynamicTriShape(0x00C72180);

// ??_7NiTriShape@@6B@
static const RelocPtr<uintptr_t> s_NiTriShapeVtbl(0x0176D060);

// ??_7NiTriStrips@@6B@
static const RelocPtr<uintptr_t> s_NiTriStripsVtbl(0x0176D5A0);

void NiGeometryData::AllocateVerts(UInt32 numVerts)
{
	m_pkVertex = (NiPoint3 *)Heap_Allocate(sizeof(NiPoint3) * numVerts);
	m_pkTexture = (NiPoint2 *)Heap_Allocate(sizeof(NiPoint2) * numVerts);

	memset(m_pkVertex, 0, sizeof(NiPoint3) * numVerts);
	memset(m_pkTexture, 0, sizeof(NiPoint2) * numVerts);
}

void NiGeometryData::AllocateNormals(UInt32 numVerts)
{
	m_pkNormal = (NiPoint3 *)Heap_Allocate(sizeof(NiPoint3) * numVerts);
	memset(m_pkNormal, 0, sizeof(NiPoint3) * numVerts);
}

void NiGeometryData::AllocateNBT(UInt32 numVerts)
{
	m_pkNormal = (NiPoint3 *)Heap_Allocate(sizeof(NiPoint3) * 3 * numVerts);
	memset(m_pkNormal, 0, sizeof(NiPoint3) * 3 * numVerts);
}

void NiGeometryData::AllocateColors(UInt32 numVerts)
{
	m_pkColor = (NiColorA *)Heap_Allocate(sizeof(NiColorA) * numVerts);
	memset(m_pkColor, 0, sizeof(NiColorA) * numVerts);
}

void NiSkinPartition::Partition::AllocateWeights(UInt32 numVerts)
{
	m_pfWeights = (float *)Heap_Allocate(sizeof(float) * 4 * numVerts);
	m_pucBonePalette = (UInt8 *)Heap_Allocate(sizeof(UInt8) * 4 * numVerts);

	memset(m_pfWeights, 0, sizeof(float) * 4 * numVerts);
	memset(m_pucBonePalette, 0, sizeof(UInt8) * 4 * numVerts);
}

void NiSkinData::BoneData::AllocateWeights(UInt32 numWeights)
{
	m_pkBoneVertData = (BoneVertData *)Heap_Allocate(sizeof(BoneVertData) * numWeights);
	memset(m_pkBoneVertData, 0, sizeof(BoneVertData) * numWeights);
}

NiSkinInstance * NiSkinInstance::Create()
{
	void* memory = Heap_Allocate(sizeof(NiSkinInstance));
	memset(memory, 0, sizeof(NiSkinInstance));
	NiSkinInstance* xData = (NiSkinInstance*)memory;
	CALL_MEMBER_FN(xData, ctor)();
	return xData;
}

BSDismemberSkinInstance * BSDismemberSkinInstance::Create()
{
	void* memory = Heap_Allocate(sizeof(BSDismemberSkinInstance));
	memset(memory, 0, sizeof(BSDismemberSkinInstance));
	BSDismemberSkinInstance* xData = (BSDismemberSkinInstance*)memory;
	CALL_MEMBER_FN(xData, ctor)();
	return xData;
}

NiTriShape * NiTriShape::Create(NiTriShapeData * geometry)
{
	void* memory = Heap_Allocate(sizeof(NiTriShape));
	memset(memory, 0, sizeof(NiTriShape));
	NiTriShape* xData = (NiTriShape*)memory;
	xData->ctor(geometry);
	((uintptr_t *)memory)[0] = s_NiTriShapeVtbl.GetUIntPtr();
	return xData;
}


NiTriStrips * NiTriStrips::Create(NiTriShapeData * geometry)
{
	void* memory = Heap_Allocate(sizeof(NiTriStrips));
	memset(memory, 0, sizeof(NiTriStrips));
	NiTriStrips* xData = (NiTriStrips*)memory;
	xData->ctor(geometry);
	((uintptr_t *)memory)[0] = s_NiTriStripsVtbl.GetUIntPtr();
	return xData;
}

NiSkinInstance * NiSkinInstance::Clone()
{
	NiSkinInstance * newSkinInstance = nullptr;

	EnterCriticalSection(&lock);
	BSDismemberSkinInstance* srcSkin = ni_cast(this, BSDismemberSkinInstance);
	if (srcSkin)
	{
		newSkinInstance = BSDismemberSkinInstance::Create();
		BSDismemberSkinInstance* dstSkin = ni_cast(newSkinInstance, BSDismemberSkinInstance);
		dstSkin->numPartitions = srcSkin->numPartitions;
		dstSkin->partitionFlags = (UInt32 *)Heap_Allocate(sizeof(UInt32) * srcSkin->numPartitions);
		memcpy(dstSkin->partitionFlags, srcSkin->partitionFlags, sizeof(UInt32) * srcSkin->numPartitions);
		dstSkin->unk98 = srcSkin->unk98;
		memcpy(dstSkin->pad99, srcSkin->pad99, 7);
	}
	else
	{
		newSkinInstance = NiSkinInstance::Create();
	}

	newSkinInstance->m_spSkinData = m_spSkinData;
	newSkinInstance->m_spSkinPartition = m_spSkinPartition;
	newSkinInstance->m_pkRootParent = m_pkRootParent;
	newSkinInstance->m_ppkBones = (NiAVObject**)Heap_Allocate(sizeof(NiAVObject*) * m_uiBoneNodes);
	memcpy(newSkinInstance->m_ppkBones, this->m_ppkBones, sizeof(NiAVObject*) * m_uiBoneNodes);
	newSkinInstance->unk38 = unk38;
	newSkinInstance->m_uiBoneNodes = m_uiBoneNodes;
	newSkinInstance->numFlags = numFlags;
	if (flags) {
		newSkinInstance->flags = (UInt32 *)Heap_Allocate(sizeof(UInt32) * numFlags);
	}
	else {
		flags = nullptr;
	}
	newSkinInstance->unk44 = unk44;
	memcpy(newSkinInstance->flags, flags, sizeof(UInt32) * numFlags);
	if (unk50) {
		newSkinInstance->unk50 = (UInt32 *)Heap_Allocate(sizeof(UInt32) * numFlags);
		memcpy(newSkinInstance->unk50, unk50, sizeof(UInt32) * numFlags);
	}
	else {
		unk50 = nullptr;
	}

	NiSkinData * skinData = niptr_cast<NiSkinData>(newSkinInstance->m_spSkinData);
	if (skinData) {
		newSkinInstance->m_worldTransforms = (NiTransform**)Heap_Allocate(sizeof(NiTransform*) * skinData->m_uiBones);
		memcpy(newSkinInstance->m_worldTransforms, this->m_worldTransforms, sizeof(NiTransform*) * skinData->m_uiBones);
	}

#if 0
	else
	{
		if (!newSkinInstance->flags && numFlags > 0)
		{
			newSkinInstance->numFlags = numFlags;
			newSkinInstance->flags = (UInt32 *)Heap_Allocate(sizeof(UInt32) * numFlags);
			memcpy(newSkinInstance->flags, flags, sizeof(UInt32) * numFlags);
		}

		newSkinInstance->m_uiBoneNodes = m_uiBoneNodes;
		newSkinInstance->unk38 = unk38;
		newSkinInstance->unk44 = unk44;
		newSkinInstance->unk50 = unk50;
		newSkinInstance->unk54 = unk54;
	}
#endif
	LeaveCriticalSection(&lock);
	return newSkinInstance;
}
