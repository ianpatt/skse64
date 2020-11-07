#include "skse64/NiNodes.h"
#include "skse64/GameObjects.h"

const _GetBoneNames GetBoneNames = (_GetBoneNames)0x00000000; // Builds the skeleton node name list if it doesn't exist
const _GetNodeByName GetNodeByName = (_GetNodeByName)0x00000000;
const _GetNodeNameByWeaponType GetNodeNameByWeaponType = (_GetNodeNameByWeaponType)0x00000000;
const _GetInternalNode GetInternalNode = (_GetInternalNode)0x00000000;
const _SwapNodeParent SwapNodeParent = (_SwapNodeParent)0x00000000;

NiBoneNames * NiBoneNames::GetSingleton(void)
{
	return (NiBoneNames *)0x00000000;
}

NiWeaponNodes * NiWeaponNodes::GetSingleton(void)
{
	return (NiWeaponNodes *)0x00000000;
}


NiNode * NiNode::Create(UInt32 arrBufLen)
{
	void* memory = Heap_Allocate(sizeof(NiNode));
	memset(memory, 0, sizeof(NiNode));
	NiNode* xData = (NiNode*)memory;
	xData->ctor(arrBufLen);
	return xData;
}

BSFadeNode * BSFadeNode::Create()
{
	void* memory = Heap_Allocate(sizeof(BSFadeNode));
	memset(memory, 0, sizeof(BSFadeNode));
	BSFadeNode* xData = (BSFadeNode*)memory;
	xData->ctor();
	return xData;
}
