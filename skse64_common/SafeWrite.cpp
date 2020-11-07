#include "SafeWrite.h"

void SafeWriteBuf(uintptr_t addr, void * data, size_t len)
{
	UInt32	oldProtect;

	VirtualProtect((void *)addr, len, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy((void *)addr, data, len);
	VirtualProtect((void *)addr, len, oldProtect, &oldProtect);
}

void SafeWrite8(uintptr_t addr, UInt8 data)
{
	SafeWriteBuf(addr, &data, sizeof(data));
}

void SafeWrite16(uintptr_t addr, UInt16 data)
{
	SafeWriteBuf(addr, &data, sizeof(data));
}

void SafeWrite32(uintptr_t addr, UInt32 data)
{
	SafeWriteBuf(addr, &data, sizeof(data));
}

void SafeWrite64(uintptr_t addr, UInt64 data)
{
	SafeWriteBuf(addr, &data, sizeof(data));
}

static bool SafeWriteJump_Internal(uintptr_t src, uintptr_t dst, UInt8 op)
{
#pragma pack(push, 1)
	struct Code
	{
		UInt8	op;
		SInt32	displ;
	};
#pragma pack(pop)

	STATIC_ASSERT(sizeof(Code) == 5);

	ptrdiff_t delta = dst - (src + sizeof(Code));
	if((delta < INT_MIN) || (delta > INT_MAX))
		return false;

	Code code;

	code.op = op;
	code.displ = delta;

	SafeWriteBuf(src, &code, sizeof(code));

	return true;
}

bool SafeWriteJump(uintptr_t src, uintptr_t dst)
{
	return SafeWriteJump_Internal(src, dst, 0xE9);
}

bool SafeWriteCall(uintptr_t src, uintptr_t dst)
{
	return SafeWriteJump_Internal(src, dst, 0xE8);
}
