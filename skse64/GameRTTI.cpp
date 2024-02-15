#include "GameRTTI.h"
#include "skse64_common/Relocation.h"

typedef void * (* _Runtime_DynamicCast_Internal)(void * srcObj, UInt32 arg1, const void * fromType, const void * toType, UInt32 arg4);
// 09ECDC875A847989BA08B9E8A23656EDFE1F380C+A8
RelocAddr <_Runtime_DynamicCast_Internal> Runtime_DynamicCast_Internal(0x0153DB8A);	// __RTDynamicCast

void * Runtime_DynamicCast(void * srcObj, const void * fromType, const void * toType)
{
	uintptr_t fromTypeAddr = uintptr_t(fromType) + RelocationManager::s_baseAddr;
	uintptr_t toTypeAddr = uintptr_t(toType) + RelocationManager::s_baseAddr;

	return Runtime_DynamicCast_Internal(srcObj, 0, (void *)fromTypeAddr, (void *)toTypeAddr, 0);
}

#include "GameRTTI.inl"
