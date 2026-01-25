#include "skse64/GameThreads.h"

BSTaskPool *	BSTaskPool::GetSingleton(void)
{
	// B474410E5F15240DC8FB7ABE11650955666F7775+91
	static RelocPtr<BSTaskPool*> g_taskPool(0x02F38978);
	return *g_taskPool;
}
