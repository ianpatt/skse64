#pragma once

#include "skse64_common/Utilities.h"

class PlayerCharacter;

class Main
{
public:
    virtual ~Main();

    UInt64 unk08;    // 08
    UInt64 unk10;    // 10
    UInt64 unk18;    // 18
    UInt64 unk20;    // 20
    UInt32 threadId; // 28
    // ... More
};
extern RelocPtr <Main*> g_main;

class Heap
{
public:
	MEMBER_FN_PREFIX(Heap);
	DEFINE_MEMBER_FN(Allocate, void *, 0x00CBF350, size_t size, size_t alignment, bool aligned);
	DEFINE_MEMBER_FN(Free, void, 0x00CBF7A0, void * buf, bool aligned);
};

extern RelocPtr <Heap> g_mainHeap;

void * Heap_Allocate(size_t size);
void Heap_Free(void * ptr);

class ConsoleManager
{
public:
	MEMBER_FN_PREFIX(ConsoleManager);
	DEFINE_MEMBER_FN(VPrint, void, 0x008F9390, const char * fmt, va_list args);
//	DEFINE_MEMBER_FN(Print, void, 0x001D2050, const char * str);
};

extern RelocPtr <ConsoleManager *> g_console;
extern RelocPtr <UInt32> g_consoleHandle;
extern RelocPtr <PlayerCharacter*> g_thePlayer;

void Console_Print(const char * fmt, ...);
bool IsConsoleMode(void);
__int64 GetPerfCounter(void);
