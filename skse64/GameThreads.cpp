#include "skse64/GameThreads.h"

BSTaskPool *	BSTaskPool::GetSingleton(void)
{
	// 7D1E14E40FFCA18609B3FAAD0A5750A26162D096+90
	static RelocPtr<BSTaskPool*> g_taskPool(0x031993A0);
	return *g_taskPool;
}
