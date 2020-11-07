#pragma once

#include "skse64_common/Utilities.h"

class PlayerCharacter;

class Heap
{
public:
	MEMBER_FN_PREFIX(Heap);
	DEFINE_MEMBER_FN(Allocate, void *, 0x00C02260, size_t size, size_t alignment, bool aligned);
	DEFINE_MEMBER_FN(Free, void, 0x00C02560, void * buf, bool aligned);
};

extern RelocPtr <Heap> g_mainHeap;

void * Heap_Allocate(size_t size);
void Heap_Free(void * ptr);

class ConsoleManager
{
public:
	MEMBER_FN_PREFIX(ConsoleManager);
	DEFINE_MEMBER_FN(VPrint, void, 0x0085C2C0, const char * fmt, va_list args);
//	DEFINE_MEMBER_FN(Print, void, 0x001D2050, const char * str);
};

extern RelocPtr <ConsoleManager *> g_console;
extern RelocPtr <UInt32> g_consoleHandle;
extern RelocPtr <PlayerCharacter*> g_thePlayer;

void Console_Print(const char * fmt, ...);
bool IsConsoleMode(void);
__int64 GetPerfCounter(void);
