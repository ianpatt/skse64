#include "skse64/NiNodes.h"
#include "skse64/GameObjects.h"

NiBoneNames * NiBoneNames::GetSingleton(void)
{
	// 33EBA690BF2A27ACE16840951CA740822F8A4756+9A
	static RelocAddr <NiBoneNames *> addr(0x020F7770);
	return addr;
}

NiNode * NiNode::Create(UInt16 arrBufLen)
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
